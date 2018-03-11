/*
 * PlayerManager.h
 *
 *  Created on: 2015年10月15日
 *      Author: leave
 */

#ifndef SRC_PASSPORTSVR_MANAGER_PLAYERMANAGER_H_
#define SRC_PASSPORTSVR_MANAGER_PLAYERMANAGER_H_

#include "vnSingleton.h"
#include <mutex>
#include <map>

#include "../../passportsvr/clientlisten/PlayerHandler.h"

class PlayerManager: public vn::Singleton<PlayerManager> {
public:
	virtual ~PlayerManager();

	bool init();

	void AddPlayer(vn::u64 playerId, PlayerHandlerPtr playerHandler);

	void Disconnect(vn::u32 socketId);
	void PlayerDisconnect(vn::u64 playerId);

	void RemovePlayer(vn::u64 playerId);

	PlayerHandlerPtr GetPlayer(vn::u64 playerId);
	PlayerHandlerPtr GetPlayerBySocket(vn::u32 socketId);

	PlayerPtr GetPlayerPtr(vn::u64 playerId);
	void SetPlayerPtr(PlayerPtr pPlayer);

	//根据 ppid，pptoken 查询 playerHandler
	PlayerHandlerPtr LookforPlayerHandler(vn::u64 ppid, vn::u32 pptoken);

	//重置SocketId
	//void ResetSocketId(vn::u32 oldSocketId, vn::u32 newSocketId);

	//向客户发送信息
	bool sendMsgToPlayer(vn::u32 playerId, vn::Packet &pkt);


	//订单支付成功进行发货
	int DeliverGoods(vn::u64 orderId, u32 orderPrice, std::string& receipt, int os, bool isNotice = true);

protected:
	void _cleanTimeOutPlayer();

protected:

	std::mutex m_playerMapLock;
	//key:playerId
	std::map<vn::u64, PlayerHandlerPtr> m_playerMap;

	std::mutex m_socketMapLock;
	//key: socket
	std::map<vn::u32, PlayerHandlerPtr> m_socketMap;


};

#define sPlayerManager PlayerManager::instance()

#endif /* SRC_PASSPORTSVR_MANAGER_PLAYERMANAGER_H_ */
