/*
 * GamesvrSession.cpp
 *
 *  Created on: 2015年6月11日
 *      Author: leave
 */

#include "../../passportsvr/gamesvrlisten/GamesvrSession.h"
#include "../../passportsvr/manager/OrderManger.h"
#include "../../passportsvr/manager/PlayerManager.h"
#include "../../passportsvr/Protocols.h"
#include "../../passportsvr/WorldManager.h"
#include "vnLog.h"
#include "../database/PassPortDatabaseConnection.h"
#include "vnBase64.h"
#include "curl/CurlManager.h"
#include "vnVariableJSON.h"
#include "common/cryptography/HmacHash.h"

extern PassPortDatabaseWorkerPool PassPortDatabase;


const char* QQ_IP = "140.207.54.51:80";
//返回应该返还的游戏币
void doRetrunMoney(u64 ppid,u32& diamond,u32& gen_balance,u32& first_save,u32& save_amt) {

	u32 platformId = sWorldManager.getPlatformId(ppid);
	//判断平台是否需要充值返还
	try {
		WorldManager::MoneyReturn& config = sWorldManager.m_moneyReturnInfos.at(platformId);
		if(!config.isBeginReturn) {
			return ;
		}

		//获取玩家账户信息
		PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(HISTORY_PAY_SEL_BY_PPID);
		stmt->setUInt64(0, ppid);
		PreparedQueryResult aResult = PassPortDatabase.SynQuery(stmt);
		if (aResult && aResult->GetRowCount()) {
			do {
				u64 playerid = (*aResult)[0].GetUInt64();
				u32 money = (*aResult)[1].GetUInt32();
				u32 retMoney = (*aResult)[2].GetUInt32();
				if (retMoney > 0) {
					continue; //已经返还过了
				}
				retMoney = config.persent * money;
				save_amt+= money;
				gen_balance = retMoney- money;
				diamond += retMoney;
				stmt = PassPortDatabase.GetPreparedStatement(HISTORY_PAY_UPD_BY_PLAYER);
				stmt->setUInt32(0, retMoney);
				stmt->setUInt64(1, playerid);
				PassPortDatabase.Execute(stmt, nullptr, nullptr);
			}while (aResult->NextRow());
		}
	} catch (...) {
		return ;
	}
	return ;
}



/****************************************************/
/************* dealsvr *****************/
/****************************************************/
//------------获取余额-----
VN_IMPL_PACKET_REQUEST2(GamesvrSession, pkt, req_GetBalance) {
	/*
	pkt::res_JsonDate res;
	res.data.handle = pkt->data.handle;
	res.data.result = 0;
	char buffer[512];
	memset(buffer, 0, 512);
	sprintf(buffer, "{\n \"ret\":1018, \"balance\":%u, \"gen_balance\":%u, \"first_save\":%u, \"save_amt\":%u \n}",
			99999, 0, 1, 99999 );
	res.data.jsonvalue = buffer;
	VN_LOG_DEBUG("req_GetBalance: pkt->data.openid = "<<pkt->data.openid); VN_LOG_DEBUG("req_GetBalance: \n"<<res.data.jsonvalue);
	this->sendPacket(res);
	*/

	std::vector<u8> ppidVtr = Base64::decode(pkt->data.openid);
	u64 ppid = 0;
	if (ppidVtr.size() == sizeof(u64)) {
		memcpy(&ppid, ppidVtr.data(), sizeof(ppid));
	}
	auto platId = sWorldManager.getPlatformId(ppid);

	if(platId == PLATFORM_ID_QQ) {
		GetTencentBalance(ppid, pkt,PLATFORM_ID_QQ);
	} else if(platId == PLATFORM_ID_WX) {
		GetTencentBalance(ppid, pkt,PLATFORM_ID_WX);
	} else {
		doPlayerGetBalance(ppid, *pkt);
	}
}
void GamesvrSession::_createPlayer(PlayerPtr& pPlayer, u64 ppid, u64& playerid, u32 gameid, u32 zoneid) {
	//gameid = 1; //todo del
	if (gameid != 2) { //草帽
		gameid = 1;
	}
	// 找不到该角色的钻石信息
	pPlayer = new Player;
	playerid = sWorldManager.createPlayerId();
	pPlayer->playerid = playerid;
	pPlayer->ppid = ppid;
	pPlayer->gameid = gameid;
	pPlayer->zoneid = zoneid;
	pPlayer->diamond = 0;
	pPlayer->gen_balance = 0;
	pPlayer->first_save = 0;
	pPlayer->save_amt = 0;
	//充值测试的返还
	doRetrunMoney(ppid, pPlayer->diamond, pPlayer->gen_balance, pPlayer->first_save, pPlayer->save_amt);
	sPlayerManager.SetPlayerPtr(pPlayer);
	//创建新玩家
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_INS_PLAYER);
	stmt->setUInt64(0, pPlayer->playerid);
	stmt->setUInt64(1, pPlayer->ppid);
	stmt->setUInt32(2, pPlayer->gameid);
	stmt->setUInt32(3, pPlayer->zoneid);
	stmt->setUInt32(4, pPlayer->diamond);
	stmt->setUInt32(5, pPlayer->gen_balance);
	stmt->setUInt32(6, pPlayer->first_save);
	stmt->setUInt32(7, pPlayer->save_amt);
	PassPortDatabase.Execute(stmt, nullptr, nullptr);
}
void GamesvrSession::doPlayerGetBalance(u64 ppid, pkt::req_GetBalance& pkt, bool isTencent, u32 diamond , u32 gen_balance , u32 first_save , u32 save_amt,std::string JsonStr) {
	PlayerPtr pPlayer;
	u32 gameid;
	sscanf(pkt.data.appid.c_str(), "%u", &gameid);

	u32 zoneid;
	sscanf(pkt.data.zoneid.c_str(), "%u", &zoneid);
	u64 playerid = sWorldManager.getPlayerId(ppid,1,zoneid);
	if (playerid == 0) {
		_createPlayer(pPlayer,ppid,playerid,1,zoneid);
	} else {
		pPlayer = sPlayerManager.GetPlayerPtr(playerid);
	}
	if (pPlayer.get() == nullptr) {
		pkt::res_JsonDate res;
		res.data.handle = pkt.data.handle;
		res.data.result = 0;
		res.data.jsonvalue = "{\n \"ret\":-1 \n}";
		this->sendPacket(res);
		VN_LOG_ERROR("can't find player "<<playerid);
		return;
	}
	if(isTencent && (pPlayer->diamond != diamond
			||pPlayer->gen_balance != gen_balance
			||pPlayer->first_save != first_save
			||pPlayer->save_amt != save_amt
			)){
		pPlayer->diamond = diamond;
		pPlayer->gen_balance = gen_balance;
		pPlayer->first_save = first_save;
		pPlayer->save_amt = save_amt;
		//更新数据库
		PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement( PASSPORT_UPD_DIAMOND);
		stmt->setUInt32(0, pPlayer->diamond);
		stmt->setUInt32(1, pPlayer->gen_balance);
		stmt->setUInt32(2, pPlayer->first_save);
		stmt->setUInt32(3, pPlayer->save_amt);
		stmt->setUInt64(4, pPlayer->playerid);
		PassPortDatabase.Execute(stmt, nullptr, nullptr);
	}
	pkt::res_JsonDate res;
	res.data.handle = pkt.data.handle;
	res.data.result = 0;
	if(!JsonStr.empty()){
		res.data.jsonvalue = JsonStr;
	} else {
		std::string tss_list ="[";
		std::map<std::string,PlayerSubscribePtr> subscribeMap = sOrderManger.GetPlayerSubscribeInfo(pPlayer->playerid);
		for(auto pair : subscribeMap){
			PlayerSubscribePtr pSubscribe = pair.second;
			if(!pSubscribe){
				continue;
			}
			Variable_object vobj;
			vobj.setAttribute("inner_productid", vnnew Variable_string(pSubscribe->inner_productid));
			vobj.setAttribute("begintime", vnnew Variable_string(ChangeTimeToStr(pSubscribe->begintime,"%Y-%m-%d %H:%M:%S")));
			vobj.setAttribute("endtime", vnnew Variable_string(ChangeTimeToStr(pSubscribe->endtime,"%Y-%m-%d %H:%M:%S")));
			vobj.setAttribute("grandtotal_opendays", vnnew Variable_int32(pSubscribe->grandtotal_opendays));

			tss_list += VariableJSON::generate(&vobj);
			tss_list += ",";
		}
		if(tss_list.length() > 1){
			tss_list[tss_list.length() - 1] = ']';
		} else {
			tss_list.push_back( ']');
		}

		char buffer[512];
		memset(buffer, 0, 512);
		sprintf(buffer, "{\n \"ret\":0, \"balance\":%u, \"gen_balance\":%u, \"first_save\":%u, \"save_amt\":%u, \"tss_list\":%s \n}", pPlayer->diamond, pPlayer->gen_balance, pPlayer->first_save, pPlayer->save_amt,tss_list.c_str());
		res.data.jsonvalue = buffer;
		VN_LOG_DEBUG("req_GetBalance: pkt->data.openid = "<<pkt.data.openid);
	}
	VN_LOG_DEBUG("req_GetBalance: \n"<<res.data.jsonvalue);
	this->sendPacket(res);


}
//------------赠送钻石-----
VN_IMPL_PACKET_REQUEST2(GamesvrSession, pkt, req_Present) {
/*	pkt::res_JsonDate res;
	res.data.handle = pkt->data.handle;
	res.data.result = 0;
	res.data.jsonvalue = "{\n \"ret\":0 \n}";
	this->sendPacket(res);*/

	std::vector<u8> ppidVtr = Base64::decode(pkt->data.openid);
	u64 ppid = 0;
	if (ppidVtr.size() == sizeof(u64)) {
		memcpy(&ppid, ppidVtr.data(), sizeof(ppid));
	}
	auto platId = sWorldManager.getPlatformId(ppid);

	if (platId == PLATFORM_ID_QQ) {
		TencentPresent(ppid, pkt,PLATFORM_ID_QQ);
	} else if (platId == PLATFORM_ID_WX) {
		TencentPresent(ppid, pkt,PLATFORM_ID_WX);
	} else {
		PlayerPtr pPlayer;
		u64 playerid = sWorldManager.getPlayerId(pkt->data.openid,pkt->data.appid,pkt->data.zoneid);
		if (playerid == 0) {
			u32 zoneid;
			sscanf(pkt->data.zoneid.c_str(), "%u", &zoneid);
			_createPlayer(pPlayer, ppid, playerid, 1, zoneid);
		} else {
			pPlayer = sPlayerManager.GetPlayerPtr(playerid);
		}
		char buffer[512];
		if(pPlayer.get() != nullptr) {
			int value = atoi(pkt->data.presenttimes.c_str());
			if(value>0) {
				pPlayer->diamond+=value;
				pPlayer->gen_balance+=value;
				PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement( PASSPORT_UPD_DIAMOND);
				stmt->setUInt32(0, pPlayer->diamond);
				stmt->setUInt32(1, pPlayer->gen_balance);
				stmt->setUInt32(2, pPlayer->first_save);
				stmt->setUInt32(3, pPlayer->save_amt);
				stmt->setUInt64(4, pPlayer->playerid);
				PassPortDatabase.Execute(stmt, nullptr, nullptr);
			}
			sprintf( buffer,"{\n \"ret\":0, \"balance\":%d \n}",pPlayer->diamond);
		} else {
			sprintf( buffer,"{\n \"ret\":0, \"balance\":0 \n}");
		}
		pkt::res_JsonDate res;
		res.data.handle = pkt->data.handle;
		res.data.result = 0;
//		res.data.jsonvalue = "{\n \"ret\":0 \n}";
//		char buffer[512];
//		sprintf( buffer,"{\n \"ret\":0, \"balance\":%d \n}",pPlayer->diamond);
		res.data.jsonvalue = buffer;
		this->sendPacket(res);
	}
}

//------------支付-----
VN_IMPL_PACKET_REQUEST2(GamesvrSession, pkt, req_Pay) {
	std::vector<u8> ppidVtr = Base64::decode(pkt->data.openid);
	u64 ppid = 0;
	if (ppidVtr.size() == sizeof(u64)) {
		memcpy(&ppid, ppidVtr.data(), sizeof(ppid));
	}
	auto platId = sWorldManager.getPlatformId(ppid);

	if (platId == PLATFORM_ID_QQ) {

		TencentPay(ppid, pkt, PLATFORM_ID_QQ);
	} else if (platId == PLATFORM_ID_WX) {
		std::string billno = "";
		TencentPay(ppid, pkt, PLATFORM_ID_WX);
	} else {
		PlayerPtr pPlayer;
		u64 playerid = sWorldManager.getPlayerId(pkt->data.openid,pkt->data.appid,pkt->data.zoneid);
		if (playerid == 0) {
			u32 zoneid;
			sscanf(pkt->data.zoneid.c_str(), "%u", &zoneid);
			_createPlayer(pPlayer, ppid, playerid, 1, zoneid);
		} else {
			pPlayer = sPlayerManager.GetPlayerPtr(playerid);
		}
		if(pPlayer.get() != nullptr) {
			u32 amt = atoi(pkt->data.amt.c_str());
			if(pPlayer->diamond >= amt) {
				pPlayer->diamond -= amt;
				PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement( PASSPORT_UPD_DIAMOND);
				stmt->setUInt32(0, pPlayer->diamond);
				stmt->setUInt32(1, pPlayer->gen_balance);
				stmt->setUInt32(2, pPlayer->first_save);
				stmt->setUInt32(3, pPlayer->save_amt);
				stmt->setUInt64(4, pPlayer->playerid);
				PassPortDatabase.Execute(stmt, nullptr, nullptr);
				//支付ok
				pkt::res_JsonDate res;
				res.data.handle = pkt->data.handle;
				res.data.result = 0;
				char buffer[512];
				sprintf( buffer,"{\n \"ret\":0, \"balance\":%d \n}",pPlayer->diamond);
				res.data.jsonvalue = buffer;
				this->sendPacket(res);
			}
		} else {
			pkt::res_JsonDate res;
			res.data.handle = pkt->data.handle;
			res.data.result = 0;
			char buffer[512];
			sprintf(buffer, "{\n \"ret\":0, \"balance\":%d, \"billno\":\"123\" \n}", 0);
			res.data.jsonvalue = buffer;
			this->sendPacket(res);
		}
	}
}
//------------取消支付-----
VN_IMPL_PACKET_REQUEST2(GamesvrSession, pkt, req_CancelPay) {
    pkt::res_JsonDate res;
    res.data.handle = pkt->data.handle;
    res.data.result = 0;
    res.data.jsonvalue = "{\n \"ret\":0 \n}";
    this->sendPacket(res);
}
//------------账目保存-----
VN_IMPL_PACKET_REQUEST2(GamesvrSession, pkt, req_AcctSave) {
    pkt::res_JsonDate res;
    res.data.handle = pkt->data.handle;
    res.data.result = 0;
    res.data.jsonvalue = "{\n \"ret\":0 \n}";
    this->sendPacket(res);
}
//------------查询资格-----
VN_IMPL_PACKET_REQUEST2(GamesvrSession, pkt, req_QueryQualify) {
    pkt::res_JsonDate res;
    res.data.handle = pkt->data.handle;
    res.data.result = 0;
    res.data.jsonvalue = "{\n \"ret\":0 \n}";
    this->sendPacket(res);
}
//------------购买货物-----
VN_IMPL_PACKET_REQUEST2(GamesvrSession, pkt, req_BuyGoods) {
    pkt::res_JsonDate res;
    res.data.handle = pkt->data.handle;
    res.data.result = 0;
    res.data.jsonvalue = "{\n \"ret\":0 \n}";
    this->sendPacket(res);
}
//------------订阅M-----
VN_IMPL_PACKET_REQUEST2(GamesvrSession, pkt, req_SubscribeM) {
	std::vector<u8> ppidVtr = Base64::decode(pkt->data.openid);
	u64 ppid = 0;
	if (ppidVtr.size() == sizeof(u64)) {
		memcpy(&ppid, ppidVtr.data(), sizeof(ppid));
	}
	auto platId = sWorldManager.getPlatformId(ppid);

	if (platId == PLATFORM_ID_QQ) {
		TencentSubscribeM(ppid, pkt, PLATFORM_ID_QQ);
	} else if (platId == PLATFORM_ID_WX) {
		std::string billno = "";
		TencentSubscribeM(ppid, pkt, PLATFORM_ID_WX);
	} else {
		u64 playerid = sWorldManager.getPlayerId(pkt->data.openid,pkt->data.appid,pkt->data.zoneid);
		int ret = -1;
		if(playerid > 0) {
			ret = sOrderManger.Subscribe(playerid,pkt->data.tss_inner_product_id,atoi(pkt->data.buy_quantity.c_str()));
		}

		pkt::res_JsonDate res;
		res.data.handle = pkt->data.handle;
		res.data.result = 0;
		res.data.jsonvalue = "{\n \"ret\":";
		res.data.jsonvalue += std::to_string(ret);
		res.data.jsonvalue += " \n}";
		this->sendPacket(res);
	}
}
std::string Get_QQ_APPKEY_PAY(){
	if(!sWorldManager.QQ_APPKEY_PAY.empty()){
		return sWorldManager.QQ_APPKEY_PAY;
	}
	return QQ_APPKEY_PAY;
}
std::string Get_QQ_HOST(){
	if(!sWorldManager.QQ_HOST.empty()){
		//const char* Get_QQ_HOST() = "msdktest.qq.com";
		return sWorldManager.QQ_HOST;
	}
	return "msdk.qq.com";
}
void GamesvrSession::GetTencentBalance(u64 ppid, pkt::req_GetBalance* pkt,u32 platid){
	//get openid
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_SEL_ACCOUNT_BY_KEY);
	stmt->setUInt64(0, ppid);
	PreparedQueryResult result = PassPortDatabase.SynQuery(stmt);
	if (!(result && result->GetRowCount())) {
		return;
	}
	std::string tencentApi = "/mpay/get_balance_m";

	std::string appid =QQ_APPID;
	std::string appkey =Get_QQ_APPKEY_PAY();
	std::string pay_token = pkt->data.pay_token;

	std::string cookie = "session_id=";
	if(platid == PLATFORM_ID_QQ){
		cookie += "openid;session_type=kp_actoken";
	} else {
		//WX
		cookie += "hy_gameid;session_type=wc_actoken";
		pay_token = "";
//		appid =WX_APPID;
//		appkey =WX_APPKEY;
	}

	cookie += ";org_loc=";
	cookie += UrlEncode(tencentApi);
	cookie += ";";

	std::map<std::string, std::string> paramMap;
	paramMap["openid"] = (*result)[2].GetString();
	paramMap["openkey"] = pkt->data.openkey;
	paramMap["pay_token"] = pay_token;
	paramMap["appid"] = appid;
	paramMap["ts"] = std::to_string(time(NULL));
	paramMap["pf"] = pkt->data.pf;
	paramMap["pfkey"] = pkt->data.pfkey;
	paramMap["zoneid"] = pkt->data.zoneid;
	std::string secret = appkey + "&";
	std::string content = SetURLParameter(paramMap);
	VN_LOG_DEBUG("sig pam:\n"<<content );
	std::string sigContent = "GET&" + UrlEncode(tencentApi);
	sigContent += "&";
	sigContent += UrlEncode(content);

	HmacSha1 hmac((uint32) secret.length(), (uint8 const*) secret.c_str());
	hmac.UpdateData(sigContent);
	hmac.Finalize();
	std::string signNew = Base64::encode(hmac.GetDigest(), hmac.GetLength());

	content += "&sig=";
	content += UrlEncode(signNew);
	//HTTP GET
	HttpRequestPtr aReqPtr(new HttpRequestC);
	aReqPtr->addr.from_string(QQ_IP);
	aReqPtr->host = Get_QQ_HOST();
	aReqPtr->isPost = false;
	aReqPtr->cookie += cookie;
	aReqPtr->content = tencentApi + "?";
	aReqPtr->content += content;
	aReqPtr->isHttps = false;
	aReqPtr->lineBreak = "\r\n";
	aReqPtr->headMap.insert( { "Content-Type", "application/x-www-form-urlencoded" });
	RefCountedPtr<pkt::req_GetBalance> pp(pkt, true);
	RefCountedPtr<GamesvrSession> session(this, true);
	//aReqPtr->callBackFn = [session,pp, ppid](const HttpResponse& _httpResponse) {
	auto cbfn = [this, ppid, pp](const HttpResponse& _httpResponse) {
		pkt::res_JsonDate res;
		res.data.handle = pp->data.handle;
		res.data.result = 0;
		res.data.jsonvalue = "{\n \"ret\":-1 \n}";

		BufferPtr pBuffer = _httpResponse.content();
		if (!pBuffer) {
			this->sendPacket(res);
			return;
		}
		VN_LOG_DEBUG( (char*)pBuffer->lock());
		VariableJSON m_variableJSON;
		auto vJSONPtr = m_variableJSON.parse((char*) pBuffer->lock(), pBuffer->size());
		if(!vJSONPtr) {
			this->sendPacket(res);
			return;
		}
		auto ret = vJSONPtr->queryAttributeInt32("ret");
		if(ret == 0) {
			auto balance = vJSONPtr->queryAttributeInt32("balance");
			auto gen_balance = vJSONPtr->queryAttributeInt32("gen_balance");
			auto first_save = vJSONPtr->queryAttributeInt32("first_save");
			auto save_amt = vJSONPtr->queryAttributeInt32("save_amt");
			this->doPlayerGetBalance(ppid, *pp,true,balance,gen_balance,first_save,save_amt,(char*)pBuffer->lock());
			return;
		} else {
			this->sendPacket(res);
			return;
		}
	};
	aReqPtr->callBackFn = [session, cbfn](const HttpResponse &_httpResponse) {
		session->post(std::bind(cbfn, _httpResponse));
	};
	sCurlManager.CurlSend(aReqPtr);
}

void GamesvrSession::TencentPresent(u64 ppid, pkt::req_Present* pkt,u32 platid){
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_SEL_ACCOUNT_BY_KEY);
	stmt->setUInt64(0, ppid);
	PreparedQueryResult result = PassPortDatabase.SynQuery(stmt);
	if (!(result && result->GetRowCount())) {
		return;
	}
	std::string tencentApi = "/mpay/present_m";

	std::string appid = QQ_APPID;
	std::string appkey = Get_QQ_APPKEY_PAY();
	std::string pay_token = pkt->data.pay_token;

	std::string cookie = "session_id=";
	if (platid == PLATFORM_ID_QQ) {
		cookie += "openid;session_type=kp_actoken";
	} else {
		//WX
		cookie += "hy_gameid;session_type=wc_actoken";
		pay_token = "";
//		appid = WX_APPID;
//		appkey = WX_APPKEY;
	}

	cookie += ";org_loc=";
	cookie += UrlEncode(tencentApi);
	cookie += ";";

	std::map<std::string, std::string> paramMap;
	paramMap["openid"] = (*result)[2].GetString();
	paramMap["openkey"] = pkt->data.openkey;
	paramMap["pay_token"] = pay_token;
	paramMap["appid"] = appid;
	paramMap["ts"] = std::to_string(time(NULL));
	paramMap["pf"] = pkt->data.pf;
	paramMap["pfkey"] = pkt->data.pfkey;
	paramMap["zoneid"] = pkt->data.zoneid;
	paramMap["discountid"] = pkt->data.discountid;
	paramMap["giftid"] = pkt->data.giftid;
	paramMap["presenttimes"] = pkt->data.presenttimes;
	paramMap["giftid"] = pkt->data.giftid;
	std::string secret = appkey + "&";
	std::string content = SetURLParameter(paramMap);
	VN_LOG_DEBUG("sig pam:\n"<<content );
	std::string sigContent = "GET&" + UrlEncode(tencentApi);
	sigContent += "&";
	sigContent += UrlEncode(content);

	HmacSha1 hmac((uint32) secret.length(), (uint8 const*) secret.c_str());
	hmac.UpdateData(sigContent);
	hmac.Finalize();
	std::string signNew = Base64::encode(hmac.GetDigest(), hmac.GetLength());

	content += "&sig=";
	content += UrlEncode(signNew);
	//HTTP GET
	HttpRequestPtr aReqPtr(new HttpRequestC);
	aReqPtr->addr.from_string(QQ_IP);
	aReqPtr->host = Get_QQ_HOST();
	aReqPtr->isPost = false;
	aReqPtr->cookie += cookie;
	aReqPtr->content = tencentApi + "?";
	aReqPtr->content += content;
	aReqPtr->isHttps = false;
	aReqPtr->lineBreak = "\r\n";
	aReqPtr->headMap.insert( { "Content-Type", "application/x-www-form-urlencoded" });
	RefCountedPtr<pkt::req_Present> pp(pkt, true);
	RefCountedPtr<GamesvrSession> session(this, true);
	auto cbfn = [this, ppid, pp](const HttpResponse& _httpResponse) {
		pkt::res_JsonDate res;
		res.data.handle = pp->data.handle;
		res.data.result = 0;
		res.data.jsonvalue = "{\n \"ret\":-1 \n}";

		BufferPtr pBuffer = _httpResponse.content();
		if (!pBuffer) {
			this->sendPacket(res);
			return;
		}
		VN_LOG_DEBUG( (char*)pBuffer->lock());
		VariableJSON m_variableJSON;
		auto vJSONPtr = m_variableJSON.parse((char*) pBuffer->lock(), pBuffer->size());
		if(!vJSONPtr) {
			this->sendPacket(res);
			return;
		}
		auto ret = vJSONPtr->queryAttributeInt32("ret");
		if(ret == 0) {
			res.data.jsonvalue = "{\n \"ret\":0 \n}";
			this->sendPacket(res);
			return;
		} else {
			this->sendPacket(res);
			return;
		}
	};
	aReqPtr->callBackFn = [session, cbfn](const HttpResponse &_httpResponse) {
		session->post(std::bind(cbfn, _httpResponse));
	};
	sCurlManager.CurlSend(aReqPtr);
}
void GamesvrSession::TencentPay(u64 ppid, pkt::req_Pay* pkt,u32 platid){
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_SEL_ACCOUNT_BY_KEY);
	stmt->setUInt64(0, ppid);
	PreparedQueryResult result = PassPortDatabase.SynQuery(stmt);
	if (!(result && result->GetRowCount())) {
		return;
	}
	std::string tencentApi = "/mpay/pay_m";

	std::string appid = QQ_APPID;
	std::string appkey = Get_QQ_APPKEY_PAY();
	std::string pay_token = pkt->data.pay_token;

	std::string cookie = "session_id=";
	if (platid == PLATFORM_ID_QQ) {
		cookie += "openid;session_type=kp_actoken";
	} else {
		//WX
		cookie += "hy_gameid;session_type=wc_actoken";
		pay_token = "";
//		appid = WX_APPID;
//		appkey = WX_APPKEY;
	}

	cookie += ";org_loc=";
	cookie += UrlEncode(tencentApi);
	cookie += ";";

	std::map<std::string, std::string> paramMap;
	paramMap["openid"] = (*result)[2].GetString();
	paramMap["openkey"] = pkt->data.openkey;
	paramMap["pay_token"] = pay_token;
	paramMap["appid"] = appid;
	paramMap["ts"] = std::to_string(time(NULL));
	paramMap["pf"] = pkt->data.pf;
	paramMap["pfkey"] = pkt->data.pfkey;
	paramMap["zoneid"] = pkt->data.zoneid;
	paramMap["amt"] = pkt->data.amt;
	std::string secret = appkey + "&";
	std::string content = SetURLParameter(paramMap);
	VN_LOG_DEBUG("sig pam:\n"<<content );
	std::string sigContent = "GET&" + UrlEncode(tencentApi);
	sigContent += "&";
	sigContent += UrlEncode(content);

	HmacSha1 hmac((uint32) secret.length(), (uint8 const*) secret.c_str());
	hmac.UpdateData(sigContent);
	hmac.Finalize();
	std::string signNew = Base64::encode(hmac.GetDigest(), hmac.GetLength());

	content += "&sig=";
	content += UrlEncode(signNew);
	//HTTP GET
	HttpRequestPtr aReqPtr(new HttpRequestC);
	aReqPtr->addr.from_string(QQ_IP);
	aReqPtr->host = Get_QQ_HOST();
	aReqPtr->isPost = false;
	aReqPtr->cookie += cookie;
	aReqPtr->content = tencentApi + "?";
	aReqPtr->content += content;
	aReqPtr->isHttps = false;
	aReqPtr->lineBreak = "\r\n";
	aReqPtr->headMap.insert( { "Content-Type", "application/x-www-form-urlencoded" });
	RefCountedPtr<pkt::req_Pay> pp(pkt, true);
	RefCountedPtr<GamesvrSession> session(this, true);
	//aReqPtr->callBackFn = [session,pp, ppid](const HttpResponse& _httpResponse) {
	auto cbfn = [this, ppid, pp](const HttpResponse& _httpResponse) {
		pkt::res_JsonDate res;
		res.data.handle = pp->data.handle;
		res.data.result = 0;
		res.data.jsonvalue = "{\n \"ret\":-1 \n}";

		BufferPtr pBuffer = _httpResponse.content();
		if (!pBuffer) {
			this->sendPacket(res);
			return;
		}
		//VN_LOG_DEBUG( (char*)pBuffer->lock());
		res.data.jsonvalue =(char*)pBuffer->lock();
		this->sendPacket(res);
	};
	aReqPtr->callBackFn = [session, cbfn](const HttpResponse &_httpResponse) {
		session->post(std::bind(cbfn, _httpResponse));
	};
	sCurlManager.CurlSend(aReqPtr);
}
void GamesvrSession::TencentCancelPay(u64 ppid, pkt::req_Pay* pkt,u32 platid,std::string billno){
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_SEL_ACCOUNT_BY_KEY);
	stmt->setUInt64(0, ppid);
	PreparedQueryResult result = PassPortDatabase.SynQuery(stmt);
	if (!(result && result->GetRowCount())) {
		return;
	}
	std::string tencentApi = "/mpay/cancel_pay_m";

	std::string appid = QQ_APPID;
	std::string appkey = Get_QQ_APPKEY_PAY();
	std::string pay_token = pkt->data.pay_token;

	std::string cookie = "session_id=";
	if (platid == PLATFORM_ID_QQ) {
		cookie += "openid;session_type=kp_actoken";
	} else {
		//WX
		cookie += "hy_gameid;session_type=wc_actoken";
		pay_token = "";
//		appid = WX_APPID;
//		appkey = WX_APPKEY;
	}

	cookie += ";org_loc=";
	cookie += UrlEncode(tencentApi);
	cookie += ";";

	std::map<std::string, std::string> paramMap;
	paramMap["openid"] = (*result)[2].GetString();
	paramMap["openkey"] = pkt->data.openkey;
	paramMap["pay_token"] = pay_token;
	paramMap["appid"] = appid;
	paramMap["ts"] = std::to_string(time(NULL));
	paramMap["pf"] = pkt->data.pf;
	paramMap["pfkey"] = pkt->data.pfkey;
	paramMap["zoneid"] = pkt->data.zoneid;
	paramMap["amt"] = pkt->data.amt;
	paramMap["billno"] = billno;
	std::string secret = appkey + "&";
	std::string content = SetURLParameter(paramMap);
	VN_LOG_DEBUG("sig pam:\n"<<content );
	std::string sigContent = "GET&" + UrlEncode(tencentApi);
	sigContent += "&";
	sigContent += UrlEncode(content);

	HmacSha1 hmac((uint32) secret.length(), (uint8 const*) secret.c_str());
	hmac.UpdateData(sigContent);
	hmac.Finalize();
	std::string signNew = Base64::encode(hmac.GetDigest(), hmac.GetLength());

	content += "&sig=";
	content += UrlEncode(signNew);
	//HTTP GET
	HttpRequestPtr aReqPtr(new HttpRequestC);
	aReqPtr->addr.from_string(QQ_IP);
	aReqPtr->host = Get_QQ_HOST();
	aReqPtr->isPost = false;
	aReqPtr->cookie += cookie;
	aReqPtr->content = tencentApi + "?";
	aReqPtr->content += content;
	aReqPtr->isHttps = false;
	aReqPtr->lineBreak = "\r\n";
	aReqPtr->headMap.insert( { "Content-Type", "application/x-www-form-urlencoded" });
	RefCountedPtr<pkt::req_Pay> pp(pkt, true);
	RefCountedPtr<GamesvrSession> session(this, true);
	//aReqPtr->callBackFn = [session,pp, ppid](const HttpResponse& _httpResponse) {
	auto cbfn = [this, ppid, pp](const HttpResponse& _httpResponse) {
		pkt::res_JsonDate res;
		res.data.handle = pp->data.handle;
		res.data.result = 0;
		res.data.jsonvalue = "{\n \"ret\":-1 \n}";

		BufferPtr pBuffer = _httpResponse.content();
		if (!pBuffer) {
			this->sendPacket(res);
			return;
		}
		VN_LOG_DEBUG( (char*)pBuffer->lock());
		VariableJSON m_variableJSON;
		auto vJSONPtr = m_variableJSON.parse((char*) pBuffer->lock(), pBuffer->size());
		if(!vJSONPtr) {
			this->sendPacket(res);
			return;
		}
		auto ret = vJSONPtr->queryAttributeInt32("ret");
		if(ret == 0) {
			res.data.jsonvalue = "{\n \"ret\":0 \n}";
			this->sendPacket(res);
			return;
		} else {
			this->sendPacket(res);
			return;
		}
	};
	aReqPtr->callBackFn = [session, cbfn](const HttpResponse &_httpResponse) {
		session->post(std::bind(cbfn, _httpResponse));
	};
	sCurlManager.CurlSend(aReqPtr);
}
void GamesvrSession::TencentSubscribeM(u64 ppid, pkt::req_SubscribeM* pkt,u32 platid){
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_SEL_ACCOUNT_BY_KEY);
	stmt->setUInt64(0, ppid);
	PreparedQueryResult result = PassPortDatabase.SynQuery(stmt);
	if (!(result && result->GetRowCount())) {
		return;
	}
	std::string tencentApi = "/mpay/subscribe_m";

	std::string appid = QQ_APPID;
	std::string appkey = Get_QQ_APPKEY_PAY();
	std::string pay_token = pkt->data.pay_token;

	/*std::string cookie = "session_id=";
	if (platid == PLATFORM_ID_QQ) {
		cookie += "openid;session_type=kp_actoken";
	} else {
		//WX
		cookie += "hy_gameid;session_type=wc_actoken";
		pay_token = "";
	}*/

	std::string cookie = "org_loc=";
	cookie += UrlEncode(tencentApi);
	cookie += ";";

	std::map<std::string, std::string> paramMap;
	paramMap["openid"] = (*result)[2].GetString();

	//paramMap["pay_token"] = pay_token;
	paramMap["appid"] = appid;
	paramMap["ts"] = std::to_string(time(NULL));
	paramMap["pf"] = pkt->data.pf;
	paramMap["pfkey"] = pkt->data.pfkey;
	paramMap["zoneid"] = pkt->data.zoneid;
	paramMap["cmd"] = pkt->data.cmd;
	paramMap["tss_inner_product_id"] = pkt->data.tss_inner_product_id;
	paramMap["buy_quantity"] = pkt->data.buy_quantity;
	if (platid == PLATFORM_ID_QQ) {
		paramMap["session_id"] = "openid";
		paramMap["session_type"] = "kp_actoken";
		paramMap["openkey"] = pay_token;
	} else {
		paramMap["session_id"] = "hy_gameid";
		paramMap["session_type"] = "wc_actoken";
		paramMap["openkey"] = pkt->data.openkey;
	}
	std::string secret = appkey + "&";
	std::string content = SetURLParameter(paramMap);
	VN_LOG_DEBUG("sig pam:\n"<<content );
	std::string sigContent = "GET&" + UrlEncode(tencentApi);
	sigContent += "&";
	sigContent += UrlEncode(content);

	HmacSha1 hmac((uint32) secret.length(), (uint8 const*) secret.c_str());
	hmac.UpdateData(sigContent);
	hmac.Finalize();
	std::string signNew = Base64::encode(hmac.GetDigest(), hmac.GetLength());

	content += "&sig=";
	content += UrlEncode(signNew);
	//HTTP GET
	HttpRequestPtr aReqPtr(new HttpRequestC);
	aReqPtr->addr.from_string(QQ_IP);
	aReqPtr->host = Get_QQ_HOST();
	aReqPtr->isPost = false;
	aReqPtr->cookie += cookie;
	aReqPtr->content = tencentApi + "?";
	aReqPtr->content += content;
	aReqPtr->isHttps = false;
	aReqPtr->lineBreak = "\r\n";
	aReqPtr->headMap.insert( { "Content-Type", "application/x-www-form-urlencoded" });
	RefCountedPtr<pkt::req_SubscribeM> pp(pkt, true);
	RefCountedPtr<GamesvrSession> session(this, true);
	//aReqPtr->callBackFn = [session,pp, ppid](const HttpResponse& _httpResponse) {
	auto cbfn = [this, ppid, pp](const HttpResponse& _httpResponse) {
		pkt::res_JsonDate res;
		res.data.handle = pp->data.handle;
		res.data.result = 0;
		res.data.jsonvalue = "{\n \"ret\":-1 \n}";

		BufferPtr pBuffer = _httpResponse.content();
		if (!pBuffer) {
			this->sendPacket(res);
			return;
		}
		//VN_LOG_DEBUG( (char*)pBuffer->lock());
		res.data.jsonvalue =(char*)pBuffer->lock();
		this->sendPacket(res);
	};
	aReqPtr->callBackFn = [session, cbfn](const HttpResponse &_httpResponse) {
		session->post(std::bind(cbfn, _httpResponse));
	};
	sCurlManager.CurlSend(aReqPtr);
}
