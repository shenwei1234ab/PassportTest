/*
 * PPLinkManger.h
 *
 *  Created on: 2016年4月19日
 *      Author: leave
 */

#ifndef SRC_OPTDSVR_PPLINK_PPLINKMANGER_H_
#define SRC_OPTDSVR_PPLINK_PPLINKMANGER_H_

#include "vnSingleton.h"
#include "PPSession.h"
#include "vnRefCounted.h"
#include "vnNetAddress.h"
#include "../Protocols.h"
#include "../../passportsvr/entity/AsyncObject.h"
namespace vn {
class LinkPool;
}

class PPLinkManger: public vn::Singleton<PPLinkManger> {
public:
	typedef vn::RefCountedPtr<PPSession> PPSessionPtr;
	typedef std::function<void(vn::s32 result,std::string&)> CallbackFn;
	typedef std::unordered_map<vn::s32,CallbackFn> CallbackMap;
	typedef std::unordered_map<vn::s32,std::pair<vn::RefCountedPtr<Session>,CallbackFn>> SessionCallbackMap;
	typedef std::unordered_map<vn::s32,std::pair<AsyncObjectPtr ,CallbackFn>> AsyncObjCallbackMap;

	bool init(vn::LinkPool * linkPool);

	void update(vn::u64 diff);

	void finalize();

	//验证请求
	//在主线程中使用
	int SendVerifyReq(CallbackFn&& callbackFn, vn::u64 timestamp,
			const char* appId, const char* sig, char* openId, char*  accessToken);
	//在 session 对象的消息处理中使用（一般都不再主线程）
	int SendVerifyReq(vn::RefCountedPtr<Session> session,CallbackFn&& callbackFn, vn::u64 timestamp,
				const char* appId, const char* sig, char* openId, char*  accessToken);

	////////////////////////////////////////////////////////
	//用户查询（appId (游戏id)和sig为任意值）
	int SendVerifyReq(AsyncObjectPtr objPtr,CallbackFn&& callbackFn, vn::u64 timestamp,
					const char* appId, const char* sig, char* openId, char*  accessToken);
	//余额查询
	int SendGetBalance(AsyncObjectPtr objPtr,CallbackFn&& callbackFn, char* openId,const char* appId ="2");
	//添加钻石
	int SendPresent(AsyncObjectPtr objPtr,CallbackFn&& callbackFn, char* openId,vn::s32 diamondCount,const char* appId ="2");
	//减少钻石
	int SendPay(AsyncObjectPtr objPtr,CallbackFn&& callbackFn, char* openId,vn::s32 diamondCount,const char* appId ="2");

protected:
	vn::s32 _GetCBId();
	void PassportResponse(pkt::res_JsonDate* pkt);
protected:
	vn::LinkPool * m_linkPool = nullptr;
	PPSessionPtr m_pPSession;
	vn::NetAddress m_ppaddr;
	vn::u32 m_checkdiff = 1;
	vn::u32 m_updateDiffSum = 0;
	vn::s32 m_CallbackId = 0;
	CallbackMap m_CBMap;
	SessionCallbackMap m_SnCBMap;
	AsyncObjCallbackMap m_AsyncCBMap;
};
#define sPPLinkMgr PPLinkManger::instance()
#endif /* SRC_OPTDSVR_PPLINK_PPLINKMANGER_H_ */
