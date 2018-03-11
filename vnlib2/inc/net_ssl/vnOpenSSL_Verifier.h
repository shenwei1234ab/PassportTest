//
//  vnOpenSSL_Verifier.h
//  vnlib
//
//  Created by Wayne on 8/16/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef vnlib_vnOpenSSL_Verifier_h
#define vnlib_vnOpenSSL_Verifier_h

#include "vnOpenSSL_Key.h"
#include "vnOpenSSL_Algorithms.h"

#include "vnDataStream.h"

_vn_begin

class OpenSSL_Verifier : public DataStream {
public:
    explicit OpenSSL_Verifier(OpenSSL_algo algo = kOpenSSL_algo_sha1);
    ~OpenSSL_Verifier();
    
    virtual size_t read(void *buffer, size_t size) override;
    virtual size_t write(const void *buffer, size_t size) override;
    virtual bool seek(s64 offset, SeekDir way) override;
    virtual s64 tell() override;
    
    bool verify(OpenSSL_KeyPtr key, const void *sign, size_t sign_size);
    bool verify_base64(OpenSSL_KeyPtr key, const str8 &sign);
    
protected:
    s64 m_processed = 0;
    void *m_ctx;
};

_vn_end
#endif
