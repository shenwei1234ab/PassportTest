//
//  vnAllocatedObject.h
//  vnlib
//
//  Created by Wayne on 5/7/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnAllocatedObject_h
#define vnlib_vnAllocatedObject_h

#include "vnMemory.h"

#ifdef new
#	undef new
#endif

#ifdef delete
#	undef delete
#endif

_vn_begin

class AllocatedObject {
public:
	void * operator new(size_t size) {
		return Memory::allocate(size);
	}
    
    void * operator new(size_t size, void *ptr) {
        return ptr;
    }
    
	void * operator new(size_t size, const char *file, int line, int hint) {
		return Memory::allocate(size, file, line, hint);
	}

	void * operator new[](size_t size) {
		return Memory::allocate(size);
	}
	
    void * operator new[](size_t size, const char *file, int line, int hint) {
		return Memory::allocate(size, file, line, hint);
	}
    
	void operator delete(void *p) {
		Memory::free(p);
	}
    
    void operator delete(void *, void *) {
        
    }
	
    void operator delete(void *p, const c8 *, int, int) {
		Memory::free(p);
	}

	void operator delete[](void *p) {
		Memory::free(p);
	}
	
    void operator delete[](void *p, const c8 *, int, int) {
		Memory::free(p);
	}
};

_vn_end

#define VN_SAFE_DELETE(p) if (p) { delete p; p = 0; }
#define VN_SAFE_DELETE_ARRAY(p) if (p) { delete []p; p = 0;}

#ifdef VN_DEBUG
#   define vnnew new(__FILE__, __LINE__, 0)
#   define vnnew_hint(hint) new(__FILE__, __LINE__, hint)
#else
#   define vnnew new
#   define vnnew_hint(hint) new
#endif


#endif
