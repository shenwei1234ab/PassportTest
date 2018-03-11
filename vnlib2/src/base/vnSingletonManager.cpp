//
//  vnSingletonManager.cpp
//  vnlib
//
//  Created by Wayne on 14-9-26.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnSingletonManager.h"
#include "vnMemoryTracker.h"

_vn_begin

SingletonManager & SingletonManager::instance() {
	static SingletonManager s_ins;
	return s_ins;
}

SingletonManager::SingletonManager() {
#ifdef VN_MEMORY_TRACKER
	MemoryTracker::instance();
#endif
}

SingletonManager::~SingletonManager() {
	destroyAll();
}

void SingletonManager::destroyAll() {
	std::lock_guard<decltype(m_lock)> guard(m_lock);
	while (!m_registry.empty()) {
		auto destoryer = m_registry.back();
		m_registry.pop_back();
		destoryer();
	}
}

void SingletonManager::_register(void (*destroyer)()) {
	std::lock_guard<decltype(m_lock)> guard(m_lock);
	m_registry.push_back(destroyer);
}

_vn_end
