//
//  HttpLink.cpp
//  demo_svr
//
//  Created by Wayne on 8/12/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "../../passportsvr/httplisten/HttpLink.h"

#include "vnVariableJSON.h"

//#include "LinkManager.h"

#include "vnLog.h"
#include "vnHttpResponse.h"
#include <string.h>
#include "common/Util.h"
#include <map>

#include "../../passportsvr/Common.h"
#include "../../passportsvr/entity/Order_360.h"
#include "../../passportsvr/entity/Order_Baidu.h"
#include "../../passportsvr/entity/Order_Flyme.h"
#include "../../passportsvr/entity/Order_Gionee.h"
#include "../../passportsvr/entity/Order_Huawei.h"
#include "../../passportsvr/entity/Order_Kuaiyong.h"
#include "../../passportsvr/entity/Order_Lenovo.h"
#include "../../passportsvr/entity/Order_Oppo.h"
#include "../../passportsvr/entity/Order_Pptv.h"
#include "../../passportsvr/entity/Order_Xiaomi.h"
#include "../../passportsvr/entity/Order_Xmwan.h"
#include "../../passportsvr/manager/OrderManger.h"
#include "../../passportsvr/manager/PlayerManager.h"
#include "../../passportsvr/manager/RsaKeyManager.h"
#include "common/cryptography/RsaVerify.h"
#include "common/cryptography/RsaSign.h"
#include "common/cryptography/HmacHash.h"
#include "vnMD5.h"
#include "vnOpenSSL_Verifier.h"
#include "vnBase64.h"

#include "third/coolyun_code/CryptHelper.h"
#include "third/appchina_code/rsa.h"
#include "third/appchina_code/base64.h"
#include "third/appchina_code/md5.h"
#include "tool/xml/tinyxml.h"

void HttpLink::async(std::function<void()> fn) {
	this->post(std::move(fn));
}

void HttpLink::_init() {
	m_peer = this->peer();
	//LinkManager::instance().add(this);
	VN_LOG_DEBUG("[http] " << m_peer.to_string() << " connected.");
}

void HttpLink::_destroy() {
	//  LinkManager::instance().remove(this);
	VN_LOG_DEBUG("[http] " << m_peer.to_string() << " disconnected.");
}

void HttpLink::_disconnected() {
	this->close();
}

void HttpLink::_sent(size_t bytes, void *tag) {
	if (tag) {
		this->close();
	}
}

//if ( pos != std::string::npos && tmpStr.find(#attribute) != std::string::npos ) {
#define SET_OBJECT_ATTR(tmpStr,orderObj,attribute)\
if ( pos != std::string::npos && tmpStr.substr(0,pos) == #attribute ) {\
	orderObj.attribute = tmpStr.substr(pos + 1);\
} else

#define SET_OBJECT_ATTR_END(tmpStr) \
{\
		VN_LOG_ERROR("can't find attribute in Str:"<<tmpStr);\
}

#define CHECK_SET_OBJECT_ATTR(valueMap,orderObj,attribute)\
		if(!orderObj.attribute.empty()){\
			valueMap[#attribute] = UrlDecode(orderObj.attribute.c_str(), orderObj.attribute.length());\
		}
#define CHECK_SET_OBJECT(valueMap, orderObj, attribute) \
		if(!orderObj.attribute.empty()) { \
			valueMap[#attribute] = orderObj.attribute; \
		}
#define CHECK_SET_OBJECT2(valueMap, orderObj, attribute) \
			valueMap[#attribute] =UrlDecode(orderObj.attribute.c_str(), orderObj.attribute.length());\

#define SET_OBJECT_STRING(order, name) \
				{auto ret = attributes.find(#name);	\
				if(ret != attributes.end()) { \
					VariablePtr var = ret->second;	\
					if(var->type() == kVT_string) { \
						order.name = var.dyn_cast<Variable_string>()->value(); \
					} \
				}}
#define SET_OBJECT_INT32(order, name) \
				{auto ret = attributes.find(#name);	\
				if(ret != attributes.end()) { \
					VariablePtr var = ret->second;	\
					if(var->type() == kVT_int32) { \
						order.name = var.dyn_cast<Variable_int32>()->value(); \
					} \
				}}
#define SET_OBJECT_INT64(order, name) \
				{auto ret = attributes.find(#name);	\
				if(ret != attributes.end()) { \
					VariablePtr var = ret->second;	\
					if(var->type() == kVT_int64) { \
						order.name = var.dyn_cast<Variable_int64>()->value(); \
					} \
				}}

std::map<std::string, std::string> getValue(char* content) {
	std::map<std::string, std::string> valueMap;

	const char * split1 = "&";
	char * valueKey = strtok(content, split1);
	while (valueKey != NULL) {
		std::string tmpStr = valueKey;
		size_t pos = tmpStr.find("=");
		if (pos != std::string::npos) {
			valueMap[tmpStr.substr(0, pos)] = tmpStr.substr(pos + 1);
			VN_LOG_DEBUG("valueMap: key=" << tmpStr.substr(0,pos) << " value="<<tmpStr.substr(pos+1));
		}
		valueKey = strtok(NULL, split1);
	}
	auto it = valueMap.find("sign");
	if (it != valueMap.end()) {
		it->second = UrlDecode(it->second.c_str(), it->second.length());
	}
	it = valueMap.find("extReserved");
	if (it != valueMap.end()) {
		it->second = UrlDecode(it->second.c_str(), it->second.length());
	}
	it = valueMap.find("sysReserved");
	if (it != valueMap.end()) {
		it->second = UrlDecode(it->second.c_str(), it->second.length());
	}
	it = valueMap.find("productName");
	if (it != valueMap.end()) {
		it->second = UrlDecode(it->second.c_str(), it->second.length());
	}

	return std::move(valueMap);
}

/**
 * 将除了sign的所有请求参数按照key的ASCII码进行升序排列，然后拼接成
 * key1=value2&key2=value2&key3=value3.....
 * 如果参数为null或空，写成key1=&key2=value2&key3=value3
 * @param params 上面获取的所有请求参数
 * @return 用于签名校验的字符串
 */
std::string getSignData(std::map<std::string, std::string>& params) {
	std::string retStr = "";
	bool isNotStart = false;
	//std::map 默认是升序排列的
	std::map<std::string, std::string>::iterator itrValueMap = params.begin();
	for (; itrValueMap != params.end(); itrValueMap++) {
		if (itrValueMap->first == "sign") {
			continue;
		}
		if (isNotStart) {
			retStr += '&';
		}
		retStr += itrValueMap->first;
		retStr += '=';
		retStr += itrValueMap->second;
		isNotStart = true;
	}VN_LOG_DEBUG("getSignData: " << retStr);
	return std::move(retStr);
}

/*
 * 小米获取验签代码
 *
 * 生成带签名字符串
 * 表中各参数按字母顺序排序(不包含 signature),如果第一个字母相同,按第二个字母排序,依次类推。
 * 排序后拼接成par1=val1&par2=val2&par3=val3的格式,所生成的字符串即为待签名的字符串。
 * 没有值的参数请不要参与签名。
 * 由于有些数据根据HTTP协议需求,需要进行URLencoding,这样接收方才可以
 * 接收到正确的参数,但如果这个参数参与签名,那么待签名字符串必须是字符串原值而非URLencoding 的值。
 */
std::string getXiaomiValue(OrderXiaomi& orderXiaomi) {
	std::map<std::string, std::string> valueMap;
//	if(orderXiaomi.appId.empty()){
//		valueMap["appId"] = orderXiaomi.appId;
//	}

	CHECK_SET_OBJECT_ATTR(valueMap, orderXiaomi, appId)
	CHECK_SET_OBJECT_ATTR(valueMap, orderXiaomi, cpOrderId)
	CHECK_SET_OBJECT_ATTR(valueMap, orderXiaomi, cpUserInfo)
	CHECK_SET_OBJECT_ATTR(valueMap, orderXiaomi, uid)
	CHECK_SET_OBJECT_ATTR(valueMap, orderXiaomi, orderId)
	CHECK_SET_OBJECT_ATTR(valueMap, orderXiaomi, orderStatus)
	CHECK_SET_OBJECT_ATTR(valueMap, orderXiaomi, payFee)
	CHECK_SET_OBJECT_ATTR(valueMap, orderXiaomi, productCode)
	CHECK_SET_OBJECT_ATTR(valueMap, orderXiaomi, productName)
	CHECK_SET_OBJECT_ATTR(valueMap, orderXiaomi, productCount)
	CHECK_SET_OBJECT_ATTR(valueMap, orderXiaomi, payTime)
	CHECK_SET_OBJECT_ATTR(valueMap, orderXiaomi, orderConsumeType)
	CHECK_SET_OBJECT_ATTR(valueMap, orderXiaomi, partnerGiftConsume)
	/*
	 valueMap["appId"] = orderXiaomi.appId;
	 valueMap["cpOrderId"] = orderXiaomi.cpOrderId;
	 valueMap["cpUserInfo"] = orderXiaomi.cpUserInfo;
	 valueMap["uid"] = orderXiaomi.uid;
	 valueMap["orderId"] = orderXiaomi.orderId;
	 valueMap["orderStatus"] = orderXiaomi.orderStatus;
	 valueMap["payFee"] = orderXiaomi.payFee;
	 valueMap["productCode"] = orderXiaomi.productCode;
	 valueMap["productName"] = orderXiaomi.productName;
	 valueMap["productCount"] = orderXiaomi.productCount;
	 valueMap["payTime"] = orderXiaomi.payTime;
	 valueMap["orderConsumeType"] = orderXiaomi.orderConsumeType;
	 valueMap["partnerGiftConsume"] = orderXiaomi.partnerGiftConsume;*/

	std::string retStr = "";
	bool isStart = true;
	//std::map 默认是升序排列的
	std::map<std::string, std::string>::iterator itrValueMap = valueMap.begin();
	for (; itrValueMap != valueMap.end(); itrValueMap++) {
		if (itrValueMap->first == "signature") {
			continue;
		}
		if (itrValueMap->second == "") {
			continue;
		}
		if (!isStart) {
			retStr += '&';
		}
		retStr += itrValueMap->first;
		retStr += '=';
		retStr += itrValueMap->second;
		isStart = false;
	}VN_LOG_DEBUG("getXiaomiValue: " << retStr);

	return std::move(retStr);
}

void HttpLink::_received(LinkBufferPtr buffers) {
	auto tb = buffers;
	do {
		VN_LOG_DEBUG("\n" << str8((const char*)tb->lock(), tb->size()));
		tb = tb->next();
	} while (tb);
	m_parser.append(std::move(buffers));
	for (;;) {
		auto ret = m_parser.fetch();
		if (ret == HttpRequestParser::kError) {
			VN_LOG_DEBUG("[http] " << m_peer.to_string() << " request error.");
			this->close();
			break;
		} else if (ret == HttpRequestParser::kWait) {
			break;
		} else {
			VN_LOG_DEBUG("[http] " << m_peer.to_string() << " request:\nURL: " << m_parser.result().url() << "\nHost: " << m_parser.result().host());
			std::string urlStr = UrlDecode(m_parser.result().url().c_str(),
					m_parser.result().url().length());
			//获取 GET 参数
			const char * split = "/";
			char * p;
			p = strtok((char*) urlStr.c_str(), split);
			int iPfid = 0;
			while (p != NULL) {
				std::string tmpStr = p;
				size_t pos = tmpStr.find("=");

				if (tmpStr.find("verify") != std::string::npos){
					//登录验证逻辑
					doVerify();
					return;
				}

				if (tmpStr.find("pfid") != std::string::npos
						&& pos != std::string::npos) {
					std::string pfid = tmpStr.substr(pos + 1);
					//VN_LOG_DEBUG ("find pfid.c_str()="<<pfid.c_str());
					iPfid = atoi(pfid.c_str());
					if (iPfid == 0) {
						// 错误的平台id
						VN_LOG_ERROR("错误的平台id："<<pfid.c_str());
					}VN_LOG_DEBUG ("find iPfid="<<iPfid);
					break;
				}
				p = strtok(NULL, split);
			}
			try {
				switch (iPfid) {
				case PLATFORM_ID_TEST: { //官服
					doTestReq();
					break;
				}
				case PLATFORM_ID_HUAWEI: { //华为
					doHuaweiReq();
					break;
				}
				case PLATFORM_ID_XIAOMI: { //小米
					doXiaomiReq();
					break;
				}
				case PLATFORM_ID_LENOVO: { //联想
					doLenovoReq();
					break;
				}
				case PLATFORM_ID_KUAIYONG: { //快用苹果助手
					doKuaiyongReq();
					break;
				}
				case PLATFORM_ID_360: {
					do360Req();
					break;
				}
				case PLATFORM_ID_UC: {
					doUCReq();
					break;
				}
				case PLATFORM_ID_ANZHI: {
					doAnzhiReq();
					break;
				}
				case PLATFORM_ID_XMW: {
					doXmwanReq();
					break;
				}
				case PLATFORM_ID_BAIDU: {
					doBaiduReq();
					break;
				}
				case PLATFORM_ID_4399: {
					do4399Req();
					break;
				}
				case PLATFORM_ID_OPPO: {
					doOppoReq();
					break;
				}
				case PLATFORM_ID_PPTV: {
					doPptvReq();
					break;
				}
				case PLATFORM_ID_GIONEE: {
					doGioneeReq();
					break;
				}
				case PLATFORM_ID_FLYME: {
					doFlymeReq();
					break;
				}
				case PLATFORM_ID_APPCHINA: {
					doAppchinaReq();
					break;
				}
				case PLATFORM_ID_XY: {
					doXYReq();
					break;
				}
				case PLATFORM_ID_19196: {
					do19196Req();
					break;
				}
				case PLATFORM_ID_WANDOUJIA: {
					doWandoujiaReq();
					break;
				}
				case PLATFORM_ID_MUZHIWAN: {
					doMuzhiwanReq();
					break;
				}
				case PLATFORM_ID_DOWNJOY: {
					doDownjoyReq();
					break;
				}
				case PLATFORM_ID_COOLYUN: {
					doCoolyunReq();
					break;
				}
				case PLATFORM_ID_CCPLAY: {
					doCcplayReq();
					break;
				}
				case PLATFORM_ID_I4: {
					doI4Req();
					break;
				}
				case PLATFORM_ID_TBT: {
					doTBTReq();
					break;
				}
				case PLATFORM_ID_VIVO: {
					doVivoReq();
					break;
				}
				case PLATFORM_ID_HAIMA_AND: {
					doHMW_AND_Req();
					break;
				}
				case PLATFORM_ID_HAIMA_IOS: {
					doHMW_IOS_Req();
					break;
				}
				case PLATFORM_ID_SOGOU: {
					doSougouReq();
					break;
				}
				case PLATFORM_ID_SINA: {
					doSinaReq();
					break;
				}
				case PLATFORM_ID_GFAN: {
					doGfanReq();
					break;
				}
				case PLATFORM_ID_YOUKU: {
					doYoukuReq();
					break;
				}
				case PLATFORM_ID_IAPPLE: {
					doIAppleReq();
					break;
				}
				case PLATFORM_ID_QIYPPS: {
					doPpsReq();
					break;
				}
				case PLATFORM_ID_GUOPAN_AND: {
					doGuoPanAndReq();
					break;
				}
				case PLATFORM_ID_GUOPAN_IOS: {
					doGuoPanIOSReq();
					break;
				}
				case PLATFORM_ID_PP_IOS: {
					doPPIOSReq();
					break;
				}
				case PLATFORM_ID_XINMEI365: {
					doXINMEI365Req();
					break;
				}
				default:
					VN_LOG_DEBUG ("can't find pfid:"<<iPfid);
					break;
				}

			} catch (std::exception& e) {
				VN_LOG_DEBUG ("[Excepction] in Pfid:"<<iPfid << " Reason:"<<e.what());
			}
			/*
			 HttpResponse res;
			 res.set_status(200);
			 res.set_status_text("OK");
			 res.set_content_type("application/json");
			 str8 content = "{\"url\":\"";
			 content += m_parser.result().url();
			 content += "\"}";
			 res.set_content(vnnew ProxyBuffer((void *)content.c_str(), content.length()));
			 this->send(res.pack((void *)(long)0));*/
		}
	}
}
void HttpLink::doHuaweiReq() {
	//POST请求，json格式
	VN_LOG_DEBUG ("HttpRequest content : \n"<<m_parser.result().content());
	const BufferPtr & contet = m_parser.result().content();
	if (!contet) {
		VN_LOG_DEBUG ("contet error!");
		return;
	}
	void * str = contet->lock();
	std::string contentStr((char*) str, contet->size());
	const char * split = "?&";
	char * p = strtok((char*) str, split);
	OrderHuawei orderHuawei;
	while (p != NULL) {
		std::string tmpStr = p;
		size_t pos = tmpStr.find("=");
		//						if (tmpStr.find("result") != std::string::npos && pos != std::string::npos) {
		//							orderHuawei.result = tmpStr.substr(pos + 1);
		//						}
		SET_OBJECT_ATTR(tmpStr,orderHuawei,result)
		SET_OBJECT_ATTR(tmpStr,orderHuawei,userName)
		SET_OBJECT_ATTR(tmpStr,orderHuawei,productName)
		SET_OBJECT_ATTR(tmpStr,orderHuawei,payType)
		SET_OBJECT_ATTR(tmpStr,orderHuawei,amount)
		SET_OBJECT_ATTR(tmpStr,orderHuawei,orderId)
		SET_OBJECT_ATTR(tmpStr,orderHuawei,notifyTime)
		SET_OBJECT_ATTR(tmpStr,orderHuawei,requestId)
		SET_OBJECT_ATTR(tmpStr,orderHuawei,bankId)
		SET_OBJECT_ATTR(tmpStr,orderHuawei,orderTime)
		SET_OBJECT_ATTR(tmpStr,orderHuawei,tradeTime)
		SET_OBJECT_ATTR(tmpStr,orderHuawei,accessMode)
		SET_OBJECT_ATTR(tmpStr,orderHuawei,spending)
		SET_OBJECT_ATTR(tmpStr,orderHuawei,extReserved)
		SET_OBJECT_ATTR(tmpStr,orderHuawei,sign)
			SET_OBJECT_ATTR_END(tmpStr)
		p = strtok(NULL, split);
	}
	contet->unlock();

	orderHuawei.sign = UrlDecode(orderHuawei.sign.c_str(),
			orderHuawei.sign.length());

	// 订单支付成功进行发货
	//u64 pfOrderId = atol(orderHuawei.orderId.c_str());
	u64 cpOrderId = atol(orderHuawei.requestId.c_str());
	int os = OS_ID_ANDROID;
	//订单记入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_HUAWEI,atof(orderHuawei.amount.c_str())*100,orderHuawei.orderId, orderHuawei.requestId,contentStr);

	int result = 0;
	//todo 验签
	std::map<std::string, std::string> valueMap = getValue(
			(char*) contentStr.c_str());
	std::string sSignData = getSignData(valueMap);
//	RsaVerifySha1 rsaVerify;
//	rsaVerify.UpdateData(sSignData);
//	bool isRight = rsaVerify.Finalize(HUAWEI_PubKey,orderHuawei.sign);

	OpenSSL_KeyPtr key = sRsaKeyManager.GetRsakey(PLATFORM_ID_HUAWEI);//OpenSSL_Key::create_public(HUAWEI_PubKey.data(), HUAWEI_PubKey.length());
	//OpenSSL_KeyPtr key = OpenSSL_Key::create_public(HUAWEI_PubKey_TEST.data(),HUAWEI_PubKey_TEST.length());
	OpenSSL_Verifier verigier;
	verigier.write(sSignData.data(), sSignData.length());
	bool isRight = verigier.verify_base64(key, orderHuawei.sign);
	if (isRight) {
		//发货
		result = sPlayerManager.DeliverGoods(cpOrderId, atof(orderHuawei.amount.c_str())*100, orderHuawei.orderId,os);

		if(result == 0) {
			//ok
			VN_LOG_ERROR ("DeliverGoods ok ");
		} else {
			//发货失败
			VN_LOG_ERROR ("DeliverGoods error:"<<result);
			if(result == -1) {	//订单不存在
				result = 3;//3:业务信息错误，比如订单不存在,
			} else if(result == -5) { //用户不存在
				result = 98;//98:参数错误,
			} else if(result == -4) {
				result = 0; //对于重复的订单，要求返回0
			} else { // -2,-3,-4
				result = 94;//94:系统错误,
			}
		}
	} else {
		VN_LOG_ERROR ("OpenSSL_Verifier ERROR!!! ");
		result = 1; //验签失败
	}

	HttpResponse res;
//	res.set_status(201);
//	res.set_status_text("Created");
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content_type("application/json");
	str8 content = "{\"result\":";
	content += std::to_string(result);
	content += "}";
	VN_LOG_DEBUG ("content = "<<content);
	res.set_content(
	vnnew ProxyBuffer((void *) content.c_str(), content.length()));
	this->send(res.pack((void *) (long) 0));
	return;

}

void HttpLink::doTestReq() {
	VN_LOG_DEBUG(m_parser.result().url());
	//HTTP POST
	const BufferPtr & contet = m_parser.result().content();
	if (!contet) {
		VN_LOG_DEBUG ("contet error!");
		return;
	}
	std::string contentStr = (char *) contet->lock();


	auto paramMap = GetURLParameter(contentStr, true);
	//订单计入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_TEST, atof(paramMap["total_fee"].c_str()) * 100, paramMap["trade_no"], paramMap["out_trade_no"], (char *) contet->lock());
	if (paramMap["trade_status"] != "TRADE_SUCCESS") {
		return;
	}
	std::string basestring = "";
	for (auto &i : paramMap) {
		if (i.first == "sign" || i.first == "sign_type")
			continue;
		if (i.second.empty())
			continue;
		basestring += i.first + "=" + i.second + "&";
	}
	basestring.erase(basestring.end() - 1);
	std::string content = "fail";
	VN_LOG_DEBUG ("sign:" << paramMap["sign"]); VN_LOG_DEBUG ("basestring:"<<basestring);
	//签名
	OpenSSL_KeyPtr key = sRsaKeyManager.GetRsakey(PLATFORM_ID_TEST);
	OpenSSL_Verifier verigier;
	verigier.write(basestring.data(), basestring.length());
	bool ret = verigier.verify_base64(key, paramMap["sign"]);

	if (ret) {
		int os = OS_ID_ANDROID;
		int result = sPlayerManager.DeliverGoods( atol(paramMap["out_trade_no"].c_str()), atof(paramMap["total_fee"].c_str()) * 100, paramMap["trade_no"], os, true);
		if (result == 0) {
			VN_LOG_DEBUG ("DeliverGoods ok:");
			content = "success";
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
		}
	}

	HttpResponse res;
	res.set_content_type("text/html;charset=UTF-8;");
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content(
	vnnew ProxyBuffer((void*) content.c_str(), content.length()));
	this->send(res.pack((void*) (long) 0));
}

void HttpLink::doXiaomiReq() {
	/*
	 * 接口地址:请随《游戏SDK申请表》一起提交
	 * 请求方法:GET
	 */
	VN_LOG_DEBUG ("HttpRequest content : \n"<<m_parser.result().url());
	//const BufferPtr & contet = m_parser.result().url();
	std::string urlStr(m_parser.result().url().c_str(),
			m_parser.result().url().length());
	char * str = (char *) urlStr.c_str();
	const char * split = "?&";
	char * p = strtok(str, split);
	OrderXiaomi orderXiaomi;
	while (p != NULL) {
		//VN_LOG_DEBUG ("GET args:"<<p);
		std::string tmpStr = p;
		size_t pos = tmpStr.find("=");
		SET_OBJECT_ATTR(tmpStr,orderXiaomi,appId)
		SET_OBJECT_ATTR(tmpStr,orderXiaomi,cpOrderId)
		SET_OBJECT_ATTR(tmpStr,orderXiaomi,cpUserInfo)
		SET_OBJECT_ATTR(tmpStr,orderXiaomi,uid)
		SET_OBJECT_ATTR(tmpStr,orderXiaomi,orderId)
		SET_OBJECT_ATTR(tmpStr,orderXiaomi,orderStatus)
		SET_OBJECT_ATTR(tmpStr,orderXiaomi,payFee)
		SET_OBJECT_ATTR(tmpStr,orderXiaomi,productCode)
		SET_OBJECT_ATTR(tmpStr,orderXiaomi,productName)
		SET_OBJECT_ATTR(tmpStr,orderXiaomi,productCount)
		SET_OBJECT_ATTR(tmpStr,orderXiaomi,payTime)
		SET_OBJECT_ATTR(tmpStr,orderXiaomi,orderConsumeType)
		SET_OBJECT_ATTR(tmpStr,orderXiaomi,partnerGiftConsume)
		SET_OBJECT_ATTR(tmpStr,orderXiaomi,signature)
			SET_OBJECT_ATTR_END(tmpStr)
		p = strtok(NULL, split);
	}

	//u64 orderId = atol(orderXiaomi.orderId.c_str());
	u64 cpOrderId = atol(orderXiaomi.cpOrderId.c_str());

	//订单记入数据库
	//sOrderManger.InsertOrderTable(PLATFORM_ID_XIAOMI,atoi(orderXiaomi.payFee.c_str()),orderId, cpOrderId,m_parser.result().url());
	sOrderManger.InsertOrderTable(PLATFORM_ID_XIAOMI, atoi(orderXiaomi.payFee.c_str()), orderXiaomi.orderId, orderXiaomi.cpOrderId, m_parser.result().url());
	int result = 0;
	// 验签
	std::string SingCnt = getXiaomiValue(orderXiaomi);
	HmacSha1 hmac((uint32) XIAOMI_AppSecret.length(),
			(uint8 const*) XIAOMI_AppSecret.c_str());
	hmac.UpdateData(SingCnt);
	hmac.Finalize();
	std::string hmacSing = ByteArrayToHexStr(hmac.GetDigest(),
			hmac.GetLength());
	VN_LOG_DEBUG ("hmacSing:"<<hmacSing);
	if (hmacSing == orderXiaomi.signature) {
		// 订单支付成功进行发货
		int os = OS_ID_ANDROID;
		result = sPlayerManager.DeliverGoods(cpOrderId, atoi(orderXiaomi.payFee.c_str()), orderXiaomi.orderId,os);
		if (result != 0) {
			//发货失败
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
			if (result == -1) {
				result = 1506;
			} else if (result == -5) {
				result = 1516;
			} else if (result == -4) {
				result = 0;	//对于重复的订单，要求返回0
			} else { // -2,-3,-4
				result = 3515; // -2,-3,-4
			}
		}
	} else {
		result = 1525;
	}
	/*
	 * 200 成功
	 *	1506 cpOrderId 错误
	 *	1515 appId 错误
	 *	1516 uid 错误
	 *	1525 signature 错误
	 *	3515 订单信息不一致，用于和 CP 的订单校验
	 */
	if (result == 0) {
		result = 200;
	}
	HttpResponse res;
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content_type("application/json");
	str8 content = "{\"errcode\":";
	content += std::to_string(result);
	content += "}";
	VN_LOG_DEBUG ("content = "<<content);
	res.set_content(
	vnnew ProxyBuffer((void *) content.c_str(), content.length()));
	this->send(res.pack((void *) (long) 0));
	return;
}

void HttpLink::doLenovoReq() {
	//请求方法:POST
	const BufferPtr & contet = m_parser.result().content();
	if (!contet) {
		VN_LOG_DEBUG ("contet error!");
		return;
	}
	std::string contentStr = (char *) contet->lock();

	std::map<std::string, std::string> valueMap;
	const char * split1 = "&";
	char * valueKey = strtok((char *) contentStr.c_str(), split1);
	while (valueKey != NULL) {
		std::string tmpStr = valueKey;
		size_t pos = tmpStr.find("=");
		if (pos != std::string::npos) {
			std::string value = tmpStr.substr(pos + 1);
			valueMap[tmpStr.substr(0, pos)] = UrlDecode(value.c_str(),
					value.length());
			VN_LOG_DEBUG("valueMap: key=" << tmpStr.substr(0,pos) << " value="<<tmpStr.substr(pos+1));
		}
		valueKey = strtok(NULL, split1);
	}
	/*
	 std::string tmpStr = strtok ((char*)str,split);
	 size_t pos = tmpStr.find("=");
	 std::string transdata = tmpStr.substr(pos + 1);
	 tmpStr = std::strtok(NULL, split);
	 pos = tmpStr.find("=");
	 std::string sign = tmpStr.substr(pos + 1);
	 */
	std::string transdata = valueMap["transdata"];
	std::string sign = valueMap["sign"];
	vn::VariableJSON variableJSON;
	RefCountedPtr<Variable_object> JSONPtr = variableJSON.parse(
			transdata.c_str(), transdata.length());
	if (!JSONPtr.get()) {
		return;
	}
	auto attributes = JSONPtr->attributes();
	OrderLenovo orderLenovo;

	SET_OBJECT_STRING(orderLenovo, exorderno)
	SET_OBJECT_STRING(orderLenovo, transid)
	SET_OBJECT_STRING(orderLenovo, appid)
	SET_OBJECT_INT32(orderLenovo, waresid)
	SET_OBJECT_INT32(orderLenovo, feetype)
	SET_OBJECT_INT32(orderLenovo, money)
	SET_OBJECT_INT32(orderLenovo, count)
	SET_OBJECT_INT32(orderLenovo, result)
	SET_OBJECT_INT32(orderLenovo, transtype)
	SET_OBJECT_STRING(orderLenovo, transtime)
	SET_OBJECT_STRING(orderLenovo, cpprivate)
	//私有订单号 第三方平台的openId
//		size_t pos = orderLenovo.cpprivate.find(",");
//		std::string uid = orderLenovo.cpprivate.substr(0, pos);
//		std::string cpOrder = orderLenovo.cpprivate.substr(pos + 1);
	u64 cpOrderId = atol(orderLenovo.exorderno.c_str());

	//订单记入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_LENOVO, orderLenovo.money, orderLenovo.transid, orderLenovo.exorderno, std::string((char*)contet->lock()));

	//验签
//		RsaVerifySha1 rsa;
//		rsa.UpdateData(transdata);
//		bool ret = rsa.Finalize(LENOVO_AppKey, sign);
	//OpenSSL_KeyPtr key = OpenSSL_Key::create_private(LENOVO_AppKey.data(), LENOVO_AppKey.length());
	OpenSSL_KeyPtr key = sRsaKeyManager.GetRsakey(PLATFORM_ID_LENOVO);
	OpenSSL_Verifier verigier;
	verigier.write(transdata.data(), transdata.length());
	bool ret = verigier.verify_base64(key, sign);

	std::string content;
	int result = 0;
	if (ret) {
		content = "SUCCESS";
		int os = OS_ID_ANDROID;
		//无用户ID  用商户私有信息代替
		result = sPlayerManager.DeliverGoods( cpOrderId, orderLenovo.money, orderLenovo.exorderno,os);
		if (result != 0) {
			//发货失败
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
			if (result == -1) {
				result = 1506;
			} else if (result == -5) {
				result = 1516;
			} else if (result == -4) {
				result = 0; //对于重复的订单，要求返回0
			} else { // -2,-3,-4
				result = 3515; // -2,-3,-4
			}
		}
	} else {
		content = "FAILTURE";
	}
	contet->unlock();

	HttpResponse res;
	res.set_content_type("text/html;charset=UTF-8;");
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content(
	vnnew ProxyBuffer((void*) content.c_str(), content.length()));
	this->send(res.pack((void*) (long) 0));
}

void HttpLink::do360Req() {
	//请求方法 GET
	VN_LOG_DEBUG ("HttpRequest content : \n"<<m_parser.result().url());
	std::string str = m_parser.result().url().c_str();
	const char * split = "?&";
	char * p = strtok((char*) str.c_str(), split);
	Order360 order360;
	while (p != NULL) {			//VN_LOG_DEBUG ("GET args:"<<p);
		std::string tmpStr = p;
		size_t pos = tmpStr.find("=");
		SET_OBJECT_ATTR(tmpStr,order360, app_key)
		SET_OBJECT_ATTR(tmpStr,order360, product_id)
		SET_OBJECT_ATTR(tmpStr,order360, amount)
		SET_OBJECT_ATTR(tmpStr,order360, app_uid)
		SET_OBJECT_ATTR(tmpStr,order360, app_ext1) //参选
		SET_OBJECT_ATTR(tmpStr,order360, app_ext2) //参选
		SET_OBJECT_ATTR(tmpStr,order360, user_id)
		SET_OBJECT_ATTR(tmpStr,order360, gateway_flag)
		SET_OBJECT_ATTR(tmpStr,order360, sign_type)
		SET_OBJECT_ATTR(tmpStr,order360, app_order_id) //参选
		SET_OBJECT_ATTR(tmpStr,order360, sign_return) //不参与签名
		SET_OBJECT_ATTR(tmpStr,order360, sign)			 //不参与签名
		SET_OBJECT_ATTR(tmpStr,order360, order_id)
			SET_OBJECT_ATTR_END(tmpStr)
		p = strtok(NULL, split);
	}

	//订单记入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_360, atoi(order360.amount.c_str()), order360.order_id, order360.app_order_id, m_parser.result().url());

	//验签
	std::map<std::string, std::string> valueMap;
	CHECK_SET_OBJECT_ATTR(valueMap, order360, app_key);
	CHECK_SET_OBJECT_ATTR(valueMap, order360, product_id);
	CHECK_SET_OBJECT_ATTR(valueMap, order360, amount);
	CHECK_SET_OBJECT_ATTR(valueMap, order360, app_uid);
	CHECK_SET_OBJECT_ATTR(valueMap, order360, app_ext1);
	CHECK_SET_OBJECT_ATTR(valueMap, order360, app_ext2);
	CHECK_SET_OBJECT_ATTR(valueMap, order360, user_id);
	CHECK_SET_OBJECT_ATTR(valueMap, order360, gateway_flag);
	CHECK_SET_OBJECT_ATTR(valueMap, order360, sign_type);
	CHECK_SET_OBJECT_ATTR(valueMap, order360, app_order_id);
	CHECK_SET_OBJECT_ATTR(valueMap, order360, order_id);
	std::string signValue = "";
	std::map<std::string, std::string>::iterator itrValue = valueMap.begin();
	for (; itrValue != valueMap.end(); itrValue++) {
		if (itrValue->second.empty())
			continue;
		signValue += itrValue->second + "#";
	}
	signValue += AppSecret_360; //加上 app_secret
	vn::MD5 md5;
	md5.write(signValue.c_str(), signValue.length());
	std::string sign_calc = md5.fetch_hex_lowercase();
	std::string content = "error";
	order360.sign = UrlDecode(order360.sign.c_str(), order360.sign.length());
	if (sign_calc == order360.sign) {
		//sign success
		//发货
		int os = OS_ID_ANDROID;
		int result = sPlayerManager.DeliverGoods( atol(order360.app_order_id.c_str()), atoi(order360.amount.c_str()), order360.order_id,os,true);
		if (result == 0) {
			VN_LOG_DEBUG ("DeliverGoods ok:");
			content = "ok";
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
			content = "error";
		}
	} else {
		VN_LOG_ERROR("sign error!");
	}

	HttpResponse res;
	res.set_content_type("text/html;charset=UTF-8;");
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content(
	vnnew ProxyBuffer((void*) content.c_str(), content.length()));
	this->send(res.pack((void*) (long) 0));

}

void HttpLink::doKuaiyongReq() {
	const BufferPtr & contet = m_parser.result().content();
	if (!contet) {
		VN_LOG_DEBUG ("contet error!");
		return;
	}
	void * str = contet->lock();
	std::string contentStr((char*) str);

	VN_LOG_DEBUG ("HttpRequest content : "<<contentStr);
	const char * split = "?&";
	char * p = strtok((char*) str, split);
	OrderKuaiyong orderKuaiyong;
	while (p != NULL) {
		std::string tmpStr = p;
		size_t pos = tmpStr.find("=");
		//						if (tmpStr.find("result") != std::string::npos && pos != std::string::npos) {
		//							orderKuaiyong.result = tmpStr.substr(pos + 1);
		//						}
		//VN_LOG_DEBUG ("tmpStr : "<<tmpStr);
		SET_OBJECT_ATTR(tmpStr,orderKuaiyong,sign)
		SET_OBJECT_ATTR(tmpStr,orderKuaiyong,notify_data)
		SET_OBJECT_ATTR(tmpStr,orderKuaiyong,orderid)
		SET_OBJECT_ATTR(tmpStr,orderKuaiyong,dealseq)
		SET_OBJECT_ATTR(tmpStr,orderKuaiyong,uid)
		SET_OBJECT_ATTR(tmpStr,orderKuaiyong,subject)
		SET_OBJECT_ATTR(tmpStr,orderKuaiyong,v)
			SET_OBJECT_ATTR_END(tmpStr)
		p = strtok(NULL, split);
	}
	contet->unlock();

	//OpenSSL_KeyPtr key = OpenSSL_Key::create_public(KUAIYONG_PublicKey.data(), KUAIYONG_PublicKey.length());
	OpenSSL_KeyPtr key = sRsaKeyManager.GetRsakey(PLATFORM_ID_KUAIYONG);
	std::string notifyDecodeData = UrlDecode(orderKuaiyong.notify_data.c_str(),
			orderKuaiyong.notify_data.length());
	std::vector<u8> secret = Base64::decode(notifyDecodeData);

	std::vector<u8> dataVec = key->rsa_public_decrypt((void *) secret.data(),
			secret.size());
	if (dataVec.empty()) {
		VN_LOG_ERROR("notify_data decrypt error!");
		return;
	}
	std::vector<u8>::iterator itrVtr = dataVec.begin();
	std::string dataStr;	// = new std::string(dataVec.begin,dataVec.end());
	for (; itrVtr != dataVec.end(); itrVtr++) {
		dataStr.push_back((char) *itrVtr);
	}VN_LOG_DEBUG ("dataStr:" <<dataStr);

	p = strtok((char*) dataStr.c_str(), split);
	while (p != NULL) {
		std::string tmpStr = p;
		size_t pos = tmpStr.find("=");
		SET_OBJECT_ATTR(tmpStr,orderKuaiyong,dealseq)
		SET_OBJECT_ATTR(tmpStr,orderKuaiyong,payresult)
		SET_OBJECT_ATTR(tmpStr,orderKuaiyong,fee)
			SET_OBJECT_ATTR_END(tmpStr)
		p = strtok(NULL, split);
	}
	// 订单支付成功进行发货
	u64 cpOrderId = atol(orderKuaiyong.dealseq.c_str());
	int os = OS_ID_IOS;
	//订单记入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_KUAIYONG,atof(orderKuaiyong.fee.c_str())*100,orderKuaiyong.orderid, orderKuaiyong.dealseq,contentStr);

	int result = 0;
	// 验签
	std::map<std::string, std::string> valueMap = getValue(
			(char*) contentStr.c_str());
	valueMap["notify_data"] = notifyDecodeData;
	std::string sSignData = getSignData(valueMap);
	VN_LOG_DEBUG ("sSignData:" <<sSignData);
	OpenSSL_Verifier verigier;
	verigier.write(sSignData.data(), sSignData.length());
	bool isRight = verigier.verify_base64(key,
			UrlDecode(orderKuaiyong.sign.data(), orderKuaiyong.sign.size()));
	if (isRight && orderKuaiyong.payresult == "0") {
		//发货
		//std::string openid = "";//todo
		result = sPlayerManager.DeliverGoods(cpOrderId, atof(orderKuaiyong.fee.c_str())*100, orderKuaiyong.orderid,os);
		if(result != 0) {
			//发货失败
			VN_LOG_ERROR ("DeliverGoods error：:"<<result);
		}
	} else {
		result = 1; //验签失败
	}

	HttpResponse res;
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content_type("text/plain");
	str8 content;
	if (result == 0) {
		content = "success";
	} else {
		content = "failed";
	}
	res.set_content(
	vnnew ProxyBuffer((void *) content.c_str(), content.length()));
	this->send(res.pack((void *) (long) 0));
	return;
}

void HttpLink::doUCReq() {
//HTTP POST
	const BufferPtr & contet = m_parser.result().content();
	if (!contet) {
		VN_LOG_DEBUG ("contet error!");
		return;
	}
	std::string contentStr = (char *) contet->lock();

	vn::VariableJSON m_variableJSON;
	vn::RefCountedPtr<vn::Variable_object> m_vJSONPtr = m_variableJSON.parse(
			contentStr.data(), contentStr.size());
	if (!m_vJSONPtr.get()) {
		return;
	}
	RefCountedPtr<Variable_object> dataPtr = m_vJSONPtr->queryAttributeObject(
			"data");
	if (!dataPtr.get()) {
		return;
	}
	std::string cpOrderId = dataPtr->queryAttributeString("cpOrderId", "");
	std::string orderId = dataPtr->queryAttributeString("orderId", "");
	std::string amount = dataPtr->queryAttributeString("amount", "");
	int iAmount = atof(amount.c_str()) * 100;
	amount = std::to_string(iAmount);
	std::string signCnt = "accountId=";
	signCnt += dataPtr->queryAttributeString("accountId", "");
	signCnt += "amount=";
	signCnt += dataPtr->queryAttributeString("amount", "");
	signCnt += "callbackInfo=";
	signCnt += dataPtr->queryAttributeString("callbackInfo", "");
	signCnt += "cpOrderId=";
	signCnt += cpOrderId;
	signCnt += "creator=";
	signCnt += dataPtr->queryAttributeString("creator", "");
	signCnt += "failedDesc=";
	signCnt += dataPtr->queryAttributeString("failedDesc", "");
	signCnt += "gameId=";
	signCnt += dataPtr->queryAttributeString("gameId", "");
	signCnt += "orderId=";
	signCnt += orderId;
	signCnt += "orderStatus=";
	signCnt += dataPtr->queryAttributeString("orderStatus", "");
	signCnt += "payWay=";
	signCnt += dataPtr->queryAttributeString("payWay", "");
	signCnt += UC_ApiKey;

//订单记入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_UC, iAmount, orderId, cpOrderId,contentStr);

	std::string content = "FAILURE";
	vn::MD5 md5;
	md5.write(signCnt.c_str(), signCnt.length());
	std::string sign = md5.fetch_hex_lowercase();
	if (sign == m_vJSONPtr->queryAttributeString("sign", "")
			&& dataPtr->queryAttributeString("orderStatus", "") == "S") {
		//sign success
		//发货
		int os = OS_ID_ANDROID;
		int result = sPlayerManager.DeliverGoods( atol(cpOrderId.c_str()), iAmount,orderId,os);
		if (result == 0) {
			VN_LOG_DEBUG ("DeliverGoods ok:");
			content = "SUCCESS";
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
			content = "SUCCESS";
		}
	}

	HttpResponse res;
	res.set_content_type("text/html;charset=UTF-8;");
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content(
	vnnew ProxyBuffer((void*) content.c_str(), content.length()));
	this->send(res.pack((void*) (long) 0));
}
void HttpLink::doAnzhiReq() {
//HTTP POST
	VN_LOG_DEBUG ("HttpRequest content : \n"<<m_parser.result().url());
	const BufferPtr & contet = m_parser.result().content();
	if (!contet) {
		VN_LOG_DEBUG ("contet error!");
		return;
	}
	std::string contentStr = (char *) contet->lock();


	std::map<std::string, std::string> valueMap;
	const char * split1 = "&";
	char * valueKey = strtok((char *) contentStr.c_str(), split1);
	while (valueKey != NULL) {
		std::string tmpStr = valueKey;
		size_t pos = tmpStr.find("=");
		if (pos != std::string::npos) {
			std::string value = tmpStr.substr(pos + 1);
			valueMap[tmpStr.substr(0, pos)] = UrlDecode(value.c_str(),
					value.length());
			VN_LOG_DEBUG("valueMap: key=" << tmpStr.substr(0,pos) << " value="<<tmpStr.substr(pos+1));
		}
		valueKey = strtok(NULL, split1);
	}
	std::string dataStr = valueMap["data"];
//dataStr = UrlDecode(dataStr.data(),dataStr.length());
	std::vector<u8> secret = Base64::decode(dataStr);
	std::string secretSrt((char*) secret.data(), secret.size());
	dataStr = DES_Decrypt(secretSrt, ANZHI_AppSecret, TRIPLE_ECB);
	std::size_t pos = dataStr.find('}');
	if (pos == std::string::npos) {
		return;
	}
	dataStr = dataStr.substr(0, pos + 1);
	vn::VariableJSON m_variableJSON;
	VN_LOG_DEBUG ("dataStr:"<<dataStr);
	RefCountedPtr<Variable_object> dataPtr = m_variableJSON.parse(
			dataStr.data(), dataStr.size());
	if (!dataPtr.get()) {
		return;
	}
	std::string cpOrderId = dataPtr->queryAttributeString("cpInfo", "");
	std::string orderId = dataPtr->queryAttributeString("orderId", "");
	std::string amount = dataPtr->queryAttributeString("orderAmount", "");

//订单记入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_ANZHI, atoi(amount.c_str()), orderId, cpOrderId,contentStr);

//sign success
//发货
	std::string content = "money_error";
	int os = OS_ID_ANDROID;
	int result = sPlayerManager.DeliverGoods( atol(cpOrderId.c_str()), atoi(amount.c_str()), orderId,os,true);
	if (result == 0) {
		VN_LOG_DEBUG ("DeliverGoods ok:");
		content = "success";
	} else {
		VN_LOG_ERROR("DeliverGoods error：:"<<result);
		content = "money_error";
	}

	HttpResponse res;
	res.set_content_type("text/html;charset=UTF-8;");
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content(
	vnnew ProxyBuffer((void*) content.c_str(), content.length()));
	this->send(res.pack((void*) (long) 0));
}

void HttpLink::doXmwanReq() {
	const BufferPtr & contet = m_parser.result().content();
	VN_LOG_DEBUG (m_parser.result().url());
	if (!contet) {
		VN_LOG_DEBUG ("contet error!");
		return;
	}
	std::string contentStr = (char *) contet->lock();


	const char * split = "?&";
	char * p = strtok((char*) contentStr.c_str(), split);
	OrderXmwan orderXmwan;
	while (p != NULL) {			//VN_LOG_DEBUG ("GET args:"<<p);
		std::string tmpStr = p;
		size_t pos = tmpStr.find("=");
		SET_OBJECT_ATTR(tmpStr, orderXmwan, serial)    		//参与签名
		SET_OBJECT_ATTR(tmpStr, orderXmwan, amount)			//参与签名
		SET_OBJECT_ATTR(tmpStr, orderXmwan, status)			//参与签名
		SET_OBJECT_ATTR(tmpStr, orderXmwan, app_order_id)  //参与签名
		SET_OBJECT_ATTR(tmpStr, orderXmwan, app_user_id)	//参与签名
		SET_OBJECT_ATTR(tmpStr, orderXmwan, sign)
		SET_OBJECT_ATTR(tmpStr, orderXmwan, app_subject)   //如传则参与
		SET_OBJECT_ATTR(tmpStr, orderXmwan, app_description)   //如传则参与
		SET_OBJECT_ATTR(tmpStr, orderXmwan, app_ext1)		  //如传则参与
		SET_OBJECT_ATTR(tmpStr, orderXmwan, app_ext2)
			//如传则参与
			SET_OBJECT_ATTR_END(tmpStr)
		p = strtok(NULL, split);
	}
//订单计入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_XMW, atoi(orderXmwan.amount.c_str()) * 100, orderXmwan.serial, orderXmwan.app_order_id, (char *)contet->lock());
//验签
	std::map<std::string, std::string> valueMap;
	CHECK_SET_OBJECT(valueMap, orderXmwan, serial);
	CHECK_SET_OBJECT(valueMap, orderXmwan, amount);
	CHECK_SET_OBJECT(valueMap, orderXmwan, status);
	CHECK_SET_OBJECT(valueMap, orderXmwan, app_order_id);
	CHECK_SET_OBJECT(valueMap, orderXmwan, app_user_id);
	CHECK_SET_OBJECT(valueMap, orderXmwan, app_subject);
	CHECK_SET_OBJECT(valueMap, orderXmwan, app_description);
	CHECK_SET_OBJECT(valueMap, orderXmwan, app_ext1);
	CHECK_SET_OBJECT(valueMap, orderXmwan, app_ext2);

	std::string signValue = "";
	std::string content = "fail";

	std::map<std::string, std::string>::iterator itrValue = valueMap.begin();
	for (; itrValue != valueMap.end(); itrValue++) {
		signValue += (itrValue->first + "=" + itrValue->second + "&");
	}
	signValue += "client_secret=" + XMWAN_ClientSecret;
	VN_LOG_DEBUG (signValue);

	vn::MD5 md5;
	md5.write(signValue.c_str(), signValue.length());
	std::string sign = md5.fetch_hex_lowercase();
	if (sign == orderXmwan.sign) {
		content = "success";
		int os = OS_ID_ANDROID;
		int result = sPlayerManager.DeliverGoods( atol(orderXmwan.app_order_id.c_str()), atoi(orderXmwan.amount.c_str()) * 100, orderXmwan.serial, os, true);
		if (result == 0) {
			VN_LOG_DEBUG ("DeliverGoods ok:");
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
		}
	}
	HttpResponse res;
	res.set_content_type("text/html;charset=UTF-8;");
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content(
	vnnew ProxyBuffer((void*) content.c_str(), content.length()));
	this->send(res.pack((void*) (long) 0));

}

void HttpLink::doBaiduReq() {
	std::string contentStr;
	if (m_parser.result().method() == HttpRequest::kMethod_POST) {
		//POST
		const BufferPtr & contet = m_parser.result().content();
		if (!contet) {
			VN_LOG_DEBUG ("contet error!");
			return;
		}
		contentStr = (char *) contet->lock();
	} else if (m_parser.result().method() == HttpRequest::kMethod_GET) {
		contentStr = m_parser.result().url();
	}


	auto paramMap = GetURLParameter(contentStr);

	std::string content = paramMap["Content"];
	auto _data = Base64::decode(content);
	//std::string c((const c8 *)_data.data(), _data.size());
	//std::string c = (const char*)Base64::decode(paramMap["Content"]).data();
	//VN_LOG_DEBUG("Content" << c);

	vn::VariableJSON m_variableJSON;
	vn::RefCountedPtr<vn::Variable_object> JSONPtr = m_variableJSON.parse(
			(const c8 *) _data.data(), _data.size());
	if (!JSONPtr) {
		return;
	}

	auto orderMoney = JSONPtr->queryAttributeString("OrderMoney");

	std::string msg;

	if (m_parser.result().method() == HttpRequest::kMethod_POST) {
		const BufferPtr & contet = m_parser.result().content();
		msg = (char *) contet->lock();
	} else if (m_parser.result().method() == HttpRequest::kMethod_GET) {
		msg = m_parser.result().url();
	}

	//订单计入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_BAIDU, atof(orderMoney.c_str()) * 100, paramMap["OrderSerial"], paramMap["CooperatorOrderSerial"], msg);

	std::string baseString = BAIDU_AppID + paramMap["OrderSerial"]
			+ paramMap["CooperatorOrderSerial"] + content + BAIDU_SecretKey;
	//验签
	vn::MD5 md5;
	md5.write(baseString.c_str(), baseString.length());
	std::string signValue = md5.fetch_hex_lowercase();
	int code = 0;
	std::string resultCode = "0";
	if (signValue == paramMap["Sign"]) {
		//发货
		int os = OS_ID_ANDROID;
		int result = sPlayerManager.DeliverGoods( atol(paramMap["CooperatorOrderSerial"].c_str()), atof(orderMoney.c_str()) * 100, paramMap["OrderSerial"], os, true);
		if (result == 0) {
			code = 1;
			resultCode = "1";
			VN_LOG_DEBUG ("DeliverGoods ok:");
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
		}
	}

	baseString = BAIDU_AppID + resultCode + BAIDU_SecretKey;
	md5.write(baseString.c_str(), baseString.length());
	signValue = md5.fetch_hex_lowercase();

	Variable_object vobj;
	vobj.setAttribute("AppID", vnnew Variable_int32(6685863));
	vobj.setAttribute("ResultCode", vnnew Variable_int32(code));
	vobj.setAttribute("ResultMsg", vnnew Variable_string());
	vobj.setAttribute("Sign", vnnew Variable_string(signValue));
	vobj.setAttribute("Content", vnnew Variable_string());
	content = VariableJSON::generate(&vobj);

	HttpResponse res;
	res.set_content_type("text/html;charset=UTF-8;");
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content(
	vnnew ProxyBuffer((void*) content.c_str(), content.length()));
	this->send(res.pack((void*) (long) 0));
}

void HttpLink::doOppoReq() {
//POST
	const BufferPtr & contet = m_parser.result().content();
	VN_LOG_DEBUG (m_parser.result().url());
	if (!contet) {
		VN_LOG_DEBUG ("contet error!");
		return;
	}
	std::string contentStr = (char *) contet->lock();


	const char * split = "?&";
	char * p = strtok((char*) contentStr.c_str(), split);
	OrderOppo orderOppo;
	while (p != NULL) {			//VN_LOG_DEBUG ("GET args:"<<p);
		std::string tmpStr = p;
		size_t pos = tmpStr.find("=");
		SET_OBJECT_ATTR(tmpStr, orderOppo, notifyId)
		SET_OBJECT_ATTR(tmpStr, orderOppo, partnerOrder)
		SET_OBJECT_ATTR(tmpStr, orderOppo, productName)
		SET_OBJECT_ATTR(tmpStr, orderOppo, productDesc)
		SET_OBJECT_ATTR(tmpStr, orderOppo, price)
		SET_OBJECT_ATTR(tmpStr, orderOppo, count)
		SET_OBJECT_ATTR(tmpStr, orderOppo, attach)
		//SET_OBJECT_ATTR(tmpStr, orderOppo, userId)
		SET_OBJECT_ATTR(tmpStr, orderOppo, sign)
			SET_OBJECT_ATTR_END(tmpStr)
		p = strtok(NULL, split);
	}
//订单计入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_OPPO, atoi(orderOppo.price.c_str()), orderOppo.notifyId, orderOppo.partnerOrder,(char *)contet->lock());
	orderOppo.productDesc = UrlDecode(orderOppo.productDesc.c_str(),
			orderOppo.productDesc.length());

#define BASESTRING_APPEND(order,attribute) \
		baseString.append(#attribute"=");\
		baseString.append(order.attribute);\
		baseString.append("&");

	std::string baseString;
	BASESTRING_APPEND(orderOppo, notifyId)
	BASESTRING_APPEND(orderOppo, partnerOrder)
	BASESTRING_APPEND(orderOppo, productName)
	BASESTRING_APPEND(orderOppo, productDesc)
	BASESTRING_APPEND(orderOppo, price)
	BASESTRING_APPEND(orderOppo, count)
	BASESTRING_APPEND(orderOppo, attach)
//BASESTRING_APPEND(orderOppo, userId)
	baseString.erase(baseString.end() - 1);
	VN_LOG_DEBUG(baseString);

	//OpenSSL_KeyPtr key = OpenSSL_Key::create_public(OPPO_PubKey.data(), OPPO_PubKey.length());
	OpenSSL_KeyPtr key = sRsaKeyManager.GetRsakey(PLATFORM_ID_OPPO);
	OpenSSL_Verifier verigier;
	verigier.write(baseString.data(), baseString.length());
	bool isRight = verigier.verify_base64(key,
			UrlDecode(orderOppo.sign.c_str(), orderOppo.sign.length()));

	std::string content = "";
	if (isRight) {
		content = "result=OK&resultMsg=";
		//发货
		int os = OS_ID_ANDROID;
		int result = sPlayerManager.DeliverGoods( atol(orderOppo.partnerOrder.c_str()), atoi(orderOppo.price.c_str()), orderOppo.notifyId, os, true);
		if (result == 0) {
			VN_LOG_DEBUG ("DeliverGoods ok:");
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
		}
	} else {
		content = "result=FAIL&resultMsg=error";
	}
	HttpResponse res;
	res.set_content_type("text/html;charset=UTF-8;");
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content(
	vnnew ProxyBuffer((void*) content.c_str(), content.length()));
	this->send(res.pack((void*) (long) 0));
}

void HttpLink::doPptvReq() {
//GET
	auto &_url = m_parser.result().url();
	std::string contentStr(_url.c_str(), _url.length());
//	VN_LOG_DEBUG (contentStr);
//	const BufferPtr & contet = m_parser.result().content();
//	if(contet) {
//		std::string contentStr = (char *)contet->lock();
//
//	}

	const char * split = "?&";
	char * p = strtok((char*) contentStr.c_str(), split);
	OrderPptv orderPptv;
	while (p != NULL) {			//VN_LOG_DEBUG ("GET args:"<<p);
		std::string tmpStr = p;
		size_t pos = tmpStr.find("=");
		SET_OBJECT_ATTR(tmpStr, orderPptv, sid)
		SET_OBJECT_ATTR(tmpStr, orderPptv, roid)
		SET_OBJECT_ATTR(tmpStr, orderPptv, username)
		SET_OBJECT_ATTR(tmpStr, orderPptv, oid)
		SET_OBJECT_ATTR(tmpStr, orderPptv, amount)
		SET_OBJECT_ATTR(tmpStr, orderPptv, extra)
		SET_OBJECT_ATTR(tmpStr, orderPptv, time)
		SET_OBJECT_ATTR(tmpStr, orderPptv, sign)
			SET_OBJECT_ATTR_END(tmpStr)
		p = strtok(NULL, split);
	}

//订单计入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_PPTV, atoi(orderPptv.amount.c_str()) * 100, orderPptv.oid, orderPptv.extra, m_parser.result().url());

//验签
	std::string signValue = "";
	signValue += orderPptv.sid;
	signValue += UrlDecode(orderPptv.username.c_str(),
			orderPptv.username.length());
	signValue += orderPptv.roid;
	signValue += orderPptv.oid;
	signValue += orderPptv.amount;
	signValue += orderPptv.time;
	signValue += PPTV_Key;

	vn::MD5 md5;
	md5.write(signValue.c_str(), signValue.length());
	std::string sign = md5.fetch_hex_lowercase();
	std::string codeNum = "2";
	if (sign == orderPptv.sign) {
		codeNum = "1";
		//发货
		int os = OS_ID_ANDROID;
		int result = sPlayerManager.DeliverGoods( atol(orderPptv.extra.c_str()), atoi(orderPptv.amount.c_str()) * 100, orderPptv.oid, os, true);
		if (result == 0) {
			VN_LOG_DEBUG ("DeliverGoods ok:");
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
		}
	}
	std::string content = "{\"code\":\"";
	content += codeNum;
	content += "\",\"message\":\"\"}";

	HttpResponse res;
	res.set_content_type("text/html;charset=UTF-8;");
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content(
	vnnew ProxyBuffer((void*) content.c_str(), content.length()));
	this->send(res.pack((void*) (long) 0));
}

void HttpLink::doGioneeReq() {
//POST
	const BufferPtr & contet = m_parser.result().content();
//VN_LOG_DEBUG (m_parser.result().url());
	if (!contet) {
		VN_LOG_DEBUG ("contet error!");
		return;
	}
	std::string contentStr = "";
	contentStr = (char *) contet->lock();


	const char * split = "?&";
	char * p = strtok((char*) contentStr.c_str(), split);
	OrderGionee orderGionee;
	while (p != NULL) {			//VN_LOG_DEBUG ("GET args:"<<p);
		std::string tmpStr = p;
		size_t pos = tmpStr.find("=");
		SET_OBJECT_ATTR(tmpStr, orderGionee, api_key)
		SET_OBJECT_ATTR(tmpStr, orderGionee, close_time)
		SET_OBJECT_ATTR(tmpStr, orderGionee, create_time)
		SET_OBJECT_ATTR(tmpStr, orderGionee, deal_price)
		SET_OBJECT_ATTR(tmpStr, orderGionee, out_order_no)
		SET_OBJECT_ATTR(tmpStr, orderGionee, pay_channel)
		SET_OBJECT_ATTR(tmpStr, orderGionee, submit_time)
		SET_OBJECT_ATTR(tmpStr, orderGionee, user_id)
		SET_OBJECT_ATTR(tmpStr, orderGionee, sign)
			SET_OBJECT_ATTR_END(tmpStr)
		p = strtok(NULL, split);
	}

//订单计入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_GIONEE, atoi(orderGionee.deal_price.c_str()) * 100, "", orderGionee.out_order_no, (char *) contet->lock());
//验签
	std::map<std::string, std::string> valueMap;
	CHECK_SET_OBJECT(valueMap, orderGionee, api_key);
	CHECK_SET_OBJECT(valueMap, orderGionee, close_time);
	CHECK_SET_OBJECT(valueMap, orderGionee, create_time);
	CHECK_SET_OBJECT(valueMap, orderGionee, deal_price);
	CHECK_SET_OBJECT(valueMap, orderGionee, out_order_no);
	CHECK_SET_OBJECT(valueMap, orderGionee, pay_channel);
	CHECK_SET_OBJECT(valueMap, orderGionee, submit_time);
	CHECK_SET_OBJECT(valueMap, orderGionee, user_id);

	std::string signValue = "";
//std::string content = "fail";
	std::map<std::string, std::string>::iterator itrValue = valueMap.begin();
	for (; itrValue != valueMap.end(); itrValue++) {
		signValue += (itrValue->first + "=" + itrValue->second + "&");
	}
	signValue.erase(signValue.end() - 1);

	//OpenSSL_KeyPtr key = OpenSSL_Key::create_public(GIONEE_PublicKey.data(), GIONEE_PublicKey.length());
	OpenSSL_KeyPtr key = sRsaKeyManager.GetRsakey(PLATFORM_ID_GIONEE);
	OpenSSL_Verifier verigier;
	verigier.write(signValue.data(), signValue.length());
	bool isRight = verigier.verify_base64(key,
			UrlDecode(orderGionee.sign.c_str(), orderGionee.sign.length()));
	if (isRight) {
		//成功发货
		int os = OS_ID_ANDROID;
		int result = sPlayerManager.DeliverGoods( atol(orderGionee.out_order_no.c_str()), atoi(orderGionee.deal_price.c_str()) * 100, orderGionee.out_order_no, os, true);
		if (result == 0) {
			VN_LOG_DEBUG ("DeliverGoods ok:");
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
		}
	}
	std::string content = "success";
	HttpResponse res;
	res.set_content_type("text/html;charset=UTF-8;");
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content(
	vnnew ProxyBuffer((void*) content.c_str(), content.length()));
	this->send(res.pack((void*) (long) 0));
}

void HttpLink::doFlymeReq() {
	//POST
	const BufferPtr & contet = m_parser.result().content();
	VN_LOG_DEBUG (m_parser.result().url());
	if (!contet) {
		VN_LOG_DEBUG ("contet error!");
		return;
	}
	std::string contentStr = (char *) contet->lock();


	const char * split = "?&";
	char * p = strtok((char*) contentStr.c_str(), split);
	OrderFlyme orderFlyme;
	while (p != NULL) {			//VN_LOG_DEBUG ("GET args:"<<p);
		std::string tmpStr = p;
		size_t pos = tmpStr.find("=");
		SET_OBJECT_ATTR(tmpStr, orderFlyme, notify_time)
		SET_OBJECT_ATTR(tmpStr, orderFlyme, notify_id)
		SET_OBJECT_ATTR(tmpStr, orderFlyme, cp_order_id)
		SET_OBJECT_ATTR(tmpStr, orderFlyme, order_id)
		SET_OBJECT_ATTR(tmpStr, orderFlyme, app_id)
		SET_OBJECT_ATTR(tmpStr, orderFlyme, uid)
		SET_OBJECT_ATTR(tmpStr, orderFlyme, partner_id)
		SET_OBJECT_ATTR(tmpStr, orderFlyme, product_id)
		SET_OBJECT_ATTR(tmpStr, orderFlyme, product_unit)
		SET_OBJECT_ATTR(tmpStr, orderFlyme, buy_amount)
		SET_OBJECT_ATTR(tmpStr, orderFlyme, product_per_price)
		SET_OBJECT_ATTR(tmpStr, orderFlyme, total_price)
		SET_OBJECT_ATTR(tmpStr, orderFlyme, trade_status)
		SET_OBJECT_ATTR(tmpStr, orderFlyme, create_time)
		SET_OBJECT_ATTR(tmpStr, orderFlyme, pay_time)
		SET_OBJECT_ATTR(tmpStr, orderFlyme, pay_type)
		SET_OBJECT_ATTR(tmpStr, orderFlyme, user_info)
		SET_OBJECT_ATTR(tmpStr, orderFlyme, sign_type)
		SET_OBJECT_ATTR(tmpStr, orderFlyme, sign)
			SET_OBJECT_ATTR_END(tmpStr)
		p = strtok(NULL, split);
	}

	//订单计入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_FLYME, atof(orderFlyme.total_price.c_str()) * 100, orderFlyme.order_id, orderFlyme.cp_order_id, (char *) contet->lock());
	//验签
	std::map<std::string, std::string> valueMap;
	CHECK_SET_OBJECT2(valueMap, orderFlyme, notify_time);
	CHECK_SET_OBJECT2(valueMap, orderFlyme, notify_id);
	CHECK_SET_OBJECT2(valueMap, orderFlyme, order_id);
	CHECK_SET_OBJECT2(valueMap, orderFlyme, app_id);
	CHECK_SET_OBJECT2(valueMap, orderFlyme, uid);
	CHECK_SET_OBJECT2(valueMap, orderFlyme, partner_id);
	CHECK_SET_OBJECT2(valueMap, orderFlyme, cp_order_id);
	CHECK_SET_OBJECT2(valueMap, orderFlyme, product_id);
	CHECK_SET_OBJECT2(valueMap, orderFlyme, product_unit);
	CHECK_SET_OBJECT2(valueMap, orderFlyme, buy_amount);
	CHECK_SET_OBJECT2(valueMap, orderFlyme, product_per_price);
	CHECK_SET_OBJECT2(valueMap, orderFlyme, total_price);
	CHECK_SET_OBJECT2(valueMap, orderFlyme, trade_status);
	CHECK_SET_OBJECT2(valueMap, orderFlyme, create_time);
	CHECK_SET_OBJECT2(valueMap, orderFlyme, pay_time);
	CHECK_SET_OBJECT2(valueMap, orderFlyme, pay_type);
	CHECK_SET_OBJECT2(valueMap, orderFlyme, user_info);
	//CHECK_SET_OBJECT2(valueMap, orderFlyme, sign_type);

	std::string signValue = "";
	std::map<std::string, std::string>::iterator itrValue = valueMap.begin();
	for (; itrValue != valueMap.end(); itrValue++) {
		signValue += (itrValue->first + "=" + itrValue->second + "&");
	}
	signValue.erase(signValue.end() - 1);
	signValue += ":" + FLYME_AppSecret;

	VN_LOG_DEBUG(signValue);
	std::string codeNum = "120014";

	vn::MD5 md5;
	md5.write(signValue.c_str(), signValue.length());
	signValue = md5.fetch_hex_lowercase();
	if (signValue == orderFlyme.sign) {
		//验签成功 发货
		//成功发货
		int os = OS_ID_ANDROID;
		int result = sPlayerManager.DeliverGoods( atol(orderFlyme.cp_order_id.c_str()), atof(orderFlyme.total_price.c_str()) * 100, orderFlyme.order_id, os, true);
		if (result == 0) {
			VN_LOG_DEBUG ("DeliverGoods ok:");
			codeNum = "200";
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
		}
	}
	std::string content = "{\"code\":\"" + codeNum + "\"}";

	HttpResponse res;
	res.set_content_type("text/html;charset=UTF-8;");
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content(
	vnnew ProxyBuffer((void*) content.c_str(), content.length()));
	this->send(res.pack((void*) (long) 0));
}

void HttpLink::doAppchinaReq() {
	const BufferPtr & contet = m_parser.result().content();
	VN_LOG_DEBUG(m_parser.result().url());
	if (!contet) {
		VN_LOG_DEBUG ("contet error!");
		return;
	}
	std::string contentStr = (char *) contet->lock();

	std::map<std::string, std::string> valueMap;
	const char * split1 = "&";
	char * valueKey = strtok((char *) contentStr.c_str(), split1);
	while (valueKey != NULL) {
		std::string tmpStr = valueKey;
		size_t pos = tmpStr.find("=");
		if (pos != std::string::npos) {
			std::string value = tmpStr.substr(pos + 1);
			valueMap[tmpStr.substr(0, pos)] = UrlDecode(value.c_str(),
					value.length());
			VN_LOG_DEBUG("valueMap: key=" << tmpStr.substr(0,pos) << " value="<<tmpStr.substr(pos+1));
		}
		valueKey = strtok(NULL, split1);
	}

	std::string transdata = valueMap["transdata"];
	std::string sign = valueMap["sign"];

	VN_LOG_DEBUG("transdata:" << transdata); VN_LOG_DEBUG("sign" << sign);

	vn::VariableJSON m_variableJSON;
	vn::RefCountedPtr<vn::Variable_object> JSONPtr = m_variableJSON.parse(
			transdata.data(), transdata.size());
	if (!JSONPtr) {
		return;
	}
	std::string cporder = JSONPtr->queryAttributeString("exorderno");
	std::string transid = JSONPtr->queryAttributeString("transid");
	auto money = JSONPtr->queryAttributeInt32("money");

	//订单计入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_APPCHINA, money, transid, cporder, (char *) contet->lock());

#define MAX_KEY_LEN (64)
	struct _ {
		static int validsign(char *sKey, char *pTransData, int datalen,
				char *pSign, int signlen) {
			//获取rsa的密钥，rsa密钥格式:privatekey+modkey
			char szCompKey[MAX_KEY_LEN * 2 + 2];
			char szPrivateKey[MAX_KEY_LEN + 1];
			char szModKey[MAX_KEY_LEN + 1];
			memset(szCompKey, 0, sizeof(szCompKey));
			memset(szPrivateKey, 0, sizeof(szPrivateKey));
			memset(szModKey, 0, sizeof(szModKey));
			gIAppBase64Decode(sKey, szCompKey);
			char *pSplit;
			pSplit = strchr(szCompKey, '+');
			strcpy(szModKey, pSplit + 1);   //modkey
			strncpy(szPrivateKey, szCompKey, pSplit - szCompKey); //privatekey
			//开始验证
			char *pSignMD5;  //签名值的md5
			int nlen;
			pSignMD5 = appchina::gIAppDecRSA(&nlen, pSign, signlen,
					szPrivateKey, szModKey);
			char szOrgMD5[32];
			szOrgMD5[0] = '\0';
			appchina::gIAppMD5((char*) pTransData, strlen(pTransData),
					(char*) szOrgMD5);
			/*验证比较签名*/
			return strcmp(szOrgMD5, pSignMD5);
		}
	};
	char *key = new char[APPCHINA_AppKey.length() + 1];
	strcpy(key, APPCHINA_AppKey.c_str());
	int result = _::validsign(key, (char*) transdata.c_str(),
			strlen(transdata.c_str()), (char*) sign.c_str(),
			strlen(sign.c_str()));

	std::string content = "FAIL";
	if (!result) {
		//成功
		content = "SUCCESS";
		int os = OS_ID_ANDROID;
		int result = sPlayerManager.DeliverGoods( atol(cporder.c_str()), money, transid, os, true);
		if (result == 0) {
			VN_LOG_DEBUG ("DeliverGoods ok:");
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
		}
	}
	delete[] key;

	HttpResponse res;
	res.set_content_type("text/html;charset=UTF-8;");
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content(
	vnnew ProxyBuffer((void*) content.c_str(), content.length()));
	this->send(res.pack((void*) (long) 0));

}

void HttpLink::do4399Req() {
	//GET
//	const BufferPtr & contet = m_parser.result().url()();
//	if(!contet){
//		return ;
//	}
	std::string contentStr = m_parser.result().url();
	if (contentStr.empty()) {
		VN_LOG_DEBUG ("contet error!");
		return;
	}

	auto paramMap = GetURLParameter(contentStr, false);
	std::string basestring = "";
	basestring += paramMap["orderid"];
	basestring += paramMap["uid"];
	basestring += paramMap["money"];
	basestring += paramMap["gamemoney"];
	basestring += paramMap["serverid"];
	basestring += APP_Secrect4399;
	basestring += paramMap["mark"];
	basestring += paramMap["roleid"];
	basestring += paramMap["time"];

	//订单计入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_4399, atof(paramMap["money"].c_str()) * 100, paramMap["orderid"], paramMap["mark"], m_parser.result().url());

	vn::MD5 md5;
	md5.write(basestring.c_str(), basestring.length());
	std::string sign_calc = md5.fetch_hex_lowercase();

	int state = 3;
	if (sign_calc == paramMap["sign"]) {
		//验签成功 发货
		//成功发货
		int os = OS_ID_ANDROID;
		int result = sPlayerManager.DeliverGoods( atol(paramMap["mark"].c_str()), atof(paramMap["money"].c_str()) * 100, paramMap["orderid"], os, true);
		if (result == 0) {
			VN_LOG_DEBUG ("DeliverGoods ok:");
			state = 2;
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
		}
	}

	Variable_object vobj;
	vobj.setAttribute("status", vnnew Variable_int32(state));
	vobj.setAttribute("code", vnnew Variable_string());
	vobj.setAttribute("money", vnnew Variable_string(paramMap["money"]));
	vobj.setAttribute("gamemoney",
			vnnew Variable_string(paramMap["gamemoney"]));
	vobj.setAttribute("msg", vnnew Variable_string());
	std::string content = VariableJSON::generate(&vobj);

	HttpResponse res;
	res.set_content_type("text/html;charset=UTF-8;");
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content(
	vnnew ProxyBuffer((void*) content.c_str(), content.length()));
	this->send(res.pack((void*) (long) 0));

}

void HttpLink::doI4Req() {
	//POST
	VN_LOG_DEBUG(m_parser.result().url());
	const BufferPtr & contet = m_parser.result().content();
	if (!contet) {
		VN_LOG_DEBUG ("contet error!");
		return;
	}
	std::string contentStr = (char *) contet->lock();

	auto paramMap = GetURLParameter(contentStr);
	//订单计入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_I4, atof(paramMap["amount"].c_str())*100, paramMap["order_id"], paramMap["billno"], (char *) contet->lock());
	std::string basestring = "";
	for (auto &p : paramMap) {
		if (p.first == "sign")
			continue;
		basestring += p.first + "=" + p.second + "&";
	}
	basestring.erase(basestring.end() - 1);

	VN_LOG_DEBUG ("sign:"<< paramMap["sign"]);
	size_t i = 0;
	while ((i = paramMap["sign"].find("\n")) != std::string::npos) {
		paramMap["sign"].erase(i, 1);
	} VN_LOG_DEBUG ("sign:"<< paramMap["sign"]);
	OpenSSL_KeyPtr key = sRsaKeyManager.GetRsakey(PLATFORM_ID_I4);
	//1
	//OpenSSL_Verifier verigier;
	//verigier.write(basestring.data(), basestring.length());
	//bool ret = verigier.verify_base64(key, paramMap["sign"]);
	//2
	auto sign = Base64::decode(paramMap["sign"]);
	auto string = key->rsa_public_decrypt(sign.data(), sign.size());
	string.push_back(0);
	std::string s = (const char *) string.data();

	std::string content = "fail";
	if (s == basestring) {
		int os = OS_ID_IOS;
		int result = sPlayerManager.DeliverGoods( atol(paramMap["billno"].c_str()), atof(paramMap["amount"].c_str())*100, paramMap["order_id"], os, true);
		if (result == 0) {
			VN_LOG_DEBUG ("DeliverGoods ok:");
			content = "success";
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
		}
	}
	HttpResponse res;
	res.set_content_type("text/html;charset=UTF-8;");
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content(
	vnnew ProxyBuffer((void*) content.c_str(), content.length()));
	this->send(res.pack((void*) (long) 0));
}

void HttpLink::doCcplayReq() {
	const BufferPtr & contet = m_parser.result().content();
	if (!contet) {
		VN_LOG_DEBUG ("contet error!");
		return;
	}
	std::string contentStr = (char *) contet->lock();

	auto paramMap = GetURLParameter(contentStr);

	std::string cporderid = paramMap["partnerTransactionNo"];
	std::string pforderid = paramMap["transactionNo"];

	//订单计入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_CCPLAY, atof(paramMap["orderPrice"].c_str())*100, pforderid, cporderid, (char *) contet->lock());

	std::string basestring = "";
	for (auto &i : paramMap) {
		if (i.first == "sign")
			continue;
		basestring += i.first + "=" + i.second + "&";
	}
	basestring += CCPLAY_Secret;
	vn::MD5 md5;
	md5.write(basestring.c_str(), basestring.length());
	std::string sign_calc = md5.fetch_hex_lowercase();

	std::string content = "fail";
	if (sign_calc == paramMap["sign"]) {
		int os = OS_ID_ANDROID;
		int result = sPlayerManager.DeliverGoods( atol(cporderid.c_str()), atof(paramMap["orderPrice"].c_str())*100, pforderid, os, true);
		if (result == 0) {
			VN_LOG_DEBUG ("DeliverGoods ok:");
			content = "success";
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
		}
	}
	HttpResponse res;
	res.set_content_type("text/html;charset=UTF-8;");
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content(
	vnnew ProxyBuffer((void*) content.c_str(), content.length()));
	this->send(res.pack((void*) (long) 0));
}

void HttpLink::doCoolyunReq() {
	//GET
	const BufferPtr & contet = m_parser.result().content();
	if (!contet) {
		VN_LOG_DEBUG ("contet error!");
		return;
	}
	std::string contentStr = (char *) contet->lock();


	auto paramMap = GetURLParameter(contentStr, true);
	std::string transdata = paramMap["transdata"];
	std::string sign = paramMap["sign"];

	vn::VariableJSON m_variableJSON;
	vn::RefCountedPtr<vn::Variable_object> JSONPtr = m_variableJSON.parse(
			transdata.data(), transdata.size());
	if (!JSONPtr) {
		return;
	}
	std::string cporder = JSONPtr->queryAttributeString("cporderid");
	std::string transid = JSONPtr->queryAttributeString("transid");
	auto money = JSONPtr->queryAttributeFloat("money");

	//订单计入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_COOLYUN, money *100, transid, cporder, (char *) contet->lock());

	EVP_PKEY* pkey = UTILS::CryptHelper::getKeyByPKCS1(COOLYUN_PlatKey, 0);
	if (!pkey) {
		return;
	}
	std::string content = "FAILURE";
	if (0 == UTILS::CryptHelper::verifyMd5WithRsa(transdata, sign, pkey)) {
		int os = OS_ID_ANDROID;
		int result = sPlayerManager.DeliverGoods( atol(cporder.c_str()), money *100, transid, os, true);
		if (result == 0) {
			VN_LOG_DEBUG ("DeliverGoods ok:");
			content = "SUCCESS";
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
		}
	}

	HttpResponse res;
	res.set_content_type("text/html;charset=UTF-8;");
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content(
	vnnew ProxyBuffer((void*) content.c_str(), content.length()));
	this->send(res.pack((void*) (long) 0));

}

void HttpLink::doTBTReq() {
	//GET
	std::string contentStr = m_parser.result().url();
	if (contentStr.empty()) {
		VN_LOG_DEBUG ("contet error!");
		return;
	}

	auto paramMap = GetURLParameter(contentStr, false);

	std::string cporderid = paramMap["trade_no"];
	std::string pforderid = paramMap["tborder"];

	//订单计入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_TBT, atof(paramMap["amount"].c_str()), pforderid, cporderid, m_parser.result().url());

	std::string basestring = "";
	basestring += "source=" + paramMap["source"];
	basestring += "&trade_no=" + paramMap["trade_no"];
	basestring += "&amount=" + paramMap["amount"];
	basestring += "&partner=" + paramMap["partner"];
	basestring += "&paydes=" + paramMap["paydes"];
	basestring += "&debug=" + paramMap["debug"];
	basestring += "&tborder=" + paramMap["tborder"];
	basestring += "&key=" + TBT_APPKEY;

	vn::MD5 md5;
	md5.write(basestring.c_str(), basestring.length());
	std::string sign_calc = md5.fetch_hex_lowercase();
	std::string ret = "fail";
	if (sign_calc == paramMap["sign"]) {
		int os = OS_ID_IOS;
		int result = sPlayerManager.DeliverGoods( atol(cporderid.c_str()), atof(paramMap["amount"].c_str()),pforderid, os, true);
		if (result == 0) {
			ret = "successs";
			VN_LOG_DEBUG ("DeliverGoods ok:");
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
		}
	}
	Variable_object vobj;
	vobj.setAttribute("status", vnnew Variable_string(ret));
	std::string content = VariableJSON::generate(&vobj);

	HttpResponse res;
	res.set_content_type("text/html;charset=UTF-8;");
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content(
	vnnew ProxyBuffer((void*) content.c_str(), content.length()));
	this->send(res.pack((void*) (long) 0));

}

void HttpLink::doVivoReq() {
	//HTTP POST
	const BufferPtr & contet = m_parser.result().content();
	if (!contet) {
		VN_LOG_DEBUG ("contet error!");
		return;
	}
	std::string contentStr = (char *) contet->lock();

	auto paramMap = GetURLParameter(contentStr, true);

	std::string cporderid = paramMap["cpOrderNumber"];
	std::string pforderid = paramMap["orderNumber"];

	//订单计入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_VIVO, atof(paramMap["orderAmount"].c_str()), pforderid, cporderid, (char *) contet->lock());

	vn::MD5 md5;
	std::string signValue = "";
	for (auto &p : paramMap) {
		if (p.first == "signature" || p.first == "signMethod")
			continue;
		if (p.second.empty())
			continue;
		signValue += p.first + "=" + p.second + "&";
	}
	md5.write(VIVO_CPKEY.c_str(), VIVO_CPKEY.length());
	signValue += md5.fetch_hex_lowercase();
	VN_LOG_DEBUG (signValue);
	md5.write(signValue.c_str(), signValue.length());
	signValue = md5.fetch_hex_lowercase();
	VN_LOG_DEBUG (signValue); VN_LOG_DEBUG (paramMap["signature"]);
	std::string content = "fail";
	if (signValue == paramMap["signature"]) {
		int os = OS_ID_ANDROID;
		int result = sPlayerManager.DeliverGoods( atol(cporderid.c_str()), atof(paramMap["orderAmount"].c_str()),pforderid, os, true);
		if (result == 0) {
			VN_LOG_DEBUG ("DeliverGoods ok:");
			content = "success";
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
		}
	}

	HttpResponse res;
	res.set_content_type("text/html;charset=UTF-8;");
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content(
	vnnew ProxyBuffer((void*) content.c_str(), content.length()));
	this->send(res.pack((void*) (long) 0));
}

void HttpLink::doHMW_AND_Req() {
	//HTTP POST
	const BufferPtr & contet = m_parser.result().content();
	if (!contet) {
		VN_LOG_DEBUG ("contet error!");
		return;
	}
	std::string contentStr = (char *) contet->lock();

	auto paramMap = GetURLParameter(contentStr, false);

	//订单计入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_HAIMA_AND, atof(paramMap["total_fee"].c_str()) *100, "", paramMap["out_trade_no"], (char *) contet->lock());

	vn::MD5 md5;
	std::string signValue = "";
	signValue += "notify_time=" + paramMap["notify_time"];
	signValue += "&appid=" + paramMap["appid"];
	signValue += "&out_trade_no=" + paramMap["out_trade_no"];
	signValue += "&total_fee=" + paramMap["total_fee"];
	signValue += "&subject=" + paramMap["subject"];
	signValue += "&body=" + paramMap["body"];
	signValue += "&trade_status=" + paramMap["trade_status"];
	signValue += HMW_AND_APPKEY;

	md5.write(signValue.c_str(), signValue.length());
	signValue = md5.fetch_hex_lowercase();
	std::string content = "fail";
	std::string pforderid = "";
	if (signValue == paramMap["sign"]) {
		int os = OS_ID_ANDROID;
		int result = sPlayerManager.DeliverGoods( atol(paramMap["out_trade_no"].c_str()), atof(paramMap["total_fee"].c_str()) *100, pforderid, os, true);
		if (result == 0) {
			VN_LOG_DEBUG ("DeliverGoods ok:");
			content = "success";
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
		}
	}

	HttpResponse res;
	res.set_content_type("text/html;charset=UTF-8;");
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content(
	vnnew ProxyBuffer((void*) content.c_str(), content.length()));
	this->send(res.pack((void*) (long) 0));
}

void HttpLink::doHMW_IOS_Req() {
	//HTTP POST
	const BufferPtr & contet = m_parser.result().content();
	if (!contet) {
		VN_LOG_DEBUG ("contet error!");
		return;
	}
	std::string contentStr = (char *) contet->lock();

	auto paramMap = GetURLParameter(contentStr, false);

	//订单计入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_HAIMA_IOS, atof(paramMap["total_fee"].c_str()) *100, "", paramMap["out_trade_no"], (char *) contet->lock());

	vn::MD5 md5;
	std::string signValue = "";
	signValue += "notify_time=" + paramMap["notify_time"];
	signValue += "&appid=" + paramMap["appid"];
	signValue += "&out_trade_no=" + paramMap["out_trade_no"];
	signValue += "&total_fee=" + paramMap["total_fee"];
	signValue += "&subject=" + paramMap["subject"];
	signValue += "&body=" + paramMap["body"];
	signValue += "&trade_status=" + paramMap["trade_status"];
	signValue += HMW_IOS_APPKEY;

	md5.write(signValue.c_str(), signValue.length());
	signValue = md5.fetch_hex_lowercase();
	std::string content = "fail";
	std::string pforderid = "";
	if (signValue == paramMap["sign"]) {
		int os = OS_ID_IOS;
		int result = sPlayerManager.DeliverGoods( atol(paramMap["out_trade_no"].c_str()), atof(paramMap["total_fee"].c_str()) *100, pforderid, os, true);
		if (result == 0) {
			VN_LOG_DEBUG ("DeliverGoods ok:");
			content = "success";
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
		}
	}

	HttpResponse res;
	res.set_content_type("text/html;charset=UTF-8;");
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content(
	vnnew ProxyBuffer((void*) content.c_str(), content.length()));
	this->send(res.pack((void*) (long) 0));
}

void HttpLink::doSougouReq() {
	//HTTP POST
	VN_LOG_DEBUG (m_parser.result().url());
	const BufferPtr & contet = m_parser.result().content();
	if (!contet) {
		VN_LOG_DEBUG ("contet error!");
		return;
	}
	std::string contentStr = (char *) contet->lock();

	auto paramMap = GetURLParameter(contentStr, false);

	//订单计入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_SOGOU, atol(paramMap["amount1"].c_str()) *100, paramMap["oid"], paramMap["appdata"], (char *) contet->lock());

	std::string basestring = "";
	for (auto &i : paramMap) {
		if (i.first == "auth")
			continue;
		basestring += i.first + "=" + i.second + "&";
	}
	//basestring = UrlEncode(basestring.c_str(), basestring.length());
	basestring += SOGOU_PAYKEY;
	vn::MD5 md5;
	md5.write(basestring.c_str(), basestring.length());
	std::string sign = md5.fetch_hex_lowercase();

	std::string content = "ERR_500";
	if (sign == paramMap["auth"]) {
		int os = OS_ID_ANDROID;
		int result = sPlayerManager.DeliverGoods( atol(paramMap["appdata"].c_str()), atof(paramMap["amount1"].c_str()) *100, paramMap["oid"], os, true);
		if (result == 0) {
			content = "OK";
			VN_LOG_DEBUG ("DeliverGoods ok:");
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
		}
	}

		HttpResponse res;
		res.set_content_type("text/html;charset=UTF-8;");
		res.set_status(200);
		res.set_status_text("OK");
		res.set_content(
		vnnew ProxyBuffer((void*) content.c_str(), content.length()));
		this->send(res.pack((void*) (long) 0));
}

void HttpLink::doSinaReq() {
	//HTTP GET
	std::string contentStr = m_parser.result().url();
	if (contentStr.empty()) {
		VN_LOG_DEBUG ("contet error!");
		return;
	}
	contentStr = contentStr.substr(contentStr.find("?") + 1);


	auto paramMap = GetURLParameter(contentStr, false);

	//订单计入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_SINA, atof(paramMap["amount"].c_str()), paramMap["order_id"], paramMap["pt"], m_parser.result().url());

	std::string basestring = "";
	for (auto &i : paramMap) {
		if (i.first == "signature")
			continue;
		basestring += i.first + "|" + i.second + "|";
	}
	basestring += SINA_APPSECRET;
	VN_LOG_DEBUG ("basestring:"<< basestring);

	unsigned char sign[SHA_DIGEST_LENGTH];
	SHA1((const unsigned char*) basestring.c_str(), basestring.length(), sign);
	std::string signature = ByteArrayToHexStr(sign, SHA_DIGEST_LENGTH);

	std::string content = "FAIL";
	if (signature == paramMap["signature"]) {
		int os = OS_ID_ANDROID;
		int result = sPlayerManager.DeliverGoods( atol(paramMap["pt"].c_str()), atof(paramMap["amount"].c_str()), paramMap["order_id"], os, true);
		if (result == 0) {
			VN_LOG_DEBUG ("DeliverGoods ok:");
			content = "OK";
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
		}
	}

	HttpResponse res;
	res.set_content_type("text/html;charset=UTF-8;");
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content(
	vnnew ProxyBuffer((void*) content.c_str(), content.length()));
	this->send(res.pack((void*) (long) 0));
}

void HttpLink::doPpsReq() {
	std::string contentStr = m_parser.result().url();
	if (contentStr.empty()) {
		VN_LOG_DEBUG ("contet error!");
		return;
	}
	contentStr = contentStr.substr(contentStr.find("?") + 1);


	auto paramMap = GetURLParameter(contentStr, false);

	//订单计入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_QIYPPS, atof(paramMap["money"].c_str()) * 100, paramMap["order_id"], paramMap["userData"], m_parser.result().url());

	std::string basestring = paramMap["user_id"];
	basestring += paramMap["role_id"];
	basestring += paramMap["order_id"];
	basestring += paramMap["money"];
	basestring += paramMap["time"];
	basestring += PPS_KEY;

	VN_LOG_DEBUG ("basestring:" << basestring);
	vn::MD5 md5;
	md5.write(basestring.c_str(), basestring.length());
	std::string sign_calc = md5.fetch_hex_lowercase();

	int ret = -6;
	if (sign_calc == paramMap["sign"]) {
		int os = OS_ID_ANDROID;
		int result = sPlayerManager.DeliverGoods( atol(paramMap["userData"].c_str()),atof(paramMap["money"].c_str()) * 100, paramMap["order_id"], os, true);
		if (result == 0) {
			ret = 0;
			VN_LOG_DEBUG ("DeliverGoods ok:");
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
		}
	}

	Variable_object vobj;
	vobj.setAttribute("message", vnnew Variable_string());
	vobj.setAttribute("result", vnnew Variable_int32(ret));
	std::string content = VariableJSON::generate(&vobj);

	HttpResponse res;
	res.set_content_type("text/html;charset=UTF-8;");
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content(
	vnnew ProxyBuffer((void*) content.c_str(), content.length()));
	this->send(res.pack((void*) (long) 0));

}

void HttpLink::doGfanReq() {
	std::string contentStr = m_parser.result().url();
	if (contentStr.empty()) {
		VN_LOG_DEBUG ("contet error!");
		return;
	}
	contentStr = contentStr.substr(contentStr.find("?") + 1);

	auto paramMap = GetURLParameter(contentStr, false);

	std::string basestring = GFUN_UID + paramMap["time"];
	vn::MD5 md5;
	md5.write(basestring.c_str(), basestring.length());
	std::string sign_calc = md5.fetch_hex_lowercase();

	const BufferPtr & contet = m_parser.result().content();
	if (!contet) {
		VN_LOG_DEBUG ("contet error!");
		return;
	}
	std::string c = (char *) contet->lock();
	TiXmlDocument *xml = new TiXmlDocument();
	xml->Parse(c.c_str());
	TiXmlElement *root = xml->RootElement();
	auto p = root->FirstChildElement("order_id");
	if (!p) {
		VN_LOG_ERROR(" error!");
		return;
	}
	std::string order_id = p->GetText();
	p = root->FirstChildElement("cost");
	if (!p) {
		VN_LOG_ERROR(" error!");
		return;
	}
	std::string cost = p->GetText();
	VN_SAFE_DELETE(xml)
	//订单计入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_GFAN, atof(cost.c_str()) * 100, "", order_id, (char *) contet->lock());
	std::string pfid = "";
	std::string ret = "0";
	if (sign_calc == paramMap["sign"]) {
		int os = OS_ID_ANDROID;
		int result = sPlayerManager.DeliverGoods( atol(order_id.c_str()), atof(cost.c_str()) * 100, pfid, os, true);
		if (result == 0) {
			ret = "1";
			VN_LOG_DEBUG ("DeliverGoods ok:");
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
		}
	}
	std::string content = "<response><ErrorCode>" + ret
			+ "</ErrorCode><ErrorDesc>Success</ErrorDesc></response>";

	HttpResponse res;
	res.set_content_type("text/html;charset=UTF-8;");
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content(
	vnnew ProxyBuffer((void*) content.c_str(), content.length()));
	this->send(res.pack((void*) (long) 0));
}

void HttpLink::doYoukuReq() {
	//HTTP POST
	const BufferPtr & contet = m_parser.result().content();
	if (!contet) {
		VN_LOG_DEBUG ("contet error!");
		return;
	}
	std::string contentStr = (char *) contet->lock();

	auto paramMap = GetURLParameter(contentStr, false);
	//订单计入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_YOUKU, atof(paramMap["price"].c_str()) , "", paramMap["apporderID"], (char *) contet->lock());

	std::string basestring = "http://115.159.36.200:36004/pfid=34/?apporderID="
			+ paramMap["apporderID"] + "&price=" + paramMap["price"] + "&uid="
			+ paramMap["uid"];
	HmacMd5 md5(YOUKU_PAYKEY.length(), (uint8 const *) YOUKU_PAYKEY.c_str());
	md5.UpdateData(basestring);
	md5.Finalize();
	std::string sign = ByteArrayToHexStr(md5.GetDigest(), md5.GetLength());
	std::string pforderid = "";
	std::string ret = "fail";
	if (sign == paramMap["sign"]) {
		int os = OS_ID_ANDROID;
		int result = sPlayerManager.DeliverGoods( atol(paramMap["apporderID"].c_str()), atof(paramMap["price"].c_str()), pforderid, os, true);
		if (result == 0) {
			VN_LOG_DEBUG ("DeliverGoods ok:");
			ret = "success";
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
		}
	}

	Variable_object vobj;
	vobj.setAttribute("status", vnnew Variable_string(ret));
	vobj.setAttribute("desc", vnnew Variable_string());
	std::string content = VariableJSON::generate(&vobj);

	HttpResponse res;
	res.set_content_type("text/html;charset=UTF-8;");
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content(
	vnnew ProxyBuffer((void*) content.c_str(), content.length()));
	this->send(res.pack((void*) (long) 0));
}

void HttpLink::doIAppleReq() {
	//HTTP POST
	const BufferPtr & contet = m_parser.result().content();
	if (!contet) {
		VN_LOG_DEBUG ("contet error!");
		return;
	}
	std::string contentStr = (char *) contet->lock();

	auto paramMap = GetURLParameter(contentStr, false);
	//订单计入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_IAPPLE, atof(paramMap["amount"].c_str()) * 100 , paramMap["transaction"], paramMap["gameExtend"], (char *) contet->lock());
	std::string basestring = "";
	for (auto &i : paramMap) {
		if (i.first == "_sign")
			continue;
		basestring += i.first + "=" + i.second + "&";
	}
	basestring.erase(basestring.end() - 1);
	vn::MD5 md5;
	md5.write(basestring.c_str(), basestring.length());
	std::string sign_calc = md5.fetch_hex_lowercase();
	sign_calc += IAPPLE_SECRETKEY;
	md5.write(sign_calc.c_str(), sign_calc.length());
	sign_calc = md5.fetch_hex_lowercase();
	int ret = 1;

	if (sign_calc == paramMap["_sign"]) {
		int os = OS_ID_IOS;
		int result = sPlayerManager.DeliverGoods( atol(paramMap["gameExtend"].c_str()), atof(paramMap["amount"].c_str()) * 100, paramMap["transaction"], os, true);
		if (result == 0) {
			VN_LOG_DEBUG ("DeliverGoods ok:");
			ret = 0;
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
		}
	}

	Variable_object vobj;
	vobj.setAttribute("status", vnnew Variable_int32(ret));
	vobj.setAttribute("transID0", vnnew Variable_int32());
	std::string content = VariableJSON::generate(&vobj);

	HttpResponse res;
	res.set_content_type("text/html;charset=UTF-8;");
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content(
	vnnew ProxyBuffer((void*) content.c_str(), content.length()));
	this->send(res.pack((void*) (long) 0));

}

