//
//  LoggedUserManager.h
//  optd_srv
//
//  Created by Wayne on 6/20/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__LoggedUserManager__
#define __optd_srv__LoggedUserManager__

#include "vnSingleton.h"
#include "vnReadWriteLock.h"

#include "../worker/LoggedUser.h"

#include <map>

using namespace vn;

class LoggedUserManager : public Singleton<LoggedUserManager> {
public:
    void fini();
    
    void addUser(LoggedUserPtr user);
    /*void removeUser(const UUID &uuid);
    LoggedUserPtr findUser(const UUID &uuid);*/
    void removeUser(const std::string &strUUId);
   LoggedUserPtr findUser(const std::string &strUUId);
    
    void reportLevel();
    
    void broadcast(const std::function<void (LoggedUser *)> &func);
    
private:
    //std::map<UUID, LoggedUserPtr> m_users;
    std::map<std::string , LoggedUserPtr> m_users;
    ReadWriteLock m_lock;
};



#endif /* defined(__optd_srv__LoggedUserManager__) */
