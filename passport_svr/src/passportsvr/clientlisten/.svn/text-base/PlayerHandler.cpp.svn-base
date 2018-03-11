/*
 * PlayerHandler.cpp
 *
 *  Created on: 2015年10月15日
 *      Author: leave
 */

#include "../../passportsvr/clientlisten/PlayerHandler.h"

#include "../../passportsvr/Common.h"
#include "../../passportsvr/manager/OrderManger.h"
#include "../../passportsvr/manager/PlayerManager.h"
#include "../../passportsvr/manager/SessionManager.h"
#include "../../passportsvr/WorldManager.h"
#include "../database/PassPortDatabaseConnection.h"
#include "vnBase64.h"
#include "curl/CurlManager.h"
extern PassPortDatabaseWorkerPool PassPortDatabase;

PlayerHandler::PlayerHandler(vn::u64 ppid, vn::u32 pptoken, u32 gameId,
		u64 playerId) :
		m_ppId(ppid), m_tmpToken(pptoken), m_gameId(gameId), m_playerId(
				playerId) {
	loadPlayerInfo();
	loadOrderInfo();
}

PlayerHandler::PlayerHandler(PlayerPtr pPlayer) :
		m_ppId(pPlayer->ppid), m_tmpToken(0), m_gameId(pPlayer->gameid), m_playerId(
				pPlayer->playerid) {
	m_player = pPlayer;
	loadOrderInfo();
}
PlayerHandler::PlayerHandler(u64 playerId) :
		m_ppId(0), m_tmpToken(0), m_gameId(0), m_playerId(playerId) {
	loadPlayerInfo();
	if (m_player) {
		m_ppId = m_player->ppid;
		m_gameId = m_player->gameid;
	}
	loadOrderInfo();
}
PlayerHandler::~PlayerHandler() {
}

void PlayerHandler::SetToken(u32 token) {
	m_tmpToken = token;
//	loadPlayerInfo();
//	loadOrderInfo();
}

//------------客户端申请订单号------
void PlayerHandler::CreateOrder(pkt::req_CreateOrder* pkt) {
	int errorCode = 0;
	//检验产品ID
	std::shared_ptr<ProductTemplet> pProductTmpl;
	//u32 platformId =sWorldManager.getPlatformId(m_ppId) ;

	AccountPtr pAccount = sWorldManager.GetAccountInfo(m_ppId);
	if (!pAccount) {
		pkt::res_CreateOrderError res;
		res.data.errorCode = ERROR_CODE_Player_NOPlayer;
		_clientSession->sendPacket(res);
	}
	u32 platformId = pAccount->os;
	if (platformId == OS_ID_ANDROID) {
		pProductTmpl = sWorldManager.m_AndroidProductTempletMap[pkt->data.productId ];
	} else if (platformId == OS_ID_IOS) {
		pProductTmpl = sWorldManager.m_IOSProductTempletMap[pkt->data.productId ];
	}
	if (pProductTmpl == nullptr) {
		VN_LOG_ERROR("can't find productId : "<<pkt->data.productId);
		errorCode = ERROR_CODE_ORDER_NO_PRODUCT;
	}
	/*
	 //检验游戏服务器ID是否注册
	 if(!sSessionManager.isGameSvrOk(pkt->data.serverId)){
	 VN_LOG_ERROR("can't find zoneid : "<<pkt->data.serverId);
	 errorCode = ERROR_CODE_ORDER_NO_zoneid;
	 }
	 */
	vn::u32 thirdplatformId = sWorldManager.getPlatformId(pAccount->ppid);
	if (thirdplatformId == PLATFORM_ID_GUOPAN_AND || thirdplatformId == PLATFORM_ID_GUOPAN_IOS) {
		//errorCode = ERROR_CODE_PAY_CLOSE; //test for pptv
	}

	// 根据 serverId（zoneid）查找player，没有的话创建
	m_playerId = sWorldManager.getPlayerId(m_ppId,m_gameId,pkt->data.serverId);
	if (m_playerId == 0) {
		//insertPlayerRecord(pkt->data.serverId);
		errorCode = ERROR_CODE_Player_NOPlayer;
	}

	if (errorCode) {
		pkt::res_CreateOrderError res;
		res.data.errorCode = errorCode;
		_clientSession->sendPacket(res);
		return;
	}
	u64 orderid = sWorldManager.createOrderId();

	OrderPtr pOrder = new Order;
	pOrder->orderid = orderid;
	pOrder->ppid = m_ppId;
	pOrder->playerid = m_playerId;
	pOrder->gameid = m_gameId;
	pOrder->zoneid = pkt->data.serverId;
	pOrder->productid = pkt->data.productId;
	pOrder->platformid = platformId;
	pOrder->status = ORDER_STATUS_NO_PAY;
	pOrder->receipt = "";
	//生成该产品的订单id
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(
			PASSPORT_INS_ORDER);

	stmt->setUInt64(0, orderid); //orderid
	stmt->setUInt64(1, m_ppId);
	stmt->setUInt64(2, m_playerId);
	stmt->setUInt32(3, m_gameId);
	stmt->setUInt32(4, pkt->data.serverId);
	stmt->setUInt32(5, pkt->data.productId);
	stmt->setUInt32(6, platformId);
	stmt->setUInt32(7, ORDER_STATUS_NO_PAY);
	stmt->setString(8, "");
	PassPortDatabase.Execute(stmt, _clientSession,
			std::bind(&PlayerHandler::doExecuteCreateOrder, this,
					std::placeholders::_1, pOrder, pProductTmpl));
}

void PlayerHandler::doExecuteCreateOrder(bool isExecute, OrderPtr pOrder,
		std::shared_ptr<ProductTemplet> pProductTmpl) {
	if (isExecute) {
		//std::map<u64,OrderPtr>& m_pOrderMap = sOrderManger.GetOrderMap(m_ppId);
		m_orderMap[pOrder->orderid] = pOrder;
		pkt::res_CreateOrder res;
		res.data.orderId = pOrder->orderid;
		res.data.productName = pProductTmpl->productName; //todo 从模版中拿取相应的值
		res.data.price = pProductTmpl->price;
		_clientSession->sendPacket(res);
	} else {
		pkt::res_CreateOrderError res;
		res.data.errorCode = ERROR_CODE_ORDER_CREART;
		_clientSession->sendPacket(res);
	}
}

std::string CreateAppleReceipt(vn::RefCountedPtr<vn::Variable_object> vJSONPtr){
	std::string receipt;
	RefCountedPtr<Variable_object> receiptObj = vJSONPtr->queryAttributeObject("receipt");
	if (receiptObj ) {
		receipt += receiptObj->queryAttributeString("bundle_id");
		receipt += "-";
		receipt += receiptObj->queryAttributeString("receipt_creation_date_ms");
//		receipt += "-";
//		receipt += receiptObj->queryAttributeString("request_date_ms");
		receipt += "-";
		receipt += receiptObj->queryAttributeString("original_purchase_date_ms");
	}
	return std::move(receipt);
}
//------------客户端发送订单数据------
void PlayerHandler::Purchase(pkt::req_Purchase* pkt) {
	AccountPtr pAccount = sWorldManager.GetAccountInfo(m_ppId);
	if(!pAccount) {
		pkt::res_CreateOrderError res;
		res.data.errorCode = ERROR_CODE_Player_NOPlayer;
		_clientSession->sendPacket(res);
		return;
	}

	//sOrderManger.loadOrderInfo(m_ppId);
	//std::map<u64,OrderPtr>& m_pOrderMap = sOrderManger.GetOrderMap(m_ppId);
	OrderPtr pOrder (m_orderMap[pkt->data.orderId]);
	//验证订单ID是否存在
	if(pOrder == nullptr) {
		pkt::res_OrderFinished res;
		res.data.orderId = pkt->data.orderId;
		res.data.errorCode = ERROR_CODE_ORDER_NO_ORDERID;
		_clientSession->sendPacket(res);
		return;
	}
	//验证订单状态是否正常
	if(pOrder->status == ORDER_STATUS_CONFIRM) {
		pkt::res_OrderFinished res;
		res.data.orderId = pkt->data.orderId;
		res.data.errorCode = 0; //已经成功发货了
		_clientSession->sendPacket(res);
		return;
	}
	// 未发货的
	int thirdPT = sWorldManager.getPlatformId(m_ppId);

	//腾讯平台，游戏币托管，直接返回发货成功的信息
	if(thirdPT == PLATFORM_ID_QQ || thirdPT == PLATFORM_ID_WX) {
		//发货成功
		pkt::res_OrderFinished res;
		res.data.orderId = pOrder->orderid;
		res.data.errorCode = 0;
		_clientSession->sendPacket(res);
	}
	//非官服的平台
/*	if(thirdPT != PLATFORM_ID_TEST && pAccount->os != OS_ID_IOS) {
		//不回消息
		return;
	}*/
	/*	if (pOrder->status == ORDER_STATUS_NOTICE) {
	 //游戏服没有响应或者客户端重复发送
	 return;
	 }*/


	//官服发货 todo

	if (thirdPT == PLATFORM_ID_TEST && pAccount->os == OS_ID_ANDROID) {
		//android 平台 直接发货
		auto pProductTmpl = sWorldManager.m_AndroidProductTempletMap[pOrder->productid ];
		if(!pProductTmpl) {
			VN_LOG_ERROR ("DeliverGoods error ");
			return;
		}
		int result = DeliverGoods(pOrder->orderid, pProductTmpl->price * 100, pOrder->receipt,pAccount->os);
		if(result != 0) {
			VN_LOG_ERROR ("DeliverGoods error ");
		}
		return ;
	}
	/**/

	if( pAccount->os == OS_ID_IOS && (thirdPT == PLATFORM_ID_TEST || thirdPT == PLATFORM_ID_VISITOR)) {
		//苹果平台
		//检验订单凭据是否重复
	/*	for (auto pOrder : m_orderMap) {
			if (pOrder.second &&pOrder.second->status == ORDER_STATUS_CONFIRM&& pOrder.second->receipt == pkt->data.receiptData) {
				pkt::res_OrderFinished res;
				res.data.orderId = pkt->data.orderId;
				res.data.errorCode = ERROR_CODE_ORDER_REPEAT_RECEIPT;
				_clientSession->sendPacket(res);
				return;
			}
		}*/
		//去苹果检验订单凭据是否合法 todo
		//https://sandbox.itunes.apple.com/verifyReceipt
		HttpRequestPtr aReqPtr(new HttpRequestC);
		aReqPtr->addr.from_string("17.154.66.159:443");
		//aReqPtr->host = "sandbox.itunes.apple.com";
		aReqPtr->host = sWorldManager.Apple_Verify_URL;
		aReqPtr->isPost = true;
		aReqPtr->content = "/verifyReceipt";
		aReqPtr->postUri = "{\"receipt-data\":\"";
		aReqPtr->postUri += pkt->data.receiptData;
		aReqPtr->postUri +="\"}";
		//aReqPtr->lineBreak = "\r\n";
		//aReqPtr->headMap.insert( { "Content-Type", "application/json" });
		//VN_LOG_DEBUG("send content = "<<aReqPtr->content);
		aReqPtr->isHttps = true;
		RefCountedPtr<PlayerHandler> handler(this, true);
		aReqPtr->callBackFn = [handler,pOrder](const HttpResponse& _httpResponse ) {
			vn::RefCountedPtr<vn::Variable_object> vJSONPtr = _getJsonObj(_httpResponse);
			int result = -1;
			if (vJSONPtr.get()) {
				s32 status = vJSONPtr->queryAttributeInt32("status",-1);
				if (status == 0) {
					// 生成支付凭据
					pOrder->receipt = CreateAppleReceipt(vJSONPtr);
					// 验证订单
					RefCountedPtr<Variable_object> receiptObj = vJSONPtr->queryAttributeObject("receipt");
					if (receiptObj ) {
						RefCountedPtr<Variable_object> in_app = receiptObj->queryAttributeObject("in_app");
						if(in_app && in_app->size()) {
							RefCountedPtr<Variable_object> app0 = in_app->at(0).dyn_cast<Variable_object>();
							if(app0) {
								std::string product_name = app0->queryAttributeString("product_id");
								for(auto item : sWorldManager.m_IOSProductTempletMap) {
									auto pProductTmpl = item.second;
									if(pProductTmpl && product_name == pProductTmpl->productName) {
										if(pProductTmpl->productId == pOrder->productid) {
											//auto pProductTmpl = sWorldManager.m_IOSProductTempletMap[pOrder->productid];
											result = handler->DeliverGoods(pOrder->orderid, pProductTmpl->price * 100, pOrder->receipt,OS_ID_IOS);
										} else {
											VN_LOG_WARN("app receipt productid is "<< pProductTmpl->productId <<" but order productid is "<< pOrder->productid);
											result = -2;
										}
									}
								}
							}
						} else {
							//VN_LOG_WARN("app receipt productid is "<< pProductTmpl->productId <<" but order productid is "<< pOrder->productid);
							VN_LOG_WARN( "error: can't find in_app");
							result = -3;
						}
					}
				}
			}
			if(result!=0) {
				VN_LOG_ERROR ("DeliverGoods error ");
				//向客户端发送发货成功的消息
				pkt::res_OrderFinished res;
				res.data.orderId = pOrder->orderid;
				res.data.errorCode = -1;
				sPlayerManager.sendMsgToPlayer(pOrder->playerid,res);
			}
		};
		sCurlManager.CurlSend(aReqPtr);
		return;
	} else {
		//不回消息
		return;
	}
}

//------------客户端发送订单操作------
void PlayerHandler::OrderCancel(pkt::req_OrderCancel* pkt) {
	pkt::res_OrderFinished res;
	res.data.orderId = pkt->data.orderId;
	res.data.errorCode = ERROR_CODE_ORDER_CLANCEL;
	_clientSession->sendPacket(res);
}

bool PlayerHandler::loadOrderInfo(bool isforce) {
	if (!m_orderMap.empty() && !isforce) {
		return true;
	}
	m_orderMap.clear();
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(
			PASSPORT_SEL_ORDER_PPID);
	stmt->setUInt64(0, m_ppId);
	PreparedQueryResult pResult = PassPortDatabase.SynQuery(stmt);
	if (pResult && pResult->GetRowCount()) {
		do {
			OrderPtr pOrder = new Order;
			pOrder->orderid = (*pResult)[0].GetUInt64();
			pOrder->ppid = (*pResult)[1].GetUInt64();
			pOrder->playerid = (*pResult)[2].GetUInt64();
			pOrder->gameid = (*pResult)[3].GetUInt32();
			pOrder->zoneid = (*pResult)[4].GetUInt32();
			pOrder->productid = (*pResult)[5].GetUInt32();
			pOrder->platformid = (*pResult)[6].GetUInt32();
			pOrder->status = (*pResult)[7].GetUInt32();
			pOrder->receipt = (*pResult)[8].GetString();
			m_orderMap.insert(std::make_pair(pOrder->orderid, pOrder));
		} while (pResult->NextRow());
	}
	return true;
}

//订单支付成功进行发货
int PlayerHandler::DeliverGoods(vn::u64 orderId, u32 orderPrice,
		std::string& receipt, int os, bool isNotice) {
	m_lastOptTime = time(NULL);
	OrderPtr pOrder = m_orderMap[orderId];
	if (!pOrder.get()) {
		//找不到该订单
		return -1;
	}
	//std::map<u64,OrderPtr>& m_pOrderMap = GetOrderMap(pOrder->ppid);
	//m_orderMap[orderId] = pOrder;

	//验证订单状态是否正常
	if (pOrder->status == ORDER_STATUS_CONFIRM) {
		if (isNotice) {
			//向客户端发送发货成功的消息
			pkt::res_OrderFinished res;
			res.data.orderId = pOrder->orderid;
			res.data.errorCode = 0;
			sPlayerManager.sendMsgToPlayer(pOrder->playerid,res);
		}
		return 0;	//游戏服已经发货成功
	}

	std::shared_ptr<ProductTemplet> pProductTmpl;
	if (os == OS_ID_ANDROID) {
		pProductTmpl = sWorldManager.m_AndroidProductTempletMap[pOrder->productid ];
	} else if (os == OS_ID_IOS) {
		pProductTmpl = sWorldManager.m_IOSProductTempletMap[pOrder->productid ];
	}
	if (pProductTmpl == nullptr) {
		VN_LOG_ERROR("can't find productId : "<<pOrder->productid);
		return -3;
	}
	//check amount  error return -6  add by ts 2015/10/19
	if ((pProductTmpl->price * 100) != orderPrice) {
		VN_LOG_ERROR(
				"productPrice error: " << (pProductTmpl->price * 100) << ": " << orderPrice);
		return -6;
	}
	//未验证的情况
	if (pOrder->status == ORDER_STATUS_NO_PAY) {
		//检验订单凭据是否重复
		/*for (auto pOrder : m_pOrderMap) {
		 if (pOrder.second->receipt == receipt) {
		 return -4;
		 }
		 }*/
		if (!receipt.empty()) {
			pOrder->receipt = receipt;

			//订单凭据校验
			PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_SEL_ORDER_RECEIPT);
			stmt->setString(0, pOrder->receipt);
			PreparedQueryResult result = PassPortDatabase.SynQuery(stmt);
			if (result && result->GetRowCount()) {
				//找到了重复的凭据
				VN_LOG_ERROR("[ERROR] duplicate receipt . OrderId : " << pOrder->orderid);
				return -4;
			}

			// 更新订单凭据到数据库
			stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_UPD_ORDER_RECEIPT);
			stmt->setString(0, pOrder->receipt);
			stmt->setUInt64(1, pOrder->orderid);
			PassPortDatabase.Execute(stmt, nullptr, nullptr);
		}
		//更新订单状态
		pOrder->status = ORDER_STATUS_PAY;
	}
	if (pOrder->status == ORDER_STATUS_PAY) {
		if (pProductTmpl->productId != 1000 && pProductTmpl->productId != 10002) {
			//钻石
			u32 money = pProductTmpl->money;
			u32 count = sOrderManger.AddPlayerBuyCount(pOrder->playerid, pOrder->productid);
			if (count <= pProductTmpl->limitBuyCount) {
				//赠送
				money += pProductTmpl->extraValue;
			}
			//增加钻石
//			std::mutex& rPlayerLock = sWorldManager.getPlayerLock( pOrder->playerid);
//			std::lock_guard<std::mutex> lock(rPlayerLock);
			//PlayerPtm_playerer = sWorldManager.getPlayer(pOrder->playerid);
			if (m_player.get() == nullptr) {
				return -5;
			} else {
				//更新
				m_player->diamond += money;
				m_player->save_amt += pProductTmpl->money;
				m_player->first_save = 1;
				PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(
						PASSPORT_UPD_DIAMOND);
				stmt->setUInt32(0, m_player->diamond);
				stmt->setUInt32(1, m_player->gen_balance);
				stmt->setUInt32(2, m_player->first_save);
				stmt->setUInt32(3, m_player->save_amt);
				stmt->setUInt64(4, m_player->playerid);
				PassPortDatabase.Execute(stmt, nullptr, nullptr);
			}
		} else {
			//月卡
			sOrderManger.Subscribe(pOrder->playerid,std::to_string(pProductTmpl->productId),pProductTmpl->money);
		}
		pOrder->status = ORDER_STATUS_CONFIRM;
	}
	// 更新状态到数据库
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(
			PASSPORT_UPD_ORDER_STATUS);
	stmt->setUInt32(0, pOrder->status);
	stmt->setUInt64(1, pOrder->orderid);
	PassPortDatabase.Execute(stmt, nullptr, nullptr);
	if (isNotice) {
		//向客户端发送发货成功的消息
		pkt::res_OrderFinished res;
		res.data.orderId = pOrder->orderid;
		res.data.errorCode = 0;
		sPlayerManager.sendMsgToPlayer(pOrder->playerid,res);
	}
	return 0;
}

bool PlayerHandler::loadPlayerInfo(bool isforce) {
	if (m_player && !isforce) {
		return true;
	}
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(
			PASSPORT_SEL_BY_KEY);
	stmt->setUInt64(0, m_playerId);
	PreparedQueryResult pResult = PassPortDatabase.SynQuery(stmt);
	if (pResult && pResult->GetRowCount()) {
		m_player = new Player;
		m_player->playerid = (*pResult)[0].GetUInt64();
		m_player->ppid = (*pResult)[1].GetUInt64();
		m_player->gameid = (*pResult)[2].GetUInt32();
		m_player->zoneid = (*pResult)[3].GetUInt32();
		m_player->diamond = (*pResult)[4].GetUInt32();
		m_player->gen_balance = (*pResult)[5].GetUInt32();
		m_player->first_save = (*pResult)[6].GetUInt32();
		m_player->save_amt = (*pResult)[7].GetUInt32();
	} else {
		VN_LOG_ERROR("loadPlayerInfo  error! can't find playerid "<<m_playerId);
		return false;
	}
	return true;
}
