//
//  vnBase64.h
//  vnlib
//
//  Created by Wayne on 6/15/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef vnlib_vnBase64_h
#define vnlib_vnBase64_h

#include "../vnDefine.h"

#include <vector>

_vn_begin

class Base64 {
public:
    static str8 encode(const void *data, size_t size);
    static std::vector<u8> decode(const str8 &base64);
};

_vn_end

#endif
