/*
 * NetLogServer.cpp
 *
 *  Created on: 2015年11月9日
 *      Author: leave
 */

#include "NetLogServer.h"

#include "manager/TimerManager.h"
#include "tranbase/notifymanager/NotifyMgr.h"
#include "transaction/GSTransaction.h"
#include "transaction/GSTransactionMgr.h"
#include "transaction/GSTransContext.h"
#include "transaction/notifymanager/GSNotifyMgr.h"
#include "vnLog.h"
#include "manager/configmgr/ConfigMgr.h"
#include "database/NetLogDBConnection.h"
#include "netlogsvr/netloglisten/NetLogListen.h"

NetLogDBWorkerPool NetLogDatabase;

IMPL_SERVER_APP(NetLogServer)

NetLogServer::NetLogServer() {
}

NetLogServer::~NetLogServer() {
}

bool NetLogServer::initialize(int argc, const char *argv[]) {
	if (!sConfigMgr.loadBaseConfig()) {
		return false;
	}
	sConfigMgr.loadSqlMap();
	if (!_startDB()) {
		return false;
	}

	m_linkPool = LinkPool::create(1);
	if (!m_linkPool) {
		VN_LOG_FATAL("LinkPool create fail!");
		return false;
	}
	VN_LOG_DEBUG("LinkPool init OK! ");

	m_netLogListen = m_linkPool->listen(&NetLogListen::create, NetAddress(0, sConfigMgr.m_netlog_port), true);
	if (!m_netLogListen) {
		VN_LOG_FATAL("listen *:" << sConfigMgr.m_netlog_port << " FAILED!");
		return false;
	}

	VN_LOG_FATAL("NetLogServer start OK");
	return true;
}

void NetLogServer::finalize() {
	if (m_netLogListen) {
		m_netLogListen->close();
	}
	if (m_linkPool) {
		m_linkPool->release();
	}
	_stopDB();
	VN_LOG_INFO("stop.");
}



bool NetLogServer::_startDB() {
	 mysql_library_init(-1, NULL, NULL);
	 if (!NetLogDatabase.Open(sConfigMgr.m_sDBInfo, sConfigMgr.m_dbThreads)) {
		VN_LOG_FATAL("Cannot connect to database");
		return false;
	}
	 return true;
}
bool NetLogServer::_stopDB() {
	NetLogDatabase.Close();
	 mysql_library_end();
	return true;
}

void NetLogServer::update(f32 deltaTime) {
	SGSTransactionMgr.Process();
	SNotifyManager.Process();
	sTimerManager.updateTimers(deltaTime);
}
