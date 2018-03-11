//
//  BSIntrusion.h
//  optd_srv
//
//  Created by Cloverz on 9/12/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__BSIntrusion__
#define __optd_srv__BSIntrusion__

#include <time.h>
#include "vnDataStream.h"
#include "vnVariable.h"
#include "../database/PassPortDatabaseConnection.h"
using namespace vn;

class LoggedUser;
class BSIntrusion
{
public:
    
    BSIntrusion(LoggedUser *);
    void init();
    void load(u32 version, DataStream &stream);
    void load(PreparedQueryResult baseinfo);
    void save(DataStream &stream) const;
    void update(f32 deltaTime);
    void Asyncupdate(f32 deltaTime,std::function<void(bool ret)> doResult);
    void intrusion();
    void _intrusion();
    bool Sync_intrusion();
    void Async_intrusion(AsyncObjectPtr asyncObjPtr,std::function<void(bool ret)> doResult);
    void Asyncsave(AsyncObjectPtr asyncObjPtr,std::function<void(bool)> doResulet);
    void save(Variable_object &vobj) const;
    void load(Variable_object &vobj);
     bool Getrunning()
     {
    	 return m_running;
     }
     f32 GetTimer()
     {
    	 return m_timer;
     }
     vn::u64 m_lastIntrusionTimeStamp;
private:
    


    LoggedUser *m_user;
    bool m_running;
    f32 m_timer;


};

#endif /* defined(__optd_srv__BSIntrusion__) */
