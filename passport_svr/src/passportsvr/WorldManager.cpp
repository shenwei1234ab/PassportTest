/*
 * WorldManager.cpp
 *
 *  Created on: 2015年6月13日
 *      Author: leave
 */

#include "../passportsvr/WorldManager.h"

#include "database/PassPortDatabaseConnection.h"
#include "../passportsvr/manager/PlayerManager.h"
#include "vnVariableScript.h"
#include "vnBase64.h"

extern PassPortDatabaseWorkerPool PassPortDatabase;

using namespace _vn_ns;

bool WorldManager::loadFromDB(){

	m_MaxppIdMap[PLATFORM_ID_VISITOR]=1;
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_SEL_MAXPPID);
	PreparedQueryResult aResult = PassPortDatabase.SynQuery(stmt);
	if (aResult && aResult->GetRowCount()) {
		do{
			VN_LOG_DEBUG("platformid : " << (*aResult)[0].GetUInt32()<< "  Max ppID : "<< ((*aResult)[1].GetUInt64()& 0xFFFFF));
			//m_MaxppIdMap.insert(std::make_pair((*aResult)[0].GetUInt32(),(*aResult)[1].GetUInt64() & 0xFFFFF));
			m_MaxppIdMap[(*aResult)[0].GetUInt32()] = (*aResult)[1].GetUInt64() & 0xFFFFF;
		} while((*aResult).NextRow());
		//VN_LOG_DEBUG("load PPID info OK! ");
	}
	for(int i=0; i< PLATFORM_ID_COUNT; i++){
		auto itr = m_MaxppIdMap.find(i);
			if (itr == m_MaxppIdMap.end()){
				m_MaxppIdMap[i] = 1;
			}
		}

	stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_SEL_MAXORDER);
	aResult = PassPortDatabase.SynQuery(stmt);
	if (aResult && aResult->GetRowCount()) {
		m_maxOrderId = (*aResult)[0].GetUInt64();
	}

	stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_SEL_MAXPLAYERID);
	aResult = PassPortDatabase.SynQuery(stmt);
	if (aResult && aResult->GetRowCount()) {
		m_maxPlayerId = (*aResult)[0].GetUInt64();
	}
	VN_LOG_DEBUG("loadFromDB OK! ");
	return true;
}

u64 WorldManager::createPPId(u32 tpId) {
	auto itr = m_MaxppIdMap.find(tpId);
	if (itr == m_MaxppIdMap.end()){
		//平台ID 错误
		return -1;
	}
	u64 uMaxppId = itr->second;
	itr->second = ++uMaxppId;
	return uMaxppId + ((u64)tpId << 40);
}
/*str8 WorldManager::createTmpToken( std::shared_ptr<std::array<vn::u8,64>> sessionKey,std::string const& name) {
	HmacSha256 proof(SESSIONKEY_LENGTH, sessionKey->data());
	proof.UpdateData(name);
	proof.Finalize();

	return ByteArrayToHexStr(proof.GetDigest(), proof.GetLength(), true);
	return "";
}
*/
/*vn::u32 WorldManager::createToken() {
	vn::u32 ret = 0;
	PlayerHandlerPtr pPlayerHandler = nullptr;
	do {
		ret = tokenIndex++;
		if (ret == 0) {
			continue;
		}
		pPlayerHandler = sPlayerManager.GetPlayerBySocket(ret);
	}while(pPlayerHandler);
	return ret;
}*/

u64 WorldManager::createOrderId(){
	return ++m_maxOrderId;
}
u64 WorldManager::createPlayerId(){
	return ++m_maxPlayerId;
}

u32 WorldManager::getPlatformId(u64 ppid){
	return ppid >> 40;
}
bool WorldManager::loadTemplet(){
	VariableScript vs;
	RefCountedPtr<Variable_object> m_config;
	m_config = vs.parse({0, "product.var"});
	vn::VariableAccessor<Variable_object> product(m_config->queryAttributeObject("iapProducts"));
	while (product.fetch()) {
	    auto item = product.at<0>();
	    std::shared_ptr<ProductTemplet> pProduct (new ProductTemplet);
	    pProduct->productId = item->queryAttributeInt32("productId");
	    pProduct->productName = item->queryAttributeString("productName");
	    pProduct->money = item->queryAttributeInt32("money");
	    pProduct->price = item->queryAttributeInt32("price");
		 pProduct->limitBuyCount = item->queryAttributeInt32("limitBuyCount");
		 pProduct->extraValue = item->queryAttributeInt32("extraValue");
	    m_IOSProductTempletMap[pProduct->productId] = pProduct;
	}

	vn::VariableAccessor < Variable_object > productAndroid(m_config->queryAttributeObject("androidProducts"));
	while (productAndroid.fetch()) {
		auto item = productAndroid.at<0>();
		std::shared_ptr<ProductTemplet> pProduct(new ProductTemplet);
		pProduct->productId = item->queryAttributeInt32("productId");
		pProduct->productName = item->queryAttributeString("productName");
		pProduct->money = item->queryAttributeInt32("money");
		pProduct->price = item->queryAttributeInt32("price");
		pProduct->limitBuyCount = item->queryAttributeInt32("limitBuyCount");
		pProduct->extraValue = item->queryAttributeInt32("extraValue");
		m_AndroidProductTempletMap[pProduct->productId] = pProduct;
	}

	return true;
}
bool WorldManager::loadBaseConfig() {
    VariableScript vs;
    RefCountedPtr<Variable_object> m_config;
    m_config = vs.parse({0, "config.var"});
    if (!m_config) {
        VN_LOG_FATAL("can NOT load config!");
        return false;
    }

    m_thread_count = (size_t)m_config->queryAttributeInt32("thread_count", (int)m_thread_count);
    m_client_port = (u16)m_config->queryAttributeInt32("client_port", (int)m_client_port);
    m_gamesvr_port = (u16)m_config->queryAttributeInt32("gamesvr_port", (int)m_gamesvr_port);
    m_admin_port = (u16)m_config->queryAttributeInt32("admin_port", (int)m_admin_port);
    m_console_mode = m_config->queryAttributeBool("console_mode", m_console_mode);
    m_http_port = (u16)m_config->queryAttributeInt32("http_port", (int)m_http_port);
    m_httpVerifyPort= (u16)m_config->queryAttributeInt32("http_verify_port", (int)m_httpVerifyPort);

    VariableAccessor<Variable_int32, Variable_bool, Variable_number> va(m_config->queryAttributeObject("return_money"));
    while (va.fetch()) {
    	auto	pfid = va.at<0>()->value();
    	auto isReturn = va.at<1>()->value();
    	auto persent = va.at<2>()->getFloat();
    	m_moneyReturnInfos.insert({pfid, WorldManager::MoneyReturn(persent,isReturn)});

    	WorldManager::MoneyReturn& tmp = m_moneyReturnInfos.at(pfid);
    	VN_LOG_DEBUG("pfid="<<pfid<<" isBeginReturn="<<tmp.isBeginReturn<<" persent="<<tmp.persent);
    }

    m_loadBalanceNum = m_config->queryAttributeInt32("loadBalanceNum", 1);
    auto iPdefault = m_config->queryAttributeString("loadBalanceNumDefaultIp");
    for(int i = 0; i < m_loadBalanceNum; ++i) {
    	BalanceNetAddress address;
    	char buf[256];
    	sprintf(buf, "loadBalanceIp%d", i);
    	address.ip = m_config->queryAttributeString(buf, iPdefault);
    	sprintf(buf, "loadBalancePort%d", i);
    	address.port = m_config->queryAttributeInt32(buf, 8802);
    	VN_LOG_DEBUG("loadBalance:[" << i << "]"<< "  " << address.ip  << ":" << address.port);
    	m_loadBalance.push_back(std::move(address));
     }

    //m_gamesvrIP = m_config->queryAttributeString("link_gamesvr_ip0", "115.159.101.74");
    //m_gamesvrPort = m_config->queryAttributeInt32("link_gamesvr_port0", 8802);

    m_sDBInfo = m_config->queryAttributeString("psssport_db_info", "");
	if (m_sDBInfo == "") {
		VN_LOG_FATAL("database not specified in configuration file !");
		return false;
	}
	m_dbThreads = m_config->queryAttributeInt32("psssport_db_threads", 1);


	//******亲加即时通讯******
	GOTYE_UserName = m_config->queryAttributeString("GOTYE_UserName", "");
	GOTYE_Password = m_config->queryAttributeString("GOTYE_Password", "");
	GOTYE_Max_Friend_Num = m_config->queryAttributeString("GOTYE_Max_Friend_Num", "");
	GOTYE_Appkey = m_config->queryAttributeString("GOTYE_Appkey", "");



	//******腾讯沙箱环境******
	QQ_APPKEY_PAY = m_config->queryAttributeString("QQ_APPKEY_PAY", "");
	QQ_HOST = m_config->queryAttributeString("QQ_HOST", "");

	//******苹果支付验证地址******
	Apple_Verify_URL = m_config->queryAttributeString("Apple_Verify_URL", "");
    return true;
}
/*
bool WorldManager::loadDiamondInfo(u64 playerId){
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement( PASSPORT_SEL_DIAMOND_BY_PLYERID);
	stmt->setUInt64(0, playerId);
	PreparedQueryResult pResult = PassPortDatabase.SynQuery(stmt);
	if (pResult && pResult->GetRowCount()) {
		std::map<u32, DiamondPtr> aDiamondMap;
		for (vn::u64 i = 0; i < pResult->GetRowCount(); i++) {
			DiamondPtr pDiamond(new Diamond, true);
			pDiamond->playerid = (*pResult)[0].GetUInt64();
			pDiamond->serverid = (*pResult)[1].GetUInt32();
			pDiamond->diamond = (*pResult)[2].GetUInt32();
			aDiamondMap.insert(std::make_pair(pDiamond->serverid, pDiamond));
			pResult->NextRow();
		}
		m_diamondMapLock.lock();
		m_pDiamondMapMap[playerId] = std::move(aDiamondMap);
		m_diamondMapLock.unlock();
	}
	return true;
}
std::map<u32, DiamondPtr>& WorldManager::getDiamondMap(u64 playerId){
	 std::lock_guard<std::mutex> lock(m_diamondMapLock);
	if (m_pDiamondMapMap.find(playerId) == m_pDiamondMapMap.end()) {
		loadDiamondInfo(playerId);
	}
	return m_pDiamondMapMap[playerId];
}
int WorldManager::insertDiamond(u64 playerId,u32 serverid,DiamondPtr pDiamond){
	if (m_pDiamondMapMap.find(playerId) == m_pDiamondMapMap.end()) {
		loadDiamondInfo(playerId);
	}
	std::map<u32, DiamondPtr>& pDiamondMap =  m_pDiamondMapMap[playerId];
	pDiamondMap[serverid] = pDiamond;
	return 0;
}
DiamondPtr WorldManager::getDiamond(u64 playerId,u32 serverid){
	m_diamondMapLock.lock();
	bool isLoad = false;
	if (m_pDiamondMapMap.find(playerId) == m_pDiamondMapMap.end()) {
		isLoad = true;
	}
	m_diamondMapLock.unlock();
	if (isLoad) {
		loadDiamondInfo(playerId);
	}
	std::map<u32, DiamondPtr>&	rDiamondMap = m_pDiamondMapMap[playerId];
	return rDiamondMap[serverid];
}
std::mutex& WorldManager::getDiamondLock(u64 playerId,u32 serverid){
	std::map<u32,std::mutex>& diamondLockMap =  m_diamondLockMapMap[playerId];
	return diamondLockMap[serverid];
}


*/
/*
bool WorldManager::loadPlayerInfo(u64 playerId) {
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(
			PASSPORT_SEL_BY_KEY);
	stmt->setUInt64(0, playerId);
	PreparedQueryResult pResult = PassPortDatabase.SynQuery(stmt);
	if (pResult && pResult->GetRowCount()) {
		for (vn::u64 i = 0; i < pResult->GetRowCount(); i++) {
			PlayerPtr pPlayer(new Player, true);
			pPlayer->playerid = (*pResult)[0].GetUInt64();
			pPlayer->ppid = (*pResult)[1].GetUInt64();
			pPlayer->gameid = (*pResult)[2].GetUInt32();
			pPlayer->zoneid = (*pResult)[3].GetUInt32();
			pPlayer->diamond = (*pResult)[4].GetUInt32();
			pPlayer->gen_balance = (*pResult)[5].GetUInt32();
			pPlayer->first_save = (*pResult)[6].GetUInt32();
			pPlayer->save_amt = (*pResult)[7].GetUInt32();
			m_playerMapLock.lock();
			m_playerMapMap.insert(std::make_pair(pPlayer->playerid, pPlayer));
			m_playerMapLock.unlock();
			pResult->NextRow();
		}
	} else {
		VN_LOG_ERROR("loadPlayerInfo  error! can't find playerid "<<playerId);
		return false;
	}
	return true;
}
int WorldManager::insertPlayer(PlayerPtr pPlayer) {
	m_playerMapLock.lock();
	m_playerMapMap.insert(std::make_pair(pPlayer->playerid, pPlayer));
	m_playerMapLock.unlock();
	return 0;
}*/
/*
PlayerPtr WorldManager::getPlayer(u64 playerId) {
	if(m_playerMapMap.find(playerId) == m_playerMapMap.end()){
		if(!loadPlayerInfo(playerId)){
			return nullptr;
		}
	}
	return m_playerMapMap[playerId];
}
std::mutex& WorldManager::getPlayerLock(u64 playerId) {
	return m_playerLockMapMap[playerId];
}
*/
u64 WorldManager::getPlayerId(str8 _ppid,str8 _gameid,str8 _zoneid){
	std::vector<u8> ppidVtr = Base64::decode(_ppid);
	u64 ppid = 0;
	if (ppidVtr.size() == sizeof(u64)) {
		memcpy(&ppid, ppidVtr.data(), sizeof(ppid));
	}
	u32 gameid;
	sscanf(_gameid.c_str(),"%u",&gameid);
	u32 zoneid;
	sscanf(_zoneid.c_str(),"%u",&zoneid);
	return getPlayerId(ppid,1,zoneid);
}

u64 WorldManager::getPlayerId(u64 ppid,u32 gameid,u32 zoneid){
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(
			PASSPORT_SEL_PPID2);
	stmt->setUInt64(0, ppid);
	stmt->setUInt32(1, gameid);
	stmt->setUInt32(2, zoneid);
	PreparedQueryResult aResult = PassPortDatabase.SynQuery(stmt);
	if (aResult && aResult->GetRowCount()) {
		return (*aResult)[0].GetUInt64();
	}
	return 0;
}

u64 WorldManager::GetPPId(int pfid,std::string openId,int os){
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement( PASSPORT_SEL_PPID);
	stmt->setUInt32(0, pfid);
	stmt->setString(1, openId);
	stmt->setUInt32(2, os);
	PreparedQueryResult aResult = PassPortDatabase.SynQuery(stmt);
	if (aResult && aResult->GetRowCount()) {
		return (*aResult)[0].GetUInt64();
	}
	return 0;
}
AccountPtr WorldManager::GetAccountInfo(u64 ppid) {
	AccountPtr ret;
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement( PASSPORT_SEL_ACCOUNT_BY_KEY);
	stmt->setUInt64(0, ppid);
	PreparedQueryResult aResult = PassPortDatabase.SynQuery(stmt);
	if (aResult && aResult->GetRowCount()) {
		ret = new Account;
		ret->ppid = (*aResult)[0].GetUInt64();
		ret->platformid = (*aResult)[1].GetInt32();
		ret->openid = (*aResult)[2].GetString();
		ret->accesstoken = (*aResult)[3].GetString();
		ret->os = (*aResult)[4].GetInt32();
		ret->nickname = (*aResult)[5].GetString();
	}
	return ret;
}

//将token插入数据库
void WorldManager::updateDBToken(vn::u64 ppid,u32 pptoken){
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement( PASSPORT_UPD_PPTOKEN);
	stmt->setUInt64(0, pptoken);
	stmt->setUInt64(1, m_tokenLifeTime + time(NULL));
	stmt->setUInt64(2, ppid);
	PassPortDatabase.Execute(stmt,nullptr,nullptr);

}

vn::u32 WorldManager::createToken() {
	//返回一个随机值
	m_tokenIndex++;
	if (m_tokenIndex == 0) {
		m_tokenIndex++;
	}
	return m_tokenIndex;
}
/*
int WorldManager::GetProductTemplet(GAME_ID gameid,OS_ID osId,std::map<vn::u32, std::shared_ptr<ProductTemplet>>*&  productMap){
	switch (gameid) {
	case GAME_ID_ICE_FIRE:
		switch (osId) {
		case OS_ID_IOS:
			productMap = &m_IOSProductTempletMap;
			break;
		case OS_ID_ANDROID:
			productMap = &m_AndroidProductTempletMap;
			break;
		default:
			return -1;
		}
		break;
	case GAME_ID_Hat:
		return -1;
		break;
	case GAME_ID_GIRL:
		return -1;
		break;
	case GAME_ID_GUN:
		switch (osId) {
		case OS_ID_IOS:
			productMap = &m_IOSProductTempletMap_Gun;
			break;
		case OS_ID_ANDROID:
			productMap = &m_AndroidProductTempletMap_Gun;
			break;
		default:
			return -1;
		}
		break;
	default:
		return -1;
	}
	return 0;
}*/
