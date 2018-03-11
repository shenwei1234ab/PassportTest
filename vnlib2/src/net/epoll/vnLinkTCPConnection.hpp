//
//  vnLinkTCPConnection.cpp
//  vnlib
//
//  Created by Wayne on 4/29/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "vnLinkTCPConnection.h"
#include "vnLog.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

_vn_begin

namespace internal {

    Link_TCP_Connection::~Link_TCP_Connection() {
        if (m_manager) {
            m_manager->_remove_accepted(this);
        }
        if (m_handler) {
            m_handler->_destroy();
        }
    }
    
    void Link_TCP_Connection::attach(int sockfd, const NetAddress &peer, _vn_ns::Link_TCP_Connection *handler, RefCountedPtr<Link_TCP_Listen> manager) {
        m_sockfd = sockfd;
        m_peer = peer;
        m_handler = handler;
        m_manager = std::move(manager);
        this->set_nonblocking();
        m_handler->m_core = this;
        m_state = kConnected;
        this->post([this](){
            m_handler->_init();
            m_handler->_connected();
        });
        m_pool->reg(this);
    }
    
    bool Link_TCP_Connection::create(const std::function<_vn_ns::Link_TCP_Connection *()> &creator, const NetAddress &addr) {
        
        m_sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (m_sockfd < 0) {
            VN_LOG_ERROR("create socket failed: " << (int)errno);
            return false;
        }

        this->set_nonblocking();

        m_handler = creator();
        if (!m_handler) {
            return false;
        }
        m_handler->m_core = this;
        m_state = kConnecting;
        this->post(std::bind(&_vn_ns::Link_TCP_Connection::_init, m_handler));
        
        m_peer = addr;
        sockaddr_in sa;
        sa.sin_family = AF_INET;
        sa.sin_addr.s_addr = addr.ip;
        sa.sin_port = htons(addr.port);
        if (::connect(m_sockfd, (sockaddr *)&sa, sizeof(sa))) {
            if (errno != EINPROGRESS) {
                m_state = kDisconnected;
                this->post(std::bind(&_vn_ns::Link_TCP_Connection::_error_connecting, m_handler));
                return true;
            }
            return m_pool->reg(this);
        }
        m_state = kConnected;
        this->post(std::bind(&_vn_ns::Link_TCP_Connection::_connected, m_handler));
        return m_pool->reg(this);
    }
    
    void Link_TCP_Connection::ack(bool read, bool write, bool error) {
        m_handler->m_core_lock.lock();
        if (!m_handler->m_core) {
            m_handler->m_core_lock.unlock();
            return ;
        }
        this->post(std::bind(&Link_TCP_Connection::_ack, this, read, write, error));
        m_handler->m_core_lock.unlock();
    }

    void Link_TCP_Connection::_ack(bool read, bool write, bool error) {
        if (error) {
            if (m_state == kConnecting) {
                m_state = kDisconnected;
                m_handler->_error_connecting();
                return ;
            }
        }
        if (read) {
            if (m_state == kConnected) {
                _recv();
            }
        }
        if (write) {
            if (m_state == kConnecting) {
                m_state = kConnected;
                m_handler->_connected();
            } else if (m_state == kConnected) {
                if (m_sending_waiting) {
                    m_sending_waiting = false;
                    _send();
                }
            }
        }
    }

    void Link_TCP_Connection::send(LinkBufferPtr buffers) {
        if (m_state != kConnected) {
            return ;
        }
        this->post(std::bind(&Link_TCP_Connection::_send_append, this, std::move(buffers)));
    }
    
    void Link_TCP_Connection::close() {
        m_holder->detach();
        m_handler->m_core = nullptr;
        this->post(std::bind(&Link_TCP_Connection::_close, this));
    }
    
    void Link_TCP_Connection::_recv() {
        LinkBufferPtr head;
        LinkBuffer *last = nullptr;
        bool eof = false;
        for(;;) {
            auto buf = LinkBuffer::create();
            long ret = ::recv(m_sockfd, buf->lock(), buf->capacity(), 0);
            if (ret < 0) {
                int err = errno;
                if (err != EAGAIN && err != EWOULDBLOCK) {
                    //VN_LOG_DEBUG("sock_fd(" << m_sockfd << ") recv error(" << err << ")");
                    eof = true;
                }
                break;
            } else if (ret == 0) {
                eof = true;
                break;
            }
            buf->resize(ret);
            if (last) {
                auto p = buf.get();
                last->link_next(std::move(buf));
                last = p;
            } else {
                last = buf.get();
                head = std::move(buf);
            }
        }
        if (head) {
            m_handler->_received(std::move(head));
        }
        if (eof) {
            m_state = kDisconnected;
            m_handler->_disconnected();
        }
    }
    
    void Link_TCP_Connection::_close() {
        switch (m_state) {
            case kConnecting:
                break;
                
            case kConnected:
                ::shutdown(m_sockfd, SHUT_RDWR);
                break;
                
            case kDisconnected:
                
                break;
        }
        m_pool->unreg(this);
        this->drop();
    }

    void Link_TCP_Connection::_send() {
        while (m_sending_head) {
            auto p = m_sending_head.get();
            auto size = p->size() - m_sending_offset;
            auto ret = ::send(m_sockfd, (u8 *)p->lock() + m_sending_offset, size, 0);
            if (ret < 0) {
                if (errno == EAGAIN) {
                    m_sending_waiting = true;
                } else {
                    m_sending_head.reset();
                    m_sending_tail.reset();
                }
                return;
            }
            m_sending_offset += ret;
            size -= ret;
            if (!size) {
                m_handler->_sent(m_sending_offset, p->tag());
                m_sending_head = p->next();
                m_sending_offset = 0;
            }
        }
        m_sending_tail.reset();
    }
    
    void Link_TCP_Connection::_send_append(LinkBufferPtr buffers) {
        if (m_state != kConnected) {
            return ;
        }
        
        if (m_sending_tail) {
            auto p = buffers.release();
            m_sending_tail->link_next(p);
            m_sending_tail = p->_tail();
        } else {
            m_sending_head = std::move(buffers);
            m_sending_tail = m_sending_head->_tail();
        }
        
        if (!m_sending_waiting) {
            this->_send();
        }
    }
}

_vn_end
