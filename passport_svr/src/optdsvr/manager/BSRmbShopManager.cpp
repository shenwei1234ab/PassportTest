//
//  BSShopManager.cpp
//  optd_srv
//
//  Created by Cloverz on 10/9/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "BSRmbShopManager.h"
#include "vnVariableScript.h"

bool BSRmbShopManager::init(u32 fsId, const str8 &path)
{
    m_productMap.clear();
    
    VariableScript vs;
    auto root = vs.parse({fsId, path});
    if (!root)
        return false;
    
    Variable_object *ptr = root.get();
    //fix
    for (size_t i = ptr->findObjectIndex(); (int)i != -1; i = ptr->findObjectIndex(i))
    {
        auto p = static_cast<Variable_object *>(ptr->at(i).get());
        if (p)
        {
            Product product;
            product.id = (u32)p->queryAttributeInt32("id");
            product.price = (u32)p->queryAttributeInt32("price");
            product.count = (u32)p->queryAttributeInt32("count");
            product.name = p->queryAttributeString("name");
            product.desc = p->queryAttributeString("desc");
            product.appleProductId = p->queryAttributeString("appleProductId");
            product.hidden = p->queryAttributeBool("hidden");
            m_productMap.insert(std::make_pair(product.id, product));
        }
    }
    return true;
}

BSRmbShopManager::Product * BSRmbShopManager::find_product(u32 id)
{
    std::map<u32, BSRmbShopManager::Product>::iterator it = m_productMap.find(id);
    if (it != m_productMap.end())
    {
        return &it->second;
    }
    
    return 0;
}

const std::map<u32, BSRmbShopManager::Product> & BSRmbShopManager::productMap()
{
    return m_productMap;
}
