//
//  vnHttpResponse.h
//  vnlib
//
//  Created by Wayne on 8/12/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef vnlib_vnHttpResponse_h
#define vnlib_vnHttpResponse_h

#include "vnLinkBuffer.h"

_vn_begin

class HttpResponse : public AllocatedObject {
public:
    HttpResponse() = default;
    HttpResponse(const HttpResponse &);
    HttpResponse(HttpResponse &&);

    u32 status() const { return m_status; }
    void set_status(u32 _status) { m_status = _status; }
    
    const str8 & status_text() const { return m_status_text; }
    void set_status_text(str8 text) { m_status_text = std::move(text); }
    
    const BufferPtr & content() const { return m_content; }
    void set_content(BufferPtr _content) { m_content = std::move(_content); }
    
    const str8 & content_type() const { return m_content_type; }
    void set_content_type(str8 _content_type) { m_content_type = std::move(_content_type); }
    
    LinkBufferPtr pack(void *tail_tag = nullptr) const;
    
protected:
    u32 m_status = 0;
    str8 m_status_text;
    BufferPtr m_content;
    str8 m_content_type;
};

_vn_end

#endif
