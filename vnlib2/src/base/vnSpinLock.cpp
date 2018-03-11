//
//  vnSpinLock.cpp
//  vnlib
//
//  Created by Wayne on 5/7/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnSpinLock.h"

_vn_begin

SpinLock::SpinLock() {
    m_lock.clear(std::memory_order_release);
}

void SpinLock::lock() {
    while(m_lock.test_and_set(std::memory_order_acquire));
}

bool SpinLock::try_lock() {
    return !m_lock.test_and_set(std::memory_order_acquire);
}

void SpinLock::unlock() {
    m_lock.clear(std::memory_order_release);
}

SpinLockWithCount::SpinLockWithCount(size_t count)
: m_count(count) {
    m_lock.clear(std::memory_order_release);
}

void SpinLockWithCount::lock() {
    size_t i = 0;
    while (m_lock.test_and_set(std::memory_order_acquire)) {
        if (++i > m_count) {
            std::this_thread::yield();
            i = 0;
        }
    }
}

bool SpinLockWithCount::try_lock() {
    return !m_lock.test_and_set(std::memory_order_acquire);
}

void SpinLockWithCount::unlock() {
    m_lock.clear(std::memory_order_release);
}

_vn_end
