/*
 * WorldManager.h
 *
 *  Created on: 2015年6月13日
 *      Author: leave
 */

#ifndef SRC_OPTDSVR_WORLDMANAGER_H_
#define SRC_OPTDSVR_WORLDMANAGER_H_
#include "vnReadWriteLock.h"
#include "vnSingleton.h"
#include <map>
#include "common/Util.h"
#include <mutex>
#include "ServerData.h"
#include "../optdsvr/Protocols.h"
#include "vnLink.h"
#include "vnFilePath.h"



class WorldManager: public vn::Singleton<WorldManager> {
public:
	//更新通告（通过console）
	void updateAnnouncement(vn::str8 content);
	//发布通告(玩家登录的时候)
	void announcement(vn::u32 &id, vn::str8 *content);


	void SetLinkPool(vn::LinkPool *plinkPool) {
			m_linkPool = plinkPool;
		}

	vn::LinkPool *m_linkPool = nullptr;



	bool loadBaseConfig();
	bool loadVersionData();
	bool loadTBL();


	void update(vn::f32 deltaTime);
	bool m_console_mode = false;



	//属性
	vn::u32 intrusion_beginLevel()
	{
	    return m_intrusion_beginLevel;
	}
	vn::u32 intrusion_interval()
	{
	    return m_intrusion_interval;
	}
	vn::f32 coupon_interval() { return m_coupon_interval; }
	vn::u32 lottery_beginLevel() { return m_lottery_beginLevel; }
	vn::f32 lottery_validTime() { return m_lottery_validTime; }
	vn::u32 lottery_drawTimes() { return m_lottery_drawTimes; }
	vn::s32 lottery_drawCost() { return m_lottery_drawCost; }
	bool buy_nocheck() { return m_buy_nocheck; }
	vn::f32 GetCouponSaveTime()
	{
		return m_coupon_saveTime;
	}

	vn::f32 GetLotterySavaTime()
	{
		return m_lottery_saveTime;
	}

	vn::f32	GetMinesSaveTime()
	{
		return m_mines_saveTime;
	}
//optd
public:
	vn::u16 m_listening_port = 33552;
	size_t m_num_pool_threads = 2;
	vn::u32 m_version = 0;
	vn::u32 m_min_version = 0;
	vn::u32 m_intrusion_beginLevel = 100;
	vn::u32 m_intrusion_interval = 300;						//LoggedUser的BSInstrusion：:update
	vn::f32 m_coupon_interval = 30.f; // seconds
	//保存时间间隔
	vn::f32 m_coupon_saveTime = 300.f;
	vn::f32 m_lottery_saveTime = 300.f;
	vn::f32 m_mines_saveTime = 300.f;


	vn::u32 m_lottery_beginLevel = 150;
	vn::f32 m_lottery_validTime = 1800; // seconds
	vn::u32 m_lottery_drawTimes = 5;
	vn::u32 m_lottery_drawCost = 30;

	bool m_buy_nocheck = false;

	FilePath m_version_file = {0, "logic.pkg"};
	vn::u32 m_version_file_size = 0;
	 std::vector<std::vector<vn::u8>> m_version_file_data;
	//DB
	 vn::str8  m_sDBInfo;
	 vn::s32 m_dbThreads = 1;



	 ServerData m_data;
	 vn::ReadWriteLock m_announcementLock;
};

#define sWorldManager WorldManager::instance()

#endif /* SRC_PASSPORTSVR_WORLDMANAGER_H_ */
