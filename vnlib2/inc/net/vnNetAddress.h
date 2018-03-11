//
//  vnNetAddress.h
//  vnlib
//
//  Created by Wayne on 4/28/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef vnlib_vnNetAddress_h
#define vnlib_vnNetAddress_h

#include "../vnDefine.h"

_vn_begin

union IpAddress {
    u32 u;
    u8 b[4];
    struct {
        u8 b0, b1, b2, b3;
    };
    
    IpAddress() : u(0) {}
    IpAddress(u32 _u) : u(_u) {}
    
    operator u32 &() { return u; }
    operator const u32 &() const { return u; }
};

struct NetAddress {
    IpAddress ip;
    u16 port = 0;
    
    NetAddress() = default;
    NetAddress(u32 _ip, u16 _port) : ip(_ip), port(_port) {}
    
    bool from_string(const c8 *addr);
    str8 to_string() const;
    
    bool operator <(const NetAddress &) const;
    bool operator ==(const NetAddress &) const;
    bool operator !=(const NetAddress &) const;
};

_vn_end

#endif
