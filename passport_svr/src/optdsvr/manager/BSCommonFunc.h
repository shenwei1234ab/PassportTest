//
//  BSCommonFunc.h
//  optd_srv
//
//  Created by Cloverz on 8/20/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__BSCommonFunc__
#define __optd_srv__BSCommonFunc__

#include "vnSingleton.h"

using namespace vn;

class BSCommonFunc : public Singleton<BSCommonFunc>
{
public:
    enum
    {
        KHDC = 1,
        KTHC = 9,
        KTTC = 75,
        KHTC = 600
    };
    
    static u32 calc_diamondByRes(u32 res);
    static u32 calc_diamondByChip(u32 chip);
};

#endif /* defined(__optd_srv__BSCommonFunc__) */
