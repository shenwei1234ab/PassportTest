//
//  vnOpenSSL_BIO.cpp
//  vnlib
//
//  Created by Wayne on 6/18/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "vnOpenSSL_BIO.h"

#include <openssl/bio.h>
#include <cstring>

_vn_begin

namespace internal {
    
    int _bio_new(BIO *h) {
        h->init = 1;
        h->num = 0;
        h->ptr = nullptr;
        h->flags = 0;
        //std::cout << "_bio_new" << std::endl;
        return 1;
    }
    
    int _bio_free(BIO *h) {
        //std::cout << "_bio_free" << std::endl;
        return 1;
    }
    
    int _bio_read(BIO *h, char *buf, int size) {
//        int ret = 0;
//        MySSL *obj = static_cast<MySSL *>(h->ptr);
//        if (obj->m_size) {
//            ret = (size < obj->m_size ? size : obj->m_size);
//            memcpy(buf, obj->m_data, ret);
//            obj->m_data += ret;
//            obj->m_size -= ret;
//        }
//        if (!ret) {
//            ret = -1;
//            BIO_set_retry_read(h);
//        }
//        return ret;
        auto p = static_cast<OpenSSL_BIO *>(h->ptr);
        if (p->bio_readable()) {
            return p->bio_read(buf, size);
        }
        BIO_set_retry_read(h);
        return -1;
    }
    
    int _bio_write(BIO *h, const char *buf, int num) {
        static_cast<OpenSSL_BIO *>(h->ptr)->bio_write(buf, num);
        return num;
    }
    
    long _bio_ctrl(BIO *h, int cmd, long arg1, void *arg2) {
        int ret = 0;
        switch (cmd) {
            case BIO_CTRL_FLUSH:
                ret = 1;
                break;
        }
        return ret;
    }
}

void * OpenSSL_BIO::bio_method() {
    static BIO_METHOD s_methods = {
        BIO_TYPE_SOURCE_SINK,
        "vn_bio",
        internal::_bio_write,
        internal::_bio_read,
        nullptr,
        nullptr,
        internal::_bio_ctrl,
        internal::_bio_new,
        internal::_bio_free,
        NULL
    };
    return &s_methods;
}

void OpenSSL_BIO::bio_append(LinkBufferPtr buffers) {
    auto p = buffers.release();
    m_total += p->_total_size();
    if (m_tail) {
        m_tail->link_next(p);
    } else {
        m_head = p;
        m_size = p->size();
    }
    m_tail = p->_tail_ptr();
}

bool OpenSSL_BIO::bio_readable() const {
    return m_total != 0;
}

int OpenSSL_BIO::bio_read(char *buf, int size) {
    size_t ret = 0, _size = (size_t)size;
    while (m_total && _size) {
        vnassert(m_head);
        if (m_size > _size) {
            memcpy(buf, (u8 *)m_head->lock() + m_offset, _size);
            m_size -= _size;
            m_total -= _size;
            m_offset += _size;
            ret += _size;
            break;
        }
        memcpy(buf, (u8 *)m_head->lock() + m_offset, m_size);
        _size -= m_size;
        m_total -= m_size;
        buf += m_size;
        ret += m_size;
        m_offset = 0;
        m_head = m_head->next();
        if (m_head) {
            m_size = m_head->size();
        } else {
            m_size = 0;
            m_tail = nullptr;
            vnassert(m_total == 0);
            break;
        }
    }
    return (int)ret;
}


_vn_end
