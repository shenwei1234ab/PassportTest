//
//  vnOpenSSL_BIO.h
//  vnlib
//
//  Created by Wayne on 6/18/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef vnlib_vnOpenSSL_BIO_h
#define vnlib_vnOpenSSL_BIO_h

#include "vnLinkBuffer.h"

_vn_begin

class OpenSSL_BIO {
public:
    int bio_read(char *buf, int size);
    virtual int bio_write(const char *buf, int size) = 0;
    bool bio_readable() const;
protected:
    static void * bio_method();
    void bio_append(LinkBufferPtr buffers);
private:
    size_t m_offset = 0, m_size = 0, m_total = 0;
    LinkBufferPtr m_head;
    LinkBuffer *m_tail = nullptr;
    
    
    
};

_vn_end

#endif
