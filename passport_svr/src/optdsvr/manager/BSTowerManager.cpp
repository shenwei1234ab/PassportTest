//
//  BSTowerManager.cpp
//  optd_srv
//
//  Created by Cloverz on 8/20/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "vnAssert.h"
#include "BSTowerManager.h"

void BSTowerManager::initData(const tbl::Table_training &table)
{
    for (size_t i = 0, c = table.size(); i < c; ++i)
    {
        const tbl::Record_training *rec = table.at(i);
        vnassert(rec->f_Class.value >= 1 && rec->f_Class.value <= kNumClass);
        vnassert(rec->f_Slot.value >= 1 && rec->f_Slot.value <= kNumTowerInClass);
        /////////////内存
        vnassert(rec->f_AdvancedTimes.value >= 0 && rec->f_AdvancedTimes.value < kMaxAdvancedTimes);
        u32 index = rec->f_Class.value - 1;
        u32 slot = rec->f_Slot.value - 1;
        
        TowerInfo &info = m_info[index][slot];
        if (rec->f_AdvancedTimes.value == 0)
        {
            info.towerId = rec->f_TowerId.value;
        }
        TrainingInfo &ti = info.training[rec->f_AdvancedTimes.value];
        ti.costType = (CostType)rec->f_CostType.value;
        ti.costValue = rec->f_CostValue.value;
        ti.chipType = (ChipType)rec->f_ChipType.value;
        ti.chipValue = rec->f_ChipValue.value;
        //ti.costTime = rec->f_CostTime.value * 60;
    }
}

const BSTowerManager::TrainingInfo * BSTowerManager::getTrainingInfo(u32 index, u32 num, u32 count)
{
    if (index >= kNumClass || num >= kNumTowerInClass)
        return 0;
    
    TowerInfo &_info = m_info[index][num];
    return &_info.training[count];
}
