/*
 * Order_Flyme.h
 *
 *  Created on: 2015年9月6日
 *      Author: tanghailong
 */

#ifndef SRC_PASSPORTSVR_ENTITY_ORDER_FLYME_H_
#define SRC_PASSPORTSVR_ENTITY_ORDER_FLYME_H_


class OrderFlyme {
public:
	/*
	 *  通知发送的时间
	 */
	std::string notify_time;
	/*
	 * 通知id
	 */
	std::string notify_id;
	/*
	 * 订单id
	 */
	std::string order_id;
	/*
	 * 应用id
	 */
	std::string app_id;
	/*
	 * 用户id
	 */
	std::string uid;
	/*
	 * 商户id
	 */
	std::string partner_id;
	/*
	 * 游戏订单id
	 */
	std::string cp_order_id;
	/*
	 * 产品id
	 */
	std::string product_id;
	/*
	 * 产品单位 N
	 */
	std::string product_unit;
	/*
	 * 购买数量 N
	 */
	std::string buy_amount;
	/*
	 * 产品单价 N
	 */
	std::string product_per_price;
	/*
	 * 购买总价
	 */
	std::string total_price;
	/*
	 * 交易状态
	 * 1：待支付
	 * 2：支付中
	 * 3: 已经支付
	 * 4：订单取消
	 * 5：未知异常取消订单
	 */
	std::string trade_status;
	/*
	 * 订单时间
	 */
	std::string create_time;
	/*
	 * 支付时间
	 */
	std::string pay_time;
	/*
	 * 支付类型 N
	 */
	std::string pay_type;
	/*
	 * 用户自定义信息
	 */
	std::string user_info;
	/*
	 * 签名参数
	 */
	std::string sign;
	/*
	 * 签名类型 常量md5
	 */
	std::string sign_type;
};




#endif /* SRC_PASSPORTSVR_ENTITY_ORDER_FLYME_H_ */
