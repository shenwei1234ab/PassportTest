/*
 * OrderManger.cpp
 *
 *  Created on: 2015年8月17日
 *      Author: leave
 */

#include "../../passportsvr/manager/OrderManger.h"

#include "../../passportsvr/manager/PlayerManager.h"
#include "../../passportsvr/manager/SessionManager.h"
#include "../../passportsvr/WorldManager.h"
#include "db/QueryResult.h"
#include "../database/PassPortDatabaseConnection.h"
extern PassPortDatabaseWorkerPool PassPortDatabase;

using namespace _vn_ns;
/*
bool OrderManger::loadOrderInfo(u64 ppId) {
	std::map<u64,std::map<u64,OrderPtr>>::iterator itrMap = m_pPPOrderMap.find(ppId);
	if(itrMap == m_pPPOrderMap.end()){
		m_pPPOrderMap.insert(std::make_pair(ppId,std::map<u64,OrderPtr>()));
	} else {
		return true;
	}
	std::map<u64,OrderPtr>& rOrderMap = m_pPPOrderMap[ppId];
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_SEL_ORDER_PPID);
	stmt->setUInt64(0, ppId);
	PreparedQueryResult pResult = PassPortDatabase.SynQuery(stmt);
	if (pResult && pResult->GetRowCount()) {
		//for (vn::u64 i = 0; i < pResult->GetRowCount(); i++)
		do{
			OrderPtr pOrder(new Order,true);
			pOrder->orderid = (*pResult)[0].GetUInt64();
			pOrder->ppid = (*pResult)[1].GetUInt64();
			pOrder->playerid = (*pResult)[2].GetUInt64();
			pOrder->gameid = (*pResult)[3].GetUInt32();
			pOrder->zoneid = (*pResult)[4].GetUInt32();
			pOrder->productid = (*pResult)[5].GetUInt32();
			pOrder->platformid = (*pResult)[6].GetUInt32();
			pOrder->status = (*pResult)[7].GetUInt32();
			pOrder->receipt = (*pResult)[8].GetString();
			//m_pOrderMap.insert(std::make_pair(pOrder->orderid,pOrder));
			rOrderMap.insert(std::make_pair(pOrder->orderid,pOrder));
		} while (pResult->NextRow());
	}
	return true;
}*/
/*
std::map<u64,OrderPtr>& OrderManger::GetOrderMap(u64 ppId){
	std::map<u64, std::map<u64, OrderPtr>>::iterator itrMap = m_pPPOrderMap.find(ppId);
	if (itrMap == m_pPPOrderMap.end()) {
		loadOrderInfo(ppId);
	}
	return m_pPPOrderMap[ppId];
}*/
OrderPtr OrderManger::GetOrderInfo(u64 orderId){
	OrderPtr pOrder;
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_SEL_ORDER_ORDERID);
	stmt->setUInt64(0, orderId);
	PreparedQueryResult pResult = PassPortDatabase.SynQuery(stmt);
	if (pResult && pResult->GetRowCount()) {
		pOrder = new Order;
		pOrder->orderid = (*pResult)[0].GetUInt64();
		pOrder->ppid = (*pResult)[1].GetUInt64();
		pOrder->playerid = (*pResult)[2].GetUInt64();
		pOrder->gameid = (*pResult)[3].GetUInt32();
		pOrder->zoneid = (*pResult)[4].GetUInt32();
		pOrder->productid = (*pResult)[5].GetUInt32();
		pOrder->platformid = (*pResult)[6].GetUInt32();
		pOrder->status = (*pResult)[7].GetUInt32();
		pOrder->receipt = (*pResult)[8].GetString();
	}
		return pOrder;
}
/*

int OrderManger::DeliverGoods(u64 orderId,std::string& receipt,int os,bool isNotice){
	OrderPtr pOrder = GetOrderInfo(orderId);
	if(!pOrder.get()){
		//找不到该订单
		return -1;
	}
	std::map<u64,OrderPtr>& m_pOrderMap = GetOrderMap(pOrder->ppid);
	m_pOrderMap[orderId] = pOrder;

	//验证订单状态是否正常
	if(pOrder->status == ORDER_STATUS_CONFIRM){
		if (isNotice) {
			//向客户端发送发货成功的消息
			pkt::res_OrderFinished res;
			res.data.orderId = pOrder->orderid;
			res.data.errorCode = 0;
			sPlayerManager.sendMsgToPlayer(pOrder->playerid,res);
		}
		return 0;//游戏服已经发货成功
	}

	std::shared_ptr<ProductTemplet> pProductTmpl;
	if (os == OS_ID_ANDROID) {
		pProductTmpl = sWorldManager.m_AndroidProductTempletMap[pOrder->productid ];
	} else if (os == OS_ID_IOS) {
		pProductTmpl = sWorldManager.m_IOSProductTempletMap[pOrder->productid ];
	}
	if (pProductTmpl == nullptr) {
		VN_LOG_ERROR("can't find productId : "<<pOrder->productid);
		return -3;
	}

	//未验证的情况
	if (pOrder->status == ORDER_STATUS_NO_PAY) {
		pOrder->receipt = receipt;

		// 更新订单凭据到数据库
		PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(
				PASSPORT_UPD_ORDER_RECEIPT);
		stmt->setString(0, pOrder->receipt);
		stmt->setUInt64(1, pOrder->orderid);
		PassPortDatabase.Execute(stmt, nullptr, nullptr);

		//更新订单状态
		pOrder->status = ORDER_STATUS_PAY;
	}
	if (pOrder->status == ORDER_STATUS_PAY) {
		if (pProductTmpl->productId < 1000) {
			//钻石
			u32 money = pProductTmpl->money;
			u32 count = AddPlayerBuyCount(pOrder->playerid, pOrder->productid);
			if (count <= pProductTmpl->limitBuyCount) {
				//赠送
				money += pProductTmpl->extraValue;
			}
			//增加钻石
			std::mutex& rPlayerLock = sWorldManager.getPlayerLock( pOrder->playerid);
			std::lock_guard<std::mutex> lock(rPlayerLock);
			PlayerPtr pPlayer = sWorldManager.getPlayer(pOrder->playerid);
			if (pPlayer.get() == nullptr) {
				return -5;
			} else {
				//更新
				pPlayer->diamond += money;
				pPlayer->save_amt += pProductTmpl->money;
				pPlayer->first_save = 1;
				PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_UPD_DIAMOND);
				stmt->setUInt32(0, pPlayer->diamond);
				stmt->setUInt32(1, pPlayer->gen_balance);
				stmt->setUInt32(2, pPlayer->first_save);
				stmt->setUInt32(3, pPlayer->save_amt);
				stmt->setUInt64(4, pPlayer->playerid);
				PassPortDatabase.Execute(stmt, nullptr, nullptr);
			}
		} else {
			//月卡
			Subscribe(pOrder->playerid,std::to_string(pProductTmpl->productId),pProductTmpl->money);
		}
		pOrder->status = ORDER_STATUS_CONFIRM;
	}
	// 更新状态到数据库
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement( PASSPORT_UPD_ORDER_STATUS);
	stmt->setUInt32(0, pOrder->status);
	stmt->setUInt64(1, pOrder->orderid);
	PassPortDatabase.Execute(stmt, nullptr, nullptr);
	if(isNotice){
		//向客户端发送发货成功的消息
		pkt::res_OrderFinished res;
		res.data.orderId = pOrder->orderid;
		res.data.errorCode = 0;
		sPlayerManager.sendMsgToPlayer(pOrder->playerid,res);
	}
	return 0;
}
*/


void OrderManger::InsertOrderTable(int pfid,int money,const std::string &pforderid, const std::string &cporderid, const std::string& msg){
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement( PASSPORT_INS_ORDERTHIRD);
	stmt->setInt32(0, pfid);
	stmt->setInt32(1, money);
	stmt->setString(2, pforderid);
	stmt->setString(3, cporderid);
	stmt->setString(4, msg);
	PassPortDatabase.Execute(stmt, nullptr, nullptr);
}
void OrderManger::InsertOrderTableEx(int pfid,int money,const std::string &pforderid, const std::string &cporderid, const std::string& msg,const std::string& pfid_sec,const std::string& pforderid_sec){
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement( PASSPORT_INS_ORDERTHIRD_EX);
	stmt->setInt32(0, pfid);
	stmt->setInt32(1, money);
	stmt->setString(2, pforderid);
	stmt->setString(3, cporderid);
	stmt->setString(4, msg);
	stmt->setString(5, pfid_sec);
	stmt->setString(6, pforderid_sec);
	PassPortDatabase.Execute(stmt, nullptr, nullptr);
}

int OrderManger::AddPlayerBuyCount(vn::u64 playerid, vn::s32 productid){
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement( PLAYER_BUY_SEL_PLAYER);
	stmt->setUInt64(0, playerid);
	stmt->setInt32(1, productid);
	int count = 0;
	PreparedQueryResult pResult = PassPortDatabase.SynQuery(stmt);
	if (pResult && pResult->GetRowCount()) {
		count = (*pResult)[0].GetInt32();
		PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(
				PLAYER_BUY_UPD_PLAYER);
		stmt->setUInt64(0, playerid);
		stmt->setInt32(1, productid);
		PassPortDatabase.Execute(stmt, nullptr, nullptr);
	} else {
		PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(
				PLAYER_BUY_INS_PLAYER);
		stmt->setUInt64(0, playerid);
		stmt->setInt32(1, productid);
		stmt->setInt32(2, 1);
		PassPortDatabase.SynExecute(stmt);
	}
	return count +1;
}

//购买月卡
int OrderManger::Subscribe(vn::u64 playerid, std::string inner_productid,u32 opendays) {
	//查询是否有月卡记录，有的话更新，没有的话创建
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PLAYER_SUBSCRIBE_SEL_BY_PLAYERID);
	stmt->setUInt64(0, playerid);
	stmt->setString(1, inner_productid);

	PlayerSubscribePtr pSubscribe = new Player_Subscribe;
	u64 curTime = time(NULL);

	PreparedQueryResult pResult = PassPortDatabase.SynQuery(stmt);
	if (pResult && pResult->GetRowCount()) {
		pSubscribe->id = (*pResult)[0].GetUInt64();
		pSubscribe->playerid = (*pResult)[1].GetUInt64();
		pSubscribe->inner_productid = (*pResult)[2].GetString();
		pSubscribe->begintime = (*pResult)[3].GetUInt64();
		pSubscribe->endtime = (*pResult)[4].GetUInt64();
		pSubscribe->grandtotal_opendays = (*pResult)[5].GetUInt32();

		if(pSubscribe->endtime >= curTime){
			pSubscribe->endtime += (opendays * 24 * 60 * 60);
			pSubscribe->grandtotal_opendays += opendays;
		} else {
			pSubscribe->begintime = curTime;
			pSubscribe->endtime = curTime + opendays * 24 * 60 * 60;
			pSubscribe->grandtotal_opendays += opendays;
		}
		//更新
		PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PLAYER_SUBSCRIBE_UPD_BY_PLAYERID);
		stmt->setUInt64(0, pSubscribe->begintime);
		stmt->setUInt64(1, pSubscribe->endtime);
		stmt->setUInt32(2, pSubscribe->grandtotal_opendays);
		stmt->setUInt64(3, pSubscribe->id);
		PassPortDatabase.SynExecute(stmt);
	} else {
		pSubscribe->playerid = playerid;
		pSubscribe->inner_productid = inner_productid;
		pSubscribe->begintime = curTime;
		pSubscribe->endtime = curTime + opendays * 24 * 60 * 60;
		pSubscribe->grandtotal_opendays = opendays;
		//创建
		PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PLAYER_SUBSCRIBE_INS);
		stmt->setUInt64(0, pSubscribe->playerid);
		stmt->setString(1, pSubscribe->inner_productid);
		stmt->setUInt64(2, pSubscribe->begintime);
		stmt->setUInt64(3, pSubscribe->endtime);
		stmt->setUInt32(4, pSubscribe->grandtotal_opendays);
		PassPortDatabase.SynExecute(stmt);
	}
	return 0;
}
std::map<std::string,PlayerSubscribePtr> OrderManger::GetPlayerSubscribeInfo(vn::u64 playerid){
	std::map<std::string, PlayerSubscribePtr> retMap;
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PLAYER_SUBSCRIBE_SEL_BY_PLAYERID_2);
	stmt->setUInt64(0, playerid);

	PreparedQueryResult pResult = PassPortDatabase.SynQuery(stmt);
	if (pResult && pResult->GetRowCount()) {
		do {
			PlayerSubscribePtr pSubscribe = new Player_Subscribe;
			pSubscribe->id = (*pResult)[0].GetUInt64();
			pSubscribe->playerid = (*pResult)[1].GetUInt64();
			pSubscribe->inner_productid = (*pResult)[2].GetString();
			pSubscribe->begintime = (*pResult)[3].GetUInt64();
			pSubscribe->endtime = (*pResult)[4].GetUInt64();
			pSubscribe->grandtotal_opendays = (*pResult)[5].GetUInt32();
			retMap[pSubscribe->inner_productid] = pSubscribe;
		} while (pResult->NextRow());
	}

	return std::move(retMap);
}
