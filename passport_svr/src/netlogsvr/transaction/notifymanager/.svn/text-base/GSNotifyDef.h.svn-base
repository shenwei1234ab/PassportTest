/*******************************************************************************
@ ��Ȩ����(C) Tencent 2013
********************************************************************************/
#ifndef __REDGAMESVR_NOTIFYMANAGER_GSNOTIFYDEF_H__
#define __REDGAMESVR_NOTIFYMANAGER_GSNOTIFYDEF_H__


#include "tranbase/transactionmgr/ITransNotify.h"
#include "../macrodef/GamesvrMacro.h"
#include "db/MySQLConnection.h"

/*class DBServiceResponse {
public:
	DBServiceResponse(int cbid, PreparedQueryResult _Result) :
			m_iCallBackId(cbid), m_Result(_Result) {
	}

	int GetAsynID(){
		return m_iCallBackId;
	}

	PreparedQueryResult GetResultSet() {
		return m_Result;
	}

protected:
	int m_iCallBackId;
	PreparedQueryResult m_Result;
};*/

class DBQueryNtf: public ITransNotify {
public:
	DBQueryNtf(int cbid, PreparedQueryResult _Result) :
			ITransNotify(cbid), m_Result(_Result) {
	}

	virtual uint32_t GetId() const {
		return MACROUTIL(NotifyDBQuery);
	}

	PreparedQueryResult GetUserData() const {
		return m_Result;
	}

protected:
	PreparedQueryResult m_Result;
};

class DBExecuteNtf: public ITransNotify {
public:
	DBExecuteNtf(int cbid, bool _Result) :
			ITransNotify(cbid), m_Result(_Result) {
	}

	virtual uint32_t GetId() const {
		return MACROUTIL(NotifyDBExecute);
	}

	bool GetUserData() const {
		return m_Result;
	}

protected:
	bool m_Result;
};

#endif // end of __REDGAMESVR_NOTIFYMANAGER_GSNOTIFYDEF_H__
