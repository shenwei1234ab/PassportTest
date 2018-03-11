//
//  Session.cpp
//  passport_svr
//
//  Created by Wayne on 6/9/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "Session.h"

#include "vnLog.h"
using namespace _vn_ns;

void Session::sendPacket(Packet &pkt, SentAction sa) {
#ifdef VN_DEBUG
    if (m_console_mode) {
        _console_send(pkt, sa);
    }
#endif
    PackingStream packing;
    packing << DataStream::ccu(pkt.pid());
    pkt.save(packing);
    this->send(packing.pack((void *)(long)sa));
}


void Session::_init() {

   // SessionManager::instance().addSession(this);
}

void Session::_destroy() {
  //  SessionManager::instance().removeSession(this);
	drop();
}

void Session::_disconnected() {
    this->close();
}

void Session::_sent(size_t bytes, void *tag) {
    if (kClose == (long)tag) {
        this->close();
    }
}

void Session::_received(LinkBufferPtr buffers) {
//	VN_LOG_DEBUG("[" << this->peer().to_string() << "]  ");
/*	LinkBufferPtr p = buffers;
	while (p) {
		auto data = (unsigned char *)p->lock();
		size_t length = p->size();
		// TODO:
		str8 s;
		s.resize(length * 2);
		char *pp = &s[0];
		for (size_t i = 0; i < length; ++i, ++data) {
			sprintf(pp, "%02X", (uint8_t)*data);
			pp += 2;
		}
		p->unlock();

		VN_LOG_DEBUG(s);

		p = p->next();
	}*/

#ifdef VN_DEBUG
    if (m_console_mode) {
        _console_recv(std::move(buffers));
        return ;
    }
#endif
    m_unpacking.append(std::move(buffers));
    while (m_unpacking.ready()) {
    //	m_unpacking.lockState();
        try {
            u32 pid;
            m_unpacking >> DataStream::cu(pid);
				if (!isLogin(pid)) {
					VN_LOG_ERROR("[" << this->peer().to_string() << "] is't login,I can't process pid: " << pid);
					this->close();
				}
#ifdef VN_DEBUG
            if (!pid) {
                m_console_mode = true;
                VN_LOG_DEBUG("[" << this->peer().to_string() << "] console mode");
                return ;
            }
#endif
            auto pkt = this->createRequest(pid);
            if (!pkt) {
				VN_LOG_WARN("[" << this->peer().to_string() << "] invalid pid: " << pid <<" size:"<<m_unpacking.size());
                this->close();
            } else {
            	 //VN_LOG_DEBUG("[" << this->peer().to_string() << "] process pid: " << pid);
                pkt->load(m_unpacking);
                pkt->process(this);
                //VN_LOG_DEBUG("[" << this->peer().to_string() << "] process pid: " << pid << "end");
            }
        } catch (DataStream::Exception &) {
            m_unpacking.unlockState();
        	VN_LOG_WARN("[" << this->peer().to_string() << "] invalid packet data"<<" size:"<<m_unpacking.size());
            this->close();
        }
    //    m_unpacking.unlockState();
    }
}

#ifdef VN_DEBUG
void Session::_console_recv(LinkBufferPtr buffers) {
    m_unpacking.append(std::move(buffers));
    while (m_unpacking.ready()) {
        try {
            u32 pid;
            m_unpacking >> DataStream::cu(pid);
            Variable_object vobj;
            vobj.load(m_unpacking);
            auto pkt = this->createRequest(pid);
            if (!pkt) {
                VN_LOG_DEBUG("[" << this->peer().to_string() << "] invalid pid: " << pid);
            } else {
                pkt->load(vobj);
                pkt->process(this);
            }
        } catch(DataStream::Exception &) {
            VN_LOG_DEBUG("[" << this->peer().to_string() << "] invalid console data");
            this->close();
        }
    }
}

void Session::_console_send(vn::Packet &pkt, SentAction sa) {
	PackingStream packing;
    packing << DataStream::ccu(pkt.pid());
    Variable_object vobj;
    pkt.save(vobj);
    vobj.save(packing);
    this->send(packing.pack((void *)(long)sa));
}
#endif
