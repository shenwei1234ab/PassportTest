//
//  vnMemory.h
//  vnlib
//
//  Created by Wayne on 5/7/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnMemory_h
#define vnlib_vnMemory_h

#include "../vnDefine.h"

_vn_begin

class Memory {
public:
	static void * allocate(size_t size);
	static void * allocate(size_t size, const char *file, int line, int hint);
	static void free(void *ptr);
};

_vn_end

#ifdef VN_DEBUG
#	define vnmalloc(type, count) (type *)_vn_ns::Memory::allocate(sizeof(type) *(count), __FILE__, __LINE__, 0)
#	define vnmalloc_hint(type, count, hint) (type *)_vn_ns::Memory::allocate(sizeof(type) *(count), __FILE__, __LINE__, hint)
#else
#	define vnmalloc(type, count) (type *)_vn_ns::Memory::allocate(sizeof(type) *(count))
#	define vnmalloc_hint(type, count, hint) (type *)_vn_ns::Memory::allocate(sizeof(type) *(count))
#endif

#define vnfree(ptr) _vn_ns::Memory::free(ptr)

#endif
