//
//  vnSingleton.h
//  vnlib
//
//  Created by Wayne on 5/12/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnSingleton_h
#define vnlib_vnSingleton_h

#include "vnSingletonManager.h"
#include "vnSpinLock.h"

_vn_begin

template <class T>
struct CreateUsingNew {
    static T * create() {
        return vnnew T;
    }
    static void destroy(T *p) {
        delete p;
    }
};

template <
    class T,
    class CreationPolicy = CreateUsingNew<T>,
    class MutexType = SpinLock
>
class Singleton : public NonCopyable, public AllocatedObject {
public:
    static T & instance() {
        if (!ms_instance) {
            createInstance();
        }
        return *ms_instance;
    }
    
    static void createInstance() {
        std::lock_guard<MutexType> lck(_instanceLock());
        if (!ms_instance) {
            ms_instance = CreationPolicy::create();
			SingletonManager::instance()._register(&destroyInstance);
        }
    }
    
    static void destroyInstance() {
        std::lock_guard<MutexType> lck(_instanceLock());
        if (ms_instance) {
            CreationPolicy::destroy(ms_instance);
            ms_instance = 0;
        }
    }
    
protected:
    static MutexType & _instanceLock() {
        static MutexType s_mutex;
        return s_mutex;
    }
    
private:
    static T *ms_instance;
};

template <class T, class CP, class M>
T * Singleton<T, CP, M>::ms_instance = 0;

_vn_end

#endif
