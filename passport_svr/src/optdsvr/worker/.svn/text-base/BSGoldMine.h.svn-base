//
//  BSGoldMine.h
//  optd_srv
//
//  Created by Cloverz on 8/26/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__BSGoldMine__
#define __optd_srv__BSGoldMine__

#include <map>
#include <vector>
#include "vnDataStream.h"
#include "vnVariable.h"
#include "../../db/QueryResult.h"
#include "../../passportsvr/entity/AsyncObject.h"

using namespace vn;

class LoggedUser;
class BSGoldMine
{
public:
    struct GoldMine
    {
        struct RelevantLevel
        {
            RelevantLevel() : id(0), group(false), value(0), status(0) {}
            
            u32 id;					//金矿id
            bool group;
            u32 value;				//金矿数量
            u32 status; // 0=close, 1=intrusion, 2=run
        };
        
        GoldMine() : id(0), type(0), yield(0), yieldFactor(0), nowNum(0), maxNum(0), timer(0) {}
        bool update(f32 deltaTime);
        void calcYield();
        
        u32 id;
        u32 type;                    // 产出类型	0:金子  1:钻石
        f32 yield;                              // 每天产量   _calcYield()
        f32 yieldFactor;                        // 满开发增长系数
        u32 nowNum;                             // 当前储量
        u32 maxNum;                             // 最高储量(读表)
	     vn::u64 lastTimestamp;
        f32 timer;
        
        std::map<std::pair<u32, bool>, RelevantLevel> levels;  // 关联关卡, 是否提供产量
    };
    
    BSGoldMine(LoggedUser *);

    void init();
    void load(u32 version, DataStream &stream);
    void save(DataStream &stream) const;
    
    void save(Variable_object &vobj) const;
    void AsyncSave(AsyncObjectPtr asyncObjectPtr,std::function<void(bool ret)> doResult);
    void AsyncSave(AsyncObjectPtr asyncObjectPtr,u32 id,std::function<void(bool ret)> doResult);
    void load(Variable_object &vobj);
    void load(PreparedQueryResult result);
    void update(f32 deltaTime);

    void Asyncupdate(f32 deltaTime,std::function<void(bool ret)>doResult);
    bool collect(u32 id, u32 num,s32 *addDiamond);
    void Asynccollect(u32 id, u32 num,std::function<void(int ret)> doResult);
    //s32 levelStatus(u32 levelId);
   std::map<u32, GoldMine> & mines() { return m_mines; }
    
    void notifyVictory(u32 mineId, u32 levelId, bool group);
    void notifyIntrusion(u32 mineId, u32 levelId, bool group);
    
    void _setStatus(u32 mineId, u32 levelId, bool group, u32 status);
    
    void _calcYield();
    
    static bool loadInitialGoldMineList(u32 fsId, const str8 &path);
private:


    LoggedUser *m_user;
    std::map<u32, GoldMine> m_mines;
    
    static decltype(m_mines) s_initalMines;
};

#endif /* defined(__optd_srv__BSGoldMine__) */
