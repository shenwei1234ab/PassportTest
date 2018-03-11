/*
 * FriendsvrHandler.cpp
 *
 *  Created on: 2015年8月14日
 *      Author: leave
 */




#include "../../passportsvr/gamesvrlisten/GamesvrSession.h"
#include "../../passportsvr/Protocols.h"
#include "../../passportsvr/WorldManager.h"
#include "../../passportsvr/manager/NicknameManager.h"
#include "../../passportsvr/manager/DirtyManager2.h"
#include "vnLog.h"
#include "../database/PassPortDatabaseConnection.h"
#include "vnBase64.h"
#include "curl/CurlWrapper.h"
#include "curl/CurlManager.h"

extern PassPortDatabaseWorkerPool PassPortDatabase;

extern vn::RefCountedPtr<vn::Variable_object> _getJsonObj(const vn::HttpResponse& _httpResponse);

/****************************************************/
/********** friendsvr ******************/
/****************************************************/
//------------第三方验证请求-----
VN_IMPL_PACKET_REQUEST2(GamesvrSession, pkt, req_Verify) {
	VN_LOG_DEBUG("req_Verify: pkt->data.ppid = "<<pkt->data.ppid << "pkt->data.ppToken = "<<pkt->data.ppToken);
	//查询该openId的玩家是否已经存在
	std::vector<u8> ppidVtr = Base64::decode(pkt->data.ppid);
	if(ppidVtr.size() != sizeof(u64)){
		VN_LOG_ERROR("pkt->data.ppid = "<<pkt->data.ppid << "Base64::decode error!");
		return;
	}
	u64 openId = 0;
	memcpy(&openId, ppidVtr.data(), sizeof(u64));


	VN_LOG_DEBUG("openId = "<<openId );
	//todo test
	/*u32 gameId = 1;
	SessionDataPtr pSessionData = sWorldManager.getSessionData(openId,gameId);
	if (pSessionData.get()) {
		//验证成功
		ret = 0;
		VN_LOG_DEBUG("req_ValidateUser:find ppID : "<<openId);

		std::vector<u8> pptokenVtr = Base64::decode(pkt->data.ppToken);

		u32 accessToken = 0;
		if (!pptokenVtr.empty()) {
			memcpy(&accessToken, pptokenVtr.data(), sizeof(accessToken));
		}
		if (pSessionData->pptoken != accessToken) {
			//验证失败,token 无效
			ret = -2;
		}
	} else {
		//验证失败
		ret = -1;
	}*/
	//验证玩家token是否合法，是否过期

	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_SEL_PPTOKEN);
	stmt->setUInt64(0, openId);
	u64 handle = pkt->data.handle;
	std::vector<u8> pptokenVtr = Base64::decode(pkt->data.ppToken);
	u32 accessToken = 0;
	if (pptokenVtr.size() == sizeof(accessToken)) {
		memcpy(&accessToken, pptokenVtr.data(), sizeof(accessToken));
	}
	PassPortDatabase.Query(stmt, { this, true }, [this,handle,accessToken](PreparedQueryResult result) {
		int ret = 0;
		pkt::res_JsonDate res;
		res.data.handle = handle;
		res.data.result = 0;
		if (result && result->GetRowCount()) {
			uint64 ppToken = (*result)[0].GetUInt64();
			uint64 deadline = (*result)[1].GetUInt64();
			if( ppToken == accessToken && deadline-time(NULL) >0 ) {
				ret = 0;
			} else {
				ret = -2;
				VN_LOG_ERROR("friendsvr: ppid[?] token["<< ppToken<<"] error ! accessToken["<<accessToken<<"]");
			}
		}else{
			ret = -1;
		}
		char buffer[512];
		sprintf(buffer, "{\n \"ret\":%d  \n}", ret);
		res.data.jsonvalue = buffer;
		this->sendPacket(res);
	});

}
VN_IMPL_PACKET_REQUEST2(GamesvrSession, pkt, req_GetVip) {
	int ret = 0;
	//todo
	pkt::res_JsonDate res;
	res.data.handle = pkt->data.handle;
	res.data.result = 0;
	char buffer[512];
	sprintf(buffer, "{\n \"ret\":%d  \n}", ret);
	res.data.jsonvalue = buffer;
	this->sendPacket(res);
}

VN_IMPL_PACKET_REQUEST2(GamesvrSession, pkt, req_GetProfile) {
	//查询该openId的玩家是否已经存在
	std::vector<u8> ppidVtr = Base64::decode(pkt->data.ppid);
	if(ppidVtr.size() != sizeof(u64)){
		VN_LOG_ERROR("pkt->data.ppid = "<<pkt->data.ppid << "Base64::decode error!");
		return;
	}
	u64 openId = 0;
	memcpy(&openId, ppidVtr.data(), sizeof(u64));
	std::string nickName="";
	AccountPtr pAccount = sWorldManager.GetAccountInfo(openId);
	if(pAccount.get()){
		nickName = pAccount->nickname;
	}
	int ret = 0;
	//todo 获取昵称
	pkt::res_JsonDate res;
	res.data.handle = pkt->data.handle;
	res.data.result = 0;
	char buffer[512];
	sprintf(buffer, "{\n \"ret\":%d ,\"lists\":[{\"nickName\":\"%s\"}] \n}", ret,nickName.c_str());
	res.data.jsonvalue = buffer;
	this->sendPacket(res);
}
VN_IMPL_PACKET_REQUEST2(GamesvrSession, pkt, req_GetToken) {
	int ret = 0;
	//todo
	pkt::res_JsonDate res;
	res.data.handle = pkt->data.handle;
	res.data.result = 0;
	char buffer[512];
	sprintf(buffer, "{\n \"ret\":%d  \n}", ret);
	res.data.jsonvalue = buffer;
	this->sendPacket(res);
}
VN_IMPL_PACKET_REQUEST2(GamesvrSession, pkt, req_RefreshToken) {
	int ret = 0;
	//todo
	pkt::res_JsonDate res;
	res.data.handle = pkt->data.handle;
	res.data.result = 0;
	char buffer[512];
	sprintf(buffer, "{\n \"ret\":%d  \n}", ret);
	res.data.jsonvalue = buffer;
	this->sendPacket(res);
}
void GamesvrSession::getGotyeAccessToken(std::function<void(bool)> doCallBackFun){

	u64 curTime = time(NULL);
	if(curTime > sWorldManager.expires_in){
		VN_LOG_DEBUG("expires_in:"<<sWorldManager.expires_in<<"  curTime:"<<curTime);
		//token过期
		//https://rest.gotye.com.cn/api/GetFriends
		HttpRequestPtr aReqPtr(new HttpRequestC);
		aReqPtr->addr.from_string("42.62.79.105:443");
		aReqPtr->host = "rest.gotye.com.cn";
		aReqPtr->content = "/api/accessToken";
		aReqPtr->isPost = true;
		aReqPtr->isHttps = true;

		aReqPtr->postUri = "{\n";
		aReqPtr->postUri += "\"username\":\"";
		aReqPtr->postUri += sWorldManager.GOTYE_UserName;
		aReqPtr->postUri += "\",";
		aReqPtr->postUri += "\"grant_type\":\"";
		aReqPtr->postUri += "password";
		aReqPtr->postUri += "\",";
		aReqPtr->postUri += "\"password\":\"";
		aReqPtr->postUri += sWorldManager.GOTYE_Password;
		aReqPtr->postUri += "\",";
		aReqPtr->postUri += "\n}";
		RefCountedPtr<GamesvrSession> session(this, true);
		aReqPtr->callBackFn = [session,doCallBackFun](const HttpResponse& _httpResponse ) {
			BufferPtr pBuffer = _httpResponse.content();
			vn::RefCountedPtr<vn::Variable_object>	vJSONPtr = _getJsonObj(_httpResponse);
			if (vJSONPtr.get()) {
				sWorldManager. expires_in = vJSONPtr->queryAttributeInt64("expires_in",0);
				if(sWorldManager.expires_in <=0) {
					doCallBackFun(false);
					return;
				} else {
					sWorldManager.expires_in+=time(NULL);
				}
				sWorldManager.access_token = vJSONPtr->queryAttributeString("access_token","");
				doCallBackFun(true);
			}
		};
		sCurlManager.CurlSend(aReqPtr);
	}else {
		doCallBackFun(true);
	}

}
void GamesvrSession::getFriendList(bool isRightToken, RefCountedPtr<pkt::req_GetFriendList> pkt){
	if(isRightToken){

		//POST 方式

		//https://rest.gotye.com.cn/api/GetFriends
		HttpRequestPtr aReqPtr(new HttpRequestC);
		aReqPtr->addr.from_string("42.62.79.105:443");
		aReqPtr->host = "rest.gotye.com.cn";
		aReqPtr->content = "/api/GetFriends";
		aReqPtr->isPost = true;
		aReqPtr->isHttps = true;

		aReqPtr->headMap["Authorization"] = "Bearer " + sWorldManager.access_token;
		aReqPtr->postUri = "{\n";
		aReqPtr->postUri += "\"appkey\":\"";
		aReqPtr->postUri += sWorldManager.GOTYE_Appkey;
		aReqPtr->postUri += "\",";
		aReqPtr->postUri += "\"userAccount\":\"";
		aReqPtr->postUri += pkt->data.ppid;
		aReqPtr->postUri += "\",";
		aReqPtr->postUri += "\"index\":";
		aReqPtr->postUri += "0";
		aReqPtr->postUri += ",";
		aReqPtr->postUri += "\"count\":";
		aReqPtr->postUri += sWorldManager.GOTYE_Max_Friend_Num;
		aReqPtr->postUri += "\n}";

		u64 handle = pkt->data.handle;
		RefCountedPtr<GamesvrSession> session(this, true);
		aReqPtr->callBackFn = [session,handle](const HttpResponse& _httpResponse ) {
			pkt::res_JsonDate res;
			res.data.handle = handle;
			res.data.result = 0;
			res.data.jsonvalue = "{\n \"ret\":0, \"lists\":[";
			BufferPtr pBuffer = _httpResponse.content();
			//VN_LOG_DEBUG("Rev : "<<(const char *)pBuffer->lock());
				try {
					vn::RefCountedPtr<vn::Variable_object> vJSONPtr = _getJsonObj(_httpResponse);
					if (vJSONPtr.get()) {
						vn::RefCountedPtr<vn::Variable_object> entities = vJSONPtr->queryAttributeObject("entities");
						if (entities.get()) {
							for(size_t i =0; i< entities->size();i++) {
								VariablePtr pVar = entities->at(i);
								vn::RefCountedPtr<vn::Variable_object> entity = pVar.dyn_cast<Variable_object>();
								res.data.jsonvalue +="{\"openid\":\"";
								res.data.jsonvalue += entity->queryAttributeString("account","");
								res.data.jsonvalue +="\",\"nickName\":\"";
								res.data.jsonvalue += entity->queryAttributeString("nickname","");
								res.data.jsonvalue +="\",\"picture\":\"";
								res.data.jsonvalue += entity->queryAttributeString("headId","");
								if(i!= entities->size()-1) {
									res.data.jsonvalue +="\"},";
								} else {
									res.data.jsonvalue +="\"}";
								}
							}
						}

						//			char buffer[1024];
						//			sprintf(buffer,
						//					"{\n \"ret\":%d, \"lists\":[{\"openid\":\"FgAAAAABAAA=\",\"nickName\":\"TestFriend1\",\"gender\":1},{\"openid\":\"AgAAAAABAAA=\",\"nickName\":\"TestFriend2\",\"gender\":1},{\"openid\":\"AwAAAAABAAA=\",\"nickName\":\"TestFriend3\",\"gender\":1},{\"openid\":\"BAAAAAABAAA=\",\"nickName\":\"TestFriend4\",\"gender\":1},{\"openid\":\"BQAAAAABAAA=\",\"nickName\":\"TestFriend5\",\"gender\":1},{\"openid\":\"GAAAAAABAAA=\",\"nickName\":\"TestFriend6\",\"gender\":1},{\"openid\":\"GQAAAAABAAA=\",\"nickName\":\"TestFriend7\",\"gender\":1},{\"openid\":\"GgAAAAABAAA=\",\"nickName\":\"TestFriend8\",\"gender\":1},{\"openid\":\"HAAAAAABAAA=\",\"nickName\":\"TestFriend9\",\"gender\":1},{\"openid\":\"HQAAAAABAAA=\",\"nickName\":\"TestFriend10\",\"gender\":1}]  \n}",
						//					ret);
						//			res.data.jsonvalue = buffer;

					}
				} catch(std::exception& e) {
					VN_LOG_DEBUG ("[Excepction]:"<<e.what());
				}
				res.data.jsonvalue +="]  \n}";
				VN_LOG_DEBUG("send:"<<res.data.jsonvalue);
				session->sendPacket(res);

			};

		sCurlManager.CurlSend(aReqPtr);
	} else {
		int ret = 0;
		pkt::res_JsonDate res;
		res.data.handle = pkt->data.handle;
		res.data.result = 0;
		char buffer[512];
		sprintf(buffer, "{\n \"ret\":%d  \n}", ret);
		res.data.jsonvalue = buffer;
		this->sendPacket(res);
	}

}
VN_IMPL_PACKET_REQUEST2(GamesvrSession, pkt, req_GetFriendList) {
/*	int ret = 0;
	pkt::res_JsonDate res;
	res.data.handle = pkt->data.handle;
	res.data.result = 0;
	char buffer[512];
	sprintf(buffer, "{\n \"ret\":%d  , \"lists\":[{\"openid\":\"未知\",\"nickName\":\"未知\",\"picture\":\"\"},{\"openid\":\"nAQAAAABAAA=\",\"nickName\":\"181818\",\"picture\":\"\"},{\"openid\":\"ngQAAAABAAA=\",\"nickName\":\"wjb0108\",\"picture\":\"\"}]\n}", ret);
	res.data.jsonvalue = buffer;
	this->sendPacket(res);*/

	getGotyeAccessToken(std::bind(&GamesvrSession::getFriendList,this,std::placeholders::_1,RefCountedPtr<pkt::req_GetFriendList>(pkt,true)));
}
VN_IMPL_PACKET_REQUEST2(GamesvrSession, pkt, req_GetFriendDetail) {
	int ret = 0;
	//todo
	pkt::res_JsonDate res;
	res.data.handle = pkt->data.handle;
	res.data.result = 0;
	char buffer[512];
	sprintf(buffer, "{\n \"ret\":%d  \n}", ret);
	res.data.jsonvalue = buffer;
	this->sendPacket(res);
}
VN_IMPL_PACKET_REQUEST2(GamesvrSession, pkt, req_GetStrangerDetail) {
	int ret = 0;
	//todo
	pkt::res_JsonDate res;
	res.data.handle = pkt->data.handle;
	res.data.result = 0;
	char buffer[512];
	sprintf(buffer, "{\n \"ret\":%d  \n}", ret);
	res.data.jsonvalue = buffer;
	this->sendPacket(res);
}
VN_IMPL_PACKET_REQUEST2(GamesvrSession, pkt, req_UpdScore) {
	int ret = 0;
	//todo
	pkt::res_JsonDate res;
	res.data.handle = pkt->data.handle;
	res.data.result = 0;
	char buffer[512];
	sprintf(buffer, "{\n \"ret\":%d  \n}", ret);
	res.data.jsonvalue = buffer;
	this->sendPacket(res);
}
VN_IMPL_PACKET_REQUEST2(GamesvrSession, pkt, req_FeedNotify) {
	int ret = 0;
	//todo
	pkt::res_JsonDate res;
	res.data.handle = pkt->data.handle;
	res.data.result = 0;
	char buffer[512];
	sprintf(buffer, "{\n \"ret\":%d  \n}", ret);
	res.data.jsonvalue = buffer;
	this->sendPacket(res);
}
void GamesvrSession::SendJosnPkg(pkt::res_JsonDate& res,int error){
	char buffer[512];
	sprintf(buffer, "{\n \"ret\":%d  \n}", error);
	res.data.jsonvalue = buffer;
	this->sendPacket(res);
}

// ---- 修改昵称 ------------2016-02-01
VN_IMPL_PACKET_REQUEST2(GamesvrSession, pkt, req__PF_ResetNickname) {
	pkt::res_JsonDate res;
	res.data.handle = pkt->data.handle;
	res.data.result = 0;

	//查询该openId的玩家是否已经存在
	std::vector<u8> ppidVtr = Base64::decode(pkt->data.ppid);
	if(ppidVtr.size() != sizeof(u64)){
		VN_LOG_ERROR("pkt->data.ppid = "<<pkt->data.ppid << "Base64::decode error!");
		SendJosnPkg(res,-1);
		return;
	}
	u64 openId = 0;
	memcpy(&openId, ppidVtr.data(), sizeof(u64));

	//长度是否合法
	if (6 > pkt->data.nickname.length() || pkt->data.nickname.length() > 18) {
		SendJosnPkg(res,ERROR_CODE_NICKNAME_ILLEGAL_SIZE);
		return;
	}
	//是否包含敏感词
	if (!DirtyManager2::instance().check(pkt->data.nickname)) {
		SendJosnPkg(res, ERROR_CODE_NICKNAME_ILLEGAL_SYMBOL);
		return;
	}

	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_SEL_ACCOUNT_BY_NICKNAME);
	stmt->setString(0, pkt->data.nickname);
	PreparedQueryResult result = PassPortDatabase.SynQuery(stmt);
	if (result && result->GetRowCount()) {
		SendJosnPkg(res, ERROR_CODE_NICKNAME_USED);
		return;
	}

	//设置昵称
	stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_UPD_ACCOUNT_NICKNAME_BY_PPID);
	stmt->setString(0, pkt->data.nickname);
	stmt->setUInt64(1, openId);
	bool isExecute = PassPortDatabase.SynExecute(stmt);
	if (isExecute) { //update success
		SendJosnPkg(res, 0);
		//从随机库中删除
		NicknameManager::instance().deleteNickname(pkt->data.nickname);
	} else { //update fail
		SendJosnPkg(res, ERROR_CODE_DB_ERROR);
	}

	return;
}
