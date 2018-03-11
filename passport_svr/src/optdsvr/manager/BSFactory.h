//
//  BSFactory.h
//  optd_srv
//
//  Created by Cloverz on 9/4/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__BSFactory__
#define __optd_srv__BSFactory__

#include "vnSingleton.h"
#include "../table/Table_tower.h"
#include "../table/Table_herorune.h"
#include "../table/Table_progress.h"
#include "../table/Table_level.h"
#include "../table/Table_levelclass.h"
#include "../table/Table_monster.h"
#include "../table/Table_item_store.h"
#include "../table/Table_diamond_shop.h"

using namespace vn;

class BSFactory : public Singleton<BSFactory>
{
public:
    const tbl::Record_level * find_level(u32 id);
    const tbl::Record_tower * find_tower(u32 id);
    const tbl::Record_herorune * find_herorune(u32 runeClass, u32 rank);
    const tbl::Record_item_store * find_itemStore(u32 id);
    
    tbl::Table_level & level();
    tbl::Table_tower & tower();
    tbl::Table_herorune & herorune();
    tbl::Table_progress & progress();
    tbl::Table_item_store & itemStore();
    tbl::Table_monster & monster();
private:
    tbl::Table_level m_level;
    tbl::Table_tower m_tower;
    tbl::Table_herorune m_heroRune;
    tbl::Table_progress m_progress;
    tbl::Table_item_store m_itemStore;
    tbl::Table_monster m_monster;
};

#endif /* defined(__optd_srv__BSFactory__) */
