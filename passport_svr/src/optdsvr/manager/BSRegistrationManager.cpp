//
//  RegistrationManager.cpp
//  optd_srv
//
//  Created by Cloverz on 3/5/15.
//  Copyright (c) 2015 longsgoo.com. All rights reserved.
//

#include "BSRegistrationManager.h"

bool BSRegistrationManager::isLeap(u32 year) {
    return year % 400 == 0 || (year % 4 == 0 && (year % 100 != 0));
}

u32 BSRegistrationManager::dayInYear(Date date)
{
    u32 _day = 0;
    u32 DAY[12]={31,28,31,30,31,30,31,31,30,31,30,31};
    if(isLeap(date.year))
        DAY[1] = 29;
    for(int i = 0; i<date.month - 1; ++i)
    {
        _day += DAY[i];
    }
    _day += date.day;
    return _day;
}

u32 BSRegistrationManager::daysBetween2Date(Date date1, Date date2) {
    if (date1.year == date2.year && date1.month == date2.month) {
        return date1.day > date2.day ? date1.day - date2.day : date2.day - date1.day;
    } else if (date1.year == date2.year) {
        u32 d1, d2;
        d1 = dayInYear(date1);
        d2 = dayInYear(date2);
        return d1 > d2 ? d1 - d2 : d2 - d1;
    } else {
        if (date1.year > date2.year) {
            date1.date = date1.date ^ date2.date;
            date2.date = date1.date ^ date2.date;
            date1.date = date1.date ^ date2.date;
        }
        u32 d1,d2,d3;
        if(isLeap(date1.year))
            d1 = 366 - dayInYear(date1);
        else
            d1 = 365 - dayInYear(date1);
        d2 = dayInYear(date2);
        //cout<<"d1:"<<d1<<", d2:"<<d2;
        
        d3 = 0;
        for(u32 year = date1.year + 1; year < date2.year; ++year)
        {
            if(isLeap(year))
                d3 += 366;
            else
                d3 += 365;
        }
        return d1 + d2 + d3;
    }
}

bool BSRegistrationManager::isBetween2Time(DateTime date1,DateTime date2,DateTime date3) {
    //is date1 between date2 and date3
    
    return false;
}

void BSRegistrationManager::initData(const tbl::Table_Registration &table) {
    for (size_t i = 0, c = table.size(); i < c; ++i) {
        auto *registration = table.at(i);
        Reward reward;
        reward.rewardTheprops = registration->f_Theprops.value;
        reward.rewardNumber = registration->f_Number.value;
        rewardGroup rg;
        m_rewardGroups.push_back(rg);
        m_rewardGroups[registration->f_Id.value - 1].insert({ registration->f_StepId.value, reward });
        //m_rewards.insert({mission->f_Id.value, false});
    }
}

bool operator < (const Date & lhs, const Date &rhs) {
    if (lhs.year < rhs.year) {
        return true;
    } else if (lhs.year != rhs.year) {
        return false;
    } else {
        
        if (lhs.month < rhs.month) {
            return true;
        } else if (lhs.month != rhs.month) {
            return false;
        } else {
            if (lhs.day < rhs.day) {
                return true;
            } else {
                return false;
            }
        }
    }
}

bool operator < (const DateTime & lhs, const DateTime &rhs) {
//    if (lhs.date < rhs.date) {
//        return true;
//    } else if (lhs. != rhs.year) {
//        return false;
//    }
    return false;
}
