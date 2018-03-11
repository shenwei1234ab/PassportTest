//
//  ClientListen.h
//  passport_svr
//
//  Created by Wayne on 6/9/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __optd_svr__ClientListen__
#define __optd_svr__ClientListen__

#include "vnLink.h"

using namespace _vn_ns;

class ClientListen : public Link_TCP_Listen, public AllocatedObject {
public:
    virtual ~ClientListen() = default;
    static Link_TCP_Listen * create();
protected:
    virtual void _destroy();
    virtual Link_TCP_Connection * _accepted(const NetAddress &peer);
};

#endif /* defined(__passport_svr__ClientListen__) */
