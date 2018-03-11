//
//  BSLevelGroupInfoManager.h
//  optd_srv
//
//  Created by Wayne on 11/7/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__BSLevelGroupInfoManager__
#define __optd_srv__BSLevelGroupInfoManager__

#include "vnSingleton.h"

#include "../worker/BSLevelGroupInfo.h"

#include <map>

class BSLevelGroupInfoManager : public Singleton<BSLevelGroupInfoManager> {
public:
    
    void initData(const tbl::Table_levelclass &table);
    
    const BSLevelGroupInfo * findGroup(u32 groupId);
    
    const std::map<u32, BSLevelGroupInfo> & groups() { return m_groups; }
    
private:
    std::map<u32, BSLevelGroupInfo> m_groups;
};


#endif /* defined(__optd_srv__BSLevelGroupInfoManager__) */
