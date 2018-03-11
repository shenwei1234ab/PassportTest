//
//  Session.h
//  passport_svr
//
//  Created by Wayne on 6/9/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __passport_svr__Session__
#define __passport_svr__Session__

#include "vnLink.h"
#include "vnPacket.h"
#include "vnPackingStream.h"
#include "vnUnpackingStream.h"

//using namespace _vn_ns;

class Session : public vn::Link_TCP_Connection, public vn::RefCounted {
public:
    
    enum SentAction {
        kNone = 0,
        kClose
    };
    
    virtual vn::PacketPtr createRequest(vn::u32 pid) { return nullptr; };
    
    void sendPacket(vn::Packet &pkt, SentAction sa = kNone);
    
protected:
    virtual void _init();
    virtual void _destroy();
    virtual void _disconnected();
    virtual void _sent(size_t bytes, void *tag);
    virtual void _received(vn::LinkBufferPtr buffers);
    
    virtual bool isLogin(vn::u32 /*pid*/ ){return true;}
    
    //vn::PackingStream m_packing;
    vn::UnpackingStream m_unpacking;

#ifdef VN_DEBUG
    bool m_console_mode = false;
    void _console_recv(vn::LinkBufferPtr buffers);
    void _console_send(vn::Packet &pkt, SentAction sa = kNone);
#endif
};

typedef vn::RefCountedPtr<Session> SessionPtr;

#endif /* defined(__passport_svr__Session__) */
