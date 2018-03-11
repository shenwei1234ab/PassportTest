//
//  BSPlayerBaseInfo.h
//  optd_srv
//
//  Created by Cloverz on 8/18/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__BSPlayerBaseInfo__
#define __optd_srv__BSPlayerBaseInfo__

#include "vnDataStream.h"
#include "vnVariable.h"
#include "../database/Dbtable.h"
using namespace vn;

class LoggedUser;
class BSPlayerBaseInfo
{
public:
    BSPlayerBaseInfo(LoggedUser *);
    BSPlayerBaseInfo(LoggedUser *,s32 money,s32 diamond,s32 clip0,s32 clip1,s32 clip2);
    ~BSPlayerBaseInfo();
    void init(vn::s32 diamond=0);
    void load(u32 version, DataStream &stream);
    void save(DataStream &stream) const;
    void save(Variable_object &vobj) const;
    void load(Variable_object &vobj);

    void load(DBTable_baseinfo &tbaseinfo);
    void load(PreparedQueryResult baseinfo,vn::s32 balance);
    void addMoney(s32 value);
    void addDiamond(s32 value);
    void addChip(u32 index, s32 value);
    void addByType(u32 type, s32 value,s32 *addDiamond);
    void addChipByType(u32 type,s32 value);
    
    s32 money();
    s32 diamond();
    void SetDiamond(s32 diamond);
    s32 chip(u32 index);
    
private:
    s32 m_money;
    s32 m_diamond;
    LoggedUser *m_user;
    
    //add by ts
    s32 m_chip[3];
};

#endif /* defined(__optd_srv__BSPlayerBaseInfo__) */
