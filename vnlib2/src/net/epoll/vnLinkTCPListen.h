//
//  vnLinkTCPListen.h
//  vnlib
//
//  Created by Wayne on 4/28/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __vnlib__vnLinkTCPListen__
#define __vnlib__vnLinkTCPListen__

#include "vnLinkBase.h"

#include <set>

_vn_begin
namespace internal {
 
    class Link_TCP_Connection;

    class Link_TCP_Listen : public LinkBase {
    public:
        Link_TCP_Listen(RefCountedPtr<LinkPool> pool) : LinkBase(std::move(pool)) {}
        virtual ~Link_TCP_Listen();
        
        bool create(const std::function<_vn_ns::Link_TCP_Listen *()> &creator, const NetAddress &addr, bool reuseAddr, bool manageAccepted);
        
        void close();

        void traverseAccepted(std::function<bool(_vn_ns::Link_TCP_Connection *)> fn);
        
        virtual void ack(bool read, bool write, bool error);
        
        _vn_ns::Link_TCP_Listen * _handler() { return m_handler; }
        
        void _remove_accepted(Link_TCP_Connection *);
        
    protected:
        _vn_ns::Link_TCP_Listen *m_handler = 0;
        bool m_manageAccepted = false;
        SpinLock m_acceptedLock;
        std::set<Link_TCP_Connection *> m_accepted;

        void _accepted(int sockfd, const NetAddress &peer);
        void _close();
    };
    
    
}
_vn_end

#endif /* defined(__vnlib__vnLinkTCPListen__) */
