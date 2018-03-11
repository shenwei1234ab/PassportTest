/*
 * PassPortDatabaseConnection.h
 *
 *  Created on: 2015年6月10日
 *      Author: leave
 */

#ifndef SRC_DB_PASSPORTDATABASECONNECTION_H_
#define SRC_DB_PASSPORTDATABASECONNECTION_H_

#include "db/MySQLConnection.h"
#include "db/DatabaseWorkerPool.h"

class PassPortDatabaseConnection : public MySQLConnection
{
public:
	//- Constructors for sync and async connections
	PassPortDatabaseConnection(MySQLConnectionInfo& connInfo) :
			MySQLConnection(connInfo) {
	}

	//- Loads database type specific prepared statements
	void DoPrepareStatements() override;
};

typedef DatabaseWorkerPool<PassPortDatabaseConnection> PassPortDatabaseWorkerPool;

enum PassPortDatabaseStatements
{
    /*  Naming standard for defines:
        {DB}_{SEL/INS/UPD/DEL/REP}_{Summary of data changed}
        When updating more than one field, consider looking at the calling function
        name for a suiting suffix.
    */
	//账户表
	PASSPORT_INS_PPID,
	PASSPORT_SEL_PPID,
	PASSPORT_SEL_MAXPPID,
	PASSPORT_SEL_ACCOUNT_BY_KEY,
	PASSPORT_UPD_ACCOUNT_NICKNAME_BY_PPID,
	PASSPORT_UPD_ACCOUNT_AVATAR_BY_PPID,
	PASSPORT_SEL_ACCOUNT_BY_NICKNAME,
	PASSPORT_UPD_ACCOUNT_TOKEN_BY_PPID,
	PASSPORT_UPD_ACCOUNT_BING,
	PASSPORT_UPD_NICKNAME,//修改昵称
	PASSPORT_UPD_PPTOKEN,//更新pptoken和有效期
	PASSPORT_SEL_PPTOKEN,//获取pptoken和有效期


	//账户绑定表
	PASSPORT_INS_BING,
	PASSPORT_SEL_BING,

	//订单表
	PASSPORT_INS_ORDER,
	PASSPORT_SEL_ORDER_ORDERID,
	PASSPORT_SEL_ORDER_PPID,
	PASSPORT_UPD_ORDER_STATUS,
	PASSPORT_UPD_ORDER_RECEIPT,
	PASSPORT_SEL_MAXORDER,
	PASSPORT_SEL_ORDER_RECEIPT,//订单凭据校验

	//玩家表
	PASSPORT_INS_PLAYER,
	PASSPORT_SEL_PPID2,
//	PASSPORT_UPD_PPTOKEN,
//	PASSPORT_SEL_PLAYER_PPID,
	PASSPORT_SEL_MAXPLAYERID,
	PASSPORT_SEL_BY_KEY,
	PASSPORT_UPD_DIAMOND,

	//官服账户表
	PASSPORT_SEL_BY_NAME,
	PASSPORT_INS_ACCOUNT_LONGSGOO,
	PASSPORT_SEL_PPID_BY_PWD,
	PASSPORT_UPD_PWD,//修改密码

	//第三方支付记录表
	PASSPORT_INS_ORDERTHIRD,
	PASSPORT_INS_ORDERTHIRD_EX,

	//昵称表
	NICKNAME_SEL_ALL,
	NICKNAME_DEL_NAME,
	
	//脏字表
	DIRTY_SEL_ALL,

	//限购物品购买记录表
	PLAYER_BUY_SEL_PLAYER,
	PLAYER_BUY_INS_PLAYER,
	PLAYER_BUY_UPD_PLAYER,

	//充值返还记录表
	HISTORY_PAY_SEL_BY_PPID,
	HISTORY_PAY_UPD_BY_PLAYER,

	//订阅表(月卡/周卡)
	PLAYER_SUBSCRIBE_SEL_BY_PLAYERID,
	PLAYER_SUBSCRIBE_INS,
	PLAYER_SUBSCRIBE_UPD_BY_PLAYERID,
	PLAYER_SUBSCRIBE_SEL_BY_PLAYERID_2,

	//修正oppo 数据
	SELECT_OPPO_ERROR_DATA_BY_OPENID,
	DELETE_OPPO_DATA_BY_OPENID,
	UPDATE_ACCOUNT_OPENID,

	//Test
	Test_INS_Test,
	Test_SEL_ACCOUNT_ALL,
	CHAT_INS_FRIENDSLIST,

	//棱镜子渠道查询
	CHANNEL_LABEL_LJ_SEL,
	MAX_PASSPORTDATABASE_STATEMENTS,	
};

#endif /* SRC_DB_PASSPORTDATABASECONNECTION_H_ */
