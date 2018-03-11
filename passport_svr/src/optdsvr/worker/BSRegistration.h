//
//  BSRegistration.h
//  optd_srv
//
//  Created by Cloverz on 3/5/15.
//  Copyright (c) 2015 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__BSRegistration__
#define __optd_srv__BSRegistration__

#include "vnAllocatedObject.h"
#include "../manager/BSRegistrationManager.h"
#include <map>
#include "../database/Dbtable.h"
#include "vnVariable.h"

class LoggedUser;

class BSRegistration : public AllocatedObject {
public:
    BSRegistration(LoggedUser *user) : m_user(user) {
        m_firstDate.year = 0;
        m_lastDate.year = 0;
        m_nums = 0;
        m_group = 0;
    }
    void save(DataStream &stream) const;
    void load(u8 version, DataStream &stream);
    
    void save(Variable_object &vobj) const;
    void load(Variable_object &vobj);

    void Asyncsave(std::function<void(bool ret)> doResult);
    void reset();
    
    //数据库
    //void load(DBTable_baseinfo &tbaseInfo);
    void load(PreparedQueryResult baseinfo);


    Date m_firstDate;
    Date m_lastDate;
    vn::u32 m_nums = 0;
    vn::u32 m_group = 0;
private:
    LoggedUser *m_user;
};


#endif /* defined(__optd_srv__BSRegistration__) */
