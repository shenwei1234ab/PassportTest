/*
 * ConfigMgr.cpp
 *
 *  Created on: 2015年11月9日
 *      Author: leave
 */

#include "ConfigMgr.h"
#include "vnVariableScript.h"
#include "vnBase64.h"
#include "vnLog.h"
#include "common/LogPrinter.h"
#include "../../database/NetLogDBConnection.h"

using namespace _vn_ns;

ConfigMgr::ConfigMgr() {

}

ConfigMgr::~ConfigMgr() {
}

bool ConfigMgr::loadBaseConfig(){
	VariableScript vs;
	RefCountedPtr<Variable_object> m_config;
	m_config = vs.parse( { 0, "GunNetLogConfig.var" });
	if (!m_config) {
		VN_LOG_FATAL("can NOT load config!");
		return false;
	}
	m_netlog_port = (u16) m_config->queryAttributeInt32("netlog_port", (int) m_netlog_port);
	m_console_mode = m_config->queryAttributeBool("console_mode", m_console_mode);
	if (!m_console_mode) {
		Log::instance().printer(new LogPrinterFile("logs"));
	}

    m_sDBInfo = m_config->queryAttributeString("psssport_db_info", "");
	if (m_sDBInfo == "") {
		VN_LOG_FATAL("database not specified in configuration file !");
		return false;
	}
	m_dbThreads = m_config->queryAttributeInt32("psssport_db_threads", 1);

	VN_LOG_FATAL("loadBaseConfig OK! ");
	return true;
}

bool ConfigMgr::loadSqlMap() {
	logMaps.clear();
	logMaps.insert( { "PlayerLevelFlow", GUN_NETLOG_INS_PlayerLevelFlow });
	logMaps.insert( { "CharacterUnlockFlow", GUN_NETLOG_INS_CharacterUnlockFlow });
	logMaps.insert( { "QuestionnaireFlow", GUN_NETLOG_INS_QuestionnaireFlow });
	logMaps.insert( { "PartnerUnlockFlow", GUN_NETLOG_INS_PartnerUnlockFlow });
	logMaps.insert( { "DayLoginRewardsFlow", GUN_NETLOG_INS_DayLoginRewardsFlow });
	logMaps.insert( { "PartnerStarLevelFlow", GUN_NETLOG_INS_PartnerStarLevelFlow });
	logMaps.insert( { "PlayerRegister", GUN_NETLOG_INS_PlayerRegister });
	logMaps.insert( { "TaskFlow", GUN_NETLOG_INS_TaskFlow });
	logMaps.insert( { "PveFlow", GUN_NETLOG_INS_PveFlow });
	logMaps.insert( { "PvpFlow", GUN_NETLOG_INS_PvpFlow });
	logMaps.insert( { "GameSnsFlow", GUN_NETLOG_INS_GameSnsFlow });
	logMaps.insert( { "PkLevelFlow", GUN_NETLOG_INS_PkLevelFlow });
	logMaps.insert( { "EquipStageLevelFlow", GUN_NETLOG_INS_EquipStageLevelFlow });
	logMaps.insert( { "EquipLevelFlow", GUN_NETLOG_INS_EquipLevelFlow });
	logMaps.insert( { "WeaponSetFlow", GUN_NETLOG_INS_WeaponSetFlow });
	logMaps.insert( { "PlayerPhysicalPowerFlow", GUN_NETLOG_INS_PlayerPhysicalPowerFlow });
	logMaps.insert( { "PvpScoreFlow", GUN_NETLOG_INS_PvpScoreFlow });
	logMaps.insert( { "PvpTokenFlow", GUN_NETLOG_INS_PvpTokenFlow });
	logMaps.insert( { "CharacterLevelFlow", GUN_NETLOG_INS_CharacterLevelFlow });
	logMaps.insert( { "PlayerExpFlow", GUN_NETLOG_INS_PlayerExpFlow });
	logMaps.insert( { "PvpGradeFlow", GUN_NETLOG_INS_PvpGradeFlow });
	logMaps.insert( { "CarnivalRewardFlow", GUN_NETLOG_INS_CarnivalRewardFlow });
	logMaps.insert( { "AchievementFlow", GUN_NETLOG_INS_AchievementFlow });
	logMaps.insert( { "SnsFlow", GUN_NETLOG_INS_SnsFlow });
	logMaps.insert( { "CarnivalFlow", GUN_NETLOG_INS_CarnivalFlow });
	logMaps.insert( { "PartnerLevelFlow", GUN_NETLOG_INS_PartnerLevelFlow });
	logMaps.insert( { "CharacterStarLevelFlow", GUN_NETLOG_INS_CharacterStarLevelFlow });
	logMaps.insert( { "PlayerVipLevelFlow", GUN_NETLOG_INS_PlayerVipLevelFlow });
	logMaps.insert( { "TaskActiveFlow", GUN_NETLOG_INS_TaskActiveFlow });
	logMaps.insert( { "ReportFlow", GUN_NETLOG_INS_ReportFlow });
	logMaps.insert( { "RoundFlow", GUN_NETLOG_INS_RoundFlow });
	logMaps.insert( { "MoneyFlow", GUN_NETLOG_INS_MoneyFlow });
	logMaps.insert( { "PlayerLogin", GUN_NETLOG_INS_PlayerLogin });
	logMaps.insert( { "EquipSetFlow", GUN_NETLOG_INS_EquipSetFlow });
	logMaps.insert( { "SyncPvpFlow", GUN_NETLOG_INS_SyncPvpFlow });
	logMaps.insert( { "PlayerLogout", GUN_NETLOG_INS_PlayerLogout });
	logMaps.insert( { "CharacterWakeUpLevelFlow", GUN_NETLOG_INS_CharacterWakeUpLevelFlow });
	logMaps.insert( { "GameCouponFlow", GUN_NETLOG_INS_GameCouponFlow });
	logMaps.insert( { "GameFreshTeachFlow", GUN_NETLOG_INS_GameFreshTeachFlow });
	logMaps.insert( { "CreateGameName", GUN_NETLOG_INS_CreateGameName });
	logMaps.insert( { "GameSvrState", GUN_NETLOG_INS_GameSvrState });
	logMaps.insert( { "ItemFlow", GUN_NETLOG_INS_ItemFlow });
	logMaps.insert( { "ShopFlow", GUN_NETLOG_INS_ShopFlow });
	logMaps.insert( { "ShopRefreshFlow", GUN_NETLOG_INS_ShopRefreshFlow });
	logMaps.insert( { "TowerFlow", GUN_NETLOG_INS_TowerFlow });
	logMaps.insert( { "PvpExpFlow", GUN_NETLOG_INS_PvpExpFlow });

	return true;
}
