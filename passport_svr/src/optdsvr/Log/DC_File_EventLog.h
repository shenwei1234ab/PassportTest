//
//  DC_File_EventLog.h
//  optd_srv
//
//  Created by Wayne on 1/29/15.
//  Copyright (c) 2015 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__DC_File_EventLog__
#define __optd_srv__DC_File_EventLog__

#include "../DateTime.h"
#include "vnDataStream.h"

#include <map>

using namespace vn;

enum DC_File_EventLog_Type : u32 {
    kELT_UserSignin = 1,
    kELT_UserLogin,
    kELT_UserLogout,
    kELT_UserBeginBattle,
    kELT_UserEndBattle,
};

struct DC_File_EventLog : AllocatedObject {
    virtual ~DC_File_EventLog() = default;
    DateTime date;
    
    virtual void save(DataStream &stream) const;
    virtual void load(DataStream &stream);
    
    virtual DC_File_EventLog_Type type() const = 0;
    
    static std::map<DC_File_EventLog_Type, DC_File_EventLog *(*)()> s_registry;
};

struct DC_File_EventLog_User : DC_File_EventLog {
    std::pair<u64, u64> userId = {0, 0};
    
    virtual void save(DataStream &stream) const;
    virtual void load(DataStream &stream);
};

#define DECL_EVENT_LOG(t) \
    static DC_File_EventLog_Type _type() { return t; }\
    virtual DC_File_EventLog_Type type() const { return _type(); }\
    static DC_File_EventLog * create();

struct DC_File_EventLog_User_Signin : DC_File_EventLog_User {
    DECL_EVENT_LOG(kELT_UserSignin)
};

struct DC_File_EventLog_User_Login : DC_File_EventLog_User {
    DECL_EVENT_LOG(kELT_UserLogin)
};

struct DC_File_EventLog_User_Logout : DC_File_EventLog_User {
    DECL_EVENT_LOG(kELT_UserLogout)
};

struct DC_File_EventLog_User_BeginBattle : DC_File_EventLog_User {
    DECL_EVENT_LOG(kELT_UserBeginBattle)
    
    u32 levelId = 0;
    bool intrusion = false;
    
    virtual void save(DataStream &stream) const;
    virtual void load(DataStream &stream);
};


struct DC_File_EventLog_User_EndBattle : DC_File_EventLog_User {
    DECL_EVENT_LOG(kELT_UserEndBattle)
    
    u32 levelId = 0;
    bool intrusion = false;
    u8 stars = 0;
    bool shovel = false, watch = false;
    
    virtual void save(DataStream &stream) const;
    virtual void load(DataStream &stream);
};

#endif /* defined(__optd_srv__DC_File_EventLog__) */
