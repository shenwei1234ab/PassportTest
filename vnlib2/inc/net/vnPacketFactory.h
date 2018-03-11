//
//  vnPacketFactory.h
//  vnlib
//
//  Created by Wayne on 14-5-24.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnPacketFactory_h
#define vnlib_vnPacketFactory_h

#include "vnPacket.h"
#include "vnSingleton.h"

//#if VN_PLATFORM == VN_PLATFORM_FREEBSD
//#   include <map>
//#else
#   include <unordered_map>
//#endif

_vn_begin

template <typename _Handler>
class PacketFactory : public Singleton< PacketFactory<_Handler> > {
	friend struct CreateUsingNew< PacketFactory<_Handler> >;
public:
	
	template <typename _Packet>
	void registerRequest() {
		class PacketEx : _Packet {
		public:
			static Packet * create() { return vnnew PacketEx(); }
			virtual void process(void *param) {
				 static_cast<_Handler *>(param)->onRequest((_Packet *)this);
			}
		};
		m_requests[_Packet::ID] = PacketEx::create;
	}
    
    template <typename _Packet>
    void registerRequestNoProcess() {
        class PacketEx : _Packet {
        public:
            static Packet * create() { return vnnew PacketEx(); }
        };
        m_requests[_Packet::ID] = PacketEx::create;
    }
	
	template <typename _Packet>
	void registerResponse() {
		class PacketEx : _Packet {
		public:
			static Packet * create() { return vnnew PacketEx(); }
			virtual void process(void *param) {
				static_cast<_Handler *>(param)->onResponse((_Packet *)this);
			}
		};
		m_responses[_Packet::ID] = PacketEx::create;
	}
    
    template <typename _Packet>
    void registerResponseNoProcess() {
        class PacketEx : _Packet {
        public:
            static Packet * create() { return vnnew PacketEx(); }
        };
        m_responses[_Packet::ID] = PacketEx::create;
    }
	
	PacketPtr createRequest(u32 pid) {
		auto it = m_requests.find(pid);
		if (it == m_requests.end()) {
			return PacketPtr();
		}
		return it->second();
	}
	
	PacketPtr createResponse(u32 pid) {
		auto it = m_responses.find(pid);
		if (it == m_responses.end()) {
			return PacketPtr();
		}
		return it->second();
	}
	
private:
//#if VN_PLATFORM == VN_PLATFORM_FREEBSD
//    std::map<u32, Packet *(*)()>
//#else
    std::unordered_map<u32, Packet *(*)()>
//#endif
    m_requests, m_responses;
};

_vn_end

#define VN_DECL_PACKET_HANDLER() \
	public:\
		template <class _Packet>\
		void onRequest(_Packet *);\
		template <class _Packet>\
		void onResponse(_Packet *);

#define VN_IMPL_PACKET_REQUEST(handler, packet) \
	static struct _auto_reg_##handler##_req_##packet {\
		_auto_reg_##handler##_req_##packet() {\
			_vn_ns::PacketFactory<handler>::instance().registerRequest<packet>();\
		}\
	} _auto_reg_##handler##_req_##packet##_ins;\
	template<> void handler::onRequest<packet>(packet *pkt)

#define VN_IMPL_PACKET_REQUEST2(handler, ns, packet) \
        static struct _auto_reg_##handler##_req_##packet {\
                _auto_reg_##handler##_req_##packet() {\
                        _vn_ns::PacketFactory<handler>::instance().registerRequest<ns::packet>();\
                }\
        } _auto_reg_##handler##_req_##packet##_ins;\
        template<> void handler::onRequest<ns::packet>(ns::packet *pkt)

#define VN_IMPL_PACKET_RESPONSE(handler, packet) \
	static struct _auto_reg_##handler##_res_##packet {\
		_auto_reg_##handler##_res_##packet() {\
			_vn_ns::PacketFactory<handler>::instance().registerResponse<packet>();\
		}\
	} _auto_reg_##handler##_res_##packet##_ins;\
	template<> void handler::onResponse<packet>(packet *pkt)

#define VN_IMPL_PACKET_RESPONSE2(handler, ns, packet) \
        static struct _auto_reg_##handler##_res_##packet {\
                _auto_reg_##handler##_res_##packet() {\
                        _vn_ns::PacketFactory<handler>::instance().registerResponse<ns::packet>();\
                }\
        } _auto_reg_##handler##_res_##packet##_ins;\
        template<> void handler::onResponse<ns::packet>(ns::packet *pkt)

#endif

