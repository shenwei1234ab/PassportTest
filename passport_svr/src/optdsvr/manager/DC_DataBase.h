#ifndef __optd_srv__DC_DataBase__
#define __optd_srv__DC_DataBase__

#include "vnFileManager.h"
#include "DataCenter.h"
#include "../Log/DC_File_EventLog.h"
#include "vnReadWriteLock.h"
#include "vnSpinLock.h"
#include <map>
#include "../Log/DC_DataBase_EventLog.h"
class DC_DataBase : public DataCenter {
public:
	static DataCenter * create();
	static void destroy(DataCenter *p);

	virtual bool init()override;
	virtual void fini()override;
	virtual void update(f32 deltaTime)override;
	virtual void saveServerData(const ServerData &data)override;
	virtual bool loadServerData(ServerData &data)override;

	void AsyncCreateUser(AsyncObjectPtr obj,std::function<void (bool ret,LoggedUserPtr user)>doResult);

	virtual LoggedUserPtr createUser()override ;
	virtual LoggedUserPtr loadUser(const UUID &uuid)override ;

	virtual void saveUser(LoggedUserPtr user)override;
	//订单
	virtual vn::str8 generateOrder(const UUID &uuid, vn::u32 productId, vn::s32 price)override;
	virtual OrderStatus queryOrder(const vn::str8 &orderId, UUID &userId, vn::u32 &productId)override;
	virtual bool finishOrder(const vn::str8 &orderId, const UUID &userId, bool confirmed)override;

	//记录日志
	void AsyncLog(AsyncObjectPtr obj,UserEventLogPtr log,std::function<void(bool ret)> doResult);

	void _log(DC_File_EventLog *el);
	void _log(DC_File_EventLog_User *el, const UUID &uuid);
	void _flush_log();


	virtual void log_user_signin(const UUID &uuid)override;
	virtual void log_user_login(const UUID &uuid)override;
	virtual void log_user_logout(const UUID &uuid)override;
	virtual void log_user_beginBattle(const UUID &uuid, u32 levelId, bool intrusion);
	virtual void log_user_endBattle(const UUID &uuid, u32 levelId, bool intrusion, u32 stars, bool watch, bool shovel);



	virtual bool addRedeemPacket(RedeemPacketPtr redeemPacket)override;
	virtual RedeemPacketPtr queryRedeemPacket(u32 codeId)override;
	virtual void removeRedeemPacket(u32 codeId, const UUID &uuid)override;
	virtual str8 queryRedeemPacketCategoryName(u32 category)override;
	virtual void setRedeemPacketCategory(u32 category, const str8 &name)override;

	virtual ~DC_DataBase(){};






	//异步
	 //void AsyncLoadBaseInfo(const UUID &uuid,vn::RefCountedPtr<AsyncObject> asyncObject,std::function<void (PreparedQueryResult )> doResult);
	void AsyncLoadBaseInfo(std::string &stdUUid,vn::RefCountedPtr<AsyncObject> asyncObject,std::function<void (PreparedQueryResult )> doResult);
	 void  AsyncLoadForceGuide(LoggedUserPtr user,vn::RefCountedPtr<AsyncObject> asyncObject,std::function<void (PreparedQueryResult )> doResult);
	 void  AsyncLoadItemStore(LoggedUserPtr user,vn::RefCountedPtr<AsyncObject> asyncObject,std::function<void (PreparedQueryResult )> doResult);
	 void AsyncLoadLevels(LoggedUserPtr user,vn::RefCountedPtr<AsyncObject> asyncObject,std::function<void (PreparedQueryResult )> doResult);
	 void AsyncLoadMines(LoggedUserPtr user,vn::RefCountedPtr<AsyncObject> asyncObject,std::function<void (PreparedQueryResult )> doResult);
	 void AsyncLoadMissions(LoggedUserPtr user,vn::RefCountedPtr<AsyncObject> asyncObject,std::function<void (PreparedQueryResult )> doResult);
	 void AsyncLoadMonsterMap(LoggedUserPtr user,vn::RefCountedPtr<AsyncObject> asyncObject,std::function<void (PreparedQueryResult )> doResult);
	 void AsyncLoadPlatformStore(LoggedUserPtr user,vn::RefCountedPtr<AsyncObject> asyncObject,std::function<void (PreparedQueryResult )> doResult);
	 void AsyncLoadTechUpgrade(LoggedUserPtr user,vn::RefCountedPtr<AsyncObject> asyncObject,std::function<void (PreparedQueryResult )> doResult);
	 void AsyncLoadTowerUpgrade(LoggedUserPtr user,vn::RefCountedPtr<AsyncObject> asyncObject,std::function<void (PreparedQueryResult )> doResult);
	 void  AsyncLoadLevelGroup(LoggedUserPtr user,vn::RefCountedPtr<AsyncObject> asyncObject,std::function<void (PreparedQueryResult )> doResult);
	 void  AsyncLoadLevelGroupLevels(vn::u32 levelId,vn::RefCountedPtr<AsyncObject> asyncObject,std::function<void (PreparedQueryResult )> doResult);
	PreparedQueryResult  LoadLevelGroupLevels(vn::u32 groupId);

	//baseinfo表
	//void AsyncUpdateBaseInfo(AsyncObjectPtr asyncObject,LoggedUserPtr pUser,std::function<void(bool)> doResulet);
	void AsyncUpdateBaseInfo_Baseinfo(AsyncObjectPtr asyncObject,std::string &strUUid,s32 clip0,s32 clip1,s32 clip2,s32 money,s32 diamond,u32 purchaseDiamond,std::function<void(bool)> doResulet);
	void AsyncUpdateBaseInfo_Hero(AsyncObjectPtr asyncObject,std::string &strUUid,s32 shovel0,s32 shovel1,s32 skill0,s32 skill1,s32 watch0,s32 watch1,std::function<void(bool)> doResulet);
	void AsyncUpdateBaseInfo_Registion(AsyncObjectPtr asyncObject,std::string &strUUid,std::string &strfirstDate,std::string &strlastDate,s32 count,s32 group,std::function<void(bool)> doResulet);
	void AsyncUpdateBaseInfo_Lottery(AsyncObjectPtr asyPtr,std::string &strUUid,u32 lotteryDraw,s32 lotteryState,f32 lotteryTimer,std::string &lotteryPool,vn::u64 timestamp,std::function<void(bool)> doResulet);
	void AsyncUpdateBaseInfo_Coupon(AsyncObjectPtr asyPtr,std::string &strUUid,s32 couponId,f32 couponTimer,vn::u64 timeStamp,std::function<void(bool)> doResulet);

	void AsyncUpdateBaseInfoAll(AsyncObjectPtr asyncObject,LoggedUserPtr pUser,std::function<void(bool)> doResulet);
	void AsyncUpdateIntrusion(std::string &strUUId,bool running,f32 timer,vn::u64 timestamp,AsyncObjectPtr asyncObject,std::function<void(bool)> doResulet);
	//void AsyncUpdateCoupon(AsyncObjectPtr asyPtr,LoggedUserPtr pUser,std::function<void(bool)> doResulet);
	//void AsyncUpdateLottery(AsyncObjectPtr asyPtr,LoggedUserPtr pUser,std::function<void(bool)> doResulet);
	void AsyncUpdateTimeStamp(AsyncObjectPtr asyncObject,std::string &strUUId,u32 stamp,std::function<void(bool)> doResulet);
	void AsyncUpdateLoginInfo(AsyncObjectPtr asyPtr,LoggedUserPtr pUser,std::function<void(bool)> doResulet);
	void AsyncUpdateMission(AsyncObjectPtr asyObj,std::string &strUUId,u32 missionId,bool missionStatus,std::function<void(bool)> doResulet);
	void AsyncInsertForceGuide(LoggedUserPtr pUser,u32 guideid,std::function<void(bool)> doResulet);
	void AsyncUpdateMonsterMap(LoggedUserPtr pUser,u32 monsterid,bool monsterStatus,std::function<void(bool)> doResulet);

	void AsyncUpdateItemStore(LoggedUserPtr pUser,u32 itemId,u32 itemNum,std::function<void(bool)> doResulet);
	void AsyncInsertLevel(AsyncObjectPtr obj,std::string &strUUId,u32 levelId,bool intrusion,u32 intrusiontimes,u32 starNum,s32 score,bool shovel,bool watch,std::function<void(bool)> doResulet);
	//关卡组
	void AsyncInsertLevelGroups(LoggedUserPtr pUser,u32 groupId,bool pressed,u32 intrusionCount,u32 rewardsCount,std::function<void(bool)> doResulet);
	void AsyncUpdateLevelGroups(AsyncObjectPtr asyncObjPtr,LoggedUserPtr user,u32 groupId,bool pressed,u32 intrusionCount,u32 rewardsCount,std::function<void(bool)> doResulet);
	bool SyncUpdateLevelGroups(std::string &strUUId,u32 groupId,bool pressed,u32 intrusionCount,u32 rewardsCount);
	void AsyncInsertLevelGroup_Levels(LoggedUserPtr pUser,u32 groupId,u32 levelId,std::string heros,std::string platforms, bool resolved,bool shovel ,u32 stars,u8 passed,bool watch,std::function<void(bool ret)> doResulet);
	void AsyncUpdateLevelGroup_Levels(AsyncObjectPtr asyncObjPtr,LoggedUserPtr user,u32 groupId,u32 levelId,std::string heros,std::string platforms, bool resolved,bool shovel ,u32 stars,u8 passed,bool watch,std::function<void(bool ret)> doResulet);
	bool SyncUpdateLevelGroup_Levels(std::string &strUUId,u32 groupId,u32 levelId,std::string &heros,std::string &platforms, bool resolved,bool shovel ,u32 stars,u8 passed,bool watch);

	void AsyncUpdateLevel(AsyncObjectPtr asyncObjPtr,std::string &strUUId,u32 levelId,bool intrusion,u32 intrusiontimes,u32 starNum,s32 score,bool shovel,bool watch,std::function<void(bool)> doResulet);
	bool SyncUpdateLevel(std::string &strUUId,u32 levelId,bool intrusion,u32 intrusiontimes,u32 starNum,s32 score,bool shovel,bool watch);

	void AsyncUpdateTech(LoggedUserPtr pUser,u32 techIndex,u32 techStatus,std::function<void(bool)> doResulet);
	void AsyncUpdateTower(LoggedUserPtr pUser,u32 classIndex,u32 inclassIndex,u32 upgradeValue,std::function<void(bool)> doResulet);
	//英雄技能
	bool UpdateRunesStatus(u32 runId,s32 runstatus,std::string &heroId);
	void AsyncUpdateRunesStatus(u32 runId,s32 runstatus,LoggedUserPtr pUser,std::string &heroId,std::function<void(bool)>doResult);
	PreparedQueryResult LoadHeroIdbyUUIdTowerClass(std::string &strUUId,u32 towerClass);
	void  AsyncLoadHeroIdbyUUIdTowerClass(LoggedUserPtr user,u32 towerClass,std::function<void(PreparedQueryResult)> doResulet);

	//金矿
	void AsyncUpdateGoldMine(std::string &strUUId,u32 id, u32 num,f32 timer,vn::u64 timeStamp,vn::RefCountedPtr<AsyncObject> asyncObjectPtr,std::function<void(bool)>doResult);
	bool UpdateItemStore(std::string &strUUId,u32 itemId,u32 itemNum);
	bool InsertItemStore(std::string &strUUId,u32 itemId,u32 itemNum);
	bool UpdateMoney(std::string &strUUId,s32 money);
	bool UpdateDiamond(std::string &strUUId,s32 diamond);
	bool UpdateClip(std::string &strUUId,s32 clip0,s32 clip1,s32 clip2);
	bool LoadHero(LoggedUserPtr user);
	//解锁英雄
	bool UpdateHeroStatus(u32 status,LoggedUserPtr user,u32 id);
	void AsyncupgradeHero(AsyncObjectPtr ptr,std::string &strUUId,u32 id,s32 status,u32 towerclass,std::function<void(bool)> doResulet);


	//高台
	void AsyncUpdatePlatform(AsyncObjectPtr asyObj,std::string &strUUId,u32 category,u32 level,std::function<void(bool)> doResulet);








	//Log
	void AsyncInsertLog(AsyncObjectPtr asyObj,std::string &strUUId,u32 eventType,std::string &strMessage,std::function<void(bool)> doResulet);

protected:
		    vn::f32 m_timer = 0;
		    struct OrderInfo {
		        UUID userId;
		        u32 productId;
		        s32 price;
		        OrderStatus status;
		    };


      bool m_dirty = false;
      vn::FileSystemPtr m_fs, m_fs_log;
      std::map<str8, UUID> m_accounts;
      vn::ReadWriteLock m_accountsLock;
      std::map<str8, OrderInfo> m_orders;
      vn::ReadWriteLock m_ordersLock;

      ServerData m_serverData;
      void _flush();
      //日志
      std::deque<std::unique_ptr<DC_File_EventLog>> m_logs;
      SpinLock m_logs_lock;

      std::map<u32, str8> m_redeemPacketCategories;
         std::map<u32, RedeemPacketPtr> m_redeemPackets;
         ReadWriteLock m_redeemPacketCategoriesLock, m_redeemPacketPacketsLock;
      void _init_redeem_packets();
      void _flush_redeem_packets();
private:
      bool _Insertbaseinfo(std::string &strUUid);
       void  _AsyncInsertbaseinfo(std::string &strUUid,AsyncObjectPtr obj,std::function<void(bool ret)>doResult);
       void _HandlAsyncCreateUser(bool ret,AsyncTaskPtr createUserTask, LoggedUserPtr user,std::function<void (bool ret,  LoggedUserPtr user)>doResult);

       //查询数据库并载入内存
       bool _InsertForceGuide(LoggedUserPtr user);
       bool _LoadForceGuide(LoggedUserPtr user);


       bool _InsertHero(LoggedUserPtr user);
       bool _LoadHero(std::string &strUUId,BSHero &hero);
       void Async_InsertHero(AsyncObjectPtr obj,LoggedUserPtr user,std::function<void(bool ret)>doResult);

       bool _InsertItemStore(LoggedUserPtr user);
       void Async_InsertItemStore(AsyncObjectPtr obj,LoggedUserPtr user,std::function<void(bool ret)>doResult);
       bool _LoadItemStore(std::string &strUUId,BSItemStore &itemStore);


       bool _InsertLevels(LoggedUserPtr user);
       bool _LoadLevels(LoggedUserPtr user);
       void Async_InsertLevels(AsyncObjectPtr obj,LoggedUserPtr user,std::function<void(bool ret)>doResult);

       bool _InsertMines(LoggedUserPtr user);
       bool _LoadMines(std::string &strUUId,BSGoldMine &goldMine);
       void Async_InsertMines(AsyncObjectPtr obj,LoggedUserPtr user,std::function<void(bool ret)>doResult);

       bool _InsertMissions(LoggedUserPtr user);
       bool _LoadMissions(std::string &strUUId,BSMission &bsMission);
       void Async_InsertMissions(AsyncObjectPtr obj,LoggedUserPtr user,std::function<void(bool ret)>doResult);

       bool _InsertMonsterMap(LoggedUserPtr user);
       bool _LoadMonsterMap(std::string &strUUId,BSMonsterMap &monsterMap);
       void Async_InsertMonsterMap(AsyncObjectPtr obj,LoggedUserPtr user,std::function<void(bool ret)>doResult);

		 bool _InsertPlatformStore(LoggedUserPtr user);
		 bool _LoadPlatformStore(std::string &strUUId ,BSPlatformStore &platStore);
		 void Async_InsertPlatformStore(AsyncObjectPtr obj,LoggedUserPtr user,std::function<void(bool ret)>doResult);

		 bool _InsertTechUpgrade(LoggedUserPtr user);
		 bool _LoadTechUpgrade(std::string &strUUId,BSTechUpgrade &bsTech);
		 void  Async_InsertTechUpgrade(AsyncObjectPtr obj,LoggedUserPtr user,std::function<void(bool ret)>doResult);

		 bool _InsertTowerUpgrade(LoggedUserPtr user);
		 //读取database数据并且载入内存
		 bool _LoadTowerUpgrade(std::string &strUUId,BSTowerUpgrade &bstowergrade);
		 void Async_InsertTowerUpgrade(AsyncObjectPtr obj,LoggedUserPtr user,std::function<void(bool ret)>doResult);
};
#endif /* defined(__optd_srv__DC_File__) */
