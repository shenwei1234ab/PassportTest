/*
 * Order_AppChina.h
 *
 *  Created on: 2015年9月10日
 *      Author: tanghailong
 */

#ifndef SRC_PASSPORTSVR_ENTITY_ORDER_APPCHINA_H_
#define SRC_PASSPORTSVR_ENTITY_ORDER_APPCHINA_H_

class OrderAppchina {
	/*
	 * 商户订单号
	 */
	std::string exorderno;
	/*
	 * 计费支付平台的交易流水号
	 */
	std::string transid;
	/*
	 * 平台为商户分配的唯一代码
	 */
	std::string appid;
	/*
	 * 商品编号
	 */
	int waresid;
	/*
	 * 计费方式
	 */
	int feetype;

	/*
	 * 交易金额
	 */
	int money;
	/*
	 * 购买数量
	 */
	int count;
	/*
	 * 交易结果
	 */
	int result;
	/*
	 * 交易类型 0 交易 1充值
	 */
	int transtype;
	/*
	 * 交易时间
	 */
	std::string transtime;
	/*
	 * 商户私有信息
	 */
	std::string cpprivate;
};

#endif /* SRC_PASSPORTSVR_ENTITY_ORDER_APPCHINA_H_ */
