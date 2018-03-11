//
//  vnHttpResponseParser.cpp
//  vnlib
//
//  Created by Wayne on 8/13/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "vnHttpResponseParser.h"

#include <cstring>

_vn_begin

void HttpResponseParser::append(LinkBufferPtr buffers) {
    auto p = buffers.release();
    m_size += p->_total_size();
    if (m_buffers) {
        m_buffers->_tail_ptr()->link_next(std::move(buffers));
    } else {
        m_buffers = p;
    }
}

HttpResponseParser::State HttpResponseParser::fetch() {
    switch (m_state) {
        case 0:
            if (_fetch_line()) {
                auto p = m_line.c_str();
                auto s = ::strchr(p, ' ');
                if (!s) {
                    return kError;
                }
                str8 t1(p, s);
                	p = t1.c_str();
                //*s = 0;
                if (::strcmp(p, "http/1.1") && ::strcmp(p, "http/1.0")) {
                    return kError;
                }
                p = s + 1;
                s = ::strchr(p, ' ');
                if (!s) {
                    return kError;
                }
                u32 status = 0;
                if (!sscanf(p, "%u", &status)) {
                    return kError;
                }
                m_result.set_status(status);
                m_result.set_status_text(s + 1);
                m_line.clear();
                m_state = 1;
                m_content_chunked = false;
                m_content_length = 0;
            } else {
                break;
            }
            
        case 1:
            while (_fetch_line()) {
                if (m_line.empty()) {
                    if (m_content_chunked) {
                        m_result.set_content({});
                        m_state = 3;
                        goto l_case3;
                    } else if (m_content_length == 0) {
                        m_result.set_content({});
                        m_state = 0;
                        return kDone;
                    } else if (_fetch_content()) {
                        m_state = 0;
                        return kDone;
                    } else {
                        m_state = 2;
                        break;
                    }
                } else {
                    auto p = m_line.c_str();
                    auto s = ::strchr(p, ':');
                    if (!s) {
                        return kError;
                    }
                    //*s = 0;
							str8 key(p, s);
							p = key.c_str();
                    for (++s; *s == ' '; ++s);
                    
                    if (!::strcmp(p, "content-type")) {
                        m_result.set_content_type(s);
                    } else if (!::strcmp(p, "content-length")) {
                        if (!sscanf(s, "%lu", &m_content_length)) {
                            return kError;
                        }
                        if (m_content_length) {
                            m_content_remaining = m_content_length;
                            m_result.set_content(vnnew FixedBuffer(m_content_length, true));
                        }
                    } else if (!::strcmp(p, "transfer-encoding") && !::strcmp(s, "chunked")) {
                        m_content_chunked = true;
                        m_content_length = 0;
                        m_content_remaining = 0;
                    }
                    m_line.clear();
                }
            }
            break;
            
        case 2:
            if (_fetch_content()) {
                m_state = 0;
                return kDone;
            }
            break;
            
        case 3:
        l_case3:
            if (!_fetch_line()) {
                break;
            }
            if (!sscanf(m_line.c_str(), "%lX", &m_content_remaining)) {
                return kError;
            }
            m_line.clear();
            if (m_content_remaining) {
                m_state = 4;
                m_content_length += m_content_remaining;
                m_chunks.push_back(vnnew FixedBuffer(m_content_remaining, true));
            } else {
                if (m_content_length) {
                    if (m_chunks.size() == 1) {
                        m_result.set_content(std::move(m_chunks.front()));
                    } else {
                        auto content = vnnew FixedBuffer(m_content_length, true);
                        m_result.set_content(content);
                        size_t off = 0;
                        for (auto &i : m_chunks) {
                            size_t s = i->size();
                            content->write(off, i->lock(), s);
                            i->unlock();
                            off += s;
                        }
                    }
                    m_chunks.clear();
                }
                m_state = 6;
                goto l_case6;
            }
            
            
        case 4:
            if (!_fetch_chunk()) {
                break;
            }
            m_state = 5;
            
        case 5:
            if (!_fetch_line()) {
                break;
            }
            m_line.clear();
            m_state = 3;
            goto l_case3;
            
        case 6:
        l_case6:
            if (!_fetch_line()) {
                break;
            }
            m_line.clear();
            m_state = 0;
            return kDone;
    }
    return kWait;
}

bool HttpResponseParser::_fetch_line() {
    c8 chr = 0;
    while (_fetch_char(chr)) {
        if (chr == '\r') {
            continue;
        }
        if (chr == '\n') {
            return true;
        }
        m_line.push_back(chr);
    }
    return false;
}

bool HttpResponseParser::_fetch_char(c8 &c) {
    if (m_size == 0) {
        return false;
    }
    c = static_cast<c8 *>(m_buffers->lock())[m_offset];
    m_buffers->unlock();
    ++m_offset;
    --m_size;
    if (m_offset == m_buffers->size()) {
        m_offset = 0;
        m_buffers = m_buffers->next();
    }
    if (c >= 'A' && c <= 'Z') {
        c += 'a' - 'A';
    }
    return true;
}

bool HttpResponseParser::_fetch_content() {
    while (m_content_remaining) {
        if (m_size == 0) {
            return false;
        }
        auto p = static_cast<u8 *>(m_buffers->lock()) + m_offset;
        auto len = m_buffers->size() - m_offset;
        auto content = m_result.content().get();
        if (m_content_remaining < len) {
            content->write(m_content_length - m_content_remaining, p, m_content_remaining);
            m_size -= m_content_remaining;
            m_offset += m_content_remaining;
            m_content_remaining = 0;
            break;
        }
        
        content->write(m_content_length - m_content_remaining, p, len);
        m_content_remaining -= len;
        m_offset = 0;
        m_size -= len;
        m_buffers = m_buffers->next();
    }
    return true;
}

bool HttpResponseParser::_fetch_chunk() {
    while (m_content_remaining) {
        if (m_size == 0) {
            return false;
        }
        auto p = static_cast<u8 *>(m_buffers->lock()) + m_offset;
        auto len = m_buffers->size() - m_offset;
        auto chunk = m_chunks.back().get();
        if (m_content_remaining < len) {
            chunk->write(chunk->size() - m_content_remaining, p, m_content_remaining);
            m_size -= m_content_remaining;
            m_offset += m_content_remaining;
            m_content_remaining = 0;
            break;
        }
        
        chunk->write(chunk->size() - m_content_remaining, p, len);
        m_content_remaining -= len;
        m_offset = 0;
        m_size -= len;
        m_buffers = m_buffers->next();
    }
    return true;
}

_vn_end
