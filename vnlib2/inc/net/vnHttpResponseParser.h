//
//  vnHttpResponseParser.h
//  vnlib
//
//  Created by Wayne on 8/13/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef vnlib_vnHttpResponseParser_h
#define vnlib_vnHttpResponseParser_h

#include "vnHttpResponse.h"

#include <list>

_vn_begin

class HttpResponseParser : public AllocatedObject {
public:
    enum State : int {
        kError = -1,
        kWait = 0,
        kDone = 1,
    };
    
    void append(LinkBufferPtr buffers);
    State fetch();
    const HttpResponse & result() { return m_result; }
    
    
protected:
    size_t m_size = 0, m_offset = 0, m_content_length = 0, m_content_remaining = 0;
    LinkBufferPtr m_buffers;
    HttpResponse m_result;
    
    int m_state = 0;
    str8 m_line;
    
    bool m_content_chunked = false;
    std::list<BufferPtr> m_chunks;
    
    bool _fetch_line();
    bool _fetch_char(c8 &c);
    bool _fetch_content();
    bool _fetch_chunk();
};

_vn_end

#endif
