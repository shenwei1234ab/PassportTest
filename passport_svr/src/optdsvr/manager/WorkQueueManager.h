//
//  WorkQueueManager.h
//  optd_srv
//
//  Created by Wayne on 6/19/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__WorkQueueManager__
#define __optd_srv__WorkQueueManager__

#include "vnSingleton.h"
#include "../worker/WorkQueue.h"

#include <vector>

class WorkQueueManager : public vn::Singleton<WorkQueueManager> {
public:
    void init(size_t numWorkQueues);
    void fini();
    optd::WorkQueue * selectWorkQueue();
private:
    std::vector<std::unique_ptr<optd::WorkQueue>> m_queues;
};

#endif /* defined(__optd_srv__WorkQueueManager__) */
