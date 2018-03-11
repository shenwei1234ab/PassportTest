//
//  vnOpenSSL_Signer.cpp
//  vnlib
//
//  Created by Wayne on 8/16/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "vnOpenSSL_Signer.h"

#include "vnBase64.h"

#include <openssl/evp.h>

_vn_begin

OpenSSL_Signer::OpenSSL_Signer(OpenSSL_algo algo)
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
    EVP_SignInit((EVP_MD_CTX *)m_ctx, type);
}

OpenSSL_Signer::~OpenSSL_Signer() {
    EVP_MD_CTX_cleanup((EVP_MD_CTX *)m_ctx);
    Memory::free(m_ctx);
}

size_t OpenSSL_Signer::read(void *buffer, size_t size) {
    return 0;
}

size_t OpenSSL_Signer::write(const void *buffer, size_t size) {
    if (!EVP_SignUpdate((EVP_MD_CTX *)m_ctx, buffer, (unsigned int)size)) {
        return 0;
    }
    return (unsigned int)size;
}

bool OpenSSL_Signer::seek(s64 offset, SeekDir way) {
    return false;
}

s64 OpenSSL_Signer::tell() {
    return m_processed;
}

std::vector<u8> OpenSSL_Signer::sign(OpenSSL_KeyPtr key) {
    if (!key) {
        return {};
    }
    EVP_PKEY *_k = (EVP_PKEY *)key->value();
    if (!_k) {
        return {};
    }
    unsigned int length = EVP_PKEY_size(_k);
    std::vector<u8> ret(length);
    if (!EVP_SignFinal((EVP_MD_CTX *)m_ctx, ret.data(), &length, _k)) {
        return {};
    }
    ret.resize(length);
    return std::move(ret);
}

str8 OpenSSL_Signer::sign_base64(OpenSSL_KeyPtr key) {
    auto t = this->sign(std::move(key));
    if (t.empty()) {
        return {};
    }
    return Base64::encode(t.data(), t.size());
}

_vn_end