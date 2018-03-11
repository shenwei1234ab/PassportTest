//
//  BSMonsterMap.cpp
//  optd_srv
//
//  Created by Cloverz on 10/13/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "LoggedUser.h"
#include "../manager/BSFactory.h"
#include "BSMonsterMap.h"
#include "vnVariableAccessor.h"
#include "../manager/DC_DataBase.h"
BSMonsterMap::BSMonsterMap(LoggedUser *user)
: m_user(user) {}

BSMonsterMap::~BSMonsterMap()
{
    m_user = 0;
    m_monsterMap.clear();
}

void BSMonsterMap::init()
{
    m_monsterMap.clear();
    
    tbl::Table_monster &monster = BSFactory::instance().monster();
    for (uint i = 0; i < monster.size(); ++i)
    {
        const tbl::Record_monster *rm = monster.at(i);
        
        if (rm && (rm->f_Flags.value & 1) == 0)
        {
            m_monsterMap.insert({rm->f_MonsterID.value, false});
        }
        
    }
}

void BSMonsterMap::load(u32 version, DataStream &stream)
{
    m_monsterMap.clear();
    
    size_t size = 0;
    stream >> DataStream::cu(size);
    for (size_t i = 0; i < size; ++i)
    {
        u32 id;
        bool meet;
        stream >> id >> meet;
        m_monsterMap.insert({id, meet});
    }
}

void BSMonsterMap::save(DataStream &stream) const
{
    stream << DataStream::ccu(m_monsterMap.size());
    for (auto &i : m_monsterMap)
    {
        stream << i.first << i.second;
    }
}

void BSMonsterMap::save(Variable_object &vobj) const {
    auto p = vnnew Variable_object();
    vobj.setAttribute("monster_map", p);
    for (auto &i : m_monsterMap) {
        p->add(vnnew Variable_int32(i.first));
        p->add(vnnew Variable_bool(i.second));
    }
}

void BSMonsterMap::load(Variable_object &vobj) {
    m_monsterMap.clear();
    VariableAccessor<Variable_int32, Variable_bool> item(vobj.queryAttributeObject("monster_map"));
    while (item.fetch()) {
        m_monsterMap[item.at<0>()->value()] = item.at<1>()->value();
    }
}


void BSMonsterMap::load(PreparedQueryResult result )
{
	m_monsterMap.clear();
	do
	{
		  s32 monterid = (*result)[0].GetInt32();
		  bool monsterstatus = (*result)[1].GetBool();
		  m_monsterMap[monterid] = monsterstatus;
	}while(result->NextRow());
}


void  BSMonsterMap::unmeet_monster(u32 id)
{
	auto i = m_monsterMap.find(id);
	if (i != m_monsterMap.end())
	{
		i->second = false;
	}
}

bool BSMonsterMap::meet_monster(u32 id)
{
    auto i = m_monsterMap.find(id);
    if (i != m_monsterMap.end())
    {
        i->second = true;
        return true;
    }
    
    return false;
}

void BSMonsterMap::Asyncsave(u32 id,std::function<void(bool ret)>doResult)
{
	auto it = m_monsterMap.find(id);
	if (it == m_monsterMap.end())
	{
		return doResult(false);
	}
	bool monsterStatus = it->second;
	((DC_DataBase *)&DataCenter::instance())->AsyncUpdateMonsterMap({m_user,true},id,monsterStatus,doResult);
}



std::map<u32, bool> & BSMonsterMap::monsterMap() {
    return m_monsterMap;
}
