//
//  BSLevelInfo.h
//  optd_srv
//
//  Created by Cloverz on 8/19/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__BSLevelInfo__
#define __optd_srv__BSLevelInfo__

#include <map>
#include "vnDataStream.h"
#include "vnAllocatedObject.h"
#include "../table/Table_level.h"
#include "vnVariable.h"
#include "../../db/QueryResult.h"
#include "../../passportsvr/entity/AsyncObject.h"
#include <vector>

using namespace vn;

class LoggedUser;
class BSLevel
{
public:
    struct LevelInfo : public AllocatedObject
    {
        LevelInfo() : level(0), starNum(0), score(0), shovel(false), watch(false), intrusion(false), intrusionTimes(0) {}
        
        const tbl::Record_level *level;
        u32 starNum;
        s32 score;
        bool shovel;
        bool watch;
        
        bool intrusion;
        u32 intrusionTimes;
    };
    
    BSLevel(LoggedUser *);
    ~BSLevel();
    void init();
    void load(u32 version, DataStream &stream);
    void save(DataStream &stream) const;
    
    void save(Variable_object &vobj) const;
    void AsyncInsert(u32 levelId,std::function<void(bool ret)> doResult);
    void AsyncUpdate(u32 levelId,std::function<void(bool ret)> doResult);

    void load(Variable_object &vobj);
    void load(PreparedQueryResult result);
    bool update_levelInfo(const tbl::Record_level *data, u32 starNum, s32 score, bool shovel, bool watch, bool *intrusion = 0,bool *ifNew=0,s32 *addDiamond=0);
    //（-1：参数错误 -2: 数据库插入失败）
    void Asyncupdate_levelInfo(const tbl::Record_level *data, u32 starNum, s32 score, bool shovel, bool watch, bool *intrusion,std::function<void(int ret)> doResult);

    LevelInfo * find(u32 id);
    std::map<u32, LevelInfo *> & levelList();
    void pass_level(u32 param); // cheat

    void intrude(u32 levelId);
    bool Syncintrude(u32 levelId);
    void Asyncintrude(AsyncObjectPtr asyncObjPtr,u32 levelId,std::function<void(bool ret)> doResult);
    
    bool passed(u32 levelId);
    
    bool intrusion(u32 levelId);
private:
    void _clear();
    
    LoggedUser *m_user;
    std::map<u32, LevelInfo *> m_levelList;			//保存玩家玩过的所有关卡
};

#endif /* defined(__optd_srv__BSLevelInfo__) */
