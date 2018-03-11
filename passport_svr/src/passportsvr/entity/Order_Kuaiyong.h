#ifndef SRC_PASSPORTSVR_ENTITY_ORDER_KUAIYONG_H_
#define SRC_PASSPORTSVR_ENTITY_ORDER_KUAIYONG_H_

class OrderKuaiyong {
public:

	/*
	 * RSA 加密的关键数据，解密后格式为：dealseq=20130219160809567&fee=0.01&payresult=0；
	 * 其中payresult 是支付结果：0：成功，-1：支付失败，-2 超时失败；dealseq 是开发商交易号；fee 是支付金额：支付成功时，
	 * 表示实际支付金额（注意：在卡类支付时，会出现订单额和支付成功额不一致情况，请开发商根据自身业务决定给用户的充值额度）
	 */
	std::string notify_data;
	/*
	 * 支付结果：0：成功，-1：支付失败，-2 超时失败
	 */
	std::string payresult;
//	/*
//	 *开发商交易号
//	 */
//	std::string dealseq;
	/*
	 *支付金额：支付成功时，表示实际支付金额
	 */
	std::string fee;

	/*
	 * 快用平台订单号
	 */
	std::string orderid;
	/*
	 * 游戏订单号（透传，唯一开发商可以自定义的参数）
	 */
	std::string dealseq;
	/*
	 * 游戏帐号,在SDK 提交此参数时，必须传入快用用户系统返回的guid，开发商不要做修改
	 */
	std::string uid;
	/*
	 * 购买物品名
	 */
	std::string subject;
	/*
	 * 版本号（固定参数=1.0）
	 */
	std::string v;
	/*
	 * RSA 签名。签名的原串，是将收到，除去sign 这个值之外的参数，根据key 值做排序组成的url 参数形式，形如右面的示例。
	 */
	std::string sign;


};

#endif
