//
//  WorkQueue.h
//  optd_srv
//
//  Created by Wayne on 6/19/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__WorkQueue__
#define __optd_srv__WorkQueue__

#include "vnRunLoop.h"

#include <thread>
#include <list>
#include <atomic>
#include "vnRefCounted.h"
#include "../../passportsvr/entity/AsyncObject.h"

namespace optd
{
class Worker;
typedef vn::RefCountedPtr<Worker> WorkerPtr;

class WorkQueue : public vn::AllocatedObject {
public:
    WorkQueue();
    WorkQueue(const WorkQueue &) = delete;
    ~WorkQueue();
    
    void attach(WorkerPtr worker);
    void detach(WorkerPtr worker);
    
    vn::RunLoop & runLoop() { return m_loop; }
    
    size_t numWorks() const { return m_numWorkers; }
    
    WorkQueue & operator =(const WorkQueue &) = delete;
    
    std::string strTag;
protected:
    vn::RunLoop m_loop;
    std::thread *m_thread = 0;
    std::list<WorkerPtr> m_workers;
    std::atomic_size_t m_numWorkers;
    
    void _work();
    
};

class Worker : public AsyncObject {
    friend class WorkQueue;
public:
    virtual void async(std::function<void()> fn)override
    {
    	if(!m_queue)
    	{
    		return;
    	}
    	m_queue->runLoop().post(std::move(fn));
    }
    virtual void update(vn::f32 deltaTime) {}
    virtual void shutdown() {}
    std::string  strTag;
   optd::WorkQueue * queue() const { return m_queue; }
private:
   optd::WorkQueue *m_queue = 0;
    std::list<WorkerPtr>::iterator m_iterator;
};


}

#endif /* defined(__optd_srv__WorkQueue__) */
