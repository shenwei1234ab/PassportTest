//
//  vnSingletonManager.h
//  vnlib
//
//  Created by Wayne on 14-9-26.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnSingletonManager_h
#define vnlib_vnSingletonManager_h


#include "vnAllocatedObject.h"
#include <list>
#include <mutex>

_vn_begin

class SingletonManager {
public:
	
	static SingletonManager & instance();
	
	~SingletonManager();
	
	void destroyAll();
	
	void _register(void (*destroyer)());
	
private:
	SingletonManager();
	std::recursive_mutex m_lock;
	std::list<void (*)()> m_registry;
};

_vn_end

#endif
