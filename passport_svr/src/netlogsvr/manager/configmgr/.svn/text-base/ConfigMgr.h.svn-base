/*
 * ConfigMgr.h
 *
 *  Created on: 2015年11月9日
 *      Author: leave
 */

#ifndef SRC_NETLOGSVR_MANAGER_CONFIGMGR_CONFIGMGR_H_
#define SRC_NETLOGSVR_MANAGER_CONFIGMGR_CONFIGMGR_H_

#include "vnSingleton.h"
#include<map>
class ConfigMgr: public vn::Singleton<ConfigMgr> {
public:
	ConfigMgr();
	virtual ~ConfigMgr();

	bool loadBaseConfig();

	vn::u16 m_netlog_port = 36901;
	bool m_console_mode = false;

	//DB
	vn::str8 m_sDBInfo;
	vn::s32 m_dbThreads = 1;

	bool loadSqlMap();
	int GetStatementID(const std::string& name) {
		try {
			return logMaps.at(name);
		} catch (...) {
			return -1;
		}
	}
protected:
	std::map<std::string, int> logMaps;
};

#define sConfigMgr ConfigMgr::instance()

#endif /* SRC_NETLOGSVR_MANAGER_CONFIGMGR_CONFIGMGR_H_ */
