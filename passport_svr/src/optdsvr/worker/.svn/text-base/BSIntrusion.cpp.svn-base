//
//  BSIntrusion.cpp
//  optd_srv
//
//  Created by Cloverz on 9/12/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "BSIntrusion.h"
#include "LoggedUser.h"
#include "vnLog.h"
#include "../WorldManager.h"
#include "../manager/DC_DataBase.h"
BSIntrusion::BSIntrusion(LoggedUser *user)
: m_user(user)
, m_running(false)
, m_timer(0)
{
    m_timer =sWorldManager.intrusion_interval();
    //srand((unsigned)time(0));
}

void BSIntrusion::init()
{
    m_timer = 0;
    m_running = false;
}

void BSIntrusion::load(u32 version, DataStream &stream)
{
    stream >> m_timer >> m_running;
}

void BSIntrusion::save(DataStream &stream) const
{
    stream << m_timer << m_running;
}

void BSIntrusion::save(Variable_object &vobj) const {
    auto p = vnnew Variable_object();
    vobj.setAttribute("intrusion", p);
    p->setAttribute("timer", vnnew Variable_float(m_timer));
    p->setAttribute("running",vnnew Variable_bool( m_running));
}


void BSIntrusion::Asyncsave(AsyncObjectPtr asyncObjPtr,std::function<void(bool)> doResulet)
{
	std::string strUUId = m_user->GetUUId();
	vn::u64 curTime = time(NULL);
	((DC_DataBase *)&DataCenter::instance())->AsyncUpdateIntrusion(strUUId,m_running,m_timer,curTime,asyncObjPtr,doResulet);
}


void BSIntrusion::load(Variable_object &vobj) {
    auto p = vobj.queryAttributeObject("intrusion");
    if (p) {
        m_timer = p->queryAttributeFloat("timer");
        m_running = p->queryAttributeBool("running");
    }
}



void BSIntrusion::load(PreparedQueryResult baseinfo)
{
	m_running= (*baseinfo)[16].GetBool();
	m_timer=(*baseinfo)[17].GetFloat();
	m_lastIntrusionTimeStamp=(*baseinfo)[18].GetUInt64();
}

void BSIntrusion::update(f32 deltaTime)
{
    if (m_running)
    {
        m_timer -= deltaTime;
        m_user->_markDirty();
        if (m_timer <= 0)
        {
            m_timer = sWorldManager.intrusion_interval();	//300s
            _intrusion();
            //更新数据库
        }
    }
}


void BSIntrusion::Asyncupdate(f32 deltaTime,std::function<void(bool ret)> doResult)
{
    if (m_running)
    {
        m_timer -= deltaTime;
        m_user->_markDirty();
        if (m_timer <= 0)
        {
            m_timer = sWorldManager.intrusion_interval();	//300s
           // _intrusion();
            //更新数据库
            Async_intrusion({m_user,true},[doResult](bool ret)
			 {
				return doResult(ret);
			 });
        }
        else
        {
        	return doResult(true);
        }
    }
    else
    {
    	return	doResult(true);
    }
}


void BSIntrusion::intrusion()
{
    if (!m_running) {
        if (m_user->bsLevel().find(sWorldManager.intrusion_beginLevel())) {
        		//数据更新
            m_user->_markDirty();
			  m_running = true;
        }
    }
}


void BSIntrusion::_intrusion()
{
    std::vector<std::pair<u32, bool>> levels;
    for (auto &i : m_user->bsLevel().levelList()) {
        auto &info = *i.second;
        if (info.starNum == 3 && !info.intrusion && info.intrusionTimes < 3) {
            levels.push_back({i.first, false});
        }
    }
    
    for (auto &i : m_user->bsLevelGroupMgr().groups()) {
        auto &info = i.second;
        if (info.starsCompleted && !info.intrusion && info.intrusionTimes < 3) {
            levels.push_back({i.first, true});
        }
    }
    if (levels.empty()) {
        m_running = false;
        return ;
    }
    //随机选择一个关卡入侵
    size_t idx = rand() % levels.size();
    auto &ret = levels[idx];
    if (ret.second)
    {
        m_user->bsLevelGroupMgr().intrude(ret.first);
    }
    else
    {
        m_user->bsLevel().intrude(ret.first);
    }
    m_user->send_intrusion(ret.first, ret.second);
}

bool BSIntrusion::Sync_intrusion()
{
	bool bret=true;
	std::vector<std::pair<u32, bool>> levels;
	for (auto &i : m_user->bsLevel().levelList()) {
		auto &info = *i.second;
		if (info.starNum == 3 && !info.intrusion && info.intrusionTimes < 3) {
			levels.push_back({i.first, false});
		}
	}

	for (auto &i : m_user->bsLevelGroupMgr().groups()) {
		auto &info = i.second;
		if (info.starsCompleted && !info.intrusion && info.intrusionTimes < 3) {
			levels.push_back({i.first, true});
		}
	}
	if (levels.empty()) {
		m_running = false;
		return true;
	}
	//随机选择一个关卡入侵
	size_t idx = rand() % levels.size();
	auto &ret = levels[idx];
	if (ret.second)
	{
		if(!m_user->bsLevelGroupMgr().Syncintrude(ret.first))
		{
			bret = false;
		}
	}
	else
	{
		if(!m_user->bsLevel().Syncintrude(ret.first))
		{
			bret = false;
		}
	}
	/*if(bret)
	{
		m_user->send_intrusion(ret.first, ret.second);
	}*/
	return	bret;
}







//改变关卡的入侵状态和金矿产量
void BSIntrusion::Async_intrusion(AsyncObjectPtr asyncObjPtr,std::function<void(bool ret)> doResult)
{
	std::vector<std::pair<u32, bool>> levels;
	    for (auto &i : m_user->bsLevel().levelList())
	    {
	        auto &info = *i.second;
	        if (info.starNum == 3 && !info.intrusion && info.intrusionTimes < 3) {
	            levels.push_back({i.first, false});
	        }
	    }
	    for (auto &i : m_user->bsLevelGroupMgr().groups())
	    {
	        auto &info = i.second;
	        if (info.starsCompleted && !info.intrusion && info.intrusionTimes < 3) {
	            levels.push_back({i.first, true});
	        }
	    }
	    if (levels.empty())
	    {
	    	//不用入侵
	        m_running = false;
	        return doResult(true);
	    }
	    size_t idx = rand() % levels.size();
	    auto &ret = levels[idx];
	    //组入侵
	    if (ret.second)
	    {
	    	u32 levelId =ret.first;
	    	bool group  = ret.second;
	        //m_user->bsLevelGroupMgr().intrude(ret.first);
	        //写入数据库
	        m_user->bsLevelGroupMgr().Asyncintrude(asyncObjPtr,ret.first,[this,levelId,group,doResult](bool ret)
			{
				if(!ret)
				{
					return doResult(ret);
				}
				m_user->send_intrusion(levelId, group);
				return doResult(ret);
			});

	    }
	    //单个关卡入侵
	    else
	    {
	    	u32 levelId =ret.first;
			bool group  = ret.second;
	        //m_user->bsLevel().intrude(ret.first);
	        //写入数据库
	    	m_user->bsLevel().Asyncintrude(asyncObjPtr,ret.first,[this,levelId,group,doResult](bool ret)
			{
				if(!ret)
				{
					return doResult(ret);
				}
				m_user->send_intrusion(levelId, group);
				return doResult(ret);
			});
	    }
	   // m_user->send_intrusion(ret.first, ret.second);
}
















