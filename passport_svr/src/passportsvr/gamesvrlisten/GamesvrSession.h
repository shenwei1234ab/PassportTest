/*
 * PassportSession.h
 *
 *  Created on: 2015年6月11日
 *      Author: leave
 */

#ifndef SRC_SERVER_GAMESVR_SESSION_H_
#define SRC_SERVER_GAMESVR_SESSION_H_
#include "framework/Session.h"

#include "vnPacketFactory.h"
#include <functional>

#include "../../passportsvr/Protocols.h"
#include "manager/TimerManager.h"

class GamesvrSession : public Session{
	VN_DECL_PACKET_HANDLER()
public:
	virtual vn::PacketPtr createRequest(vn::u32 pid);

	GamesvrSession();
	virtual ~GamesvrSession();

	void getGotyeAccessToken(std::function<void(bool)> doCallBackFun);
	void getFriendList(bool isRightToken, RefCountedPtr<pkt::req_GetFriendList> pkt);

	void GetQQBakance(PlayerPtr pPlayer,pkt::req_GetBalance& pkt);
	void doPlayerGetBalance(u64 ppid, pkt::req_GetBalance& pkt, bool isTencent = false,u32 diamond = 0, u32 gen_balance = 0, u32 first_save = 0, u32 save_amt = 0,std::string JSON="");

	//void GetQQBalance(u64 ppid, pkt::req_GetBalance* pkt);
	//void GetWXBalance(u64 ppid, pkt::req_GetBalance* pkt);

	void GetTencentBalance(u64 ppid, pkt::req_GetBalance* pkt,u32 platid);
	void TencentPresent(u64 ppid, pkt::req_Present* pkt,u32 platid);
	void TencentPay(u64 ppid, pkt::req_Pay* pkt,u32 platid);
	void TencentCancelPay(u64 ppid, pkt::req_Pay* pkt,u32 platid,std::string billno);
	void TencentSubscribeM(u64 ppid, pkt::req_SubscribeM* pkt,u32 platid);

	void SendJosnPkg(pkt::res_JsonDate& res,int error);

protected:
	void _cleanTimeOutSession();
	void _createPlayer(PlayerPtr& pPlayer,u64 ppid,u64& playerid ,u32 gameid,u32 zoneid);
	uint64_t m_lastHeartTime = 0;
	timer_handler m_hander;
};

typedef vn::RefCountedPtr<GamesvrSession> GamesvrSessionPtr;

#endif /* SRC_SERVER_GAMESVR_SESSION_H_ */
