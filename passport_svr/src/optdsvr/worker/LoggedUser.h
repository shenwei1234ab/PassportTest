#ifndef __optd_srv__LoggedUser__
#define __optd_srv__LoggedUser__


#include "../clientlisten/Client.h"
#include "BSPlayerBaseInfo.h"
#include "PacketHandler.h"
#include "BSTechUpgrade.h"
#include "BSGoldMine.h"
#include "BSTowerUpgrade.h"
#include "BSLevelGroupInfo.h"
#include "BSLevel.h"
#include "BSHero.h"
#include "BSIntrusion.h"
#include "BSItemStore.h"
#include "BSMission.h"
#include "BSRegistration.h"
#include "BSMonsterMap.h"
#include "../manager/BSCouponManager.h"
#include "../manager/BSAwardManager.h"
#include "BSLevelGroup.h"
#include "BSForceGuide.h"
#include "BSPlatformStore.h"
#include <set>
#include <vector>
#include "../database/Dbtable.h"
#include "UUID.h"
#include "AsyncTask.h"
using namespace vn;



class LoggedUser : public PacketHandler {
	VN_DECL_PACKET_HANDLER()
public:
	 //virtual void async(std::function<void()> fn)override;
	LoggedUser(const std::string  &uuid);
	LoggedUser(const UUID &uuid);
	virtual ~LoggedUser();
	virtual void update(f32 deltaTime)override;
	  virtual vn::PacketPtr createRequest(vn::u32 pid);
	virtual void shutdown()override;
	void OtherLogin(const ClientPtr &client);
	//登录
	void signin(const ClientPtr &client);
	//注册
	void login(const ClientPtr &client);

	void send_intrusion(u32 levelId, bool group);
	 void _notify_couponUsed();
	 void _notify_heroUnlocked(u32 heroClassId);
	 void _notify_levelPassed(const BSLevel::LevelInfo &info);
	 void reset_data(vn::s32 diamond = 0);

	 //处理订单
	 void order_ok(const vn::str8 &cooOrderSerial,s32 *addDiamond);
	 void Asyncorder_ok(const str8 &cooOrderSerial,std::function<void(bool ret)>doResult);
	 void order_failed(const vn::str8 &cooOrderSerial);

	//属性
	void load(Variable_object &vobj);
	void save(Variable_object &vobj)const;
	void load(PreparedQueryResult result,vn::s32 balance=0);

	BSPlatformStore & bsPlatformStore();
	BSMonsterMap & bsMonsterMap();
	BSPlayerBaseInfo & bsPlayerBaseInfo();
	BSTechUpgrade & bsTechUpgrade();
	BSTowerUpgrade & bsTowerUpgrade();
	BSLevel & bsLevel();
	BSGoldMine & bsGoldMine();
	BSHero & bsHero();
	BSLevelGroupManager & bsLevelGroupMgr();
	BSMission & bsMission();
	BSRegistration & bsRegistration();
	BSItemStore & bsItemStore();
	BSForceGuide &bsForceGuide();
	const BSCoupon * bsCoupon() { return m_coupon; }
	BSIntrusion & bsIntrusion() { return m_bsIntrusion; }
	//const UUID & uuid() const { return m_uuid; }
	 std::string GetUUId()const
	{
		//return m_uuid.to_string();
		 return m_uuid;
	}

	void _markDirty()
	{
	    m_dirty = true;
	}


	//问题
	bool restart()
	{
	    // TODO:
	    return true;
	}


	//dbupdate
	//void AsyncUpdateInstrusion(bool running,f32 timer);
	void DbOperFailHandl();
	void _invalid_operation();




	vn::f32 GetCouponTimer()
	{
		return m_couponTimer;
	}

	vn::u32 GetCouponId()
	{
		if(!m_coupon)
		{
			return 0;
		}
		return m_coupon->couponId;
	}

	const BSCoupon * GetCoupon()
	{
		return m_coupon;
	}

	vn::u32 GetPurchaseDiamond()
	{
		return m_purchasedDiamonds;
	}
	vn::u32 GetStamp()
	{
		return m_stamp;
	}
	vn::f32 GetLotteyTimer()
	{
		return m_lotteryTimer;
	}
	vn::u32 GetLotteryDrawTimer()
	{
		return m_lotteryDrawTimes;
	}
	s32 GetLotteryState()
	{
		return m_lotteryState;
	}

	void SetCouponTimer(vn::f32 couponTimer)
	{
		m_couponTimer = couponTimer;
	}

	void SetCoupon(BSCoupon *coupon)
	{
		m_coupon = coupon;
	}


	void CalculateCoupon(vn::u64 lastTime);
	bool CalculateSecondDayLogin(vn::u64 lastLoginTime);
	void CalculateLottery(vn::u64 lastCouponTime,vn::u64 lastLoginTime);

	void GetLotteryPool(std::string &strLottery);

	vn::u64 m_lastCouponTimeStamp=0;
	vn::u64 m_lastLotteryTimeStamp=0;
	vn::u64 m_lastLoginTimeStamp=0;
	 void _updateStamp();

protected:
	//UUID m_uuid;
	 std::string m_uuid;
	ClientPtr m_client;
	 bool m_dirty = false;
	 //网络线程close 后 post到work线程调用
	virtual  void _closed()override;

private:
	vn::f32 m_timer = 0;
	vn::u32 m_stamp;
	vn::u32 m_purchasedDiamonds = 0;
	const BSCoupon *m_coupon = 0;     //优惠券
	vn::f32 m_couponTimer = 0;
	enum LotteryState : s32
	{
	        kLottery_Locked = 0,
	        kLottery_Unlocked = 1,
	        kLottery_Opened = 2,
	        kLottery_Closed = 3
	};
	LotteryState m_lotteryState = kLottery_Locked;
	f32 m_lotteryTimer = 0;
	u32 m_lotteryDrawTimes = 5;		//抽奖次数
	 std::vector<std::pair<const BSAward *, u32>> m_lotteryPool;
	 void _ConvertStrToLotteryPool(std::string &strPoo);
	 std::set<u32> m_redeemPacketCategories;

	 bool m_battleIntrusion = false;
	 bool m_battle = false;
	 vn::u32 m_battleId = 0;

	BSPlayerBaseInfo m_bsPlayerBaseInfo;
	BSTechUpgrade m_bsTechUpgrade;
	BSTowerUpgrade m_bsTowerUpgrade;
	BSLevel m_bsLevel;
	BSGoldMine m_bsGoldMine;
	BSHero m_bsHero;
	BSIntrusion m_bsIntrusion;
	BSItemStore m_bsItemStore;
	BSMonsterMap m_bsMonsterMap;
	BSForceGuide m_bsForceGuide;
	BSPlatformStore m_bsPlatformStore;
	BSLevelGroupManager m_bsLevelGroupMgr;
	BSMission m_bsMission;
	BSRegistration m_bsRegistration;

	bool  _generateCoupon();//优惠券
	void	_HandlTaskFinish(AsyncTaskPtr task);
	void	_HandlClose(AsyncTaskPtr task);


	 void _checkLottery(vn::u32 levelId);
	 //void Async_checkLottery(u32 levelId,std::function<void (bool ret)> doResult);
	 void _updateLottery(vn::f32 deltaTime);
	 void  _sendLotteryInfo();
	bool  _openLottery();  //彩票
	void _buildPlayerData(pkt::Msg_PlayerData &data);
	void _buildTimedData(pkt::Msg_TimedData &data);
	void _sendAnnouncementId();	//下发通告
	void _sendCouponInfo();

	void _sendRegistrationInfo();

	 void _updateCoupon(vn::f32 deltaTime);



};
typedef vn::RefCountedPtr<LoggedUser> LoggedUserPtr;









#endif
