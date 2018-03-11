//
//  vnPackingStream.h
//  vnlib
//
//  Created by Wayne on 23/5/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnPackingStream_h
#define vnlib_vnPackingStream_h

#include "vnLinkBuffer.h"

#include <vector>

_vn_begin

class PackingStream : public DataStream, public AllocatedObject {
public:
    PackingStream();
    
    virtual size_t read(void *buffer, size_t size);
	virtual size_t write(const void *buffer, size_t size);
	virtual bool seek(s64 offset, SeekDir way = kBegin);
	virtual s64 tell();
    
    LinkBufferPtr pack(void *tail_tag = nullptr);
    
protected:
    size_t m_size = 0;
    size_t m_offset = 0;
    LinkBufferPtr m_head;
    LinkBuffer *m_tail = nullptr;
};

_vn_end

#endif
