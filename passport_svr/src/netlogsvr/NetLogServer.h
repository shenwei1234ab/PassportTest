/*
 * NetLogServer.h
 *
 *  Created on: 2015年11月9日
 *      Author: leave
 */

#ifndef SRC_NETLOGSVR_NETLOGSERVER_H_
#define SRC_NETLOGSVR_NETLOGSERVER_H_

#include "framework/ServerApp.h"
#include "vnVariable.h"
#include "vnLink.h"

class NetLogServer: public ServerApp {
DECL_SERVER_APP()
public:
	NetLogServer();
	virtual ~NetLogServer();

public:
	virtual bool initialize(int argc, const char *argv[]) override;
	virtual void finalize() override;

protected:

	virtual void update(f32 deltaTime) override;

	bool _startDB();
	bool _stopDB();

	LinkPool *m_linkPool = nullptr;
	Link_TCP_Listen *m_netLogListen = nullptr; //监听器
};

#endif /* SRC_NETLOGSVR_NETLOGSERVER_H_ */
