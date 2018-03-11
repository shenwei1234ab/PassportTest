//
//  AsyncObject.h
//  demo_svr
//
//  Created by Wayne on 7/17/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __demo_svr__AsyncObject__
#define __demo_svr__AsyncObject__

#include "vnRefCounted.h"

#include <functional>

using namespace vn;

class AsyncObject;

typedef RefCountedPtr<AsyncObject> AsyncObjectPtr;

class AsyncObject : public RefCounted {
public:
    virtual void async(std::function<void()>) = 0;
    void async(AsyncObjectPtr obj, std::function<void()> fn) {
        this->async(std::bind([](const RefCountedPtr<AsyncObject> &, std::function<void()> &fn){
            fn();
        }, std::move(obj), std::move(fn)));
    }
};



#endif /* defined(__demo_svr__AsyncObject__) */
