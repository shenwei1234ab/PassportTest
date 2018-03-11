/*
 * OrderManger.h
 *
 *  Created on: 2015年8月17日
 *      Author: leave
 */

#ifndef SRC_PASSPORTSVR_MANAGER_ORDERMANGER_H_
#define SRC_PASSPORTSVR_MANAGER_ORDERMANGER_H_

#include "vnSingleton.h"
#include <map>

#include "../../passportsvr/entity/Order.h"
#include "../../passportsvr/entity/Player_Buy.h"
#include "../../passportsvr/entity/Player_Subscribe.h"
#include "common/Util.h"

class OrderManger: public vn::Singleton<OrderManger> {
public:
	//bool loadOrderInfo(vn::u64 ppId);


	OrderPtr GetOrderInfo(vn::u64 orderId);

//	std::map<vn::u64, OrderPtr>& GetOrderMap(vn::u64 ppId);
//	//订单支付成功进行发货
//	int DeliverGoods(vn::u64 orderId, std::string& receipt, int os,
//			bool isNotice = true);

	void InsertOrderTable(int pfid, int money, const std::string& pforderid,
			const std::string& cporderid, const std::string& msg);

	void InsertOrderTableEx(int pfid, int money, const std::string& pforderid,
			const std::string& cporderid, const std::string& msg,const std::string& pfid_sec,const std::string& pforderid_sec);

	//限购物品购买记录
	int AddPlayerBuyCount(vn::u64 playerid, vn::s32 productid);

	//购买月卡
	int Subscribe(vn::u64 playerid,std::string inner_productid,u32 opendays);

	std::map<std::string,PlayerSubscribePtr> GetPlayerSubscribeInfo(vn::u64 playerid);
protected:

	//std::map<vn::u64, std::map<vn::u64, OrderPtr>> m_pPPOrderMap;
};

#define sOrderManger OrderManger::instance()

#endif /* SRC_PASSPORTSVR_MANAGER_ORDERMANGER_H_ */
