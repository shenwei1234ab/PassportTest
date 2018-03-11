//
//  ClientSession.h
//  passport_svr
//
//  Created by Wayne on 6/9/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __passport_svr__ClientSession__
#define __passport_svr__ClientSession__

#include "framework/Session.h"

#include "vnPacketFactory.h"
#include <memory>
#include "common/Util.h"
#include "db/QueryResult.h"
#include <functional>

#include "../../passportsvr/manager/OrderManger.h"
#include "../../passportsvr/Protocols.h"
#include "vnVariableJSON.h"
#include "vnHttpResponse.h"
//#include "tool/TSTime.h"

std::string _getContent(const vn::HttpResponse& _httpResponse);
vn::RefCountedPtr<vn::Variable_object> _getJsonObj(const vn::HttpResponse& _httpResponse);

class ProductTemplet;

class ClientSession : public Session {
    VN_DECL_PACKET_HANDLER()
public:
    virtual PacketPtr createRequest(u32 pid);
    enum State {
    	kUnlogin,
		kLogin,
     };

protected:
    void _init();
    void _destroy();

//    void doQueryResultGetPPID(PreparedQueryResult aResult, RefCountedPtr<pkt::req_LoginByTPInfo> pkt);
//    void doExecuteInsertAccount(bool isExecute, RefCountedPtr<pkt::req_LoginByTPInfo> pkt);

    void Process_GetPPID(PreparedQueryResult aResult, bool isThird);
    void Process_InsertAccount(bool isExecute);
    //---------------------------第三方平台相关--------------------------------------------
//    //第三方平台 登录请求----获取ppid，存在的话返回登录成功，不存在注册
//    void doQueryResultGetPPID(PreparedQueryResult aResult, RefCountedPtr<pkt::req_LoginByTPInfo2> pkt);
//    //第三方平台 登录请求----注册ppid结果处理
//    void doExecuteInsertAccount(bool isExecute, RefCountedPtr<pkt::req_LoginByTPInfo2> pkt);


    void doExecuteCreateOrder(bool isExecute,OrderPtr pOrder,std::shared_ptr<ProductTemplet> pProductTmpl);


    //---------------------------官服相关--------------------------------------------

    //官方服务器注册-------同名校验，密码校验
    void doQueryResultGetName(PreparedQueryResult aResult, RefCountedPtr<pkt::req_UserRegister> pkt);
    //官方服务器注册-------插入结果处理
    void doExecuteInsertAccountLongsgoo(bool isExecute, RefCountedPtr<pkt::req_UserRegister> pkt);

//    //官方服务器验证登录-------------获取ppid，没有的话注册
//    void doQueryPPID(PreparedQueryResult aResult, RefCountedPtr<pkt::req_UserLogin> pkt);
//    //官方服务器验证登录-------------注册ppid结果处理
//    void doExecuteInsertAccount(bool isExecute, RefCountedPtr<pkt::req_UserLogin> pkt);
    //官方服务器验证登录-------------获取帐号信息，验证密码
    void doQueryResultGetName(PreparedQueryResult aResult, RefCountedPtr<pkt::req_UserLogin> pkt);

    void doExecuteUpdateNickname(bool isExecute, RefCountedPtr<pkt::req_SetNickname> pkt);
    void doExecuteUpdateAvatar(bool isExecute, RefCountedPtr<pkt::req_SetAvatar> pkt);
    void doQueryNameCanuse(PreparedQueryResult aResult);
    //--------------------------------------------------------------------------------


	// 检验第三方用户身份
	//void SendVerifyReq(int pfid,int ppid,std::function<void(bool)> doVerifyReqFun);
	void SendVerifyReq(int pfid,std::string& openid,std::string& accessToken,std::function<void(bool)> doVerifyReqFun);
	//void doVerifyReqFun(bool isVerify, RefCountedPtr<pkt::req_LoginByTPInfo> pkt);

	//第三方平台 登录请求----处理第三方返回结果
	void doVerifyReqFun(bool isVerify, RefCountedPtr<pkt::req_LoginByTPInfo2> pkt);

	void SendVerifyReq_Xiaomi(std::string& openid,std::string& accessToken,std::function<void(bool)> doVerifyReqFun);
	void SendVerifyReq_Huawei(std::string& openid,std::string& accessToken,std::function<void(bool)> doVerifyReqFun);
	void SendVerifyReq_Lenovo(std::string& openid,std::string& accessToken,std::function<void(bool)> doVerifyReqFun);
	void SendVerifyReq_360(std::string& openid,std::string& accessToken,std::function<void(bool)> doVerifyReqFun);
	//快用苹果助手
	void SendVerifyReq_7659(std::string& openid,std::string& accessToken,std::function<void(bool)> doVerifyReqFun);
	void SendVerifyReq_Anzhi(std::string& openid,std::string& accessToken,std::function<void(bool)> doVerifyReqFun);
	void SendVerifyReq_UC(std::string& openid,std::string& accessToken,std::function<void(bool)> doVerifyReqFun);
	void SendVerifyReq_Xmwan(std::string& openid, std::string& accessToken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyReq_Baidu(std::string& openid, std::string& accessToken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyReq_Oppo(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyReq_Pptv(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyReq_Gionee(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyReq_Flyme(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyreq_Appchina(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyreq_XY(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyreq_19196(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyreq_4399(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyreq_Wandoujia(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyreq_Muzhiwan(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyreq_Downjoy(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyreq_I4(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyreq_Coolyun(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyreq_Ccplay(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyreq_QQ(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyreq_WX(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyreq_Vivo(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyreq_Haima_And(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyreq_Haima_Ios(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyreq_TBT(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyreq_PPS(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyreq_Sogou(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyreq_Sina(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyreq_GFAN(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyreq_Youku(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyreq_IAPPLE(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyreq_GUOPAN_AND(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyreq_GUOPAN_IOS(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyreq_PP_IOS(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	void SendVerifyreq_XINMEI365(std::string& openid, std::string& accesstoken, std::function<void(bool)> doVerifyReqFun);
	// 检验第三方支付结果
	void SendDealReq(int pfid,int ppid,std::function<void(bool)> doDealReqFun);

	void SendDealReq_Xiaomi(std::string& openid,std::string& accessToken,std::function<void(bool)> doDealReqFun);
	void SendDealReq_Huawei(std::string& openid,std::string& accessToken,std::function<void(bool)> doDealReqFun);
	void SendDealReq_Lenovo(std::string& openid,std::string& accessToken,std::function<void(bool)> doDealReqFun);
	void SendDealReq_360(std::string& openid,std::string& accessToken,std::function<void(bool)> doDealReqFun);
	//快用苹果助手
	void SendDealReq_7659(std::string& openid,std::string& accessToken,std::function<void(bool)> doDealReqFun);


   /*void insertPlayerRecord(u32 zoneId);*/
   //void updateToken();

   //注册player
	void RegisterPlayer(vn::u64 ppid, int oldSocketId);
	void RegisterPlayer(vn::u64 ppid, vn::u32 pptoken, u32 m_gameId, u64 m_playerId);

    u64 m_ppId = 0;
    u32 m_tmpToken = 0;
    u32 m_gameId = 0;
    u64 m_playerId = 0;
   // std::string m_nickname;
    //PlayerHandlerPtr m_playerHandler;
    str8 m_account = "";//官服账户名

    str8 m_openid3 = "";//第三方openid
    PLATFORM_ID m_tpId = PLATFORM_ID_ERROR;//第三方平台id

    OS_ID m_osId = OS_ID_ANDROID; //用户的操作系统

    State m_state = kUnlogin;

    std::map<std::string,uint32_t> ljChannelMap;
};

typedef RefCountedPtr<ClientSession> ClientSessionPtr;

#endif /* defined(__passport_svr__ClientSession__) */
