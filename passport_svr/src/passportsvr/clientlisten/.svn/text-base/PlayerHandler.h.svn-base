/*
 * PlayerHandler.h
 *
 *  Created on: 2015年10月15日
 *      Author: leave
 */

#ifndef SRC_PASSPORTSVR_CLIENTLISTEN_PLAYERHANDLER_H_
#define SRC_PASSPORTSVR_CLIENTLISTEN_PLAYERHANDLER_H_

#include <memory>

#include "../../passportsvr/clientlisten/ClientSession.h"
#include "../../passportsvr/Protocols.h"

class ProductTemplet;

class PlayerHandler : public vn::RefCounted{
	friend  class PlayerManager;
public:
	PlayerHandler(vn::u64 ppid, vn::u32 pptoken, u32 gameId, u64 playerId);

	PlayerHandler(PlayerPtr pPlayer);

	PlayerHandler(u64 playerId);

	virtual ~PlayerHandler();

	//------------客户端申请订单号------
	void CreateOrder(pkt::req_CreateOrder* pkt);
	void doExecuteCreateOrder(bool isExecute, OrderPtr pOrder,std::shared_ptr<ProductTemplet> pProductTmpl);

	//------------客户端发送订单数据------
	void Purchase(pkt::req_Purchase* pkt);

	//------------客户端发送订单操作------
	void OrderCancel(pkt::req_OrderCancel* pkt);

	void SetClientSession(RefCountedPtr<ClientSession> value){
		_clientSession = value;
	}
	RefCountedPtr<ClientSession> GetClientSession(){
		return _clientSession;
	}

	void SetToken(u32 token);

	u64 GetPlayerId(){
		return m_playerId;
	}

	void SetPlayerPtr(PlayerPtr pPlayer){
		m_lastOptTime = time(NULL);
		m_player = pPlayer;
	}
	PlayerPtr getPlayer() {
		m_lastOptTime = time(NULL);
		return m_player;
	}
	//订单支付成功进行发货
	int DeliverGoods(vn::u64 orderId, u32 orderPrice, std::string& receipt, int os, bool isNotice = true);
protected:
	//载入订单信息
	bool loadOrderInfo(bool isforce = false);
	bool loadPlayerInfo(bool isforce = false);

protected:
	RefCountedPtr<ClientSession> _clientSession;

   u64 m_ppId;
	u32 m_tmpToken;
	u32 m_gameId;
	u64 m_playerId;//没有下单的情况下，该值无效

	PlayerPtr m_player;

	//订单信息 key：OrderId
	std::map<vn::u64, OrderPtr> m_orderMap;

	//最后一次操作的时间
	u64 m_lastOptTime = time(NULL);
};

typedef RefCountedPtr<PlayerHandler> PlayerHandlerPtr;

#endif /* SRC_PASSPORTSVR_CLIENTLISTEN_PLAYERHANDLER_H_ */
