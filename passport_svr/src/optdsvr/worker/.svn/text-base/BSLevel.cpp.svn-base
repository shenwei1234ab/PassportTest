//
//  BSLevelInfo.cpp
//  optd_srv
//
//  Created by Cloverz on 8/19/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "BSLevel.h"
#include "LoggedUser.h"
#include "../manager/BSFactory.h"
#include "vnVariableAccessor.h"
#include "../manager/DC_DataBase.h"
BSLevel::BSLevel(LoggedUser *user) :m_user(user) {}

BSLevel::~BSLevel()
{
    _clear();
}

void BSLevel::init()
{
    _clear();
}

void BSLevel::load(u32 version, DataStream &stream)
{
    u32 size = 0;
    stream >> size;
    for (uint i = 0; i < size; ++i)
    {
        u32 levelId;
        stream >> levelId;
        LevelInfo *li = vnnew LevelInfo;
        stream >> li->starNum >> li->score >> li->shovel >> li->watch;
        if (version >= 2) {
            stream >> li->intrusion >> DataStream::cu(li->intrusionTimes);
        }
        auto level = BSFactory::instance().find_level(levelId);
        if (level) {
            li->level = level;
            m_levelList.insert(std::make_pair(levelId, li));
            if (li->intrusion) {
                m_user->bsGoldMine()._setStatus(level->f_MineId.value, levelId, false, 1);
            } else if (li->starNum == 3) {
                m_user->bsGoldMine()._setStatus(level->f_MineId.value, levelId, false, 2);
            }
        } else {
            delete li;
        }
    }
}

void BSLevel::save(DataStream &stream) const
{
    stream << (u32)m_levelList.size();
    for (auto &i : m_levelList)
    {
        stream << i.first << i.second->starNum << i.second->score << i.second->shovel << i.second->watch;
        stream << i.second->intrusion << DataStream::ccu(i.second->intrusionTimes);
    }
}

void BSLevel::save(Variable_object &vobj) const {
    auto levels = vnnew Variable_object();
    for (auto &i : m_levelList) {
        auto p = vnnew Variable_object();
        levels->add(p);
        p->setAttribute("id",  vnnew Variable_int32(i.first));
        p->setAttribute("stars",vnnew Variable_int32( i.second->starNum));
        p->setAttribute("score", vnnew Variable_int32(i.second->score));
        p->setAttribute("shovel", vnnew Variable_bool(i.second->shovel));
        p->setAttribute("watch", vnnew Variable_bool(i.second->watch));
        p->setAttribute("intrusion", vnnew Variable_bool(i.second->intrusion));
        p->setAttribute("intrusion_times", vnnew Variable_int32(i.second->intrusionTimes));
    }
    vobj.setAttribute("levels", levels);
}
void BSLevel::AsyncInsert(u32 levelId,std::function<void(bool ret)> doResult)
{
	auto iter = m_levelList.find(levelId);
	if(iter == m_levelList.end())
	{
		return doResult(false);
	}
	bool intrusion = iter->second->intrusion;
	u32 intrusiontimes = iter->second->intrusionTimes;
	u32 starNum = iter->second->starNum ;
	s32 score = iter->second->score;
	bool shovel = iter->second->shovel;
	bool watch = iter->second->watch;
	std::string strUUid = m_user->GetUUId();
	((DC_DataBase *)&DataCenter::instance())->AsyncInsertLevel({m_user,true},strUUid,levelId,intrusion,intrusiontimes,starNum,score,shovel,watch,doResult);
}

void BSLevel::AsyncUpdate(u32 levelId,std::function<void(bool ret)> doResult)
{
	auto iter = m_levelList.find(levelId);
		if(iter == m_levelList.end())
		{
			return doResult(false);
		}
		bool intrusion = iter->second->intrusion;
		u32 intrusiontimes = iter->second->intrusionTimes;
		u32 starNum = iter->second->starNum ;
		s32 score = iter->second->score;
		bool shovel = iter->second->shovel;
		bool watch = iter->second->watch;
		std::string strUUid = m_user->GetUUId();
		((DC_DataBase *)&DataCenter::instance())->AsyncUpdateLevel({m_user,true},strUUid,levelId,intrusion,intrusiontimes,starNum,score,shovel,watch,doResult);
}


void BSLevel::load(Variable_object &vobj) {
    VariableAccessor<Variable_object> level(vobj.queryAttributeObject("levels"));
    while (level.fetch()) {
        auto p = level.at<0>();
        u32 levelId = p->queryAttributeInt32("id");
        auto info = BSFactory::instance().find_level(levelId);
        if (info) {
            auto li = vnnew LevelInfo();
            li->level = info;
            li->starNum = p->queryAttributeInt32("stars");
            li->score = p->queryAttributeInt32("score");
            li->shovel = p->queryAttributeBool("shovel");
            li->watch = p->queryAttributeBool("watch");
            li->intrusion = p->queryAttributeBool("intrusion");
            li->intrusionTimes = p->queryAttributeInt32("intrusion_times");
            m_levelList[levelId] = li;
            
            if (li->intrusion) {
                m_user->bsGoldMine()._setStatus(info->f_MineId.value, levelId, false, 1);
            } else if (li->starNum == 3) {
                m_user->bsGoldMine()._setStatus(info->f_MineId.value, levelId, false, 2);
            }
        }
    }
}

void BSLevel::load(PreparedQueryResult result)
{
	if(!result->GetRowCount())
	{
		return;
	}
	do
	{
		u32 levelid=(*result)[0].GetInt32();
		bool instrusion=(*result)[1].GetBool();
		u32 instrTimes=(*result)[2].GetUInt32();
		s32 scores=(*result)[3].GetInt32();
		s32 shovel=(*result)[4].GetInt32();
		s32 stars=(*result)[5].GetInt32();
		s32 watch=(*result)[6].GetInt32();
		BSLevel::LevelInfo *li=vnnew BSLevel::LevelInfo;
		li->shovel = shovel;
		li->watch = watch;
		li->score = scores;
		li->starNum = stars;
		auto info = BSFactory::instance().find_level(levelid);
		if (info)
		{
			auto li = vnnew BSLevel::LevelInfo();
			li->level = info;
			li->starNum = stars;
			li->score = scores;
			li->shovel = shovel;
			li->watch = watch;
			li->intrusion = instrusion;
			li->intrusionTimes = instrTimes;
			//user->bsLevel().levelList().insert(std::make_pair(levelid,li));
			m_levelList[levelid] = li;
			if (li->intrusion)
			{
				m_user->bsGoldMine()._setStatus(info->f_MineId.value, levelid, false, 1);
			} else if (li->starNum == 3)
			{
				m_user->bsGoldMine()._setStatus(info->f_MineId.value, levelid, false, 2);
			}
		}
	}while(result->NextRow());
}



//bool BSLevel::update_levelInfo(u32 id, u32 starNum, s32 score, bool shovel, bool watch)
//{
//    const tbl::Record_level *data = BSFactory::instance().find_level(id);
//    return update_levelInfo(data, starNum, score, shovel, watch);
//}

bool BSLevel::update_levelInfo(const tbl::Record_level *data, u32 starNum, s32 score, bool shovel, bool watch, bool *intrusion,bool *ifNew,s32 *addDiamond) {
    if (!data)
    {
        return false;
    }
    u32 id = data->f_LevelId.value;
    LevelInfo *&li = m_levelList.insert({id, nullptr}).first->second;
    u32 _stars = 0;
    //新的关卡
    if (!li)
    {
        li = vnnew LevelInfo();
        li->level = data;
        li->starNum = starNum;
        li->score = score;
        li->shovel = shovel;
        li->watch = watch;
		if(ifNew)
		{
			*ifNew = true;
		}

        //获取奖励
        m_user->bsPlayerBaseInfo().addByType(data->f_FirstPassRewardType.value, data->f_FirstPassRewardValue.value,addDiamond);
        m_user->bsPlayerBaseInfo().addChipByType(data->f_FirstChipType.value, data->f_FirstChipValue.value);
        //
        if (li->starNum == 3) {
            m_user->bsGoldMine().notifyVictory(data->f_MineId.value, id, false);
        }

        //获取优惠券
        m_user->_notify_levelPassed(*li);

        if (intrusion) {
            *intrusion = false;
        }
    }
    //老关卡
   else
   {
		if(ifNew)
			{
				*ifNew = false;
			}
        _stars = li->starNum;
        if (li->starNum < starNum) {
            li->starNum = starNum;
        }

        if (li->score < score) {
            li->score = score;
        }

        if (shovel) {
            li->shovel = true;
        }

        if (watch) {
            li->watch = true;
        }
        if (intrusion) {
            *intrusion = li->intrusion;
        }
        if (li->intrusion) {
            li->intrusion = false;
            switch (li->intrusionTimes) {
                case 1:
                    m_user->bsPlayerBaseInfo().addByType(data->f_Reward1Type.value, data->f_Reward1Value.value,addDiamond);
                    m_user->bsPlayerBaseInfo().addChipByType(data->f_Reward1ChipType.value, data->f_Reward1ChipValue.value);
                    break;
                case 2:
                    m_user->bsPlayerBaseInfo().addByType(data->f_Reward2Type.value, data->f_Reward2Value.value,addDiamond);
                    m_user->bsPlayerBaseInfo().addChipByType(data->f_Reward2ChipType.value, data->f_Reward2ChipValue.value);
                    break;
                case 3:
                    m_user->bsPlayerBaseInfo().addByType(data->f_Reward3Type.value, data->f_Reward3Value.value,addDiamond);
                    m_user->bsPlayerBaseInfo().addChipByType(data->f_Reward3ChipType.value, data->f_Reward3ChipValue.value);
                    break;
            }
            m_user->bsGoldMine().notifyVictory(data->f_MineId.value, id, false);
        } else if (_stars != 3 && li->starNum == 3) {
            m_user->bsGoldMine().notifyVictory(data->f_MineId.value, id, false);
        }
    }
    return true;
}


//参数错误-1，数据库插入失败0  成功1
void BSLevel::Asyncupdate_levelInfo(const tbl::Record_level *data, u32 starNum, s32 score, bool shovel, bool watch, bool *intrusion,std::function<void(int ret)> doResult)
{
	/*if (!data)
	{
		return doResult(-1);
	}
    u32 id = data->f_LevelId.value;
    LevelInfo *&li = m_levelList.insert({id, nullptr}).first->second;
    u32 _stars = 0;

    //新的关卡
    if (!li)
    {
		li = vnnew LevelInfo();
		li->level = data;
		li->starNum = starNum;
		li->score = score;
		li->shovel = shovel;
		li->watch = watch;
		//获取奖励
		m_user->bsPlayerBaseInfo().addByType(data->f_FirstPassRewardType.value, data->f_FirstPassRewardValue.value);
		m_user->bsPlayerBaseInfo().addChipByType(data->f_FirstChipType.value, data->f_FirstChipValue.value);
		//
		if (li->starNum == 3)
		{
			m_user->bsGoldMine().notifyVictory(data->f_MineId.value, id, false);
		}
		//获取优惠券
		m_user->_notify_levelPassed(*li);

		if (intrusion)
		{
			*intrusion = false;
		}
		std::string strUUId= m_user->GetUUId();
        //插入levelinfo
       ((DC_DataBase *)&DataCenter::instance())->AsyncInsertLevel({m_user,true},strUUId,id,0,0,starNum,score,shovel,watch,[doResult](bool ret)
		{
    	   	   return doResult(ret);
		});
    }
    //老关卡
   else
   {
		_stars = li->starNum;
		if (li->starNum < starNum)
		{
		  li->starNum = starNum;
		}

		if (li->score < score)
		{
		  li->score = score;
		}

		if (shovel)
		{
		  li->shovel = true;
		}

		if (watch)
		{
		  li->watch = true;
		}
		if (intrusion)
		{
		  *intrusion = li->intrusion;
		}
		//击退入侵
		if (li->intrusion)
		{
		  li->intrusion = false;
		  switch (li->intrusionTimes)
		  {
			  case 1:
				  m_user->bsPlayerBaseInfo().addByType(data->f_Reward1Type.value, data->f_Reward1Value.value);
				  m_user->bsPlayerBaseInfo().addChipByType(data->f_Reward1ChipType.value, data->f_Reward1ChipValue.value);
				  break;
			  case 2:
				  m_user->bsPlayerBaseInfo().addByType(data->f_Reward2Type.value, data->f_Reward2Value.value);
				  m_user->bsPlayerBaseInfo().addChipByType(data->f_Reward2ChipType.value, data->f_Reward2ChipValue.value);
				  break;
			  case 3:
				  m_user->bsPlayerBaseInfo().addByType(data->f_Reward3Type.value, data->f_Reward3Value.value);
				  m_user->bsPlayerBaseInfo().addChipByType(data->f_Reward3ChipType.value, data->f_Reward3ChipValue.value);
				  break;
		  }
		  //回复产量
		  m_user->bsGoldMine().notifyVictory(data->f_MineId.value, id, false);
		}
		else if (_stars != 3 && li->starNum == 3)
		{
		  m_user->bsGoldMine().notifyVictory(data->f_MineId.value, id, false);
		}
		std::string strUUid = m_user->GetUUId();
		//更新levelinfo
		((DC_DataBase *)&DataCenter::instance())->AsyncUpdateLevel({m_user,true},strUUid,id,0,0,starNum,score,shovel,watch,[doResult](bool ret)
		{
			  return doResult(ret);
		});
    }*/
}





BSLevel::LevelInfo * BSLevel::find(u32 id) {
    auto it = m_levelList.find(id);
    if (it != m_levelList.end())
        return it->second;

    return 0;
}

std::map<u32, BSLevel::LevelInfo *> & BSLevel::levelList() {
    return m_levelList;
}

void BSLevel::intrude(u32 levelId) {
    auto it = m_levelList.find(levelId);
    if (it == m_levelList.end()) {
        return ;
    }
    auto &info = *it->second;
    info.intrusion = true;
    ++info.intrusionTimes;
    //改变金矿产量
    m_user->bsGoldMine().notifyIntrusion(info.level->f_MineId.value, levelId, false);
}


bool BSLevel::Syncintrude(u32 levelId)
{
    auto it = m_levelList.find(levelId);
    if (it == m_levelList.end())
    {
        return false;
    }
    auto &info = *it->second;
    info.intrusion = true;
    ++info.intrusionTimes;
    std::string strUUId=m_user->GetUUId();
    //改变金矿产量
    m_user->bsGoldMine().notifyIntrusion(info.level->f_MineId.value, levelId, false);
    //保存level到数据库
    return  ((DC_DataBase *)&DataCenter::instance())->SyncUpdateLevel(strUUId,levelId,info.intrusion,info.intrusionTimes,info.starNum,info.score,info.shovel,info.watch);
}






void BSLevel::Asyncintrude(AsyncObjectPtr asyncObjPtr,u32 levelId,std::function<void(bool ret)> doResult)
{
    auto it = m_levelList.find(levelId);
    if (it == m_levelList.end())
    {
        return doResult(true);
    }
    auto &info = *it->second;
    info.intrusion = true;
    ++info.intrusionTimes;
    u32 minId =info.level->f_MineId.value;
    std::string strUUId =	m_user->GetUUId();
    ((DC_DataBase *)&DataCenter::instance())->AsyncUpdateLevel(asyncObjPtr,strUUId,levelId,info.intrusion,info.intrusionTimes,info.starNum,info.score,info.shovel,info.watch,[this,levelId,doResult,minId](bool ret){
    	if(!ret)
		{
    		VN_LOG_ERROR("UUID:"<<m_user->GetUUId()<<"levelid"<<levelId<<"intrude failed");
    		return doResult(false);
		}
	   //改变金矿产量
		m_user->bsGoldMine().notifyIntrusion(minId, levelId, false);
		return doResult(true);
    });
}








bool BSLevel::passed(u32 levelId) {
    auto it = m_levelList.find(levelId);
    if (it == m_levelList.end()) {
        return false;
    }
    return it->second->starNum != 0;
}

bool BSLevel::intrusion(u32 levelId) {
    auto it = m_levelList.find(levelId);
    if (it == m_levelList.end()) {
        return false;
    }
    return it->second->intrusion;
}

void BSLevel::pass_level(u32 param) {
    auto &tbl = BSFactory::instance().level();
    size_t index = 0;
    while (auto *p = tbl.at(index++)) {
        if (p->f_LevelId.value > param) {
            break;
        }
        if (p->f_LevelGroupId.value) {
            m_user->bsLevelGroupMgr().win(p->f_LevelId.value, 3, true, true, {}, {}, {});
        } else {
            update_levelInfo(p, 3, 0, true, true);
        }
    }
}

//-------------------------
void BSLevel::_clear()
{
    for (auto &i : m_levelList) {
        VN_SAFE_DELETE(i.second);
    }
    m_levelList.clear();
}
