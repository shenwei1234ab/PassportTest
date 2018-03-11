//
//  vnLinkPool.h
//  vnlib
//
//  Created by Wayne on 4/28/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __vnlib__vnLinkPool__
#define __vnlib__vnLinkPool__

#include "vnLink.h"
#include "vnRunLoop.h"
#include "vnSpinLock.h"

#include <thread>
#include <vector>
#include <list>
#include <deque>

_vn_begin

namespace internal {

    class LinkBase;
    class LinkBaseHolder;

    class LinkPool : public RefCounted, public _vn_ns::LinkPool {
    public:
        LinkPool(size_t threads);
        virtual ~LinkPool();
        
        virtual void release();
        
        virtual _vn_ns::Link_TCP_Listen * listen(const std::function<_vn_ns::Link_TCP_Listen *()> &creator, const NetAddress &addr, bool reuseAddr, bool manageAccepted);
        virtual _vn_ns::Link_TCP_Connection * connect(const std::function<_vn_ns::Link_TCP_Connection *()> &creator, const NetAddress &addr);
        
        bool reg(LinkBase *link);
        bool unreg(LinkBase *link);
        
        void post(std::function<void()> fn);
    private:
        int m_epoll;
        std::vector<std::thread> m_threads;
        
        void _query_loop();
        size_t _do_events();

        std::list<RefCountedPtr<LinkBaseHolder>> m_dropHolders;
        SpinLock m_dropHoldersLock;

        std::deque<std::function<void()>> m_events;
        SpinLock m_eventsLock;

        int m_sleep_time = 0;
    };
    
} // namespace internal

_vn_end

#endif /* defined(__vnlib__vnLinkPool__) */
