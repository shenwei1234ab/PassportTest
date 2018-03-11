//
//  vnNetAddress.cpp
//  vnlib
//
//  Created by Wayne on 4/28/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "vnNetAddress.h"

_vn_begin

str8 NetAddress::to_string() const {
    c8 buf[32];
    return {buf, (size_t)sprintf(buf, "%u.%u.%u.%u:%hu", (u32)ip.b0, (u32)ip.b1, (u32)ip.b2, (u32)ip.b3, port)};
}

bool NetAddress::from_string(const c8 *addr) {
    u32 d[4];
    if (sscanf(addr, "%u.%u.%u.%u:%hu", d, d + 1, d + 2, d + 3, &port) >= 4) {
        for (int i = 0; i < 4; ++i) {
            ip.b[i] = (u8)d[i];
        }
        return true;
    }
    return false;
}

bool NetAddress::operator <(const NetAddress &rhs) const {
    if (ip.u < rhs.ip.u) {
        return true;
    }
    if (ip.u == rhs.ip.u && port < rhs.port) {
        return true;
    }
    return false;
}

bool NetAddress::operator ==(const NetAddress &rhs) const {
    return ip.u == rhs.ip.u && port == rhs.port;
}

bool NetAddress::operator !=(const NetAddress &rhs) const {
    return ip.u != rhs.ip.u || port != rhs.port;
}

_vn_end
