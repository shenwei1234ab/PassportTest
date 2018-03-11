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

	//账户表
	PrepareStatement(PASSPORT_INS_PPID, "INSERT INTO account (ppid, platformid , openid,accesstoken,os, nickname, avatar,timestamp,isbingother) VALUES ( ?, ?, ?, ?, ?, ?, ?,current_timestamp(),?)");
	PrepareStatement(PASSPORT_SEL_PPID, "SELECT ppid, nickname, avatar,isbingother FROM account where platformid = ? and openid = ? and os = ? ");
	PrepareStatement(PASSPORT_SEL_MAXPPID, "SELECT platformid , max(ppid) FROM account group by platformid");
	PrepareStatement(PASSPORT_SEL_ACCOUNT_BY_KEY, "SELECT ppid, platformid,openid,accesstoken,os,nickname FROM account where ppid = ? ");
	PrepareStatement(PASSPORT_UPD_ACCOUNT_NICKNAME_BY_PPID, "UPDATE account SET nickname = ? WHERE ppid = ?");
	PrepareStatement(PASSPORT_UPD_ACCOUNT_AVATAR_BY_PPID, "UPDATE account SET avatar = ? WHERE ppid = ?");
	PrepareStatement(PASSPORT_SEL_ACCOUNT_BY_NICKNAME, "SELECT nickname from account where nickname = ?");
	PrepareStatement(PASSPORT_UPD_ACCOUNT_TOKEN_BY_PPID, "UPDATE account SET accesstoken = ? WHERE ppid = ?");
	PrepareStatement(PASSPORT_UPD_ACCOUNT_BING, "UPDATE account SET isbingother = 1 WHERE ppid = ?");
	PrepareStatement(PASSPORT_UPD_NICKNAME, "UPDATE account SET nickname = ? WHERE ppid = ?");
	PrepareStatement(PASSPORT_UPD_PPTOKEN, "UPDATE account SET pptoken = ?,deadline_time=from_unixtime(?) WHERE ppid = ?");
	PrepareStatement(PASSPORT_SEL_PPTOKEN, "SELECT pptoken,unix_timestamp(deadline_time) FROM account  WHERE ppid = ?");

	//账户绑定表
	PrepareStatement(PASSPORT_INS_BING,"INSERT INTO account_bing (oldppid, ppid) VALUES ( ?, ?)");
	PrepareStatement(PASSPORT_SEL_BING,"SELECT ppid, nickname, avatar FROM account WHERE ppid = (  SELECT ppid FROM account_bing WHERE oldppid = ?)");

	//订单表
	PrepareStatement(PASSPORT_INS_ORDER, "INSERT INTO order_data (orderid,ppid,playerid,gameid,zoneid,productid,platformid,status,receipt,timestamp) VALUES ( ?,?, ?, ?, ?, ?, ?, ?, ?,current_timestamp())");
	PrepareStatement(PASSPORT_SEL_ORDER_ORDERID, "SELECT orderid,ppid,playerid,gameid,zoneid,productid,platformid,status,receipt FROM order_data where orderid = ? ");
	PrepareStatement(PASSPORT_SEL_ORDER_PPID, "SELECT orderid,ppid,playerid,gameid,zoneid,productid,platformid,status,receipt FROM order_data where ppid = ? ");
	PrepareStatement(PASSPORT_UPD_ORDER_STATUS, "UPDATE  order_data SET status = ? WHERE orderid = ? ");
	PrepareStatement(PASSPORT_UPD_ORDER_RECEIPT, "UPDATE  order_data SET receipt = ? WHERE orderid = ? ");
	PrepareStatement(PASSPORT_SEL_MAXORDER, "SELECT max(orderid) FROM order_data");
	PrepareStatement(PASSPORT_SEL_ORDER_RECEIPT, "SELECT orderid FROM order_data where receipt = ? ");

	//玩家表
	PrepareStatement(PASSPORT_INS_PLAYER, "INSERT INTO player (playerid,ppid,gameid,zoneid,diamond,gen_balance,first_save,save_amt) VALUES ( ?,?,?,?,?,?,?,?)");
	PrepareStatement(PASSPORT_SEL_PPID2, "SELECT playerid,ppid,gameid,zoneid,diamond FROM player where ppid = ? and gameid = ? and zoneid = ?");
//    PrepareStatement(PASSPORT_UPD_PPTOKEN, "UPDATE  player SET pptoken = ? WHERE ppid = ? and gameid = ? and zoneid = ?");
//    PrepareStatement(PASSPORT_SEL_PLAYER_PPID, "SELECT ppid,gameid FROM player where ppid = ? ");
	PrepareStatement(PASSPORT_SEL_MAXPLAYERID, "SELECT max(playerid) FROM player");
	PrepareStatement(PASSPORT_SEL_BY_KEY, "SELECT playerid,ppid,gameid,zoneid,diamond,gen_balance,first_save,save_amt FROM player where playerid = ?");
	PrepareStatement(PASSPORT_UPD_DIAMOND, "UPDATE  player SET diamond = ?,gen_balance = ?,first_save = ?,save_amt = ? WHERE playerid = ?");

	//官服账户表
	PrepareStatement(PASSPORT_SEL_BY_NAME, "SELECT account_id,name,password from account_longsgoo where name = ?");
	PrepareStatement(PASSPORT_INS_ACCOUNT_LONGSGOO, "INSERT INTO account_longsgoo (name, password, phonenumber) VALUES (?, ?, ?)");
	PrepareStatement(PASSPORT_SEL_PPID_BY_PWD, "SELECT account.ppid FROM account,account_longsgoo WHERE account_longsgoo.`name` = account.openid AND account.platformid =1 AND account_longsgoo.`name` = ? AND account_longsgoo.`password` = ?");
	PrepareStatement(PASSPORT_UPD_PWD, "UPDATE account_longsgoo SET password = ? WHERE name = ? AND password = ?");

	//第三方支付记录表
	PrepareStatement(PASSPORT_INS_ORDERTHIRD, "INSERT INTO order_third (pfid,money,pforderid,cporderid,msg,time) VALUES ( ?,?,?,?,?,current_timestamp())");
	PrepareStatement(PASSPORT_INS_ORDERTHIRD_EX, "INSERT INTO order_third (pfid,money,pforderid,cporderid,msg,time,pfid_sec,pforderid_sec) VALUES ( ?,?,?,?,?,current_timestamp(),?,?)");

	//昵称表
	PrepareStatement(NICKNAME_SEL_ALL, "SELECT nickname from random_nickname");
	PrepareStatement(NICKNAME_DEL_NAME, "DELETE FROM random_nickname WHERE nickname = ?");

	//脏字表
	PrepareStatement(DIRTY_SEL_ALL, "SELECT  dirtyword from dirty");

	//限购物品购买记录表
	//PrepareStatement(PLAYER_BUY_SEL_PLAYER, "SELECT  id,playerid,productid,buycount from player_buy");
	PrepareStatement(PLAYER_BUY_SEL_PLAYER, "SELECT  buycount from player_buy WHERE playerid=? AND productid=?");
	PrepareStatement(PLAYER_BUY_INS_PLAYER, "INSERT INTO player_buy (playerid,productid,buycount) VALUES ( ?,?,?)");
	PrepareStatement(PLAYER_BUY_UPD_PLAYER, "UPDATE  player_buy SET buycount = buycount+1 WHERE playerid=? and productid=?");

	//充值返还记录表
	PrepareStatement(HISTORY_PAY_SEL_BY_PPID, "SELECT playerid,money,return_money from history_pay WHERE ppid = ?");
	PrepareStatement(HISTORY_PAY_UPD_BY_PLAYER, "UPDATE history_pay SET return_money=? WHERE playerid = ?");

	//订阅表(月卡/周卡)
	PrepareStatement(PLAYER_SUBSCRIBE_SEL_BY_PLAYERID, "SELECT id,playerid,inner_productid,unix_timestamp(begintime),unix_timestamp(endtime),grandtotal_opendays from player_subscribe WHERE playerid = ? and inner_productid = ?");
	PrepareStatement(PLAYER_SUBSCRIBE_INS, "INSERT INTO player_subscribe (playerid,inner_productid,begintime,endtime,grandtotal_opendays) VALUES ( ?,?,from_unixtime(?),from_unixtime(?),?)");
	PrepareStatement(PLAYER_SUBSCRIBE_UPD_BY_PLAYERID, "UPDATE  player_subscribe SET begintime = from_unixtime(?),endtime= from_unixtime(?),grandtotal_opendays= ? WHERE id=?");
	PrepareStatement(PLAYER_SUBSCRIBE_SEL_BY_PLAYERID_2, "SELECT id,playerid,inner_productid,unix_timestamp(begintime),unix_timestamp(endtime),grandtotal_opendays from player_subscribe WHERE playerid = ?");

	//OPPO 错误处理
	PrepareStatement(SELECT_OPPO_ERROR_DATA_BY_OPENID, "SELECT openid FROM oppo_error_data WHERE openid = ?");
	PrepareStatement(DELETE_OPPO_DATA_BY_OPENID, "DELETE FROM oppo_error_data WHERE openid = ?");
	PrepareStatement(UPDATE_ACCOUNT_OPENID, "UPDATE  account SET openid = ? WHERE platformid = ? and openid = ?");

	//Test
	PrepareStatement(Test_INS_Test, "INSERT INTO test (content) VALUES (?)");
	PrepareStatement(Test_SEL_ACCOUNT_ALL, "select ppid from account");

	//好友
	PrepareStatement(CHAT_INS_FRIENDSLIST, "INSERT INTO friends(playerid,friendid,flags) VALUES (?,?,?)");

	//棱镜子渠道查询
	PrepareStatement(CHANNEL_LABEL_LJ_SEL,"SELECT channelLabel,channelid FROM channelLabel_lj");
}

