/*
 * PPSession.cpp
 *
 *  Created on: 2016年4月19日
 *      Author: leave
 */

#include "PPSession.h"
#include "vnLog.h"


vn::PacketPtr PPSession::createRequest(vn::u32 pid){
	return vn::PacketFactory<PPSession>::instance().createRequest(pid);
}


VN_IMPL_PACKET_REQUEST2(PPSession,pkt, res_JsonDate) {
	if(m_callbackFn){
		m_callbackFn(pkt);
	}
}

VN_IMPL_PACKET_REQUEST2(PPSession, pkt,res_Heart) {
	//VN_LOG_DEBUG("rec heart pkt");
}
