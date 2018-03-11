/*
 * SendDealReq.cpp
 *
 *  Created on: 2015年8月21日
 *      Author: leave
 */




#include "../database/PassPortDatabaseConnection.h"
#include "vnBase64.h"
#include <time.h>
#include <vector>

#include "../../passportsvr/clientlisten/ClientSession.h"
#include "../../passportsvr/WorldManager.h"
#include "vnMD5.h"
#include "curl/CurlWrapper.h"
#include "curl/CurlManager.h"

#include "common/cryptography/RsaVerify.h"
#include "common/cryptography/RsaSign.h"
#include "common/cryptography/HmacHash.h"




// 检验第三方支付结果
void ClientSession::SendDealReq(int pfid,int ppid,std::function<void(bool)> doDealReqFun){
	AccountPtr pAccount = sWorldManager.GetAccountInfo(ppid);
	std::string openid= pAccount->openid;
	std::string accessToken= pAccount->accesstoken;

	switch (pfid) {
	case PLATFORM_ID_TEST: { //官服
		break;
	}
	case PLATFORM_ID_HUAWEI: { //华为
		SendDealReq_Huawei(openid,accessToken,doDealReqFun);
		break;
	}
	case PLATFORM_ID_XIAOMI: { //小米
		SendDealReq_Xiaomi(openid,accessToken,doDealReqFun);
		break;
	}
	case PLATFORM_ID_LENOVO: { //联想
		SendDealReq_Lenovo(openid,accessToken,doDealReqFun);
		break;
	}
	case PLATFORM_ID_KUAIYONG: { //快用苹果助手
		SendDealReq_7659(openid,accessToken,doDealReqFun);
		break;
	}
	case PLATFORM_ID_360: {
		SendDealReq_360(openid,accessToken,doDealReqFun);
		break;
	}
	default:
		VN_LOG_ERROR("can't find pfid:"<<pfid)
		break;
	}
}

void ClientSession::SendDealReq_Xiaomi(std::string& openid, std::string& accessToken, std::function<void(bool)> doDealReqFun) {

	/* 主动查询订单支付状态接口
	 * 接口地址:http://mis.migc.xiaomi.com/api/biz/service/queryOrder.do
	 * 请求方法:GET
	 * appId	必须	游戏ID
	 *	cpOrderId	必须	开发商订单ID
	 *	uid	必须	用户ID
	 *	signature	必须	签名,签名方法见后面说明
	 */
	HttpRequestPtr aReqPtr(new HttpRequestC);
	aReqPtr->addr.from_string("42.62.48.210:80");
	aReqPtr->isPost = false;
	aReqPtr->content = "/api/biz/service/queryOrder.do";
	std::string signCnt =  "appId=";
	signCnt += XIAOMI_AppID;
	signCnt += "&session=";
	signCnt += accessToken;
	signCnt += "&uid=";
	signCnt += openid;
	aReqPtr->content += signCnt;
	//签名
	HmacSha1 hmac((uint32)XIAOMI_AppSecret.length(),(uint8 const*)XIAOMI_AppSecret.c_str());
	hmac.UpdateData(signCnt);
	hmac.Finalize();
	std::string signature=ByteArrayToHexStr(hmac.GetDigest(), hmac.GetLength());

	aReqPtr->isHttps = false;
	aReqPtr->callBackFn = [doDealReqFun](const HttpResponse& _httpResponse ) {
		vn::RefCountedPtr<vn::Variable_object>	vJSONPtr = _getJsonObj(_httpResponse);
		if (vJSONPtr.get()) {
			std::map<str8, VariablePtr> attributes = vJSONPtr->attributes();
			std::map<str8, VariablePtr>::iterator itrVar = attributes.begin();
			for (;itrVar != attributes.end(); itrVar++) {
				str8 name = itrVar->first;
				VariablePtr var = itrVar->second;
				if (var->type() == kVT_string) {
					VN_LOG_DEBUG( "json info : name = "<<name<<"  var = " <<var.dyn_cast<Variable_string>()->value());
				} else {
					VN_LOG_DEBUG("json info : name = "<<name);
				}
				if (doDealReqFun != nullptr) {
					if(name == "errcode"){
						//验证失败
						doDealReqFun(false);
					} else {
						//验证通过
						doDealReqFun(true);
					}
				}

			}
		}
	};
	sCurlManager.CurlSend(aReqPtr);

}
void ClientSession::SendDealReq_Huawei(std::string& openid,
		std::string& accessToken, std::function<void(bool)> doDealReqFun) {
	//没有查询接口
}
void ClientSession::SendDealReq_Lenovo(std::string& openid,
		std::string& accessToken, std::function<void(bool)> doDealReqFun) {
	//没有查询接口
}
void ClientSession::SendDealReq_360(std::string& openid,
		std::string& accessToken, std::function<void(bool)> doDealReqFun) {
}
//快用苹果助手
void ClientSession::SendDealReq_7659(std::string& openid,
		std::string& accessToken, std::function<void(bool)> doDealReqFun) {
}
