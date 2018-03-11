//
//  vnOpenSSL_Signer.h
//  vnlib
//
//  Created by Wayne on 8/16/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef vnlib_vnOpenSSL_Signer_h
#define vnlib_vnOpenSSL_Signer_h

#include "vnOpenSSL_Key.h"
#include "vnOpenSSL_Algorithms.h"

#include "vnDataStream.h"

#include <vector>

_vn_begin

class OpenSSL_Signer : public DataStream {
public:
    explicit OpenSSL_Signer(OpenSSL_algo algo = kOpenSSL_algo_sha1);
    ~OpenSSL_Signer();
    
    virtual size_t read(void *buffer, size_t size) override;
    virtual size_t write(const void *buffer, size_t size) override;
    virtual bool seek(s64 offset, SeekDir way) override;
    virtual s64 tell() override;
    
    std::vector<u8> sign(OpenSSL_KeyPtr key);
    str8 sign_base64(OpenSSL_KeyPtr key);
    
protected:
    s64 m_processed = 0;
    void *m_ctx;
};

_vn_end

#endif
