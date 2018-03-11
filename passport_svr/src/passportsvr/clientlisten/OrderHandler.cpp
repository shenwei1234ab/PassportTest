/*
 * OrderHandler.cpp
 *
 *  Created on: 2015年6月24日
 *      Author: leave
 */

#include "../../passportsvr/clientlisten/ClientSession.h"
#include "../../passportsvr/manager/PlayerManager.h"
#include "../../passportsvr/WorldManager.h"


/*
//新建玩家记录
void ClientSession::insertPlayerRecord(u32 zoneId){
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(
			PASSPORT_INS_PLAYER);
	m_playerId = sWorldManager.createPlayerId();
	stmt->setUInt64(0, m_playerId);
	stmt->setUInt64(1, m_ppId);
	stmt->setUInt32(2, m_gameId);
	stmt->setUInt32(3, zoneId);
	stmt->setUInt32(4, 0);
	stmt->setUInt32(5, 0);
	stmt->setUInt8(6, 0);
	stmt->setUInt32(7, 0);
	PassPortDatabase.Execute(stmt, nullptr, nullptr);

//	sSessionManager.addClientSession(m_playerId,{this,true});
}*/
void ClientSession::RegisterPlayer(vn::u64 ppid, vn::u32 pptoken, u32 gameId, u64 playerId){
	PlayerHandlerPtr pPlayerHandler = sPlayerManager.GetPlayer(playerId);
	if(!pPlayerHandler) {
		pPlayerHandler= new PlayerHandler(ppid, pptoken, gameId, playerId);
	}
	pPlayerHandler ->SetClientSession( {this,true});
	//pPlayerHandler ->SetToken(pptoken);
	pPlayerHandler ->SetToken( sockfd());
	sPlayerManager.AddPlayer(playerId,pPlayerHandler);
	//m_playerHandler = pPlayerHandler;
}
//------------客户端申请订单号------
VN_IMPL_PACKET_REQUEST2(ClientSession, pkt, req_CreateOrder) {
	m_playerId = sWorldManager.getPlayerId(m_ppId,m_gameId,pkt->data.serverId);
	if (m_playerId == 0) {
		pkt::res_CreateOrderError res;
		res.data.errorCode = ERROR_CODE_Player_NOPlayer;
		this->sendPacket(res);
		return;
	}
	RegisterPlayer(m_ppId,m_tmpToken,m_gameId,m_playerId);
	PlayerHandlerPtr pPlayerHandler= sPlayerManager.GetPlayer(m_playerId);
	if(pPlayerHandler){
		pPlayerHandler->CreateOrder(pkt);
	} else{
		pkt::res_CreateOrderError res;
		res.data.errorCode = ERROR_CODE_LOGIN_TOKEN;
		this->sendPacket(res);
		return;
	}
}



//------------客户端发送订单数据------
VN_IMPL_PACKET_REQUEST2(ClientSession, pkt, req_Purchase) {
	PlayerHandlerPtr pPlayerHandler = sPlayerManager.GetPlayer(m_playerId);
	if (!pPlayerHandler) {
		OrderPtr pOrder = sOrderManger.GetOrderInfo(pkt->data.orderId);
		if(pOrder) {
			RegisterPlayer(m_ppId,m_tmpToken,m_gameId,pOrder->playerid);
			pPlayerHandler= sPlayerManager.GetPlayer(pOrder->playerid);
		}
	}

	if(pPlayerHandler) {
		if(!pPlayerHandler->GetClientSession()){
			pPlayerHandler->SetClientSession( {this,true});
		}
		pPlayerHandler->Purchase(pkt);
	} else {
		pkt::res_CreateOrderError res;
		res.data.errorCode = ERROR_CODE_LOGIN_TOKEN;
		this->sendPacket(res);
		return;
	}
}

//------------客户端发送订单操作------
VN_IMPL_PACKET_REQUEST2(ClientSession, pkt, req_OrderCancel) {
	/*PlayerHandlerPtr pPlayerHandler = sPlayerManager.GetPlayer(m_playerId);
	if (pPlayerHandler) {
		pPlayerHandler->OrderCancel(pkt);
	} else {
		pkt::res_OrderFinished res;
		res.data.errorCode = ERROR_CODE_LOGIN_TOKEN;
		this->sendPacket(res);
		return;
	}*/
	pkt::res_OrderFinished res;
	res.data.orderId = pkt->data.orderId;
	res.data.errorCode = ERROR_CODE_ORDER_CLANCEL;
	this->sendPacket(res);
}
