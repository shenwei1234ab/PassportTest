//
//  vnWorkQueue.h
//  vnlib
//
//  Created by Wayne on 6/9/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef vnlib_vnWorkQueue_h
#define vnlib_vnWorkQueue_h

#include "vnNonCopyable.h"

#include <functional>
#include <deque>
#include <mutex>
#include <condition_variable>


_vn_begin

class WorkQueue : public NonCopyable {
public:
    void post(std::function<void(void *)> work);
    void process(void *);
    bool process(void *, u32 milliseconds);
    bool enabled() const;
    void setEnabled(bool _enabled);
protected:
    std::deque<std::function<void(void *)>> m_works;
    std::mutex m_lock;
    std::condition_variable m_cond;
    bool m_enabled = true;
};

_vn_end

#endif
