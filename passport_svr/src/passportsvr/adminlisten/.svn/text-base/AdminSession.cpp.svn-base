/*
 * AdminSession.cpp
 *
 *  Created on: 2015年6月9日
 *      Author: leave
 */

#include "../../passportsvr/adminlisten/AdminSession.h"

#include "../../passportsvr/Protocols.h"


PacketPtr AdminSession::createRequest(u32 pid) {
    return PacketFactory<AdminSession>::instance().createRequest(pid);
}
/*

VN_IMPL_PACKET_REQUEST2(AdminSession, pkt, req_Login) {
    // TODO:
    pkt::res_LoginFailed res;
    res.data.error = 1;
    this->sendPacket(res, kClose);
}
*/
