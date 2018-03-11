/*
 * LoadBalance.cpp
 *
 *  Created on: 2015年9月28日
 *      Author: leave
 */

#include "../../passportsvr/clientlisten/ClientSession.h"
#include "../../passportsvr/manager/SessionManager.h"
#include "../../passportsvr/WorldManager.h"
#include "../database/PassPortDatabaseConnection.h"
#include "vnBase64.h"

int balanceIndex = 0;

//---------------------------------------- 获取游戏服务器地址-------------------------------------------------------------
VN_IMPL_PACKET_REQUEST2(ClientSession, pkt, req_GetGameSvrAddr) {
	/*
	 Link_TCP_Listen * lister = sWorldManager.GetGamesvrListen();
	 if(!lister){
	 pkt::res_GetGameSvrAddrFail res;
	 res.data.errorCode = -1;
	 this->sendPacket(res);
	 return;
	 }
	 std::vector<NetAddress> linkVtr;
	 std::function<bool (vn::Link_TCP_Connection *)> fn = [&linkVtr](vn::Link_TCP_Connection * _link ) ->bool{
	 if(_link){
	 linkVtr.push_back(_link->peer());
	 }
	 return true;
	 };

	 lister->traverseAccepted(fn);
	 if(linkVtr.empty()){
	 pkt::res_GetGameSvrAddrFail res;
	 res.data.errorCode = -1;
	 this->sendPacket(res);
	 return;
	 }
	 int index = sWorldManager.m_index % linkVtr.size();
	 sWorldManager.m_index++;
	 if(sWorldManager.m_index >0xFFFFFFFF){
	 sWorldManager.m_index = 0;
	 }
	 IpAddress& ip =linkVtr[index].ip;
	 c8 buf[32];
	 sprintf(buf, "%u.%u.%u.%u", (u32)ip.b0, (u32)ip.b1, (u32)ip.b2, (u32)ip.b3);

	 */

	pkt::res_GetGameSvrAddr res;
	/*
	 res.data.ip = "115.159.0.144";//
	 res.data.port = 8811;//8811 for dev

	 res.data.ip ="115.159.101.74";
	 res.data.port = 8802;

	 res.data.ip ="182.254.219.77";
	 res.data.port = 8802;
	 */
	if (balanceIndex == sWorldManager.m_loadBalanceNum) {
		balanceIndex = 0;
	}
	res.data.ip = sWorldManager.m_loadBalance[balanceIndex].ip;
	res.data.port = sWorldManager.m_loadBalance[balanceIndex++].port;

	//res.data.ip =sWorldManager.m_gamesvrIP;
	//res.data.port =sWorldManager.m_gamesvrPort;

	//res.data.port = 8811;
	/*if(ip.b3 == 144){
	 res.data.port = 8811;
	 }*/

	// todo 仅供测试服使用(36001)
//	if (sWorldManager.m_client_port == 36001) {
//		/**/int iVerInfo = atoi(pkt->data.versionInfo.c_str());
//		if(iVerInfo < 120) {
//			res.data.port = 8811;
//		} else {
//			res.data.port = 8813;
//		}
//	}
	VN_LOG_DEBUG("loadBalance:" << res.data.ip << ":" << res.data.port);
	this->sendPacket(res);
}
