//
//  vnLinkPool.cpp
//  vnlib
//
//  Created by Wayne on 4/28/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "vnLinkPool.h"
#include "vnLinkTCPListen.h"
#include "vnLinkTCPConnection.h"

//#include "vnLog.h"

#include <mutex>
#include <chrono>

#include <unistd.h>
#include <sys/epoll.h>

_vn_begin

namespace internal {
    
    LinkPool::LinkPool(size_t threads)
    : m_epoll(::epoll_create(1024)) {
        if (!threads) {
            threads = 1;
        }
        m_threads.reserve(threads + 1);
        
        m_threads.push_back(std::thread(&LinkPool::_query_loop, this));
    }

    LinkPool::~LinkPool() {
        //VN_LOG_DEBUG("close epoll_fd.");
        ::close(m_epoll);
        //VN_LOG_DEBUG("wait all threads.");
        for (auto &i : m_threads) {
            i.join();
        }
    }

    void LinkPool::release() {
        while (ref_count(this) > 1) {
            std::this_thread::yield();
        }
        this->drop();
    }
    
    _vn_ns::Link_TCP_Listen * LinkPool::listen(const std::function<_vn_ns::Link_TCP_Listen *()> &creator, const NetAddress &addr, bool reuseAddr, bool manageAccepted) {
        auto obj = vnnew Link_TCP_Listen({this, true});
        if (!obj->create(creator, addr, reuseAddr, manageAccepted)) {
            delete obj;
            return nullptr;
        }
        return obj->_handler();
    }
    
    _vn_ns::Link_TCP_Connection * LinkPool::connect(const std::function<_vn_ns::Link_TCP_Connection *()> &creator, const NetAddress &addr) {
        auto obj = vnnew Link_TCP_Connection({this, true});
        if (!obj->create(creator, addr)) {
            delete obj;
            return nullptr;
        }
        return obj->_handler();
    }
    
    bool LinkPool::reg(LinkBase *link) {
        auto holder = link->holder();
        struct epoll_event ev;
        ev.events = (u32)(EPOLLIN | EPOLLOUT | EPOLLET);
        ev.data.ptr = holder;
        //link->grab();
        holder->grab();
        if (::epoll_ctl(m_epoll, EPOLL_CTL_ADD, link->sockfd(), &ev)) {
            //link->drop();
            link->drop();
            //VN_LOG_DEBUG("sock_fd(" << link->sockfd() << ") reg failed!");
            return false;
        }
        //VN_LOG_DEBUG("sock_fd(" << link->sockfd() << ") reg ok.");
        return true;
    }

    bool LinkPool::unreg(LinkBase *link) {
        if (::epoll_ctl(m_epoll, EPOLL_CTL_DEL, link->sockfd(), 0)) {
            //VN_LOG_DEBUG("sock_fd(" << link->sockfd() << ") unreg failed!");
            return false;
        }
        //link->drop();
        m_dropHoldersLock.lock();
        m_dropHolders.push_back(link->holder());
        m_dropHoldersLock.unlock();
        //VN_LOG_DEBUG("sock_fd(" << link->sockfd() << ") unreg ok.");
        return true;
    }

    void LinkPool::post(std::function<void()> fn) {
        std::lock_guard<SpinLock> lck(m_eventsLock);
        m_events.push_back(std::move(fn));
    }

    void LinkPool::_query_loop() {
        const int evc = 1024;
        struct epoll_event evv[evc];

        for (;;) {
            int ret = ::epoll_wait(m_epoll, evv, evc, 0);
            //VN_LOG_DEBUG("epoll_wait() returned(" << ret << ")");
            if (ret < 0) {
                //VN_LOG_DEBUG("epoll_wait error(" << (int)errno << ")");
                if (errno == EINTR) {
                    continue;
                }
                while (_do_events());
                break;
            }
            for (int i = 0; i < ret; ++i) {
                auto &ev = evv[i];
                //VN_LOG_DEBUG("sock_fd(" << static_cast<LinkBase *>(ev.data.ptr)->sockfd() << ") " << (ev.events & EPOLLIN ? "IN " : " ") << (ev.events & EPOLLOUT ? "OUT " : " ") << (ev.events & EPOLLERR ? "ERR" : ""));
                static_cast<LinkBaseHolder *>(ev.data.ptr)->ack(bool(ev.events & EPOLLIN), bool(ev.events & EPOLLOUT), bool(ev.events & EPOLLERR));
            }
            std::list<RefCountedPtr<LinkBaseHolder>> tmp;
            m_dropHoldersLock.lock();
            tmp.swap(m_dropHolders);
            m_dropHoldersLock.unlock();

            

            if (_do_events() + ret == 0) {
                if (m_sleep_time < 100) {
                    ++m_sleep_time;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(m_sleep_time));
            } else {
                m_sleep_time = 0;
            }
        }
        //VN_LOG_DEBUG("query_loop exit.");
    }

    size_t LinkPool::_do_events() {
        std::deque<std::function<void()>> events;
        m_eventsLock.lock();
        events.swap(m_events);
        m_eventsLock.unlock();
        for (auto &i : events) {
            i();
        }
        return events.size();
    }
    
} // namespace internal

LinkPool * LinkPool::create(size_t threads) {
    return vnnew internal::LinkPool(threads);
}

_vn_end
