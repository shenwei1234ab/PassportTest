/*
 * AdminSession.h
 *
 *  Created on: 2015年6月9日
 *      Author: leave
 */

#ifndef SRC_SERVER_ADMINSESSION_H_
#define SRC_SERVER_ADMINSESSION_H_

#include "framework/Session.h"

#include "vnPacketFactory.h"

class AdminSession  : public Session{
	VN_DECL_PACKET_HANDLER()
public:
	virtual vn::PacketPtr createRequest(vn::u32 pid);
};

#endif /* SRC_SERVER_ADMINSESSION_H_ */
