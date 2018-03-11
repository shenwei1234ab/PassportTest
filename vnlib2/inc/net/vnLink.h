//
//  vnLink.h
//  vnlib
//
//  Created by Wayne on 4/28/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef vnlib_vnLink_h
#define vnlib_vnLink_h

#include "vnNetAddress.h"
#include "vnLinkBuffer.h"
#include "vnSpinLock.h"

#include <functional>

_vn_begin

namespace internal {
    class Link_TCP_Listen;
    class Link_TCP_Connection;
}

class Link_TCP_Connection;

class Link_TCP_Listen : public NonCopyable {
    friend class internal::Link_TCP_Listen;
public:
    void close();
    bool post(std::function<void()> fn);
    void traverseAccepted(std::function<bool(Link_TCP_Connection *)> fn);
    
protected:
    virtual void _init() {}
    virtual void _destroy() = 0;
    virtual Link_TCP_Connection * _accepted(const NetAddress &peer) = 0;
    virtual void _error_accepting(int err) {}
private:
    internal::Link_TCP_Listen *m_core = nullptr;
    mutable SpinLock m_core_lock;
};

class Link_TCP_Connection : public NonCopyable {
    friend class internal::Link_TCP_Connection;
public:
    bool post(std::function<void()> fn);
    bool connected() const;
    NetAddress peer() const;
    void send(LinkBufferPtr buffers);
    void close();
    int sockfd();
protected:
    virtual void _init() {}
    virtual void _destroy() = 0;
    virtual void _connected() {}
    virtual void _error_connecting() {}
    virtual void _disconnected() {}
    virtual void _sent(size_t bytes, void *tag) {}
    virtual void _received(LinkBufferPtr buffers) {}
private:
    internal::Link_TCP_Connection *m_core = nullptr;
    mutable SpinLock m_core_lock;
};

class LinkPool {
public:
    static LinkPool * create(size_t threads = 0);
    virtual void release() = 0;
    
    virtual Link_TCP_Listen * listen(const std::function<Link_TCP_Listen *()> &creator, const NetAddress &addr, bool reuseAddr = false, bool manageAccepted = true) = 0;
    virtual Link_TCP_Connection * connect(const std::function<Link_TCP_Connection *()> &creator, const NetAddress &addr) = 0;
};

_vn_end

#endif
