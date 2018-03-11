//
//  BSItemStore.cpp
//  optd_srv
//
//  Created by Cloverz on 10/11/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "../manager/BSFactory.h"
#include "BSItemStore.h"
#include "LoggedUser.h"
#include "vnVariableAccessor.h"
#include "../manager/DC_DataBase.h"
BSItemStore::BSItemStore(LoggedUser *user)
: m_user(user) {}

BSItemStore::~BSItemStore()
{
    m_user = 0;
    m_itemMap.clear();
}

void BSItemStore::init()
{
    m_itemMap.clear();
    
    tbl::Table_item_store &table = BSFactory::instance().itemStore();
    for (uint i = 0; i < table.size(); ++i)
    {
        const tbl::Record_item_store *ris = table.at(i);
        if (ris)
            m_itemMap.insert(std::make_pair(ris->f_id.value, 0));
    }
}


void BSItemStore::load(PreparedQueryResult result)
{
	if(!result->GetRowCount())
	{
		return;
	}
	do
	{
		uint itemId = (*result)[0].GetUInt32();
		uint itemStatus= (*result)[1].GetUInt32();
		m_itemMap[itemId]=itemStatus;
	}while(result->NextRow());
}

void BSItemStore::load(u32 version, DataStream &stream)
{
    m_itemMap.clear();
    
    u32 size = 0;
    stream >> size;
    for (uint i = 0; i < size; ++i)
    {
        u32 id = 0, count = 0;
        stream >> id >> count;
        m_itemMap.insert(std::make_pair(id, count));
    }
}

void BSItemStore::save(DataStream &stream) const
{
    stream << (u32)m_itemMap.size();
    for (auto &i : m_itemMap)
    {
        stream << i.first << i.second;
    }
}

void BSItemStore::save(Variable_object &vobj) const {
    auto p = vnnew Variable_object();
    vobj.setAttribute("item_store", p);
    for (auto &i : m_itemMap) {
        p->add(vnnew Variable_int32(i.first));
        p->add(vnnew Variable_int32(i.second));
    }
}

void BSItemStore::load(Variable_object &vobj) {
    m_itemMap.clear();
    VariableAccessor<Variable_int32, Variable_int32> item(vobj.queryAttributeObject("item_store"));
    while (item.fetch()) {
        m_itemMap[item.at<0>()->value()] = item.at<1>()->value();
    }
}

//update
void BSItemStore::Asyncsave(u32 id,std::function<void (bool ret)> doResult)
{
	auto it = m_itemMap.find(id);
	if (it == m_itemMap.end())
	{
		return doResult(false);
	}
	u32 itemNum = it->second;
	((DC_DataBase *)&DataCenter::instance())->AsyncUpdateItemStore({m_user,true},id,itemNum,doResult);
}

bool BSItemStore::buy(u32 id,s32 *payDiamond)
{
    auto i = m_itemMap.find(id);
    if (i != m_itemMap.end())
    {
        const tbl::Record_item_store *ris = BSFactory::instance().find_itemStore(id);
        if (ris && (s32)ris->f_price.value <= m_user->bsPlayerBaseInfo().diamond())
        {
            ++i->second;
          //  m_user->bsPlayerBaseInfo().addDiamond(-(s32)ris->f_price.value);
            if(payDiamond)
            {
            	*payDiamond =	-(s32)ris->f_price.value;
            }
            return true;
        }
            
    }

    return false;
}



//-1 无效的操作   0 数据库插入失败  1
void BSItemStore::AsyncBuy(u32 id,std::function<void (int result)>doResult)
{
	auto i = m_itemMap.find(id);
	if (i == m_itemMap.end())
	{
		return doResult(-1);
	}
	else
	{
		const tbl::Record_item_store *ris = BSFactory::instance().find_itemStore(id);
		if(!ris || (s32)ris->f_price.value > m_user->bsPlayerBaseInfo().diamond())
		{
			return doResult(-1);
		}
		//std::string strUUId = m_user->uuid().to_string();
		u32 itemNum = ++i->second;
		m_user->bsPlayerBaseInfo().addDiamond(-(s32)ris->f_price.value);
		((DC_DataBase *)&DataCenter::instance())->AsyncUpdateItemStore({m_user,true},id,itemNum,[doResult](bool result)
		{
			if(!result)
			{
				VN_LOG_ERROR("AsyncBuyItemStore FAILED");
			}
			return doResult(result);
		});
	}
}





bool BSItemStore::use(u32 id)
{
    auto i = m_itemMap.find(id);
    if (i != m_itemMap.end() && i->second > 0)
    {
        --i->second;
        return true;
    }
    return false;
}


void BSItemStore::AsyncUse(u32 id,std::function<void (int result)>doResult)
{
	auto i = m_itemMap.find(id);
	if (i == m_itemMap.end() || i->second <= 0)
	{
		return doResult(-1);
	}
	else
	{
		u32 itemid = i->first;
		u32 itemstatus = --i->second;
		((DC_DataBase *)&DataCenter::instance())->AsyncUpdateItemStore({m_user,true},itemid,itemstatus,[doResult](bool result)
		{
			if(!result)
			{
				VN_LOG_ERROR("AsyncUseItemStore FAILED");
			}
			return doResult(result);
		});
	}
}







 std::map<u32, u32> & BSItemStore::itemMap()
{
    return m_itemMap;
}

void BSItemStore::_add(u32 id, u32 count) {
    auto it = m_itemMap.find(id);
    if (it != m_itemMap.end()) {
        it->second += count;
    }
}


void BSItemStore::Asyncadd(u32 id, u32 count,std::function<void (bool ret)>doResult)
{
    auto it = m_itemMap.find(id);
    if(it == m_itemMap.end())
    {
    	return doResult(true);
    }
	it->second += count;
	u32 itemcount = it->second;
	std::string strUUId = m_user->GetUUId();
	((DC_DataBase *)&DataCenter::instance())->AsyncUpdateItemStore({m_user,true},id,itemcount,[doResult,strUUId,id](bool result)
	{
		if(!result)
		{
			VN_LOG_ERROR("uuid:"<<strUUId<<"itemid "<<id<<"update failed");
		}
		return doResult(result);
	});
}





