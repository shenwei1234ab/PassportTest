/*
 * PlayerManager.cpp
 *
 *  Created on: 2015年10月15日
 *      Author: leave
 */

#include "../../passportsvr/manager/PlayerManager.h"

#include <functional>
#include "manager/TimerManager.h"

#define PLAYER_MAX_TIMEOUT 300
#define PLAYER_CHECK_TIME 60

PlayerManager::~PlayerManager() {
	// TODO Auto-generated destructor stub
}

bool PlayerManager::init(){
	sTimerManager.createTimer(PLAYER_CHECK_TIME,std::bind(&PlayerManager::_cleanTimeOutPlayer,this),-1);
	return true;
}
void PlayerManager::_cleanTimeOutPlayer(){
	u64 curTime = time(NULL);
	VN_LOG_DEBUG("PlayerManager::_cleanTimeOutPlayer");
	std::lock_guard<std::mutex> lck (m_playerMapLock);
	std::map<vn::u64, PlayerHandlerPtr>::iterator itrPlayer = m_playerMap.begin();
	for(;itrPlayer != m_playerMap.end(); ){
		PlayerHandlerPtr pPlayerHandler = itrPlayer->second;
		if(pPlayerHandler
				&& ! pPlayerHandler->_clientSession
				&& curTime - pPlayerHandler->m_lastOptTime > PLAYER_MAX_TIMEOUT){
			if (pPlayerHandler->m_tmpToken) {
				m_socketMapLock.lock();
				m_socketMap.erase(pPlayerHandler->m_tmpToken);
				m_socketMapLock.unlock();
			}
			itrPlayer = m_playerMap.erase(itrPlayer);
		} else {
			itrPlayer++;
		}
	}
}
void PlayerManager::AddPlayer(vn::u64 playerId, PlayerHandlerPtr playerHandler){
	if (playerId == 0) {
		return;
	}
	m_playerMapLock.lock();
	m_playerMap[playerId] = playerHandler;
	m_playerMapLock.unlock();

	if (playerHandler->m_tmpToken) {
		m_socketMapLock.lock();
		m_socketMap[playerHandler->m_tmpToken] = playerHandler;
		m_socketMapLock.unlock();
	}
}
void PlayerManager::RemovePlayer(vn::u64 playerId){
	if (playerId == 0) {
		return;
	}

	PlayerHandlerPtr playerHandler = m_playerMap[playerId];
	if (playerHandler) {
		if (playerHandler->m_tmpToken) {
			m_socketMapLock.lock();
			m_socketMap.erase(playerHandler->m_tmpToken);
			m_socketMapLock.unlock();
		}
		m_playerMapLock.lock();
		m_playerMap.erase(playerId);
		m_playerMapLock.unlock();
	}
}

PlayerHandlerPtr PlayerManager::GetPlayer(vn::u64 playerId){
	std::lock_guard<std::mutex> lck (m_playerMapLock);
	return m_playerMap[playerId];
}

PlayerHandlerPtr PlayerManager::GetPlayerBySocket(vn::u32 socketId){
	std::lock_guard<std::mutex> lck (m_socketMapLock);
	return m_socketMap[socketId];
}

void PlayerManager::Disconnect(vn::u32 socketId){
	PlayerHandlerPtr playerHandler = m_socketMap[socketId];
	if (playerHandler) {
		playerHandler->m_lastOptTime = time(NULL);
		playerHandler->_clientSession = nullptr;
	}
}
void PlayerManager::PlayerDisconnect(vn::u64 playerId){
	PlayerHandlerPtr playerHandler = m_playerMap[playerId];
	if (playerHandler) {
		playerHandler->m_lastOptTime = time(NULL);
		playerHandler->_clientSession = nullptr;
	}
}
//根据 ppid，pptoken 查询 playerHandler
PlayerHandlerPtr PlayerManager::LookforPlayerHandler(vn::u64 ppid, vn::u32 pptoken){
	std::lock_guard<std::mutex> lck (m_socketMapLock);
	PlayerHandlerPtr playerHandler = m_socketMap[pptoken];
	if(playerHandler && playerHandler->m_ppId == ppid){
		return std::move(playerHandler);
	}
	return nullptr;
//	playerHandler = new PlayerHandler(ppid,pptoken);
//	m_socketMap[pptoken] = playerHandler;
//	return playerHandler;
}

//向客户发送信息
bool PlayerManager::sendMsgToPlayer(vn::u32 playerId, vn::Packet &pkt){
	std::lock_guard<std::mutex> lck (m_playerMapLock);
	PlayerHandlerPtr playerHandler = m_playerMap[playerId];
	if (nullptr == playerHandler.get() || nullptr == playerHandler->_clientSession.get()) {
		return false;
	}
	playerHandler->_clientSession->sendPacket(pkt);
	return true;
}

/*//重置SocketId
void PlayerManager::ResetSocketId(vn::u32 oldSocketId, vn::u32 newSocketId) {
	PlayerHandlerPtr playerHandler = m_socketMap[oldSocketId];
	if (playerHandler) {
		m_socketMapLock.lock();
		m_socketMap[newSocketId] = m_socketMap[oldSocketId];
		m_socketMap.erase(oldSocketId);
		m_socketMapLock.unlock();
	}

}*/

//订单支付成功进行发货
int PlayerManager::DeliverGoods(vn::u64 orderId, u32 orderPrice, std::string& receipt, int os, bool isNotice){
	OrderPtr pOrder = sOrderManger.GetOrderInfo(orderId);
	if(!pOrder.get()) {
		//找不到该订单
		return -1;
	}

	m_playerMapLock.lock();
	PlayerHandlerPtr playerHandler = m_playerMap[pOrder->playerid];
	m_playerMapLock.unlock();
	if(!playerHandler){
		// 载入玩家
		playerHandler = new PlayerHandler(pOrder->playerid);
		sPlayerManager.AddPlayer(pOrder->playerid,playerHandler);
	}
	return playerHandler->DeliverGoods(orderId, orderPrice, receipt,os,isNotice);
}

void PlayerManager::SetPlayerPtr(PlayerPtr pPlayer){
	if(!pPlayer){
		return;
	}
	//std::lock_guard<std::mutex> lck (m_playerMapLock);
	m_playerMapLock.lock();
	PlayerHandlerPtr playerHandler = m_playerMap[pPlayer->playerid];
	m_playerMapLock.unlock();
	if (playerHandler) {
		playerHandler->SetPlayerPtr(pPlayer);
	} else {
		PlayerHandlerPtr pPlayerHandler = new PlayerHandler(pPlayer);
		sPlayerManager.AddPlayer(pPlayer->playerid,pPlayerHandler);
	}
}
PlayerPtr PlayerManager::GetPlayerPtr(vn::u64 playerId) {
	m_playerMapLock.lock();
	PlayerHandlerPtr playerHandler = m_playerMap[playerId];
	m_playerMapLock.unlock();
	if (!playerHandler) {
		playerHandler = new PlayerHandler(playerId);
		sPlayerManager.AddPlayer(playerId,playerHandler);
	}
	return playerHandler->getPlayer();
}
