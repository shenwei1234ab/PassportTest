//
//  BSItemStore.h
//  optd_srv
//
//  Created by Cloverz on 10/11/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__BSItemStore__
#define __optd_srv__BSItemStore__

#include <map>

#include "vnDataStream.h"
#include "vnVariable.h"
#include "../../db/QueryResult.h"
using namespace vn;

class LoggedUser;
class BSItemStore
{
public:
    BSItemStore(LoggedUser *);
    ~BSItemStore();
    void init();
    void load(u32 version, DataStream &stream);
    void save(DataStream &stream) const;
    
    void save(Variable_object &vobj) const;
    void Asyncsave(u32 id,std::function<void (bool ret)> doResult);
    void load(Variable_object &vobj);

    void load(PreparedQueryResult result);
    bool buy(u32 itemId,s32 *payDiamond);
    bool use(u32 itemId);
    

    void AsyncBuy(u32 id,std::function<void (int result)>doResult);
    void AsyncUse(u32 id,std::function<void (int result)>doResult);

    std::map<u32, u32> & itemMap();
    
    void _add(u32 id, u32 count);
    void Asyncadd(u32 id, u32 count,std::function<void (bool ret)>doResult);
private:
    LoggedUser *m_user;
    std::map<u32, u32> m_itemMap;
};

#endif /* defined(__optd_srv__BSItemStore__) */
