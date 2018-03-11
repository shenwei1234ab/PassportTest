//
//  BSTowerUpgrade.h
//  optd_srv
//
//  Created by Cloverz on 8/19/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__BSTowerUpgrade__
#define __optd_srv__BSTowerUpgrade__

#include "vnDataStream.h"

using namespace vn;

class LoggedUser;
class BSTowerUpgrade
{
public:
    enum
    {
        kNumClass = 3,
        kNumTowerInClass = 3,
        kMaxAdvancedTimes = 3
    };
    
    BSTowerUpgrade(LoggedUser *);
    ~BSTowerUpgrade();
    void init();
    void load(u32 version, DataStream &stream);
    void save(DataStream &stream) const;
    
    void save(Variable_object &vobj) const;
    void	Asyncsave(u32 idx, u32 num,std::function<void(bool ret)> doResult);
    void load(Variable_object &vobj);
    

    void load(PreparedQueryResult  result);

    bool upgrade_tower(u32 idx, u32 num,vn::s32 *diamondPay);
    void  upgrade_tower(u32 idx, u32 num,std::function<void(int ret)> doResult);
    void  Asyncupgrade_tower(u32 idx, u32 num,std::function<void(int ret)> doResult);
    const std::vector<std::vector<u32>> & towerList();
private:
    LoggedUser *m_user;
    std::vector<std::vector<u32>> m_towerList;
};

#endif /* defined(__optd_srv__BSTowerUpgrade__) */
