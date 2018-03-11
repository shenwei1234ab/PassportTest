//
//  BSDiamondShop.h
//  optd_srv
//
//  Created by LEGo on 15/1/6.
//  Copyright (c) 2015 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__BSDiamondShop__
#define __optd_srv__BSDiamondShop__

#include <map>
#include <vector>

#include "vnSingleton.h"
#include "../table/Table_diamond_shop.h"

using namespace vn;

class BSDiamondShop : public Singleton<BSDiamondShop>
{
public:
    struct Product
    {
        Product(u32 id, u32 p, u32 c) :id(id), price(p), count(c){}
        
        u32 id;
        u32 price;
        u32 count;
    };
    
    void initData(const tbl::Table_diamond_shop &table);
    Product * find_product(u32 id);
    
    const std::map<u32, Product> & productMap();
private:
    std::map<u32, Product> m_productMap;
};

#endif /* defined(__optd_srv__BSDiamondShop__) */
