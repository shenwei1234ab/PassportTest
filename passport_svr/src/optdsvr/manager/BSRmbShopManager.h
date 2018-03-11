//
//  BSShopManager.h
//  optd_srv
//
//  Created by Cloverz on 10/9/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__BSShopManager__
#define __optd_srv__BSShopManager__

#include <map>
#include <vector>

#include "vnSingleton.h"

using namespace vn;

class BSRmbShopManager : public Singleton<BSRmbShopManager>
{
public:
    struct Product
    {
        Product() : id(0), price(0), count(0), hidden(false) {}
        
        u32 id;
        u32 price;
        u32 count;
        str8 name;
        str8 desc;
        str8 appleProductId;
        bool hidden;
    };
    
    bool init(u32 fsId, const str8 &path);
    Product * find_product(u32 id);
    
    const std::map<u32, Product> & productMap();
private:
    std::map<u32, Product> m_productMap;
};

#endif /* defined(__optd_srv__BSShopManager__) */
