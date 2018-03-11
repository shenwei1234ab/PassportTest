//
//  BSPlayerBaseInfo.cpp
//  optd_srv
//
//  Created by Cloverz on 8/18/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "../worker/BSPlayerBaseInfo.h"

#include "../worker/LoggedUser.h"
#include "vnLog.h"
#include "../manager/DC_DataBase.h"
BSPlayerBaseInfo::BSPlayerBaseInfo(LoggedUser *user)
: m_money(0)
, m_diamond(0)
, m_user(user) {
    for (int i = 0 ; i<3 ; ++i) {
        m_chip[i] = 0;
    }
}



BSPlayerBaseInfo::BSPlayerBaseInfo(LoggedUser *user,s32 money,s32 diamond,s32 clip0,s32 clip1,s32 clip2)
:m_money(money)
,m_diamond(diamond)
,m_user(user)
{
	 m_chip[0] = clip0;
	 m_chip[1] = clip1;
	 m_chip[2] = clip2;
}
BSPlayerBaseInfo::~BSPlayerBaseInfo()
{
    m_user = 0;
}

void BSPlayerBaseInfo::init(vn::s32 diamond)
{
    m_money = 0;
   m_diamond = diamond;
    for (int i = 0 ; i<3 ; ++i) {
        m_chip[i] = 0;
    }
}


void BSPlayerBaseInfo::load(u32 version, DataStream &stream)
{
    if (version <= 1) {
        s32 hp;
        f32 time;
        stream >> hp >> m_money >> m_diamond >> time;
    } else {
        stream >> m_money >> m_diamond >> m_chip[0] >> m_chip[1] >> m_chip[2];
    }

}

void BSPlayerBaseInfo::save(DataStream &stream) const
{
    stream << m_money << m_diamond << m_chip[0] << m_chip[1] << m_chip[2];
}

void BSPlayerBaseInfo::save(Variable_object &vobj) const {
    auto p = vnnew Variable_object();
    p->setAttribute("money",vnnew Variable_int32 (m_money));
    p->setAttribute("diamonds", vnnew Variable_int32 (m_diamond));
    p->setAttribute("clip0", vnnew Variable_int32 (m_chip[0]));
    p->setAttribute("clip1", vnnew Variable_int32 (m_chip[1]));
    p->setAttribute("clip2", vnnew Variable_int32 (m_chip[2]));
    vobj.setAttribute("base_info", p);
}



void BSPlayerBaseInfo::load(Variable_object &vobj) {
    auto p = vobj.queryAttributeObject("base_info");
    if (p) {
        m_money = p->queryAttributeInt32("money");
        m_diamond = p->queryAttributeInt32("diamonds");
        m_chip[0] = p->queryAttributeInt32("clip0");
        m_chip[1] = p->queryAttributeInt32("clip1");
        m_chip[2] = p->queryAttributeInt32("clip2");
    }
}


void BSPlayerBaseInfo::load(PreparedQueryResult baseinfo,vn::s32 balance)
{
	m_chip[0] = (*baseinfo)[0].GetInt32();
	m_chip[1] = (*baseinfo)[1].GetInt32();
	m_chip[2] = (*baseinfo)[2].GetInt32();
	m_money = (*baseinfo)[3].GetInt32();
	//m_diamond = (*baseinfo)[4].GetInt32();
	m_diamond = balance;
}

void BSPlayerBaseInfo::load(DBTable_baseinfo &tbaseinfo)
{
		m_money = tbaseinfo.money;
		m_diamond = tbaseinfo.diamonds;
		m_chip[0] =tbaseinfo.chip[0];
		m_chip[1] = tbaseinfo.chip[1];
		m_chip[2] = tbaseinfo.chip[2];
}




void BSPlayerBaseInfo::addMoney(s32 value)
{
	VN_LOG_DEBUG("uuid:"<<m_user->GetUUId()<<"addMoney:"<<value<<"oldMoney:"<<m_money);
	m_money += value;
}

void BSPlayerBaseInfo::addDiamond(s32 value)
{
	 m_diamond += value;
}

void BSPlayerBaseInfo::addChip(u32 index, s32 value)
{
	 m_chip[index] += value;
}

void BSPlayerBaseInfo::addByType(u32 type, s32 value,s32 *addDiamond)
{
    switch (type)
    {
        case 1:
        	 VN_LOG_DEBUG("uuid:"<<m_user->GetUUId()<<"addMoney:"<<value<<"oldMoney:"<<m_money);
            m_money += value;
            break;
        case 2:
            //m_diamond += value;
            if(addDiamond)
            {
            	*addDiamond =value;
            }
            break;
        case 3:
            m_chip[0] += value;
            break;
        case 4:
            m_chip[1] += value;
            break;
        case 5:
            m_chip[2] += value;
            break;
    }
}

void BSPlayerBaseInfo::addChipByType(u32 type,s32 value) {
    if (type) {
        m_chip[type-1] += value;
    }
}

s32 BSPlayerBaseInfo::money()
{
    return m_money;
}

void BSPlayerBaseInfo::SetDiamond(s32 diamond)
{
	m_diamond = diamond;
}
s32 BSPlayerBaseInfo::diamond()
{
    return m_diamond;
}

s32 BSPlayerBaseInfo::chip(u32 index) {
    vnassert(index < 3);
    return m_chip[index];
}
