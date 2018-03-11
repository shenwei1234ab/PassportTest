//
//  vnUnpackingStream.h
//  vnlib
//
//  Created by Wayne on 5/23/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnUnpackingStream_h
#define vnlib_vnUnpackingStream_h

#include "vnLinkBuffer.h"

#include <deque>

_vn_begin

class UnpackingStream : public DataStream, public AllocatedObject {
public:
    virtual size_t read(void *buffer, size_t size);
	virtual size_t write(const void *buffer, size_t size);
	virtual bool seek(s64 offset, SeekDir way = kBegin);
	virtual s64 tell();
    
    void append(LinkBufferPtr buffers);
    
    bool ready();
    size_t size();
    
    void lockState();
    void unlockState();
    
protected:
    void _update();
    void _read(void *data, size_t size);
    
    size_t m_offset = 0;
    size_t m_available = 0;
    size_t m_size = 0;
    int m_state = 0;
    bool m_stateLocked = false;
    BufferStream m_stream { vnnew FixedBuffer(16) };
    
    LinkBufferPtr m_head;
    LinkBuffer *m_tail = nullptr;
};

_vn_end

#endif
