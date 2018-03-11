//
//  BSTechUpgrade.cpp
//  optd_srv
//
//  Created by Cloverz on 8/18/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "../worker/BSTechUpgrade.h"

#include "../manager/BSTechManager.h"
#include "../manager/BSCommonFunc.h"
#include "../worker/BSPlayerBaseInfo.h"
#include "../worker/LoggedUser.h"
#include "vnLog.h"
#include "vnVariableAccessor.h"
#include "../manager/DC_DataBase.h"
BSTechUpgrade::BSTechUpgrade(LoggedUser *user)
: m_user(user) {}

BSTechUpgrade::~BSTechUpgrade()
{
    m_user = 0;
}

void BSTechUpgrade::init()
{
    m_techList.clear();
    
    for (int i = 0; i < kTechCount; ++i)
    {
        m_techList.push_back(0);
    }
}

void BSTechUpgrade::load(u32 version, DataStream &stream)
{
    for (int i = 0; i < kTechCount; ++i)
    {
        u32 value = 0;
        stream >> value;
        m_techList.push_back(value);
    }
}

void BSTechUpgrade::save(DataStream &stream) const
{
    for (int i = 0; i < kTechCount; ++i)
    {
        stream << m_techList[i];
    }
}

void BSTechUpgrade::save(Variable_object &vobj) const {
    auto p = vnnew Variable_object();
    
    for (auto i : m_techList) {
        p->add(vnnew Variable_int32(i));
    }
    
    vobj.setAttribute("tech_upgrade", p);
}


void BSTechUpgrade::Asyncsave(u32 idx,std::function<void(bool ret)> doResult)
{
	if(idx >= kTechCount)
	{
		 return doResult(false);
	}
	vn::u32 techStatus =m_techList[idx];
  ((DC_DataBase *)&DataCenter::instance())->AsyncUpdateTech({m_user,true},idx,techStatus,doResult);
}
//todo
void BSTechUpgrade::load(Variable_object &vobj) {
    VariableAccessor<Variable_int32> va(vobj.queryAttributeObject("tech_upgrade"));
    for (int i = 0; i < kTechCount && va.fetch(); ++i) {
        m_techList.push_back(va.at<0>()->value());
    }
}


void BSTechUpgrade::load(PreparedQueryResult result)
{
		for(uint i=0;i<kTechCount;++i)
		{
			u32 techstatus= (*result)[0].GetUInt32();
			 m_techList.push_back(techstatus);
			if(!result->NextRow())
			{
				break;
			}
		}
}

bool BSTechUpgrade::upgrade_tech(u32 idx,vn::s32 *diamondPay)
{
    if (idx >= kTechCount)
        return false;
    
    const BSTechManager::TechItem * _item = BSTechManager::instance().getTechItem(idx, m_techList[idx]);
    
    if (!_item)
        return false;
    
    s32  _money = m_user->bsPlayerBaseInfo().money();
    s32  _diamond = m_user->bsPlayerBaseInfo().diamond();
    
    switch (_item->costType)
    {
        case BSTechManager::kCT_Money:
            
            if(_item->costValue <= _money)
            {
                m_user->bsPlayerBaseInfo().addMoney(-(s32)_item->costValue);
            }
            else
            {
                u32 _needDiamond = BSCommonFunc::instance().calc_diamondByRes(_item->costValue - _money);
                //todo
                if((s32)_needDiamond <= _diamond)
                {
                    m_user->bsPlayerBaseInfo().addMoney(-(s32)_money);
                    //m_user->bsPlayerBaseInfo().addDiamond(-(s32)_needDiamond);
                    if(diamondPay)
                    {
                    	*diamondPay =	-(s32)_needDiamond;
                    }
                }
                else
                {
                    return false;
                }
            }
            
            break;
            
        case BSTechManager::kCT_Gem:
            
            if(_item->costValue <= _diamond)
            {
                //_user->bsPlayerBaseInfo().addDiamond(-(s32)_item->costValue);
				if(diamondPay)
				{
					*diamondPay =	-(s32)_item->costValue;
				}
            }
            else
            {
                return false;
            }
            
            break;
            
        default:
            break;
    }
    
    ++m_techList[idx];


    return true;
}




void BSTechUpgrade::Asyncupgrade_tech(u32 idx,std::function<void(int ret)> doResult)
{
	if (idx >= kTechCount)
	{
		 return doResult(-1);
	}
	const BSTechManager::TechItem * _item = BSTechManager::instance().getTechItem(idx, m_techList[idx]);

	if (!_item)
	{
		 return doResult(-1);
	}
    s32  _money = m_user->bsPlayerBaseInfo().money();
    s32  _diamond = m_user->bsPlayerBaseInfo().diamond();

    switch (_item->costType)
    {
        case BSTechManager::kCT_Money:

            if(_item->costValue <= _money)
            {
                m_user->bsPlayerBaseInfo().addMoney(-(s32)_item->costValue);
            }
            else
            {
                u32 _needDiamond = BSCommonFunc::instance().calc_diamondByRes(_item->costValue - _money);
                //todo
                if((s32)_needDiamond <= _diamond)
                {
                    m_user->bsPlayerBaseInfo().addMoney(-(s32)_money);
                    m_user->bsPlayerBaseInfo().addDiamond(-(s32)_needDiamond);

                }
                else
                {
                      return doResult(-1);;
                }
            }

            break;

        case BSTechManager::kCT_Gem:

            if(_item->costValue <= _diamond)
            {
                m_user->bsPlayerBaseInfo().addDiamond(-(s32)_item->costValue);
                //_diamond -= _item->costValue;
            }
            else
            {
            	 return doResult(-1);
            }
            break;
        default:
            break;
    }
    ++m_techList[idx];
    m_user->_updateStamp();
    u32 techStatus = m_techList[idx];
    u32 techIndex = idx;
    std::string strUUId= m_user->GetUUId();
    //db更新
   /* s32 nowClip0 = m_user->bsPlayerBaseInfo().chip(0);
    s32 nowClip1 = m_user->bsPlayerBaseInfo().chip(1);
    s32 nowClip2 = m_user->bsPlayerBaseInfo().chip(2);
    s32 nowMoney = m_user->bsPlayerBaseInfo().money();
    s32 nowDiamond = m_user->bsPlayerBaseInfo().diamond();
    s32 nowPurchase =m_user->GetPurchaseDiamond();*/
    AsyncTaskPtr task = vnnew AsyncTask();
    task->Init(2);
    ((DC_DataBase *)&DataCenter::instance())->AsyncUpdateTech({m_user,true},techIndex,techStatus,[this,doResult,techStatus,techIndex,strUUId,task](bool ret)
	{
    	if(!ret)
		{
    		VN_LOG_ERROR("uuid"<<m_user->GetUUId()<<"techIndex:"<<techIndex<<"techIndex"<<techIndex<<"AsyncUpdateTech failed");
		}
    	task->FinishTask(ret);
	});
    ((DC_DataBase *)&DataCenter::instance())->AsyncUpdateBaseInfoAll({m_user,true},{m_user,true},[this,doResult,task](bool ret)
	{
    	if(!ret)
		{
    		VN_LOG_ERROR("uuid"<<m_user->GetUUId()<<"AsyncUpdateTech failed");

		}

	});
}



 std::vector<u32> & BSTechUpgrade::techList()
{
    return m_techList;
}
