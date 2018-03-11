//
//  BSMissionManager.cpp
//  optd_srv
//
//  Created by Cloverz on 2/28/15.
//  Copyright (c) 2015 longsgoo.com. All rights reserved.
//

#include "BSMissionManager.h"

void BSMissionManager::initData(const tbl::Table_mission &table) {
    for (size_t i = 0, c = table.size(); i < c; ++i) {
        auto *mission = table.at(i);
        m_missions.insert({ mission->f_Id.value, *mission });
        //m_rewards.insert({mission->f_Id.value, false});
    }
}

BSMissionManager::Mission * BSMissionManager::find_mission(u32 id) {
    std::map<u32, BSMissionManager::Mission>::iterator it = m_missions.find(id);
    if (it != m_missions.end()) {
        return &it->second;
    }
    
    return 0;
}