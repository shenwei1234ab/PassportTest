#ifndef SRC_PASSPORTSVR_ENTITY_ORDER_PPTV_H_
#define SRC_PASSPORTSVR_ENTITY_ORDER_PPTV_H_

class OrderPptv {
public:
	/*
	 * 服务器标识  字母和数字的组合
	 */
	std::string sid;
	/*
	 * 服务器角色id，无特殊必要，可以为空
	 */
	std::string roid;
	/*
	 *	用户名 urlencode
	 */
	std::string username;
	/*
	 *  PPTV订单号
	 */
	std::string oid;
	/*
	 * 金额 元
	 */
	std::string amount;
	/*
	 * 附属信息 需要urlencode 一般情况此参数为空 不能用, | - 和其他特殊字符
	 */
	std::string extra;
	/*
	 * unix时间戳
	 */
	std::string time;
	/*
	 * 验证串 sign中的username为urlencode前的值
	 */
	std::string sign;
};

#endif
