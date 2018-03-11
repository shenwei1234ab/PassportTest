//
//  vnReadWriteLock.h
//  vnlib
//
//  Created by Wayne on 6/17/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnReadWriteLock_h
#define vnlib_vnReadWriteLock_h

#include "vnNonCopyable.h"

_vn_begin

namespace internal {
    class ReadWriteLockImpl;
}

class ReadWriteLock : public NonCopyable {
public:
    ReadWriteLock();
    ~ReadWriteLock();
    
    void lock(); // read.
    bool try_lock(); // read.
    
    void lock_write();
    bool try_lock_write();
    
    void unlock();
private:
    internal::ReadWriteLockImpl *m_impl;
};

_vn_end

#endif
