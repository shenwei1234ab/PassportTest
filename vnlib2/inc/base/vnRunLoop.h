//
//  vnRunLoop.h
//  vnlib
//
//  Created by Wayne on 4/27/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef vnlib_vnRunLoop_h
#define vnlib_vnRunLoop_h

#include "vnSpinLock.h"

#include <vector>
#include <functional>
#include <atomic>
#include <condition_variable>

_vn_begin

class RunLoop : public NonCopyable {
public:
    RunLoop();
    RunLoop(size_t reserved);
    
    bool running() const { return m_running; }
    
    void quit();
    void post_quit();
    void post(std::function<void()> fn);

    bool run();
    bool run(f32 interval, const std::function<void (f32)> &idle);
    
private:
    std::condition_variable_any m_cond;
    SpinLock m_lock;
    std::vector<std::function<void()>> m_incoming, m_processing;
    std::vector<std::function<void()>>::iterator m_current;
    std::atomic_bool m_running;
};

_vn_end

#endif
