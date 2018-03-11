//
//  vnReadWriteLock.cpp
//  vnlib
//
//  Created by Wayne on 6/17/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnReadWriteLock.h"

#if VN_PLATFORM == VN_PLATFORM_MAC || VN_PLATFORM == VN_PLATFORM_FREEBSD || VN_PLATFORM == VN_PLATFORM_LINUX
#   include "pthread/vnReadWriteLockImpl.h"
#else

#endif

_vn_begin

ReadWriteLock::ReadWriteLock()
: m_impl(vnnew internal::ReadWriteLockImpl()){
    
}

ReadWriteLock::~ReadWriteLock() {
    delete m_impl;
}

void ReadWriteLock::lock() {
    m_impl->lock();
}

bool ReadWriteLock::try_lock() {
    return m_impl->try_lock();
}

void ReadWriteLock::lock_write() {
    m_impl->lock_write();
}

bool ReadWriteLock::try_lock_write() {
    return m_impl->try_lock_write();
}

void ReadWriteLock::unlock() {
    m_impl->unlock();
}

_vn_end
