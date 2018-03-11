#ifndef SRC_PASSPORTSVR_ENTITY_ORDER_LENOVO_H_
#define SRC_PASSPORTSVR_ENTITY_ORDER_LENOVO_H_

class OrderLenovo {
public:
/*
 * 外部订单号 商户生成的订单号
 */
std::string exorderno;
/*
 * 交易流水号 计费支付平台的交易
 */
std::string transid;
/*
 * 游戏id open AppId
 */
std::string appid;
/*
 *  商品编码 商品编号
 */
int waresid;
/*
 * 计费方式
 * 0：按次计费
 * 1：开放价格
 */
int feetype;
/*
 * 本次交易金额， 单位：分
 */
int money;
/*
 * 本次购买的商品的数量
 */
int count;
/*
 * 交易结果
 * 0：交易成功
 * 1：交易失败
 */
int result;
/*
 * 交易类型
 * 0：交易
 * 1：冲正
 */
int transtype;
/*
 * 交易时间
 * 交易时间格式：yyyy-mm-dd hh24:mi:ss
 */
std::string transtime;
/*
 * 商户私有信息
 */
std::string cpprivate;
/*
 * 支付类型
 * 0：话费支付
 * 1：充值卡
 * 2：游戏点卡
 * 3：银行卡
 * 401：支付宝
 * 402：财付通
 * 5：联想币
 * 6 联想一键支付
 */
int payType;

};
#endif
