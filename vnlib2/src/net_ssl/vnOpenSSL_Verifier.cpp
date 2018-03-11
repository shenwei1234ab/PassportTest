//
//  vnOpenSSL_Verifier.cpp
//  vnlib
//
//  Created by Wayne on 8/16/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "vnOpenSSL_Verifier.h"

#include "vnBase64.h"

#include <openssl/evp.h>

_vn_begin

OpenSSL_Verifier::OpenSSL_Verifier(OpenSSL_algo algo)
: m_ctx(Memory::allocate(sizeof(EVP_MD_CTX), __FILE__, __LINE__, 0)) {
    const EVP_MD *type = nullptr;
    switch (algo) {
        case kOpenSSL_algo_sha1:
            type = EVP_sha1();
            break;
            
        case kOpenSSL_algo_md5:
            type = EVP_md5();
            break;
            
        case kOpenSSL_algo_md4:
            type = EVP_md4();
            break;
            
        case kOpenSSL_algo_md2:
            type = EVP_md2();
            break;
            
        case kOpenSSL_algo_dss1:
            type = EVP_dss1();
            break;
            
        case kOpenSSL_algo_sha224:
            type = EVP_sha224();
            break;
            
        case kOpenSSL_algo_sha256:
            type = EVP_sha256();
            break;
            
        case kOpenSSL_algo_sha384:
            type = EVP_sha384();
            break;
            
        case kOpenSSL_algo_sha512:
            type = EVP_sha512();
            break;
            
        case kOpenSSL_algo_ripemd160:
            type = EVP_ripemd160();
            break;
    }
    EVP_VerifyInit((EVP_MD_CTX *)m_ctx, type);
}

OpenSSL_Verifier::~OpenSSL_Verifier() {
    EVP_MD_CTX_cleanup((EVP_MD_CTX *)m_ctx);
    Memory::free(m_ctx);
}

size_t OpenSSL_Verifier::read(void *buffer, size_t size) {
    return 0;
}

size_t OpenSSL_Verifier::write(const void *buffer, size_t size) {
    if (!EVP_VerifyUpdate((EVP_MD_CTX *)m_ctx, buffer, (unsigned int)size)) {
        return 0;
    }
    return (unsigned int)size;
}

bool OpenSSL_Verifier::seek(s64 offset, SeekDir way) {
    return false;
}

s64 OpenSSL_Verifier::tell() {
    return m_processed;
}

bool OpenSSL_Verifier::verify(OpenSSL_KeyPtr key, const void *sign, size_t sign_size) {
    if (!key) {
        return {};
    }
    EVP_PKEY *_k = (EVP_PKEY *)key->value();
    if (!_k) {
        return {};
    }
    return EVP_VerifyFinal((EVP_MD_CTX *)m_ctx, (unsigned char *)sign, (unsigned int)sign_size, _k) > 0;
}

bool OpenSSL_Verifier::verify_base64(OpenSSL_KeyPtr key, const str8 &sign) {
    auto t = Base64::decode(sign);
    return this->verify(std::move(key), t.data(), t.size());
}

_vn_end
