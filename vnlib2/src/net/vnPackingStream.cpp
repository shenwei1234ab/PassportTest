//
//  vnPackingStream.cpp
//  vnlib
//
//  Created by Wayne on 23/5/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnPackingStream.h"


_vn_begin

PackingStream::PackingStream() {
    m_head = LinkBuffer::create();
    m_tail = m_head.get();
    m_tail->offset(9);
}

size_t PackingStream::read(void *buffer, size_t size) {
    return 0;
}

size_t PackingStream::write(const void *buffer, size_t size) {
    size_t remaining = size;
    const u8 *ptr = (const u8 *)buffer;
    while (remaining) {
        size_t bytes = m_tail->write(m_offset, ptr, remaining);
        remaining -= bytes;
        ptr += bytes;
        m_offset += bytes;
        if (m_offset == m_tail->capacity()) {
            auto p = LinkBuffer::create().release();
            m_tail->link_next(p);
            m_tail = p;
            m_offset = 0;
        }
    }
    m_size += size;
    return size;
}

bool PackingStream::seek(s64 offset, SeekDir way) {
    return false;
}

s64 PackingStream::tell() {
    return m_size;
}

LinkBufferPtr PackingStream::pack(void *tail_tag) {
    LinkBufferPtr head = std::move(m_head);
    size_t s = _compressedDataSizeUInt(m_size);
    head->offset(9 - s);
    BufferStream(head) << DataStream::ccu(m_size);
    m_tail->tag(tail_tag);
    
    m_head = LinkBuffer::create();
    m_tail = m_head.get();
    m_tail->offset(9);
    m_offset = 0;
    m_size = 0;
    
    return std::move(head);
}








_vn_end
