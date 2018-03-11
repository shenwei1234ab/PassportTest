//
//  BSPlatformStoreInfo.h
//  optd_srv
//
//  Created by Wayne on 11/4/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__BSPlatformStoreInfo__
#define __optd_srv__BSPlatformStoreInfo__

#include "vnSingleton.h"
#include "../table/Table_platform.h"

#include <array>

using namespace vn;

class BSPlayerBaseInfo;

class BSPlatformStoreInfo : public Singleton<BSPlatformStoreInfo> {
public:
    enum {
        kMaxNum = 9,
        kCategoryCount = 3
    };
    
    
    void initData(const tbl::Table_platform &table);
    
    bool buy(u32 category, u32 currentNum, BSPlayerBaseInfo &player,s32 *payDiamond);
private:
    struct PlatformInfo {
        std::array<s32, kMaxNum> price;
    };
    
    std::array<PlatformInfo, kCategoryCount> m_info;
};


#endif /* defined(__optd_srv__BSPlatformStoreInfo__) */
