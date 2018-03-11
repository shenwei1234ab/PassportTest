//
//  BSTechUpgrade.h
//  optd_srv
//
//  Created by Cloverz on 8/18/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__BSTechUpgrade__
#define __optd_srv__BSTechUpgrade__

#include <vector>
#include "vnDataStream.h"
#include "vnVariable.h"
#include "../../db/QueryResult.h"
using namespace vn;

class LoggedUser;
class BSTechUpgrade
{
public:
    enum
    {
        kTechCount = 9
    };
    
    BSTechUpgrade(LoggedUser *);
    ~BSTechUpgrade();
    void init();
    void load(u32 version, DataStream &stream);
    void save(DataStream &stream) const;
    
   void save(Variable_object &vobj) const;
   void Asyncsave(u32 idx,std::function<void(bool ret)> doResult);
    void load(Variable_object &vobj);
    void load(PreparedQueryResult result);
    bool upgrade_tech(u32 idx,vn::s32 *diamondPay);
    void Asyncupgrade_tech(u32 idx,std::function<void(int ret)> doResult);
     std::vector<u32> & techList();

private:
    LoggedUser *m_user;
    std::vector<u32> m_techList;
};

#endif /* defined(__optd_srv__BSTechUpgrade__) */
