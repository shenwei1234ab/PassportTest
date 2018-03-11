//
//  vnName.cpp
//  vnlib
//
//  Created by Wayne on 15/6/27.
//  Copyright (c) 2015年 viichi.com. All rights reserved.
//

#include "vnName.h"
#include "vnRefCounted.h"
#include "vnSingleton.h"

#include <map>

_vn_begin

namespace internal {
	
	class NameItem : public RefCounted {
	public:
		NameItem(const str8 &_name) : name(_name) {}
		virtual ~NameItem();
		
		const str8 &name;
		long tag = 0;
	};
	
	class NameManager : public Singleton<NameManager> {
	public:
		NameItem * getName(const str8 &name) {
			auto ret = m_items.insert({name, nullptr});
			if (!ret.second) {
				ret.first->second->grab();
			} else {
				ret.first->second = vnnew NameItem(ret.first->first);
			}
			return ret.first->second;
		}
		
		void _removeName(const str8 &name) {
			m_items.erase(name);
		}
		
	private:
		std::map<str8, NameItem *> m_items;
	};
	
	NameItem::~NameItem() {
		NameManager::instance()._removeName(name);
	}
}

Name::Name()
: m_index((long)internal::NameManager::instance().getName(str8())) {
	
}

Name::Name(const str8 &name)
: m_index((long)internal::NameManager::instance().getName(name)) {
	
}

Name::Name(const Name &rhs)
: m_index(rhs.m_index) {
	reinterpret_cast<internal::NameItem *>(m_index)->grab();
}

Name::~Name() {
	reinterpret_cast<internal::NameItem *>(m_index)->drop();
}

const str8 & Name::str() const {
	return reinterpret_cast<internal::NameItem *>(m_index)->name;
}

long Name::tag() const {
	return reinterpret_cast<internal::NameItem *>(m_index)->tag;
}

void Name::setTag(long _tag) const {
	reinterpret_cast<internal::NameItem *>(m_index)->tag = _tag;
}



_vn_end
