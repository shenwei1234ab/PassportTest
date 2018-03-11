/*
 * NetLogSession.h
 *
 *  Created on: 2015年11月9日
 *      Author: leave
 */

#ifndef SRC_NETLOGSVR_NETLOGLISTEN_NETLOGSESSION_H_
#define SRC_NETLOGSVR_NETLOGLISTEN_NETLOGSESSION_H_

#include "framework/Session.h"
#include "vnPacketFactory.h"

class NetLogSession: public Session {
VN_DECL_PACKET_HANDLER()
public:
	NetLogSession();
	virtual ~NetLogSession();

public:
	virtual vn::PacketPtr createRequest(vn::u32 pid);
};

#endif /* SRC_NETLOGSVR_NETLOGLISTEN_NETLOGSESSION_H_ */
