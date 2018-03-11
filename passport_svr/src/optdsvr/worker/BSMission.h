//
//  BSMission.h
//  optd_srv
//
//  Created by Cloverz on 2/28/15.
//  Copyright (c) 2015 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__BSMission__
#define __optd_srv__BSMission__

#include "vnAllocatedObject.h"
#include "../manager/BSMissionManager.h"
#include <map>
#include "../../db/QueryResult.h"
#include "vnVariable.h"

class LoggedUser;

class BSMission : public AllocatedObject {
public:
    BSMission(LoggedUser *user) : m_user(user) {
        auto m = BSMissionManager::instance().missions();
        for (auto &i : m) {
            m_rewards.insert({i.first, false});
        }
    }
    void save(DataStream &stream) const;
    void load(u8 version, DataStream &stream);
    
    void save(Variable_object &vobj) const;
    void Asyncsave(u32 missionId,std::function<void(bool ret)>doResult);
    void load(Variable_object &vobj);
    void load(PreparedQueryResult result );
    std::map<u32, bool> & list()  { return m_rewards; }
    void reset();
    void setReward(u32 id ,bool b);
    void AsyncsetReward(u32 missionId ,bool missionStatus,std::function<void(bool ret)> doResult);
private:
    LoggedUser *m_user;
    std::map<u32, bool> m_rewards;
};

#endif /* defined(__optd_srv__BSMission__) */
