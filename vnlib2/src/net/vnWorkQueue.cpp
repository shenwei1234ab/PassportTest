//
//  vnWorkQueue.cpp
//  vnlib
//
//  Created by Wayne on 6/9/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "vnWorkQueue.h"

#include <chrono>

_vn_begin

void WorkQueue::post(std::function<void(void *)> work) {
    if (!work) {
        return ;
    }
    m_lock.lock();
    m_works.push_front(std::move(work));
    if (m_enabled) {
        m_cond.notify_one();
    }
    m_lock.unlock();
}

void WorkQueue::process(void *param) {
    std::unique_lock<std::mutex> lck(m_lock);
    while (m_enabled) {
        if (m_works.empty()) {
            m_cond.wait(lck);
            continue;
        }
        auto fn = std::move(m_works.back());
        m_works.pop_back();
        lck.unlock();
        fn(param);
        lck.lock();
    }
}

bool WorkQueue::process(void *param, u32 milliseconds) {
    std::unique_lock<std::mutex> lck(m_lock);
    while (m_enabled) {
        if (m_works.empty()) {
            if (m_cond.wait_for(lck, std::chrono::milliseconds(milliseconds)) == std::cv_status::timeout) {
                return true;
            }
            continue;
        }
        auto fn = std::move(m_works.back());
        m_works.pop_back();
        lck.unlock();
        fn(param);
        lck.lock();
    }
    return false;
}

bool WorkQueue::enabled() const {
    return m_enabled;
}

void WorkQueue::setEnabled(bool _enabled) {
    m_lock.lock();
    if (m_enabled != _enabled) {
        if (!(m_enabled = _enabled)) {
            m_cond.notify_all();
        }
    }
    m_lock.unlock();
}

_vn_end
