//
//  vnMD5.h
//  vnlib
//
//  Created by Wayne on 8/13/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef vnlib_vnMD5_h
#define vnlib_vnMD5_h

#include "vnDataStream.h"

#include <vector>

_vn_begin

class MD5 : public DataStream {
public:
    MD5();
    
    std::vector<u8> fetch();
    str8 fetch_hex_uppercase();
    str8 fetch_hex_lowercase();
    
    virtual size_t read(void *buffer, size_t size) override;
    virtual size_t write(const void *buffer, size_t size) override;
    virtual bool seek(s64 offset, SeekDir way) override;
    virtual s64 tell() override;
    
    struct MD5_CTX {
        u32 count[2];
        u32 state[4];
        u8 buffer[64];
    };
private:
    MD5_CTX m_ctx;
    s64 m_processed = 0;
};

_vn_end

#endif
