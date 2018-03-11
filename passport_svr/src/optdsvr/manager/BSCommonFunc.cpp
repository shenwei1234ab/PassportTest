//
//  BSCommonFunc.cpp
//  optd_srv
//
//  Created by Cloverz on 8/20/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "BSCommonFunc.h"

u32 BSCommonFunc::calc_diamondByRes(u32 res)
{
    u32 _diamond = 0;
    
    if(res > 10000)
    {
        _diamond = (u32)((f32)(KHTC - KTTC)/(f32)(100000 - 10000) * (res - 10000) + KTTC);
    }
    else if(res > 1000)
    {
        _diamond = (u32)((f32)(KTTC - KTHC)/(f32)(10000 - 1000) * (res - 1000) + KTHC);
    }
    else if(res > 100)
    {
        _diamond = (u32)((f32)(KTHC - KHDC)/(f32)(1000 - 100) * (res - 100) + KHDC);
    }
    else if(res > 0)
    {
        _diamond = 1;
    }
    else if(res == 0)
    {
        _diamond = 0;
    }
    
    return _diamond;
}

u32 BSCommonFunc::calc_diamondByChip(u32 chip)
{
    return chip * 10;
}