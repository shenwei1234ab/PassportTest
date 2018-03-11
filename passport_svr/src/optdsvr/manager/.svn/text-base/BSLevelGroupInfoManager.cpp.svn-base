//
//  BSLevelGroupInfoManager.cpp
//  optd_srv
//
//  Created by Wayne on 11/7/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "BSLevelGroupInfoManager.h"

#include "BSFactory.h"


void BSLevelGroupInfoManager::initData(const tbl::Table_levelclass &table) {
    m_groups.clear();
    
    for (size_t i = 0, c = table.size(); i < c; ++i) {
        auto *rec = table.at(i);
        m_groups.insert({rec->f_ID.value, *rec});
    }
    
    auto &levels = BSFactory::instance().level();
    for (size_t i = 0, c = levels.size(); i < c; ++i) {
        auto *rec = levels.at(i);
        if (rec->f_LevelGroupId.value) {
            auto it = m_groups.find(rec->f_LevelGroupId.value);
            if (it != m_groups.end()) {
                it->second.levels.insert({rec->f_LevelId.value, rec});
            }
        }
    }
    
    for (auto &i : m_groups) {
        if (i.second.levels.empty()) {
            continue;
        }
        auto p = i.second.levels.begin()->second;
        if (!p->f_PreId.null) {
            i.second.previousType = p->f_PreType.value;
            i.second.previousId = p->f_PreId.value;
        }
    }
}

const BSLevelGroupInfo * BSLevelGroupInfoManager::findGroup(u32 groupId) {
    auto it = m_groups.find(groupId);
    if (it == m_groups.end()) {
        return nullptr;
    }
    return &it->second;
}