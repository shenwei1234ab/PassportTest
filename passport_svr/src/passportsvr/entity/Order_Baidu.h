/*
 * Order_Baidu.h
 *
 *  Created on: 2015年9月7日
 *      Author: tanghailong
 */

#ifndef SRC_PASSPORTSVR_ENTITY_ORDER_BAIDU_H_
#define SRC_PASSPORTSVR_ENTITY_ORDER_BAIDU_H_

class OrderBaidu {
public:
	/*
	 * AppId
	 */
	int AppID;
	/*
	 *  SDK 系统内部订单号
	 */
	std::string OrderSerial;
	/*
	 *  CP 订单号
	 */
	std::string CooperatorOrderSerial;
	/*
	 *  签名
	 */
	std::string sign;
	/*
	 * 很多很多内容
	 */
	std::string Content;

};


#endif /* SRC_PASSPORTSVR_ENTITY_ORDER_BAIDU_H_ */
