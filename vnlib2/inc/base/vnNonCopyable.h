//
//  vnNonCopyable.h
//  vnlib
//
//  Created by Wayne on 5/7/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnNonCopyable_h
#define vnlib_vnNonCopyable_h

#include "../vnDefine.h"

_vn_begin

class NonCopyable {
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable &) = delete;
    NonCopyable & operator =(const NonCopyable &) = delete;
};

_vn_end

#endif
