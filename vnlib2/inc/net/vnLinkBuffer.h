//
//  vnLinkBuffer.h
//  vnlib
//
//  Created by Wayne on 4/28/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef vnlib_vnLinkBuffer_h
#define vnlib_vnLinkBuffer_h

#include "vnBuffer.h"

_vn_begin

class LinkBuffer;
typedef RefCountedPtr<LinkBuffer> LinkBufferPtr;

class LinkBuffer : public Buffer, public NonCopyable {
public:
    
    enum { kStoregeSize = 8192 };
    
    static LinkBufferPtr create();
    static size_t pool_total();
    static size_t pool_unused();
    static size_t pool_maxUnused();
    static void pool_setMaxUnused(size_t _max);
    
    virtual LinkBuffer * clone() const { return nullptr; }
    virtual void * lock();
    virtual void unlock(); // do nothing.
    virtual size_t size() const;
    virtual size_t read(size_t offset, void *data, size_t size) const;
    virtual size_t write(size_t offset, const void *data, size_t size);
    
    void resize(size_t size);
    void clear();
    
    size_t offset(size_t _offset);
    size_t offset() const;
    size_t capacity() const;
    
    LinkBufferPtr next() const;
    void link_next(LinkBufferPtr _buf);
    LinkBufferPtr unlink_next();
    
    void * tag() const;
    void tag(void *_tag);
    
    LinkBufferPtr _tail();
    LinkBuffer * _tail_ptr();
    size_t _total_size() const;
protected:
    LinkBuffer() ;
    virtual void _release() const;
    
    u8 m_data[kStoregeSize];
    size_t m_size = 0;
    size_t m_offset = 0;
    
    LinkBufferPtr m_next;
    void *m_tag = nullptr;
};

_vn_end

#endif
