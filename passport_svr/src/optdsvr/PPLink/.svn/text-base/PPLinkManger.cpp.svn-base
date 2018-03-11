/*
 * PPLinkManger.cpp
 *
 *  Created on: 2016年4月19日
 *      Author: leave
 */

#include "PPLinkManger.h"
//#include "WorldManager.h"
#include "vnLink.h"
#include "vnLog.h"
#include "../worker/BSTool.h"

bool PPLinkManger::init(vn::LinkPool * linkPool) {
	m_linkPool = linkPool;
	std::string sPPAddress = "115.159.36.200:36002"; //m_config->queryAttributeString("idip_address", "");
	m_checkdiff = 5;
	m_ppaddr.from_string(sPPAddress.c_str());
	m_pPSession = (PPSession*)m_linkPool->connect([]() {return new PPSession();}, m_ppaddr);
	m_pPSession->grab();
	m_pPSession->SetCallbackFn(std::bind(&PPLinkManger::PassportResponse,this,std::placeholders::_1));
	return true;
}



void PPLinkManger::update(vn::u64 diff){
	m_updateDiffSum+=diff;
	if (m_updateDiffSum > m_checkdiff) {
		if (!m_pPSession->connected()) {
			VN_LOG_ERROR("PPSession is reconnect...");
			//清空回调映射
			std::string nullStr;
			for (auto itr : m_CBMap) {
				CallbackFn fn = itr.second;
				if (fn) {
					fn(-1, nullStr);
				}
			}
			m_CBMap.clear();
			for (auto itr : m_SnCBMap) {
				itr.second.first->post(std::bind(itr.second.second,-1, nullStr));
			}
			m_SnCBMap.clear();
			m_linkPool->connect([this]() {return m_pPSession.get();}, m_ppaddr);
			m_pPSession->grab();
		} else {
			//发送心跳包
			::pkt::req_Heart res;
			res.data.serverid = 123; //todo配置
			vn::s32 http_handle = 0;
			m_pPSession->SendToPassport(http_handle, res);
		}
		m_updateDiffSum = diff;
	}
}

void PPLinkManger::finalize(){
	m_pPSession->close();
	m_linkPool = nullptr;
}


vn::s32 PPLinkManger::_GetCBId()
{
	if( m_CallbackId < 1 )
	{
		m_CallbackId = 1;
	}

    //int 越界了情况
	CallbackMap::iterator it = m_CBMap.find(m_CallbackId);
	vn::s32 cnt = 0;
	while( it != m_CBMap.end() )
	{
		++cnt;
		if( cnt >= 100 ) { return -1;}
		++m_CallbackId;
		if( m_CallbackId < 1 ) { m_CallbackId = 1; }
		it = m_CBMap.find(m_CallbackId);
	}

	SessionCallbackMap::iterator itSn = m_SnCBMap.find(m_CallbackId);
	cnt = 0;
	while( itSn != m_SnCBMap.end() )
	{
		++cnt;
		if( cnt >= 100 ) { return -1;}
		++m_CallbackId;
		if( m_CallbackId < 1 ) { m_CallbackId = 1; }
		itSn = m_SnCBMap.find(m_CallbackId);
	}
	return m_CallbackId++;
}

void PPLinkManger::PassportResponse(pkt::res_JsonDate* pkt){

	vn::u32 callbackId =  pkt->data.handle;
	SessionCallbackMap::iterator itSn = m_SnCBMap.find(callbackId);
	CallbackMap::iterator it = m_CBMap.find(callbackId);
	AsyncObjCallbackMap::iterator asyIt = m_AsyncCBMap.find(callbackId);


	if( it != m_CBMap.end() ){
		CallbackFn& callbackFn = it->second;
		callbackFn(pkt->data.result,pkt->data.jsonvalue);
		m_CBMap.erase(it);
	} else if (itSn != m_SnCBMap.end()) {
		std::pair<vn::RefCountedPtr<Session>,CallbackFn> pair = itSn->second;
		pair.first->post(std::bind(pair.second,pkt->data.result, pkt->data.jsonvalue));
		m_SnCBMap.erase(itSn);
	}
	else  if(asyIt != m_AsyncCBMap.end())
	{
		std::pair<AsyncObjectPtr ,CallbackFn> pair = asyIt->second;
		pair.first->async(pair.first,std::bind(pair.second,pkt->data.result, pkt->data.jsonvalue));
		m_AsyncCBMap.erase(asyIt);
	}
	else {
		VN_LOG_ERROR("Can't find callback Id ["<< callbackId <<"]");
	}
}

//验证请求
int PPLinkManger::SendVerifyReq(CallbackFn&& callbackFn, vn::u64 timestamp,
		const char* appId, const char* sig, char* openId, char*  accessToken) {
	vn::s32 callbackId = _GetCBId();
	if (callbackId < 0) {
		VN_LOG_ERROR("Don't have free callback Id...");
		return -1;
	}
	m_CBMap.insert(std::make_pair(callbackId, std::move(callbackFn)));
	::pkt::req_Verify res;
	res.data.timestamp = timestamp;
	res.data.appId = appId;
	res.data.sig = sig;
	res.data.ppid = openId;//std::move(std::string(openId,MAX_OPENID_LEN));
	res.data.ppToken = accessToken;//std::move(std::string(accessToken,MAX_TOKEN_LEN));
	m_pPSession->SendToPassport(callbackId,res);
	return 0;
}

int PPLinkManger::SendVerifyReq(vn::RefCountedPtr<Session> session,CallbackFn&& callbackFn, vn::u64 timestamp,
				const char* appId, const char* sig, char* openId, char*  accessToken){
	vn::s32 callbackId = _GetCBId();
	if (callbackId < 0) {
		VN_LOG_ERROR("Don't have free callback Id...");
		return -1;
	}
	m_SnCBMap.insert(std::make_pair(callbackId, std::make_pair(session,std::move(callbackFn))));
	::pkt::req_Verify res;
	res.data.timestamp = timestamp;
	res.data.appId = appId;
	res.data.sig = sig;
	res.data.ppid = openId;//std::move(std::string(openId,MAX_OPENID_LEN));
	res.data.ppToken = accessToken;//std::move(std::string(accessToken,MAX_TOKEN_LEN));
	m_pPSession->SendToPassport(callbackId,res);
	return 0;
}

int PPLinkManger::SendVerifyReq(AsyncObjectPtr objPtr,CallbackFn&& callbackFn, vn::u64 timestamp,
					const char* appId, const char* sig, char* openId, char*  accessToken)
{
	vn::s32 callbackId = _GetCBId();
		if (callbackId < 0) {
			VN_LOG_ERROR("Don't have free callback Id...");
			return -1;
		}
		m_AsyncCBMap.insert(std::make_pair(callbackId, std::make_pair(objPtr,std::move(callbackFn))));
		::pkt::req_Verify res;
		res.data.timestamp = timestamp;
		res.data.appId = appId;
		res.data.sig = sig;
		res.data.ppid = openId;//std::move(std::string(openId,MAX_OPENID_LEN));
		res.data.ppToken = accessToken;//std::move(std::string(accessToken,MAX_TOKEN_LEN));
		m_pPSession->SendToPassport(callbackId,res);
		return 0;
}

//passport余额查询
int PPLinkManger::SendGetBalance(AsyncObjectPtr objPtr,CallbackFn&& callbackFn, char* openId,const char* appId )
{
	vn::s32 callbackId = _GetCBId();
	if (callbackId < 0) {
		VN_LOG_ERROR("Don't have free callback Id...");
		return -1;
	}
	m_AsyncCBMap.insert(std::make_pair(callbackId, std::make_pair(objPtr,std::move(callbackFn))));
	pkt::req_GetBalance req;
	req.data.appid = appId;
	req.data.openid = openId;
	req.data.zoneid = "1";
	m_pPSession->SendToPassport(callbackId,req);
	return 0;
}


//添加钻石
int PPLinkManger::SendPresent(AsyncObjectPtr objPtr,CallbackFn&& callbackFn, char* openId,vn::s32 diamondCount,const char* appId)
{
	vn::s32 callbackId = _GetCBId();
	if (callbackId < 0) {
		VN_LOG_ERROR("Don't have free callback Id...");
		return -1;
	}
	m_AsyncCBMap.insert(std::make_pair(callbackId, std::make_pair(objPtr,std::move(callbackFn))));
	pkt::req_Present req;
	req.data.appid = appId;
	req.data.openid = openId;
	req.data.zoneid = "1";
	ConvertToStr(diamondCount,req.data.presenttimes);
	m_pPSession->SendToPassport(callbackId,req);
	return 0;
}



//减少钻石
int PPLinkManger::SendPay(AsyncObjectPtr objPtr,CallbackFn&& callbackFn, char* openId,vn::s32 diamondCount,const char* appId )
{
	vn::s32 callbackId = _GetCBId();
		if (callbackId < 0) {
			VN_LOG_ERROR("Don't have free callback Id...");
			return -1;
		}
		m_AsyncCBMap.insert(std::make_pair(callbackId, std::make_pair(objPtr,std::move(callbackFn))));
		pkt::req_Pay req;
		req.data.appid = appId;
		req.data.openid = openId;
		req.data.zoneid = "1";
		ConvertToStr(diamondCount,req.data.amt);
		m_pPSession->SendToPassport(callbackId,req);
		return 0;
}
