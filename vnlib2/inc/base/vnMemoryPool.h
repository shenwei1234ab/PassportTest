//
//  vnMemoryPool.h
//  vnlib
//
//  Created by Wayne on 5/27/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnMemoryPool_h
#define vnlib_vnMemoryPool_h

#include "vnMemory.h"
#include "vnSpinLock.h"
#include "vnAssert.h"

_vn_begin

template <size_t _size>
class HeapMemPool {
public:
    template <bool, size_t>
    struct _SizeTrait;
    template <size_t size> struct _SizeTrait<true, size> {
        enum { kSize = size };
    };
    template <size_t size> struct _SizeTrait<false, size> {
        enum { kSize = sizeof(void *) };
    };
    
    enum { kSize = _SizeTrait<(_size > sizeof(void *)), _size>::kSize };
    
    ~HeapMemPool() {
        //vnassert(m_total == m_unused);
        while (m_list) {
            void *p = m_list;
            m_list = (void **)*m_list;
            vnfree(p);
        }
    }
    
    void * alloc() {
        m_lock.lock();
        if (m_list) {
            --m_unused;
            void *p = m_list;
            m_list = (void **)*m_list;
            m_lock.unlock();
            return p;
        }
        ++m_total;
        m_lock.unlock();
        return vnmalloc(u8, kSize);
    }
    
    void free(void *ptr) {
        m_lock.lock();
        if (m_unused == m_maxUnused) {
            --m_total;
            m_lock.unlock();
            vnfree(ptr);
            return ;
        }
        ++m_unused;
        *(void **)ptr = m_list;
        m_list = (void **)ptr;
        m_lock.unlock();
    }
    
    size_t total() const { return m_total; }
    size_t unused() const { return m_unused; }
    size_t maxUnused() const { return m_maxUnused; }
    void setMaxUnused(size_t _max) {
        m_maxUnused = _max;
        m_lock.lock();
        while (_max < m_unused) {
            void *p = m_list;
            m_list = (void **)*m_list;
            --m_unused;
            vnfree(p);
        }
        m_lock.unlock();
    }
    
private:
    size_t m_total = 0, m_unused = 0, m_maxUnused = 0xFF;
    void **m_list = 0;
    SpinLock m_lock;
};

template <typename T>
class ObjectHeapMemPool : public HeapMemPool<sizeof(T)> {
public:
    T * create() {
        return new(this->alloc()) T();
    }
    
    T * create(const T &obj) {
        return new (this->alloc()) T(obj);
    }
    
    T * create(T &&obj) {
        return new (this->alloc()) T(std::move(obj));
    }
    
    void destroy(T *ptr) {
        ptr->~T();
        free(ptr);
    }
};

_vn_end

#endif
