//
//  BSMonsterMap.h
//  optd_srv
//
//  Created by Cloverz on 10/13/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__BSMonsterMap__
#define __optd_srv__BSMonsterMap__

#include <map>

#include "vnDataStream.h"
#include "vnVariable.h"
#include "../../db/QueryResult.h"
using namespace vn;

class LoggedUser;
class BSMonsterMap
{
public:
    
    BSMonsterMap(LoggedUser *);
    ~BSMonsterMap();
    void init();
    void load(u32 version, DataStream &stream);
    void save(DataStream &stream) const;
    
    void save(Variable_object &vobj) const;
    void load(Variable_object &vobj);
    void load(PreparedQueryResult result );
    void Asyncsave(u32 id,std::function<void(bool ret)>doResult);
    bool meet_monster(u32 id);
    void  unmeet_monster(u32 id);
     std::map<u32, bool>  & monsterMap();
private:
    LoggedUser *m_user;
    std::map<u32, bool> m_monsterMap;
};

#endif /* defined(__optd_srv__BSMonsterMap__) */
