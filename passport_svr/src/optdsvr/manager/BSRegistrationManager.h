//
//  RegistrationManager.h
//  optd_srv
//
//  Created by Cloverz on 3/5/15.
//  Copyright (c) 2015 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__RegistrationManager__
#define __optd_srv__RegistrationManager__

#include "vnSingleton.h"
#include "../table/Table_Registration.h"
#include <map>
#include "../DateTime.h"

using namespace vn;

class BSRegistrationManager : public Singleton<BSRegistrationManager> {
    friend class Singleton<BSRegistrationManager>;
public:
    struct Registration {
        Registration(const tbl::Record_Registration table)
        :id(table.f_Id.value)
        ,stepId(table.f_StepId.value)
        ,theprops(table.f_Theprops.value)
        ,number(table.f_Number.value){
        }
        u32 id;
        u32 stepId;
        u32 theprops;
        u32 number;
    };
    
    struct Reward {
        Reward(){}
        Reward(const tbl::Record_Registration &table)
        :rewardTheprops(table.f_Theprops.value)
        ,rewardNumber(table.f_Number.value){
        }
        u32 rewardTheprops;
        u32 rewardNumber;
    };
    
    bool isLeap(u32 year);
    u32 dayInYear(Date date);
    u32 daysBetween2Date(Date date1, Date date2);
    
    bool isBetween2Time(DateTime date1,DateTime date2,DateTime date3);
    
    void initData(const tbl::Table_Registration &table);
    
    typedef std::map<u32, Reward> rewardGroup;
    
    const std::vector<rewardGroup> & rewardGroups() { return m_rewardGroups; }
private:
    //BSRegistrationManager() = default;
    std::vector<rewardGroup> m_rewardGroups;
};



#endif /* defined(__optd_srv__RegistrationManager__) */
