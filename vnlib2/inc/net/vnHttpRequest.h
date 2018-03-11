//
//  vnHttpRequest.h
//  vnlib
//
//  Created by Wayne on 8/12/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef vnlib_vnHttpRequest_h
#define vnlib_vnHttpRequest_h

#include "vnLinkBuffer.h"

_vn_begin

class HttpRequest : public AllocatedObject {
public:
    enum Method {
        kMethod_GET = 0,
        kMethod_POST,
    };
    
    Method method() const { return m_method; }
    void set_method(Method _method) { m_method = _method; }
    
    const str8 & url() const { return m_url; }
    void set_url(str8 _url) { m_url = std::move(_url); }
    
    const str8 & host() const { return m_host; }
    void set_host(str8 _host) { m_host = std::move(_host); }
    
    const BufferPtr & content() const { return m_content; }
    void set_content(BufferPtr _content) { m_content = std::move(_content); }
    
    const str8 & content_type() const { return m_content_type; }
    void set_content_type(str8 _content_type) { m_content_type = std::move(_content_type); }
    
    LinkBufferPtr pack(void *tail_tag = nullptr) const;
    
protected:
    Method m_method = kMethod_GET;
    str8 m_url;
    str8 m_host;
    BufferPtr m_content;
    str8 m_content_type;
};

_vn_end

#endif
