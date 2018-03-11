//
//  vnLink.cpp
//  vnlib
//
//  Created by Wayne on 4/28/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "vnLink.h"

#include <mutex>

#if VN_PLATFORM == VN_PLATFORM_MAC || VN_PLATFORM == VN_PLATFORM_FREEBSD

#include "kqueue/vnLinkBase.hpp"
#include "kqueue/vnLinkTCPListen.hpp"
#include "kqueue/vnLinkTCPConnection.hpp"
#include "kqueue/vnLinkPool.hpp"

#elif VN_PLATFORM == VN_PLATFORM_LINUX

#include "epoll/vnLinkBase.hpp"
#include "epoll/vnLinkTCPListen.hpp"
#include "epoll/vnLinkTCPConnection.hpp"
#include "epoll/vnLinkPool.hpp"

#else

#endif

_vn_begin

void Link_TCP_Listen::close() {
    m_core_lock.lock();
    if (m_core) {
        m_core->close();
    }
    m_core_lock.unlock();
}

bool Link_TCP_Listen::post(std::function<void ()> fn) {
    std::lock_guard<SpinLock> lck(m_core_lock);
    if (m_core) {
        m_core->post(std::move(fn));
        return true;
    }
    return false;
}

void Link_TCP_Listen::traverseAccepted(std::function<bool(Link_TCP_Connection *)> fn) {
    if (!fn) {
        return ;
    }
    std::lock_guard<SpinLock> lck(m_core_lock);
    if (!m_core) {
        return ;
    }
    m_core->traverseAccepted(std::move(fn));
}

bool Link_TCP_Connection::connected() const {
    std::lock_guard<SpinLock> lck(m_core_lock);
    if (m_core) {
        return m_core->connected();
    }
    return false;
}

NetAddress Link_TCP_Connection::peer() const {
    std::lock_guard<SpinLock> lck(m_core_lock);
    if (m_core) {
        return m_core->peer();
    }
    return NetAddress();
}

void Link_TCP_Connection::send(LinkBufferPtr buffers) {
    std::lock_guard<SpinLock> lck(m_core_lock);
    if (m_core) {
        m_core->send(std::move(buffers));
    }
}

void Link_TCP_Connection::close() {
    std::lock_guard<SpinLock> lck(m_core_lock);
    if (m_core) {
        m_core->close();
    }
}

int Link_TCP_Connection::sockfd() {
	std::lock_guard<SpinLock> lck(m_core_lock);
	if (m_core) {
		return m_core->sockfd();
	}
	return -1;
}

bool Link_TCP_Connection::post(std::function<void ()> fn) {
    std::lock_guard<SpinLock> lck(m_core_lock);
    if (m_core) {
        m_core->post(std::move(fn));
        return true;
    }
    return false;
}

_vn_end
