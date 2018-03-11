//
//  vnSpinLock.h
//  vnlib
//
//  Created by Wayne on 14-5-7.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnSpinLock_h
#define vnlib_vnSpinLock_h

#include "vnNonCopyable.h"
#include <atomic>
#include <thread>

_vn_begin

// STL Lockable.

class SpinLock : public NonCopyable {
public:
    SpinLock();
    
    void lock();
    bool try_lock();
    void unlock();

private:
	std::atomic_flag m_lock;

};

class SpinLockWithCount : public NonCopyable {
public:
    SpinLockWithCount(size_t count = 0);
    
    void lock();
    bool try_lock();
    void unlock();

private:
	std::atomic_flag m_lock;
    size_t m_count = 0;
};

_vn_end


#endif
