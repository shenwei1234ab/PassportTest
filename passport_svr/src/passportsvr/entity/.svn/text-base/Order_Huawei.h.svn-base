/*
 * Order_huawei.h
 *
 *  Created on: 2015年8月14日
 *      Author: leave
 */

#ifndef SRC_PASSPORTSVR_ENTITY_ORDER_HUAWEI_H_
#define SRC_PASSPORTSVR_ENTITY_ORDER_HUAWEI_H_

class OrderHuawei {
public:
	/*
	 * 支付结果：
	 “0”：支付成功
	 “1”：退款成功（暂未启用）
	 */
	std::string result;
	/*
	 * 开发者社区用户名或联盟用户编号；
	 终端sdk上报了联盟用户编号，即支付ID时，填写的内容是用户编号，反之是社区用户名；对应APK填写的userID
	 */
	std::string userName;
	/*
	 *商品名称；对应APK填写的productName
	 */
	std::string productName;
	/*
	 *支付类型:
	 0：华为钱包，
	 1：充值卡
	 2：游戏卡
	 3：信用卡
	 4：支付宝
	 16：借记卡
	 */
	std::string payType;
	/*
	 *商品支付金额 (格式为：元.角分，最小精确到分，例如：20.01)
	 注：退款通知下，为退款金额，目前仅仅支持全额退款；
	 对应APK填写的amount
	 */
	std::string amount;
	/*
	 *华为订单号，为华为支付平台生成
	 */
	std::string orderId;
	/*
	 *通知时间，自1970年1月1日0时起的毫秒数
	 */
	std::string notifyTime;
	/*
	 *开发者支付请求ID，原样返回开发者App调用支付SDK时填写的requestId参数值；对应APK填写的requestId
	 */
	std::string requestId;
	/*
	 *银行编码-支付通道信息，传递条件如下：
	  只有在有具体取值时才传递，具体取值请参考下文；
	  商户在前台sdk中指定了urlver配置项，且取值为‘2’;
	 */
	std::string bankId;
	/*
	 *下单时间 yyyy-MM-dd hh:mm:ss；
	 仅在sdk中指定了urlver为2 时有效。
	 */
	std::string orderTime;
	/*
	 *交易/退款时间 yyyy-MM-dd hh:mm:ss；
	 仅在sdk中指定了urlver为2时有效。
	 */
	std::string tradeTime;
	/*
	 *接入方式：
	 0: 移动
	 1: PC-Web
	 2: Mobile-Web
	 仅在sdk 中指定了urlver为2时有效。
	 */
	std::string accessMode;
	/*
	 *渠道开销，保留两位小数，单位元。  仅在sdk中指定了urlver为2时有效。
	 *注：本参数依赖于上游支付平台的能力，进一步的信息请联系我们获取。
	 */
	std::string spending;
	/*
	 *商户侧保留信息，原样返回商户调用支付sdk 输入的保留信息。未输入时，不返回。
	 注：考虑到自选参数格式多样化，本参数在“utf-8”urlencode后发送。
	 注：Java 中request的getParameter()方法会隐含进行urldecode。
	 */
	std::string extReserved;
	/*
	 *RSA签名。
	 注：考虑到自选参数格式多样化，本参数在“utf-8”urlencode后发送。
	 注：Java 中request的getParameter()方法会隐含进行urldecode。
	 */
	std::string sign;
};

#endif /* SRC_PASSPORTSVR_ENTITY_ORDER_HUAWEI_H_ */
