//
//  vnBuffer.cpp
//  vnlib
//
//  Created by Wayne on 14-5-11.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnBuffer.h"
#include "vnMemory.h"
#include "vnAssert.h"

#include <cstring>

_vn_begin

BufferStream::BufferStream(BufferPtr buffer)
: m_buffer(std::move(buffer)) {
    vnassert(m_buffer);
}

const BufferPtr & BufferStream::buffer() const {
	return m_buffer;
}

size_t BufferStream::read(void *data, size_t size) {
    size_t ret = m_buffer->read(m_offset, data, size);
    m_offset += ret;
	return ret;
}

size_t BufferStream::write(const void *data, size_t size) {
	size_t ret = m_buffer->write(m_offset, data, size);
	m_offset += ret;
	return ret;
}

bool BufferStream::seek(s64 offset, SeekDir way) {
	s64 pos;
    s64 size = m_buffer->size();
	switch (way) {
		case kBegin:
			pos = offset;
			break;
		case kCurrent:
			pos = m_offset + offset;
			break;
		case kEnd:
			pos = size + offset;
			break;
        default:
            return false;
	}
	if (pos < 0 || pos >size) {
		return false;
	}
	m_offset = (size_t)pos;
	return true;
}

s64 BufferStream::tell() {
	return m_offset;
}

///////////////////////////////////////////////////////

FixedBuffer::FixedBuffer(size_t size, bool extraNull)
: m_data(0)
, m_size(size)
, m_extraNull(extraNull) {
	vnassert(size != 0);
    if (extraNull) {
        auto p = vnmalloc(u8, size + 1);
        p[size] = 0;
        m_data = p;
    } else {
        m_data = vnmalloc(u8, size);
    }
}

FixedBuffer::FixedBuffer(const FixedBuffer &o)
: m_data(0)
, m_size(o.m_size)
, m_extraNull(o.m_extraNull) {
    if (m_extraNull) {
        auto p = vnmalloc(u8, m_size + 1);
        p[m_size] = 0;
        m_data = p;
    } else {
        m_data = vnmalloc(u8, m_size);
    }
    memcpy(m_data, o.m_data, m_size);
} 

FixedBuffer::~FixedBuffer() {
	vnfree(m_data);
	m_data = 0;
    m_size = 0;
}

FixedBuffer * FixedBuffer::clone() const {
    return vnnew FixedBuffer(*this);
}

void * FixedBuffer::lock() {
	return m_data;
}

void FixedBuffer::unlock() {
	
}

size_t FixedBuffer::size() const {
	return m_size;
}

size_t FixedBuffer::read(size_t offset, void *data, size_t size) const {
    if (offset >= m_size || !size) {
        return 0;
    }
    size_t t = m_size - offset;
    if (size > t) {
        size = t;
    }
    memcpy(data, (u8 *)m_data + offset, size);
    return size;
}

size_t FixedBuffer::write(size_t offset, const void *data, size_t size) {
	if (offset >= m_size || !size) {
		return 0;
	}
	size_t t = m_size - offset;
	if (size > t) {
		size = t;
	}
	memcpy((u8 *)m_data + offset, data, size);
	return size;
}

ProxyBuffer::ProxyBuffer(void *data, size_t size)
: m_data(data)
, m_size(size) {
	vnassert(data && size);
}

ProxyBuffer * ProxyBuffer::clone() const {
    return vnnew ProxyBuffer(*this);
}

void * ProxyBuffer::lock() {
	return m_data;
}

void ProxyBuffer::unlock() {
	
}

size_t ProxyBuffer::size() const {
	return m_size;
}

size_t ProxyBuffer::read(size_t offset, void *data, size_t size) const {
    if (offset >= m_size || !size) {
        return 0;
    }
    size_t t = m_size - offset;
    if (size > t) {
        size = t;
    }
    memcpy(data, (u8 *)m_data + offset, size);
    return size;
}

size_t ProxyBuffer::write(size_t offset, const void *data, size_t size) {
	if (offset >= m_size || !size) {
		return 0;
	}
	size_t t = m_size - offset;
	if (size > t) {
		size = t;
	}
	memcpy((u8 *)m_data + offset, data, size);
	return size;
}

_vn_end
