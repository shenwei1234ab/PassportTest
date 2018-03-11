//
//  vnHttpRequestParser.cpp
//  vnlib
//
//  Created by Wayne on 8/12/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "vnHttpRequestParser.h"

#include <cstring>

_vn_begin

void HttpRequestParser::append(LinkBufferPtr buffers) {
    auto p = buffers.release();
    m_size += p->_total_size();
    if (m_buffers) {
        m_buffers->_tail_ptr()->link_next(std::move(buffers));
    } else {
        m_buffers = p;
    }
}

HttpRequestParser::State HttpRequestParser::fetch() {
    switch (m_state) {
        case 0:
            if (_fetch_line()) {
                auto p = (c8 *)m_line.c_str();
                auto s = ::strchr(p, ' ');
                if (!s) {
                    return kError;
                }
                *s = 0;
                if (!::strcmp(p, "GET")) {
                    m_result.set_method(HttpRequest::kMethod_GET);
                } else if (!::strcmp(p, "POST")) {
                    m_result.set_method(HttpRequest::kMethod_POST);
                } else {
                    return kError;
                }
                p = s + 1;
                s = ::strchr(p, ' ');
                if (!s) {
                    return kError;
                }
                m_result.set_url({p, (size_t)(s - p)});
                p = s + 1;
                if (::strcmp(p, "HTTP/1.1")) {
                    return kError;
                }
                m_line.clear();
                m_state = 1;
            } else {
                break;
            }
            
        case 1:
            while (_fetch_line()) {
                if (m_line.empty()) {
                    if (m_content_remaining == 0) {
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
                    auto p = (c8 *)m_line.c_str();
                    auto s = ::strchr(p, ':');
                    if (!s) {
                        return kError;
                    }
                    *s = 0;
                    for (++s; *s == ' '; ++s);
                    
                    if (!::strcmp(p, "Host")) {
                        m_result.set_host(s);
                    } else if (!::strcmp(p, "Content-Type")) {
                        m_result.set_content_type(s);
                    } else if (!::strcmp(p, "Content-Length")) {
                        if (!sscanf(s, "%lu", &m_content_remaining)) {
                            return kError;
                        }
                        if (m_content_remaining) {
                            m_result.set_content(vnnew FixedBuffer(m_content_remaining, true));
                        }
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
    }
    
    return kWait;
}

bool HttpRequestParser::_fetch_line() {
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

bool HttpRequestParser::_fetch_char(c8 &c) {
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
    return true;
}

bool HttpRequestParser::_fetch_content() {
    while (m_content_remaining) {
        if (m_size == 0) {
            return false;
        }
        auto p = static_cast<u8 *>(m_buffers->lock()) + m_offset;
        auto len = m_buffers->size() - m_offset;
        auto content = m_result.content().get();
        if (m_content_remaining < len) {
            content->write(content->size() - m_content_remaining, p, m_content_remaining);
            m_size -= m_content_remaining;
            m_offset += m_content_remaining;
            m_content_remaining = 0;
            break;
        }
        
        content->write(content->size() - m_content_remaining, p, len);
        m_content_remaining -= len;
        m_offset = 0;
        m_size -= len;
        m_buffers = m_buffers->next();
    }
    return true;
}

_vn_end
