/*
 * NetLogDBConnection.cpp
 *
 *  Created on: 2015年11月9日
 *      Author: leave
 */

#include "NetLogDBConnection.h"

void NetLogDBConnection::DoPrepareStatements() {
	if (!m_reconnecting)
		m_stmts.resize(MAX_GUNNETLOGDATABASE_STATEMENTS);

	PrepareStatement(GUN_NETLOG_INS_PlayerLevelFlow, "INSERT INTO PlayerLevelFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,LevelChange,AfterLevel,Reason,SubReason,AddOrReduce) VALUES (?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_CharacterUnlockFlow, "INSERT INTO CharacterUnlockFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,CharacterId,Reason,SubReason,AddOrReduce,Costs) VALUES (?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_QuestionnaireFlow, "INSERT INTO QuestionnaireFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,QuestionnaireId,Type,PlayerLevel,VipLevel,url) VALUES (?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_PartnerUnlockFlow, "INSERT INTO PartnerUnlockFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,PartnerId,Reason,SubReason,AddOrReduce,Costs) VALUES (?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_DayLoginRewardsFlow, "INSERT INTO DayLoginRewardsFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,RewardId,ContinueDayNum,CumuDayNum,IsReSign,Rewards,Reason,SubReason,SignType) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_PartnerStarLevelFlow, "INSERT INTO PartnerStarLevelFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,PartnerId,StarLevelChange,AfterStarLevel,Reason,SubReason,AddOrReduce,Costs) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_PlayerRegister, "INSERT INTO PlayerRegister(PlatID,iZoneAreaID,RegChannel,vopenid,DeviceId,SystemHardware,SystemSoftware,ClientIP,dtEventTime,GameSvrId,vGameAppid,ClientVersion,TelecomOper,Network,ScreenWidth,ScreenHight,Density,CpuHardware,Memory,GLRender,GLVersion,guid) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_TaskFlow, "INSERT INTO TaskFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,TaskId,TaskCategory,Rewards,PlayerLevel,TaskType) VALUES (?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_PveFlow, "INSERT INTO PveFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,CharacterId1,CharacterId2,ParterId,PveLevelId,ConsumePhysicalPower,IsCleanOut,ReviveTimes,BeginTime,EndTime,StarNum,GameRewards,TeamBattleForce,IsSuccess,PlayerLevel,Character1Force,Character2Force,ParterForce,ClientBattleTime) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_PvpFlow, "INSERT INTO PvpFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,CharacterId1,CharacterId2,CharacterId3,ParterId,OppGuid,OppCharacterId1,OppCharacterId2,OppCharacterId3,OppParterId,BeforePvpOrder,AfterPvpOrder,BeginTime,EndTime,GameRewards,TeamBattleForce,OppTeamBattleForce,IsSuccess,PlayerLevel,SubMode,Character1Force,Character2Force,Character3Force,ParterForce,OppCharacter1Force,OppCharacter2Force,OppParterForce,OppCharacter3Force,ClientBattleTime) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_GameSnsFlow, "INSERT INTO GameSnsFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,vopenid2,guid2,Num,SNSType,SNSSubType,iMoney,iMoneyType) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_PkLevelFlow, "INSERT INTO PkLevelFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,LevelChange,BeforeLevel,AfterLevel,Reason,SubReason,AddOrReduce) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_EquipStageLevelFlow, "INSERT INTO EquipStageLevelFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,CharacterId,EquipId,StageLevelChange,AfterStageLevel,Reason,SubReason,AddOrReduce,Costs,EquipConfigId,EquipType) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_EquipLevelFlow, "INSERT INTO EquipLevelFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,CharacterId,EquipId,LevelChange,AfterLevel,Reason,SubReason,AddOrReduce,Costs,EquipConfigId,EquipType) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_WeaponSetFlow, "INSERT INTO WeaponSetFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,CharacterId,SetWeaponId,UnSetWeaponId) VALUES (?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_PlayerPhysicalPowerFlow, "INSERT INTO PlayerPhysicalPowerFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,PhysicalPowerChange,AfterLPhysicalPower,Reason,SubReason,AddOrReduce) VALUES (?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_PvpScoreFlow, "INSERT INTO PvpScoreFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,ScoreChange,BeforeScore,AfterScore,Reason,SubReason,AddOrReduce) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_PvpTokenFlow, "INSERT INTO PvpTokenFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,TokenChange,BeforeToken,AfterToken,Reason,SubReason,AddOrReduce) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_CharacterLevelFlow, "INSERT INTO CharacterLevelFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,CharacterId,LevelChange,AfterLevel,Reason,SubReason,AddOrReduce,Costs) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_PlayerExpFlow, "INSERT INTO PlayerExpFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,ExpChange,BeforeLevel,AfterLevel,Time,Reason,SubReason,guid) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_PvpGradeFlow, "INSERT INTO PvpGradeFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,GradeChange,BeforeGrade,AfterGrade,Reason,SubReason,AddOrReduce) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_CarnivalRewardFlow, "INSERT INTO CarnivalRewardFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,RewardId) VALUES (?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_AchievementFlow, "INSERT INTO AchievementFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,AchievementId,AchievementLevel,IsFinish,PlayerLevel) VALUES (?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_SnsFlow, "INSERT INTO SnsFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,ActorOpenID,RecNum,Count,SNSType,SNSSubType,guid,vopenid) VALUES (?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_CarnivalFlow, "INSERT INTO CarnivalFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,CharacterId1,CharacterId2,ParterId,CarnivalType,CarnivalLevelId,ReviveTimes,BeginTime,EndTime,GameRewards,TeamBattleForce,IsSuccess,PlayerLevel,Character1Force,Character2Force,ParterForce,ClientBattleTime) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_PartnerLevelFlow, "INSERT INTO PartnerLevelFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,PartnerId,LevelChange,AfterLevel,Reason,SubReason,AddOrReduce,Costs) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_CharacterStarLevelFlow, "INSERT INTO CharacterStarLevelFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,CharacterId,StarLevelChange,AfterStarLevel,Reason,SubReason,AddOrReduce,Costs) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_PlayerVipLevelFlow, "INSERT INTO PlayerVipLevelFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,LevelChange,AfterLevel,Reason,SubReason,AddOrReduce) VALUES (?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_TaskActiveFlow, "INSERT INTO TaskActiveFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,BoxId,BeforeActiveVal,AfterActiveVal,AddOrReduce,Rewards,PlayerLevel,TaskType,TaskId) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_ReportFlow, "INSERT INTO ReportFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,EventName,EventArgs) VALUES (?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_RoundFlow, "INSERT INTO RoundFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,BattleID,BattleType,RoundScore,RoundTime,Result,Rank,Gold,guid) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_MoneyFlow, "INSERT INTO MoneyFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,Sequence,Level,AfterMoney,iMoney,Reason,SubReason,AddOrReduce,iMoneyType,guid) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_PlayerLogin, "INSERT INTO PlayerLogin(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,Level,PlayerFriendsNum,ClientVersion,SystemSoftware,SystemHardware,TelecomOper,Network,ScreenWidth,ScreenHight,Density,LoginChannel,vRoleID,vRoleName,CpuHardware,Memory,GLRender,GLVersion,DeviceId,guid,NickName,GameFriendsNum,PlatFriendOpenidsNum,PlatFriendGuidsNum,AndroidAccountNum,IosAccountNum,GameVipLevel,ClientIP) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_EquipSetFlow, "INSERT INTO EquipSetFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,CharacterId,SetEquipId,SetEquipConfigId,SetEquipStar,SetEquipLevel,UnSetEquipId,UnSetEquipConfigId,UnSetEquipStar,UnSetEquipLevel,EquipType) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_SyncPvpFlow, "INSERT INTO SyncPvpFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,PvpMode,PvpGrade,PlayCount,CharacterId,Rank,BattleRes,GameMaxLevel,GameDuration,SelectTalent,Reason,SubReason,AddOrReduce,Rewards,Talent,BattleID,BattleEndTime,KofCharacterIds,BattleResInfo) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_PlayerLogout, "INSERT INTO PlayerLogout(PlatID,iZoneAreaID,vopenid,ClientVersion,DeviceId,LoginIP,LoginTime,LogoutTime,GameSvrId,dtEventTime,vGameAppid,OnlineTime,Level,PlayerFriendsNum,SystemSoftware,SystemHardware,TelecomOper,Network,ScreenWidth,ScreenHight,Density,LoginChannel,CpuHardware,Memory,GLRender,GLVersion,guid,GameVipLevel,CharacterNum) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_CharacterWakeUpLevelFlow, "INSERT INTO CharacterWakeUpLevelFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,CharacterId,StarLevelChange,AfterWakeUpLevel,Reason,SubReason,AddOrReduce,Costs) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_GameCouponFlow, "INSERT INTO GameCouponFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,Level,CouponType,ChangeValue,AfterValue,Reason,SubReason,AddOrReduce) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_GameFreshTeachFlow, "INSERT INTO GameFreshTeachFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,Level,StepId) VALUES (?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_CreateGameName, "INSERT INTO CreateGameName(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,GameName) VALUES (?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_GameSvrState, "INSERT INTO GameSvrState(PlatID,iZoneAreaID,OnlineNum,dtEventTime,vopenid,vGameIP,GameSvrId,vGameAppid) VALUES (?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_ItemFlow, "INSERT INTO ItemFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,Level,Sequence,iGoodsType,iGoodsId,Count,AfterCount,Reason,SubReason,iMoney,iMoneyType,AddOrReduce,guid) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_ShopFlow, "INSERT INTO ShopFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,BuyItemType,BuyItemId,BuyItemCount,CostItemType,CostItemId,CostItemCount,ShopId) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_ShopRefreshFlow, "INSERT INTO ShopRefreshFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,ShopId,ShopTabId,IsAutoRefresh,iMoney,iMoneyType) VALUES (?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_TowerFlow, "INSERT INTO TowerFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,CharacterId1,CharacterId2,ParterId,TowerGameMode,TowerLayerNum,ReviveTimes,BeginTime,EndTime,GameRewards,TeamBattleForce,IsSuccess,PlayerLevel,IsCleanOut,Character1Force,Character2Force,ParterForce,ClientBattleTime) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

	PrepareStatement(GUN_NETLOG_INS_PvpExpFlow, "INSERT INTO PvpExpFlow(GameSvrId,dtEventTime,vGameAppid,PlatID,iZoneAreaID,vopenid,guid,ExpChange,BeforeExp,AfterExp,Reason,SubReason,AddOrReduce) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?)");







}
