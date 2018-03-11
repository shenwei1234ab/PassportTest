//
//  UUID.h
//  optd_srv
//
//  Created by Wayne on 6/20/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__UUID__
#define __optd_srv__UUID__

#include "vnAllocatedObject.h"
//#include <uuid/uuid.h>
#include <stdio.h>
using namespace vn;

class uuid_t{

};
//16字节的uuid
class UUID : public AllocatedObject {
public:
    UUID();
    UUID(u64 low, u64 high);
    UUID(const str8 &s);
    UUID(const UUID &other);
    
    void generate();
    void clear();
    int compare(const UUID &other) const;
    str8 to_string() const;
    bool null() const;
    
    operator bool() const {
        return !null();
    }
    
    bool operator <(const UUID &rhs) const {
        return compare(rhs) < 0;
    }
    bool operator >(const UUID &rhs) const {
        return compare(rhs) > 0;
    }
    
    bool operator <=(const UUID &rhs) const {
        return compare(rhs) <= 0;
    }
    bool operator >=(const UUID &rhs) const {
        return compare(rhs) >= 0;
    }
    
    bool operator ==(const UUID &rhs) const {
        return compare(rhs) == 0;
    }
    
    bool operator !=(const UUID &rhs) const {
        return compare(rhs) != 0;
    }
    
    u64 low() const { return m_uuid_low; }
    u64 & low() { return m_uuid_low; }
    
    u64 high() const { return m_uuid_high; }
    u64 & high() { return m_uuid_high; }
private:
    union {
        struct {
            u64 m_uuid_low;
            u64 m_uuid_high;
        };
       uuid_t m_uuid;
    };
};

#endif /* defined(__optd_srv__UUID__) */
