//
//  DC_File_EventLog.h
//  optd_srv
//
//  Created by shenwei on 4/11/16.
//  Copyright (c) 2015 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__DC_DataBase_EventLog__
#define __optd_srv__DC_DataBase_EventLog__

#include "../DateTime.h"
#include "../../passportsvr/entity/AsyncObject.h"

using namespace vn;

enum DC_DataBase_EventLog_Type : u32 {
    kTYPE_UserSignin = 1,
    kTYPE_UserLogin,
    kTYPE_UserLogout,
    kTYPE_UserBeginBattle,
    kTYPE_UserEndBattle,
};

struct DC_DataBase_EventLog: RefCounted {
    
    virtual void Asyncsave(AsyncObjectPtr obj,std::function<void(bool ret)>)=0;

    virtual DC_DataBase_EventLog_Type type() const = 0;

};

struct DC_DataBase_EventLog_User:DC_DataBase_EventLog {
    //std::pair<u64, u64> userId = {0, 0};
    std::string strUUId="";
    std::string strMessage="";
    virtual void Asyncsave(AsyncObjectPtr obj,std::function<void(bool ret)>doResult)override;

};

#define DATABASE_DECL_EVENT_LOG(t) \
	static DC_DataBase_EventLog_Type _type() { return t; }\
    virtual DC_DataBase_EventLog_Type type() const { return _type(); }


struct DC_DataBase_EventLog_User_Signin : DC_DataBase_EventLog_User {
	DATABASE_DECL_EVENT_LOG(kTYPE_UserSignin)
	virtual void Asyncsave(AsyncObjectPtr obj,std::function<void(bool ret)>doResult)override;
};

struct DC_DataBase_EventLog_User_Login : DC_DataBase_EventLog_User {
	DATABASE_DECL_EVENT_LOG(kTYPE_UserLogin)
	 virtual void Asyncsave(AsyncObjectPtr obj,std::function<void(bool ret)>)override;

};

struct DC_DataBase_EventLog_User_Logout : DC_DataBase_EventLog_User {
	DATABASE_DECL_EVENT_LOG(kTYPE_UserLogout)
	 virtual void Asyncsave(AsyncObjectPtr obj,std::function<void(bool ret)>)override;
};

struct DC_DataBase_EventLog_User_BeginBattle : DC_DataBase_EventLog_User {
	DATABASE_DECL_EVENT_LOG(kTYPE_UserBeginBattle)
    
    u32 levelId = 0;
    bool intrusion = false;
    virtual void Asyncsave(AsyncObjectPtr obj,std::function<void(bool ret)>)override;

};


struct DC_DataBase_EventLog_User_EndBattle : DC_DataBase_EventLog_User {
	DATABASE_DECL_EVENT_LOG(kTYPE_UserEndBattle)
    
    u32 levelId = 0;
    bool intrusion = false;
    u8 stars = 0;
    bool shovel = false, watch = false;
    
    virtual void Asyncsave(AsyncObjectPtr obj,std::function<void(bool ret)>)override;

};

typedef vn::RefCountedPtr<DC_DataBase_EventLog_User>  UserEventLogPtr;


#endif
