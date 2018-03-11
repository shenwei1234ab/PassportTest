//
//  BSDiamondShop.cpp
//  optd_srv
//
//  Created by LEGo on 15/1/6.
//  Copyright (c) 2015年 longsgoo.com. All rights reserved.
//

#include "BSDiamondShop.h"
#include "vnVariableScript.h"

void BSDiamondShop::initData(const tbl::Table_diamond_shop &table) {
    m_productMap.clear();
    
    for (size_t i = 0, c = table.size(); i < c; ++i) {				//size()返回行数
        auto *rec = table.at(i);											//rec指向第几行
        Product product(rec->f_Id.value,rec->f_DiamondCost.value,rec->f_GoldValue.value);
        m_productMap.insert(std::make_pair(product.id, product));
    }
}

BSDiamondShop::Product * BSDiamondShop::find_product(u32 id) {
    std::map<u32, BSDiamondShop::Product>::iterator it = m_productMap.find(id);
    if (it != m_productMap.end())
    {
        return &it->second;
    }
    
    return 0;
}

const std::map<u32, BSDiamondShop::Product> & BSDiamondShop::productMap() {
    return m_productMap;
}
