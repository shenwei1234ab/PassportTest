/*
 * OrderXiaomi.h
 *
 *  Created on: 2015年8月17日
 *      Author: leave
 */

#ifndef SRC_SERVER_ENTITY_ORDERXIAOMI_H_
#define SRC_SERVER_ENTITY_ORDERXIAOMI_H_

class OrderXiaomi {
public:
	/*
	 *必须	游戏ID
	 */
	std::string appId;
	/*
	 *必须	开发商订单ID
	 */
	std::string cpOrderId;
	/*
	 *可选	开发商透传信息
	 */
	std::string cpUserInfo;
	/*
	 *必须	用户ID
	 */
	std::string uid;
	/*
	 *必须	游戏平台订单ID
	 */
	std::string orderId;
	/*
	 *必须	订单状态，TRADE_SUCCESS 代表成功
	 */
	std::string orderStatus;
	/*
	 *必须	支付金额,单位为分,即0.01 米币。
	 */
	std::string payFee;
	/*
	 *必须	商品代码
	 */
	std::string productCode;
	/*
	 *必须	商品名称
	 */
	std::string productName;
	/*
	 *必须	商品数量
	 */
	std::string productCount;
	/*
	 *必须	支付时间,格式 yyyy-MM-dd HH:mm:ss
	 */
	std::string payTime;
	/*
	 *可选	订单类型：10：普通订单11：直充直消订单
	 */
	std::string orderConsumeType;
	/*
	 *必选	使用游戏券金额 （如果订单使用游戏券则有,long型），如果有则参与签名
	 */
	std::string partnerGiftConsume;
	/*
	 *必须	签名,签名方法见后面说明
	 */
	std::string signature;

};

#endif /* SRC_SERVER_ENTITY_ORDERXIAOMI_H_ */
