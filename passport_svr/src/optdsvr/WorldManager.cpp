/*
 * WorldManager.cpp
 *
 *  Created on: 2015年6月13日
 *      Author: leave
 */

#include "../optdsvr/WorldManager.h"
#include "manager/DC_File.h"
#include "manager/BSTechManager.h"
#include "manager/BSRmbShopManager.h"
#include "manager/BSLevelGroupInfoManager.h"
#include "manager/BSFactory.h"
#include "manager/BSDiamondShop.h"
#include "manager/BSTowerManager.h"
#include "database/PassPortDatabaseConnection.h"
#include "vnVariableScript.h"
#include "vnBase64.h"
#include "File/FileSystemPKG.h"
#include "table/Table_training.h"
extern PassPortDatabaseWorkerPool PassPortDatabase;

using namespace _vn_ns;

//optd 读取配置文件
bool WorldManager::loadBaseConfig() {
	VariableScript vs;
	RefCountedPtr<Variable_object> m_config;
	m_config = vs.parse({0, "optd.var"});
	if (!m_config) {
	VN_LOG_FATAL("can NOT load config!");
	return false;
	}
	//optd
	m_listening_port = (u16)m_config->queryAttributeInt32("client_port", m_listening_port);
	m_num_pool_threads = m_config->queryAttributeInt32("thread_count", (u32)m_num_pool_threads);
	m_version = m_config->queryAttributeInt32("version", m_version);
	m_min_version = m_config->queryAttributeInt32("min_version", m_min_version);
	//tbl 表格路径
	//str8 strVersionFile = m_config->queryAttributeString("version_file","");
	 m_version_file = m_config->queryAttributeString("version_file", m_version_file.fullPath()).c_str();

	m_console_mode = m_config->queryAttributeBool("console_mode",m_console_mode);
	m_intrusion_beginLevel = (u32)m_config->queryAttributeInt32("intrusion_beginLevel", (s32)m_intrusion_beginLevel);
	m_intrusion_interval = (u32)m_config->queryAttributeInt32("intrusion_interval", (s32)m_intrusion_interval);

	m_coupon_interval = m_config->queryAttributeFloat("coupon_interval", m_coupon_interval);

	m_lottery_beginLevel = (u32)m_config->queryAttributeInt32("lottery_beginLevel", (s32)m_lottery_beginLevel);
	m_lottery_validTime = m_config->queryAttributeFloat("lottery_validTime", m_lottery_validTime);
	m_lottery_drawTimes = (u32)m_config->queryAttributeInt32("lottery_drawTimes", (s32)m_lottery_drawTimes);
	m_lottery_drawCost = (u32)m_config->queryAttributeInt32("lottery_drawCost", (s32)m_lottery_drawCost);

	m_buy_nocheck = m_config->queryAttributeBool("buy_nocheck");
	m_sDBInfo = m_config->queryAttributeString("psssport_db_info", "");
		if (m_sDBInfo == "") {
			VN_LOG_FATAL("database not specified in configuration file !");
			return false;
		}

	m_dbThreads = m_config->queryAttributeInt32("psssport_db_threads", 1);
	//rmb_shop.var hero.var 路径
	str8 strResPath=m_config->queryAttributeString("resource", "");
	if(strResPath == "")
	{
		VN_LOG_FATAL(" not specified resource file path !");
		return false;
	}
	 FileManager::instance().setFileSystem(2, vnnew FileSystemOS(m_config->queryAttributeString("resource")));
    return true;
}


//todo
bool WorldManager::loadVersionData()
{
	FileStream *fs = FileManager::instance().open(m_version_file);
	if (!fs)
	{
		return false;
	}
	m_version_file_size = (u32)fs->size();
	const int kSize = 4096;
	int count = m_version_file_size / kSize;
	int rest = m_version_file_size % kSize;
	m_version_file_data.resize((rest ? count + 1 : count));
	for (auto &i : m_version_file_data) {
	u8 buf[kSize];
	size_t bytes = fs->read(buf, kSize);
	i.resize(bytes);
	memcpy(i.data(), buf, bytes);
	}

	fs->close();
	return true;
}
bool WorldManager::loadTBL()
{
	u32 fsId = 0;
	str8 path = "";

	RefCountedPtr<FileSystemPKG> pkg = vnnew FileSystemPKG();
	if (pkg->load(FileManager::instance().findFileSystem(0).get(), m_version_file.fileName))
	{
		fsId = 1;
		FileManager::instance().setFileSystem(fsId, std::move(pkg));
	} else {
		 path = "res/";
	}
	 tbl::Table_tech techs;
	    if (!techs.init(fsId, path))
	    {
	        VN_LOG_ERROR("Load Table_tech failed!");
	    }
	    else
	    {
	        BSTechManager::instance().initData(techs);
	    }

	    tbl::Table_training training;
	    if (!training.init(fsId, path))
	    {
	        VN_LOG_ERROR("Load Table_training failed!");
	    }
	    else
	    {
	        BSTowerManager::instance().initData(training);
	    }

	    BSRmbShopManager::instance().init(2, "rmb_shop.var");

	    tbl::Table_diamond_shop diamondShop;
	    if (!diamondShop.init(fsId, path)) {
	        VN_LOG_ERROR("Load Table_diamond_shop failed!");
	    } else {
	        BSDiamondShop::instance().initData(diamondShop);
	    }

	    if (!BSFactory::instance().level().init(fsId, path)) {
	        VN_LOG_ERROR("Load Table_level failed!");
	    }

	    tbl::Table_levelclass levelclass;
	    if (!levelclass.init(fsId, path)) {
	        VN_LOG_ERROR("Load Table_levelclass failed!");
	    } else {
	        BSLevelGroupInfoManager::instance().initData(levelclass);
	    }

	    if (!BSFactory::instance().progress().init(fsId, path))
	        VN_LOG_DEBUG("Load Table_progress failed!");

	    if (!BSFactory::instance().herorune().init(fsId, path))
	        VN_LOG_DEBUG("Load Table_herorune failed!");

	    if (!BSFactory::instance().tower().init(fsId, path))
	        VN_LOG_DEBUG("Load Table_tower failed!");

	    if (!BSFactory::instance().itemStore().init(fsId, path))
	        VN_LOG_DEBUG("Load Table_itemstore failed.");

	    if (!BSFactory::instance().monster().init(fsId, path))
	        VN_LOG_DEBUG("Load Table_monster failed.");

	    if (!BSHero::loadInitalHeroList(2, "hero.var")) {
	        VN_LOG_DEBUG("Load HeroList failed!");
	    }

	    if (!BSGoldMine::loadInitialGoldMineList(fsId, path)) {
	        VN_LOG_DEBUG("Load GoldMineList failed!");
	    }

	    tbl::Table_platform platforms;
	    if (!platforms.init(fsId, path)) {
	        VN_LOG_ERROR("Load Table_platform failed!");
	    } else {
	        BSPlatformStoreInfo::instance().initData(platforms);
	    }

	    tbl::Table_coupon coupons;
	    if (!coupons.init(2, "")) {
	        VN_LOG_ERROR("Load Table_coupon failed!");
	    } else {
	        BSCouponManager::instance().initData(coupons);
	    }

	    tbl::Table_award awards;
	    if (!awards.init(2, "")) {
	        VN_LOG_ERROR("load Table_award failed!");
	    } else {
	        BSAwardManager::instance().initData(awards);
	    }

	    tbl::Table_mission missions;
	    if (!missions.init(fsId, path)) {
	        VN_LOG_ERROR("load Table_mission failed!");
	    } else {
	        BSMissionManager::instance().initData(missions);
	    }

	    tbl::Table_Registration registration;
	    if (!registration.init(fsId, path)) {
	        VN_LOG_ERROR("load Table_registration failed!");
	    } else {
	        BSRegistrationManager::instance().initData(registration);
	    }
	return true;
}

//更新通告（通过console）
void WorldManager::updateAnnouncement(vn::str8 content)
{
	m_announcementLock.lock_write();
	++m_data.announcementId;
	m_data.announcementContent = std::move(content);
	m_announcementLock.unlock();
	//保存在数据库中
	DataCenter::instance().saveServerData(m_data);
	//LoggedUserManager::instance().broadcast(&LoggedUser::bc_updateAnnouncement);
}


//发布通告(玩家登录的时候)
void WorldManager::announcement(vn::u32 &id, vn::str8 *content)
{
	 m_announcementLock.lock();
	id = m_data.announcementId;
	if (content) {
		*content = m_data.announcementContent;
	}
	m_announcementLock.unlock();
}




void WorldManager::update(vn::f32 deltaTime)
{
	//bool dirty = false;
	DateTime dt = DateTime::now();
	if (m_data.last_registration_date != dt) {
		//dirty = true;
		m_data.last_registration_date = dt;
		//LoggedUserManager::instance().broadcast(&LoggedUser::bc_lotteryAndRegistration);
	}
}




















