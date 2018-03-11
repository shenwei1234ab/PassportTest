/*
 * HttpLink2.cpp
 *
 *  Created on: 2015年9月10日
 *      Author: leave
 */




#include "vnVariableJSON.h"

//#include "LinkManager.h"

#include "vnLog.h"
#include "vnHttpResponse.h"
#include <string.h>
#include "common/Util.h"
#include <map>

#include "../../passportsvr/Common.h"
#include "../../passportsvr/httplisten/HttpLink.h"
#include "../../passportsvr/manager/OrderManger.h"
#include "../../passportsvr/manager/PlayerManager.h"
#include "../../passportsvr/manager/RsaKeyManager.h"
#include "common/cryptography/RsaVerify.h"
#include "common/cryptography/RsaSign.h"
#include "common/cryptography/HmacHash.h"
#include "vnMD5.h"
#include "vnOpenSSL_Verifier.h"
#include "vnBase64.h"
#include <openssl/rsa.h>
#include "../../passportsvr/WorldManager.h"
#include "../database/PassPortDatabaseConnection.h"

extern PassPortDatabaseWorkerPool PassPortDatabase;

void HttpLink::doXYReq() {
	//HTTP POST
	const BufferPtr & contet = m_parser.result().content();
	if(!contet){
		VN_LOG_DEBUG ("contet error!");
		return ;
	}
	std::string contentStr = (char *) contet->lock();


	std::map<std::string, std::string> paramMap = GetURLParameter(contentStr);
	std::string& orderId = paramMap.at("orderid");
	std::string& cpOrderId = paramMap.at("extra");
	int iAmount = atoi(paramMap.at("amount").c_str()) * 100;

	//订单记入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_XY, iAmount, orderId, cpOrderId,(char *) contet->lock());
	std::string sign = paramMap.at("sign");
	std::string sig  = paramMap.at("sig");
	paramMap.erase("sign");
	paramMap.erase("sig");
	std::string signCntBase = SetURLParameter(paramMap);
	std::string content = "fail";
	MD5 md5_sign;
	std::string signCnt = XY_AppKey + signCntBase;
	md5_sign.write(signCnt.c_str(), signCnt.length());
	std::string signNew = md5_sign.fetch_hex_lowercase();

	MD5 md5_sig;
	std::string sigCnt = XY_PayKey + signCntBase;
	md5_sig.write(sigCnt.c_str(), sigCnt.length());
	std::string sigNew = md5_sig.fetch_hex_lowercase();

	if (sign == signNew && sig == sigNew) {
		//sign success
		//发货
		int os = OS_ID_IOS;
		int result = sPlayerManager.DeliverGoods( atol(cpOrderId.c_str()), iAmount, orderId,os);
		if (result == 0) {
			VN_LOG_DEBUG ("DeliverGoods ok:");
			content = "success";
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
			content = "fail";
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
//游龙
void HttpLink::do19196Req(){
	//HTTP POST
	const BufferPtr & contet = m_parser.result().content();
	if(!contet){
		VN_LOG_DEBUG ("contet error!");
		return ;
	}
	std::string contentStr = (char *) contet->lock();


	std::map<std::string, std::string> paramMap = GetURLParameter(contentStr);
	std::string& cpOrderId= paramMap.at("orderId");
	std::string& orderId  = paramMap.at("extra");
	int iAmount = atoi(paramMap.at("amount").c_str()) * 100;

	//订单记入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_19196, iAmount, orderId, cpOrderId,(char *) contet->lock());
	std::string sign = paramMap.at("flag");
	paramMap.erase("sign");
	std::string signCnt = cpOrderId + paramMap.at("userName")+ paramMap.at("amount")+ paramMap.at("extra") + P_19196_PKEY;
	std::string content = "FAILURE";
	MD5 md5_sign;
	md5_sign.write(signCnt.c_str(), signCnt.length());
	std::string signNew = md5_sign.fetch_hex_uppercase();

	if (sign == signNew ) {
		//sign success
		//发货
		int os = OS_ID_ANDROID;
		int result = sPlayerManager.DeliverGoods( atol(cpOrderId.c_str()), iAmount, orderId,os);
		if (result == 0) {
			VN_LOG_DEBUG ("DeliverGoods ok:");
			content = "OK";
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
			content = "FAILURE";
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
//豌豆枷
void HttpLink::doWandoujiaReq(){
	//HTTP POST
	const BufferPtr & contet = m_parser.result().content();
	if(!contet){
		VN_LOG_DEBUG ("contet error!");
		return ;
	}
	std::string contentStr = (char *) contet->lock();



	std::map<std::string, std::string> paramMap = GetURLParameter(contentStr);
	std::string& contentJson =paramMap.at("content");


	std::string orderId ;
	std::string cpOrderId ;
	int iAmount ;

	vn::VariableJSON m_variableJSON;
	vn::RefCountedPtr<vn::Variable_object> vJSONPtr = m_variableJSON.parse( contentJson.c_str(), contentJson.length());
	auto orderIdPtr = vJSONPtr->findAttribute("orderId").dyn_cast< vn::Variable_number>();
	if (orderIdPtr) {
		orderId = orderIdPtr->desc();
	}
	auto cpOrderIdPtr = vJSONPtr->findAttribute("out_trade_no").dyn_cast< vn::Variable_string>();
	if (cpOrderIdPtr) {
		cpOrderId = cpOrderIdPtr->value();
	}
	auto moneyPtr = vJSONPtr->findAttribute("money").dyn_cast< vn::Variable_number>();
	if (moneyPtr) {
		iAmount = moneyPtr->getInt32();
	}
	//订单记入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_WANDOUJIA, iAmount, orderId, cpOrderId,contentStr);
	std::string sign = paramMap.at("sign");

	OpenSSL_KeyPtr key = sRsaKeyManager.GetRsakey(PLATFORM_ID_WANDOUJIA);
	OpenSSL_Verifier verigier;
	verigier.write(contentJson.data(), contentJson.length());
	bool ret = verigier.verify_base64(key, sign);

	std::string content = "FAILURE";
	if (ret ) {
		//sign success
		//发货
		int os = OS_ID_ANDROID;
		int result = sPlayerManager.DeliverGoods( atol(cpOrderId.c_str()), iAmount,orderId,os);
		if (result == 0) {
			VN_LOG_DEBUG ("DeliverGoods ok:");
			content = "success";
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
			content = "FAILURE";
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
void HttpLink::doMuzhiwanReq(){
	//HTTP GET
	std::string contentStr = m_parser.result().url().c_str();


	std::map<std::string, std::string> paramMap = GetURLParameter(contentStr);
	std::string& orderId = paramMap.at("orderID");
	std::string& cpOrderId = paramMap.at("extern");
	int iAmount = atoi(paramMap.at("money").c_str()) * 100;

	//订单记入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_MUZHIWAN, iAmount, orderId, cpOrderId,m_parser.result().url());
	std::string sign = paramMap.at("sign");
	paramMap.erase("sign");
	std::string signCnt = paramMap.at("appkey")+ orderId + paramMap.at("productName")+ paramMap.at("productDesc")
			+ paramMap.at("productID")+ paramMap.at("money")+ paramMap.at("uid")+ paramMap.at("extern") + MUZHIWAN_PAYMENT_KEY;
	std::string content = "FAILURE";
	MD5 md5_sign;
	md5_sign.write(signCnt.c_str(), signCnt.length());
	std::string signNew = md5_sign.fetch_hex_lowercase();

	if (sign == signNew ) {
		//sign success
		//发货
		int os = OS_ID_ANDROID;
		int result = sPlayerManager.DeliverGoods( atol(cpOrderId.c_str()), iAmount, orderId,os);
		if (result == 0) {
			VN_LOG_DEBUG ("DeliverGoods ok:");
			content = "SUCCESS";
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
			content = "FAILURE";
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
void HttpLink::doDownjoyReq(){
	//HTTP GET
	std::string contentStr = m_parser.result().url().c_str();


	std::map<std::string, std::string> paramMap = GetURLParameter(contentStr);
	std::string& orderId = paramMap.at("order");
	std::string& cpOrderId = paramMap.at("ext");
	int iAmount = atof(paramMap.at("money").c_str()) * 100;

	//订单记入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_DOWNJOY, iAmount, orderId, cpOrderId,m_parser.result().url());
	std::string sign = paramMap.at("signature");

	std::string signCnt = "order="+ paramMap.at("order")
		   + "&money="	+ paramMap.at("money")
			+ "&mid="	+ paramMap.at("mid")
			+ "&time="	+ paramMap.at("time")
			+ "&result="	+ paramMap.at("result")
			+ "&ext="	+ paramMap.at("ext")
			+ "&key="	+ DOWNJON_PAYMENT_Key;
	std::string content = "failure";
	MD5 md5_sign;
	md5_sign.write(signCnt.c_str(), signCnt.length());
	std::string signNew = md5_sign.fetch_hex_lowercase();

	if (sign == signNew ) {
		//sign success
		//发货
		int os = OS_ID_ANDROID;
		int result = sPlayerManager.DeliverGoods( atol(cpOrderId.c_str()), iAmount, orderId,os);
		if (result == 0) {
			VN_LOG_DEBUG ("DeliverGoods ok:");
			content = "success";
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
			content = "failure";
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


std::string QQEncode(const char* pszBuf, const int iBufLen)
{
    std::string strTemp;
    for (int i = 0; i < iBufLen; i++)
    {
        if (isalnum((unsigned char)pszBuf[i]) ||
            (pszBuf[i] == '!') ||
            (pszBuf[i] == '*') ||
            (pszBuf[i] == '(') ||
            (pszBuf[i] == ')'))
            strTemp += pszBuf[i];
        else
        {
            strTemp += '%';
            strTemp += ToHex((unsigned char)pszBuf[i] >> 4);
            strTemp += ToHex((unsigned char)pszBuf[i] % 16);
        }
    }
    return std::move(strTemp);
}

void HttpLink::doQQReq(){
	//HTTP GET
	std::string contentStr = m_parser.result().url().c_str();


	std::map<std::string, std::string> paramMap = GetURLParameter(contentStr);
	std::string& orderId = paramMap.at("billno");
	std::string& cpOrderId = paramMap.at("app_metadata");
	int iAmount = atoi(paramMap.at("amt").c_str()) * 10;

	//订单记入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_QQ, iAmount, orderId, cpOrderId,m_parser.result().url());
	std::string sign = paramMap.at("sig");

	paramMap.erase("cee_extend");
	paramMap.erase("sig");
	for(auto& item :paramMap){
		item.second = QQEncode(item.second.c_str(),item.second.length());
		item.second = UrlEncode(item.second.c_str(),item.second.length());
	}
	std::string signCnt ;
	if(m_parser.result().method() == HttpRequest::kMethod_GET){
		signCnt = "GET&";
	} else {
		signCnt = "POST&";
	}
	std::string apiStr = "/pfid=25/";
	signCnt += UrlEncode(apiStr.c_str(),apiStr.length());
	signCnt += "&";
	signCnt += SetURLParameter(paramMap);

	std::string appkey = QQ_APPKEY + "&";
	HmacSha1 hmac((uint32) appkey.length(), (uint8 const*) appkey.c_str());
	hmac.UpdateData(signCnt);
	hmac.Finalize();
	//std::string hmacSing = ByteArrayToHexStr(hmac.GetDigest(), hmac.GetLength());
	std::string signNew = Base64::encode(hmac.GetDigest(), hmac.GetLength());
	std::string content = "failure";
	if (sign == signNew ) {
		//sign success
		//发货
		int os = OS_ID_ANDROID;
		int result = sPlayerManager.DeliverGoods( atol(cpOrderId.c_str()), iAmount, orderId,os);
		if (result == 0) {
			VN_LOG_DEBUG ("DeliverGoods ok:");
			content = "OK";
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
			content = "failure";
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
void HttpLink::doGuoPanAndReq() {
	//HTTP POST
	const BufferPtr & contet = m_parser.result().content();
	if (!contet) {
		VN_LOG_DEBUG ("contet error!");
		return;
	}
	std::string contentStr = (char *) contet->lock();

	std::map<std::string, std::string> paramMap = GetURLParameter(contentStr);

	std::string orderId = paramMap.at("trade_no");
	std::string cpOrderId = paramMap.at("serialNumber");
	std::string money = paramMap.at("money");
	int iAmount = atof(money.c_str()) * 100;
	std::string status = paramMap.at("status");
	std::string timestamp = paramMap.at("t");

	//订单记入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_GUOPAN_AND, iAmount, orderId, cpOrderId,contentStr);
	std::string sign = paramMap.at("sign");

	MD5 md5;
	std::string basestring = cpOrderId + money + status +  timestamp + GUOPAN_APP_KEY_AND;
	md5.write(basestring.c_str(), basestring.length());
	std::string signNew = md5.fetch_hex_lowercase();
	bool ret = (sign == signNew);

	std::string content = "FAILURE";
	if (ret) {
		//sign success
		//发货
		int os = OS_ID_ANDROID;
		int result = sPlayerManager.DeliverGoods( atol(cpOrderId.c_str()), iAmount,orderId,os);
		if (result == 0) {
			VN_LOG_DEBUG ("DeliverGoods ok:");
			content = "success";
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
			content = "FAILURE";
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
void HttpLink::doGuoPanIOSReq() {
	//HTTP POST
	const BufferPtr & contet = m_parser.result().content();
	if (!contet) {
		VN_LOG_DEBUG ("contet error!");
		return;
	}
	std::string contentStr = (char *) contet->lock();

	std::map<std::string, std::string> paramMap = GetURLParameter(contentStr);

	std::string orderId = paramMap.at("trade_no");
	std::string cpOrderId = paramMap.at("serialNumber");
	std::string money = paramMap.at("money");
	int iAmount = atof(money.c_str()) * 100;
	std::string status = paramMap.at("status");
	std::string timestamp = paramMap.at("t");
	//订单记入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_GUOPAN_IOS, iAmount, orderId, cpOrderId,contentStr);
	std::string sign = paramMap.at("sign");

	MD5 md5;
	std::string basestring = cpOrderId + money + status + timestamp + GUOPAN_APP_KEY_IOS;
	md5.write(basestring.c_str(), basestring.length());
	std::string signNew = md5.fetch_hex_lowercase();
	bool ret = (sign == signNew);

	std::string content = "FAILURE";
	if (ret) {
		//sign success
		//发货
		int os = OS_ID_IOS;
		int result = sPlayerManager.DeliverGoods( atol(cpOrderId.c_str()), iAmount,orderId,os);
		if (result == 0) {
			VN_LOG_DEBUG ("DeliverGoods ok:");
			content = "success";
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
			content = "FAILURE";
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

void HttpLink::doPPIOSReq(){
	//HTTP POST
	const BufferPtr & contet = m_parser.result().content();
	if (!contet) {
		VN_LOG_DEBUG ("contet error!");
		return;
	}
	std::string contentStr = (char *) contet->lock();

	std::map<std::string, std::string> paramMap = GetURLParameter(contentStr);

	std::string orderId = paramMap.at("order_id");
	std::string cpOrderId = paramMap.at("billno");
	std::string money = paramMap.at("amount");
	int iAmount = atof(money.c_str()) * 100;
	std::string status = paramMap.at("status");
	//订单记入数据库
	sOrderManger.InsertOrderTable(PLATFORM_ID_PP_IOS, iAmount, orderId, cpOrderId,contentStr);
	std::string sign = paramMap.at("sign");
	std::vector<u8> secret = Base64::decode(sign);
	OpenSSL_KeyPtr key = sRsaKeyManager.GetRsakey(PLATFORM_ID_PP_IOS);
	std::vector<u8> data = key->rsa_public_decrypt(secret.data(),secret.size());
	data.push_back(0);
	std::string signData = (const char *) data.data();
	VN_LOG_DEBUG (signData);
	vn::VariableJSON m_variableJSON;
	vn::RefCountedPtr<vn::Variable_object> JSONPtr = m_variableJSON.parse(signData.data(), signData.size());

	std::string content = "FAILURE";
	if (JSONPtr && JSONPtr->queryAttributeString("billno") == cpOrderId
			&& JSONPtr->queryAttributeString("amount") == money ) {
		//sign success
		//发货
		int os = OS_ID_IOS;
		int result = sPlayerManager.DeliverGoods( atol(cpOrderId.c_str()), iAmount,orderId,os);
		if (result == 0) {
			VN_LOG_DEBUG ("DeliverGoods ok:");
			content = "success";
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
			content = "FAILURE";
		}
	} else {
		VN_LOG_ERROR("PP order sign error!!");
	}

	HttpResponse res;
	res.set_content_type("text/html;charset=UTF-8;");
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content(
	vnnew ProxyBuffer((void*) content.c_str(), content.length()));
	this->send(res.pack((void*) (long) 0));
}


//登录验证
void HttpLink::doVerify() {
	const BufferPtr & contet = m_parser.result().content();
	if (!contet) {
		std::string content = "Please use post request .";
		HttpResponse res;
		res.set_content_type("text/html;charset=UTF-8;");
		res.set_status(200);
		res.set_status_text("OK");
		res.set_content(
		vnnew ProxyBuffer((void*) content.c_str(), content.length()));
		this->send(res.pack((void*) (long) 0));
		return;
	}
	std::string contentStr = (char *) contet->lock();

	std::map<std::string, std::string> paramMap = GetURLParameter(contentStr);

	std::string ppid = paramMap.at("ppid");
	std::string pptoken = paramMap.at("pptoken");

	std::vector<u8> ppidVtr = Base64::decode(ppid);
	if (ppidVtr.size() == sizeof(u64)) {
		VN_LOG_ERROR("pkt->data.ppid = "<<ppid << "Base64::decode error!");
		return;
	}
	u64 openId = 0;
	memcpy(&openId, ppidVtr.data(), sizeof(u64));
//		int ret;
//		u32 gameId = 1;
	VN_LOG_DEBUG("openId = "<<openId );
	std::vector<u8> pptokenVtr = Base64::decode(pptoken);
	u32 accessToken = 0;
	if (ppidVtr.size() == sizeof(u32)) {
		memcpy(&accessToken, pptokenVtr.data(), sizeof(accessToken));
	}
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_SEL_PPTOKEN);
	stmt->setUInt64(0, openId);
	PreparedQueryResult result = PassPortDatabase.SynQuery(stmt);

	int ret = 0;

	if (result && result->GetRowCount()) {
		uint64 ppToken = (*result)[0].GetUInt64();
		uint64 deadline = (*result)[1].GetUInt64();
		if (ppToken == accessToken && deadline - time(NULL) > 0) {
			ret = 0;
		} else {
			ret = -2;
			VN_LOG_ERROR("friendsvr: ppid[?] token["<< ppToken<<"] error ! accessToken["<<accessToken<<"]");
		}
	} else {
		ret = -1;
	}
	std::string content = std::to_string(ret);
	HttpResponse res;
	res.set_content_type("text/html;charset=UTF-8;");
	res.set_status(200);
	res.set_status_text("OK");
	res.set_content(
	vnnew ProxyBuffer((void*) content.c_str(), content.length()));
	this->send(res.pack((void*) (long) 0));

//		SessionDataPtr pSessionData = sWorldManager.getSessionData(openId,gameId);
//		if (pSessionData.get()) {
//			//验证成功
//			ret = 0;
//			VN_LOG_DEBUG("req_ValidateUser:find ppID : "<<openId);
//
//			std::vector<u8> pptokenVtr = Base64::decode(pptoken);
//
//			u32 accessToken = 0;
//			if (!pptokenVtr.empty()) {
//				memcpy(&accessToken, pptokenVtr.data(), sizeof(accessToken));
//			}
//			if (pSessionData->pptoken != accessToken) {
//				//验证失败,token 无效
//				ret = -2;
//			}
//		} else {
//			//验证失败
//			ret = -1;
//		}
//		std::string content = std::to_string(ret);
//		HttpResponse res;
//		res.set_content_type("text/html;charset=UTF-8;");
//		res.set_status(200);
//		res.set_status_text("OK");
//		res.set_content(
//		vnnew ProxyBuffer((void*) content.c_str(), content.length()));
//		this->send(res.pack((void*) (long) 0));
}

void HttpLink::doXINMEI365Req(){
	//HTTP GET
	std::string contentStr = m_parser.result().url().c_str();

	std::map<std::string, std::string> paramMap = GetURLParameter(contentStr);

	std::string orderId = paramMap.at("orderId");
	std::string callbackInfo = paramMap.at("callbackInfo");
	std::string channelLabel = paramMap.at("channelLabel");
	std::string channelOrderId = paramMap.at("channelOrderId");

	std::vector<u8> callbackInfoVtr = Base64::decode(callbackInfo);
	std::string cpOrderId((char*)callbackInfoVtr.data(), callbackInfoVtr.size());

	std::string price = paramMap.at("price"); //充值金额，整数，单位分
	int iAmount = atof(price.c_str());
	std::string sign = paramMap.at("sign");

	//订单记入数据库
//	sOrderManger.InsertOrderTable(PLATFORM_ID_XINMEI365, iAmount, orderId, cpOrderId,m_parser.result().url().c_str());
	sOrderManger.InsertOrderTableEx(PLATFORM_ID_XINMEI365, iAmount, orderId, cpOrderId,m_parser.result().url().c_str(),
			channelLabel,channelOrderId);

	MD5 md5;
	std::string basestring = orderId + price + callbackInfo + XINMEI365_TOKEN;
	md5.write(basestring.c_str(), basestring.length());
	std::string signNew = md5.fetch_hex_lowercase();
	bool ret = (sign == signNew);

	std::string content = "fail";
	if (ret) {
		//sign success
		//发货
		int os = OS_ID_ANDROID;
		int result = sPlayerManager.DeliverGoods( atol(cpOrderId.c_str()), iAmount,orderId,os);
		if (result == 0) {
			VN_LOG_DEBUG ("DeliverGoods ok:");
			content = "success";
		} else {
			VN_LOG_ERROR("DeliverGoods error：:"<<result);
			content = "fail";
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
