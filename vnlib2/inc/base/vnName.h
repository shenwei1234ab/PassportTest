//
//  vnName.h
//  vnlib
//
//  Created by Wayne on 15/6/27.
//  Copyright (c) 2015年 viichi.com. All rights reserved.
//

#ifndef vnlib_vnName_h
#define vnlib_vnName_h

#include "vnAllocatedObject.h"

#include <functional>

_vn_begin

class Name : public AllocatedObject {
public:
	Name();
	Name(const str8 &name);
	Name(const Name &);
	~Name();
	
	const str8 & str() const;
	
	long tag() const;
	void setTag(long _tag) const;
	
	long idx() const { return m_index; }
	
	bool operator ==(const Name &rhs) const {
		return m_index == rhs.m_index;
	}
	
	bool operator !=(const Name &rhs) const {
		return m_index != rhs.m_index;
	}
	
	bool operator <(const Name &rhs) const {
		return m_index < rhs.m_index;
	}
	
private:
	long m_index;
};

_vn_end

namespace std {

	template <>
	struct hash<_vn_ns::Name> : public unary_function<const _vn_ns::Name &, size_t> {
		size_t operator ()(const _vn_ns::Name &__v) const {
			return static_cast<size_t>(__v.idx());
		}
	};

}

#endif
