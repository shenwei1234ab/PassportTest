//
//  BSTowerManager.h
//  optd_srv
//
//  Created by Cloverz on 8/20/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__BSTowerManager__
#define __optd_srv__BSTowerManager__

#include "vnSingleton.h"
#include "../table/Table_training.h"

using namespace vn;

class BSTowerManager : public Singleton<BSTowerManager>
{
public:
    enum
    {
        kNumClass = 3,
        kNumTowerInClass = 3,
        kMaxAdvancedTimes = 4
    };
    
    enum CostType
    {
        kCT_Invalid = 0,
        kCT_Money = 1,
        kCT_Gem = 2,
        kCT_Chip = 3
    };
    
    enum ChipType
    {
        kCT_Inval = 0,
        kCT_Green = 1,
        kCT_Red = 2,
        kCT_White = 3
    };
    
    struct TrainingInfo
    {
        TrainingInfo() :costType(kCT_Invalid), costValue(0), chipType(kCT_Inval), chipValue(0) {}
        
        CostType costType;
        u32 costValue;
        ChipType chipType;
        u32 chipValue;
        //u32 costTime; // in seconds.
        
        
    };
    struct TowerInfo
    {
        TowerInfo() :towerId(0) {}
        
        u32 towerId;
        TrainingInfo training[kMaxAdvancedTimes];
    };
    
    void initData(const tbl::Table_training &table);
    const TrainingInfo * getTrainingInfo(u32 index, u32 num, u32 count);
private:
    TowerInfo m_info[kNumClass][kNumTowerInClass];
};

#endif /* defined(__optd_srv__BSTowerManager__) */
