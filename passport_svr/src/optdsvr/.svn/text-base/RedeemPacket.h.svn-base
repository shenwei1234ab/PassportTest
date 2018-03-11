//
//  RedeemPacket.h
//  optd_srv
//
//  Created by Wayne on 5/21/15.
//  Copyright (c) 2015 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__RedeemPacket__
#define __optd_srv__RedeemPacket__

#include "vnRefCounted.h"
#include "vnSpinLock.h"

using namespace _vn_ns;

_vn_begin

struct RedeemPacket : RefCounted {
    u32 codeId = 0;
    u32 category = 0;
    u32 money = 0;
    u32 diamonds = 0;
    
    SpinLock _lock;
    bool _valid = true;
};

typedef RefCountedPtr<RedeemPacket> RedeemPacketPtr;

_vn_end

#endif /* defined(__optd_srv__RedeemPacket__) */
