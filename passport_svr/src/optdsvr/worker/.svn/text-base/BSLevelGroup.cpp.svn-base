//
//  BSLevelGroup.cpp
//  optd_srv
//
//  Created by Wayne on 11/7/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "BSLevelGroup.h"
#include "../manager/BSFactory.h"
#include "../manager/BSLevelGroupInfoManager.h"
#include "LoggedUser.h"
#include "BSTool.h"
#include "vnVariableAccessor.h"
#include "../manager/DC_DataBase.h"
BSLevelGroup::BSLevelGroup(const BSLevelGroupInfo *_info)
: info(_info) {
    for (auto &i : info->levels) {
        levels.insert({i.first, i.second});
    }
}

BSLevelGroup::LevelInfo * BSLevelGroup::win(u32 levelId, u32 stars, bool watch, bool shovel, const std::vector<u32> &heros, const std::vector<std::pair<u32, u32>> &towers, const std::vector<std::pair<u32, u32>> &platforms, bool &firstVictory, size_t &firstReward) {
    auto it = levels.find(levelId);
    if (it == levels.end()) {
        return nullptr;
    }
    
    auto &info = it->second;
    //新的
    if (!starsCompleted) {
        info.stars = stars;
    }
    if (!watchCompleted) {
        info.watch = watch;
    }
    if (!shovelCompleted) {
        info.shovel = shovel;
    }
    info.heros = heros;
    info.towers = towers;
    info.platforms = platforms;
    
    if (!info.passed) {
        firstVictory = true;
        info.passed = 1;
    } else {
        firstVictory = false;
    }
    //判断组是否过关
    size_t starsCount = 0, watchCount = 0, shovelCount = 0, passedCount = 0;
    for (auto &i : levels) {
        if (i.second.stars == 3) {
            ++starsCount;
        }
        if (i.second.watch) {
            ++watchCount;
        }
        if (i.second.shovel) {
            ++shovelCount;
        }
        if (i.second.stars) {
            ++passedCount;
        }
    }
    //全部过关了levelgroup passed
    if (!passed && passedCount == levels.size()) {
        passed = true;
    }
    
    if (!starsCompleted && starsCount == levels.size()) {
        starsCompleted = true;
    }
    
    if (!watchCompleted && watchCount == levels.size()) {
        watchCompleted = true;
    }
    
    if (!shovelCompleted && shovelCount == levels.size()) {
        shovelCompleted = true;
    }
    
    firstReward = rewardsCount;
    
    if (starsCount > rewardsCount) {
        rewardsCount = starsCount;
    }
    

    return &info;
}


BSLevelGroup::LevelInfo * BSLevelGroup::reset(u32 levelId) {
    auto it = levels.find(levelId);
    if (it == levels.end()) {
        return nullptr;
    }
    
    auto &info = it->second;
    
    if (!starsCompleted) {
        info.stars = 0;
    }
    
    if (!watchCompleted) {
        info.watch = false;
    }
    
    if (!shovelCompleted) {
        info.shovel = false;
    }
    
    info.heros.clear();
    info.towers.clear();
    info.platforms.clear();
    
    if (intrusion && info.passed == 1) {
        ++intrusion;
        info.passed = 2;
    }
    
    return &info;
}

void BSLevelGroup::refresh() {
    size_t starsCount = 0, watchCount = 0, shovelCount = 0;
    const size_t levelCount = levels.size();
    intrusion = 0;
    for (auto &i : levels) {
        if (i.second.stars == 3) {
            ++starsCount;
        }
        if (i.second.watch) {
            ++watchCount;
        }
        if (i.second.shovel) {
            ++shovelCount;
        }
        if (i.second.passed == 2) {
            ++intrusion;
        }
    }
    
    if (starsCount == levelCount) {
        starsCompleted = true;
    }
    
    if (watchCount == levelCount) {
        watchCompleted = true;
    }
    
    if (shovelCount == levelCount) {
        shovelCompleted = true;
    }
    
    if (!passed) {
        size_t passedCount = 0;
        for (auto &i : levels) {
            if (i.second.stars) {
                ++passedCount;
            }
        }
        if (passedCount == levelCount) {
            passed = true;
        }
    }
}

void BSLevelGroupManager::save(DataStream &stream) const {
    stream << DataStream::ccu(m_groups.size());
    for (auto &i : m_groups) {
        stream << DataStream::ccu(i.second.info->groupId);
        stream << i.second.passed;
        stream << DataStream::ccu(i.second.rewardsCount);
        stream << DataStream::ccu(i.second.intrusionTimes);
        stream << DataStream::ccu(i.second.levels.size());
        for (auto &j : i.second.levels) {
            stream << DataStream::ccu(j.second.level->f_LevelId.value) << j.second.passed << DataStream::ccu(j.second.stars) << j.second.watch << j.second.shovel << j.second.resolved;
            stream << DataStream::ccu(j.second.heros.size());
            for (auto k : j.second.heros) {
                stream << DataStream::ccu(k);
            }
            stream << DataStream::ccu(j.second.towers.size());
            for (auto &k : j.second.towers) {
                stream << DataStream::ccu(k.first) << DataStream::ccu(k.second);
            }
            stream << DataStream::ccu(j.second.platforms.size());
            for (auto &k : j.second.platforms) {
                stream << DataStream::ccu(k.first) << DataStream::ccu(k.second);
            }
        }
    }
}

void BSLevelGroupManager::load(u8 version, DataStream &stream) {
    size_t group_size;
    auto &mgr = BSLevelGroupInfoManager::instance();
    stream >> DataStream::cu(group_size);
    for (size_t i = 0; i < group_size; ++i) {
        u32 groupId;
        stream >> DataStream::cu(groupId);
        auto &group = m_groups.insert({groupId, mgr.findGroup(groupId)}).first->second;
        if (version >= 3) {
            stream >> group.passed;
        }
        stream >> DataStream::cu(group.rewardsCount);
        stream >> DataStream::cu(group.intrusionTimes);
        size_t level_size;
        stream >> DataStream::cu(level_size);
        for (size_t j = 0; j < level_size; ++j) {
            BSLevelGroup::LevelInfo li;
            u32 levelId;
            stream >> DataStream::cu(levelId) >> li.passed >> DataStream::cu(li.stars) >> li.watch >> li.shovel >> li.resolved;
            size_t size;
            stream >> DataStream::cu(size);
            li.heros.resize(size);
            for (auto &k : li.heros) {
                stream >> DataStream::cu(k);
            }
            stream >> DataStream::cu(size);
            li.towers.resize(size);
            for (auto &k : li.towers) {
                stream >> DataStream::cu(k.first) >> DataStream::cu(k.second);
            }
            stream >> DataStream::cu(size);
            li.platforms.resize(size);
            for (auto &k : li.platforms) {
                stream >> DataStream::cu(k.first) >> DataStream::cu(k.second);
            }
            auto it = group.levels.find(levelId);
            if (it != group.levels.end()) {
                it->second.passed = li.passed;
                it->second.stars = li.stars;
                it->second.watch = li.watch;
                it->second.shovel = li.shovel;
                it->second.heros = li.heros;
                it->second.towers = li.towers;
                it->second.platforms = li.platforms;
            }
        }
        group.refresh();
        if (group.info) {
            if (group.intrusion) {
                m_user->bsGoldMine()._setStatus(group.info->mineId, groupId, true, 1);
            } else if (group.starsCompleted) {
                m_user->bsGoldMine()._setStatus(group.info->mineId, groupId, true, 2);
            }
        }
    }
}

void BSLevelGroupManager::save(Variable_object &vobj) const {
    auto p = vnnew Variable_object();
    vobj.setAttribute("level_groups", p);
    for (auto &i : m_groups) {
        auto gi = vnnew Variable_object();
        p->add(gi);
        
        gi->setAttribute("id", vnnew Variable_int32(i.second.info->groupId));
        gi->setAttribute("passed", vnnew Variable_bool(i.second.passed));
        gi->setAttribute("rewards_count",vnnew Variable_int32( (u32)i.second.rewardsCount));
        gi->setAttribute("intrusion_count", vnnew Variable_int32(i.second.intrusionTimes));
        
        auto q = vnnew Variable_object();
        gi->setAttribute("levels", q);
        for (auto &j : i.second.levels) {
            auto li = vnnew Variable_object();
            q->add(li);
            li->setAttribute("id", vnnew Variable_int32(j.second.level->f_LevelId.value));
            li->setAttribute("status", vnnew Variable_int32((u32)j.second.passed));
            li->setAttribute("stars",vnnew Variable_int32(j.second.stars));
            li->setAttribute("shovel",vnnew Variable_bool(j.second.shovel));
            li->setAttribute("watch", vnnew Variable_bool(j.second.watch));
            li->setAttribute("resolved", vnnew Variable_bool(j.second.resolved));
            auto r = vnnew Variable_object();
            li->setAttribute("heros", r);
            for (auto k : j.second.heros) {
                r->add(vnnew Variable_int32(k));
            }
            r = vnnew Variable_object();
            li->setAttribute("platforms", r);
            for (auto &k : j.second.platforms) {
                r->add(vnnew Variable_int32(k.first));
                r->add(vnnew Variable_int32(k.second));
            }
        }
    }
}

void BSLevelGroupManager::load(Variable_object &vobj) {
    m_groups.clear();
    auto &mgr = BSLevelGroupInfoManager::instance();
    VariableAccessor<Variable_object> va(vobj.queryAttributeObject("level_groups"));
    while (va.fetch()) {
        auto gi = va.at<0>();
        u32 groupId = gi->queryAttributeInt32("id");
        auto info = mgr.findGroup(groupId);
        if (!info) {
            continue;
        }
        auto &group = m_groups.insert({groupId, info}).first->second;
        group.passed = gi->queryAttributeBool("passed");
        group.rewardsCount = gi->queryAttributeInt32("rewards_count");
        group.intrusionTimes = gi->queryAttributeInt32("intrusion_count");
        
        VariableAccessor<Variable_object> vb(gi->queryAttributeObject("levels"));
        while (vb.fetch()) {
            auto li = vb.at<0>();
            u32 levelId = li->queryAttributeInt32("id");
            auto it = group.levels.find(levelId);
            if (it == group.levels.end()) {
                continue;
            }
            it->second.passed = (u8)li->queryAttributeInt32("status");
            it->second.stars = li->queryAttributeInt32("stars");
            it->second.shovel = li->queryAttributeBool("shovel");
            it->second.watch = li->queryAttributeBool("watch");
            it->second.resolved = li->queryAttributeBool("resolved");
            VariableAccessor<Variable_int32> vc(li->queryAttributeObject("heros"));
            while (vc.fetch()) {
                it->second.heros.push_back(vc.at<0>()->value());
            }
            VariableAccessor<Variable_int32, Variable_int32> vd(li->queryAttributeObject("platforms"));
            while (vd.fetch()) {
                it->second.platforms.push_back({vd.at<0>()->value(), vd.at<1>()->value()});
            }
        }
        group.refresh();
        if (group.intrusion) {
            m_user->bsGoldMine()._setStatus(group.info->mineId, groupId, true, 1);
        } else if (group.starsCompleted) {
            m_user->bsGoldMine()._setStatus(group.info->mineId, groupId, true, 2);
        }
    }
}






void	BSLevelGroupManager::load(PreparedQueryResult groupResult)
{
	if(!groupResult->GetRowCount())
	{
		return;
	}
	m_groups.clear();
	auto &mgr = BSLevelGroupInfoManager::instance();
	do
	{
		vn::u32 groupId =(*groupResult)[0].GetUInt32();
		 auto info = mgr.findGroup(groupId);
		if (!info)
		{
			continue;
		}
		auto &group = m_groups.insert({groupId, info}).first->second;
		group.passed = (*groupResult)[1].GetBool();
		group.intrusion =  (*groupResult)[2].GetUInt32();
		group.rewardsCount = (*groupResult)[2].GetUInt32();
		PreparedQueryResult levelResult= ((DC_DataBase *)&DataCenter::instance())->LoadLevelGroupLevels(groupId);
		if(!levelResult || !levelResult->GetRowCount() )
		{
			continue;
		}
		do
		{
			vn::u32 levelId =(*levelResult)[0].GetUInt32();
			 auto it = group.levels.find(levelId);
			if (it == group.levels.end())
			{
				continue;
			}
			it->second.resolved =(*levelResult)[3].GetBool();
			it->second.shovel = (*levelResult)[4].GetBool();
			it->second.stars = (*levelResult)[5].GetUInt32();
			it->second.passed = (*levelResult)[6].GetUInt8();
			it->second.watch =(*levelResult)[7].GetBool();
			std::string strHeros = (*levelResult)[1].GetString();
			std::string strPlatform =(*levelResult)[2].GetString();
			it->second.ConvertStrToHeroArray(strHeros);
			it->second.ConvertStrToPlatforms(strPlatform);

		}while(levelResult->NextRow());
	}while(groupResult->NextRow());
}







bool BSLevelGroupManager::win(u32 levelId, u32 stars, bool watch, bool shovel, const std::vector<u32> &heroes, const std::vector<std::pair<u32, u32>> &towers, const std::vector<std::pair<u32, u32>> &platforms, bool *intrusion,bool *ifNew,vn::s32 *pAddDiamond) {
    auto p = BSFactory::instance().find_level(levelId);
    if (!p || !p->f_LevelGroupId.value) {
        return false;
    }
    const BSLevelGroupInfo *info = BSLevelGroupInfoManager::instance().findGroup(p->f_LevelGroupId.value);
    if (!info) {
        return false;
    }
    //找到关卡id 对应的关卡组id
    auto it = m_groups.find(info->groupId);
    if (it == m_groups.end())
    {
    	//新的关卡组
        it = m_groups.insert({info->groupId, info}).first;
        //添加一个关卡组
        if(ifNew)
        {
        	*ifNew = true;
        }
    }
    else
    {
    	if(ifNew)
    	{
    		*ifNew = false;
    	}
    }
    bool firstVictory;
    size_t firstReward;
    BSLevelGroup &group = it->second;
    bool _completed = group.starsCompleted;
    auto level = group.win(levelId, stars, watch, shovel, heroes, towers, platforms, firstVictory, firstReward);
    if (!level) {
        return false;
    }
    if (firstVictory) {
        m_user->bsPlayerBaseInfo().addByType(level->level->f_FirstPassRewardType.value, level->level->f_FirstPassRewardValue.value,pAddDiamond);
        m_user->bsPlayerBaseInfo().addChipByType(level->level->f_FirstChipType.value, level->level->f_FirstChipValue.value);
    }
    if (firstReward < group.rewardsCount) {
        auto &reward = group.info->rewards[firstReward];
        switch (reward.type) {
            case 1:
                m_user->bsPlayerBaseInfo().addMoney(reward.value);
                break;
            case 2:
                //m_user->bsPlayerBaseInfo().addDiamond(reward.value);
                if(pAddDiamond)
                {
                	s32 old =*pAddDiamond;
                	*pAddDiamond =old+reward.value;
                }
                break;
            case 3:
                m_user->bsPlayerBaseInfo().addChipByType(1, reward.value);
                break;
            case 4:
                m_user->bsPlayerBaseInfo().addChipByType(2, reward.value);
                break;
            case 5:
                m_user->bsPlayerBaseInfo().addChipByType(3, reward.value);
                break;
            default:
                m_user->bsItemStore()._add(reward.type, reward.value);
                break;
        }
    }
    if (intrusion) {
        *intrusion = group.intrusion != 0;
    }
    if (group.intrusion) {
        if (level->passed == 2) {
            level->passed = 1;
            if (!level->resolved) {
                level->resolved = true;
                switch (group.intrusionTimes) {
                    case 1:
                        m_user->bsPlayerBaseInfo().addByType(level->level->f_Reward1Type.value, level->level->f_Reward1Value.value,pAddDiamond);
                        m_user->bsPlayerBaseInfo().addChipByType(level->level->f_Reward1ChipType.value, level->level->f_Reward1ChipValue.value);
                        break;
                    case 2:
                        m_user->bsPlayerBaseInfo().addByType(level->level->f_Reward2Type.value, level->level->f_Reward2Value.value,pAddDiamond);
                        m_user->bsPlayerBaseInfo().addChipByType(level->level->f_Reward2ChipType.value, level->level->f_Reward2ChipValue.value);
                        break;
                    case 3:
                        m_user->bsPlayerBaseInfo().addByType(level->level->f_Reward3Type.value, level->level->f_Reward3Value.value,pAddDiamond);
                        m_user->bsPlayerBaseInfo().addChipByType(level->level->f_Reward3ChipType.value, level->level->f_Reward3ChipValue.value);
                        break;
                }
            }
            if (!--group.intrusion) {
                m_user->bsGoldMine().notifyVictory(info->mineId, info->groupId, true);
                	//计算入侵
                m_user->bsIntrusion().intrusion();
            }
        }
    } else if (_completed != group.starsCompleted) {
        m_user->bsGoldMine().notifyVictory(info->mineId, info->groupId, true);
        m_user->bsIntrusion().intrusion();
    }
    //如果是新的关卡组插入关卡组，并插入关卡组的所有关卡

    //如果是老的关卡组，直接更新关卡组，关卡


    //跟新基本信息
    return true;
}



//插入新的关卡组
void BSLevelGroupManager::AsyncsaveNew(u32 levelId,std::function<void(int ret)>doResult)
{
		auto p = BSFactory::instance().find_level(levelId);
		if (!p || !p->f_LevelGroupId.value)
		{
			return doResult(-1);
		}

		const BSLevelGroupInfo *info = BSLevelGroupInfoManager::instance().findGroup(p->f_LevelGroupId.value);
		if (!info)
		{
			return doResult(-1);
		}
		auto it = m_groups.find(info->groupId);
		u32 groupId=info->groupId;
		if (it == m_groups.end())
		{
			return doResult(-1);
		}
		BSLevelGroup &group = it->second;
		bool passed=group.passed;
		u32 intrusionCount = group.intrusionTimes;
		u32 rewardCount = group.rewardsCount;
		int num=group.levels.size()+1;
		AsyncTaskPtr task=vnnew AsyncTask();
		task->Init(num);
		((DC_DataBase *)&DataCenter::instance())->AsyncInsertLevelGroups({m_user,true},groupId,passed,intrusionCount,rewardCount,[this,doResult,groupId,levelId,task](bool ret)
		{
			if(!ret)
			{
				VN_LOG_ERROR("user:"<<m_user->GetUUId()<<"insert"<<"groupid:"<<groupId<<"failed");
			}
			task->FinishTask(ret);
			_AsyncsaveNewHandl(task,doResult);
		});
		for(auto &level:group.levels)
		{
			//把hero数组变成字符串
			std::string strHero;
			ConvertHeroArrayToString(level.second.heros,strHero);
			std::string strPlatform;
			ConvertPlatformArrayToString(level.second.platforms,strPlatform);
			u32 levelId = level.first;
			((DC_DataBase *)&DataCenter::instance())->AsyncInsertLevelGroup_Levels({m_user,true},groupId,levelId,strHero,strPlatform,level.second.resolved,level.second.shovel,level.second.stars,level.second.passed,level.second.watch,[this,groupId,levelId,task,doResult](bool ret)
			{
				if(!ret)
				{
					VN_LOG_ERROR("UUID:"<<m_user->GetUUId()<<"groupid:"<<groupId<<"levelId"<<levelId<<"insert failed");
				}
				task->FinishTask(ret);
				_AsyncsaveNewHandl(task,doResult);
			});
		}
}
void	BSLevelGroupManager::_AsyncsaveNewHandl(AsyncTaskPtr task,std::function<void(int ret)>doResult)
{
	//全部插入完成
	if(!task->IsFinished())
	{
		return;
	}
	int size = task->TaskSize();
	bool result = true;
	for(int i=0;i<size;++i)
	{
		if(task->at(i) == false)
		{
			result = false;
			break;
		}
	}
	return doResult(result);
}



//跟新关卡信息
void BSLevelGroupManager::Asyncsave(u32 levelId,std::function<void(int ret)>doResult)
{
	auto p = BSFactory::instance().find_level(levelId);
	if (!p || !p->f_LevelGroupId.value)
	{
		return doResult(-1);
	}

	const BSLevelGroupInfo *info = BSLevelGroupInfoManager::instance().findGroup(p->f_LevelGroupId.value);
	if (!info)
	{
		return doResult(-1);
	}
	auto it = m_groups.find(info->groupId);
	u32 groupId=info->groupId;
	if (it == m_groups.end())
	{
		return doResult(-1);
	}
	//找到关卡组信息
	BSLevelGroup &group = it->second;
	bool passed=group.passed;
	u32 intrusionCount = group.intrusionTimes;
	u32 rewardCount = group.rewardsCount;
	//找到关卡信息
	 auto iter = group.levels.find(levelId);
	    if (iter == group.levels.end()) {
	        return doResult(-1);
	    }
	auto &level = iter->second;
	std::string _strHero="";
	ConvertHeroArrayToString(level.heros,_strHero);
	std::string _strPlatform="";
	ConvertPlatformArrayToString(level.platforms,_strPlatform);
	bool _resolved = level.resolved;
	bool _shovel =level.shovel;
	u32 _stars=level.stars;
	u8 _passed=level.passed;
	bool _watch=level.watch;
	   //更新关卡组，指定关卡id的关卡信息
	((DC_DataBase *)&DataCenter::instance())->AsyncUpdateLevelGroups({m_user,true},{m_user,true},groupId,passed,intrusionCount,rewardCount,[this,doResult,groupId,levelId,_strHero,_strPlatform,_resolved,_shovel,_stars,_passed,_watch](bool result){
		if(!result)
		{
			VN_LOG_ERROR("user:"<<m_user->GetUUId()<<"update"<<"groupid:"<<groupId<<"failed");
			return doResult(false);
		}
		//更新关卡
		((DC_DataBase *)&DataCenter::instance())->AsyncUpdateLevelGroup_Levels({m_user,true},{m_user,true},groupId,levelId,_strHero,_strPlatform,_resolved,_shovel,_stars,_passed,_watch,[this,groupId,levelId,doResult](bool result)
		{
			if(!result)
			{
				VN_LOG_ERROR("user:"<<m_user->GetUUId()<<"update"<<"groupid:"<<groupId<<"levelid:"<<levelId<<"failed");
			}
			return doResult(result);
		});
	});


}

/*
void BSLevelGroupManager::Asyncwin(u32 levelId, u32 stars, bool watch, bool shovel, const std::vector<u32> &heroes, const std::vector<std::pair<u32, u32>> &towers, const std::vector<std::pair<u32, u32>> &platforms, bool *intrusion,std::function<void(int ret)> doResult)
{

}
*/




void	BSLevelGroupManager::_AsyncInsertLevelToGroupHandl(bool ret,u32 groupId,u32 levelId,AsyncTaskPtr task,std::function<void(int ret)> doResult)
{
	if(!ret)
	{
		VN_LOG_ERROR("UUId:"<<m_user->GetUUId()<<"groupid:"<<groupId<<"levelId:"<<levelId<<"insert failed");
	}
	task->FinishTask(ret);
	//全部插入完成
	if(task->IsFinished())
	{
		int size = task->TaskSize();
		bool result = true;
		for(int i=0;i<size;++i)
		{
			if(task->at(i) == false)
			{
				result = false;
				break;
			}
		}
		return doResult(result);
	}
}




bool BSLevelGroupManager::reset(u32 levelId) {
    auto p = BSFactory::instance().find_level(levelId);
    if (!p || !p->f_LevelGroupId.value) {
        return false;
    }
    const BSLevelGroupInfo *info = BSLevelGroupInfoManager::instance().findGroup(p->f_LevelGroupId.value);
    if (!info) {
        return false;
    }
    auto it = m_groups.find(info->groupId);
    if (it == m_groups.end()) {
        it = m_groups.insert({info->groupId, info}).first;
    }
    it->second.reset(levelId);
    return true;
}

void BSLevelGroupManager::intrude(u32 groupId) {
    auto it = m_groups.find(groupId);
    if (it == m_groups.end()) {
        return ;
    }
    it->second.intrusion = (u32)it->second.levels.size();
    ++it->second.intrusionTimes;
    for (auto &i : it->second.levels) {
        i.second.passed = 2;
        i.second.resolved = false;
        i.second.heros.clear();
        i.second.towers.clear();
        i.second.platforms.clear();
    }
    
    if (!it->second.shovelCompleted) {
        for (auto &i : it->second.levels) {
            i.second.shovel = false;
        }
    }
    
    if (!it->second.watchCompleted) {
        for (auto &i : it->second.levels) {
            i.second.watch = false;
        }
    }
    m_user->bsGoldMine().notifyIntrusion(it->second.info->mineId, groupId, true);
}



bool BSLevelGroupManager::Syncintrude(u32 groupId)
{
    auto it = m_groups.find(groupId);
    if (it == m_groups.end())
    {
        return true;
    }
    it->second.intrusion = (u32)it->second.levels.size();
    ++it->second.intrusionTimes;
    for (auto &i : it->second.levels)
    {
        i.second.passed = 2;
        i.second.resolved = false;
        i.second.heros.clear();
        i.second.towers.clear();
        i.second.platforms.clear();
    }

    if (!it->second.shovelCompleted)
    {
        for (auto &i : it->second.levels)
        {
            i.second.shovel = false;
        }
    }

    if (!it->second.watchCompleted)
    {
        for (auto &i : it->second.levels)
        {
            i.second.watch = false;
        }
    }
    std::string strUUId = m_user->GetUUId();
    if(!((DC_DataBase *)&DataCenter::instance())->SyncUpdateLevelGroups(strUUId,groupId,it->second.passed,it->second.intrusionTimes,it->second.rewardsCount))
    {
    	VN_LOG_ERROR("UUid:"<<strUUId<<"groupId"<<groupId<<"UpdateLevelGroups FAILED");
    	return false;
    }
    for (auto &i : it->second.levels)
	{
		 u32  levelId = i.first;
		//u32 mineId =it->second.info->mineId;
		std::string newHeros= "";
		std::string newPlatforms="";
		if(!((DC_DataBase *)&DataCenter::instance())->SyncUpdateLevelGroup_Levels(strUUId,groupId,levelId,newHeros,newPlatforms,i.second.resolved,i.second.shovel,i.second.stars,i.second.passed,i.second.watch))
		{
			VN_LOG_ERROR("UUid:"<<strUUId<<"groupId"<<groupId<<"SyncUpdateLevelGroup_Levels FAILED");
			return false;
		}
	}
    m_user->bsGoldMine().notifyIntrusion(it->second.info->mineId, groupId, true);
    return true;
}



void BSLevelGroupManager::Asyncintrude(AsyncObjectPtr asyncObjPtr,u32 groupId,std::function<void(bool ret)> doResult)
{
    auto it = m_groups.find(groupId);
    if (it == m_groups.end())
    {
        return doResult(true);
    }
    it->second.intrusion = (u32)it->second.levels.size();
    ++it->second.intrusionTimes;
    for (auto &i : it->second.levels) {
        i.second.passed = 2;
        i.second.resolved = false;
        i.second.heros.clear();
        i.second.towers.clear();
        i.second.platforms.clear();
    }

    if (!it->second.shovelCompleted) {
        for (auto &i : it->second.levels) {
            i.second.shovel = false;
        }
    }

    if (!it->second.watchCompleted) {
        for (auto &i : it->second.levels) {
            i.second.watch = false;
        }
    }
    std::string strUUId = m_user->GetUUId();
    //先异步更新levelgroup
    ((DC_DataBase *)&DataCenter::instance())->AsyncUpdateLevelGroups(asyncObjPtr,{m_user,true},groupId,it->second.passed,it->second.intrusionTimes,it->second.rewardsCount,[this,strUUId,groupId,doResult,asyncObjPtr](bool ret){
      	if(!ret)
  		{
      		VN_LOG_ERROR("UUID:"<<strUUId<<"groupid"<<groupId<<"intrude failed");
      		return doResult(false);
  		}
        auto it = m_groups.find(groupId);
        if(it == m_groups.end())
		{
			return doResult(false);
		}
      int levelNum= it->second.levels.size();
  	   //在异步更新groupid 下面的所有Level
      	AsyncTaskPtr task=vnnew AsyncTask();
      	task->Init(levelNum);
      	 for (auto &i : it->second.levels)
		 {
      		 u32  levelId = i.first;
      		u32 mineId =it->second.info->mineId;
      		std::string newHeros= "";
      		std::string newPlatforms="";
			((DC_DataBase *)&DataCenter::instance())->AsyncUpdateLevelGroup_Levels(asyncObjPtr,{m_user,true},groupId,levelId,newHeros,newPlatforms,i.second.resolved,i.second.shovel,i.second.stars,i.second.passed,i.second.watch,std::bind(&BSLevelGroupManager::_AsyncintrudeHandl,this,std::placeholders::_1,task,doResult,groupId,levelId,mineId));
		 }
      });

}

void BSLevelGroupManager::_AsyncintrudeHandl(bool ret,AsyncTaskPtr task,std::function<void(bool ret)> doResult,u32 groupId,u32 levelId,u32 mineId)
{
	//插入失败
	if(!ret)
	{
		VN_LOG_ERROR("UUId:"<<m_user->GetUUId()<<"groupid:"<<groupId<<"levelid:"<<levelId<<"Asyncintrude");
	}
	m_user->bsGoldMine().notifyIntrusion(mineId, groupId, true);
	task->FinishTask(ret);
	if(task->IsFinished())
	{
		int size = task->TaskSize();
		bool result = true;
		for(int i=0;i<size;++i)
		{
			if(task->at(i) == false)
			{
				result = false;
				break;
			}
		}
		return doResult(result);
	}
}







void BSLevelGroupManager::clear() {
    m_groups.clear();
}

bool BSLevelGroupManager::passed(u32 groupId) {
    auto it = m_groups.find(groupId);
    if (it == m_groups.end()) {
        return false;
    }
    return it->second.passed;
}

bool BSLevelGroupManager::intrusion(u32 groupId) {
    auto it = m_groups.find(groupId);
    if (it == m_groups.end()) {
        return false;
    }
    return it->second.intrusion != 0;
}

u32 BSLevelGroupManager::checkGroupStatus(u32 groupId) {
    //0 not passed, 1 passed, 2 intrusion, 3 passedWithAllStars 100 error
    auto it = m_groups.find(groupId);
    if (it == m_groups.end()) {
        return 100;
    }
    
    if (it->second.intrusion) {
        return 2;
    }
    if (it->second.passed) {
        if (it->second.starsCompleted) {
            return 3;
        }
        return 1;
    }
    
    return 100;
}

