/*
 * Order_Gionee.h
 *
 *  Created on: 2015年9月4日
 *      Author: tanghailong
 */

#ifndef SRC_PASSPORTSVR_ENTITY_ORDER_GIONEE_H_
#define SRC_PASSPORTSVR_ENTITY_ORDER_GIONEE_H_


class OrderGionee {
public:
	/*
	 * 商户API_key
	 */
	std::string api_key;
	/*
	 * 支付订单关闭时间 格式yyyyMMddHHmmss
	 */
	std::string close_time;
	/*
	 * 订单创建时间 格式yyyyMMddHHmmss
	 */
	std::string create_time;
	/*
	 * 商品总金额
	 */
	std::string deal_price;
	/*
	 * 商户订单号
	 */
	std::string out_order_no;
	/*
	 * 用户支付方式
	 */
	std::string pay_channel;
	/*
	 * 商户提交订单时间 格式yyyyMMddHHmmss
	 */
	std::string submit_time;
	/*
	 * user_id = null
	 */
	std::string user_id;
	/*
	 * 签名
	 */
	std::string sign;
};

#endif /* SRC_PASSPORTSVR_ENTITY_ORDER_GIONEE_H_ */
