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
	m_config = vs.parse( { 0, "config.var" });
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
	logMaps.insert( { "GameSvrState", NETLOG_INS_GameSvrState });
	logMaps.insert( { "PlayerRegister", NETLOG_INS_PlayerRegister });
	logMaps.insert( { "PlayerLogin", NETLOG_INS_PlayerLogin });
	logMaps.insert( { "PlayerLogout", NETLOG_INS_PlayerLogout });
	logMaps.insert( { "MoneyFlow", NETLOG_INS_MoneyFlow });
	logMaps.insert( { "ItemFlow", NETLOG_INS_ItemFlow });
	logMaps.insert( { "PlayerExpFlow", NETLOG_INS_PlayerExpFlow });
	logMaps.insert( { "PlayerLvUpFlow", NETLOG_INS_PlayerLvUpFlow });
	logMaps.insert( { "TicketFlow", NETLOG_INS_TicketFlow });
	logMaps.insert( { "TicketSend", NETLOG_INS_TicketSend });
	logMaps.insert( { "MailFlow", NETLOG_INS_MailFlow });
	logMaps.insert( { "IdipOpFlow", NETLOG_INS_IdipOpFlow });
	logMaps.insert( { "DiamondPayRecord", NETLOG_INS_DiamondPayRecord });
	logMaps.insert( { "DiamondPresentRecord", NETLOG_INS_DiamondPresentRecord });
	logMaps.insert( { "EndlessRecord", NETLOG_INS_EndlessRecord });
	logMaps.insert( { "PvpRecord", NETLOG_INS_PvpRecord });
	logMaps.insert( { "BattleEndReport", NETLOG_INS_BattleEndReport });
	logMaps.insert( { "PvpBattleReport", NETLOG_INS_PvpBattleReport });
	logMaps.insert( { "DotaPvpBattleReport", NETLOG_INS_DotaPvpBattleReport });
	logMaps.insert( { "StrengthUpRecord", NETLOG_INS_StrengthUpRecord });
	logMaps.insert( { "SellCardRecord", NETLOG_INS_SellCardRecord });
	logMaps.insert( { "LotteryRecord", NETLOG_INS_LotteryRecord });
	logMaps.insert( { "DropAwardRecord", NETLOG_INS_DropAwardRecord });
	logMaps.insert( { "ItemUseRecord", NETLOG_INS_ItemUseRecord });
	logMaps.insert( { "HeartFlowRecord", NETLOG_INS_HeartFlowRecord });
	logMaps.insert( { "LevUpRecord", NETLOG_INS_LevUpRecord });
	logMaps.insert( { "ChipCombineRecord", NETLOG_INS_ChipCombineRecord });
	logMaps.insert( { "ChipChangeRecord", NETLOG_INS_ChipChangeRecord });
	logMaps.insert( { "ItemMoneyFlow", NETLOG_INS_ItemMoneyFlow });
	logMaps.insert( { "SnsFlow", NETLOG_INS_SnsFlow });
	logMaps.insert( { "PlayerLvUpRecord", NETLOG_INS_PlayerLvUpRecord });
	logMaps.insert( { "StrengthUpRecordEx", NETLOG_INS_StrengthUpRecordEx });
	logMaps.insert( { "RaidsRecord", NETLOG_INS_RaidsRecord });
	logMaps.insert( { "PlayerCardRecord", NETLOG_INS_PlayerCardRecord });
	logMaps.insert( { "PlayerChipRecord", NETLOG_INS_PlayerChipRecord });
	logMaps.insert( { "PlayerEquipmentRecord", NETLOG_INS_PlayerEquipmentRecord });
	logMaps.insert( { "EquipmentFlow", NETLOG_INS_EquipmentFlow });
	logMaps.insert( { "ConsumableFlow", NETLOG_INS_ConsumableFlow });
	logMaps.insert( { "GemFlow", NETLOG_INS_GemFlow });
	logMaps.insert( { "AntModeBtlRecord", NETLOG_INS_AntModeBtlRecord });
	logMaps.insert( { "TaskRecord", NETLOG_INS_TaskRecord });
	logMaps.insert( { "AntiCheatRecord", NETLOG_INS_AntiCheatRecord });
	logMaps.insert( { "SubscribeFlow", NETLOG_INS_SubscribeFlow });
	logMaps.insert( { "SubscribeStore", NETLOG_INS_SubscribeStore });
	logMaps.insert( { "ActivityAward", NETLOG_INS_ActivityAward });
	logMaps.insert( { "DiamondBuyRemnantRecord", NETLOG_INS_DiamondBuyRemnantRecord });
	logMaps.insert( { "SecRoundStartFlow", NETLOG_INS_SecRoundStartFlow });
	logMaps.insert( { "SecRoundEndFlow", NETLOG_INS_SecRoundEndFlow });
	logMaps.insert( { "SecPVPStartFlow", NETLOG_INS_SecPVPStartFlow });
	logMaps.insert( { "SecPVPEndFlow", NETLOG_INS_SecPVPEndFlow });
	logMaps.insert( { "VIPPrivilegeUsageRecord", NETLOG_INS_VIPPrivilegeUsageRecord });
	logMaps.insert( { "VIPRechargeRecord", NETLOG_INS_VIPRechargeRecord });
	logMaps.insert( { "VIPLoginRecord", NETLOG_INS_VIPLoginRecord });
	logMaps.insert( { "TowerEquipLvUpRecord", NETLOG_INS_TowerEquipLvUpRecord });
	logMaps.insert( { "TaskGetAwardRecord", NETLOG_INS_TaskGetAwardRecord });
	logMaps.insert( { "MarketBuyItem", NETLOG_INS_MarketBuyItem });
	logMaps.insert( { "MarketRefresh", NETLOG_INS_MarketRefresh });
	logMaps.insert( { "MarketActive", NETLOG_INS_MarketActive });
	logMaps.insert( { "MarketUnlock", NETLOG_INS_MarketUnlock });
	logMaps.insert( { "PureModeBuyUnlock", NETLOG_INS_PureModeBuyUnlock });
	logMaps.insert( { "PureModeBuyKey", NETLOG_INS_PureModeBuyKey });
	logMaps.insert( { "PureModeUseTreasureBox", NETLOG_INS_PureModeUseTreasureBox });
	logMaps.insert( { "PureModeUseKey", NETLOG_INS_PureModeUseKey });
	logMaps.insert( { "GemMerge", NETLOG_INS_GemMerge });
	logMaps.insert( { "GemEmbed", NETLOG_INS_GemEmbed });
	logMaps.insert( { "GemExcise", NETLOG_INS_GemExcise });
	logMaps.insert( { "EventRecord", NETLOG_INS_EventRecord });
	logMaps.insert( { "OnlinePlayer", NETLOG_INS_OnlinePlayer });
	return true;
}
