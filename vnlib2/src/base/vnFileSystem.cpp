//
//  vnFileSystem.cpp
//  vnlib
//
//  Created by Wayne on 5/12/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnFileSystem.h"

#if VN_PLATFORM == VN_PLATFORM_WIN
#	include "win/vnFileSystemOS.hpp"
#else
#   include "posix/vnFileSystemOS.hpp"
#endif