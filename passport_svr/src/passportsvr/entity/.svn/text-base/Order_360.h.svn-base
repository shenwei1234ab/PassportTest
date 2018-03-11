#ifndef SRC_PASSPORTSVR_ENTITY_ORDER_360_H_
#define SRC_PASSPORTSVR_ENTITY_ORDER_360_H_


class Order360 {
public:
	/*
	 * 应用的appk
	 */
	std::string app_key;
	/*
	 * 应用自定义的商品id
	 */
	std::string product_id;
	/*
	 * 总价 以分做单位
	 */
	std::string amount;
	/*
	 * 应用分配给用户的id
	 */
	std::string app_uid;
	/*
	 * 应用扩展信息1原样返回
	 * 可选
	 */
	std::string app_ext1;
	/*
	 * 应用扩展信息2原样返回
	 * 可选
	 */
	std::string app_ext2;
	/*
	 * 360帐号id
	 */
	std::string user_id;
	/*
	 * 360返回的支付订单号
	 */
	std::string order_id;
	/*
	 * 如果支付成功返回success
	 * 应用需要确认是success才给用户加钱
	 */
	std::string gateway_flag;
	/*
	 * 定值md5
	 */
	std::string sign_type;
	/*
	 * 应用订单号
	 * 支付请求时传递 原样返回
	 * 可选
	 */
	std::string app_order_id;
	/*
	 * 应用回传给订单核实接口的参数
	 * 不加入签名校验计算
	 */
	std::string sign_return;
	/*
	 * 签名
	 * 不加入签名校验计算
	 */
	std::string sign;

};

#endif
