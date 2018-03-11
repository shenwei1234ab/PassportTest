//
//  BSFactory.cpp
//  optd_srv
//
//  Created by Cloverz on 9/4/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "BSFactory.h"

const tbl::Record_level * BSFactory::find_level(u32 id)
{
    return m_level.at(m_level.find(id));
}

const tbl::Record_tower * BSFactory::find_tower(u32 id)
{
    return m_tower.at(m_tower.find(id));
}

const tbl::Record_herorune * BSFactory::find_herorune(u32 runeClass, u32 rank)
{
    return m_heroRune.at(m_heroRune.find(runeClass, rank));
}

const tbl::Record_item_store * BSFactory::find_itemStore(u32 id)
{
    return m_itemStore.at(m_itemStore.find(id));
}

tbl::Table_level & BSFactory::level()
{
    return m_level;
}

tbl::Table_tower & BSFactory::tower()
{
    return m_tower;
}

tbl::Table_herorune & BSFactory::herorune()
{
    return m_heroRune;
}

tbl::Table_progress & BSFactory::progress()
{
    return m_progress;
}

tbl::Table_item_store & BSFactory::itemStore()
{
    return m_itemStore;
}

tbl::Table_monster & BSFactory::monster()
{
    return m_monster;
}