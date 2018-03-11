//
//  BSPlatformStore.cpp
//  optd_srv
//
//  Created by Wayne on 11/4/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "BSPlatformStore.h"
#include "LoggedUser.h"
#include "vnVariableAccessor.h"
#include "../manager/DC_DataBase.h"

void BSPlatformStore::reset() {
    for (auto &i : m_count) {
        i = 1;
    }
}

void BSPlatformStore::save(DataStream &stream) const {
    for (auto i : m_count) {
        stream << i;
    }
}

void BSPlatformStore::load(u8 version, DataStream &stream) {
    for (auto &i : m_count) {
        stream >> i;
    }
}

void BSPlatformStore::save(Variable_object &vobj) const {
    auto p = vnnew Variable_object();
    vobj.setAttribute("platform_store", p);
    for (auto i : m_count) {
        p->add(vnnew Variable_int32(i));
    }
}

void BSPlatformStore::load(Variable_object &vobj) {
    VariableAccessor<Variable_int32> item(vobj.queryAttributeObject("platform_store"));
    for (auto &i : m_count) {
        if (item.fetch()) {
            i = item.at<0>()->value();
        } else {
            break;
        }
    }
}


void BSPlatformStore::load(PreparedQueryResult result )
{
	for(auto &i:m_count)
	{
		i= (*result)[0].GetUInt32();
		if(!result->NextRow())
		{
			break;
		}
	}
}

bool BSPlatformStore::buy(u32 category,s32 *payDiamond) {
    if (category >= BSPlatformStoreInfo::kCategoryCount) {
        return false;
    }
    auto &t = m_count[category];
    if (BSPlatformStoreInfo::instance().buy(category, t, m_user->bsPlayerBaseInfo(),payDiamond)) {
        ++t;
        return true;
    }
    return false;
}

//update
void BSPlatformStore::Asyncsave(u32 category,std::function<void(bool ret)>doResult)
{
	 if (category >= BSPlatformStoreInfo::kCategoryCount)
	    {
	        return doResult(false);
	    }
	    u32 level = m_count[category];
	    std::string strUUid = m_user->GetUUId();
	    ((DC_DataBase *)&DataCenter::instance())->AsyncUpdatePlatform({m_user,true},strUUid,category,level,doResult);

}

void BSPlatformStore::Asyncbuy(u32 category,std::function<void(int ret)> doResult)
{
 /*   if (category >= BSPlatformStoreInfo::kCategoryCount)
    {
        return doResult(-1);
    }
    auto &t = m_count[category];
    if (BSPlatformStoreInfo::instance().buy(category, t, m_user->bsPlayerBaseInfo()))
    {
        ++t;
        u32 level = t;
        //存数据库
        std::string strUUid=m_user->GetUUId();
       ((DC_DataBase *)&DataCenter::instance())->AsyncUpdatePlatform({m_user,true},strUUid,category,level,doResult);
    }
    else
    {
    	return doResult(-1);
    }*/
}



