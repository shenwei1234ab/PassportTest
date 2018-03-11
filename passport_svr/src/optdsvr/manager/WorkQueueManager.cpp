//
//  WorkQueueManager.cpp
//  optd_srv
//
//  Created by Wayne on 6/19/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "WorkQueueManager.h"
#include "vnLog.h"

void WorkQueueManager::init(size_t numWorkQueues) {
    m_queues.resize(numWorkQueues);
    for (auto &i : m_queues) {
        i.reset(vnnew optd::WorkQueue());
    }
}

void WorkQueueManager::fini() {
    m_queues.clear();
}

optd::WorkQueue * WorkQueueManager::selectWorkQueue() {
	optd::WorkQueue *ret = 0;
    size_t best = -1;
    for (auto &i : m_queues) {
        if (i->numWorks() < best) {
            best = i->numWorks();
            ret = i.get();
        }
    }
    return ret;
}
