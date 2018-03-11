/*
 * GamesvrSession.cpp
 *
 *  Created on: 2015年8月14日
 *      Author: leave
 */




#include "../../passportsvr/gamesvrlisten/GamesvrSession.h"

#include "../../passportsvr/Protocols.h"
#include "../../passportsvr/WorldManager.h"
#include "vnLog.h"
#include "../database/PassPortDatabaseConnection.h"
#include "vnBase64.h"


#define Gamesvr_MAX_TIMEOUT 120
#define Gamesvr_CHECK_TIME 60

GamesvrSession::GamesvrSession():Session() {
	m_lastHeartTime = time(NULL);
	m_hander = sTimerManager.createTimer(Gamesvr_CHECK_TIME,std::bind(&GamesvrSession::_cleanTimeOutSession,this),-1);
}
GamesvrSession::~GamesvrSession() {
	sTimerManager.destroyTimer(m_hander);
}
PacketPtr GamesvrSession::createRequest(u32 pid) {
    return PacketFactory<GamesvrSession>::instance().createRequest(pid);
}
//------------心跳包-----
VN_IMPL_PACKET_REQUEST2(GamesvrSession, pkt, req_Heart) {
	m_lastHeartTime = time(NULL);
	pkt::res_Heart res;
	res.data.handle = pkt->data.handle;
	res.data.serverid = 0;
	this->sendPacket(res);
}

void GamesvrSession::_cleanTimeOutSession(){
	uint64_t curTime = time(NULL);
	if(curTime - m_lastHeartTime > Gamesvr_MAX_TIMEOUT){
		close();
	}
}
