//
//  vnOpenSSL_Algorithms.h
//  vnlib
//
//  Created by Wayne on 8/16/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef vnlib_vnOpenSSL_Algorithms_h
#define vnlib_vnOpenSSL_Algorithms_h

#include "../vnDefine.h"

_vn_begin

enum OpenSSL_algo {
    kOpenSSL_algo_sha1,
    kOpenSSL_algo_md5,
    kOpenSSL_algo_md4,
    kOpenSSL_algo_md2,
    kOpenSSL_algo_dss1,
    kOpenSSL_algo_sha224,
    kOpenSSL_algo_sha256,
    kOpenSSL_algo_sha384,
    kOpenSSL_algo_sha512,
    kOpenSSL_algo_ripemd160,
};

_vn_end

#endif
