//
//  BSTowerUpgrade.cpp
//  optd_srv
//
//  Created by Cloverz on 8/19/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "LoggedUser.h"
#include "BSTowerUpgrade.h"
#include "../manager/BSTowerManager.h"
#include "../manager/BSCommonFunc.h"
#include "vnLog.h"
#include "vnVariableAccessor.h"
#include "../manager/DC_DataBase.h"

BSTowerUpgrade::BSTowerUpgrade(LoggedUser *user)
: m_user(user) {}

BSTowerUpgrade::~BSTowerUpgrade()
{
    m_user = 0;
}

void BSTowerUpgrade::init()
{
    m_towerList.clear();
    for (int i = 0; i < kNumClass; ++i)
    {
        std::vector<u32> list;
        for (int j = 0; j < kNumTowerInClass; ++j)
        {
            list.push_back(0);
        }
        m_towerList.push_back(list);
    }
}

void BSTowerUpgrade::load(u32 version, DataStream &stream)
{
    m_towerList.clear();
    for (int i = 0; i < kNumClass; ++i)
    {
        std::vector<u32> list;
        for (int j = 0; j < kNumTowerInClass; ++j)
        {
            u32 value = 0;
            stream >> value;
            list.push_back(value);
        }
        m_towerList.push_back(list);
    }
}

void BSTowerUpgrade::save(DataStream &stream) const
{
    for (int i = 0; i < kNumClass; ++i)
    {
        for (int j = 0; j < kNumTowerInClass; ++j)
        {
            stream << m_towerList[i][j];
        }
    }
}




void BSTowerUpgrade::save(Variable_object &vobj) const {
    auto p = vnnew Variable_object();
    
    for (auto &i : m_towerList) {
        for (auto j : i) {
            p->add(vnnew Variable_int32(j));
        }
    }
    
    vobj.setAttribute("tower_upgrade", p);
}




void BSTowerUpgrade::load(Variable_object &vobj) {
    VariableAccessor<Variable_int32> va(vobj.queryAttributeObject("tower_upgrade"));
    
    m_towerList.assign(kNumClass, {});
    for (int i = 0; i < kNumClass; ++i) {
        m_towerList[i].assign(kNumTowerInClass, 0);
        for (int j = 0; j < kNumTowerInClass; ++j) {
            if (va.fetch()) {
                m_towerList[i][j] = va.at<0>()->value();
            }
        }
    }
}

void BSTowerUpgrade::load(PreparedQueryResult result)
{
	  m_towerList.assign(kNumClass, {});
		for (int i = 0; i < kNumClass; ++i)
		{
			 m_towerList[i].assign(kNumTowerInClass, 0);
			for (int j = 0; j < kNumTowerInClass; ++j)
			{
				u32 value = (*result)[0].GetUInt32();
				m_towerList[i][j] = value;
				if(!result->NextRow())
				{
					break;
				}
			}
		}
}




void	BSTowerUpgrade::Asyncsave(u32 idx, u32 num,std::function<void(bool ret)> doResult)
{
	if (idx >= kNumClass || num >= kNumTowerInClass)
	{
		return doResult(false);
	}
	 u32 towerclassIndex = idx;
	u32 towerinClassIndex=num;
	u32 upgradeValue= m_towerList[idx][num];
   ((DC_DataBase *)&DataCenter::instance())->AsyncUpdateTower({m_user,true},towerclassIndex,towerinClassIndex,upgradeValue,doResult);
}


bool BSTowerUpgrade::upgrade_tower(u32 idx, u32 num,vn::s32 *diamondPay)
{
    if (idx >= kNumClass || num >= kNumTowerInClass)
    {
        return false;
    }
    
    const BSTowerManager::TrainingInfo * _info = BSTowerManager::instance().getTrainingInfo(idx, num, m_towerList[idx][num]);
    
    if (!_info)
        return false;
    
    s32 _money = m_user->bsPlayerBaseInfo().money();
    s32 _diamond = m_user->bsPlayerBaseInfo().diamond();
    s32 _chip = m_user->bsPlayerBaseInfo().chip(idx);
    
    switch (_info->costType)
    {
        case BSTowerManager::kCT_Money:
            //fix
            if((signed int)_info->costValue <= _money)
            {
            	//fix
                if ((s32)_info->chipValue <= _chip) {
                    m_user->bsPlayerBaseInfo().addMoney(-(s32)_info->costValue);
                    m_user->bsPlayerBaseInfo().addChip(idx, -(s32)_info->chipValue);
                } else {
                    u32 _needDiamond = BSCommonFunc::instance().calc_diamondByChip(_info->chipValue - _chip);
                    	//fix
                    if ((s32)_needDiamond <= _diamond) {
                        m_user->bsPlayerBaseInfo().addMoney(-(s32)_info->costValue);
                        m_user->bsPlayerBaseInfo().addChip(idx, -(s32)_chip);
                       // m_user->bsPlayerBaseInfo().addDiamond(-(s32)_needDiamond);
                        if(diamondPay)
                        {
                        	*diamondPay =-(s32)_needDiamond;
                        }
                    } else {
                        return false;
                    }
                }
                //_money -= _info->costValue;
            } else {
                u32 _needDiamond = BSCommonFunc::instance().calc_diamondByRes(_info->costValue - _money);
                //fix
                if((s32)_needDiamond <= _diamond)
                {
                	//fix
                    if ((s32)_info->chipValue <= _chip) {
                        m_user->bsPlayerBaseInfo().addChip(idx, -(s32)_info->chipValue);
                        m_user->bsPlayerBaseInfo().addMoney(-(s32)_money);
                        //m_user->bsPlayerBaseInfo().addDiamond(-(s32)_needDiamond);
                        if(diamondPay)
					  {
						*diamondPay =-(s32)_needDiamond;
					  }
                        
                    } else {
                        _needDiamond += BSCommonFunc::instance().calc_diamondByChip(_info->chipValue - _chip);
                        //fix
                        if ((s32)_needDiamond <= _diamond) {
                            m_user->bsPlayerBaseInfo().addMoney(-(s32)_money);
                            m_user->bsPlayerBaseInfo().addChip(idx, -(s32)_chip);
                           // m_user->bsPlayerBaseInfo().addDiamond(-(s32)_needDiamond);
                            if(diamondPay)
						  {
							*diamondPay =-(s32)_needDiamond;
						  }
                        } else {
                            return false;
                        }
                    }
                }
                else
                {
                    return false;
                }
            }
            
            break;
            
        case BSTowerManager::kCT_Gem:
            //fix
            if((s32)_info->costValue <= _diamond)
            {
            	//fix
                if ((s32)_info->chipValue <= _chip) {
                    //m_user->bsPlayerBaseInfo().addDiamond(-(s32)_info->costValue);
                    if(diamondPay)
				  {
					*diamondPay =-(s32)_info->costValue;
				  }
                    m_user->bsPlayerBaseInfo().addChip(idx, -(s32)_info->chipValue);
                } else {
                    //m_user->bsPlayerBaseInfo().addDiamond(-(s32)_info->costValue);
                    if(diamondPay)
					  {
						*diamondPay =-(s32)_info->costValue;
					  }
                    u32 _needDiamond = BSCommonFunc::instance().calc_diamondByChip(_info->chipValue - _chip);
                    //fix
                    if ((s32)_needDiamond <= _diamond) {
                        m_user->bsPlayerBaseInfo().addChip(idx, -(s32)_chip);
                        //m_user->bsPlayerBaseInfo().addDiamond(_needDiamond);
                        if(diamondPay)
						  {
							*diamondPay =-(s32)_needDiamond;
						  }
                    } else {
                        return false;
                    }
                    
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
    ++m_towerList[idx][num];
    return true;
}


void  BSTowerUpgrade::Asyncupgrade_tower(u32 idx, u32 num,std::function<void(int ret)> doResult)
{
    if (idx >= kNumClass || num >= kNumTowerInClass)
    {
        return doResult(-1);
    }
    
    const BSTowerManager::TrainingInfo * _info = BSTowerManager::instance().getTrainingInfo(idx, num, m_towerList[idx][num]);
    
    if (!_info)
    	 return doResult(-1);
    
    s32 _money = m_user->bsPlayerBaseInfo().money();
    s32 _diamond = m_user->bsPlayerBaseInfo().diamond();
    s32 _chip = m_user->bsPlayerBaseInfo().chip(idx);

    switch (_info->costType)
    {
        case BSTowerManager::kCT_Money:
            //fix
            if((signed int)_info->costValue <= _money)
            {
            	//fix
                if ((s32)_info->chipValue <= _chip) {
                    m_user->bsPlayerBaseInfo().addMoney(-(s32)_info->costValue);
                    m_user->bsPlayerBaseInfo().addChip(idx, -(s32)_info->chipValue);
                } else {
                    u32 _needDiamond = BSCommonFunc::instance().calc_diamondByChip(_info->chipValue - _chip);
                    	//fix
                    if ((s32)_needDiamond <= _diamond) {
                        m_user->bsPlayerBaseInfo().addMoney(-(s32)_info->costValue);
                        m_user->bsPlayerBaseInfo().addChip(idx, -(s32)_chip);
                        m_user->bsPlayerBaseInfo().addDiamond(-(s32)_needDiamond);
                    } else {
                    	 return doResult(-1);
                    }
                }
                //_money -= _info->costValue;
            } else {
                u32 _needDiamond = BSCommonFunc::instance().calc_diamondByRes(_info->costValue - _money);
                //fix
                if((s32)_needDiamond <= _diamond)
                {
                	//fix
                    if ((s32)_info->chipValue <= _chip) {
                        m_user->bsPlayerBaseInfo().addChip(idx, -(s32)_info->chipValue);

                        m_user->bsPlayerBaseInfo().addMoney(-(s32)_money);
                        //_money = 0;
                        m_user->bsPlayerBaseInfo().addDiamond(-(s32)_needDiamond);
                        //_diamond -= _needDiamond;

                    } else {
                        _needDiamond += BSCommonFunc::instance().calc_diamondByChip(_info->chipValue - _chip);
                        //fix
                        if ((s32)_needDiamond <= _diamond) {
                            m_user->bsPlayerBaseInfo().addMoney(-(s32)_money);
                            m_user->bsPlayerBaseInfo().addChip(idx, -(s32)_chip);
                            m_user->bsPlayerBaseInfo().addDiamond(-(s32)_needDiamond);
                        } else {
                        	 return doResult(-1);
                        }
                    }
                }
                else
                {
                	 return doResult(-1);
                }
            }

            break;

        case BSTowerManager::kCT_Gem:
            //fix
            if((s32)_info->costValue <= _diamond)
            {
            	//fix
                if ((s32)_info->chipValue <= _chip) {
                    m_user->bsPlayerBaseInfo().addDiamond(-(s32)_info->costValue);
                    m_user->bsPlayerBaseInfo().addChip(idx, -(s32)_info->chipValue);
                    //_diamond -= _info->costValue;
                } else {
                    m_user->bsPlayerBaseInfo().addDiamond(-(s32)_info->costValue);
                    u32 _needDiamond = BSCommonFunc::instance().calc_diamondByChip(_info->chipValue - _chip);
                    //fix
                    if ((s32)_needDiamond <= _diamond) {
                        m_user->bsPlayerBaseInfo().addChip(idx, -(s32)_chip);
                        m_user->bsPlayerBaseInfo().addDiamond(_needDiamond);
                    } else {
                    	 return doResult(-1);
                    }

                }

            }
            else
            {
            	 return doResult(-1);
            }

            break;

        default:
            break;
    }
    ++m_towerList[idx][num];
    u32 towerclassIndex = idx;
    u32 towerinClassIndex=num;
    u32 upgradeValue= m_towerList[idx][num];
    std::string strUUId= m_user->GetUUId();
    s32 nowClip0 = m_user->bsPlayerBaseInfo().chip(0);
    s32 nowClip1 = m_user->bsPlayerBaseInfo().chip(1);
	 s32 nowClip2 = m_user->bsPlayerBaseInfo().chip(2);
	 s32 nowMoney = m_user->bsPlayerBaseInfo().money();
	 s32 nowDiamond = m_user->bsPlayerBaseInfo().diamond();
	 s32 nowPurchase =m_user->GetPurchaseDiamond();
      ((DC_DataBase *)&DataCenter::instance())->AsyncUpdateTower({m_user,true},towerclassIndex,towerinClassIndex,upgradeValue,[=](bool ret)
    		  {
      	if(!ret)
  		{
      		VN_LOG_ERROR("uuid"<<strUUId<<"towerclassIndex:"<<towerclassIndex<<"towerinClassIndex"<<towerinClassIndex<<"AsyncUpdateTower failed");
      		return doResult(ret);
  		}
      	std::string strUUId= m_user->GetUUId();
      	((DC_DataBase *)&DataCenter::instance())->AsyncUpdateBaseInfo_Baseinfo({m_user,true},strUUId,nowClip0,nowClip1,nowClip2,nowMoney,nowDiamond,nowPurchase,[=](bool ret)
		{
			if(!ret)
			{
				VN_LOG_ERROR("uuid"<<strUUId<<"update basinfo_baseinfo failed");
			}
			return doResult(ret);
		});
  	});
}







const std::vector<std::vector<u32>> & BSTowerUpgrade::towerList()
{
    return m_towerList;
}
