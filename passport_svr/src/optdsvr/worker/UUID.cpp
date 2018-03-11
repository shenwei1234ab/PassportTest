//
//  UUID.cpp
//  optd_srv
//
//  Created by Wayne on 6/20/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "UUID.h"

#include <stdlib.h>

UUID::UUID()
{
//  ::uuid_clear(m_uuid);
}

UUID::UUID(u64 low, u64 high)
: m_uuid_low(low), m_uuid_high(high) {
    
}

UUID::UUID(const str8 &s)
{
//	if (::uuid_parse(s.c_str(), m_uuid)) {
//	        ::uuid_clear(m_uuid);
//	    }
}

UUID::UUID(const UUID &other)
{
//   ::uuid_copy(m_uuid, other.m_uuid);
}

void UUID::generate() {
//   ::uuid_generate(m_uuid);
}

void UUID::clear()
{
//   ::uuid_clear(m_uuid);

}

int UUID::compare(const UUID &other) const {
//    return ::uuid_compare(m_uuid, other.m_uuid);
	return 0;
}

str8 UUID::to_string() const
{
    char s[37];
//    uuid_unparse_lower(m_uuid, s);
    return {s, 36};
}

bool UUID::null() const {
//    return uuid_is_null(m_uuid) != 0;
	return true;

}
