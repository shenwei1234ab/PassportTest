#ifndef SRC_PASSPORTSVR_ENTITY_ORDER_XMWAN_H_
#define SRC_PASSPORTSVR_ENTITY_ORDER_XMWAN_H_

class OrderXmwan {
public:
	/*
	 * 熊猫玩消费订单序列号  参与签名
	 */
	std::string serial;
	/*
	 * 充值金额 单位为元	   参与签名
	 */
	std::string amount;
	/*
	 *  消费订单状态		   参与签名
	 * unpaid 标识尚未支付
	 *	success 标识已经支付完成
	 */
	std::string status;
	/*
	 * 应用游戏的订单号     参与签名
	 */
	std::string app_order_id;
	/*
	 * 应用游戏的用户标识   参与签名
	 */
	std::string app_user_id;
	/*
	 * 参数签名 （小写字符串）
	 */
	std::string sign;
	/*
	 * 应用游戏订单名称    如果有则参与
	 */
	std::string app_subject;
	/*
	 * 应用游戏订单描述    如果有则参与
	 */
	std::string app_description;
	/*
	 * 订单额外参数1       如果有则参与
	 */
	std::string app_ext1;
	/*
	 * 订单额外参数2      如果有则参与
	 */
	std::string app_ext2;
};

#endif


