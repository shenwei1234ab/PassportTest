//
//  ClientSession.cpp
//  passport_svr
//
//  Created by Wayne on 6/9/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "../../passportsvr/clientlisten/ClientSession.h"
#include "../../passportsvr/manager/DirtyManager2.h"
#include "../../passportsvr/manager/NicknameManager.h"
#include "../../passportsvr/manager/PlayerManager.h"
#include "../../passportsvr/WorldManager.h"
#include "../database/PassPortDatabaseConnection.h"
#include "vnBase64.h"
#include "tool/crypt/Crypt.h"
#include "tool/TSTime.h"

extern PassPortDatabaseWorkerPool PassPortDatabase;
#define CHECK_STATE(name) \
	if(m_state != kLogin) { \
		pkt::name res;      \
		res.data.errorCode = ERROR_CODE_STATE_UNLOGIN; \
		this->sendPacket(res, kClose); \
		return;					\
	}


//---------------------------------------- 登陆请求，首次登陆进行注册 -------------------------------------------------------------
//VN_IMPL_PACKET_REQUEST2(ClientSession, pkt, req_LoginByTPInfo) {
//	// 检验第三方用户身份
//	u32 tpId = pkt->data.tpId >> 16;
//	if (tpId == PLATFORM_ID_TEST) {
//		tpId = PLATFORM_ID_VISITOR;
//	}
//	void (ClientSession::*pfn)(bool, RefCountedPtr<pkt::req_LoginByTPInfo>) = &ClientSession::doVerifyReqFun;
//	RefCountedPtr<ClientSession> session(this, true);
//	SendVerifyReq(tpId, pkt->data.openId, pkt->data.token, std::bind(pfn, session, std::placeholders::_1, RefCountedPtr<pkt::req_LoginByTPInfo>(pkt, true)));
//}
//void ClientSession::doVerifyReqFun(bool isVerify, RefCountedPtr<pkt::req_LoginByTPInfo> pkt) {
//	if (isVerify && !pkt->data.openId.empty()) {
//		m_gameId = pkt->data.gameId;
//		//查询该openId的玩家是否已经存在
//		PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_SEL_PPID);
//		u32 tpId = pkt->data.tpId >> 16;
//
//		u32 os = pkt->data.tpId & 0xFFFF;
//		stmt->setUInt32(0, tpId);
//		stmt->setString(1, pkt->data.openId);
//		stmt->setUInt32(2, os);
//		void (ClientSession::*pfn)(PreparedQueryResult, RefCountedPtr<pkt::req_LoginByTPInfo>) = &ClientSession::doQueryResultGetPPID;
//		PassPortDatabase.Query(stmt, { this, true }, std::bind(pfn, this, std::placeholders::_1, RefCountedPtr<pkt::req_LoginByTPInfo>(pkt.get(), true)));
//	} else {
//		//用户创建失败
//		VN_LOG_INFO("Verify openid Error ! tpId = "<< (pkt->data.tpId >> 16) << "openId="<< pkt->data.openId << "token="<<pkt->data.token);
//		pkt::res_LoginError res;
//		res.data.errorCode = 1;
//		this->sendPacket(res, kClose);
//	}
//}
//void ClientSession::doQueryResultGetPPID(PreparedQueryResult aResult, RefCountedPtr<pkt::req_LoginByTPInfo> pkt) {
//	if (aResult && aResult->GetRowCount()) {
//		VN_LOG_INFO("find ppID : "<<(*aResult)[0].GetUInt64());
//		m_ppId = (*aResult)[0].GetUInt64();
//		m_gameId = pkt->data.gameId;
//		m_tmpToken = sWorldManager.createToken();
//		SessionDataPtr pSessionData = sWorldManager.getSessionData(m_ppId,m_gameId);
//		if (pSessionData.get()) {
//			pSessionData->pptoken = m_tmpToken;
//			VN_LOG_DEBUG("SessionData: ppid = "<<pSessionData->ppid <<",pptoken=" <<pSessionData->pptoken );
//		} else {
//			SessionDataPtr pSessionData(new SessionData, true);
//			pSessionData->ppid = m_ppId;
//			pSessionData->gameid = m_gameId;
//			pSessionData->pptoken = m_tmpToken;
//			sWorldManager.insertSessionData(pSessionData);
//			VN_LOG_DEBUG("SessionData: ppid = "<<pSessionData->ppid <<",pptoken=" <<pSessionData->pptoken );
//		}
//		pkt::res_LoginOkWithInfo res;
//		res.data.ppid = m_ppId;
//		res.data.pptoken = m_tmpToken;
//		//res.data.nickname = (*aResult)[1].GetString();
//		this->sendPacket(res);
//	} else {
//		u32 tpId = pkt->data.tpId >> 16;
//
//		//向数据库插入数据
//		PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_INS_PPID);
//		m_ppId = sWorldManager.createPPId(tpId);
//		if (m_ppId == u64(-1)) {
//			//未识别的平台
//			pkt::res_LoginError res;
//			res.data.errorCode = 1;
//			this->sendPacket(res, kClose);
//			return;
//		}
//
//		u32 os = pkt->data.tpId & 0xFFFF;
//		m_tmpToken = sWorldManager.createToken();
//		stmt->setUInt64(0, m_ppId);
//		stmt->setUInt32(1, tpId);
//		stmt->setString(2, pkt->data.openId);
//		stmt->setString(3, "");
//		stmt->setUInt32(4, os);
//		stmt->setString(5, "");
//		stmt->setString(6, "");
//		void (ClientSession::*pfn)(bool, RefCountedPtr<pkt::req_LoginByTPInfo>) = &ClientSession::doExecuteInsertAccount;
//		PassPortDatabase.Execute(stmt, { this, true }, std::bind(pfn, this, std::placeholders::_1, RefCountedPtr<pkt::req_LoginByTPInfo>(pkt.get(), true)));
//	}
//}
//
//void ClientSession::doExecuteInsertAccount(bool isExecute, RefCountedPtr<pkt::req_LoginByTPInfo> pkt) {
//	if (isExecute) {
//		//用户创建成功
//		VN_LOG_INFO("Insert Account OK");
//		pkt::res_LoginOkWithInfo res;
//		res.data.ppid = m_ppId;
//		res.data.pptoken = m_tmpToken;
//		//res.data.nickname = "";
//		this->sendPacket(res);
//
//		SessionDataPtr pSessionData(new SessionData, true);
//		pSessionData->ppid = m_ppId;
//		pSessionData->gameid = pkt->data.gameId;
//		pSessionData->pptoken = m_tmpToken;
//		sWorldManager.insertSessionData(pSessionData);
//		VN_LOG_DEBUG("SessionData: ppid = "<<pSessionData->ppid <<",pptoken=" <<pSessionData->pptoken );
//	} else {
//		//用户创建失败
//		VN_LOG_INFO("Insert Account Error!!");
//		pkt::res_LoginError res;
//		res.data.errorCode = 1;
//		this->sendPacket(res, kClose);
//	}
//}

//----------------------------------第三方平台 登录请求2-------------------------------------------------------
VN_IMPL_PACKET_REQUEST2(ClientSession, pkt, req_LoginByTPInfo2) {
	m_tpId = (PLATFORM_ID)(pkt->data.tpId >> 16);
	m_osId = (OS_ID)(pkt->data.tpId & 0xFFFF);
	m_openid3 = pkt->data.openId;
	m_gameId = pkt->data.gameId;
	void (ClientSession::*pfn)(bool, RefCountedPtr<pkt::req_LoginByTPInfo2>) = &ClientSession::doVerifyReqFun;
	RefCountedPtr<ClientSession> session(this, true);
	//向第三方发送验证请求，官服请求会直接做失败处理
	SendVerifyReq(m_tpId, pkt->data.openId, pkt->data.token, std::bind(pfn, session, std::placeholders::_1, RefCountedPtr<pkt::req_LoginByTPInfo2>(pkt, true)));
}
//第三方平台 登录请求----处理第三方返回结果
void ClientSession::doVerifyReqFun(bool isVerify, RefCountedPtr<pkt::req_LoginByTPInfo2> pkt) {
	if (isVerify && !pkt->data.openId.empty()) {
//		m_gameId = pkt->data.gameId;
		//查询该openId的玩家是否已经存在
		PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_SEL_PPID);
//		u32 tpId = pkt->data.tpId >> 16;
//		u32 os = pkt->data.tpId & 0xFFFF;
		stmt->setUInt32(0, m_tpId);
		stmt->setString(1, pkt->data.openId);
		stmt->setUInt32(2, m_osId);
//		void (ClientSession::*pfn)(PreparedQueryResult, RefCountedPtr<pkt::req_LoginByTPInfo2>) = &ClientSession::doQueryResultGetPPID;
//		PassPortDatabase.Query(stmt, { this, true }, std::bind(pfn, this, std::placeholders::_1, RefCountedPtr<pkt::req_LoginByTPInfo2>(pkt.get(), true)));
		PassPortDatabase.Query(stmt, { this, true }, std::bind(&ClientSession::Process_GetPPID, this, std::placeholders::_1, true));
	} else {
		//用户验证失败
		VN_LOG_INFO("Verify openid Error ! tpId = "<< (pkt->data.tpId >> 16) << "openId="<< pkt->data.openId << "token="<<pkt->data.token);
		pkt::res_LoginError res;
		res.data.errorCode = 1;
		this->sendPacket(res, kClose);
	}
}
/*//第三方平台 登录请求----获取ppid，存在的话返回登录成功，不存在注册
void ClientSession::doQueryResultGetPPID(PreparedQueryResult aResult, RefCountedPtr<pkt::req_LoginByTPInfo2> pkt) {
	m_tmpToken = sWorldManager.createToken();
	if (aResult && aResult->GetRowCount()) {
		VN_LOG_INFO("find ppID : "<<(*aResult)[0].GetUInt64());
		m_ppId = (*aResult)[0].GetUInt64();
		m_gameId = pkt->data.gameId;

		SessionDataPtr pSessionData = sWorldManager.getSessionData(m_ppId,m_gameId);
		if (pSessionData.get()) {
			pSessionData->pptoken = m_tmpToken;
			VN_LOG_DEBUG("SessionData: ppid = "<<pSessionData->ppid <<",pptoken=" <<pSessionData->pptoken );
		} else {
			SessionDataPtr pSessionData = new SessionData;
			pSessionData->ppid = m_ppId;
			pSessionData->gameid = m_gameId;
			pSessionData->pptoken = m_tmpToken;
			sWorldManager.insertSessionData(pSessionData);
			VN_LOG_DEBUG("SessionData: ppid = "<<pSessionData->ppid <<",pptoken=" <<pSessionData->pptoken );
		}
		m_state = kLogin;
		pkt::res_LoginOkWithInfo2 res;
		res.data.ppid = m_ppId;
		res.data.pptoken = m_tmpToken;
		res.data.nickname = (*aResult)[1].GetString();
		res.data.avatar = (*aResult)[2].GetString();
		this->sendPacket(res);
	} else {
		u32 tpId = pkt->data.tpId >> 16;
		//向数据库插入数据
		PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_INS_PPID);
		m_ppId = sWorldManager.createPPId(tpId);
		if (m_ppId == u64(-1)) {
			//未识别的平台
			pkt::res_LoginError res;
			res.data.errorCode = 1;
			this->sendPacket(res, kClose);
			return;
		}
		u32 os = pkt->data.tpId & 0xFFFF;
		stmt->setUInt64(0, m_ppId);
		stmt->setUInt32(1, tpId);
		stmt->setString(2, pkt->data.openId);
		stmt->setString(3, "");
		stmt->setUInt32(4, os);
		stmt->setString(5, "");
		stmt->setString(6, "");
		stmt->setUInt8(7, 0);
		void (ClientSession::*pfn)(bool, RefCountedPtr<pkt::req_LoginByTPInfo2>) = &ClientSession::doExecuteInsertAccount;
		PassPortDatabase.Execute(stmt, { this, true }, std::bind(pfn, this, std::placeholders::_1, RefCountedPtr<pkt::req_LoginByTPInfo2>(pkt.get(), true)));
	}
}
//第三方平台 登录请求----注册ppid结果处理
void ClientSession::doExecuteInsertAccount(bool isExecute, RefCountedPtr<pkt::req_LoginByTPInfo2> pkt) {
	if (isExecute) {
		m_state = kLogin;
		VN_LOG_INFO("Insert Account OK");
		pkt::res_LoginOkWithInfo2 res;
		res.data.ppid = m_ppId;
		res.data.pptoken = m_tmpToken;
		res.data.nickname = "";
		res.data.avatar = "";
		this->sendPacket(res);

		SessionDataPtr pSessionData = new SessionData;
		pSessionData->ppid = m_ppId;
		pSessionData->gameid = pkt->data.gameId;
		pSessionData->pptoken = m_tmpToken;
		sWorldManager.insertSessionData(pSessionData);
		VN_LOG_DEBUG("SessionData: ppid = "<<pSessionData->ppid <<",pptoken=" <<pSessionData->pptoken );
	} else {
		//用户创建失败
		VN_LOG_INFO("Insert Account Error!!");
		pkt::res_LoginError res;
		res.data.errorCode = 1;
		this->sendPacket(res, kClose);
	}
}*/

//---------------------------------------- 客户端登陆,返回成功失败(99) -------------------------------------------------------------
VN_IMPL_PACKET_REQUEST2(ClientSession, pkt, req_Login) {
	m_gameId = pkt->data.gameId;
	m_ppId = pkt->data.ppid;
	m_tmpToken = pkt->data.pptoken;

	//以下信息可能失效
	//m_tpId = "";
	//m_osId = "";
	//m_openid3 = "";

	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_SEL_PPTOKEN);
	stmt->setUInt64(0, pkt->data.ppid);

	u32 accessToken = pkt->data.pptoken;
	PassPortDatabase.Query(stmt, { this, true }, [this,accessToken](PreparedQueryResult result) {
			if (result && result->GetRowCount()) {
				uint64 ppToken = (*result)[0].GetUInt64();
				uint64 deadline = (*result)[1].GetUInt64();
				if( ppToken == accessToken && deadline-time(NULL) >0 ) {
					//验证成功
				pkt::res_LoginOk res;
				this->sendPacket(res);
				m_state = kLogin;
				this->RegisterPlayer(m_ppId,m_tmpToken);
			} else {
				VN_LOG_ERROR("Client: ppid["<<m_ppId << "] token["<< ppToken<<"] error ! accessToken["<<accessToken<<"]");
				//验证失败,token 无效
				pkt::res_LoginError res;
				res.data.errorCode = -2;
				this->sendPacket(res, kClose);
				return;
			}

		} else {
			VN_LOG_ERROR("Client: can't find token for ppid["<<m_ppId << "] ");
			//验证失败
			pkt::res_LoginError res;
			res.data.errorCode = -1;
			this->sendPacket(res, kClose);
			return;
		}

	});
/*
	SessionDataPtr pSessionData = sWorldManager.getSessionData(pkt->data.ppid,pkt->data.gameId);
	if(pSessionData.get()) {
		if (pSessionData->pptoken != pkt->data.pptoken) {
			VN_LOG_ERROR("Client: ppid["<<pkt->data.ppid << "] token["<< pSessionData->pptoken<<"] error ! accessToken["<<pkt->data.pptoken<<"]");
			//验证失败,token 无效
			pkt::res_LoginError res;
			res.data.errorCode = -2;
			this->sendPacket(res, kClose);
			return;
		}

		//验证成功
		pkt::res_LoginOk res;
		this->sendPacket(res);
		m_state = kLogin;
		RegisterPlayer(m_ppId,m_tmpToken);

		return;
	} else {
		VN_LOG_ERROR("Client: can't find token for ppid["<<pkt->data.ppid << "] ");
		//验证失败
		pkt::res_LoginError res;
		res.data.errorCode = -1;
		this->sendPacket(res, kClose);
		return;
	}*/
}

//// 异步更新 m_tmpToken
//void ClientSession::updateToken() {
//	SessionDataPtr pSessionData = sWorldManager.getSessionData(m_ppId,m_gameId);
//	if(pSessionData.get()) {
//		pSessionData ->pptoken = m_tmpToken;
//	}
//}

//------------------------------------官方服务器注册-----------------------------------------
VN_IMPL_PACKET_REQUEST2(ClientSession, pkt, req_UserRegister) {
	//检测账号是否合法4-12位数字字母组合;
	if (pkt->data.account.length() < 4 || pkt->data.account.length() > 12) {
		pkt::res_RegisterFail res;
		res.data.errorCode = ERROR_CODE_REGISTER_FAIL_ACCOUNT_ILLEGAL_SIZE;
		this->sendPacket(res);
		return;
	}
	//检测帐号格式（英文数字）
	for (unsigned int i = 0; i < pkt->data.account.length(); ++i) {
		if (('0' <= pkt->data.account[i] && pkt->data.account[i] <= '9') || ('A' <= pkt->data.account[i] && pkt->data.account[i] <= 'z')) {
			continue;
		} else {
			pkt::res_RegisterFail res;
			res.data.errorCode = ERROR_CODE_REGISTER_FAIL_ACCOUNT_ILLEGAL_UNKNOWN_SYMBOL;
			this->sendPacket(res);
			return;
		}
	}
	//检测密码是否6-12位
	if (pkt->data.password.length() < 6 || pkt->data.password.length() > 12) {
		pkt::res_RegisterFail res;
		res.data.errorCode = ERROR_CODE_REGISTER_FAIL_PASSWORD_SIZE;
		this->sendPacket(res);
		return;
	}
	//检测手机号
	/*if(pkt->data.phonenumber.length() != 11) {
	 pkt::res_RegisterFail res;
	 res.data.errorCode = ERROR_CODE_REGISTER_FAIL_PHONENUMBER_ILLEGAL;
	 this->sendPacket(res);
	 return;
	 }
	 for(unsigned int i = 0; i < pkt->data.phonenumber.length(); ++i) {
	 if('0' <= pkt->data.phonenumber[i] && pkt->data.phonenumber[i] <= '9') {
	 continue;
	 } else {
	 pkt::res_RegisterFail res;
	 res.data.errorCode = ERROR_CODE_REGISTER_FAIL_PHONENUMBER_ILLEGAL;
	 this->sendPacket(res);
	 return;
	 }
	 }*/

	void (ClientSession::*pfn)(PreparedQueryResult, RefCountedPtr<pkt::req_UserRegister>) = &ClientSession::doQueryResultGetName;
	//检测帐号是否可用(数据库查询)
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_SEL_BY_NAME);
	stmt->setString(0, pkt->data.account);
	PassPortDatabase.Query(stmt, { this, true }, std::bind(pfn, this, std::placeholders::_1, RefCountedPtr<pkt::req_UserRegister>(pkt, true)));
}

//官方服务器注册-------同名校验，密码校验
void ClientSession::doQueryResultGetName(PreparedQueryResult aResult, RefCountedPtr<pkt::req_UserRegister> pkt) {
	//结果未0
	if (aResult && aResult->GetRowCount()) {
		pkt::res_RegisterFail res;
		res.data.errorCode = ERROR_CODE_REGISTER_FAIL_ACCOUNT_USED;
		this->sendPacket(res);
		return;
	}
	//密码加密
	std::string password_encrypt;
	if (!Crypt::Encrypt(pkt->data.password, password_encrypt)) {
		pkt::res_RegisterFail res;
		res.data.errorCode = ERROR_CODE_REGISTER_FAIL_PALTFORM_ID_ERROR;
		this->sendPacket(res);
		return;
	}
	std::string s = Base64::encode(password_encrypt.c_str(), password_encrypt.length());

	//向数据库插入数据
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_INS_ACCOUNT_LONGSGOO);
	stmt->setString(0, pkt->data.account);
	stmt->setString(1, s);
	stmt->setString(2, pkt->data.phonenumber);
	PassPortDatabase.Execute(stmt, { this, true }, std::bind(&ClientSession::doExecuteInsertAccountLongsgoo, this, std::placeholders::_1, RefCountedPtr<pkt::req_UserRegister>(pkt.get(), true)));
}
//官方服务器注册-------插入结果处理
void ClientSession::doExecuteInsertAccountLongsgoo(bool isExecute, RefCountedPtr<pkt::req_UserRegister> pkt) {
	if (isExecute) { //插入成功
		pkt::res_RegisterOk res; //注册成功
		this->sendPacket(res);
		return;
	} else {
		pkt::res_RegisterFail res;
		res.data.errorCode = ERROR_CODE_REGISTER_FAIL_INSERT_ACOUNT_LONGSGOO_FAIL;
		this->sendPacket(res, kClose);
		return;
	}
}

//-------------------------------------帐号是否可用------------------------------------------
VN_IMPL_PACKET_REQUEST2(ClientSession, pkt, req_CheckAccountCanUse) {
	if (pkt->data.account.length() < 4 || pkt->data.account.length() > 12) {
		pkt::res_CheckAccountCanUse res;
		res.data.errorCode = ERROR_CODE_ACCOUNT_ILLEGAL_SIZE;
		this->sendPacket(res, kClose);
		return;
	}
	//检测帐号格式（英文数字）
	for (unsigned int i = 0; i < pkt->data.account.length(); ++i) {
		if (('0' <= pkt->data.account[i] && pkt->data.account[i] <= '9') || ('A' <= pkt->data.account[i] && pkt->data.account[i] <= 'z')) {
			continue;
		} else {
			pkt::res_CheckAccountCanUse res;
			res.data.errorCode = ERROR_CODE_ACCOUNT_ILLEGAL_UNKNOWN_SYMBOL;
			this->sendPacket(res, kClose);
			return;
		}
	}
	void (ClientSession::*pfn)(PreparedQueryResult) = &ClientSession::doQueryNameCanuse;
	//检测帐号是否可用(数据库查询)
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_SEL_BY_NAME);
	stmt->setString(0, pkt->data.account);
	PassPortDatabase.Query(stmt, { this, true }, std::bind(pfn, this, std::placeholders::_1));
}

void ClientSession::doQueryNameCanuse(PreparedQueryResult aResult) {
	if (aResult && aResult->GetRowCount()) {
		pkt::res_CheckAccountCanUse res;
		res.data.errorCode = ERROR_CODE_ACCOUNT_ACCOUNT_USED;
		this->sendPacket(res, kClose);
		return;
	}
	pkt::res_CheckAccountCanUse res;
	res.data.errorCode = ACCOUNT_OK;
	this->sendPacket(res, kClose);
	return;
}

//---------------------------------------------------------------------------------------------------
//----------------------------------官方服务器验证登录-------------------------------------密码加密
VN_IMPL_PACKET_REQUEST2(ClientSession, pkt, req_UserLogin) {
	m_tpId = PLATFORM_ID_TEST;
	m_account = pkt->data.account;
	m_gameId = pkt->data.gameId;
	m_osId = (OS_ID)(pkt->data.tpId & 0xFFFF);
	//检测账号是否合法4-12位数字字母组合
	if (pkt->data.account.length() < 4 || pkt->data.account.length() > 12) {
		pkt::res_LoginError res;
		res.data.errorCode = ERROR_CODE_LOGIN_FAIL_ACCOUNT_ILLEGAL_SIZE;
		this->sendPacket(res);
		return;
	}
	//检测帐号格式（英文数字）
	for (unsigned int i = 0; i < pkt->data.account.length(); ++i) {
		if (('0' <= pkt->data.account[i] && pkt->data.account[i] <= '9') || ('A' <= pkt->data.account[i] && pkt->data.account[i] <= 'z')) {
			continue;
		} else {
			pkt::res_LoginError res;
			res.data.errorCode = ERROR_CODE_LOGIN_FAIL_ACCOUNT_ILLEGAL_UNKNOWN_SYMBOL;
			this->sendPacket(res);
			return;
		}
	}

	//检测密码是否6-12位
	if (pkt->data.password.length() < 8 || pkt->data.password.length() > 16) {
		pkt::res_LoginError res;
		res.data.errorCode = ERROR_CODE_LOGIN_FAIL_PASSWORD_SIZE;
		this->sendPacket(res);
		return;
	}
	//检测帐号是否可用(数据库查询)
	void (ClientSession::*pfn)(PreparedQueryResult, RefCountedPtr<pkt::req_UserLogin>) = &ClientSession::doQueryResultGetName;
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_SEL_BY_NAME);
	stmt->setString(0, pkt->data.account);
	PassPortDatabase.Query(stmt, { this, true }, std::bind(pfn, this, std::placeholders::_1, RefCountedPtr<pkt::req_UserLogin>(pkt, true)));
}



//官方服务器验证登录-------------获取帐号信息，验证密码
void ClientSession::doQueryResultGetName(PreparedQueryResult aResult, RefCountedPtr<pkt::req_UserLogin> pkt) {
	if (aResult && aResult->GetRowCount() > 1) {
		VN_LOG_ERROR("SAME ACCOUNT IN ACCOUNT_LONGSGOO");
	} else if (aResult && aResult->GetRowCount() == 1) {
		if ((*aResult)[2].GetString() == pkt->data.password) {
			//*********************************************************************
			m_gameId = pkt->data.gameId;
			//查询该openId的玩家是否已经存在
			PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_SEL_PPID);
			u32 tpId = 1; //
			u32 os = pkt->data.tpId & 0xFFFF;
			stmt->setUInt32(0, tpId);
			stmt->setString(1, pkt->data.account);
			stmt->setUInt32(2, os);
			//PassPortDatabase.Query(stmt, { this, true }, std::bind(&ClientSession::doQueryPPID, this, std::placeholders::_1, RefCountedPtr<pkt::req_UserLogin>(pkt.get(), true)));
			PassPortDatabase.Query(stmt, { this, true }, std::bind(&ClientSession::Process_GetPPID, this, std::placeholders::_1, false));

		} else {
			pkt::res_LoginError res;
			res.data.errorCode = ERROR_CODE_LOGIN_FAIL_ERROR_PASSWORD;
			this->sendPacket(res);
		}
	} else {
		pkt::res_LoginError res;
		res.data.errorCode = ERROR_CODE_LOGIN_FAIL_ACCOUNT_UNFINED;
		this->sendPacket(res);
		return;
	}
}
void ClientSession::Process_GetPPID(PreparedQueryResult aResult, bool isThird) {
	m_tmpToken = sWorldManager.createToken();
	if (aResult && aResult->GetRowCount()) {
		//账户存在
		VN_LOG_INFO("find ppID : "<<(*aResult)[0].GetUInt64());
		m_ppId = (*aResult)[0].GetUInt64();
		bool isbingother =  (*aResult)[3].GetUInt8();
		if(!isThird && isbingother){
			PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_SEL_BING);
			stmt->setUInt64(0, m_ppId);
			PreparedQueryResult queryRet = PassPortDatabase.SynQuery(stmt);
			if(queryRet && queryRet->GetRowCount()) {
				m_ppId = (*queryRet)[0].GetUInt64();
				aResult = queryRet;
			}
		}
//		SessionDataPtr pSessionData = sWorldManager.getSessionData(m_ppId,m_gameId);
//		if (pSessionData.get()) {
//			pSessionData->pptoken = m_tmpToken;
//			VN_LOG_DEBUG("SessionData: ppid = "<<pSessionData->ppid <<",pptoken=" <<pSessionData->pptoken );
//		} else {
//			SessionDataPtr pSessionData(new SessionData, true);
//			pSessionData->ppid = m_ppId;
//			pSessionData->gameid = m_gameId;
//			pSessionData->pptoken = m_tmpToken;
//			sWorldManager.insertSessionData(pSessionData);
//			VN_LOG_DEBUG("SessionData: ppid = "<<pSessionData->ppid <<",pptoken=" <<pSessionData->pptoken );
//		}
		//更新token到数据库
		sWorldManager.updateDBToken(m_ppId,m_tmpToken);
		pkt::res_LoginOkWithInfo2 res;
		res.data.ppid = m_ppId;
		res.data.pptoken = m_tmpToken;
		res.data.nickname = (*aResult)[1].GetString();
		res.data.avatar = (*aResult)[2].GetString();
		this->sendPacket(res);
		m_state = kLogin;
	} else {
		//账户不存在，进行注册
		//向数据库插入数据
		PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_INS_PPID);
		m_ppId = sWorldManager.createPPId(m_tpId);
		if (m_ppId == u64(-1)) {
			//未识别的平台
			pkt::res_LoginError res;
			res.data.errorCode = 1;
			this->sendPacket(res, kClose);
			return;
		}

		stmt->setUInt64(0, m_ppId);
		stmt->setUInt32(1, m_tpId);
		if(isThird) {
			stmt->setString(2, m_openid3);
		} else {
			stmt->setString(2, m_account);
		}
		stmt->setString(3, "");
		stmt->setUInt32(4, m_osId);
		stmt->setString(5, "");
		stmt->setString(6, "");
		stmt->setUInt8(7, 0);

		PassPortDatabase.Execute(stmt, { this, true }, std::bind(&ClientSession::Process_InsertAccount, this, std::placeholders::_1));
	}
}
void ClientSession::Process_InsertAccount(bool isExecute) {
	if (isExecute) {
		pkt::res_LoginOkWithInfo2 res;
		res.data.ppid = m_ppId;
		res.data.pptoken = m_tmpToken;
		res.data.nickname = "";
		res.data.avatar = "";
		this->sendPacket(res);

//		SessionDataPtr pSessionData(new SessionData, true);
//		pSessionData->ppid = m_ppId;
//		pSessionData->gameid = m_gameId;
//		pSessionData->pptoken = m_tmpToken;
//		sWorldManager.insertSessionData(pSessionData);
		//更新token到数据库
		sWorldManager.updateDBToken(m_ppId,m_tmpToken);
		m_state = kLogin;
	} else {
		VN_LOG_INFO("Insert Account Error!!");
		pkt::res_LoginError res;
		res.data.errorCode = 1;
		this->sendPacket(res, kClose);
		return;
	}
}

/*//官方服务器验证登录-------------获取ppid，没有的话注册
void ClientSession::doQueryPPID(PreparedQueryResult aResult, RefCountedPtr<pkt::req_UserLogin> pkt) {
	m_tmpToken = sWorldManager.createToken();
	if (aResult && aResult->GetRowCount()) {
		//账户存在
		VN_LOG_INFO("find ppID : "<<(*aResult)[0].GetUInt64());
		m_ppId = (*aResult)[0].GetUInt64();
		m_gameId = pkt->data.gameId;

		bool isbingother =  (*aResult)[3].GetUInt8();
		if(isbingother){
			PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_SEL_BING);
			stmt->setUInt64(0, m_ppId);
			PreparedQueryResult queryRet = PassPortDatabase.SynQuery(stmt);
			if(queryRet && queryRet->GetRowCount()) {
				m_ppId = (*queryRet)[0].GetUInt64();
				aResult = queryRet;
			}
		}
		SessionDataPtr pSessionData = sWorldManager.getSessionData(m_ppId,m_gameId);
		if (pSessionData.get()) {
			pSessionData->pptoken = m_tmpToken;
			VN_LOG_DEBUG("SessionData: ppid = "<<pSessionData->ppid <<",pptoken=" <<pSessionData->pptoken );
		} else {
			SessionDataPtr pSessionData(new SessionData, true);
			pSessionData->ppid = m_ppId;
			pSessionData->gameid = m_gameId;
			pSessionData->pptoken = m_tmpToken;
			sWorldManager.insertSessionData(pSessionData);
			VN_LOG_DEBUG("SessionData: ppid = "<<pSessionData->ppid <<",pptoken=" <<pSessionData->pptoken );
		}
		pkt::res_LoginOkWithInfo2 res;
		res.data.ppid = m_ppId;
		res.data.pptoken = m_tmpToken;
		res.data.nickname = (*aResult)[1].GetString();
		res.data.avatar = (*aResult)[2].GetString();
		this->sendPacket(res);
		m_state = kLogin;
	} else {
		//账户不存在，进行注册
		u32 tpId = 1;
		//向数据库插入数据
		PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_INS_PPID);
		m_ppId = sWorldManager.createPPId(tpId);
		if (m_ppId == u64(-1)) {
			//未识别的平台
			pkt::res_LoginError res;
			res.data.errorCode = 1;
			this->sendPacket(res, kClose);
			return;
		}
		u32 os = pkt->data.tpId & 0xFFFF;
		stmt->setUInt64(0, m_ppId);
		stmt->setUInt32(1, tpId);
		stmt->setString(2, pkt->data.account);
		stmt->setString(3, "");
		stmt->setUInt32(4, os);
		stmt->setString(5, "");
		stmt->setString(6, "");
		stmt->setUInt8(7, 0);
		void (ClientSession::*pfn)(bool, RefCountedPtr<pkt::req_UserLogin>) = &ClientSession::doExecuteInsertAccount;
		PassPortDatabase.Execute(stmt, { this, true }, std::bind(pfn, this, std::placeholders::_1, RefCountedPtr<pkt::req_UserLogin>(pkt.get(), true)));
	}
}
//官方服务器验证登录-------------注册ppid结果处理
void ClientSession::doExecuteInsertAccount(bool isExecute, RefCountedPtr<pkt::req_UserLogin> pkt) {
	if (isExecute) {
		pkt::res_LoginOkWithInfo2 res;
		res.data.ppid = m_ppId;
		res.data.pptoken = m_tmpToken;
		res.data.nickname = "";
		res.data.avatar = "";
		this->sendPacket(res);

		SessionDataPtr pSessionData(new SessionData, true);
		pSessionData->ppid = m_ppId;
		pSessionData->gameid = pkt->data.gameId;
		pSessionData->pptoken = m_tmpToken;
		sWorldManager.insertSessionData(pSessionData);
		m_state = kLogin;
	} else {
		VN_LOG_INFO("Insert Account Error!!");
		pkt::res_LoginError res;
		res.data.errorCode = 1;
		this->sendPacket(res, kClose);
		return;
	}
}*/

//昵称注册
VN_IMPL_PACKET_REQUEST2(ClientSession, pkt, req_SetNickname) {
	CHECK_STATE(res_SetNicknameFail)

	//长度是否合法
	if (6 > pkt->data.nickname.length() || pkt->data.nickname.length() > 18) {
		pkt::res_SetNicknameFail res;
		res.data.errorCode = ERROR_CODE_NICKNAME_ILLEGAL_SIZE;
		this->sendPacket(res);
		return;
	}
	//是否包含敏感词
	if (!DirtyManager2::instance().check(pkt->data.nickname)) {
		pkt::res_SetNicknameFail res;
		res.data.errorCode = ERROR_CODE_NICKNAME_ILLEGAL_SYMBOL;
		this->sendPacket(res);
		return;
	}

	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_SEL_ACCOUNT_BY_NICKNAME);
	stmt->setString(0, pkt->data.nickname);
	PreparedQueryResult result = PassPortDatabase.SynQuery(stmt);
	if (result && result->GetRowCount()) {
		pkt::res_SetNicknameFail res;
		res.data.errorCode = ERROR_CODE_NICKNAME_USED;
		this->sendPacket(res);
		return;
	}

	//昵称写入数据库doExecuteUpdateNickname
	{
		void (ClientSession::*pfn)(bool, RefCountedPtr<pkt::req_SetNickname>) = &ClientSession::doExecuteUpdateNickname;
		PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_UPD_ACCOUNT_NICKNAME_BY_PPID);
		stmt->setString(0, pkt->data.nickname);
		stmt->setUInt64(1, m_ppId);
		PassPortDatabase.Execute(stmt, { this, true }, std::bind(pfn, this, std::placeholders::_1, RefCountedPtr<pkt::req_SetNickname>(pkt, true)));
	}
}

void ClientSession::doExecuteUpdateNickname(bool isExecute, RefCountedPtr<pkt::req_SetNickname> pkt) {
	if (isExecute) { //update success
		pkt::res_SetNicknameOk res;
		this->sendPacket(res);

		//从随机库中删除
		NicknameManager::instance().deleteNickname(pkt->data.nickname);
	} else { //update fail
		pkt::res_SetNicknameFail res;
		res.data.errorCode = ERROR_CODE_DB_ERROR;
		this->sendPacket(res);
	}
}

//----------------------------------------获取随机名字------------------------------------------------
VN_IMPL_PACKET_REQUEST2(ClientSession, pkt, req_RandomNickname) {
	if (m_state != kLogin) {
		return;
	}
	auto nickname = NicknameManager::instance().getNickname();
	VN_LOG_DEBUG("GET RANDOM NICKNAME :" << nickname);
	pkt::res_RandomNickname res;
	res.data.nickname = nickname;
	this->sendPacket(res);
}

//--------------------------------------设置avatar--------------------------------------
VN_IMPL_PACKET_REQUEST2(ClientSession, pkt, req_SetAvatar) {
	if (m_state != kLogin) {
		return;
	}
	void (ClientSession::*pfn)(bool, RefCountedPtr<pkt::req_SetAvatar>) = &ClientSession::doExecuteUpdateAvatar;
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_UPD_ACCOUNT_AVATAR_BY_PPID);
	stmt->setString(0, pkt->data.avatar);
	stmt->setUInt64(1, m_ppId);
	PassPortDatabase.Execute(stmt, { this, true }, std::bind(pfn, this, std::placeholders::_1, RefCountedPtr<pkt::req_SetAvatar>(pkt, true)));
}

void ClientSession::doExecuteUpdateAvatar(bool isExecute, RefCountedPtr<pkt::req_SetAvatar> pkt) {
	if (isExecute) { //update success
		pkt::res_SetAvatarOk res;
		this->sendPacket(res);
	} else { //update fail
		VN_LOG_DEBUG("SET AVATAR ERROR");
	}
}

//注册player
void ClientSession::RegisterPlayer(vn::u64 ppid, int token) {
	//登录成功，根据ppid，pptoken查找player
	PlayerHandlerPtr pPlayerHandler = sPlayerManager.LookforPlayerHandler(ppid,token);
	if(pPlayerHandler) {
		pPlayerHandler ->SetClientSession( {this,true});
		m_playerId = pPlayerHandler->GetPlayerId();
		//sPlayerManager.ResetSocketId(oldSocketId,newSocketId);
		}
	}

