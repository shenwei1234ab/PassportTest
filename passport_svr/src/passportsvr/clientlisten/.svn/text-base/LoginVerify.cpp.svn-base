/*
 * LoginVerify.cpp
 *
 *  Created on: 2015年9月10日
 *      Author: leave
 */




#include "../database/PassPortDatabaseConnection.h"
#include "vnBase64.h"
#include <time.h>
#include <vector>

#include "../../passportsvr/clientlisten/ClientSession.h"
#include "../../passportsvr/manager/SessionManager.h"
#include "../../passportsvr/WorldManager.h"
#include "vnMD5.h"
#include "curl/CurlWrapper.h"
#include "curl/CurlManager.h"
#include "common/cryptography/RsaVerify.h"
#include "common/cryptography/RsaSign.h"
#include "common/cryptography/HmacHash.h"

extern PassPortDatabaseWorkerPool PassPortDatabase;

void ClientSession::SendVerifyReq_Xiaomi(std::string& openid,
		std::string& accessToken, std::function<void(bool)> doVerifyReqFun) {
	//用户session验证接口
	/*
	 * 接口地址:http://mis.migc.xiaomi.com/api/biz/service/verifySession.do
	 * 请求方法:GET
	 * 参数名称	重要性	说明
	 *	appId	必须	游戏ID
	 *	session	必须	用户sessionID
	 *	uid	必须	用户ID
	 *	signature	必须	签名,签名方法见后面说明
	 */
	HttpRequestPtr aReqPtr(new HttpRequestC);
	aReqPtr->addr.from_string("42.62.48.210:80");
	aReqPtr->host = "mis.migc.xiaomi.com";
	aReqPtr->isPost = false;
	aReqPtr->content = "/api/biz/service/verifySession.do?";

	std::string signCnt = "appId=";
	signCnt += XIAOMI_AppID;
	signCnt += "&session=";
	signCnt += accessToken;
	signCnt += "&uid=";
	signCnt += openid;
	aReqPtr->content += signCnt;
	//签名
	//std::vector<u8> secret = Base64::decode(XIAOMI_AppSecret);
	//HmacSha1 hmac((uint32)secret.size(),(uint8 const*)&secret[0]);
	HmacSha1 hmac((uint32) XIAOMI_AppSecret.length() + 1,
			(uint8 const*) XIAOMI_AppSecret.c_str());
	VN_LOG_DEBUG("signCnt = "<<signCnt);
	hmac.UpdateData(signCnt);
	hmac.Finalize();
	std::string signature = ByteArrayToHexStr(hmac.GetDigest(),
			hmac.GetLength());
	aReqPtr->content += "&signature=";
	aReqPtr->content += signature;
	VN_LOG_DEBUG("send content = "<<aReqPtr->content);
	aReqPtr->isHttps = false;

	aReqPtr->callBackFn = [doVerifyReqFun](const HttpResponse& _httpResponse ) {
		// errcode	必须	状态码200 验证正确1515 appId 错误1516 uid 错误1520 session 错误1525 signature 错误
			vn::RefCountedPtr<vn::Variable_object> vJSONPtr = _getJsonObj(_httpResponse);
			if (vJSONPtr.get()) {
				std::map<str8, VariablePtr>& attributes = vJSONPtr->attributes();
				std::map<str8, VariablePtr>::iterator itrVar = attributes.begin();
				for (;itrVar != attributes.end(); itrVar++) {
					str8 name = itrVar->first;
					VariablePtr var = itrVar->second;
					if (var->type() == kVT_string) {
						VN_LOG_DEBUG( "json info : name = "<<name<<"  var = " <<var.dyn_cast<Variable_string>()->value());
					} else {
						VN_LOG_DEBUG("json info : name = "<<name);
					}
					if(name == "errcode") {
						int status = var.dyn_cast<Variable_int32>()->value();
						VN_LOG_DEBUG("errcode :  "<<status);
						if(status != 200 ) {
							//验证失败
							doVerifyReqFun(false);
							break;
						} else {
							//验证通过
							doVerifyReqFun(true);
							break;
						}
					}
				}
			}
		};
	sCurlManager.CurlSend(aReqPtr);

}
void ClientSession::SendVerifyReq_Huawei(std::string& openid,
		std::string& accessToken, std::function<void(bool)> doVerifyReqFun) {
	//https://api.vmall.com/rest.php
	HttpRequestPtr aReqPtr(new HttpRequestC);
	aReqPtr->addr.from_string("122.11.38.57:443");
	aReqPtr->host = "api.vmall.com";
	aReqPtr->isPost = false;
	aReqPtr->content = "/rest.php?nsp_svc=OpenUP.User.getInfo&nsp_ts=";
	u64 curTime = time(NULL);

	char buffer[64];
	int pos = sprintf(buffer, "%llu", curTime);
	buffer[pos] = '\0';
	std::string sCurTime = buffer;
	VN_LOG_DEBUG("sCurTime = "<<sCurTime<< "  curTime="<<curTime);
	aReqPtr->content += sCurTime;
	aReqPtr->content += "&access_token=";
	std::string enCodeToken = UrlEncode(accessToken.c_str(),
			accessToken.length());
	VN_LOG_DEBUG("accessToken = "<<accessToken);VN_LOG_DEBUG("enCodeToken = "<<enCodeToken);
	std::string strTemp;
	for (size_t i = 0; i < enCodeToken.length(); i++) {
		if (enCodeToken[i] == '+') {
			strTemp += "%2B";
		} else {
			strTemp += enCodeToken[i];
		}
	}VN_LOG_DEBUG("strTemp = "<<strTemp);
	aReqPtr->content += strTemp;
	VN_LOG_DEBUG("send content = "<<aReqPtr->content);
	aReqPtr->isHttps = true;

	aReqPtr->callBackFn =
			[doVerifyReqFun,openid](const HttpResponse& _httpResponse ) {
				vn::RefCountedPtr<vn::Variable_object> vJSONPtr = _getJsonObj(_httpResponse);
				/*size_t start = _httpResponse.m_vJSONStr.find('{');
				 size_t end = _httpResponse.m_vJSONStr.find('}');
				 //VN_LOG_DEBUG("JSONStr = "<<_httpResponse.m_vJSONStr.substr(start,end));
				 std::string JSONStr =_httpResponse.m_vJSONStr.substr(start,end-start+1);
				 VN_LOG_DEBUG("JSONStr = "<<JSONStr);
				 vn::VariableJSON m_variableJSON;
				 vn::RefCountedPtr<vn::Variable_object> m_vJSONPtr = m_variableJSON.parse(JSONStr.data(), JSONStr.size());*/
				if (vJSONPtr.get()) {
					std::map<str8, VariablePtr>& attributes = vJSONPtr->attributes();
					std::map<str8, VariablePtr>::iterator itrVar = attributes.begin();
					for (;itrVar != attributes.end(); itrVar++) {
						str8 name = itrVar->first;
						VariablePtr var = itrVar->second;
						if (var->type() == kVT_string) {
							VN_LOG_DEBUG(
									"json info : name = "<<name<<"  var = " <<var.dyn_cast<Variable_string>()->value());
						} else {
							VN_LOG_DEBUG("json info : name = "<<name);
						}
						if(name == "error") {
							//验证失败
							doVerifyReqFun(false);
							break;
						} else if(name == "userID") {
							if(openid!=var.dyn_cast<Variable_string>()->value()) {
								VN_LOG_ERROR("HUAWEI userID="<<var.dyn_cast<Variable_string>()->value()<<" but openid="<<openid);
							}
							//验证通过
							doVerifyReqFun(true);
							break;
						}
					}
				}
			};
	sCurlManager.CurlSend(aReqPtr);
}
void ClientSession::SendVerifyReq_Lenovo(std::string& openid,
		std::string& accessToken, std::function<void(bool)> doVerifyReqFun) {
	//http://passport.lenovo.com/interserver/authen/1.2/getaccountid
	//HTTP GET
	HttpRequestPtr aReqPtr(new HttpRequestC);
	aReqPtr->addr.from_string("223.202.25.15:80");
	aReqPtr->host = "passport.lenovo.com";
	aReqPtr->isPost = false;
	aReqPtr->content = "/interserver/authen/1.2/getaccountid?lpsust=";
	aReqPtr->content += accessToken;
	aReqPtr->content += "&realm=";
	aReqPtr->content += LENOVO_AppID;
	VN_LOG_DEBUG("send content = "<<aReqPtr->content);
	aReqPtr->isHttps = false;

	aReqPtr->callBackFn =
			[doVerifyReqFun,&openid](const HttpResponse& _httpResponse ) {
				std::string conStr = _getContent(_httpResponse);
				size_t pos = conStr.find("<Error>");

				if(pos != std::string::npos) {
					//验证失败
					doVerifyReqFun(false);
				} else {
					size_t start = conStr.find("<AccountID>");
					size_t end = conStr.find("</AccountID>");
					std::string AccountID = conStr.substr(start+10,end - start-9);
					VN_LOG_DEBUG("AccountID = "<<AccountID);
					openid = AccountID;
					//验证通过
					doVerifyReqFun(true);
				}
			};
	sCurlManager.CurlSend(aReqPtr);
}
void ClientSession::SendVerifyReq_360(std::string& openid,
		std::string& accessToken, std::function<void(bool)> doVerifyReqFun) {
	//https://openapi.360.cn/user/me
	//HTTP GET
	HttpRequestPtr aReqPtr(new HttpRequestC);
	aReqPtr->addr.from_string("220.181.132.231:443");
	aReqPtr->host = "openapi.360.cn";
	aReqPtr->isPost = false;
	aReqPtr->content = "/user/me?access_token=";
	aReqPtr->content += accessToken;
	aReqPtr->content += "&fields=id,name,avatar,sex,area";

	VN_LOG_DEBUG("send content = "<<aReqPtr->content);
	aReqPtr->isHttps = true;
	aReqPtr->callBackFn =
			[doVerifyReqFun,&openid](const HttpResponse& _httpResponse ) {
				/*size_t start = _httpResponse.m_vJSONStr.find('{');
				 size_t end = _httpResponse.m_vJSONStr.find('}');
				 //VN_LOG_DEBUG("JSONStr = "<<_httpResponse.m_vJSONStr.substr(start,end));
				 std::string JSONStr =_httpResponse.m_vJSONStr.substr(start,end-start+1);
				 VN_LOG_DEBUG("JSONStr = "<<JSONStr);
				 vn::VariableJSON m_variableJSON;
				 vn::RefCountedPtr<vn::Variable_object> m_vJSONPtr = m_variableJSON.parse(JSONStr.data(), JSONStr.size());*/
				vn::RefCountedPtr<vn::Variable_object> vJSONPtr = _getJsonObj(_httpResponse);
				if (vJSONPtr.get()) {
					std::map<str8, VariablePtr>& attributes = vJSONPtr->attributes();
					std::map<str8, VariablePtr>::iterator itrVar = attributes.begin();
					for (;itrVar != attributes.end(); itrVar++) {
						str8 name = itrVar->first;
						VariablePtr var = itrVar->second;
						if (var->type() == kVT_string) {
							VN_LOG_DEBUG( "json info : name = "<<name<<"  var = " <<var.dyn_cast<Variable_string>()->value());
						} else {
							VN_LOG_DEBUG("json info : name = "<<name);
						}
						if(name == "error_code") {
							//验证失败
							doVerifyReqFun(false);
							break;
						}
						if(name == "id") {
							openid = var.dyn_cast<Variable_string>()->value();
							//验证通过
							doVerifyReqFun(true);
							break;
						}
					}
				}
			};
	sCurlManager.CurlSend(aReqPtr);

}
//快用苹果助手
void ClientSession::SendVerifyReq_7659(std::string& openid,
		std::string& accessToken, std::function<void(bool)> doVerifyReqFun) {
	//http://f_signin.bppstore.com/loginCheck.php
	//
	HttpRequestPtr aReqPtr(new HttpRequestC);
	aReqPtr->addr.from_string("42.62.21.134:80");
	aReqPtr->host = "f_signin.bppstore.com";
	aReqPtr->isPost = false;
	aReqPtr->content = "/loginCheck.php?tokenKey=";
	aReqPtr->content += accessToken;
	std::string singCnt = KUAIYONG_AppKey + accessToken;
	vn::MD5 md5;
	md5.write(singCnt.c_str(), singCnt.length());
	std::string sign_calc = md5.fetch_hex_lowercase();
	aReqPtr->content += "&sign=";
	aReqPtr->content += sign_calc;

	VN_LOG_DEBUG("send content = "<<aReqPtr->content);
	aReqPtr->isHttps = false;
	aReqPtr->callBackFn =
			[&openid,doVerifyReqFun](const HttpResponse& _httpResponse ) {
				vn::RefCountedPtr<vn::Variable_object> vJSONPtr = _getJsonObj(_httpResponse);
				if (vJSONPtr) {
					RefCountedPtr<Variable_object> userObj = vJSONPtr->queryAttributeObject("data");
					if (userObj ) {
						openid = userObj->queryAttributeString("guid");
					}
					auto aCode = vJSONPtr->findAttribute("code").dyn_cast<vn::Variable_number>();
					/** 0:成功
					 *	1:参数错误
					 *	2:token 无效
					 *	3：服务器忙
					 *	4：应用无效
					 *	5：sign 无效
					 *	6：用户不存在*/
					if (aCode && aCode->getInt32() == 0 && !openid.empty()) {
						// success.
						//验证通过
						doVerifyReqFun(true);
					} else {
						// failed.
						//验证失败
						doVerifyReqFun(false);
					}
				}
			};
	sCurlManager.CurlSend(aReqPtr);
}

void ClientSession::SendVerifyReq_Anzhi(std::string& openid,
		std::string& accessToken, std::function<void(bool)> doVerifyReqFun) {
	VN_LOG_DEBUG("openid = "<<openid); VN_LOG_DEBUG("accessToken = "<<accessToken);
	//http://user.anzhi.com/web/api/sdk/third/1/queryislogin
	// PSOT

	/*
	 * 完整消息
	 *	time=20130228101059123&   	请求时间yyyyMMddHHmmssSSS 精确到毫秒 例如：20130703103856771
	 *	appkey=fds12121&				应用key
	 *	sid=xxxxxx&						当前登录用户会话id
	 *	sign=xxxx							签名串，格式 Base64.encodeToString (appkey+sid+appsecret);
	 */
	HttpRequestPtr aReqPtr(new HttpRequestC);
	aReqPtr->addr.from_string("42.62.4.174:80");
	aReqPtr->host = "user.anzhi.com";
	aReqPtr->isPost = true;
	aReqPtr->content = "/web/api/sdk/third/1/queryislogin?";

	std::string signCnt = ANZHI_AppID;
	signCnt += openid;
	signCnt += accessToken;
	signCnt += ANZHI_AppSecret;

	aReqPtr->postUri = "appkey=";
	aReqPtr->postUri += ANZHI_AppID;
	aReqPtr->postUri += "&account=";
	aReqPtr->postUri += UrlEncode(openid.c_str(), openid.length());
	aReqPtr->postUri += "&sid=";
	aReqPtr->postUri += UrlEncode(accessToken.c_str(), accessToken.length());
	aReqPtr->postUri += "&sign=";

	//签名
	VN_LOG_DEBUG("signCnt = "<<signCnt);
	//std::vector<u8> secret = Base64::decode(signCnt);
	std::string secret = base64_encode(signCnt.data(), signCnt.size());
	//std::string signature = ByteArrayToHexStr((vn::u8 const*)secret.data(), secret.size());
	aReqPtr->postUri += secret;
	aReqPtr->postUri += "&time=";
	aReqPtr->postUri += ChangeTimeToStr(time(NULL), "%Y%m%d%H%M%S000");

	aReqPtr->content += aReqPtr->postUri;

	VN_LOG_DEBUG("send content = "<<aReqPtr->content);
	aReqPtr->isHttps = false;
	aReqPtr->callBackFn =
			[&openid,doVerifyReqFun](const HttpResponse& _httpResponse ) {
				/*
				 * 	{
				 * 	“sc”: “1”,						//状态码
				 * 	“st”: “成功(sid 有效) ” ，		//状态描述
				 * 	“time”:”20130228101059123”	//响应时间
				 * 	“msg” : {
				 * 		“uid”:”123456789”			//安智用户id
				 * 		"nickname":””				//用户昵称
				 * 		}
				 *	}
				 */
				vn::RefCountedPtr<vn::Variable_object> vJSONPtr = _getJsonObj(_httpResponse);
				if (vJSONPtr.get()) {
					std::map<str8, VariablePtr>& attributes = vJSONPtr->attributes();
					std::map<str8, VariablePtr>::iterator itrVar = attributes.begin();
					std::string status;
					for (;itrVar != attributes.end(); itrVar++) {
						str8 name = itrVar->first;
						VariablePtr var = itrVar->second;
						if (var->type() == kVT_string) {
							VN_LOG_DEBUG( "json info : name = "<<name<<"  var = " <<var.dyn_cast<Variable_string>()->value());
						} else {
							VN_LOG_DEBUG("json info : name = "<<name);
						}
						if(name == "sc") {
							status = var.dyn_cast<Variable_string>()->value();
							VN_LOG_DEBUG("status :  "<<status);
						}
					}
					if(status != "1" || openid.empty()) {
						//验证失败
						doVerifyReqFun(false);
					} else {
						//验证通过
						doVerifyReqFun(true);
					}
				}
			};
	sCurlManager.CurlSend(aReqPtr);

}

void ClientSession::SendVerifyReq_UC(std::string& openid,
		std::string& accessToken, std::function<void(bool)> doVerifyReqFun) {
	//POST 方式
	HttpRequestPtr aReqPtr(new HttpRequestC);

	/*	//测试环境： http://sdk.test4.g.uc.cn/cp/account.verifySession
	 aReqPtr->addr.from_string("117.135.147.207:80");
	 aReqPtr->host = "sdk.test4.g.uc.cn";
	 aReqPtr->content = "/cp/account.verifySession";*/

	//正式环境：http://sdk.g.uc.cn/cp/account.verifySession
	aReqPtr->addr.from_string("117.135.151.250:80");
	aReqPtr->host = "sdk.g.uc.cn";
	aReqPtr->content = "/cp/account.verifySession";

	aReqPtr->isPost = true;
	u64 curTime = time(NULL) * 1000;
	std::string signCnt = "sid=";
	signCnt += accessToken;
	signCnt += UC_ApiKey;

	vn::MD5 md5;
	md5.write(signCnt.c_str(), signCnt.length());
	std::string sign = md5.fetch_hex_lowercase();

	aReqPtr->postUri = "{\n";
	aReqPtr->postUri += "\"id\":";
	aReqPtr->postUri += std::to_string(curTime);
	aReqPtr->postUri += ",\n";
	aReqPtr->postUri += "\"data\":{\"sid\":\"";
	aReqPtr->postUri += accessToken;
	aReqPtr->postUri += "\"},\n";
	aReqPtr->postUri += "\"game\":{\"gameId\":";
	aReqPtr->postUri += UC_GameID;
	aReqPtr->postUri += "},\n";
	aReqPtr->postUri += "\"sign\":\"";
	aReqPtr->postUri += sign;
	aReqPtr->postUri += "\"\n}";

	//签名
	aReqPtr->isHttps = false;
	aReqPtr->callBackFn =
			[&openid,doVerifyReqFun](const HttpResponse& _httpResponse ) {
				/*
				 * 	{
				 * 	"id":1330395827,
				 * 	"state":{"code":1, "msg":"操作成功"},
				 * 	"data":{
				 * 	"accountId":"U11626774a4e39c16cf7mmsnz5002une",
				 * 	"creator":"JY",
				 * 	"nickName":"九游玩家"
				 * 		}
				 * 	}
				 */
				vn::RefCountedPtr<vn::Variable_object> vJSONPtr = _getJsonObj(_httpResponse);
				if (vJSONPtr.get()) {
					std::map<str8, VariablePtr>& attributes = vJSONPtr->attributes();
					std::map<str8, VariablePtr>::iterator itrVar = attributes.begin();
					int code = -1;
					for (;itrVar != attributes.end(); itrVar++) {
						const str8& name = itrVar->first;
						VariablePtr var = itrVar->second;
						if (var->type() == kVT_string) {
							VN_LOG_DEBUG( "json info : name = "<<name<<"  var = " <<var.dyn_cast<Variable_string>()->value());
						} else {
							VN_LOG_DEBUG("json info : name = "<<name);
						}
						if(name == "state") {
							RefCountedPtr<Variable_object> msgVar = var.dyn_cast<Variable_object>();
							code = msgVar->queryAttributeInt32("code",-1);
							VN_LOG_DEBUG("msg :  "<<msgVar->queryAttributeString("msg",""));
						}
						if(name == "data") {
							if (var->type() == kVT_object) {
								RefCountedPtr<Variable_object> msgVar = var.dyn_cast<Variable_object>();
								openid = msgVar->queryAttributeString("accountId","");
							}
						}
					}
					if(code != 1 || openid.empty()) {
						//验证失败
						doVerifyReqFun(false);
					} else {
						//验证通过
						doVerifyReqFun(true);
					}
				}
			};
	sCurlManager.CurlSend(aReqPtr);
}

void ClientSession::SendVerifyReq_Xmwan(std::string& openid,
		std::string& accessToken, std::function<void(bool)> doVerifyReqFun) {
	//GET 请求
	HttpRequestPtr aReqPtr(new HttpRequestC);
	aReqPtr->addr.from_string("121.40.104.185:80");
	aReqPtr->host = "open.xmwan.com";
	aReqPtr->isPost = false;
	aReqPtr->content = "/v2/users/me?access_token=";
	aReqPtr->content += accessToken;
	aReqPtr->isHttps = false;
	aReqPtr->callBackFn =
			[doVerifyReqFun,&openid](const HttpResponse& _httpResponse ) {
				/*					size_t start = _httpResponse.m_vJSONStr.find('{');
				 size_t end = _httpResponse.m_vJSONStr.find('}');
				 //VN_LOG_DEBUG("JSONStr = "<<_httpResponse.m_vJSONStr.substr(start,end));
				 std::string JSONStr =_httpResponse.m_vJSONStr.substr(start,end-start+1);
				 VN_LOG_DEBUG("JSONStr = "<<JSONStr);
				 vn::VariableJSON m_variableJSON;
				 vn::RefCountedPtr<vn::Variable_object> m_vJSONPtr = m_variableJSON.parse(JSONStr.data(), JSONStr.size());*/
				vn::RefCountedPtr<vn::Variable_object> vJSONPtr = _getJsonObj(_httpResponse);
				if (vJSONPtr.get()) {
					std::map<str8, VariablePtr>& attributes = vJSONPtr->attributes();
					std::map<str8, VariablePtr>::iterator itrVar = attributes.begin();
					for (;itrVar != attributes.end(); itrVar++) {
						const str8& name = itrVar->first;
						VariablePtr var = itrVar->second;
						if (var->type() == kVT_string) {
							VN_LOG_DEBUG( "json info : name = "<<name<<"  var = " <<var.dyn_cast<Variable_string>()->value());
						} else {
							VN_LOG_DEBUG("json info : name = "<<name);
						}
						if(name == "error") {
							//验证失败
							doVerifyReqFun(false);
							break;
						} else if(name == "xmw_open_id") {
							openid = var.dyn_cast<Variable_string>()->value();
							//验证通过
							doVerifyReqFun(true);
							break;
						}
					}
				}
			};
	sCurlManager.CurlSend(aReqPtr);
}

void ClientSession::SendVerifyReq_Baidu(std::string& openid,
		std::string& accessToken, std::function<void(bool)> doVerifyReqFun) {
	std::string basestring = BAIDU_AppID + accessToken + BAIDU_SecretKey;
	vn::MD5 md5;
	md5.write(basestring.c_str(), basestring.length());
	std::string sign = md5.fetch_hex_lowercase();
	//POST 文档所有接口仅至此号post方式  返回信息全部为json结构
	HttpRequestPtr aReqPtr(new HttpRequestC);
	aReqPtr->addr.from_string("59.56.20.223:80"); //
	aReqPtr->host = "querysdkapi.91.com";
	aReqPtr->isPost = true;
	aReqPtr->content = "/CpLoginStateQuery.ashx?";
	aReqPtr->content += "AppID=" + BAIDU_AppID + "&AccessToken=" + accessToken
			+ "&Sign=" + sign;
	//aReqPtr->postUri = UrlEncode(aReqPtr->postUri.c_str(), aReqPtr->postUri.length());
	aReqPtr->isHttps = false;
	aReqPtr->callBackFn =
			[doVerifyReqFun,&openid](const HttpResponse& _httpResponse ) {
				BufferPtr pBuffer = _httpResponse.content();
				VN_LOG_DEBUG(str8((char*)pBuffer->lock()));
				//验签
				vn::RefCountedPtr<vn::Variable_object> vJSONPtr = _getJsonObj(_httpResponse);
				if(!vJSONPtr.get()) {
					doVerifyReqFun(false);
					return;
				}
				int appId= vJSONPtr->queryAttributeInt32("AppID");
				if(appId != 6685863) {
					doVerifyReqFun(false);
					return;
				}
				int resultCode = vJSONPtr->queryAttributeInt32("ResultCode");
				if(resultCode != 1) {
					doVerifyReqFun(false);
					return;
				}
				std::string sign = vJSONPtr->queryAttributeString("Sign");
				if(sign.empty()) {
					doVerifyReqFun(false);
					return;
				}
				std::string content = vJSONPtr->queryAttributeString("Content");
				if(content.empty()) {
					doVerifyReqFun(false);
					return;
				}
				content = UrlDecode(content.c_str(), content.length());
				std::string basestring = "66858631" + content + BAIDU_SecretKey;

				vn::MD5 md5;
				md5.write(basestring.c_str(), basestring.length());
				std::string signValue = md5.fetch_hex_lowercase();
				if(signValue == sign) {
					doVerifyReqFun(true);
				} else {
					doVerifyReqFun(false);
				}
			};
	sCurlManager.CurlSend(aReqPtr);
}

void ClientSession::SendVerifyReq_Oppo(std::string& openid,
		std::string& accesstoken, std::function<void(bool)> doVerifyReqFun) {
	struct user_check_args {
		std::string oauth_consumer_key; //app key
		std::string oauth_nonce;        //随机数
		std::string oauth_signature_method; //HMAC-SHA1
		std::string oauth_token;  //客户端获取的access_token
		std::string oauth_timestamp;   //时间戳
		std::string oauth_version;     //版本
		std::string oauth_signature;
	};
	struct _ {
		static std::string get_base_string(const std::string& http_method,
				const std::string& request_url, const user_check_args& args) {
			std::string base_str;
			base_str = "POST&";
			base_str = base_str
					+ UrlEncode(request_url.c_str(), request_url.length())
					+ "&";
			std::string oauth_consumer_key = "oauth_consumer_key%3D"
					+ UrlEncode(args.oauth_consumer_key.c_str(),
							args.oauth_consumer_key.length());
			std::string oauth_nonce = "oauth_nonce%3D"
					+ UrlEncode(args.oauth_nonce.c_str(),
							args.oauth_nonce.length());
			std::string oauth_signature_method = "oauth_signature_method%3D"
					+ UrlEncode(args.oauth_signature_method.c_str(),
							args.oauth_signature_method.length());
			std::string oauth_token = "oauth_token%3D"
					+ UrlEncode(args.oauth_token.c_str(),
							args.oauth_token.length());
			std::string oauth_timestamp = "oauth_timestamp%3D"
					+ UrlEncode(args.oauth_timestamp.c_str(),
							args.oauth_timestamp.length());
			std::string oauth_version = "oauth_version%3D"
					+ UrlEncode(args.oauth_version.c_str(),
							args.oauth_version.length());
			std::string tmp_str = oauth_consumer_key + "%26" + oauth_nonce
					+ "%26" + oauth_signature_method + "%26" + oauth_timestamp
					+ "%26" + oauth_token + "%26" + oauth_version;
			base_str += tmp_str;
			return base_str;
		}
	};
	std::string oauth_token, oauth_token_secret;
	auto s = Base64::decode(accesstoken);
	if (!s.empty()) {
		s.push_back(0);
		accesstoken = (const char *) s.data();
	}
	vn::VariableJSON variableJSON;
	RefCountedPtr<Variable_object> JSONPtr = variableJSON.parse(
			accesstoken.c_str(), accesstoken.length());
	if (!JSONPtr) {
		return;
	}
	oauth_token = JSONPtr->queryAttributeString("oauth_token");
	if (oauth_token.empty()) {
		return;
	}
	oauth_token_secret = JSONPtr->queryAttributeString("oauth_token_secret");
	if (oauth_token_secret.empty()) {
		return;
	}

//	size_t pos = accesstoken.find("&");
//	if(pos == std::string::npos) {
//		//error
//		return;
//	}
//	std::string tmp_oauth_token = accesstoken.substr(0, pos);
//	std::string tmp_oauth_token_secret = accesstoken.substr(pos + 1);
//	size_t pos1 = tmp_oauth_token.find("=");
//	size_t pos2 = tmp_oauth_token_secret.find("=");
//	if(pos1 == std::string::npos || pos2 == std::string::npos) {
//		//error
//		return;
//	}
//
//	oauth_token = tmp_oauth_token.substr(pos1 + 1);
//	oauth_token_secret = tmp_oauth_token_secret.substr(pos2 + 1);
//
//	VN_LOG_DEBUG(accesstoken);
//	VN_LOG_DEBUG(oauth_token);
//	VN_LOG_DEBUG(oauth_token_secret);

	u64 curTime = time(NULL);
	std::string req_url = "http://thapi.nearme.com.cn/account/GetUserInfoByGame";

	user_check_args args;
	args.oauth_consumer_key = OPPO_AppKey;
	args.oauth_nonce = std::to_string(curTime);
	args.oauth_signature_method = "HMAC-SHA1";
	args.oauth_token = oauth_token;
	args.oauth_timestamp = std::to_string(curTime);
	args.oauth_version = "1.0";
	std::string basestring = _::get_base_string("POST", req_url, args);
	std::string key = UrlEncode(OPPO_AppSecret.c_str(), OPPO_AppSecret.length())
			+ "&"
			+ UrlEncode(oauth_token_secret.c_str(),
					oauth_token_secret.length());

	//签名R
	//hmacsha1
	HmacSha1 hmac((uint32) key.length(), (const uint8*) key.c_str());
	hmac.UpdateData(basestring);
	hmac.Finalize();
	std::string hash = ByteArrayToHexStr(hmac.GetDigest(), hmac.GetLength());
	//base64
	std::string ret = Base64::encode(hash.c_str(), hash.length());
	std::string::size_type p = ret.find("=");
	while (p != std::string::npos) {
		ret = ret.replace(p, 1, "%3D");
		p = ret.find("=");
	}
	std::string _org("OAuth ");
	_org += "oauth_consumer_key=\"" + OPPO_AppKey + "\"";
	_org += ",oauth_nonce=\"" + args.oauth_nonce + "\"";
	_org += ",oauth_signature_method=\"HMAC-SHA1\"";
	_org += ",oauth_timestamp=\"" + args.oauth_timestamp + "\"";
	_org += ",oauth_token=\"" + args.oauth_token + "\"";
	_org += ",oauth_version=\"1.0\"";
	_org += ",oauth_signature=\"" + ret + "\"";

	HttpRequestPtr aReqPtr(new HttpRequestC);
	aReqPtr->headMap.insert( { "Content-Type",
			"application/x-www-form-urlencoded" });
	aReqPtr->headMap.insert( { "Authorization", _org });
	aReqPtr->addr.from_string("60.12.231.68:80");
	aReqPtr->host = "thapi.nearme.com.cn";
	aReqPtr->isPost = true;
	aReqPtr->content = "/account/GetUserInfoByGame";
	aReqPtr->isHttps = false;
	aReqPtr->callBackFn =
			[doVerifyReqFun,&openid](const HttpResponse& _httpResponse ) {
				BufferPtr pBuffer = _httpResponse.content();
				std::string res = (const char *)pBuffer->lock();
				if(res.find("errorCode") != std::string::npos) {
					doVerifyReqFun(false);
					return;
				} else {
					vn::RefCountedPtr<vn::Variable_object> vJSONPtr = _getJsonObj(_httpResponse);
					if (vJSONPtr.get()) {
						auto obj = vJSONPtr->queryAttributeObject("BriefUser");
						if(!obj) {
							doVerifyReqFun(false);
							return;
						}
						//todo
						std::string name = obj->queryAttributeString("name");
						std::string id = obj->queryAttributeString("id");

						PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(SELECT_OPPO_ERROR_DATA_BY_OPENID);
						stmt->setString(0, name);
						PreparedQueryResult result = PassPortDatabase.SynQuery(stmt);
						//找到错误帐号
						if(result && result->GetRowCount()) {
							PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(UPDATE_ACCOUNT_OPENID);
							stmt->setString(0, id);
							stmt->setInt32(1, PLATFORM_ID_OPPO);
							stmt->setString(2, name);
							PreparedQueryResult result = PassPortDatabase.SynQuery(stmt);
							if(result) {
								PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(DELETE_OPPO_DATA_BY_OPENID);
								stmt->setString(0,name);
								PassPortDatabase.Execute(stmt, nullptr);
							}
						}
						if(id.empty()) {
							doVerifyReqFun(false);
							return;
						}
						openid = id;
						//验证通过
						doVerifyReqFun(true);
						return;
					}
					doVerifyReqFun(false);
			}
	};
	sCurlManager.CurlSend(aReqPtr);
}

void ClientSession::SendVerifyReq_Pptv(std::string& openid,
		std::string& accesstoken, std::function<void(bool)> doVerifyReqFun) {

	std::string content = "?type=login&sessionid=" + accesstoken + "&username="
			+ openid + "&app=mobgame";
	VN_LOG_DEBUG(content);
	//GET
	HttpRequestPtr aReqPtr(new HttpRequestC);
	aReqPtr->addr.from_string("211.151.139.27:80");
	aReqPtr->host = "api.user.vas.pptv.com";
	aReqPtr->isPost = false;
	aReqPtr->content = "/c/v2/cksession.php";
	aReqPtr->content += content;
	aReqPtr->isHttps = false;
	aReqPtr->callBackFn =
			[doVerifyReqFun,&openid](const HttpResponse& _httpResponse ) {
				vn::RefCountedPtr<vn::Variable_object> vJSONPtr = _getJsonObj(_httpResponse);
				if(!vJSONPtr) {
					doVerifyReqFun(false);
					return;
				}
				auto ret = vJSONPtr->queryAttributeInt32("status");
				if(ret == 1) {
					doVerifyReqFun(true);
				} else if(ret == 0) {
					doVerifyReqFun(false);
				}
			};
	sCurlManager.CurlSend(aReqPtr);
}

void ClientSession::SendVerifyReq_Gionee(std::string& openid,
		std::string& accesstoken, std::function<void(bool)> doVerifyReqFun) {

	VN_LOG_DEBUG("openid: " << openid); VN_LOG_DEBUG("accesstoken: " << accesstoken);
	auto data = Base64::decode(accesstoken);
	if (!data.empty()) {
		data.push_back(0);
		accesstoken = (const c8 *) data.data();
	}
	u32 curTime = time(NULL);
	c8 nonce[9];
	sprintf(nonce, "%08X", curTime);

	std::string authorization;
	std::string basestring;
	//hmac-sha1
	basestring.append(std::to_string(curTime)); //time
	basestring.append("\n");
	basestring.append(nonce); //nonce
	basestring.append("\n");
	basestring.append("POST\n");
	basestring.append("/account/verify.do\n");
	basestring.append("id.gionee.com\n");
	basestring.append("443\n\n");

	HmacSha1 hmac((uint32) GIONEE_SecretKey.length(),
			(const uint8*) GIONEE_SecretKey.c_str());
	hmac.UpdateData(basestring);
	hmac.Finalize();

	std::string mac = Base64::encode(hmac.GetDigest(), hmac.GetLength());

	//build Authorization
	authorization += "MAC ";
	authorization += "id=\"";
	authorization += GIONEE_APIKey + "\"";
	authorization += ",ts=\"";
	authorization += std::to_string(curTime) + "\"";
	authorization += ",nonce=\"";
	authorization.append(nonce);
	authorization += "\"";
	authorization += ",mac=\"";
	authorization += mac;
	authorization += "\"";

	VN_LOG_DEBUG("authorization: " << authorization);

	//POST
	HttpRequestPtr aReqPtr(new HttpRequestC);
	aReqPtr->addr.from_string("223.6.251.144:443");
	aReqPtr->host = "id.gionee.com";
	aReqPtr->isPost = true;
	aReqPtr->content = "/account/verify.do";
	aReqPtr->isHttps = true;
	aReqPtr->postUri = accesstoken;
	aReqPtr->headMap.insert( { "Content-Type", "application/json" });
	aReqPtr->headMap.insert( { "Authorization", authorization });
	aReqPtr->callBackFn =
			[doVerifyReqFun,&openid](const HttpResponse& _httpResponse ) {
				vn::RefCountedPtr<vn::Variable_object> vJSONPtr = _getJsonObj(_httpResponse);
				if(!vJSONPtr) {
					doVerifyReqFun(false);
					return;
				}
				auto ret = vJSONPtr->queryAttributeString("r", "0");
				if(ret != "0") {
					doVerifyReqFun(false);
					return;
				}
				doVerifyReqFun(true);
			};
	sCurlManager.CurlSend(aReqPtr);
}

void ClientSession::SendVerifyReq_Flyme(std::string& openid,
		std::string& accesstoken, std::function<void(bool)> doVerifyReqFun) {

	u64 ts = time(0);
	std::map<std::string, std::string> signMap;
	signMap.insert( { "app_id", FLYME_AppID });
	signMap.insert( { "session_id", accesstoken });
	signMap.insert( { "uid", openid });
	signMap.insert( { "ts", std::to_string(ts) });

	std::string basestring = "";
	for (auto &i : signMap) {
		basestring += i.first + "=" + i.second + "&";
	}
	basestring.erase(basestring.end() - 1);
	basestring += ":" + FLYME_AppSecret;
	//std::string basestring = "app_id=" + FLYME_AppID + "&session_id=" + accesstoken + "&uid=" + openid + "&ts=" + std::to_string(ts) + ":" + FLYME_AppSecret;
	VN_LOG_DEBUG(basestring);

	MD5 md5;
	md5.write(basestring.c_str(), basestring.length());
	std::string signValue = md5.fetch_hex_lowercase();

	std::string content = "app_id=" + FLYME_AppID + "&session_id=" + accesstoken
			+ "&uid=" + openid + "&ts=" + std::to_string(ts)
			+ "&sign_type=md5&sign=" + signValue;

	//GET
	HttpRequestPtr aReqPtr(new HttpRequestC);
	aReqPtr->addr.from_string("221.5.35.26:443");
	aReqPtr->headMap.insert( { "Content-Type",
			"application/x-www-form-urlencoded" });
	aReqPtr->host = "api.game.meizu.com";
	aReqPtr->isPost = true;
	aReqPtr->content = "/game/security/checksession";
	aReqPtr->postUri = content;
	aReqPtr->isHttps = true;
	aReqPtr->callBackFn =
			[doVerifyReqFun,&openid](const HttpResponse& _httpResponse ) {
				//BufferPtr pBuffer = _httpResponse.content();
				//VN_LOG_DEBUG(str8((char*)pBuffer->lock()));
				vn::RefCountedPtr<vn::Variable_object> vJSONPtr = _getJsonObj(_httpResponse);
				if(!vJSONPtr) {
					doVerifyReqFun(false);
					return;
				}
				auto obj = vJSONPtr->queryAttributeInt32("code");
				if(obj != 200) {
					doVerifyReqFun(false);
					return;
				}
				doVerifyReqFun(true);
			};
	sCurlManager.CurlSend(aReqPtr);
}

void ClientSession::SendVerifyreq_Appchina(std::string& openid,
		std::string& accesstoken, std::function<void(bool)> doVerifyReqFun) {

	std::string content = "login_id=11079&login_key=dMV3LDN921HHRW3J&ticket="
			+ accesstoken;

	//GET OR POST GET
	HttpRequestPtr aReqPtr(new HttpRequestC);
	aReqPtr->addr.from_string("223.202.15.99:80");
	aReqPtr->host = "api.appchina.com";
	aReqPtr->isPost = false;
	aReqPtr->content = "/appchina-usersdk/user/v2/get.json?";
	aReqPtr->content += content;
	aReqPtr->isHttps = false;
	aReqPtr->callBackFn =
			[doVerifyReqFun,&openid](const HttpResponse& _httpResponse ) {
				vn::RefCountedPtr<vn::Variable_object> vJSONPtr = _getJsonObj(_httpResponse);
				if(!vJSONPtr) {
					doVerifyReqFun(false);
					return;
				}
				auto obj = vJSONPtr->queryAttributeInt32("status");
				if(obj != 0) {
					doVerifyReqFun(false);
					return;
				}
//			auto p = vJSONPtr->queryAttributeObject("data");
//			if(!p) {
//				doVerifyReqFun(false);
//				return;
//			}
//			auto user_name = p->queryAttributeString("user_name");
//			if(user_name.empty()){
//				doVerifyReqFun(false);
//				return;
//			}
				//openid = user_name;
				doVerifyReqFun(true);
			};
	sCurlManager.CurlSend(aReqPtr);
}

void ClientSession::SendVerifyreq_4399(std::string& openid,
		std::string& accesstoken, std::function<void(bool)> doVerifyReqFun) {
	std::string content = "state=" + accesstoken + "&uid=" + openid;
	//GET
	HttpRequestPtr aReqPtr(new HttpRequestC);
	aReqPtr->addr.from_string("115.182.52.51:80");
	aReqPtr->host = "m.4399api.com";
	aReqPtr->isPost = false;
	aReqPtr->content = "/openapi/oauth-check.html?";
	aReqPtr->content += content;
	aReqPtr->isHttps = false;
	aReqPtr->callBackFn =
			[doVerifyReqFun,&openid](const HttpResponse& _httpResponse ) {
				vn::RefCountedPtr<vn::Variable_object> vJSONPtr = _getJsonObj(_httpResponse);
				if(!vJSONPtr) {
					doVerifyReqFun(false);
					return;
				}
				auto obj = vJSONPtr->queryAttributeString("code");
				if(obj != "100") {
					doVerifyReqFun(false);
					return;
				}
				doVerifyReqFun(true);
			};
	sCurlManager.CurlSend(aReqPtr);
}

void ClientSession::SendVerifyreq_I4(std::string& openid,
		std::string& accesstoken, std::function<void(bool)> doVerifyReqFun) {
	std::string content = "token=" + accesstoken;
	//GET
	HttpRequestPtr aReqPtr(new HttpRequestC);
	aReqPtr->addr.from_string("218.59.186.128:443");
	aReqPtr->host = "pay.i4.cn";
	aReqPtr->isPost = false;
	aReqPtr->content = "/member_third.action?";
	aReqPtr->content += content;
	aReqPtr->isHttps = true;
	aReqPtr->callBackFn =
			[doVerifyReqFun,&openid](const HttpResponse& _httpResponse ) {
				vn::RefCountedPtr<vn::Variable_object> vJSONPtr = _getJsonObj(_httpResponse);
				if(!vJSONPtr) {
					doVerifyReqFun(false);
					return;
				}
				auto obj = vJSONPtr->queryAttributeInt32("status");
				if(obj != 0) {
					doVerifyReqFun(false);
					return;
				}
				doVerifyReqFun(true);
			};
	sCurlManager.CurlSend(aReqPtr);
}

void ClientSession::SendVerifyreq_Coolyun(std::string& openid,
		std::string& accesstoken, std::function<void(bool)> doVerifyReqFun) {
	std::string content = "grant_type=authorization_code&client_id="
			+ COOLYUN_APPID + "&redirect_uri=" + COOLYUN_APPKEY + "&client_secret="
			+ COOLYUN_APPKEY + "&code=" + accesstoken;
	//GET
	HttpRequestPtr aReqPtr(new HttpRequestC);
	aReqPtr->addr.from_string("113.200.114.226:80");
	aReqPtr->host = "openapi.coolyun.com";
	aReqPtr->isPost = false;
	aReqPtr->content = "/oauth2/token?";
	aReqPtr->content += content;
	aReqPtr->isHttps = false;
	aReqPtr->callBackFn =
			[doVerifyReqFun,&openid](const HttpResponse& _httpResponse ) {
				vn::RefCountedPtr<vn::Variable_object> vJSONPtr = _getJsonObj(_httpResponse);
				if(!vJSONPtr) {
					doVerifyReqFun(false);
					return;
				}
				auto obj = vJSONPtr->queryAttributeString("error");
				if(!obj.empty()) {
					doVerifyReqFun(false);
					return;
				}
				openid = vJSONPtr->queryAttributeString("openid");
				doVerifyReqFun(true);
			};
	sCurlManager.CurlSend(aReqPtr);
}

void ClientSession::SendVerifyreq_Ccplay(std::string& openid,
		std::string& accesstoken, std::function<void(bool)> doVerifyReqFun) {
	//此版本无登录校验
	doVerifyReqFun(true);
}

void ClientSession::SendVerifyreq_Vivo(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun) {
	const std::string content = "access_token=" + accesstoken;

	HttpRequestPtr aReqPtr(new HttpRequestC);
		aReqPtr->addr.from_string("58.67.199.201:443");
		aReqPtr->host = "usrsys.inner.bbk.com";
		aReqPtr->isPost = true;
		aReqPtr->content = "/auth/user/info?";
		aReqPtr->content += content;
		aReqPtr->isHttps = true;
		aReqPtr->callBackFn =
				[doVerifyReqFun,&openid](const HttpResponse& _httpResponse ) {
					vn::RefCountedPtr<vn::Variable_object> vJSONPtr = _getJsonObj(_httpResponse);
					if(!vJSONPtr) {
						doVerifyReqFun(false);
						return;
					}
					auto obj = vJSONPtr->queryAttributeString("uid");
					if(obj.empty()) {
						doVerifyReqFun(false);
						return;
					}
					doVerifyReqFun(true);
				};
		sCurlManager.CurlSend(aReqPtr);
}

void ClientSession::SendVerifyreq_Haima_And(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun) {
	//POST
	const std::string content = "appid=" + HMW_AND_APPID + "&t=" + accesstoken;
		HttpRequestPtr aReqPtr(new HttpRequestC);
			aReqPtr->addr.from_string("120.26.16.210:80");
			aReqPtr->headMap.insert( { "Content-Type",
						"application/x-www-form-urlencoded" });
			aReqPtr->host = "api.haimawan.com";
			aReqPtr->isPost = true;
			aReqPtr->content = "/index.php?m=api&a=validate_token";
			aReqPtr->postUri += content;
			aReqPtr->isHttps = false;
			aReqPtr->callBackFn =
					[doVerifyReqFun,&openid](const HttpResponse& _httpResponse ) {
					BufferPtr pBuffer = _httpResponse.content();
					if (!pBuffer) {
						doVerifyReqFun(false);
						return;
					}
					VN_LOG_DEBUG((char*)pBuffer->lock());
					const std::string ret = (char*)pBuffer->lock();
					if(ret == "success") {
						doVerifyReqFun(true);
						return;
					} else {
						doVerifyReqFun(false);
						return;
					}
			};
			sCurlManager.CurlSend(aReqPtr);
}

void ClientSession::SendVerifyreq_Haima_Ios(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun) {
	//POST
	const std::string content = "appid=" + HMW_IOS_APPID + "&t=" + accesstoken;
		HttpRequestPtr aReqPtr(new HttpRequestC);
			aReqPtr->addr.from_string("120.26.16.210:80");
			aReqPtr->headMap.insert( { "Content-Type",
									"application/x-www-form-urlencoded" });
			aReqPtr->host = "api.haimawan.com";
			aReqPtr->isPost = true;
			aReqPtr->content = "/index.php?m=api&a=validate_token";
			aReqPtr->postUri += content;
			aReqPtr->isHttps = false;
			aReqPtr->callBackFn =
					[doVerifyReqFun,&openid](const HttpResponse& _httpResponse ) {
					BufferPtr pBuffer = _httpResponse.content();
					if (!pBuffer) {
						doVerifyReqFun(false);
						return;
					}
					const std::string ret = (char*)pBuffer->lock();
					if(ret == "success") {
						doVerifyReqFun(true);
						return;
					} else {
						doVerifyReqFun(false);
						return;
					}
			};
			sCurlManager.CurlSend(aReqPtr);
}

void ClientSession::SendVerifyreq_TBT(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun) {
	const std::string content = "session=" + accesstoken + "&appid=" + TBT_APPID;

		HttpRequestPtr aReqPtr(new HttpRequestC);
			aReqPtr->addr.from_string("112.65.220.34:80");
			aReqPtr->host = "tgi.tongbu.com";
			aReqPtr->isPost = false;
			aReqPtr->content = "/api/LoginCheck.ashx?";
			aReqPtr->content += content;
			aReqPtr->isHttps = false;
			aReqPtr->callBackFn =
					[doVerifyReqFun,&openid](const HttpResponse& _httpResponse ) {
						//vn::RefCountedPtr<vn::Variable_object> vJSONPtr = _getJsonObj(_httpResponse);
//						if(!vJSONPtr) {
//							doVerifyReqFun(false);
//							return;
//						}
//						auto obj = vJSONPtr->queryAttributeString("uid");
//						if(obj.empty()) {
//							doVerifyReqFun(false);
//							return;
//						}
//						doVerifyReqFun(true);
				BufferPtr pBuffer = _httpResponse.content();
				if(!pBuffer) {
					doVerifyReqFun(false);
					return;
				}
				std::string ret = (char*)pBuffer->lock();
				if(ret == "0") {
					doVerifyReqFun(false);
					return;
				}else if (ret == "-1") {
					doVerifyReqFun(false);
					return;
				}
				doVerifyReqFun(true);
			};
			sCurlManager.CurlSend(aReqPtr);
}

void ClientSession::SendVerifyreq_PPS(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun) {
	//此版本无登录校验
	doVerifyReqFun(true);
}

void ClientSession::SendVerifyreq_Sogou(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun) {

	std::string content = "gid=" + SOGOU_GID + "&session_key=" + accesstoken + "&user_id=" + openid + "&";
	//content = UrlEncode(content.c_str(), content.length());
	std::string sign = content + SOGOU_APPSECRET;
	MD5 md5;
	md5.write(sign.c_str(), sign.length());
	sign = md5.fetch_hex_lowercase();
	content += "auth=" + sign;

			HttpRequestPtr aReqPtr(new HttpRequestC);
			aReqPtr->addr.from_string("36.110.147.35:80");
			aReqPtr->host = "api.app.wan.sogou.com";
			//aReqPtr->addr.from_string("123.126.68.46:80");
			//aReqPtr->host = "dev.app.wan.sogou.com";
			aReqPtr->isPost = true;
			aReqPtr->content = "/api/v1/login/verify?";
			aReqPtr->content += content;
			aReqPtr->isHttps = false;
			aReqPtr->callBackFn =
					[doVerifyReqFun,&openid](const HttpResponse& _httpResponse ) {
						vn::RefCountedPtr<vn::Variable_object> vJSONPtr = _getJsonObj(_httpResponse);
						if(!vJSONPtr) {
							doVerifyReqFun(false);
							return;
						}
						auto obj = vJSONPtr->queryAttributeBool("result");
						if(obj == true) {
							doVerifyReqFun(true);
							return;
						}
						doVerifyReqFun(false);
			};
			sCurlManager.CurlSend(aReqPtr);
}

void ClientSession::SendVerifyreq_Sina(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun) {
	auto s = Base64::decode(accesstoken);
	if (!s.empty()) {
		s.push_back(0);
		accesstoken = (const char *) s.data();
	}
	vn::VariableJSON variableJSON;
	vn::RefCountedPtr<vn::Variable_object> vJSONPtr = variableJSON.parse(accesstoken.c_str(), accesstoken.length());
	if(!vJSONPtr) {
		doVerifyReqFun(false);
		return;
	}

	//HTTP POST
	std::string deviceid = vJSONPtr->queryAttributeString("deviceId");
	std::string token = vJSONPtr->queryAttributeString("token");;
	std::string content = "appkey=" + SINA_APPID + "&deviceid=" + deviceid + "&suid="+ openid + "&token=" + token;
	std::string signature = content + "|" + SINA_SIGNKEY;
	vn::MD5 md5;
	md5.write(signature.c_str(), signature.length());
	signature = md5.fetch_hex_lowercase();
	content += "&signature=" + signature;

	HttpRequestPtr aReqPtr(new HttpRequestC);
				aReqPtr->addr.from_string("123.125.29.250:80");
				aReqPtr->headMap.insert( { "Content-Type",
										"application/x-www-form-urlencoded" });
				aReqPtr->host = "m.game.weibo.cn";
				aReqPtr->isPost = true;
				aReqPtr->content = "/api/sdk/user/check.json";
				aReqPtr->postUri = content;
				aReqPtr->isHttps = false;
				aReqPtr->callBackFn =
						[doVerifyReqFun,&openid](const HttpResponse& _httpResponse ) {
							vn::RefCountedPtr<vn::Variable_object> vJSONPtr = _getJsonObj(_httpResponse);
							if(!vJSONPtr) {
								doVerifyReqFun(false);
								return;
							}
							auto obj = vJSONPtr->queryAttributeString("error");
							if(!obj.empty()) {
								doVerifyReqFun(false);
								return;
							}
							doVerifyReqFun(true);
				};
				sCurlManager.CurlSend(aReqPtr);
}

void ClientSession::SendVerifyreq_GFAN(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun) {
	//GET
	std::string content = "token=" + accesstoken;

	HttpRequestPtr aReqPtr(new HttpRequestC);
					aReqPtr->addr.from_string("117.79.130.165:80");
					aReqPtr->host = "api.gfan.com";
					aReqPtr->isPost = true;
					aReqPtr->content = "/uc1/common/verify_token?";
					aReqPtr->content += content;
					aReqPtr->isHttps = false;
					aReqPtr->callBackFn =
							[doVerifyReqFun,&openid](const HttpResponse& _httpResponse ) {
								vn::RefCountedPtr<vn::Variable_object> vJSONPtr = _getJsonObj(_httpResponse);
								if(!vJSONPtr) {
									doVerifyReqFun(false);
									return;
								}
								auto obj = vJSONPtr->queryAttributeInt32("resultCode");
								if(obj != 1) {
									doVerifyReqFun(false);
									return;
								}
								doVerifyReqFun(true);
					};
					sCurlManager.CurlSend(aReqPtr);
}

void ClientSession::SendVerifyreq_Youku(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun) {
	//POST
	std::string basestring = "appkey=" + YOUKU_APPKEY + "&sessionid=" + accesstoken;
	HmacMd5 md5(YOUKU_PAYKEY.length(), (uint8 const *)YOUKU_PAYKEY.c_str());
	md5.UpdateData(basestring);
	md5.Finalize();
	std::string sign = ByteArrayToHexStr(md5.GetDigest(),md5.GetLength());
	std::string content = basestring + "&sign=" + sign;

	HttpRequestPtr aReqPtr(new HttpRequestC);
	aReqPtr->addr.from_string("111.206.111.21:80");
	aReqPtr->host = "sdk.api.gamex.mobile.youku.com";
	aReqPtr->isPost = true;
	aReqPtr->content = "/game/user/infomation?";
	aReqPtr->content += content;
	aReqPtr->isHttps = false;
	aReqPtr->callBackFn =
			[doVerifyReqFun,&openid](const HttpResponse& _httpResponse ) {
				vn::RefCountedPtr<vn::Variable_object> vJSONPtr = _getJsonObj(_httpResponse);
				if(!vJSONPtr) {
					doVerifyReqFun(false);
					return;
				}
				auto ret = vJSONPtr->queryAttributeString("status");
				if(ret != "success") {
					doVerifyReqFun(false);
					return;
				}
				auto uid = vJSONPtr->queryAttributeInt32("uid");
				openid = std::to_string(uid);
				doVerifyReqFun(true);
	};
	sCurlManager.CurlSend(aReqPtr);
}

void ClientSession::SendVerifyreq_IAPPLE(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun) {
	//GET
	std::string basestring = "game_id=" + IAPPLE_GAMEKEY + "&session=" + accesstoken + "&user_id=" + openid;
	vn::MD5 md5;
	md5.write(basestring.c_str(), basestring.length());
	std::string sign_calc = md5.fetch_hex_lowercase();
	sign_calc += IAPPLE_SECRETKEY;
	md5.write(sign_calc.c_str(), sign_calc.length());
	basestring += "&_sign=" + md5.fetch_hex_lowercase();

	HttpRequestPtr aReqPtr(new HttpRequestC);
	aReqPtr->addr.from_string("124.202.152.246:80");
	aReqPtr->host = "ucenter.iiapple.com";
	aReqPtr->isPost = false;
	aReqPtr->content = "/foreign/oauth/verification.php?";
	aReqPtr->content += basestring;
	aReqPtr->isHttps = false;
	aReqPtr->callBackFn =
			[doVerifyReqFun,&openid](const HttpResponse& _httpResponse ) {
				vn::RefCountedPtr<vn::Variable_object> vJSONPtr = _getJsonObj(_httpResponse);
				if(!vJSONPtr) {
					doVerifyReqFun(false);
					return;
				}
				auto ret = vJSONPtr->queryAttributeInt32("status");
				if(ret != 1) {
					doVerifyReqFun(false);
					return;
				}
				doVerifyReqFun(true);
	};
	sCurlManager.CurlSend(aReqPtr);

}

void ClientSession::SendVerifyreq_XY(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun){
	//POST 方式
	HttpRequestPtr aReqPtr(new HttpRequestC);

	//正式环境： http://passport.xyzs.com/checkLogin.php
	aReqPtr->addr.from_string("121.201.0.112:80");
	aReqPtr->host = "passport.xyzs.com";
	aReqPtr->headMap.insert({"Content-Type", "application/x-www-form-urlencoded"});
	aReqPtr->content = "/checkLogin.php";
	aReqPtr->isPost = true;

	aReqPtr->postUri = "uid=";
	aReqPtr->postUri += openid;
	aReqPtr->postUri += "&appid=";
	aReqPtr->postUri += XY_AppId;
	aReqPtr->postUri += "&token=";
	aReqPtr->postUri += accesstoken;


	aReqPtr->isHttps = false;

	aReqPtr->callBackFn = [&openid,doVerifyReqFun](const HttpResponse& _httpResponse ) {
		//返回格式：JSON
		vn::RefCountedPtr<vn::Variable_object>	vJSONPtr = _getJsonObj(_httpResponse);
		if (!vJSONPtr) {
			doVerifyReqFun(false);
			return;
		}
		auto ret = vJSONPtr->queryAttributeInt32("ret");
		if(ret != 0) {
			doVerifyReqFun(false);
			return;
		}
		doVerifyReqFun(true);
	};
	sCurlManager.CurlSend(aReqPtr);
}

void ClientSession::SendVerifyreq_19196(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun){
	//POST 方式
		HttpRequestPtr aReqPtr(new HttpRequestC);

		//正式环境： http://ucapi.411game.com/Api/checkToken
		aReqPtr->addr.from_string("36.248.12.93:80");
		aReqPtr->host = "ucapi.411game.com";
		aReqPtr->content = "/Api/checkToken?";

		aReqPtr->isPost = true;

		//aReqPtr->postUri = "ip=";
		//aReqPtr->postUri += openid;
		aReqPtr->content += "token=";
		aReqPtr->content += accesstoken;
		aReqPtr->content += "&pid=";
		aReqPtr->content += P_19196_Pid;

		aReqPtr->isHttps = false;
		aReqPtr->callBackFn = [&openid,doVerifyReqFun](const HttpResponse& _httpResponse ) {
			//返回格式：JSON
			vn::RefCountedPtr<vn::Variable_object>	vJSONPtr = _getJsonObj(_httpResponse);
			if (!vJSONPtr) {
				doVerifyReqFun(false);
				return;
			}
			auto state = vJSONPtr->queryAttributeString("state");
			if(state != "1") {
				doVerifyReqFun(false);
				return;
			}
//			auto username = vJSONPtr->queryAttributeString("username");
//			if(username.empty()) {
//				doVerifyReqFun(false);
//				return;
//			}
			//验证通过
			doVerifyReqFun(true);
		};
		sCurlManager.CurlSend(aReqPtr);
}

void ClientSession::SendVerifyreq_Wandoujia(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun){
	//GET 方式
	HttpRequestPtr aReqPtr(new HttpRequestC);

	//https://pay.wandoujia.com/api/uid/check
	aReqPtr->addr.from_string("60.28.208.21:443");
	aReqPtr->host = "pay.wandoujia.com";
	aReqPtr->content = "/api/uid/check";

	aReqPtr->isPost = false;

	aReqPtr->content += "?token=";
	aReqPtr->content += accesstoken;
	aReqPtr->content += "&appkey_id=";
	aReqPtr->content += WANDOUJIA_AppKeyID;
	aReqPtr->content += "&uid=";
	aReqPtr->content += openid;

	aReqPtr->isHttps = true;
	aReqPtr->callBackFn = [doVerifyReqFun](const HttpResponse& _httpResponse ) {
		//token 有效，则返回字符串 true ，无效则返回 false
		BufferPtr pBuffer = _httpResponse.content();
		if(!pBuffer) {
			doVerifyReqFun(false);
			return;
		}
		std::string ret = (const char*)pBuffer->lock();
		if( ret!="true") {
			//验证失败
			doVerifyReqFun(false);
			return;
		} else {
			//验证通过
			doVerifyReqFun(true);
		}
	};
	sCurlManager.CurlSend(aReqPtr);
}

void ClientSession::SendVerifyreq_Muzhiwan(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun){
	//GET 方式
	HttpRequestPtr aReqPtr(new HttpRequestC);

	//http://sdk.muzhiwan.com/oauth2/getuser.php
	aReqPtr->addr.from_string("114.112.50.13:80");
	aReqPtr->host = "sdk.muzhiwan.com";
	aReqPtr->content = "/oauth2/getuser.php";

	aReqPtr->isPost = false;

	aReqPtr->content += "?token=";
	aReqPtr->content += accesstoken;
	aReqPtr->content += "&appkey=";
	aReqPtr->content += MUZHIWAN_KEY;


	aReqPtr->isHttps = false;
	aReqPtr->callBackFn = [&openid,doVerifyReqFun](const HttpResponse& _httpResponse ) {
		//返回格式：json
		vn::RefCountedPtr<vn::Variable_object>	vJSONPtr = _getJsonObj(_httpResponse);
		if (vJSONPtr) {
			RefCountedPtr<Variable_object> userObj = vJSONPtr->queryAttributeObject("user");
			if (userObj ) {
				openid = userObj->queryAttributeString("username");
			}
		}
		if( openid.empty()) {
			//验证失败
			doVerifyReqFun(false);
			return;
		} else {
			//验证通过
			doVerifyReqFun(true);
		}
	};
	sCurlManager.CurlSend(aReqPtr);
}
void ClientSession::SendVerifyreq_Downjoy(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun){
	//GET 方式
	HttpRequestPtr aReqPtr(new HttpRequestC);

	//ngsdk.d.cn/api/cp/checkToken
	aReqPtr->addr.from_string("211.147.5.133:80");
	aReqPtr->host = "ngsdk.d.cn";
	aReqPtr->content = "/api/cp/checkToken";

	aReqPtr->isPost = false;

	aReqPtr->content += "?token=";
	aReqPtr->content += accesstoken;
	aReqPtr->content += "&appid=";
	aReqPtr->content += DOWNJON_APP_ID;
	aReqPtr->content += "&umid=";
	aReqPtr->content += openid;
	aReqPtr->content += "&sig=";

	MD5 md5;
	std::string basestring = DOWNJON_APP_ID + "|" + DOWNJON_APP_KEY + "|"
			+ accesstoken + "|" + openid;
	md5.write(basestring.c_str(), basestring.length());
	aReqPtr->content += md5.fetch_hex_lowercase();

	aReqPtr->isHttps = false;
	aReqPtr->callBackFn = [doVerifyReqFun](const HttpResponse& _httpResponse ) {
		//返回格式：JSON
		vn::RefCountedPtr<vn::Variable_object>	vJSONPtr = _getJsonObj(_httpResponse);
		if (vJSONPtr.get()) {
			RefCountedPtr<Variable_string> validObj = vJSONPtr->findAttribute("valid").dyn_cast<vn::Variable_string>();
			if (validObj && validObj->value() == "1") {
				//验证通过
				doVerifyReqFun(true);
			} else {
				//验证通过
				doVerifyReqFun(true);
			}
		}
	};
	sCurlManager.CurlSend(aReqPtr);
}

void ClientSession::SendVerifyreq_QQ(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun){

	std::string timestamp = std::to_string(time(NULL));

	MD5 md5;
	std::string basestring = QQ_APPKEY + timestamp;
	md5.write(basestring.c_str(), basestring.length());
	std::string sign = md5.fetch_hex_lowercase();

	//POST 方式
	HttpRequestPtr aReqPtr(new HttpRequestC);

	//正式环境： http://msdk.qq.com/
	aReqPtr->addr.from_string("180.153.218.232:80");
	aReqPtr->host = "msdk.qq.com";
	aReqPtr->content = "/auth/verify_login/";
	aReqPtr->content += "?timestamp=";
	aReqPtr->content += timestamp;
	aReqPtr->content += "&appid=";
	aReqPtr->content += QQ_APPID;
	aReqPtr->content += "&sig=";
	aReqPtr->content += sign;
	aReqPtr->content += "&openid=";
	aReqPtr->content += openid;
	aReqPtr->content += "&encode=1";
	aReqPtr->headMap.insert({"Content-Type", "application/x-www-form-urlencoded"});
	aReqPtr->lineBreak = "\r\n";
	aReqPtr->isPost = true;
	auto netaddress = this->peer();
	std::string ip = netaddress.to_string();
	ip = ip.substr(0, ip.find(":"));

	Variable_object vobj;
	vobj.setAttribute("appid", vnnew Variable_int32(1104792347));
	vobj.setAttribute("openid", vnnew Variable_string(openid));
	vobj.setAttribute("openkey", vnnew Variable_string(accesstoken));
	vobj.setAttribute("userip", vnnew Variable_string(ip));
	std::string content = VariableJSON::generate(&vobj);
	aReqPtr->postUri += content;

	aReqPtr->isHttps = false;
	aReqPtr->callBackFn = [&openid,doVerifyReqFun](const HttpResponse& _httpResponse ) {
		//返回格式：JSON
		vn::RefCountedPtr<vn::Variable_object>	vJSONPtr = _getJsonObj(_httpResponse);
		if (!vJSONPtr) {
			doVerifyReqFun(false);
			return;
		}
		auto ret = vJSONPtr->queryAttributeInt32("ret");
		if(ret != 0) {
			doVerifyReqFun(false);
			return;
		}
		doVerifyReqFun(true);
	};
	sCurlManager.CurlSend(aReqPtr);
}

void ClientSession::SendVerifyreq_WX(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun){
	std::string timestamp = std::to_string(time(NULL));

		MD5 md5;
		std::string basestring = WX_APPKEY + timestamp;
		md5.write(basestring.c_str(), basestring.length());
		std::string sign = md5.fetch_hex_lowercase();

		//POST 方式
		HttpRequestPtr aReqPtr(new HttpRequestC);

		//正式环境： http://msdk.qq.com/
		aReqPtr->addr.from_string("180.153.218.232:80");
		aReqPtr->host = "msdk.qq.com";
		aReqPtr->content = "/auth/check_token/";
		aReqPtr->content += "?timestamp=";
		aReqPtr->content += timestamp;
		aReqPtr->content += "&appid=";
		aReqPtr->content += WX_APPID;
		aReqPtr->content += "&sig=";
		aReqPtr->content += sign;
		aReqPtr->content += "&openid=";
		aReqPtr->content += openid;
		aReqPtr->content += "&encode=1";
		aReqPtr->headMap.insert({"Content-Type", "application/x-www-form-urlencoded"});
		aReqPtr->isPost = true;
		aReqPtr->lineBreak = "\r\n";
		Variable_object vobj;
		vobj.setAttribute("accessToken", vnnew Variable_string(accesstoken));
		vobj.setAttribute("openid", vnnew Variable_string(openid));
		std::string content = VariableJSON::generate(&vobj);
		aReqPtr->postUri += content;

		aReqPtr->isHttps = false;
		aReqPtr->callBackFn = [&openid,doVerifyReqFun](const HttpResponse& _httpResponse ) {
			//返回格式：JSON
			vn::RefCountedPtr<vn::Variable_object>	vJSONPtr = _getJsonObj(_httpResponse);
			if (!vJSONPtr) {
				doVerifyReqFun(false);
				return;
			}
			auto ret = vJSONPtr->queryAttributeInt32("ret");
			if(ret != 0) {
				doVerifyReqFun(false);
				return;
			}
			doVerifyReqFun(true);
		};
		sCurlManager.CurlSend(aReqPtr);
}

void ClientSession::SendVerifyreq_GUOPAN_AND(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun) {
	std::string timestamp = std::to_string(time(NULL));

	MD5 md5;
	std::string basestring = openid + GUOPAN_APP_ID_AND + timestamp + GUOPAN_APP_KEY_AND;
	md5.write(basestring.c_str(), basestring.length());
	std::string sign = md5.fetch_hex_lowercase();

	//POST 方式
	HttpRequestPtr aReqPtr(new HttpRequestC);

	//正式环境： http://guopan.cn/gamesdk/verify/
	aReqPtr->addr.from_string("112.90.38.164:80");
	aReqPtr->host = "guopan.cn";
	aReqPtr->content = "/gamesdk/verify/";
	aReqPtr->content += "?game_uin=";
	aReqPtr->content += openid;
	aReqPtr->content += "&appid=";
	aReqPtr->content += GUOPAN_APP_ID_AND;
	aReqPtr->content += "&token=";
	aReqPtr->content += accesstoken;
	aReqPtr->content += "&t=";
	aReqPtr->content += timestamp;
	aReqPtr->content += "&sign=";
	aReqPtr->content += sign;
	aReqPtr->headMap.insert( { "Content-Type", "application/x-www-form-urlencoded" });
	aReqPtr->isPost = false;
	aReqPtr->lineBreak = "\r\n";


	aReqPtr->isHttps = false;
	aReqPtr->callBackFn = [&openid,doVerifyReqFun](const HttpResponse& _httpResponse ) {
			BufferPtr pBuffer = _httpResponse.content();
			if (!pBuffer) {
				doVerifyReqFun(false);
				return;
			}
			std::string ret = (char*) pBuffer->lock();
			if(ret == "true") {
				doVerifyReqFun(true);
				return;
			} else {
				doVerifyReqFun(false);
				return;
			}
		};
	sCurlManager.CurlSend(aReqPtr);
}
void ClientSession::SendVerifyreq_GUOPAN_IOS(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun) {
	std::string timestamp = std::to_string(time(NULL));

	MD5 md5;
	std::string basestring = openid + GUOPAN_APP_ID_IOS + timestamp + GUOPAN_APP_KEY_IOS;
	md5.write(basestring.c_str(), basestring.length());
	std::string sign = md5.fetch_hex_lowercase();

	//POST 方式
	HttpRequestPtr aReqPtr(new HttpRequestC);

	//正式环境： http://guopan.cn/gamesdk/verify/
	aReqPtr->addr.from_string("112.90.38.164:80");
	aReqPtr->host = "guopan.cn";
	aReqPtr->content = "/gamesdk/verify/";
	aReqPtr->content += "?game_uin=";
	aReqPtr->content += openid;
	aReqPtr->content += "&appid=";
	aReqPtr->content += GUOPAN_APP_ID_IOS;
	aReqPtr->content += "&token=";
	aReqPtr->content += accesstoken;
	aReqPtr->content += "&t=";
	aReqPtr->content += timestamp;
	aReqPtr->content += "&sign=";
	aReqPtr->content += sign;
	aReqPtr->headMap.insert( { "Content-Type", "application/x-www-form-urlencoded" });
	aReqPtr->isPost = false;
	aReqPtr->lineBreak = "\r\n";

	aReqPtr->isHttps = false;
	aReqPtr->callBackFn = [&openid,doVerifyReqFun](const HttpResponse& _httpResponse ) {
		BufferPtr pBuffer = _httpResponse.content();
		if (!pBuffer) {
			doVerifyReqFun(false);
			return;
		}
		std::string ret = (char*) pBuffer->lock();
		if(ret == "true") {
			doVerifyReqFun(true);
			return;
		} else {
			doVerifyReqFun(false);
			return;
		}
	};
	sCurlManager.CurlSend(aReqPtr);
}

void ClientSession::SendVerifyreq_PP_IOS(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun){
	std::string timestamp = std::to_string(time(NULL));

	MD5 md5;
	std::string basestring = accesstoken + PP_APP_KEY_IOS;
	md5.write(basestring.c_str(), basestring.length());
	std::string sign = md5.fetch_hex_lowercase();

	//POST 方式
	HttpRequestPtr aReqPtr(new HttpRequestC);

	//正式环境： http://passport_i.25pp.com:8080/account?tunnel-command=2852126760
	aReqPtr->addr.from_string("122.13.176.126:8080");
	aReqPtr->host = "passport_i.25pp.com";
	aReqPtr->content = "/account?tunnel-command=2852126760";

	aReqPtr->headMap.insert( { "Content-Type", "application/x-www-form-urlencoded" });
	aReqPtr->isPost = true;
	Variable_object vobj;
	vobj.setAttribute("id", vnnew Variable_int64(time(NULL)));
	vobj.setAttribute("service", vnnew Variable_string("account.verifySession"));

	Variable_object dataObj;
	dataObj.setAttribute("sid",  vnnew Variable_string( accesstoken));
	vobj.setAttribute("data", vnnew Variable_object(dataObj));

	Variable_object gameObj;
	gameObj.setAttribute("gameId",  vnnew Variable_string( PP_APP_ID_IOS));
	vobj.setAttribute("game", vnnew Variable_object(gameObj));

	vobj.setAttribute("encrypt", vnnew Variable_string("MD5"));
	vobj.setAttribute("sign", vnnew Variable_string(sign));

	std::string content = VariableJSON::generate(&vobj);
	aReqPtr->postUri += content;

	aReqPtr->lineBreak = "\r\n";
//{"data":"{sid:}","encrypt":"MD5","game":"{gameId:7049}","id":1449297269,"service":"account.verifySession","sign":"8b7305b4b4935635fd731ee235a103f0"}^M
	aReqPtr->isHttps = false;
	aReqPtr->callBackFn = [&openid,doVerifyReqFun](const HttpResponse& _httpResponse ) {
		//返回格式：JSON
		vn::RefCountedPtr<vn::Variable_object> vJSONPtr = _getJsonObj(_httpResponse);
		if (!vJSONPtr) {
			doVerifyReqFun(false);
			return;
		}
		vn::RefCountedPtr<vn::Variable_object>  state = vJSONPtr->queryAttributeObject("state");
		if(!state) {
			doVerifyReqFun(false);
			return;
		}
		int ret = state->queryAttributeInt32("code");
		if(ret != 1) {
			doVerifyReqFun(false);
			return;
		}
		vn::RefCountedPtr<vn::Variable_object>  data = vJSONPtr->queryAttributeObject("data");
		if(!data) {
			doVerifyReqFun(false);
			return;
		}
		openid = data->queryAttributeString("creator");
		openid += data->queryAttributeString("accountId");
		doVerifyReqFun(true);
	};
	sCurlManager.CurlSend(aReqPtr);
}

//棱镜
void ClientSession::SendVerifyreq_XINMEI365(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun){
		//GET 方式
		HttpRequestPtr aReqPtr(new HttpRequestC);

		//正式环境： http://gameproxy.xinmei365.com/game_agent/checkLogin
		aReqPtr->addr.from_string("119.29.43.18:80");
		aReqPtr->host = "gameproxy.xinmei365.com";
		aReqPtr->content = "/game_agent/checkLogin";
		aReqPtr->content += "?productCode=";
		aReqPtr->content += XINMEI365_PRODUCT_CODE;
		aReqPtr->content += "&token=";
		aReqPtr->content += XINMEI365_TOKEN;
		aReqPtr->content += "&channel=";
		aReqPtr->content += XINMEI365_CHANNEL;
		aReqPtr->content += "&userId=";
		aReqPtr->content += openid;

		aReqPtr->headMap.insert( { "Content-Type", "application/x-www-form-urlencoded" });
		aReqPtr->isPost = false;
		aReqPtr->lineBreak = "\r\n";
		aReqPtr->isHttps = false;

		aReqPtr->callBackFn = [&openid,doVerifyReqFun](const HttpResponse& _httpResponse ) {
			BufferPtr pBuffer = _httpResponse.content();
			if (!pBuffer) {
				doVerifyReqFun(false);
				return;
			}
			std::string ret = (char*) pBuffer->lock();
			if(ret == "true") {
				doVerifyReqFun(true);
				return;
			} else {
				doVerifyReqFun(false);
				return;
			}
		};
		sCurlManager.CurlSend(aReqPtr);
}
