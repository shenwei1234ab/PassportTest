//
//  vnOpenSSL_Key.cpp
//  vnlib
//
//  Created by Wayne on 8/16/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "vnOpenSSL_Key.h"

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

_vn_begin

OpenSSL_Key::~OpenSSL_Key() {
    if (m_key) {
        ::EVP_PKEY_free((EVP_PKEY *)m_key);
        m_key = 0;
    }
}

OpenSSL_Key::OpenSSL_Key(OpenSSL_Key &&rhs) {
    m_key = rhs.m_key;
    rhs.m_key = 0;
}

OpenSSL_KeyPtr OpenSSL_Key::create_public(const void *data, size_t size) {
    auto bio = ::BIO_new_mem_buf((void *)data, (int)size);
    if (!bio) {
        return nullptr;
    }
    auto key = ::PEM_read_bio_PUBKEY(bio, 0, 0, 0);
    ::BIO_free(bio);
    
    if (!key) {
        return nullptr;
    }
    
    auto ret = vnnew OpenSSL_Key();
    ret->m_key = key;
    return ret;
}

OpenSSL_KeyPtr OpenSSL_Key::create_private(const void *data, size_t size, const c8 *passphrase) {
    auto bio = ::BIO_new_mem_buf((void *)data, (int)size);
    if (!bio) {
        return nullptr;
    }
    if (!passphrase) {
        passphrase = "";
    }
    auto key = ::PEM_read_bio_PrivateKey(bio, 0, 0, (void *)passphrase);
    ::BIO_free(bio);
    
    if (!key) {
        return nullptr;
    }
    
    auto ret = vnnew OpenSSL_Key();
    ret->m_key = key;
    return ret;
}

std::vector<u8> OpenSSL_Key::rsa_public_encrypt(const void *data, size_t size, RSA_Padding padding) {
    auto _key = (EVP_PKEY *)m_key;
    if (_key->type != EVP_PKEY_RSA && _key->type != EVP_PKEY_RSA2) {
        return {};
    }
    int _padding;
    switch (padding) {
        case kRSA_PKCS1_Padding:
            _padding = RSA_PKCS1_PADDING;
            break;
            
        case kRSA_PKCS1_OAEP_Padding:
            _padding = RSA_PKCS1_OAEP_PADDING;
            break;
            
        case kRSA_SSLV23_Padding:
            _padding = RSA_SSLV23_PADDING;
            break;
            
        case kRSA_NoPadding:
            _padding = RSA_NO_PADDING;
            break;
            
        default:
            return {};
    }
    auto rsa = _key->pkey.rsa;
    std::vector<u8> ret;
    ret.resize(RSA_size(rsa));
    int n = ::RSA_public_encrypt((int)size, (const unsigned char *)data, ret.data(), rsa, _padding);
    if (n < 0) {
        return {};
    }
    ret.resize((size_t)n);
    return std::move(ret);
}

std::vector<u8> OpenSSL_Key::rsa_public_decrypt(const void *data, size_t size, RSA_Padding padding) {
    auto _key = (EVP_PKEY *)m_key;
    if (_key->type != EVP_PKEY_RSA && _key->type != EVP_PKEY_RSA2) {
        return {};
    }
    int _padding;
    switch (padding) {
        case kRSA_PKCS1_Padding:
            _padding = RSA_PKCS1_PADDING;
            break;
            
        case kRSA_PKCS1_OAEP_Padding:
            _padding = RSA_PKCS1_OAEP_PADDING;
            break;
            
        case kRSA_SSLV23_Padding:
            _padding = RSA_SSLV23_PADDING;
            break;
            
        case kRSA_NoPadding:
            _padding = RSA_NO_PADDING;
            break;
            
        default:
            return {};
    }
    auto rsa = _key->pkey.rsa;
    std::vector<u8> ret;
    ret.resize(RSA_size(rsa));
    int n = ::RSA_public_decrypt((int)size, (const unsigned char *)data, ret.data(), rsa, _padding);
    if (n < 0) {
        return {};
    }
    ret.resize((size_t)n);
    return std::move(ret);
}

std::vector<u8> OpenSSL_Key::rsa_private_encrypt(const void *data, size_t size, RSA_Padding padding) {
    auto _key = (EVP_PKEY *)m_key;
    if (_key->type != EVP_PKEY_RSA && _key->type != EVP_PKEY_RSA2) {
        return {};
    }
    int _padding;
    switch (padding) {
        case kRSA_PKCS1_Padding:
            _padding = RSA_PKCS1_PADDING;
            break;
            
        case kRSA_PKCS1_OAEP_Padding:
            _padding = RSA_PKCS1_OAEP_PADDING;
            break;
            
        case kRSA_SSLV23_Padding:
            _padding = RSA_SSLV23_PADDING;
            break;
            
        case kRSA_NoPadding:
            _padding = RSA_NO_PADDING;
            break;
            
        default:
            return {};
    }
    auto rsa = _key->pkey.rsa;
    std::vector<u8> ret;
    ret.resize(RSA_size(rsa));
    int n = ::RSA_private_encrypt((int)size, (const unsigned char *)data, ret.data(), rsa, _padding);
    if (n < 0) {
        return {};
    }
    ret.resize((size_t)n);
    return std::move(ret);
}

std::vector<u8> OpenSSL_Key::rsa_private_decrypt(const void *data, size_t size, RSA_Padding padding) {
    auto _key = (EVP_PKEY *)m_key;
    if (_key->type != EVP_PKEY_RSA && _key->type != EVP_PKEY_RSA2) {
        return {};
    }
    int _padding;
    switch (padding) {
        case kRSA_PKCS1_Padding:
            _padding = RSA_PKCS1_PADDING;
            break;
            
        case kRSA_PKCS1_OAEP_Padding:
            _padding = RSA_PKCS1_OAEP_PADDING;
            break;
            
        case kRSA_SSLV23_Padding:
            _padding = RSA_SSLV23_PADDING;
            break;
            
        case kRSA_NoPadding:
            _padding = RSA_NO_PADDING;
            break;
            
        default:
            return {};
    }
    auto rsa = _key->pkey.rsa;
    std::vector<u8> ret;
    ret.resize(RSA_size(rsa));
    int n = ::RSA_private_decrypt((int)size, (const unsigned char *)data, ret.data(), rsa, _padding);
    if (n < 0) {
        return {};
    }
    ret.resize((size_t)n);
    return std::move(ret);
}

_vn_end
