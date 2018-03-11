//
//  vnMemory.cpp
//  vnlib
//
//  Created by Wayne on 5/7/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnMemory.h"
#include "vnMemoryTracker.h"
#include <cstdlib>

#ifndef VN_MEMORY_POLICY
#	define VN_MEMORY_POLICY StdMemoryPolicy
#endif

_vn_begin

class StdMemoryPolicy {
public:
	static void * allocate(size_t size) {
		return ::malloc(size);
	}
    
	static void free(void *ptr) {
		::free(ptr);
	}
};

void * Memory::allocate(size_t size) {
	void *ptr = VN_MEMORY_POLICY::allocate(size);
#ifdef VN_MEMORY_TRACKER
	MemoryTracker::instance().recordAllocation(ptr, size);
#endif
	return ptr;
}

void * Memory::allocate(size_t size, const char *file, int line, int hint) {
	void *ptr = VN_MEMORY_POLICY::allocate(size);
#ifdef VN_MEMORY_TRACKER
	MemoryTracker::instance().recordAllocation(ptr, size, file, line, hint);
#endif
	return ptr;
}

void Memory::free(void *ptr) {
#ifdef VN_MEMORY_TRACKER
	MemoryTracker::instance().recordDeallocation(ptr);
#endif
	VN_MEMORY_POLICY::free(ptr);
}


_vn_end
