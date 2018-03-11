//
//  BSTechManager.h
//  optd_srv
//
//  Created by Cloverz on 8/20/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__BSTechManager__
#define __optd_srv__BSTechManager__

#include "vnSingleton.h"
#include "../table/Table_tech.h"

using namespace vn;

class BSTechManager : public Singleton<BSTechManager>
{
public:
    enum
    {
        kNumTech = 9,
        kNumRank = 10
    };
    
    enum CostType
    {
        kCT_Invalid = 0,
        kCT_Money = 1,
        kCT_Gem = 2
    };
    
    struct TechItem
    {
        TechItem() :costType(kCT_Invalid), costValue(0), costTime(0) {}
        
        CostType costType;
        s32 costValue;
        s32 costTime;
    };
    
    ~BSTechManager();
    void initData(const tbl::Table_tech &table);
    
    const TechItem * getTechItem(u32 slot, u32 rank);
private:
    typedef std::vector<TechItem *> TechInfo;
    typedef std::vector<TechInfo> TechInfoVector;
    TechInfoVector m_info;
};

#endif /* defined(__optd_srv__BSTechManager__) */
