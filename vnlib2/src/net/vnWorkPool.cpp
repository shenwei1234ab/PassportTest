//
//  vnWorkPool.cpp
//  vnlib
//
//  Created by Wayne on 6/10/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "vnWorkPool.h"
#include <mutex>
#include <condition_variable>
#include <iostream>       // std::cout, std::ios
#include <future>         // std::async, std::future
#include <exception>      // std::exception

_vn_begin

WorkPool::WorkPool() {
    m_queue.setEnabled(false);
}

WorkPool::~WorkPool() {
    cleanup();
}

size_t WorkPool::startup(size_t thread_count, std::function<bool(void *&)> init_fn, std::function<void(void *)> fini_fn) {
    m_lock.lock();
    if (m_queue.enabled()) {
        m_lock.unlock();
        return false;
    }
    
    
    size_t ret = 0;
    std::mutex init_lock;
    std::condition_variable init_cond;
    auto fn = [this](std::mutex &init_lock, std::condition_variable &init_cond, int &init_result, std::function<bool(void *&)> init_fn, std::function<void (void *)> fini_fn) {
        void *local = nullptr;
        if (init_fn) {
            if (!init_fn(local)) {
                init_lock.lock();
                init_result = -1;
                init_lock.unlock();
                init_cond.notify_one();
                if (fini_fn) {
                    fini_fn(local);
                }
                return ;
            }
        }
        init_lock.lock();
        init_result = 1;
        init_lock.unlock();
        init_cond.notify_one();
        m_queue.process(local);
        if (fini_fn) {
            fini_fn(local);
        }
    };
    m_queue.setEnabled(true);
    m_threads.reserve(thread_count);
    for (size_t i = 0; i < thread_count; ++i) {
        int init_result = 0;
        m_threads.push_back(std::thread(fn, std::ref(init_lock), std::ref(init_cond), std::ref(init_result) , init_fn, fini_fn));
        std::unique_lock<std::mutex> lck(init_lock);
        if (!init_result) {
            init_cond.wait(lck);
        }
        if (init_result > 0) {
            ++ret;
        }
    }
    if (!ret) {
        m_queue.setEnabled(false);
        for (auto &i : m_threads) {
            i.join();
        }
        m_threads.clear();
    }
    m_lock.unlock();
    return ret;
}

void WorkPool::post(std::function<void(void *)> work) {
    m_queue.post(std::move(work));
}
//volatile int send_index = 0;
//volatile int run_index = 0;
bool WorkPool::send(std::function<void(void *)> work) {
    if (!work) {
        return false;
    }
    if (!m_queue.enabled()) {
        return false;
    }
    m_lock.lock();
    //std::promise<bool> prom;
    std::shared_ptr<std::promise<bool>>   prom (new std::promise<bool>);
    //printf("promise[%p] construct, send_index[%d]\n",prom.get(),send_index);
    m_promises.insert(prom.get());
    m_lock.unlock();
    std::future<bool> fut = prom->get_future();
    post([this, prom, &work](void *param){
        work(param);
        m_lock.lock();
        //printf("promise[%p] use, send_index[%d],run_index[%d]\n",prom.get(),send_index,run_index++);
        m_promises.erase(prom.get());
        prom->set_value(true);
        m_lock.unlock();

    });
	try {
		fut.wait();
		if (!fut.get()) {
			printf("promise[%p] false\n",prom.get());
			return false;
		}
	} catch (std::exception& e) {
		printf("[exception caught]:%s", e.what());
		return false;
	}
	//printf("promise[%p] destructor, send_index[%d]\n",prom.get(),send_index);
	//send_index++;
    return true;
}

bool WorkPool::cleanup() {
    m_lock.lock();
    if (!m_queue.enabled()) {
        m_lock.unlock();
        return false;
    }
    for (auto p : m_promises) {
        p->set_value(false);
    }
    m_promises.clear();
    m_queue.setEnabled(false);
    for (auto &i : m_threads) {
        i.join();
    }
    m_threads.clear();
    m_lock.unlock();
    return true;
}

bool WorkPool::running() const {
    return m_queue.enabled();
}

_vn_end
