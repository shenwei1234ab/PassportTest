//
//  BSLevelGroupInfo.cpp
//  optd_srv
//
//  Created by Wayne on 11/7/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "BSLevelGroupInfo.h"

BSLevelGroupInfo::BSLevelGroupInfo(const tbl::Record_levelclass &rec)
: groupId(rec.f_ID.value)
, mineId(rec.f_MineId.value)
, mineValue(rec.f_MineValue.value)
{
    rewards[0].type = rec.f_Reward1Type.value;
    rewards[0].value = rec.f_Reward1Value.value;
    
    rewards[1].type = rec.f_Reward2Type.value;
    rewards[1].value = rec.f_Reward2Value.value;
    
    rewards[2].type = rec.f_Reward3Type.value;
    rewards[2].value = rec.f_Reward3Value.value;
    
    rewards[3].type = rec.f_Reward4Type.value;
    rewards[3].value = rec.f_Reward4Value.value;
    
    rewards[4].type = rec.f_Reward5Type.value;
    rewards[4].value = rec.f_Reward5Value.value;
}

