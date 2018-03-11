//
//  vnHttpRequestParser.h
//  vnlib
//
//  Created by Wayne on 8/12/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef vnlib_vnHttpRequestParser_h
#define vnlib_vnHttpRequestParser_h

#include "vnHttpRequest.h"

_vn_begin

class HttpRequestParser : public AllocatedObject {
public:
    enum State : int {
        kError = -1,
        kWait = 0,
        kDone = 1,
    };
    
    void append(LinkBufferPtr buffers);
    State fetch();
    const HttpRequest & result() { return m_result; }
    
    
protected:
    size_t m_size = 0, m_offset = 0, m_content_remaining = 0;
    LinkBufferPtr m_buffers;
    HttpRequest m_result;
    
    int m_state = 0;
    str8 m_line;
    
    bool _fetch_line();
    bool _fetch_char(c8 &c);
    bool _fetch_content();
};

_vn_end

#endif
