//
//  BSLevelGroupInfo.h
//  optd_srv
//
//  Created by Wayne on 11/7/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__BSLevelGroupInfo__
#define __optd_srv__BSLevelGroupInfo__

#include "vnAllocatedObject.h"

#include "../table/Table_levelclass.h"
#include "../table/Table_level.h"

#include <array>
#include <map>
#include <set>

using namespace vn;

class BSLevelGroupInfo : public AllocatedObject {
public:
    BSLevelGroupInfo(const tbl::Record_levelclass &rec);
    
    struct Reward {
        u32 type;
        u32 value;
    };
    
    // desc.
    u32 groupId;
    std::array<Reward, 5> rewards;
    u32 mineId;
    u32 mineValue;
    
    std::map<u32, const tbl::Record_level *> levels;
    u32 previousType = 0; // 0, level. 1, group.
    u32 previousId = 0;
    
};


#endif /* defined(__optd_srv__BSLevelGroupInfo__) */
