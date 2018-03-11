//
//  PacketHandler.h
//  optd_srv
//
//  Created by Wayne on 6/19/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__PacketHandler__
#define __optd_srv__PacketHandler__


#include "../worker/WorkQueue.h"
#include "vnPacket.h"



class PacketHandler : public optd::Worker {
public:
    virtual vn::PacketPtr createRequest(vn::u32 pid) { return vn::PacketPtr(); }
    virtual vn::PacketPtr createResponse(vn::u32 pid) { return vn::PacketPtr(); }
    
    void postPacket(vn::PacketPtr &&packet);


    //网络线程处理socket close请求()
    void notify_closed();
    
protected:
    //work线程处理
    virtual void _closed() {}
};

#endif /* defined(__optd_srv__PacketHandler__) */
