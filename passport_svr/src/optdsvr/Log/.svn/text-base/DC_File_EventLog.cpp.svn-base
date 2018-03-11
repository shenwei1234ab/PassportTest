//
//  DC_File_EventLog.cpp
//  optd_srv
//
//  Created by Wayne on 1/29/15.
//  Copyright (c) 2015 longsgoo.com. All rights reserved.
//

#include "DC_File_EventLog.h"

std::map<DC_File_EventLog_Type, DC_File_EventLog *(*)()> DC_File_EventLog::s_registry;

void DC_File_EventLog::save(DataStream &stream) const {
    stream << date.date << date.time;
}

void DC_File_EventLog::load(DataStream &stream) {
    stream >> date.date >> date.time;
}

void DC_File_EventLog_User::save(DataStream &stream) const {
    DC_File_EventLog::save(stream);
    stream << userId.first << userId.second;
}

void DC_File_EventLog_User::load(DataStream &stream) {
    DC_File_EventLog::load(stream);
    stream >> userId.first >> userId.second;
}

#define IMPL_EVENT_LOG(cls) \
    DC_File_EventLog * cls::create() { return vnnew cls; }\
    struct __auto_reg_##cls {\
        __auto_reg_##cls() { DC_File_EventLog::s_registry[cls::_type()] = cls::create; }\
    } __auto_reg_##cls##_ins;

IMPL_EVENT_LOG(DC_File_EventLog_User_Signin)
IMPL_EVENT_LOG(DC_File_EventLog_User_Login)
IMPL_EVENT_LOG(DC_File_EventLog_User_Logout)
IMPL_EVENT_LOG(DC_File_EventLog_User_BeginBattle)
IMPL_EVENT_LOG(DC_File_EventLog_User_EndBattle)


void DC_File_EventLog_User_BeginBattle::save(DataStream &stream) const {
    DC_File_EventLog_User::save(stream);
    stream << DataStream::ccu(levelId) << intrusion;
}

void DC_File_EventLog_User_BeginBattle::load(DataStream &stream) {
    DC_File_EventLog_User::load(stream);
    stream >> DataStream::cu(levelId) >> intrusion;
}

void DC_File_EventLog_User_EndBattle::save(DataStream &stream) const {
    DC_File_EventLog_User::save(stream);
    stream << DataStream::ccu(levelId) << intrusion << stars << shovel << watch;
}

void DC_File_EventLog_User_EndBattle::load(DataStream &stream) {
    DC_File_EventLog_User::load(stream);
    stream >> DataStream::cu(levelId) >> intrusion >> stars >> shovel >> watch;
}