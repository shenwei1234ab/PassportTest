//
//  vnOpenSSL_Key.h
//  vnlib
//
//  Created by Wayne on 8/16/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef vnlib_vnOpenSSL_Key_h
#define vnlib_vnOpenSSL_Key_h

#include "vnRefCounted.h"
#include "vnNonCopyable.h"

#include <vector>

_vn_begin

class OpenSSL_Key;

typedef RefCountedPtr<OpenSSL_Key> OpenSSL_KeyPtr;

class OpenSSL_Key : public RefCounted, public NonCopyable {
public:
    static OpenSSL_KeyPtr create_public(const void *data, size_t size);
    static OpenSSL_KeyPtr create_private(const void *data, size_t size, const c8 *passphrase = nullptr);
    
    OpenSSL_Key(OpenSSL_Key &&);
    virtual ~OpenSSL_Key();
    
    void * value() const { return m_key; }
    
    enum RSA_Padding {
        kRSA_PKCS1_Padding,
        kRSA_PKCS1_OAEP_Padding,
        kRSA_SSLV23_Padding,
        kRSA_NoPadding,
    };
    
    std::vector<u8> rsa_public_encrypt(const void *data, size_t size, RSA_Padding padding = kRSA_PKCS1_Padding);
    std::vector<u8> rsa_public_decrypt(const void *data, size_t size, RSA_Padding padding = kRSA_PKCS1_Padding);
    std::vector<u8> rsa_private_encrypt(const void *data, size_t size, RSA_Padding padding = kRSA_PKCS1_Padding);
    std::vector<u8> rsa_private_decrypt(const void *data, size_t size, RSA_Padding padding = kRSA_PKCS1_Padding);
protected:
    void *m_key = 0;
    
private:
    OpenSSL_Key() = default;
};



_vn_end

#endif
