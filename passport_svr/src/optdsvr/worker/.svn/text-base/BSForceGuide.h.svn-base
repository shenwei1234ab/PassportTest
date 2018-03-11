//
//  BSForceGuide.h
//  optd_srv
//
//  Created by Cloverz on 10/23/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__BSForceGuide__
#define __optd_srv__BSForceGuide__

#include <set>

#include "vnDataStream.h"
#include "vnVariable.h"

using namespace vn;

class LoggedUser;
class BSForceGuide
{
public:
    BSForceGuide(LoggedUser *user);
    ~BSForceGuide();
    void init();
    void load(u32 version, DataStream &stream);
    void save(DataStream &stream) const;
    
    void save(Variable_object &vobj) const;
    void load(Variable_object &vobj);



    void load(PreparedQueryResult result);
    bool insert(u32 data);
    void erase(u32 data);
    
    const std::set<u32> & disableList();
private:
    LoggedUser *m_user;
    std::set<u32> m_disableList;
};

#endif /* defined(__optd_srv__BSForceGuide__) */
