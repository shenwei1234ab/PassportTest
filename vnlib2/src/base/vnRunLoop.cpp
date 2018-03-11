//
//  vnRunLoop.cpp
//  vnlib
//
//  Created by Wayne on 4/27/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "vnRunLoop.h"
#include <chrono>

_vn_begin

RunLoop::RunLoop() {
    m_running = false;
    m_current = m_processing.end();
}

RunLoop::RunLoop(size_t reserved) {
    m_processing.reserve(reserved);
    m_incoming.reserve(reserved);
    m_running = false;
    m_current = m_processing.end();
}

void RunLoop::quit() {
    m_running = false;
    m_cond.notify_one();
}

bool RunLoop::run() {
    bool _t = false;
    if (!m_running.compare_exchange_strong(_t, true)) {
        return false;
    }
    while (m_running) {
        if (m_current != m_processing.end()) {
            (*m_current)();
            ++m_current;
            continue;
        }
        m_processing.clear();
        m_lock.lock();
        if (m_incoming.empty()) {
            m_cond.wait(m_lock);
        }
        m_incoming.swap(m_processing);
        m_lock.unlock();
        m_current = m_processing.begin();
    }
    return true;
}

void RunLoop::post(std::function<void ()> fn) {
    if (!fn) {
        return ;
    }
    m_lock.lock();
    m_incoming.push_back(std::move(fn));
    m_cond.notify_one();
    m_lock.unlock();
}

void RunLoop::post_quit() {
    m_lock.lock();
    m_incoming.push_back([this](){
        m_running = false;
    });
    m_cond.notify_one();
    m_lock.unlock();
}

bool RunLoop::run(f32 interval, const std::function<void (f32)> &idle) {
    if (!idle || interval < 0) {
        return false;
    }
    bool _t = false;
    if (!m_running.compare_exchange_strong(_t, true)) {
        return false;
    }
    auto period = std::chrono::microseconds((long long)(interval * 1000000));
    auto last = std::chrono::steady_clock::now();
    decltype(last) next = last + period;
    while (m_running) {
        if (m_current != m_processing.end()) {
            (*m_current)();
            ++m_current;
            continue;
        }
        m_processing.clear();
        m_lock.lock();
        while (m_incoming.empty()) {
            if (m_cond.wait_until(m_lock, next) == std::cv_status::no_timeout) {
                break;
            } else {
                m_lock.unlock();
                auto now = std::chrono::steady_clock::now();
                f32 dt = std::chrono::duration_cast< std::chrono::duration<f32> >(now - last).count();
                next += period;
                last = now;
                idle(dt);
                if (!m_running) {
                    return true;
                }
                m_lock.lock();
            }
        }
        m_incoming.swap(m_processing);
        m_lock.unlock();
        m_current = m_processing.begin();
    }
    return true;
}

_vn_end
