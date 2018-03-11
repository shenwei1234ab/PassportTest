//
//  BSPlatformStore.h
//  optd_srv
//
//  Created by Wayne on 11/4/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__BSPlatformStore__
#define __optd_srv__BSPlatformStore__

#include "BSPlatformStoreInfo.h"
#include "vnVariable.h"
#include "../../db/QueryResult.h"
using namespace vn;

class LoggedUser;

class BSPlatformStore {
public:
    BSPlatformStore(LoggedUser *user) : m_user(user) {}
    
    void reset();
    
    void save(DataStream &stream) const;
    void load(u8 version, DataStream &stream);
    
    void save(Variable_object &vobj) const;
    void load(Variable_object &vobj);
    void load(PreparedQueryResult result );
    
    bool buy(u32 category,s32 *payDiamond);
    void Asyncsave(u32 category,std::function<void(bool ret)>doResult);
    void Asyncbuy(u32 category,std::function<void(int ret)> doResult);
     std::array<u32, BSPlatformStoreInfo::kCategoryCount> & list()  { return m_count; }
    
private:
    LoggedUser *m_user;
    std::array<u32, BSPlatformStoreInfo::kCategoryCount> m_count;
};

#endif /* defined(__optd_srv__BSPlatformStore__) */
