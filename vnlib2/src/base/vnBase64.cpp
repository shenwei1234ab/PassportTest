//
//  vnBase64.cpp
//  vnlib
//
//  Created by Wayne on 6/15/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "vnBase64.h"

_vn_begin

namespace internal {
    const c8 * g_base64Encode = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    const c8 g_base64Decode[] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
        -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
        -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };
    
    union Base64Block {
        struct {
            u32 p1 : 6;
            u32 p2 : 6;
            u32 p3 : 6;
            u32 p4 : 6;
        };
        struct {
            u8 b1;
            u8 b2;
            u8 b3;
        };
        u32 value;
        
        Base64Block() {
            value = 0;
        }
        
        void write3(c8 *p) const {
            Base64Block n;
            n.b3 = b1;
            n.b2 = b2;
            n.b1 = b3;
            p[0] = g_base64Encode[n.p4];
            p[1] = g_base64Encode[n.p3];
            p[2] = g_base64Encode[n.p2];
            p[3] = g_base64Encode[n.p1];
        }
        
        void write2(c8 *p) const {
            Base64Block n;
            n.b3 = b1;
            n.b2 = b2;
            p[0] = g_base64Encode[n.p4];
            p[1] = g_base64Encode[n.p3];
            p[2] = g_base64Encode[n.p2];
            p[3] = '=';
        }
        
        void write1(c8 *p) const {
            Base64Block n;
            n.b3 = b1;
            p[0] = g_base64Encode[n.p4];
            p[1] = g_base64Encode[n.p3];
            p[2] = '=';
            p[3] = '=';
        }
    };
}

str8 Base64::encode(const void *data, size_t size) {
    str8 ret;
    size_t m = size / 3;
    size_t n = size % 3;
    ret.resize((n ? m+1 : m) * 4);
    
    const u8 *ptr = (const u8 *)data;
    size_t idx = 0;
    for (size_t i = 0; i < m; ++i) {
        reinterpret_cast<const internal::Base64Block *>(ptr)->write3(&ret[idx]);
        ptr += 3;
        idx += 4;
    }
    switch (n) {
        case 2:
            reinterpret_cast<const internal::Base64Block *>(ptr)->write2(&ret[idx]);
            break;
        case 1:
            reinterpret_cast<const internal::Base64Block *>(ptr)->write1(&ret[idx]);
            break;
    }
    return std::move(ret);
}

std::vector<u8> Base64::decode(const str8 &base64) {
    std::vector<u8> ret;
    if (base64.empty() || base64.length() % 4) {
        return std::move(ret);
    }
    size_t m = base64.length() / 4;
    ret.reserve(m-- * 3);
    
    auto p = base64.c_str();
    internal::Base64Block block;
    c8 c;
    for (size_t i = 0; i < m; ++i) {
        c = internal::g_base64Decode[(size_t)*p++];
        if (c < 0) {
            return {};
        }
        block.p4 = c;
        c = internal::g_base64Decode[(size_t)*p++];
        if (c < 0) {
            return {};
        }
        block.p3 = c;
        c = internal::g_base64Decode[(size_t)*p++];
        if (c < 0) {
            return {};
        }
        block.p2 = c;
        c = internal::g_base64Decode[(size_t)*p++];
        if (c < 0) {
            return {};
        }
        block.p1 = c;
        ret.push_back(block.b3);
        ret.push_back(block.b2);
        ret.push_back(block.b1);
    }
    block.value = 0;
    c = internal::g_base64Decode[(size_t)*p++];
    if (c < 0) {
        return {};
    }
    block.p4 = c;
    c = internal::g_base64Decode[(size_t)*p++];
    if (c < 0) {
        return {};
    }
    block.p3 = c;
    c = *p++;
    if (c == '=') {
        if (*p != '=') {
            return {};
        }
        ret.push_back(block.b3);
        return std::move(ret);
    }
    c = internal::g_base64Decode[(size_t)c];
    if (c < 0) {
        return {};
    }
    block.p2 = c;
    if (*p == '=') {
        ret.push_back(block.b3);
        ret.push_back(block.b2);
        return std::move(ret);
    }
    c = internal::g_base64Decode[(size_t)*p];
    if (c < 0) {
        return {};
    }
    block.p1 = c;
    ret.push_back(block.b3);
    ret.push_back(block.b2);
    ret.push_back(block.b1);
    return std::move(ret);
}

_vn_end
