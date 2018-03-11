//
//  vnAssert.h
//  vnlib
//
//  Created by Wayne on 14-5-11.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnAssert_h
#define vnlib_vnAssert_h

#include "../vnDefine.h"

#include <cassert>

#ifdef VN_DEBUG
#	define vnassert(x) assert(x)
#else
#	define vnassert(x)
#endif

#endif
