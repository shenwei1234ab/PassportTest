/*
 * NetLogDBConnection.h
 *
 *  Created on: 2015年11月9日
 *      Author: leave
 */

#ifndef SRC_NETLOGSVR_DATABASE_NETLOGDBCONNECTION_H_
#define SRC_NETLOGSVR_DATABASE_NETLOGDBCONNECTION_H_

#include "db/MySQLConnection.h"
#include "db/DatabaseWorkerPool.h"

class NetLogDBConnection: public MySQLConnection {
public:
	//- Constructors for sync and async connections
	NetLogDBConnection(MySQLConnectionInfo& connInfo) :
			MySQLConnection(connInfo) {
	}
	virtual ~NetLogDBConnection(){}

	//- Loads database type specific prepared statements
	void DoPrepareStatements() override;

};
typedef DatabaseWorkerPool<NetLogDBConnection> NetLogDBWorkerPool;

enum NetLogDatabaseStatements {
	NETLOG_INS_GameSvrState,
	NETLOG_INS_PlayerRegister,
	NETLOG_INS_PlayerLogin,
	NETLOG_INS_PlayerLogout,
	NETLOG_INS_MoneyFlow,
	NETLOG_INS_ItemFlow,
	NETLOG_INS_PlayerExpFlow,
	NETLOG_INS_PlayerLvUpFlow,
	NETLOG_INS_TicketFlow,
	NETLOG_INS_TicketSend,
	NETLOG_INS_MailFlow,
	NETLOG_INS_IdipOpFlow,
	NETLOG_INS_DiamondPayRecord,
	NETLOG_INS_DiamondPresentRecord,
	NETLOG_INS_EndlessRecord,
	NETLOG_INS_PvpRecord,
	NETLOG_INS_BattleEndReport,
	NETLOG_INS_PvpBattleReport,
	NETLOG_INS_DotaPvpBattleReport,
	NETLOG_INS_StrengthUpRecord,
	NETLOG_INS_SellCardRecord,
	NETLOG_INS_LotteryRecord,
	NETLOG_INS_DropAwardRecord,
	NETLOG_INS_ItemUseRecord,
	NETLOG_INS_HeartFlowRecord,
	NETLOG_INS_LevUpRecord,
	NETLOG_INS_ChipCombineRecord,
	NETLOG_INS_ChipChangeRecord,
	NETLOG_INS_ItemMoneyFlow,
	NETLOG_INS_SnsFlow,
	NETLOG_INS_PlayerLvUpRecord,
	NETLOG_INS_StrengthUpRecordEx,
	NETLOG_INS_RaidsRecord,
	NETLOG_INS_PlayerCardRecord,
	NETLOG_INS_PlayerChipRecord,
	NETLOG_INS_PlayerEquipmentRecord,
	NETLOG_INS_EquipmentFlow,
	NETLOG_INS_ConsumableFlow,
	NETLOG_INS_GemFlow,
	NETLOG_INS_AntModeBtlRecord,
	NETLOG_INS_TaskRecord,
	NETLOG_INS_AntiCheatRecord,
	NETLOG_INS_SubscribeFlow,
	NETLOG_INS_SubscribeStore,
	NETLOG_INS_ActivityAward,
	NETLOG_INS_DiamondBuyRemnantRecord,
	NETLOG_INS_SecRoundStartFlow,
	NETLOG_INS_SecRoundEndFlow,
	NETLOG_INS_SecPVPStartFlow,
	NETLOG_INS_SecPVPEndFlow,
	NETLOG_INS_VIPPrivilegeUsageRecord,
	NETLOG_INS_VIPRechargeRecord,
	NETLOG_INS_VIPLoginRecord,
	NETLOG_INS_TowerEquipLvUpRecord,
	NETLOG_INS_TaskGetAwardRecord,
	NETLOG_INS_MarketBuyItem,
	NETLOG_INS_MarketRefresh,
	NETLOG_INS_MarketActive,
	NETLOG_INS_MarketUnlock,
	NETLOG_INS_PureModeBuyUnlock,
	NETLOG_INS_PureModeBuyKey,
	NETLOG_INS_PureModeUseTreasureBox,
	NETLOG_INS_PureModeUseKey,
	NETLOG_INS_GemMerge,
	NETLOG_INS_GemEmbed,
	NETLOG_INS_GemExcise,
	NETLOG_INS_EventRecord,
	NETLOG_INS_OnlinePlayer,

	MAX_NETLOGDATABASE_STATEMENTS,
};

enum GunNetLogDatabaseStatements {
	GUN_NETLOG_INS_PlayerLevelFlow,
	GUN_NETLOG_INS_CharacterUnlockFlow,
	GUN_NETLOG_INS_QuestionnaireFlow,
	GUN_NETLOG_INS_PartnerUnlockFlow,
	GUN_NETLOG_INS_DayLoginRewardsFlow,
	GUN_NETLOG_INS_PartnerStarLevelFlow,
	GUN_NETLOG_INS_PlayerRegister,
	GUN_NETLOG_INS_TaskFlow,
	GUN_NETLOG_INS_PveFlow,
	GUN_NETLOG_INS_PvpFlow,
	GUN_NETLOG_INS_GameSnsFlow,
	GUN_NETLOG_INS_PkLevelFlow,
	GUN_NETLOG_INS_EquipStageLevelFlow,
	GUN_NETLOG_INS_EquipLevelFlow,
	GUN_NETLOG_INS_WeaponSetFlow,
	GUN_NETLOG_INS_PlayerPhysicalPowerFlow,
	GUN_NETLOG_INS_PvpScoreFlow,
	GUN_NETLOG_INS_PvpTokenFlow,
	GUN_NETLOG_INS_CharacterLevelFlow,
	GUN_NETLOG_INS_PlayerExpFlow,
	GUN_NETLOG_INS_PvpGradeFlow,
	GUN_NETLOG_INS_CarnivalRewardFlow,
	GUN_NETLOG_INS_AchievementFlow,
	GUN_NETLOG_INS_SnsFlow,
	GUN_NETLOG_INS_CarnivalFlow,
	GUN_NETLOG_INS_PartnerLevelFlow,
	GUN_NETLOG_INS_CharacterStarLevelFlow,
	GUN_NETLOG_INS_PlayerVipLevelFlow,
	GUN_NETLOG_INS_TaskActiveFlow,
	GUN_NETLOG_INS_ReportFlow,
	GUN_NETLOG_INS_RoundFlow,
	GUN_NETLOG_INS_MoneyFlow,
	GUN_NETLOG_INS_PlayerLogin,
	GUN_NETLOG_INS_EquipSetFlow,
	GUN_NETLOG_INS_SyncPvpFlow,
	GUN_NETLOG_INS_PlayerLogout,
	GUN_NETLOG_INS_CharacterWakeUpLevelFlow,
	GUN_NETLOG_INS_GameCouponFlow,
	GUN_NETLOG_INS_GameFreshTeachFlow,
	GUN_NETLOG_INS_CreateGameName,
	GUN_NETLOG_INS_GameSvrState,
	GUN_NETLOG_INS_ItemFlow,
	GUN_NETLOG_INS_ShopFlow,
	GUN_NETLOG_INS_ShopRefreshFlow,
	GUN_NETLOG_INS_TowerFlow,
	GUN_NETLOG_INS_PvpExpFlow,
	MAX_GUNNETLOGDATABASE_STATEMENTS,
};

#endif /* SRC_NETLOGSVR_DATABASE_NETLOGDBCONNECTION_H_ */
