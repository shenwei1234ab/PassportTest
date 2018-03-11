/*
 * TimerManager.cpp
 *
 *  Created on: 2015年11月5日
 *      Author: leave
 */

#include "manager/TimerManager.h"

#include <deque>

using namespace _vn_ns;

TimerManager::TimerManager() {
	// TODO Auto-generated constructor stub

}

TimerManager::~TimerManager() {
	std::lock_guard<SpinLock> lck(m_timers_lock);
	(void) lck;
	for (auto p : m_timers) {
		delete p;
	}
}


timer_handler TimerManager::createTimer(f32 periods, std::function<void()> fn, u32 repeats) {
    if (!fn) {
        return nullptr;
    }

    std::lock_guard<SpinLock> lck(m_timers_lock);
    (void)lck;

    auto p = new Timer(periods, repeats, std::move(fn));
    m_timers.insert(p);

    return p;
}

void TimerManager::destroyTimer(timer_handler timer) {
    std::lock_guard<SpinLock> lck(m_timers_lock);
    (void)lck;
    auto it = m_timers.find((Timer *)timer);
    if (it == m_timers.end()) {
        return ;
    }
	(*it)->drop();
	m_timers.erase(it);
}

void TimerManager::updateTimers(f32 dt) {
    std::deque<Timer *> rised, dead;
    m_timers_lock.lock();
    for (auto p : m_timers) {
        p->timer += dt;
        if (p->timer >= p->periods) {
            p->timer -= p->periods;
            p->grab();
            rised.push_back(p);
            if (p->repeats != u32(-1)) {
                if (!p->repeats--) {
                    dead.push_back(p);
                }
            }
        }
    }
    m_timers_lock.unlock();

    for (auto p : rised) {
        p->fn();
        p->drop();
    }

    if (!dead.empty()) {
        m_timers_lock.lock();
        for (auto p : dead) {
            auto it = m_timers.find(p);
            if (it != m_timers.end()) {
                p->drop();
                m_timers.erase(it);
            }
        }
        m_timers_lock.unlock();
    }
}
