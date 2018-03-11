//
//  SessionManager.cpp
//  passport_svr
//
//  Created by Wayne on 6/9/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "../../passportsvr/manager/SessionManager.h"

//void SessionManager::addSession(SessionPtr session) {
//    m_lock.lock();
//    long key = (long)session.get();
//    m_sessions[key] = std::move(session);
//    m_lock.unlock();
//}
//
//void SessionManager::removeSession(Session *session) {
//    m_lock.lock();
//    m_sessions.erase((long)session);
//    m_lock.unlock();
//}
//
//void SessionManager::closeAll() {
//    m_lock.lock();
//    for (auto &i : m_sessions) {
//        i.second->close();
//    }
//    m_lock.unlock();
//}
/*
void SessionManager::addClientSession(u64 playerId, ClientSessionPtr session) {
	if (playerId == 0) {
		return;
	}
	m_publicLock.lock();
	m_ClientSessions[playerId] = std::move(session);
	m_publicLock.unlock();
}
void SessionManager::removeClientSession(u64 playerId) {
	if (playerId == 0) {
		return;
	}
	m_publicLock.lock();
	m_ClientSessions.erase(playerId);
	m_publicLock.unlock();
}
bool SessionManager::sendMsgToPlayer(u32 serverId,Packet &pkt){
	ClientSessionPtr pSession = m_ClientSessions[serverId];
	if(nullptr == pSession.get()){
		return false;
	}
	pSession->sendPacket(pkt);
	return true;
}*/
void SessionManager::addGamesvrSession(u32 serverId, GamesvrSessionPtr session) {
	m_privateLock.lock();
	m_pGamesvrSession[serverId] = std::move(session);
	m_privateLock.unlock();
}
void SessionManager::removeGamesvrSession(u32 serverId) {
	m_privateLock.lock();
	m_pGamesvrSession.erase(serverId);
	m_privateLock.unlock();
}
bool SessionManager::sendMsgToGameSvr(u32 serverId, Packet &pkt) {
	GamesvrSessionPtr pSession = m_pGamesvrSession[serverId];
	if(nullptr == pSession.get()){
		return false;
	}
	pSession->sendPacket(pkt);
	return true;
}

bool SessionManager::isGameSvrOk(u32 serverId){
	GamesvrSessionPtr pSession = m_pGamesvrSession[serverId];
	if (nullptr == pSession.get()) {
		return false;
	}
	return true;
}
