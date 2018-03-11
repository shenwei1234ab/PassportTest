//
//  vnLinkTCPListen.cpp
//  vnlib
//
//  Created by Wayne on 4/28/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "vnLinkTCPListen.h"
#include "vnLinkTCPConnection.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


_vn_begin
namespace internal {
    
    Link_TCP_Listen::~Link_TCP_Listen() {
        if (m_handler) {
            m_handler->_destroy();
        }
    }
    
    bool Link_TCP_Listen::create(const std::function<_vn_ns::Link_TCP_Listen *()> &creator, const NetAddress &addr, bool reuseAddr, bool manageAccepted) {
        
        m_sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (m_sockfd < 0) {
            return false;
        }
       
        this->set_nonblocking(); 
        
        if (reuseAddr) {
            int opt = 1;
            ::setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        }
        
        if (!bind(addr)) {
            return false;
        }
        
        if (::listen(m_sockfd, SOMAXCONN)) {
            return false;
        }
        m_handler = creator();
        if (!m_handler) {
            return false;
        }
        m_handler->m_core = this;
        m_handler->_init();
        if (!m_pool->reg(this)) {
            return false;
        }
        m_manageAccepted = manageAccepted;
        return true;
    }

    void Link_TCP_Listen::close() {
        m_handler->m_core = nullptr;
        m_pool->unreg(this);
        this->post(std::bind(&Link_TCP_Listen::_close, this));
    }

    void Link_TCP_Listen::traverseAccepted(std::function<bool(_vn_ns::Link_TCP_Connection *)> fn) {
        if (!m_manageAccepted) {
            return ;
        }
        m_acceptedLock.lock();
        for (auto &i : m_accepted) {
            auto handler = i->_handler();
            if (handler) {
                if (!fn(handler)) {
                    break;
                }
            }
        }
        m_acceptedLock.unlock();
    }
    
    void Link_TCP_Listen::ack(bool read, bool, bool) {
        if (!read) {
            return ;
        }
        m_handler->m_core_lock.lock();
        if (!m_handler->m_core) {
            m_handler->m_core_lock.unlock();
            return ;
        }
        for (;;) {
            sockaddr_in sa;
            socklen_t len = sizeof(sa);
            int sockfd = ::accept(m_sockfd, (sockaddr *)&sa, &len);
            if (sockfd < 0) {
                auto err = errno;
                if (err == EWOULDBLOCK) {
                    break;
                }
                m_handler->_error_accepting(err);
            } else {
                this->_accepted(sockfd, NetAddress(sa.sin_addr.s_addr, ntohs(sa.sin_port)));
            }
        }
        m_handler->m_core_lock.unlock();
    }
    
    void Link_TCP_Listen::_accepted(int sockfd, const NetAddress &peer) {
        auto handler = m_handler->_accepted(peer);
        if (!handler) {
            ::close(sockfd);
        } else {
            auto link = vnnew Link_TCP_Connection(m_pool);
            if (m_manageAccepted) {
                m_acceptedLock.lock();
                m_accepted.insert(link);
                m_acceptedLock.unlock();
                link->attach(sockfd, peer, handler, {this, true});
            } else {
                link->attach(sockfd, peer, handler, {});
            }
        }
    }
    
    void Link_TCP_Listen::_close() {
        if (m_manageAccepted) {
            m_acceptedLock.lock();
            for (auto &i : m_accepted) {
                auto handler = i->_handler();
                if (handler) {
                    handler->close();
                }
            }
            m_acceptedLock.unlock();
        }
        this->drop();
    }

    void Link_TCP_Listen::_remove_accepted(Link_TCP_Connection *accepted) {
        m_acceptedLock.lock();
        m_accepted.erase(accepted);
        m_acceptedLock.unlock();
    }
    
}
_vn_end
