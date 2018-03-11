//
//  BSMissionManager.h
//  optd_srv
//
//  Created by Cloverz on 2/28/15.
//  Copyright (c) 2015 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__BSMissionManager__
#define __optd_srv__BSMissionManager__

#include "vnSingleton.h"
#include "../table/Table_mission.h"
#include <map>

using namespace vn;

class BSMissionManager : public Singleton<BSMissionManager> {
    friend class Singleton<BSMissionManager>;
public:
    struct Mission {
        Mission(const tbl::Record_mission &table)
        :rewardType(table.f_RewardType.value)
        , rewardNum(table.f_RewardValue.value)
        , level(table.f_ParameterA.value)
        , levelType(table.f_Task.value){
        }
        u32 rewardType;
        u32 rewardNum;
        u32 level;
        u32 levelType;
    };
    
    void initData(const tbl::Table_mission &table);
    Mission * find_mission(u32 id);
    
    const std::map<u32, Mission> & missions() { return m_missions; }
    
private:
    //BSMissionManager() = default;
    std::map<u32, Mission> m_missions;
};

#endif /* defined(__optd_srv__BSMissionManager__) */
