//
//  BSTechManager.cpp
//  optd_srv
//
//  Created by Cloverz on 8/20/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "vnAssert.h"
#include "BSTechManager.h"

BSTechManager::~BSTechManager()
{
    for (size_t i = 0; i < kNumTech; ++i)
    {
        for (size_t j = 0; j < kNumRank; ++j)
        {
            VN_SAFE_DELETE(m_info[i][j]);
        }
    }
}

void BSTechManager::initData(const tbl::Table_tech &table)
{
    m_info.resize(kNumTech);
    for (size_t i = 0; i < kNumTech; ++i)
    {
        m_info[i].assign(kNumRank, 0);
    }
    
    for (size_t i = 0, c = table.size(); i < c; ++i)
    {
        const tbl::Record_tech &rec = *table.at(i);
        vnassert(rec.f_Slot.value >= 1 && rec.f_Slot.value <= kNumTech);
        vnassert(rec.f_Rank.value >= 0 && rec.f_Rank.value < kNumRank);
        TechItem *&item = m_info[rec.f_Slot.value - 1][rec.f_Rank.value];
        item = new TechItem();
        item->costType = (CostType)rec.f_CostType.value;
        item->costValue = rec.f_CostValue.value;
        item->costTime = rec.f_CostTime.value * 60;
    }
}

const BSTechManager::TechItem * BSTechManager::getTechItem(u32 slot, u32 rank)
{
    if (slot >= kNumTech || rank >= kNumRank)
        return 0;
    
    return m_info[slot][rank];
}
