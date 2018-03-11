/*
 * NetLogSession.cpp
 *
 *  Created on: 2015年11月9日
 *      Author: leave
 */

#include "NetLogSession.h"
#include "../transaction/GSTransactionMgr.h"
#include "../transaction/GSTransaction.h"
#include "netlogsvr/Protocols.h"

NetLogSession::NetLogSession() {

}

NetLogSession::~NetLogSession() {
}

vn::PacketPtr NetLogSession::createRequest(vn::u32 pid) {
	return vn::PacketFactory<NetLogSession>::instance().createRequest(pid);
}

//------------心跳包-----
VN_IMPL_PACKET_REQUEST2(NetLogSession, pkt, req_Heart) {
	pkt::res_Heart res;
	res.data.handle = pkt->data.handle;
	res.data.serverid = 0;
	this->sendPacket(res);
}


VN_IMPL_PACKET_REQUEST2(NetLogSession, pkt, req_netlog) {
	GSLogContext ctx(pkt->data.log);
	TRANSACTIONALLOCMACRO(SGSTransactionMgr, GSInsertTransaction, ctx);
	pkt::res_netlog res;
	res.data.handle = pkt->data.handle;
	res.data.status = 0;
	this->sendPacket(res);
}

