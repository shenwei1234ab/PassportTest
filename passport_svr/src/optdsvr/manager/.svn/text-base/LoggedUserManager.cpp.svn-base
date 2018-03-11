//
//  LoggedUserManager.cpp
//  optd_srv
//
//  Created by Wayne on 6/20/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "LoggedUserManager.h"
#include "vnLog.h"

void LoggedUserManager::fini() {
    m_users.clear();
}

void LoggedUserManager::addUser(LoggedUserPtr user) {
    vnassert(user);
    m_lock.lock_write();
   /* LoggedUserPtr &p = m_users[user->GetUUId()];
       if (p) {
           VN_LOG_ERROR("LoggedUserManager: {" << user->GetUUId() <<"} already exist!");
       }
    p = std::move(user);*/
    m_users[user->GetUUId()] = user;
    m_lock.unlock();
}

/*void LoggedUserManager::removeUser(const UUID &uuid) {
    m_lock.lock_write();
    m_users.erase(uuid);
    m_lock.unlock();
}

LoggedUserPtr LoggedUserManager::findUser(const UUID &uuid) {
    std::lock_guard<decltype(m_lock)> guard(m_lock);
    auto it = m_users.find(uuid);
    if (it == m_users.end()) {
        return LoggedUserPtr();
    }
    return it->second;
}*/
void LoggedUserManager::removeUser(const std::string &strUUId) {
    m_lock.lock_write();
    m_users.erase(strUUId);
    m_lock.unlock();
}

LoggedUserPtr LoggedUserManager::findUser(const std::string &strUUId) {
    std::lock_guard<decltype(m_lock)> guard(m_lock);
    auto it = m_users.find(strUUId);
    if (it == m_users.end()) {
        return LoggedUserPtr();
    }
    return it->second;
}




void LoggedUserManager::broadcast(const std::function<void (LoggedUser *)> &func) {
    if (!func) {
        return ;
    }
    std::lock_guard<decltype(m_lock)> guard(m_lock);
    for (auto &i : m_users) {
        i.second->queue()->runLoop().post(std::bind([func](LoggedUserPtr user){
            func(user.get());
        }, i.second));
    }
}

void LoggedUserManager::reportLevel() {
    struct Info {
        u32 count = 0;
        u32 count1 = 0;
        u32 s1 = 0;
        u32 s2 = 0;
        u32 s3 = 0;
        u32 sw = 0;
        u32 ss = 0;
    };
    std::map<u32, Info> result;
    for (auto &i : m_users) {
        if (!i.second->bsLevel().levelList().empty()) {
            auto &t = *--i.second->bsLevel().levelList().end();
            ++result[t.first].count1;
        }
        for (auto &j : i.second->bsLevel().levelList()) {
            auto &info = result[j.first];
            ++info.count;
            if (j.second->starNum == 1) {
                ++info.s1;
            } else if (j.second->starNum == 2) {
                ++info.s2;
            } else if (j.second->starNum == 3) {
                ++info.s3;
            }
            if (j.second->watch) {
                ++info.sw;
            }
            if (j.second->shovel) {
                ++info.ss;
            }
        }
    }
    
    for (auto &i : result) {
        VN_LOG_INFO("levelId: " << i.first << " count: " << i.second.count << " count1: " << i.second.count1);
        VN_LOG_INFO("    s1: " << i.second.s1 << " | s2: " << i.second.s2 << " | s3: " << i.second.s3 << " | sw: " << i.second.sw << " | ss: " << i.second.ss);
    }
}
