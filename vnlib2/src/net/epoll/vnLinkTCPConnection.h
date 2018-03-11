//
//  vnLinkTCPConnection.h
//  vnlib
//
//  Created by Wayne on 4/29/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __vnlib__vnLinkTCPConnection__
#define __vnlib__vnLinkTCPConnection__

#include "vnLinkBase.h"

_vn_begin
namespace internal {

    class Link_TCP_Listen;

    class Link_TCP_Connection : public LinkBase {
    public:
        Link_TCP_Connection(RefCountedPtr<LinkPool> pool) : LinkBase(std::move(pool)) {}
        virtual ~Link_TCP_Connection();
        
        bool create(const std::function<_vn_ns::Link_TCP_Connection *()> &creator, const NetAddress &addr);
        
        void attach(int sockfd, const NetAddress &peer, _vn_ns::Link_TCP_Connection *handler, RefCountedPtr<Link_TCP_Listen> manager);
        
        bool connected() const { return m_state == kConnected; }
        NetAddress peer() const { return m_peer; }
        void send(LinkBufferPtr buffers);
        void close();
        
        virtual void ack(bool read, bool write, bool error);
        
        _vn_ns::Link_TCP_Connection * _handler() { return m_handler; }
        
    private:
        NetAddress m_peer;
        enum State {
            kConnecting = 0,
            kConnected,
            kDisconnected
        } m_state = kConnecting;
        bool m_sending_waiting = false;
        size_t m_sending_offset = 0;
        LinkBufferPtr m_sending_head, m_sending_tail;
        
        _vn_ns::Link_TCP_Connection *m_handler = nullptr;
        RefCountedPtr<Link_TCP_Listen> m_manager;
        
        void _ack(bool read, bool write, bool error);
        void _recv();
        void _close();
        void _send();
        void _send_append(LinkBufferPtr buffers);
    };
    
}
_vn_end

#endif /* defined(__vnlib__vnLinkTCPConnection__) */
