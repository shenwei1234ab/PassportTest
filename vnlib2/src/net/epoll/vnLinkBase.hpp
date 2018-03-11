//
//  vnLinkBase.cpp
//  vnlib
//
//  Created by Wayne on 4/28/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "vnLinkBase.h"

#include <mutex>

#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>


_vn_begin

namespace internal {
    
    LinkBase::LinkBase(RefCountedPtr<LinkPool> pool) : m_pool(std::move(pool)), m_holder(vnnew LinkBaseHolder(this)) {

    }
        
    LinkBase::~LinkBase() {
        m_holder->detach();
        if (m_sockfd != -1) {
            ::close(m_sockfd);
        }
    }

    void LinkBase::set_nonblocking() {
        int flags = ::fcntl(m_sockfd, F_GETFL, 0);
        if (flags < 0) {
            return ;
        }
        ::fcntl(m_sockfd, F_SETFL, flags | O_NONBLOCK);
    }

    bool LinkBase::bind(const NetAddress &addr) {
        ::sockaddr_in sa;
        sa.sin_family = AF_INET;
        sa.sin_addr.s_addr = addr.ip;
        sa.sin_port = htons(addr.port);
        if (::bind(m_sockfd, (sockaddr *)&sa, sizeof(sa))) {
            return false;
        }
        return true;
    }
        
    void LinkBase::post(std::function<void()> fn) {
    		struct Holder {
    			RefCountedPtr<LinkBase> obj;
    			std::function<void()> fn;
    			Holder(RefCountedPtr<LinkBase> &&_obj, std::function<void()> &&_fn) : obj(std::move(_obj)), fn(std::move(_fn)) {}
    			Holder(Holder &&o) : obj(std::move(o.obj)), fn(std::move(o.fn)) {}
    			Holder(const Holder &o) : obj(o.obj), fn(o.fn) {}
    			void operator()() const {
    				fn();
    			}
    		};
    		if (fn) {
    			m_pool->post(Holder({this, true}, std::move(fn)));
    		}
    }

    LinkBaseHolder::LinkBaseHolder(LinkBase *link)
    : m_link(link) {

    }

    bool LinkBaseHolder::ack(bool read, bool write, bool error) {
        std::unique_lock<SpinLock> lck(m_lock);
        if (!m_link) {
            return false;
        }
        m_link->ack(read, write, error);
        return true;
    }

    void LinkBaseHolder::detach() {
        std::unique_lock<SpinLock> lck(m_lock);
        m_link = nullptr;
    }
    
} // namespace internal

_vn_end
