//
//  BSForceGuide.cpp
//  optd_srv
//
//  Created by Cloverz on 10/23/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "LoggedUser.h"
#include "BSForceGuide.h"
#include "vnVariableAccessor.h"

BSForceGuide::BSForceGuide(LoggedUser *user)
: m_user(user) {}

BSForceGuide::~BSForceGuide()
{
    m_disableList.clear();
}

void BSForceGuide::init()
{
    m_disableList.clear();
}

void BSForceGuide::load(u32 version, DataStream &stream)
{
    m_disableList.clear();
    
    u32 size = 0;
    stream >> size;
    for (uint i = 0; i < size; ++i)
    {
        u32 data = 0;
        stream >> data;
        m_disableList.insert(data);
    }
}

void BSForceGuide::save(DataStream &stream) const
{
    stream << (u32)m_disableList.size();
    for (auto &i : m_disableList)
    {
        stream << i;
    }
}

void BSForceGuide::save(Variable_object &vobj) const {
    auto p = vnnew Variable_object();
    vobj.setAttribute("force_guide", p);
    for (auto i : m_disableList) {
        p->add(vnnew Variable_int32(i));
    }
}

void BSForceGuide::load(Variable_object &vobj)
{
    m_disableList.clear();
    VariableAccessor<Variable_int32> item(vobj.queryAttributeObject("force_guide"));
    while (item.fetch()) {
        m_disableList.insert(item.at<0>()->value());
    }
}

void BSForceGuide::load(PreparedQueryResult result)
{
	if(!result->GetRowCount())
	{
		return;
	}
	m_disableList.clear();
	do
	{
		u32 forceid = (*result)[0].GetUInt32();
		  m_disableList.insert(forceid);
	}while(result->NextRow());
}

void BSForceGuide::erase(u32 data)
{
	m_disableList.erase(data);
}

bool BSForceGuide::insert(u32 data) {
    return m_disableList.insert(data).second;
}

const std::set<u32> & BSForceGuide::disableList()
{
    return m_disableList;
}
