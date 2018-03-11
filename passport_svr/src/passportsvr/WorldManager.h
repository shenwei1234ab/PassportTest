/*
 * WorldManager.h
 *
 *  Created on: 2015年6月13日
 *      Author: leave
 */

#ifndef SRC_PASSPORTSVR_WORLDMANAGER_H_
#define SRC_PASSPORTSVR_WORLDMANAGER_H_

#include "vnSingleton.h"
#include <map>
#include "common/Util.h"
#include <mutex>
#include <unordered_map>

#include "../passportsvr/Protocols.h"
#include "vnLink.h"

#define SESSIONKEY_LENGTH 64

class ProductTemplet {
public:
	vn::u32 productId;
	std::string productName;
	vn::u32 money;
	vn::u32 price;
	vn::u32 limitBuyCount; //限购次数
	vn::u32 extraValue; //赠送金额
};

class WorldManager: public vn::Singleton<WorldManager> {
public:
	bool loadFromDB();

	vn::u64 createPPId(vn::u32 tpId);

	vn::u32 createToken();

	vn::u64 createOrderId();

	vn::u64 createPlayerId();

	vn::u32 getPlatformId(vn::u64 ppid);

	bool loadTemplet();

	vn::u64 getPlayerId(vn::str8 ppid, vn::str8 gameid, vn::str8 zoneid);

	vn::u64 getPlayerId(vn::u64 ppid, vn::u32 gameid, vn::u32 zoneid);

	//将token插入数据库
	void updateDBToken(vn::u64 ppid,u32 pptoken);

	void SetLinkPool(vn::LinkPool *plinkPool) {
		m_linkPool = plinkPool;
	}
	vn::LinkPool *GetLinkPool() {
		return m_linkPool;
	}
	vn::u64 GetPPId(int pfid, std::string openId, int os);
	AccountPtr GetAccountInfo(vn::u64 ppid);

	void SetGamesvrListen(Link_TCP_Listen *_listen) {
		m_gamesvrListen = _listen;
	}
	Link_TCP_Listen * GetGamesvrListen() {
		return m_gamesvrListen;
	}

	bool loadBaseConfig();

	int GetProductTemplet(GAME_ID gameid,OS_ID osId,std::map<vn::u32, std::shared_ptr<ProductTemplet>>*&);
public:
	size_t m_thread_count = 0;
	u16 m_client_port = 46001;
	u16 m_admin_port = 46002;
	u16 m_gamesvr_port = 46003;
	u16 m_http_port = 46004;
	bool m_console_mode = false;
	//DB
	 str8  m_sDBInfo;
	 s32 m_dbThreads = 1;
	//游戏服地址
	struct BalanceNetAddress {
		 std::string ip;
		 u16 port;
	 };

	int m_loadBalanceNum = 0;
	std::vector<BalanceNetAddress> m_loadBalance;

	std::string m_gamesvrIP;
	u16 m_gamesvrPort = 8802;

	//******亲加即时通讯******
	std::string GOTYE_UserName;
	std::string GOTYE_Password;
	std::string GOTYE_Max_Friend_Num;
	std::string GOTYE_Appkey;

	std::string access_token = "";
	unsigned long long int expires_in = 0;

	//******腾讯沙箱环境******
	std::string QQ_APPKEY_PAY;
	std::string QQ_HOST;

	//******苹果支付验证地址******
	std::string Apple_Verify_URL;

	//http 认证端口
	u16 m_httpVerifyPort = 8080;


public:

	std::map<vn::u32, std::shared_ptr<ProductTemplet>> m_IOSProductTempletMap;
	std::map<vn::u32, std::shared_ptr<ProductTemplet>> m_AndroidProductTempletMap;

//	//天天枪战的配置文件
//	std::map<vn::u32, std::shared_ptr<ProductTemplet>> m_IOSProductTempletMap_Gun;
//	std::map<vn::u32, std::shared_ptr<ProductTemplet>> m_AndroidProductTempletMap_Gun;

	struct MoneyReturn {
		MoneyReturn(float p, bool i) :
				persent(p), isBeginReturn(i) {
		}
		float persent = 0;
		bool isBeginReturn = false;
	};
	//充值返还配置 key：平台id
	std::map<u32, MoneyReturn> m_moneyReturnInfos;

	 u64 m_index = 0;
private:
	std::map<vn::u32, vn::u64> m_MaxppIdMap;
	RandomEngine m_randomEngine;

	vn::u64 m_maxOrderId;
	vn::u64 m_maxPlayerId;

	vn::LinkPool *m_linkPool = nullptr;

	//key1:ppid, key2:gameid
	std::map<vn::u64, std::map<vn::u32, SessionDataPtr> > m_SessionDataMapMap;
	std::map<vn::u64, std::map<vn::u32, std::mutex> > m_SessionDataLockMapMap;

	Link_TCP_Listen *m_gamesvrListen = nullptr; //friendsvr，dealsvr监听器


	//下一个待分配的token
	vn::u32 m_tokenIndex = 0;

	//token 有效期
	uint64_t m_tokenLifeTime = 24*60*60;
};

#define sWorldManager WorldManager::instance()

#endif /* SRC_PASSPORTSVR_WORLDMANAGER_H_ */
