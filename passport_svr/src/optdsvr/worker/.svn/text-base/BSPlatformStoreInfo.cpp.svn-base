//
//  BSPlatformStoreInfo.cpp
//  optd_srv
//
//  Created by Wayne on 11/4/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "BSPlatformStoreInfo.h"
#include "BSPlayerBaseInfo.h"
#include "../manager/BSCommonFunc.h"

#include "vnAssert.h"

void BSPlatformStoreInfo::initData(const tbl::Table_platform &table) {
    for (size_t i = 0, c = table.size(); i < c; ++i) {
        auto *p = table.at(i);
        if (p->f_Category.value < kCategoryCount) {
            auto &t = m_info[p->f_Category.value];
            t.price[0] = p->f_Price1.value;
            t.price[1] = p->f_Price2.value;
            t.price[2] = p->f_Price3.value;
            t.price[3] = p->f_Price4.value;
            t.price[4] = p->f_Price5.value;
            t.price[5] = p->f_Price6.value;
            t.price[6] = p->f_Price7.value;
            t.price[7] = p->f_Price8.value;
            t.price[8] = p->f_Price9.value;
        }
    }
}

bool BSPlatformStoreInfo::buy(u32 category, u32 currentNum, BSPlayerBaseInfo &player,s32 *payDiamond) {
    vnassert(category < kCategoryCount);
    if (currentNum >= kMaxNum) {
        return false;
    }
    s32 price = m_info[category].price[currentNum];
    if (player.diamond() >= price) {
    	if(payDiamond)
    	{
    		 *payDiamond  =-price;
    	}
        //player.addDiamond(-price);
        return true;
    }
    return false;
}
