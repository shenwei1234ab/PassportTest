//
//  vnLinkBuffer.cpp
//  vnlib
//
//  Created by Wayne on 4/28/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "vnLinkBuffer.h"
#include "vnSingleton.h"
#include "vnMemoryPool.h"

#include <cstring>

_vn_begin

namespace internal {
    
    class LinkBufferPool : public ObjectHeapMemPool<LinkBuffer>, public Singleton<LinkBufferPool> {

    };

}
LinkBuffer::LinkBuffer() {
	memset(m_data, 0, kStoregeSize);
}
void * LinkBuffer::lock() {
    return m_data + m_offset;
}

void LinkBuffer::unlock() {
    
}

size_t LinkBuffer::size() const {
    return m_size;
}

size_t LinkBuffer::capacity() const {
    return kStoregeSize - m_offset;
}

size_t LinkBuffer::offset() const {
    return m_offset;
}

size_t LinkBuffer::offset(size_t _offset) {
    if (_offset > kStoregeSize) {
        _offset = kStoregeSize;
    }
    ssize_t size = m_size;
    size += (ssize_t)m_offset - (ssize_t)_offset;
    if (size < 0) {
        m_size = 0;
    } else {
        m_size = size;
    }
    return m_offset = _offset;
}

size_t LinkBuffer::read(size_t offset, void *data, size_t size) const {
    if (offset >= m_size || !size) {
        return 0;
    }
    size_t rest = m_size - offset;
    if (size > rest) {
        size = rest;
    }
    memcpy(data, (u8 *)m_data + m_offset + offset, size);
    return size;
}

size_t LinkBuffer::write(size_t offset, const void *data, size_t size) {
    size_t c = kStoregeSize - m_offset;
    if (offset >= c || !size) {
        return 0;
    }
    size_t t = c - offset;
    if (size > t) {
        size = t;
    }
    memcpy((u8 *)m_data + m_offset + offset, data, size);
    t = offset + size;
    if (m_size < t) {
        m_size = t;
    }
    return size;
}

void LinkBuffer::resize(size_t size) {
    size_t c = kStoregeSize - m_offset;
    if (size > c) {
        m_size = c;
    } else {
        m_size = size;
    }
}

void LinkBuffer::clear() {
    m_size = 0;
}

LinkBufferPtr LinkBuffer::next() const {
    return m_next;
}

void LinkBuffer::link_next(LinkBufferPtr _buf) {
    auto t = std::move(m_next);
    m_next = std::move(_buf);
    if (m_next) {
        m_next->m_next = std::move(t);
    }
}

LinkBufferPtr LinkBuffer::unlink_next() {
    return std::move(m_next);
}

void * LinkBuffer::tag() const {
    return m_tag;
}

void LinkBuffer::tag(void *_tag) {
    m_tag = _tag;
}

LinkBufferPtr LinkBuffer::_tail() {
    auto p = this;
    while (p->m_next) {
        p = p->m_next.get();
    }
    return {p, true};
}

LinkBuffer * LinkBuffer::_tail_ptr() {
    auto p = this;
    while (p->m_next) {
        p = p->m_next.get();
    }
    return p;
}

size_t LinkBuffer::_total_size() const {
    size_t ret = m_size;
    auto p = m_next.get();
    while (p) {
        ret += p->m_size;
        p = p->m_next.get();
    }
    return ret;
}

LinkBufferPtr LinkBuffer::create() {
    return new(internal::LinkBufferPool::instance().alloc()) LinkBuffer();
}

void LinkBuffer::_release() const {
    this->~LinkBuffer();
    internal::LinkBufferPool::instance().free((void *)this);
}

size_t LinkBuffer::pool_total() {
    return internal::LinkBufferPool::instance().total();
}

size_t LinkBuffer::pool_unused() {
    return internal::LinkBufferPool::instance().unused();
}

size_t LinkBuffer::pool_maxUnused() {
    return internal::LinkBufferPool::instance().maxUnused();
}

void LinkBuffer::pool_setMaxUnused(size_t _max) {
    return internal::LinkBufferPool::instance().setMaxUnused(_max);
}


_vn_end
