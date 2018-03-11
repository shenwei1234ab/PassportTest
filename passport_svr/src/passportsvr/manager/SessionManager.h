//
//  SessionManager.h
//  passport_svr
//
//  Created by Wayne on 6/9/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __passport_svr__SessionManager__
#define __passport_svr__SessionManager__

#include "vnSingleton.h"
#include <mutex>
#include <map>
#include "../../passportsvr/clientlisten/ClientSession.h"
#include "../../passportsvr/gamesvrlisten/GamesvrSession.h"

class SessionManager : public Singleton<SessionManager> {
public:
//    void addSession(SessionPtr session);
//    void removeSession(Session *session);
//
//    void closeAll();
/*
    void addClientSession(u64 playerId, ClientSessionPtr session);
    void removeClientSession(u64 playerId);

    //只有创建支付订单之后函数才有效
    bool sendMsgToPlayer(u32 serverId,Packet &pkt);*/

//	const std::map<u64, ClientSessionPtr>* getClientSessions() {
//		return &m_ClientSessions;
//	}
    ClientSessionPtr getClientSession(u32 _playId){
		return m_ClientSessions[_playId];
	}


	void addGamesvrSession(u32 serverId, GamesvrSessionPtr session);
	void removeGamesvrSession(u32 serverId);
	bool sendMsgToGameSvr(u32 serverId,Packet &pkt);
	bool isGameSvrOk(u32 serverId);
private:
//    std::mutex m_lock;
//    std::map<long, SessionPtr> m_sessions;

    std::mutex m_publicLock;
    //key:playerId
    std::map<u64, ClientSessionPtr> m_ClientSessions;

    std::mutex m_privateLock;
    //key:serverID
    std::map<u32, GamesvrSessionPtr> m_pGamesvrSession;
};

#define sSessionManager SessionManager::instance()
#endif /* defined(__passport_svr__SessionManager__) */
