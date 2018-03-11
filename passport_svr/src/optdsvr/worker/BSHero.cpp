//
//  BSHero.cpp
//  optd_srv
//
//  Created by Cloverz on 9/2/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "vnVariableScript.h"
#include "vnVariableAccessor.h"
#include "../manager/BSFactory.h"
#include "LoggedUser.h"
#include "BSLevel.h"
#include "../manager/BSCommonFunc.h"
#include "../manager/DC_DataBase.h"
decltype(BSHero::m_heroList) BSHero::s_initialHeroList;

bool BSHero::loadInitalHeroList(u32 fsId, const str8 &fileName) {
    VariableScript vs;
    auto root = vs.parse({fsId, fileName});
    if (!root) {
        return false;
    }
        VariableAccessor<Variable_int32, Variable_int32> list(root->queryAttributeObject("list"));
        while (list.fetch())
        {
            HeroInfo hi;
            hi.id = (u32)list.at<0>()->getInt32();
            hi.towerClass = hi.id - 1;
            hi.unlockRes = (u32)list.at<1>()->getInt32();
            hi.status = kLock;
            
            tbl::Table_herorune &hr = BSFactory::instance().herorune();
            for (uint i = 0; i < hr.size(); ++i)
            {
                const tbl::Record_herorune *data = hr.at(i);
                if (data->f_RuneClass.value == hi.towerClass)
                {
                    if (data->f_RuneRank.value != 0) {
                        Rune rune;
                        rune.id = data->f_RuneRank.value;
                        rune.status = kNotOpen;
                        hi.runeList.push_back(rune);
                    }
                }
            }
            s_initialHeroList.push_back(hi);
        }
    
    return true;
}

BSHero::BSHero(LoggedUser *user)
{
    m_skull[0] = 0;
    m_skull[1] = 0;
    m_shovel[0] = 0;
    m_shovel[1] = 0;
    m_watch[0] = 0;
    m_watch[1] = 0;
    m_user = user;
}

BSHero::~BSHero()
{
    m_user = 0;
    m_heroList.clear();
}

void BSHero::init()
{
    m_skull[0] = 0;
    m_skull[1] = 0;
    m_shovel[0] = 0;
    m_shovel[1] = 0;
    m_watch[0] = 0;
    m_watch[1] = 0;
    m_heroList.clear();
    m_heroList = s_initialHeroList;
}

void BSHero::load(u32 version, DataStream &stream)
{
    stream >> m_skull[0] >> m_skull[1] >> m_shovel[0] >> m_shovel[1] >> m_watch[0] >> m_watch[1];
    
    u32 size = 0;
    stream >> size;
    for (uint i = 0; i < size; ++i)
    {
        HeroInfo hi;
        stream >> hi.id >> hi.towerClass >> hi.unlockRes;
        
        u32 status = 0;
        stream >> status;
        switch (status)
        {
            case 0:
                //hi.status = kNotOpen;
                hi.status = kLock;
                break;
            case 1:
                hi.status = kLock;
                break;
            case 2:
                hi.status = kUnLock;
                break;
            case 3:
                hi.status = kMaxLv;
                break;
        }
        
        u32 c = 0;
        stream >> c;
        for (uint j = 0; j < c; ++j)
        {
            Rune rune;
            stream >> rune.id;
            
            stream >> status;
            switch(status)
            {
                case 0:
                    rune.status = kNotOpen;
                    break;
                case 1:
                    rune.status = kLock;
                    break;
                case 2:
                    rune.status = kUnLock;
                    break;
            }
            hi.runeList.push_back(rune);
        }
        m_heroList.push_back(hi);
    }
}

void BSHero::save(DataStream &stream) const
{
    stream << m_skull[0] << m_skull[1] << m_shovel[0] << m_shovel[1] << m_watch[0] << m_watch[1];
    stream << (u32)m_heroList.size();
    for (auto &i : m_heroList)
    {
        stream << i.id << i.towerClass << i.unlockRes << (u32)i.status;
        
        stream << (u32)i.runeList.size();
        for (auto &j : i.runeList)
        {
            stream << j.id << (u32)j.status;
        }
    }
}

void BSHero::save(Variable_object &vobj) const {
    auto hero = vnnew Variable_object();
    vobj.setAttribute("hero", hero);
    hero->setAttribute("skull0", vnnew Variable_int32(m_skull[0]));
    hero->setAttribute("skull1", vnnew Variable_int32(m_skull[1]));
    hero->setAttribute("shovel0", vnnew Variable_int32(m_shovel[0]));
    hero->setAttribute("shovel1", vnnew Variable_int32(m_shovel[1]));
    hero->setAttribute("watch0",vnnew Variable_int32(m_watch[0]) );
    hero->setAttribute("watch1", vnnew Variable_int32(m_watch[1]));
    for (auto &i : m_heroList) {
        auto p = vnnew Variable_object();
        hero->add(p);
        p->setAttribute("id", vnnew Variable_int32(i.id));
        p->setAttribute("class", vnnew Variable_int32(i.towerClass));
        p->setAttribute("unlock",vnnew Variable_int32( i.unlockRes));
        p->setAttribute("status", vnnew Variable_int32((u32)i.status));
        auto runes = vnnew Variable_object();
        p->setAttribute("runes", runes);
        for (auto &j : i.runeList) {
            auto q = vnnew Variable_object();
            runes->add(q);
            q->setAttribute("id", vnnew Variable_int32(j.id));
            q->setAttribute("status", vnnew Variable_int32((u32)j.status));
        }
    }
}








void BSHero::load(HeroInfo &heroinfo)
{
	 m_heroList.push_back(heroinfo);
}

void BSHero::load(PreparedQueryResult baseinfo)
{
	m_shovel[0] = (*baseinfo)[10].GetUInt32();
	m_shovel[1] =(*baseinfo)[11].GetUInt32();
	m_skull[0] = (*baseinfo)[12].GetUInt32();
	m_skull[1] =(*baseinfo)[13].GetUInt32();
	m_watch[0] = (*baseinfo)[14].GetUInt32();
	m_watch[1] = (*baseinfo)[15].GetUInt32();
}


void BSHero::load(Variable_object &vobj) {
    auto hero = vobj.queryAttributeObject("hero");
    if (!hero) {
        return ;
    }
    m_skull[0] = hero->queryAttributeInt32("skull0");
    m_skull[1] = hero->queryAttributeInt32("skull1");
    m_shovel[0] = hero->queryAttributeInt32("shovel0");
    m_shovel[1] = hero->queryAttributeInt32("shovel1");
    m_watch[0] = hero->queryAttributeInt32("watch0");
    m_watch[1] = hero->queryAttributeInt32("watch1");
    VariableAccessor<Variable_object> hi(hero);
    while (hi.fetch()) {
        auto p = hi.at<0>();
        HeroInfo info;
        info.id = p->queryAttributeInt32("id");
        info.towerClass = p->queryAttributeInt32("class");
        info.unlockRes = p->queryAttributeInt32("unlock");
        info.status = (Status)p->queryAttributeInt32("status");
        VariableAccessor<Variable_object> ri(p->queryAttributeObject("runes"));
        while (ri.fetch()) {
            auto q = ri.at<0>();
            Rune rune;
            rune.id = q->queryAttributeInt32("id");
            rune.status = (Status)q->queryAttributeInt32("status");
            info.runeList.push_back(rune);
        }
        m_heroList.push_back(std::move(info));
    }
}


void BSHero::open_hero()
{
    tbl::Table_progress & progress = BSFactory::instance().progress();
    for (uint i = 0; i < progress.size(); ++i)
    {
        const tbl::Record_progress *data = progress.at(i);
        if (data->f_UnlockType.value == 5)			//解锁英雄
        {
        	//关卡满足条件就解锁英雄
            const BSLevel::LevelInfo * li = m_user->bsLevel().find(data->f_LevelId.value);
            if (li && li->starNum >= 1 && data->f_SubType.value - 1 < m_heroList.size())
            {
                HeroInfo &hi = m_heroList[data->f_SubType.value - 1];
                if (hi.status == kLock)
                	{
                    hi.status = kUnLock;
                    auto desc = BSFactory::instance().find_tower(hi.id);
                    if (desc)
                    {
                        _open_rune(hi.runeList, hi.towerClass, desc->f_Level.value);
                    }
                m_user->_notify_heroUnlocked(hi.towerClass);
                }
            }
        }
    }
    _update_rune_res();
}



bool  BSHero::SyncOpenHero()
{
	tbl::Table_progress & progress = BSFactory::instance().progress();
	    for (uint i = 0; i < progress.size(); ++i)
	    {
	        const tbl::Record_progress *data = progress.at(i);
	        if (data->f_UnlockType.value == 5)			//解锁英雄
	        {
	        	//关卡满足条件就解锁英雄
	            const BSLevel::LevelInfo * li = m_user->bsLevel().find(data->f_LevelId.value);
	            if (li && li->starNum >= 1 && data->f_SubType.value - 1 < m_heroList.size())
	            {
	                HeroInfo &hi = m_heroList[data->f_SubType.value - 1];
	                if (hi.status == kLock)
	                	{
	                    hi.status	 = kUnLock;
	                    	//同步更新Hero
	                    if(!((DC_DataBase *)&DataCenter::instance())->UpdateHeroStatus((s32)hi.status,{m_user,true},hi.id))
	                    	{
	                    		VN_LOG_ERROR("UpdateHero failed");
	                    		return false;
	                    	}
	                    auto desc = BSFactory::instance().find_tower(hi.id);
	                    if (desc)
							{
	                    		//同步更新技能
								if(!SyncOpenRune(hi.runeList, hi.towerClass, desc->f_Level.value))
								{
									return false;
								}
							}
	                m_user->_notify_heroUnlocked(hi.towerClass);
	                }
	            }
	        }
	    }
	    _update_rune_res();
	    return true;
}




void BSHero::Asyncsave(u32 idx,std::function<void(bool ret)>doResult)
{
	 if (idx >= m_heroList.size())
	 {
		 return	doResult(false);
	 }
	 HeroInfo &hi = m_heroList[idx];
	 std::string strUUId = m_user->GetUUId();
	u32 id = hi.id;
	s32 status = hi.status;
	u32 towerclass = hi.towerClass;
	((DC_DataBase *)&DataCenter::instance())->AsyncupgradeHero({m_user,true},strUUId,id,status,towerclass,doResult);
}





bool BSHero::upgrade_hero(u32 idx, bool useCoupon,s32 *payDiamond)
{
    if (idx >= m_heroList.size())
    return false;
    HeroInfo &hi = m_heroList[idx];
    //tower表
    const tbl::Record_tower *data = BSFactory::instance().find_tower(hi.id);
    if (!data)
	{
    	return false;
	}
	if (hi.status == kNotOpen || hi.status == kMaxLv)
	{
		return false;
	}
	//英雄锁住了
	if (hi.status == kLock)
	{
		u32 cost = hi.unlockRes;
		if (useCoupon)
		{
			auto coupon = m_user->bsCoupon();
			if (coupon && coupon->type() == BSCoupon::kCT_Hero)
			{
				auto pc = static_cast<const BSCoupon_Hero *>(coupon);
				if (pc->heroClassId == hi.towerClass)
				{
					cost = pc->cost;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
	  else
		{
			auto coupon = m_user->bsCoupon();
				if (coupon && coupon->type() == BSCoupon::kCT_Hero)
				{
					auto pc = static_cast<const BSCoupon_Hero *>(coupon);
					if (pc->heroClassId == hi.towerClass)
					{
						m_user->_notify_couponUsed();
					}
				}
	  }//fix
	  if (m_user->bsPlayerBaseInfo().diamond() >= (s32)cost)
		{
		 // m_user->bsPlayerBaseInfo().addDiamond(-(s32)cost);
		  if(payDiamond)
		  {
			  *payDiamond  =-(s32)cost;
		  }
		  hi.status = kUnLock;
		  m_user->_notify_couponUsed();
		}
	  else
		{
			return false;
		}
	}
	//英雄没有锁住
   else if (hi.status == kUnLock)
	{
		if (m_user->bsPlayerBaseInfo().money() >= data->f_Cost.value)
		{
			m_user->bsPlayerBaseInfo().addMoney(-data->f_Cost.value);
			++hi.id;
			if (!BSFactory::instance().find_tower(hi.id + 1))
				hi.status = kMaxLv;
		}
		else
		{
			u32 needGold = data->f_Cost.value - m_user->bsPlayerBaseInfo().money();
			u32 needDiamond = BSCommonFunc::instance().calc_diamondByRes(needGold);
			//fix
			if (m_user->bsPlayerBaseInfo().diamond() >= (s32)needDiamond)
			{
				m_user->bsPlayerBaseInfo().addMoney(-m_user->bsPlayerBaseInfo().money());
				//m_user->bsPlayerBaseInfo().addDiamond(-(s32)needDiamond);
				  if(payDiamond)
				  {
					  *payDiamond  =-(s32)needDiamond;
				  }
				//升级id
				++hi.id;
				if (!BSFactory::instance().find_tower(hi.id + 1))
				{
					hi.status = kMaxLv;
				}
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}
	//找到英雄的当前等级
	data = BSFactory::instance().find_tower(hi.id);
	if (data)
	{
		_open_rune(hi.runeList, hi.towerClass, data->f_Level.value);
	}
	return true;
}

//参数错误-1，数据库插入失败0  成功1
void BSHero::Asyncupgrade_hero(u32 idx, bool useCoupon,std::function<void(int ret)>doResult)
{
    if (idx >= m_heroList.size())
    {
		VN_LOG_ERROR("heroindex invalid");
		return doResult(-1);
    }
    HeroInfo &hi = m_heroList[idx];
    //tower表
    const tbl::Record_tower *data = BSFactory::instance().find_tower(hi.id);
    if (!data)
	{
    	VN_LOG_ERROR("can find hero id:"<<hi.id);
    	return doResult(-1);
	}
	if (hi.status == kNotOpen || hi.status == kMaxLv)
	{
		VN_LOG_DEBUG("hero:"<<hi.id<<"status:"<<(s32)hi.status);
		return doResult(-1);
	}
	//英雄锁住了
	if (hi.status == kLock)
	{
		u32 cost = hi.unlockRes;
		if (useCoupon)
		{
			auto coupon = m_user->bsCoupon();
			if (coupon && coupon->type() == BSCoupon::kCT_Hero)
			{
				auto pc = static_cast<const BSCoupon_Hero *>(coupon);
				if (pc->heroClassId == hi.towerClass)
				{
					cost = pc->cost;
				}
				else
				{
					return doResult(-1);
				}
			}
			else
			{
				return doResult(-1);
			}
		}
	  else
		{
			auto coupon = m_user->bsCoupon();
				if (coupon && coupon->type() == BSCoupon::kCT_Hero)
				{
					auto pc = static_cast<const BSCoupon_Hero *>(coupon);
					if (pc->heroClassId == hi.towerClass)
					{
						m_user->_notify_couponUsed();
					}
				}
	  }//fix
	  if (m_user->bsPlayerBaseInfo().diamond() >= (s32)cost)
		{
		  m_user->bsPlayerBaseInfo().addDiamond(-(s32)cost);
		  hi.status = kUnLock;
		  m_user->_notify_couponUsed();
		}
	  else
		{
			return doResult(-1);;
		}



	}
	//英雄没有锁住
   else if (hi.status == kUnLock)
	{
		if (m_user->bsPlayerBaseInfo().money() >= data->f_Cost.value)
		{
			m_user->bsPlayerBaseInfo().addMoney(-data->f_Cost.value);
			++hi.id;
			if (!BSFactory::instance().find_tower(hi.id + 1))
				hi.status = kMaxLv;
		}
		else
		{
			u32 needGold = data->f_Cost.value - m_user->bsPlayerBaseInfo().money();
			u32 needDiamond = BSCommonFunc::instance().calc_diamondByRes(needGold);
			//fix
			if (m_user->bsPlayerBaseInfo().diamond() >= (s32)needDiamond)
			{
				m_user->bsPlayerBaseInfo().addMoney(-m_user->bsPlayerBaseInfo().money());
				m_user->bsPlayerBaseInfo().addDiamond(-(s32)needDiamond);
				//升级id
				++hi.id;
				if (!BSFactory::instance().find_tower(hi.id + 1))
				{
					hi.status = kMaxLv;
				}
			}
			else
			{
				 return doResult(-1);;
			}
		}
	}
	else
	{
		 return doResult(-1);;
	}
	u32 id = hi.id;
	s32 status = hi.status;
	u32 towerclass = hi.towerClass;
	//异步更新hero到数据库hero表
	std::string strUUid= m_user->GetUUId();
	((DC_DataBase *)&DataCenter::instance())->AsyncupgradeHero({m_user,true},strUUid,id,status,towerclass,[this,id,doResult,idx](bool ret)
	{
		HeroInfo &hi = m_heroList[idx];
		std::string strUUid = m_user->GetUUId();
		if(!ret)
		{
			VN_LOG_ERROR("user"<<strUUid<<"update hero:"<<id<<"failed");
			return doResult(false);
		}
		//更新hero的技能
		const tbl::Record_tower *data = BSFactory::instance().find_tower(id);
		if (data)
		{
			if(!SyncOpenRune(hi.runeList,hi.towerClass,data->f_Level.value))
			{
				return doResult(false);
			}
			return doResult(true);
		}
		return doResult(true);
	});
}


bool BSHero::unlock_rune(u32 idx, u32 runeIdx)
{
    if (idx >= m_heroList.size())
        return false;
    
    HeroInfo &hi = m_heroList[idx];
    if (runeIdx >= hi.runeList.size())
        return false;
    
    if (hi.runeList[runeIdx].status != kLock)
        return false;
    
    const tbl::Record_herorune *data = BSFactory::instance().find_herorune(hi.towerClass, hi.runeList[runeIdx].id);
    if (data)
    {
        bool flag = false;
        switch (data->f_CostType.value)
        {
            case 1:
                if (m_skull[0] + data->f_CostValue.value <= m_skull[1])
                {
                    m_skull[0] += data->f_CostValue.value;
                    flag = true;
                }
                break;
            case 2:
                if (m_shovel[0] + data->f_CostValue.value <= m_shovel[1])
                {
                    m_shovel[0] += data->f_CostValue.value;
                    flag = true;
                }
                break;
            case 3:
                if (m_watch[0] + data->f_CostValue.value <= m_watch[1])
                {
                    m_watch[0] += data->f_CostValue.value;
                    flag = true;
                }
                break;
        }
        
        if (flag)
        {
            hi.runeList[runeIdx].status = kUnLock;
            return true;
        }
    }
    return false;
}



void BSHero::AsyncsaveRune(u32 idx, u32 runeIdx,std::function<void(bool ret)>doResult)
{
	 if (idx >= m_heroList.size())
	 {
		 return doResult(false);
	 }
	HeroInfo &hi = m_heroList[idx];
	if (runeIdx >= hi.runeList.size())
	{
		 return doResult(false);
	}
	u32 runid =  hi.runeList[runeIdx].id;
	s32 runstatus = hi.runeList[runeIdx].status;
	//根据uuid towerclass 找到heroid
	std::string strUUId = m_user->GetUUId();
	((DC_DataBase *)&DataCenter::instance())->AsyncLoadHeroIdbyUUIdTowerClass({m_user,true},hi.towerClass,[this,doResult,strUUId,runid,runstatus](PreparedQueryResult ret)
	{
		if (!(ret && ret->GetRowCount()))
		{
			VN_LOG_DEBUG("LOAD "<<strUUId<<"Heroid failed");
			return doResult(false);
		}
		if(ret->GetRowCount()>1)
		{
			VN_LOG_DEBUG("LOAD "<<strUUId<<"Heroid Duplicat");
			return doResult(false);
		}
		std::string heroid = (*ret)[0].GetString();;
		//更具heroid runid 跟新runstatus
		((DC_DataBase *)&DataCenter::instance())->AsyncUpdateRunesStatus(runid,runstatus,{m_user,true},heroid,[strUUId,heroid,runid,doResult](bool ret)
		{
			if(!ret)
			{
				VN_LOG_ERROR("heroid:"<<heroid<<"runid:"<<runid<<"UpdateRunesStatus Failed");
				return doResult(false);
			}
			return doResult(true);
		});
	});
}



//-1 参数错误，0 数据库插入错误
void  BSHero::Asyncunlock_rune(u32 idx, u32 runeIdx,std::function<void(int ret)>doResult)
{
    if (idx >= m_heroList.size())
        return doResult(-1);

    HeroInfo &hi = m_heroList[idx];
    if (runeIdx >= hi.runeList.size())
        return doResult(-1);
    if (hi.runeList[runeIdx].status != kLock)
    	return doResult(-1);

    const tbl::Record_herorune *data = BSFactory::instance().find_herorune(hi.towerClass, hi.runeList[runeIdx].id);
    if (!data)
    {
    	return doResult(-1);
    }
	bool flag = false;
	switch (data->f_CostType.value)
	{
		case 1:
			if (m_skull[0] + data->f_CostValue.value <= m_skull[1])
			{
				m_skull[0] += data->f_CostValue.value;
				flag = true;
			}
			break;
		case 2:
			if (m_shovel[0] + data->f_CostValue.value <= m_shovel[1])
			{
				m_shovel[0] += data->f_CostValue.value;
				flag = true;
			}
			break;
		case 3:
			if (m_watch[0] + data->f_CostValue.value <= m_watch[1])
			{
				m_watch[0] += data->f_CostValue.value;
				flag = true;
			}
			break;
	}

	if (!flag)
	{
		return doResult(-1);
	}

	hi.runeList[runeIdx].status = kUnLock;
	u32 runid =  hi.runeList[runeIdx].id;
	s32 runstatus = hi.runeList[runeIdx].status;
	//根据uuid towerclass 找到heroid
	std::string strUUId = m_user->GetUUId();
	((DC_DataBase *)&DataCenter::instance())->AsyncLoadHeroIdbyUUIdTowerClass({m_user,true},hi.towerClass,[this,doResult,strUUId,runid,runstatus](PreparedQueryResult ret)
	{
		if (!(ret && ret->GetRowCount()))
		{
			VN_LOG_DEBUG("LOAD "<<strUUId<<"Heroid failed");
			return doResult(false);
		}
		if(ret->GetRowCount()>1)
		{
			VN_LOG_DEBUG("LOAD "<<strUUId<<"Heroid Duplicat");
			return doResult(false);
		}
		std::string heroid = (*ret)[0].GetString();;
		//更具heroid runid 跟新runstatus
		((DC_DataBase *)&DataCenter::instance())->AsyncUpdateRunesStatus(runid,runstatus,{m_user,true},heroid,[strUUId,heroid,runid,doResult](bool ret)
		{
			if(!ret)
			{
				VN_LOG_ERROR("heroid:"<<heroid<<"runid:"<<runid<<"UpdateRunesStatus Failed");
				return doResult(false);
			}
			return doResult(true);
		});
	});
}

















const std::vector<BSHero::HeroInfo> & BSHero::heroList()
{
    return m_heroList;
}

const u32 * BSHero::skull()
{
    return m_skull;
}

const u32 * BSHero::shovel()
{
    return m_shovel;
}

const u32 * BSHero::watch()
{
    return m_watch;
}

bool BSHero::unlocked(u32 heroClassId) {
    for (auto &i : m_heroList) {
        if (i.towerClass == heroClassId) {
            return i.status > kLock;
        }
    }
    return false;
}



//同步打开英雄技能
bool BSHero::SyncOpenRune(std::vector<Rune> &runeList, u32 towerClass, u32 lv)
{
	DC_DataBase *pDB=((DC_DataBase *)&DataCenter::instance());
	if(!m_user)
	{
		return false;
	}
	//根据uuid towerclass 找到heroid
	std::string strUUId = m_user->GetUUId();
	PreparedQueryResult dbResult=pDB->LoadHeroIdbyUUIdTowerClass(strUUId,towerClass);
	if (!(dbResult && dbResult->GetRowCount()))
	{
		VN_LOG_DEBUG("LOAD "<<strUUId<<"Heroid failed");
		return false;
	}
	if(dbResult->GetRowCount()>1)
	{
		VN_LOG_DEBUG("LOAD "<<strUUId<<"Heroid Duplicat");
		return false;
	}
	std::string heroid = (*dbResult)[0].GetString();;
    for (auto &i : runeList)
    {
        const tbl::Record_herorune *data = BSFactory::instance().find_herorune(towerClass, i.id);
        if (data && i.status == kNotOpen && lv >= data->f_Level.value)
        {
			i.status = kLock;
			//根据heroid,runid 更新runestatus
			if( !pDB->UpdateRunesStatus(i.id,(s32)i.status,heroid))
			{
				VN_LOG_ERROR("userid:"<<strUUId<<"heroid:"<<heroid<<"runid:"<<i.id<<"UpdateRunesStatus Failed");
			   return false;
			}
        }
    }
    return true;
}





void BSHero::_open_rune(std::vector<Rune> &runeList, u32 towerClass, u32 lv)
{
    for (auto &i : runeList)
    {
        const tbl::Record_herorune *data = BSFactory::instance().find_herorune(towerClass, i.id);
        if (data && i.status == kNotOpen && lv >= data->f_Level.value)
        {

            i.status = kLock;
        }
    }
}

void BSHero::_update_rune_res()
{
    m_skull[1] = 0;
    m_shovel[1] = 0;
    m_watch[1] = 0;
    for (auto &i : m_user->bsLevel().levelList())
    {
        if (i.second->starNum == 3)
            ++m_skull[1];
        
        if (i.second->shovel)
            ++m_shovel[1];
        
        if (i.second->watch)
            ++m_watch[1];
    }
    
    for (auto &i : m_user->bsLevelGroupMgr().groups()) {
        u32 cnt = (u32)i.second.levels.size();
        if (i.second.starsCompleted) {
            m_skull[1] += cnt;
        }
        if (i.second.watchCompleted) {
            m_watch[1] += cnt;
        }
        if (i.second.shovelCompleted) {
            m_shovel[1] += cnt;
        }
    }
}
