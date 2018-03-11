//
//  vnReadWriteLockImpl.h
//  vnlib
//
//  Created by Wayne on 6/17/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __vnlib__vnReadWriteLockImpl__
#define __vnlib__vnReadWriteLockImpl__

#include "vnAllocatedObject.h"
#include <pthread.h>

_vn_begin

namespace internal {
    
    class ReadWriteLockImpl : public AllocatedObject {
    public:
        ReadWriteLockImpl() {
            pthread_rwlock_init(&m_rwlock, 0);
        }
        
        ~ReadWriteLockImpl() {
            pthread_rwlock_destroy(&m_rwlock);
        }
        
        void lock() {
            pthread_rwlock_rdlock(&m_rwlock);
        }
        
        bool try_lock() {
            return pthread_rwlock_tryrdlock(&m_rwlock) == 0;
        }
        
        void lock_write() {
            pthread_rwlock_wrlock(&m_rwlock);
        }
        bool try_lock_write() {
            return pthread_rwlock_trywrlock(&m_rwlock) == 0;
        }
        
        void unlock() {
            pthread_rwlock_unlock(&m_rwlock);
        }
        
    private:
        pthread_rwlock_t m_rwlock;
    };
    
}

_vn_end


#endif /* defined(__vnlib__vnReadWriteLockImpl__) */
