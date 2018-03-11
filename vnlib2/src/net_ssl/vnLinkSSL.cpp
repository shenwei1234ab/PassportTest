//
//  vnLinkSSL.cpp
//  vnlib
//
//  Created by Wayne on 6/18/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "vnLinkSSL.h"
#include "vnOpenSSL_Context.h"

#include <cstring>

_vn_begin

Link_TCP_Connection_SSL_Client::Link_TCP_Connection_SSL_Client() {
    auto p = OpenSSL_Context::instance().createSSL();
    m_ssl = p;
    BIO *bio = BIO_new((BIO_METHOD *)this->bio_method());
    bio->ptr = static_cast<OpenSSL_BIO *>(this);
    SSL_set_bio(p, bio, bio);
}

Link_TCP_Connection_SSL_Client::~Link_TCP_Connection_SSL_Client() {
    SSL_free((SSL *)m_ssl);
}

void Link_TCP_Connection_SSL_Client::ssl_send(const void *data, size_t size) {
    if (m_ssl_connected) {
        SSL_write((SSL *)m_ssl, data, (int)size);
    }
}

void Link_TCP_Connection_SSL_Client::ssl_send(LinkBufferPtr buffers) {
    if (!m_ssl_connected) {
        return ;
    }
    while (buffers) {
        SSL_write((SSL *)m_ssl, buffers->lock(), (int)buffers->size());
        buffers = buffers->next();
    }
}

void Link_TCP_Connection_SSL_Client::ssl_shutdown() {
    if (m_ssl_connected) {
        SSL_shutdown((SSL *)m_ssl);
    }
}

bool Link_TCP_Connection_SSL_Client::ssl_connected() const {
    return m_ssl_connected;
}

int Link_TCP_Connection_SSL_Client::bio_write(const char *buf, int size) {
    LinkBufferPtr head = LinkBuffer::create();
    LinkBuffer *tail = head.get();
    size_t _size = (size_t)size;
    while (_size) {
        auto n = tail->capacity();
        if (n >= _size) {
            memcpy(tail->lock(), buf, _size);
            tail->resize(_size);
            break;
        }
        memcpy(tail->lock(), buf, n);
        tail->resize(n);
        _size -= n;
        buf += n;
        auto p = LinkBuffer::create().release();
        tail->link_next(p);
        tail = p;
    }
    this->send(std::move(head));
    return size;
}

void Link_TCP_Connection_SSL_Client::_connected() {
    SSL_connect((SSL *)m_ssl);
}

void Link_TCP_Connection_SSL_Client::_received(LinkBufferPtr buffers) {
    this->bio_append(std::move(buffers));
    while (this->bio_readable()) {
        if (m_ssl_connected) {
            auto buf = LinkBuffer::create();
            auto ret = SSL_read((SSL *)m_ssl, buf->lock(), (int)buf->capacity());
            if (ret > 0) {
                buf->resize((size_t)ret);
                this->_ssl_received(std::move(buf));
            } else if (ret == 0) {
                m_ssl_connected = false;
                this->_ssl_disconnected();
                break;
            }
        } else {
            auto ret = SSL_connect((SSL *)m_ssl);
            if (ret > 0) {
                m_ssl_connected = true;
                this->_ssl_connected();
            } else if (ret == 0) {
                this->_ssl_error_connecting(SSL_get_error((SSL *)m_ssl, 0));
                break;
            }
        }
    }
}

_vn_end
