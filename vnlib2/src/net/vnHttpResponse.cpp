//
//  vnHttpResponse.cpp
//  vnlib
//
//  Created by Wayne on 8/12/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "vnHttpResponse.h"

_vn_begin

HttpResponse::HttpResponse(const HttpResponse &o)
: m_status(o.m_status)
, m_status_text(o.m_status_text)
, m_content_type(o.m_content_type) {
    if (o.m_content) {
        m_content = o.m_content->clone();
    }
}

HttpResponse::HttpResponse(HttpResponse &&o)
: m_status(o.m_status)
, m_status_text(std::move(o.m_status_text))
, m_content(std::move(o.m_content))
, m_content_type(std::move(o.m_content_type)) {

}

LinkBufferPtr HttpResponse::pack(void *tail_tag) const {
    auto ret = LinkBuffer::create();
    auto p = ret.get();
    size_t off = 0;
    
    auto save = [&p, &off](const void *_p, size_t _s) {
        const u8 *_ptr = (const u8 *)_p;
        for (;;) {
            auto r = p->write(off, _ptr, _s);
            if (r == _s) {
                off += r;
                break;
            }
            _ptr += r;
            _s -= r;
            off = 0;
            auto _new = LinkBuffer::create().release();
            p->link_next(_new);
            p = _new;
        }
    };
    
    c8 buf[32];
    
    save("HTTP/1.1 ", 9);
    save(buf, (size_t)sprintf(buf, "%u", m_status));
    save(" ", 1);
    save(m_status_text.c_str(), m_status_text.length());
    save("\r\n", 2);
    if (m_content && m_content->size()) {
        save("Content-Length:", 15);
        save(buf, (size_t)sprintf(buf, "%lu", m_content->size()));
        if (!m_content_type.empty()) {
            save("\r\nContent-Type:", 15);
            save(m_content_type.c_str(), m_content_type.length());
        }
        save("\r\n\r\n", 4);
        save(m_content->lock(), m_content->size());
        m_content->unlock();
    } else {
        save("\r\n", 2);
    }
    
    p->tag(tail_tag);
    return std::move(ret);
}

_vn_end
