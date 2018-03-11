//
//  WorkQueue.cpp
//  optd_srv
//
//  Created by Wayne on 6/19/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "../worker/WorkQueue.h"
#include "vnLog.h"
using namespace optd;
WorkQueue::WorkQueue() {
    m_thread = new std::thread(&WorkQueue::_work, this);
}

WorkQueue::~WorkQueue() {
    m_loop.quit();
    m_thread->join();
    delete m_thread;
}

void WorkQueue::attach(WorkerPtr worker) {
	if(!worker->m_queue)
	{
		VN_LOG_ERROR("strTag:"<<worker->strTag);
	}

    vnassert(!worker->m_queue);
    worker->m_queue = this;
    ++m_numWorkers;
    m_loop.post(std::bind([this](WorkerPtr worker){
        m_workers.push_front(worker);
        m_workers.front()->m_iterator = m_workers.begin();
        VN_LOG_DEBUG("attach worker Num:"<<m_workers.size());
    },worker));
}

void WorkQueue::detach(WorkerPtr worker) {
//	VN_LOG_DEBUG("worker"<<worker.GetPointee()<<"Detach Ref"<<worker.use_count());
   // vnassert(worker->m_queue == this);
	m_loop.post(std::bind([this](WorkerPtr worker) {
	        if (worker->m_queue) {
	        	VN_LOG_DEBUG("workRefCount:"<<worker.use_count());
	        	if(worker.use_count() == 0)
				{
	        		VN_LOG_ERROR("strTag:"<<worker->strTag);
				}

	            --m_numWorkers;
	          /*  m_workers.erase(worker->m_iterator);
	            worker->m_queue = 0;
	            worker->shutdown();*/
	            worker->m_queue = 0;
				worker->shutdown();
	            m_workers.erase(worker->m_iterator);
	            VN_LOG_DEBUG("worker Num:"<<m_workers.size());
	        }
	    }, worker));
}

void WorkQueue::_work() {
    m_loop.run(0.5f, [this](vn::f32 deltaTime){
        for (auto &i : m_workers) {
            i->update(deltaTime);
        }
        //VN_LOG_DEBUG("worker num:"<<m_workers.size());
    });
    
    for (auto &i : m_workers) {
        i->shutdown();
    }
}

