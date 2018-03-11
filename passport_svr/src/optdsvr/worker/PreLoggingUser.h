//
//  PreLoggingUser.h
//  optd_srv
//
//  Created by Wayne on 6/19/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__PreLoggingUser__
#define __optd_srv__PreLoggingUser__



#include "../clientlisten/Client.h"
#include "PacketHandler.h"
#include "vnPacketFactory.h"
#include "LoggedUser.h"
#include "AsyncTask.h"
class PreLoggingUser : public PacketHandler {
    VN_DECL_PACKET_HANDLER()
public:
   // int _id;
    //virtual void async(std::function<void()> fn)override;
    PreLoggingUser(Client *client) : m_client(client, true) {strTag = "PreLoggingUser";}
    ~PreLoggingUser();
    virtual vn::PacketPtr createRequest(vn::u32 pid)override;
    virtual void update(vn::f32 deltaTime)override;
    void DbOperFailHandl();

    void AsyncInsertbaseinfo(LoggedUserPtr user,std::function<void(bool ret)>doResult);
    void AsyncInsertHero(LoggedUserPtr user,std::function<void(bool ret)>doResult);
    void AsyncInsertItemStore(LoggedUserPtr user,std::function<void(bool ret)>doResult);
    void AsyncInsertLevels(LoggedUserPtr user,std::function<void(bool ret)>doResult);
    void AsyncInsertMines(LoggedUserPtr user,std::function<void(bool ret)>doResult);
    void AsyncInsertMissions(LoggedUserPtr user,std::function<void(bool ret)>doResult);
    void AsyncInsertMonsterMap(LoggedUserPtr user,std::function<void(bool ret)>doResult);
    void AsyncInsertPlatformStore(LoggedUserPtr user,std::function<void(bool ret)>doResult);
    void AsyncInsertTechUpgrade(LoggedUserPtr user,std::function<void(bool ret)>doResult);
    void AsyncInsertTowerUpgrade(LoggedUserPtr user,std::function<void(bool ret)>doResult);
    void AsyncTest();
    void _HandlAsyncCreateUser(bool ret,LoggedUserPtr user,AsyncTaskPtr createUserTask);
protected:

    enum State {
        kInitial = 0,
        kNormal,
        kClosed
    } m_state = kInitial;
    
    vn::f32 m_timer = 0;
    ClientPtr m_client;
    
    virtual void _closed();
    void _updateInitial(vn::f32 deltaTime);

private:
    void _HandlGetBalance(vn::s32 result,std::string& context,vn::RefCountedPtr<pkt::req_login_PPID> 	pktPtr);
    void _HandlTaskFinish(AsyncTaskPtr task,LoggedUserPtr user);
    void _AsyncCalculateIntrusion(LoggedUserPtr user,std::function<void(bool ret)> doResult);
    void _AsyncCalculateMines(LoggedUserPtr user,std::function<void(bool ret)> doResult);
    void _HandlAsyncLoadItemStore(PreparedQueryResult result,LoggedUserPtr user,AsyncTaskPtr task);
    void _HandlAsyncLoadForceGuide(PreparedQueryResult result,LoggedUserPtr user,AsyncTaskPtr task);
    void _HandlAsyncLoadLevels(PreparedQueryResult result,LoggedUserPtr user,AsyncTaskPtr task);
    void _HandlAsyncLoadMines(PreparedQueryResult result,LoggedUserPtr user,AsyncTaskPtr task);
    void _HandlAsyncLoadMissions(PreparedQueryResult result,LoggedUserPtr user,AsyncTaskPtr task);
    void _HandlAsyncLoadMonsterMap(PreparedQueryResult result,LoggedUserPtr user,AsyncTaskPtr task);
    void _HandlAsyncLoadPlatformStore(PreparedQueryResult result,LoggedUserPtr user,AsyncTaskPtr task);
    void _HandlAsyncLoadTechUpgrade(PreparedQueryResult result,LoggedUserPtr user,AsyncTaskPtr task);
    void _HandlAsyncLoadTowerUpgrade(PreparedQueryResult result,LoggedUserPtr user,AsyncTaskPtr task);
    void _HandlAsyncLoadLevelGroup(PreparedQueryResult result,LoggedUserPtr user,AsyncTaskPtr task);
    void	_HandlmineTask(AsyncTaskPtr mineTask,std::function<void(bool ret)> doResult);
    void	_HandlSignIn(const std::string &strUUId,vn::s32 diamond=0);
};

#endif /* defined(__optd_srv__PreLoggingUser__) */
