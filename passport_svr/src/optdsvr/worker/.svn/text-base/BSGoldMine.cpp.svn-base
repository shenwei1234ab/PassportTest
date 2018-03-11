//
//  BSGoldMine.cpp
//  optd_srv
//
//  Created by Cloverz on 8/26/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "BSGoldMine.h"
#include "LoggedUser.h"
#include "vnLog.h"
#include "../manager/BSFactory.h"
#include "../table/Table_gold_mine.h"
#include "../manager/BSLevelGroupInfoManager.h"
#include "vnVariableAccessor.h"
#include "../manager/DC_DataBase.h"
decltype(BSGoldMine::s_initalMines) BSGoldMine::s_initalMines;

//------GoldMine------
bool BSGoldMine::GoldMine::update(f32 deltaTime)
{
    if (nowNum >= maxNum) {
        return false;
    }
    
    timer += deltaTime;
    f32 growTime = 86400.f / yield;
    
    while(1)
    {
        if (timer >= growTime)
        {
            ++nowNum;
        
            if (nowNum >= maxNum)
            {
                timer = 0;
                nowNum = maxNum;
                break;
            }
            else
            {
                timer -= growTime;
            }
        }
        else
        {
            break;
        }
    }
    return true;
}



void BSGoldMine::GoldMine::calcYield() {
    size_t count = 0;
    yield = 0;
    for (auto &i : levels) {
    	//如果关联关卡没有被入侵
        if (i.second.status == 2) {
            ++count;
            yield += i.second.value;
        }
    }
    //如果关联关卡都没有被入侵
    if (count == levels.size()) {
        yield *= yieldFactor;
    }
}

//--------------------
BSGoldMine::BSGoldMine(LoggedUser *user)
: m_user(user)
, m_mines(s_initalMines) {}


//s_initalMines 读表
bool BSGoldMine::loadInitialGoldMineList(u32 fsId, const str8 &path) {
    tbl::Table_gold_mine table;
    if (!table.init(fsId, path)) {
        return false;
    }
    for (size_t i = 0, c = table.size(); i < c; ++i) {
        auto rec = table.at(i);
        GoldMine &gm = s_initalMines[rec->f_Id.value];
        gm.id = rec->f_Id.value;
        gm.type = rec->f_Type.value;
        gm.maxNum = rec->f_Capacity.value;
        gm.yieldFactor = rec->f_Factor.value;
    }
    auto &levels = BSFactory::instance().level();
    for (size_t i = 0, c = levels.size(); i < c; ++i) {
        auto rec = levels.at(i);
        if (rec->f_MineId.value) {
            auto &rl = s_initalMines[rec->f_MineId.value].levels[{rec->f_LevelId.value, false}];
            rl.id = rec->f_MineId.value;
            rl.group = false;
            rl.value = rec->f_MineValue.value;
        }
    }
    
    for (auto &i : BSLevelGroupInfoManager::instance().groups()) {
        if (i.second.mineId) {
            auto &rl = s_initalMines[i.second.mineId].levels[{i.second.groupId, true}];
            rl.id = i.second.groupId;
            rl.group = true;
            rl.value = i.second.mineValue;
        }
    }
    
    return true;
}

void BSGoldMine::init()
{
    m_mines = s_initalMines;
}



void BSGoldMine::load(u32 version, DataStream &stream)
{
    if (version <= 1) {
        u32 size = 0;
        stream >> size;
        for (uint i = 0; i < size; ++i)
        {
            GoldMine gm;
            stream >> gm.id >> gm.type >> gm.nowNum >> gm.maxNum >> gm.yield >> gm.yieldFactor >> gm.timer;
            
            u32 count = 0;
            stream >> count;
            for (uint j = 0; j < count; ++j)
            {
                GoldMine::RelevantLevel rl;
                u32 times;
                stream >> rl.id >> times >> rl.status;
                gm.levels.insert({{rl.id, false}, rl});
            }
            
            auto it = m_mines.find(gm.id);
            if (it != m_mines.end()) {
                it->second.nowNum = gm.nowNum;
                it->second.timer = gm.timer;
            }
        }
    } else {
        size_t size;
        stream >> DataStream::cu(size);
        for (size_t i = 0; i < size; ++i) {
            GoldMine gm;
            stream >> gm.id >> gm.nowNum >> gm.timer;
            
            auto it = m_mines.find(gm.id);
            if (it != m_mines.end()) {
                it->second.nowNum = gm.nowNum;
                it->second.timer = gm.timer;
            }
        }
        
    }
}

void BSGoldMine::save(DataStream &stream) const
{
    stream << DataStream::ccu(m_mines.size());
    for (auto &i : m_mines) {
        auto &gm = i.second;
        stream << gm.id << gm.nowNum << gm.timer;
    }
}

void BSGoldMine::save(Variable_object &vobj) const {
    auto mines = vnnew Variable_object();
    vobj.setAttribute("mines", mines);
    for (auto &i : m_mines) {
        auto &gm = i.second;
        auto p = vnnew Variable_object();
        mines->add(p);
        p->setAttribute("id",  vnnew Variable_int32(gm.id));
        p->setAttribute("count",vnnew Variable_int32(gm.nowNum));
        p->setAttribute("timer", vnnew Variable_float(gm.timer));
    }
}


void BSGoldMine::AsyncSave(vn::RefCountedPtr<AsyncObject> asyncObjectPtr,std::function<void(bool ret)> doResult)
{
	std::string strUUId = m_user->GetUUId();
	int taskNum = m_mines.size();
	AsyncTaskPtr mineTask = vnnew AsyncTask();
	mineTask->Init(taskNum);
	vn::u64 nowTime = time(NULL);
	 for (auto &mine : m_mines)
	{
		 u32 mineId=mine.second.id;
		 u32 mineNum=mine.second.nowNum;
		 f32 timer = mine.second.timer;
		((DC_DataBase *)&DataCenter::instance())->AsyncUpdateGoldMine(strUUId,mineId, mineNum,timer,nowTime,asyncObjectPtr,[mineId,mineTask,doResult,strUUId](bool ret)
		{
			if(!ret)
			{
				VN_LOG_ERROR("UUID:"<<strUUId<<"mineid"<<mineId<<"update failed");
			}
			mineTask->FinishTask(ret);
			if(mineTask->IsFinished())
			{
				int size = mineTask->TaskSize();
				bool result = true;
				for(int i=0;i<size;++i)
				{
					if(mineTask->at(i) == false)
					{
						result = false;
						break;
					}
				}
				return  doResult(result);
			}
		});
	}
}






void BSGoldMine::AsyncSave(AsyncObjectPtr asyncObjectPtr,u32 id,std::function<void(bool ret)> doResult)
{
	std::string strUUId = m_user->GetUUId();
	auto iter = m_mines.find(id);
	if(iter == m_mines.end())
	{
		return doResult(false);
	}
	u32 mineId=iter->second.id;
	u32 mineNum=iter->second.nowNum;
	f32 timer = iter->second.timer;
	vn::u64 stamp = time(NULL);
	((DC_DataBase *)&DataCenter::instance())->AsyncUpdateGoldMine(strUUId,mineId, mineNum,timer,stamp,asyncObjectPtr,doResult);
}





void BSGoldMine::load(Variable_object &vobj) {
    VariableAccessor<Variable_object> mine(vobj.queryAttributeObject("mines"));
    while (mine.fetch()) {
        auto p = mine.at<0>();
        u32 mineId = p->queryAttributeInt32("id");
        auto it = m_mines.find(mineId);
        if (it != m_mines.end()) {
            it->second.nowNum = p->queryAttributeInt32("count");
            it->second.timer = p->queryAttributeFloat("timer");
        }
    }
}


void BSGoldMine::load(PreparedQueryResult result)
{
	do
	{
		  u32 mineId = (*result)[0].GetUInt32();
		  u32 count = (*result)[1].GetUInt32();
		  f32 timer = (*result)[2].GetFloat();
		  vn::u64 mineTimeStamp =(*result)[3].GetUInt64();
			auto it = m_mines.find(mineId);
			if (it != m_mines.end())
			{
				it->second.nowNum = count;
				it->second.timer = timer;
				it->second.lastTimestamp = mineTimeStamp;
			}
	}while(result->NextRow());
}



void BSGoldMine::update(f32 deltaTime)
{
    for (auto &i : m_mines)
    {
        i.second.update(deltaTime);
    }
}


void BSGoldMine::Asyncupdate(f32 deltaTime,std::function<void(bool ret)>doResult)
{
    for (auto &i : m_mines)
    {
        i.second.update(deltaTime);
    }
}



bool BSGoldMine::collect(u32 id, u32 num,s32 *addDiamond)
{
    auto it = m_mines.find(id);
    if (it == m_mines.end()) {
        return false;
    }
    
    if (num > it->second.nowNum) {
        return false;
    }
                
    switch (it->second.type) {
        case 1:
            m_user->bsPlayerBaseInfo().addMoney(num);
            break;
        case 2:
           // m_user->bsPlayerBaseInfo().addDiamond(num);
            if(addDiamond)
            {
            	 *addDiamond = num;
            }
            break;
    }
    it->second.nowNum -= num;
    return true;
}




void BSGoldMine::Asynccollect(u32 id, u32 num,std::function<void(int ret)> doResult)
{
    auto it = m_mines.find(id);
    if (it == m_mines.end()) {
        return doResult(-1);
    }

    if (num > it->second.nowNum) {
        return doResult(-1);
    }

    switch (it->second.type) {
        case 1:
            m_user->bsPlayerBaseInfo().addMoney(num);
            //m_user->bsPlayerBaseInfo().money() += num;
            break;
        case 2:
            m_user->bsPlayerBaseInfo().addDiamond(num);
            //m_user->bsPlayerBaseInfo().diamond() += num;
            break;
    }
    it->second.nowNum -= num;
    u32 goldenId =it->second.id;
    u32 goldenNum =it->second.nowNum;
    	f32 goldenTimer = it->second.timer;
    	vn::u64 curTime = time(NULL);
    //更新db
    std::string strUUId= m_user->GetUUId();
       s32 nowClip0 = m_user->bsPlayerBaseInfo().chip(0);
       s32 nowClip1 = m_user->bsPlayerBaseInfo().chip(1);
   	 s32 nowClip2 = m_user->bsPlayerBaseInfo().chip(2);
   	 s32 nowMoney = m_user->bsPlayerBaseInfo().money();
   	 s32 nowDiamond = m_user->bsPlayerBaseInfo().diamond();
   	 s32 nowPurchase =m_user->GetPurchaseDiamond();
	((DC_DataBase *)&DataCenter::instance())->AsyncUpdateGoldMine(strUUId,goldenId,goldenNum,goldenTimer,curTime,{m_user,true},[this,goldenId,strUUId,doResult,nowClip0,nowClip1,nowClip2,nowMoney,nowDiamond,nowPurchase](bool result)
	{
		if(!result)
		{
			VN_LOG_ERROR("UUid"<<": "<<strUUId<<"goledid"<<goldenId<<"collect failed");
			return doResult(result);
		}
		 std::string strUUId= m_user->GetUUId();
	 	((DC_DataBase *)&DataCenter::instance())->AsyncUpdateBaseInfo_Baseinfo({m_user,true},strUUId,nowClip0,nowClip1,nowClip2,nowMoney,nowDiamond,nowPurchase,[this,doResult,strUUId](bool ret)
		{
			if(!ret)
			{
				VN_LOG_ERROR("uuid"<<strUUId<<"update basinfo_money_failed");
			}
			return doResult(ret);
		});
	});
}




void BSGoldMine::notifyVictory(u32 mineId, u32 levelId, bool group) {
    auto i = m_mines.find(mineId);
    if (i == m_mines.end()) {
        return ;
    }
    auto j = i->second.levels.find({levelId, group});
    if (j == i->second.levels.end()) {
        return ;
    }
    if (j->second.status != 2) {
        j->second.status = 2;
        i->second.calcYield();
    }
}

void BSGoldMine::notifyIntrusion(u32 mineId, u32 levelId, bool group) {
    auto it = m_mines.find(mineId);
    if (it == m_mines.end()) {
        return ;
    }
    auto i = m_mines.find(mineId);
    if (i == m_mines.end()) {
        return ;
    }
    auto j = i->second.levels.find({levelId, group});
    if (j == i->second.levels.end()) {
        return ;
    }
    if (j->second.status != 1) {
        j->second.status = 1;
        i->second.calcYield();
    }
}

void BSGoldMine::_setStatus(u32 mineId, u32 levelId, bool group, u32 status) {
    auto it = m_mines.find(mineId);
    if (it == m_mines.end()) {
        return ;
    }
    auto i = m_mines.find(mineId);
    if (i == m_mines.end()) {
        return ;
    }
    auto j = i->second.levels.find({levelId, group});
    if (j == i->second.levels.end()) {
        return ;
    }
    j->second.status = status;
}



void BSGoldMine::_calcYield()
{
    for (auto &i : m_mines) {
        i.second.calcYield();
    }
}
