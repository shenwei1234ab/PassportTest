/*
 * ClientSession.cpp
 *
 *  Created on: 2015年8月14日
 *      Author: leave
 */

#include "../../passportsvr/clientlisten/ClientSession.h"

#include "../database/PassPortDatabaseConnection.h"
#include "vnBase64.h"
#include <time.h>
#include <vector>
#include "tool/crypt/Crypt.h"
#include "../../passportsvr/manager/PlayerManager.h"
#include "../../passportsvr/manager/SessionManager.h"
#include "../../passportsvr/WorldManager.h"
#include "vnMD5.h"
#include "curl/CurlWrapper.h"
#include "curl/CurlManager.h"
#include "common/cryptography/RsaVerify.h"
#include "common/cryptography/RsaSign.h"
#include "common/cryptography/HmacHash.h"

extern PassPortDatabaseWorkerPool PassPortDatabase;

std::string _getContent(const vn::HttpResponse& _httpResponse) {
	return (char*) _httpResponse.content()->lock();
}
vn::RefCountedPtr<vn::Variable_object> _getJsonObj(
		const vn::HttpResponse& _httpResponse) {
	vn::VariableJSON m_variableJSON;
	BufferPtr pBuffer = _httpResponse.content();
	if (!pBuffer) {
		return {};
	} VN_LOG_DEBUG( (char*)pBuffer->lock());
	return m_variableJSON.parse((char*) pBuffer->lock(), pBuffer->size());
}

void ClientSession::_init() {
	Session::_init();
	//m_tmpToken = sWorldManager.createToken();

}
void ClientSession::_destroy() {
	sPlayerManager.Disconnect(sockfd());
	if(m_playerId) {
		sPlayerManager.PlayerDisconnect(m_playerId);
	}

	Session::_destroy();
}

PacketPtr ClientSession::createRequest(u32 pid) {
	return PacketFactory<ClientSession>::instance().createRequest(pid);
}


// 检验第三方用户身份
void ClientSession::SendVerifyReq(int pfid, std::string& openid,
		std::string& accessToken, std::function<void(bool)> doVerifyReqFun) {
	if (doVerifyReqFun == nullptr) {
		VN_LOG_ERROR("can't find call back fun for doVerifyReq");
		return;
	}
	switch (pfid) {
	case PLATFORM_ID_TEST: { //官服
		//官服不应该在这里校验
		//验证失败
		doVerifyReqFun(false);
		break;
	}
	case PLATFORM_ID_HUAWEI: { //华为
		//doVerifyReqFun(false);
		SendVerifyReq_Huawei(openid, accessToken, doVerifyReqFun);

		break;
	}
	case PLATFORM_ID_XIAOMI: { //小米
		SendVerifyReq_Xiaomi(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_LENOVO: { //联想
		SendVerifyReq_Lenovo(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_KUAIYONG: { //快用苹果助手
		SendVerifyReq_7659(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_360: {
		SendVerifyReq_360(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_ANZHI: {
		SendVerifyReq_Anzhi(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_UC: {
		SendVerifyReq_UC(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_XMW: {
		SendVerifyReq_Xmwan(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_BAIDU: {
		SendVerifyReq_Baidu(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_OPPO: {
		SendVerifyReq_Oppo(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_PPTV: {
		SendVerifyReq_Pptv(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_GIONEE: {
		SendVerifyReq_Gionee(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_FLYME: {
		SendVerifyReq_Flyme(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_APPCHINA: {
		SendVerifyreq_Appchina(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_XY: {
		SendVerifyreq_XY(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_19196: {
		SendVerifyreq_19196(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_WANDOUJIA: {
		SendVerifyreq_Wandoujia(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_MUZHIWAN: {
		SendVerifyreq_Muzhiwan(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_DOWNJOY: {
		SendVerifyreq_Downjoy(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_COOLYUN: {
		SendVerifyreq_Coolyun(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_CCPLAY: {
		SendVerifyreq_Ccplay(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_4399: {
		SendVerifyreq_4399(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_I4: {
		SendVerifyreq_I4(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_QQ: {
		SendVerifyreq_QQ(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_WX: {
		SendVerifyreq_WX(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_TBT: {
		SendVerifyreq_TBT(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_VIVO: {
		SendVerifyreq_Vivo(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_HAIMA_AND: {
		SendVerifyreq_Haima_And(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_HAIMA_IOS: {
		SendVerifyreq_Haima_Ios(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_QIYPPS: {
		SendVerifyreq_PPS(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_SOGOU: {
		SendVerifyreq_Sogou(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_SINA: {
		SendVerifyreq_Sina(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_IAPPLE: {
		SendVerifyreq_IAPPLE(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_GFAN: {
		SendVerifyreq_GFAN(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_YOUKU: {
		SendVerifyreq_Youku(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_GUOPAN_AND: {
		//doVerifyReqFun(false);
		SendVerifyreq_GUOPAN_AND(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_GUOPAN_IOS: {
		//doVerifyReqFun(false);
		SendVerifyreq_GUOPAN_IOS(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_PP_IOS: {
		doVerifyReqFun(false);
		VN_LOG_ERROR("can't find pfid:"<<pfid)
		//SendVerifyreq_PP_IOS(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_VISITOR: {
		doVerifyReqFun(true);
		break;
	}
	case PLATFORM_ID_XINMEI365: {
		SendVerifyreq_XINMEI365(openid, accessToken, doVerifyReqFun);
		break;
	}
	case PLATFORM_ID_QUICK:{
		doVerifyReqFun(true);//for test
		break;
	}
	default:
		doVerifyReqFun(false);
		VN_LOG_ERROR("can't find pfid:"<<pfid)
		break;
	}
}
// ---- 申请帐号绑定 ------------
VN_IMPL_PACKET_REQUEST2(ClientSession, pkt, req_BindingAccount) {
	//检测是否已经成功登录
	if (m_state != kLogin) {
		pkt::res_BindingAccount res;
		res.data.errorCode = ERROR_CODE_STATE_UNLOGIN;
		this->sendPacket(res);
		return;
	}
	//检测目标帐号是否合法
	//检测账号是否合法4-12位数字字母组合
	if (pkt->data.account.length() < 4 || pkt->data.account.length() > 12) {
		pkt::res_BindingAccount res;
		res.data.errorCode = ERROR_CODE_LOGIN_FAIL_ACCOUNT_ILLEGAL_SIZE;
		this->sendPacket(res);
		return;
	}
	//检测帐号格式（英文数字）
	for (unsigned int i = 0; i < pkt->data.account.length(); ++i) {
		if (('0' <= pkt->data.account[i] && pkt->data.account[i] <= '9') || ('A' <= pkt->data.account[i] && pkt->data.account[i] <= 'z')) {
			continue;
		} else {
			pkt::res_BindingAccount res;
			res.data.errorCode = ERROR_CODE_LOGIN_FAIL_ACCOUNT_ILLEGAL_UNKNOWN_SYMBOL;
			this->sendPacket(res);
			return;
		}
	}
	//检测密码是否6-12位
	if (pkt->data.password.length() < 6 || pkt->data.password.length() > 12) {
		pkt::res_BindingAccount res;
		res.data.errorCode = ERROR_CODE_LOGIN_FAIL_PASSWORD_SIZE;
		this->sendPacket(res);
		return;
	}
	uint64 newPpid =0;

	//已有帐号，不能重复注册
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_SEL_BY_NAME);
	stmt->setString(0, pkt->data.account);
	PreparedQueryResult pResult = PassPortDatabase.SynQuery(stmt);
	if (pResult && pResult->GetRowCount()) {
		pkt::res_BindingAccount res;
		res.data.errorCode = ERROR_CODE_REGISTER_FAIL_ACCOUNT_USED;
		this->sendPacket(res);
		return;
	}
	//密码加密
	std::string password_encrypt;
	if (!Crypt::Encrypt(pkt->data.password, password_encrypt)) {
		pkt::res_BindingAccount res;
		res.data.errorCode = ERROR_CODE_REGISTER_FAIL_PALTFORM_ID_ERROR;
		this->sendPacket(res);
		return;
	}
	std::string s = Base64::encode(password_encrypt.c_str(), password_encrypt.length());

	//向官服表插入数据
	stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_INS_ACCOUNT_LONGSGOO);
	stmt->setString(0, pkt->data.account);
	stmt->setString(1, s);
	stmt->setString(2, "");
	bool dbRet = PassPortDatabase.SynExecute(stmt);
	if (!dbRet) {
		//error
		pkt::res_BindingAccount res;
		res.data.errorCode = ERROR_CODE_DB_ERROR;
		this->sendPacket(res);
		return;
	}
	//向account表插入数据
	u32 tpId = 1;
	stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_INS_PPID);
	newPpid = sWorldManager.createPPId(tpId);
	if (newPpid == u64(-1)) {
		//未识别的平台
		pkt::res_BindingAccount res;
		res.data.errorCode = 1;
		this->sendPacket(res, kClose);
		return;
	}

	u32 os = pkt->data.tpId & 0xFFFF;
	//m_tmpToken = sWorldManager.createToken();
	stmt->setUInt64(0, newPpid);
	stmt->setUInt32(1, tpId);
	stmt->setString(2, pkt->data.account);
	stmt->setString(3, "");
	stmt->setUInt32(4, os);
	stmt->setString(5, "");
	stmt->setString(6, "");
	stmt->setUInt8(7, 1);

	dbRet = PassPortDatabase.SynExecute(stmt);
	if (!dbRet) {
		//error
		pkt::res_BindingAccount res;
		res.data.errorCode = ERROR_CODE_DB_ERROR;
		this->sendPacket(res);
		return;
	}
//	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_SEL_PPID_BY_PWD);
//	stmt->setString(0, pkt->data.account);
//	stmt->setString(1, pkt->data.password);
//	PreparedQueryResult pResult = PassPortDatabase.SynQuery(stmt);
//	if (!pResult || pResult->GetRowCount() <= 0) {
//		//error
//		pkt::res_BindingAccount res;
//		res.data.errorCode = ERROR_CODE_DB_ERROR;
//		this->sendPacket(res);
//		return;
//	}
//	uint64 newPpid = (*pResult)[0].GetUInt64();;

	//给本帐号做绑定跳转标识
	stmt = PassPortDatabase.GetPreparedStatement(
			PASSPORT_UPD_ACCOUNT_BING);
	stmt->setUInt64(0, m_ppId);
	bool bResult = PassPortDatabase.SynExecute(stmt);
	if(!bResult){
		//error
		pkt::res_BindingAccount res;
		res.data.errorCode = ERROR_CODE_DB_ERROR;
		this->sendPacket(res);
		return;
	}
	stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_INS_BING);
	stmt->setUInt64(0, newPpid);
	stmt->setUInt64(1, m_ppId);
	bResult = PassPortDatabase.SynExecute(stmt);
	if (!bResult) {
		//error
		pkt::res_BindingAccount res;
		res.data.errorCode = ERROR_CODE_DB_ERROR;
		this->sendPacket(res);
		return;
	}
	pkt::res_BindingAccount res;
	res.data.errorCode = 0;
	this->sendPacket(res);
	return;
}

// ---- 修改密码 ------------2016-02-01
VN_IMPL_PACKET_REQUEST2(ClientSession, pkt, req_ResetPWD) {
	//检测是否已经成功登录
	if (m_state != kLogin ) {
		pkt::res_ResetPWD res;
		res.data.errorCode = ERROR_CODE_STATE_UNLOGIN;
		this->sendPacket(res);
		return;
	}
	//检测密码是否6-12位
	if (pkt->data.password.length() < 6 || pkt->data.password.length() > 12) {
		pkt::res_ResetPWD res;
		res.data.errorCode = ERROR_CODE_LOGIN_FAIL_PASSWORD_SIZE;
		this->sendPacket(res);
		return;
	}
	std::string password_encrypt;
	if (!Crypt::Encrypt(pkt->data.password, password_encrypt)) {
		pkt::res_BindingAccount res;
		res.data.errorCode = ERROR_CODE_REGISTER_FAIL_PALTFORM_ID_ERROR;
		this->sendPacket(res);
		return;
	}
	std::string s = Base64::encode(password_encrypt.c_str(), password_encrypt.length());

	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_UPD_PWD);
	stmt->setString(0, s);  // pwd
	stmt->setString(1, pkt->data.account); // name
	stmt->setString(2, pkt->data.oldPassword); //
	bool bResult = PassPortDatabase.SynExecute(stmt);
	if (!bResult) {
		//error
		pkt::res_ResetPWD res;
		res.data.errorCode = ERROR_CODE_DB_ERROR;
		this->sendPacket(res);
		return;
	}
	pkt::res_ResetPWD res;
	res.data.errorCode = 0;
	this->sendPacket(res);
	return;
}
// ---- 查询渠道id ------------2016-07-14
VN_IMPL_PACKET_REQUEST2(ClientSession, pkt, req_GetSubChannelId) {
	if (pkt->data.mainChannelId == PLATFORM_ID_XINMEI365){
		if (ljChannelMap.empty()) {
			PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_UPD_PWD);
			PreparedQueryResult result = PassPortDatabase.SynQuery(stmt);
			if (result && result->GetRowCount())
			{
				do {
					std::string channelLabel = (*result)[0].GetString();
					vn::u32 channelid = (*result)[1].GetUInt32();
					ljChannelMap.insert(std::make_pair(channelLabel,channelid));
				} while (result->NextRow());
			} else {
				VN_LOG_ERROR("can't find SubChannelId for :"<<pkt->data.mainChannelId)
			}
		}
		std::map<std::string,uint32_t>::iterator itr = ljChannelMap.find(pkt->data.channelLabel);
		if (itr != ljChannelMap.end()) {
			pkt::res_GetSubChannelId res;
			res.data.ppChannelId = itr->second;
			this->sendPacket(res);
			return;
		}
	} else {
		VN_LOG_ERROR("can't find main ChannelId for :"<<pkt->data.mainChannelId)
	}


	//没有找到合适的子渠道号，返回主渠道号
	pkt::res_GetSubChannelId res;
	res.data.ppChannelId = pkt->data.mainChannelId;
	this->sendPacket(res);
	return;
}
