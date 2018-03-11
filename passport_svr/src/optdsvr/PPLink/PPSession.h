/*
 * PPSession.h
 *
 *  Created on: 2016年4月19日
 *      Author: leave
 */

#ifndef SRC_OPTDSVR_PPLINK_PPSESSION_H_
#define SRC_OPTDSVR_PPLINK_PPSESSION_H_


#include "framework/Session.h"

#include "vnPacketFactory.h"
#include <functional>
#include "../Protocols.h"

class PPSession  : public Session {
    VN_DECL_PACKET_HANDLER()
public:
    virtual vn::PacketPtr createRequest(vn::u32 pid);


	template<class Packet_T>
	int SendToPassport(vn::s32& r_handle, Packet_T &pkt) {

		pkt.data.handle = r_handle;
//		_sendPacket(pkt);
		sendPacket(pkt);
		return 0;
	}

	void SetCallbackFn(std::function<void(pkt::res_JsonDate* pkt)>&& _callback){
		m_callbackFn = std::move(_callback);
	}
protected:
	std::function<void(pkt::res_JsonDate* pkt)> m_callbackFn;
};

#endif /* SRC_OPTDSVR_PPLINK_PPSESSION_H_ */
