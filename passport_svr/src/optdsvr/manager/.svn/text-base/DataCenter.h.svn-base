//
//  DataCenter.h
//  optd_srv
//
//  Created by Wayne on 6/20/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__DataCenter__
#define __optd_srv__DataCenter__

#include "vnSingleton.h"
#include "../worker/LoggedUser.h"
#include "../worker/PreLoggingUser.h"
#include "../ServerData.h"
#include "../RedeemPacket.h"

using namespace vn;

struct DataCenterCreator;

class DataCenter : public Singleton<DataCenter, DataCenterCreator> {
public:

	 enum OrderStatus : int {
	        kOrderStatus_Invalid = 0,
	        kOrderStatus_Paying = 1,
	        kOrderStatus_Confirmed = 2,
	    };
    virtual bool init() = 0;
    virtual void fini() = 0;
    virtual void update(f32 deltaTime) = 0;
    virtual void saveServerData(const ServerData &data) = 0;
     virtual bool loadServerData(ServerData &data) = 0;


	virtual LoggedUserPtr createUser() = 0;
	virtual LoggedUserPtr loadUser(const UUID &uuid) = 0;
	virtual  void loadUser(const UUID &uuid,vn::RefCountedPtr<AsyncObject> asyncObject,std::function<void (std::shared_ptr<PreparedResultSet>)> callFun){}
	virtual void saveUser(LoggedUserPtr user) = 0;



	//订单
	 virtual vn::str8 generateOrder(const UUID &uuid, vn::u32 productId, vn::s32 price) = 0;
	    virtual OrderStatus queryOrder(const str8 &orderId, UUID &userId, u32 &productId) = 0;
	    virtual bool finishOrder(const str8 &orderId, const UUID &userId, bool confirmed) = 0;

	//记录日志
	virtual void log_user_signin(const UUID &uuid) = 0;
	virtual void log_user_login(const UUID &uuid) = 0;
	virtual void log_user_logout(const UUID &uuid) = 0;
    virtual void log_user_beginBattle(const UUID &uuid, vn::u32 levelId, bool intrusion) = 0;
    virtual void log_user_endBattle(const UUID &uuid, vn::u32 levelId, bool intrusion, u32 stars, bool watch, bool shovel) = 0;


	//RedeemPacket
	virtual bool addRedeemPacket(RedeemPacketPtr redeemPacket) = 0;
	    virtual RedeemPacketPtr queryRedeemPacket(u32 codeId) = 0;
	    virtual void removeRedeemPacket(u32 codeId, const UUID &uuid) = 0;
	    virtual str8 queryRedeemPacketCategoryName(u32 category) = 0;
	    virtual void setRedeemPacketCategory(u32 category, const str8 &name) = 0;
	    void _addUser(const LoggedUserPtr &user);
	    void _removeUser(const LoggedUserPtr &user);
protected:


};

struct DataCenterCreator {
    static DataCenter * create();
    static void destroy(DataCenter *p);
};

#endif /* defined(__optd_srv__DataCenter__) */
