//
//  DateTime.h
//  optd_srv
//
//  Created by Wayne on 1/29/15.
//  Copyright (c) 2015 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__DataTime__
#define __optd_srv__DataTime__

#include "vnAllocatedObject.h"

using namespace vn;

struct Date {
    union {
        struct {
            u32 day : 8;
            u32 month : 8;
            u32 year : 16;
        };
        u32 date;
    };
    
    Date() : date(0) {}
    
    bool operator <(const Date &) const;
    bool operator ==(const Date &) const;
    bool operator !=(const Date &) const;
    
    bool convertFromString(const str8 &s);
    str8 convertToString() const;
    
    static Date today();
};

struct Time {
    union {
        struct {
            u32 second : 8;
            u32 minute : 8;
            u32 hour : 8;
        };
        u32 time;
    };
    
    Time() : time(0) {}
    
    bool operator <(const Time &) const;
    bool operator ==(const Time &) const;
    bool operator !=(const Time &) const;
    
    bool convertFromString(const str8 &s);
    str8 convertToString() const;
    
    static Time now();
};

struct DateTime : Time, Date {
    bool operator <(const DateTime &) const;
    bool operator ==(const DateTime &) const;
    bool operator !=(const DateTime &) const;
    
    bool convertFromString(const str8 &s);
    str8 convertToString() const;
    
    static DateTime now();
};

#endif /* defined(__optd_srv__DataTime__) */
