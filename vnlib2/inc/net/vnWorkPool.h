//
//  vnWorkPool.h
//  vnlib
//
//  Created by Wayne on 6/10/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef vnlib_vnWorkPool_h
#define vnlib_vnWorkPool_h

#include "vnWorkQueue.h"

#include <thread>
#include <vector>
#include <atomic>
#include <future>
#include <set>

_vn_begin

class WorkPool : public NonCopyable {
public:
    WorkPool();
    ~WorkPool();
    
    size_t startup(size_t thread_count, std::function<bool(void *&)> init_fn = nullptr, std::function<void(void *)> fini_fn = nullptr);
    void post(std::function<void(void *)> work);
    bool send(std::function<void(void *)> work);
    bool cleanup();
    
    bool running() const;
protected:
    WorkQueue m_queue;
    std::vector<std::thread> m_threads;
    std::set<std::promise<bool> *> m_promises;
    std::mutex m_lock;
};

_vn_end

#endif
