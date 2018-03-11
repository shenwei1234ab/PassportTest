/*
 * PassPortDatabaseConnection.cpp
 *
 *  Created on: 2015年6月10日
 *      Author: leave
 */

#include "PassPortDatabaseConnection.h"

void PassPortDatabaseConnection::DoPrepareStatements() {
	if (!m_reconnecting)
		m_stmts.resize(MAX_PASSPORTDATABASE_STATEMENTS);

		PrepareStatement(ASY_TEST,"INSERT INTO test(Id)values(?)");
		//用户表
		PrepareStatement(UP_BASEINFO_BASEINFO,"update baseinfo set  clip0=?,clip1=?,clip2=?,money=?,diamonds=?,purchased_diamonds=? where UUId=?");
		PrepareStatement(UP_BASEINFO_HERO,"update baseinfo set  shovel0=?,shovel1=?,skull0=?,skull1=?,watch0=?,watch1=? where UUId=?");
		PrepareStatement(UP_BASEINFO_REGISTRATION,"update baseinfo set first_date=?,last_date=?,count=?,logingroup=? where UUId=?");
		PrepareStatement(SEL_BASEINFO,"select clip0,clip1,clip2,money,diamonds,purchased_diamonds,stamp,coupon_id,coupon_timer,unix_timestamp(coupon_timestamp),shovel0,shovel1,skull0,skull1,watch0,watch1,running,timer,unix_timestamp(intrusion_timestamp),count,first_date,logingroup,last_date,lottery_draw,lottery_state,lottery_timer,lottery_pool,unix_timestamp(lottery_timestamp),unix_timestamp(lastLoginTime) from baseinfo where UUId=?");
		PrepareStatement(INSERT_BASEINFO,"INSERT INTO baseinfo(UUId,clip0,clip1,clip2,money,diamonds,purchased_diamonds,stamp,coupon_id,coupon_timer,coupon_timestamp,shovel0,shovel1,skull0,skull1,watch0,watch1,running,timer,intrusion_timestamp,count,first_date,logingroup,last_date,lottery_draw,lottery_state,lottery_timer,lottery_pool,lottery_timestamp,lastLoginTime)  VALUES (?,?,?,?,?,?,?,?,?,?,from_unixtime(?),?,?,?,?,?,?,?,?,from_unixtime(?),?,?,?,?,?,?,?,?,from_unixtime(?),from_unixtime(?))");
		PrepareStatement(OPEN_HERO,"update  baseinfo set UUId=?,clip0=?,clip1=?,clip2=?,money=?,diamonds=?,purchased_diamonds=?,stamp=?,coupon_id=?,coupon_timer=?,shovel0=?,shovel1=?,skull0=?,skull1=?,watch0=?,watch1=?,running=?,timer=?,count=?,first_date=?,logingroup=?,last_date=?,lottery_draw=?,lottery_state=?,lottery_timer=? where UUId=?");
		//PrepareStatement(UP_BASEINFO_BASEINFO,"update baseinfo set  clip0=?,clip1=?,clip2=?,money=?,diamonds=?,purchased_diamonds=?,stamp=?,shovel0=?,shovel1=?,skull0=?,skull1=?,watch0=?,watch1=? where UUId=?");
		PrepareStatement(UP_BASEINFO_ALL,"update baseinfo set  clip0=?,clip1=?,clip2=?,money=?,diamonds=?,purchased_diamonds=?,stamp=?,coupon_id=?,coupon_timer=?,coupon_timestamp=from_unixtime(?),shovel0=?,shovel1=?,skull0=?,skull1=?,watch0=?,watch1=?,running=?,timer=?,intrusion_timestamp=from_unixtime(?),lottery_draw=?,lottery_state=?,lottery_timer=?,lottery_pool=?,lottery_timestamp=from_unixtime(?) where UUId=?");

		PrepareStatement(UP_INTRUSION,"update baseinfo set timer=? where UUId=?");
		PrepareStatement(UP_TIMESTAMP,"update baseinfo set stamp=? where UUId=?");
		PrepareStatement(UP_ADDMONEY,"update baseinfo set money=? where UUId=?");
		PrepareStatement(UP_ADDDIAMOND,"update baseinfo set diamonds=? where UUId=?");
		PrepareStatement(UP_CLIP,"update baseinfo set clip0=?,clip1=?,clip2=? where UUId=?");
		PrepareStatement(UP_LEVELPASSREWARD,"update baseinfo set money=?,diamonds=?,clip0=?,clip1=?,clip2=?,coupon_id=?,coupon_timer=? where UUId=?");
		PrepareStatement(UP_BASEINFO_LOTTERY,"update baseinfo set lottery_draw=?,lottery_state=?,lottery_timer=?,lottery_pool=?,lottery_timestamp=from_unixtime(?) where UUId=?");
		PrepareStatement(UP_BASEINFO_COUPON,"update baseinfo set coupon_id=?,coupon_timer=?,coupon_timestamp=from_unixtime(?)  where UUId=?");
		PrepareStatement(UP_LOGIN,"update baseinfo set coupon_id=?,coupon_timer=?,coupon_timestamp=from_unixtime(?),lottery_draw=?,lottery_state=?,lottery_timer=?,lottery_pool=?,lottery_timestamp=from_unixtime(?) ,lastLoginTime=from_unixtime(?) where UUId=?");

		PrepareStatement(UP_BASEINFO_INTRUSION,"update baseinfo set running=?,timer=?,intrusion_timestamp=from_unixtime(?) where UUId=?");




		//强制引导关系表
		PrepareStatement(SEL_BASEINFOFORCEGUIDE,"SELECT guideid FROM baseinfo_forceguide where UUId = ? ");
		PrepareStatement(INSERT_BASEINFOFORCEGUIDE,"insert into baseinfo_forceguide(UUId,guideid)values(?,?);");
		PrepareStatement(UP_BASEINFOFORCEGUIDE,"update baseinfo_forceguide set guideid=? where UUId=?");




		//查询指定UUid对应的英雄
		PrepareStatement(SEL_BASEINFOHERO,"SELECT heroid,id,towerclass,status,unlockRes from baseinfo_hero where UUId = ? ");
		PrepareStatement(INSERT_BASEINFOHERO,"insert into baseinfo_hero(UUId,heroid,id,towerclass,status,unlockRes)values(?,?,?,?,?,?);");
		PrepareStatement(UP_HEROSTATUS,"update baseinfo_hero set status=? where UUId=? and id=?");

		PrepareStatement(UP_UPGRADEHERO,"update baseinfo_hero set id=?,status=? where UUId=? and towerclass=?");

		//查询heroid对应的runes
		PrepareStatement(SEL_HERORUNES,"SELECT runeid,runestatus FROM hero_runes where heroid = ?");
		PrepareStatement(INSERT_HERORUNES,"insert into hero_runes(heroid,runeid,runestatus) values(?,?,?);");
		PrepareStatement(UP_RUNESTATUS,"update hero_runes set runestatus=? where heroid=? and runeid=?");
		PrepareStatement(FIND_HEROID,"select heroid from baseinfo_hero where UUId=? and towerclass=?");


		//Itemstore
		PrepareStatement(SEL_BASEINFOITEMSTORE,"select itemid,itemstatus from baseinfo_itemstore where UUId= ?");
		PrepareStatement(INSERT_BASEINFOITEMSTORE,"insert into baseinfo_itemstore(UUId,itemid,itemstatus)values(?,?,?);");
		PrepareStatement(UP_BASEINFOITEMSTORE,"update baseinfo_itemstore set itemstatus=? where UUId=? and itemid=?");


		//levels
		PrepareStatement(SEL_BASEINFOLEVELS,"select levelid,intrusion,intrusion_times,score,shovel,stars,watch from baseinfo_levels where UUId= ?");
		PrepareStatement(INSERT_BASEINFOLEVELS,"insert into baseinfo_levels(UUId,levelid,intrusion,intrusion_times,score,shovel,stars,watch)values(?,?,?,?,?,?,?,?);");
		PrepareStatement(UP_BASEINFOLEVELS,"update baseinfo_levels set intrusion=?,intrusion_times=?,score=?,shovel=?,stars=?,watch=? where UUId=? and levelid=?");

		//levelgroups
		PrepareStatement(INSERT_BASEINFOLEVELGROUPS,"insert into baseinfo_levelgroups(UUId,groupId,passed,intrusion_count,rewards_count)values(?,?,?,?,?);");
		PrepareStatement(UP_BASEINFOLEVELGROUPS,"update baseinfo_levelgroups set passed=?,intrusion_count=?,rewards_count=? where UUId=? and groupId=?");
		PrepareStatement(SEL_BASEINFOLEVELGROUPS,"select groupId,passed,intrusion_count,rewards_count from baseinfo_levelgroups where UUId=?");

		PrepareStatement(INSERT_LEVELGROUPS_LEVELS,"insert into levelgroups_levels(groupId,levelid,heros,platforms,resolved,shovel,stars,status,watch)values(?,?,?,?,?,?,?,?,?)");
		PrepareStatement(UP_LEVELGROUPSLEVELS,"update levelgroups_levels set heros=?,platforms=?,resolved=?,shovel=?,stars=?,status=?,watch=? where groupId=? and levelid=?");
		PrepareStatement(SEL_LEVELGROUPSLEVELS,"select levelid,heros,platforms,resolved,shovel,stars,status,watch from levelgroups_levels where groupId=?");




		//mines
		PrepareStatement(SEL_BASEINFOMINES,"select minesid,count,timer,unix_timestamp(mine_timestamp) from baseinfo_mines where UUId= ?");
		PrepareStatement(INSERT_BASEINFOMINES,"insert into baseinfo_mines(UUId,minesid,count,timer,mine_timestamp)values(?,?,?,?,from_unixtime(?));");
		PrepareStatement(UP_BASEINFOMINES,"update baseinfo_mines set count=?,timer=?,mine_timestamp=from_unixtime(?) where UUId=? and minesid=?");

		//missions
		PrepareStatement(SEL_BASEINFOMISSION,"select missionid,missionstatus from baseinfo_mission where UUId= ?");
		PrepareStatement(INSERT_BASEINFOMISSION,"insert into baseinfo_mission(UUId,missionid,missionstatus)values(?,?,?);");
		PrepareStatement(UP_BASEINFOMISSION,"update baseinfo_mission set missionstatus=? where UUId=? and missionid=? ");
		//monstermap
		PrepareStatement(SEL_BASEINFOMONSTERMAP,"select monsterid,monsterstatus from baseinfo_monstermap where UUId= ?");
		PrepareStatement(INSERT_BASEINFOMONSTERMAP,"insert into baseinfo_monstermap(UUId,monsterid,monsterstatus)values(?,?,?);");
		PrepareStatement(UP_BASEINFOMONSTERMAP,"update baseinfo_monstermap set monsterstatus=? where UUId=? and monsterid=? ");


		//baseinfo_platformstore
		PrepareStatement(SEL_BASEINFPLATFORMSTORE,"select level from baseinfo_platformstore where UUId= ? order by category asc");
		PrepareStatement(INSERT_BASEINFPLATFORMSTORE,"insert into baseinfo_platformstore(UUId,category,level)values(?,?,?);");
		PrepareStatement(UP_BASEINFPLATFORMSTORE,"update baseinfo_platformstore set level=? where UUId=? and category=?");
		//techupgrade
		PrepareStatement(SEL_BASEINFOTECHUPGRADE,"select techindex,techstatus from baseinfo_techupgrade where UUId= ?");
		PrepareStatement(INSERT_BASEINFOTECHUPGRADE,"insert into baseinfo_techupgrade(UUId,techindex,techstatus)values(?,?,?);");
		PrepareStatement(UP_BASEINFOTECHUPGRADE,"update baseinfo_techupgrade set techstatus=? where UUId=? and techindex=?");
		//towerupgrade
		PrepareStatement(SEL_BASEINFOTOWERUPGRADE,"select upgradevalue from baseinfo_towerupgrade where UUId=? ORDER BY towerClassIndex,towerinClassIndex");
		PrepareStatement(INSERT_BASEINFOTOWERUPGRADE,"insert into baseinfo_towerupgrade(UUId,towerClassIndex,towerinClassIndex,upgradevalue)values(?,?,?,?);");
		PrepareStatement(UP_BASEINFOTOWERUPGRADE,"update baseinfo_towerupgrade set upgradevalue=? where UUId=? and  towerClassIndex=? and towerinClassIndex=?");

		//日志
		PrepareStatement(INSERT_NETLOG,"insert into  netLog(UUId,eventType,message,timestamp)values(?,?,?,from_unixtime(?));");

}

