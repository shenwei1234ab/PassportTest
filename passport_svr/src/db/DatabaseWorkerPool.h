/*
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _DATABASEWORKERPOOL_H
#define _DATABASEWORKERPOOL_H


#include "MySQLConnection.h"
#include "PreparedStatement.h"
#include "vnLog.h"
#include "QueryResult.h"
#include "ProducerConsumerQueue.h"
#include "vnWorkPool.h"
#include "framework/Session.h"
#include "tranbase/logmgr/Log.h"
#include "../passportsvr/transaction/notifymanager/GSNotifyDef.h"
#include "../passportsvr/transaction/notifymanager/GSNotifyMgr.h"
#include "common/Timer.h"
#include "../passportsvr/entity/AsyncObject.h"
#define MIN_MYSQL_SERVER_VERSION 50100u
#define MIN_MYSQL_CLIENT_VERSION 50100u


template<class T>
class DatabaseWorkerPool {
private:
	enum InternalIndex {
		IDX_ASYNC, IDX_SYNCH, IDX_SIZE
	};

public:
	/* Activity state */
	DatabaseWorkerPool() :
			_connectionInfo(NULL) {
		//WPFatal(mysql_thread_safe(), "Used MySQL library isn't thread-safe.");
		if(!mysql_thread_safe()){
			LOG_FATAL("Used MySQL library isn't thread-safe.");
			exit(1);
		}
		//WPFatal(mysql_get_client_version() >= MIN_MYSQL_CLIENT_VERSION, "TrinityCore does not support MySQL versions below 5.1");
		if (!(mysql_get_client_version() >= MIN_MYSQL_CLIENT_VERSION)) {
			LOG_FATAL("TrinityCore does not support MySQL versions below 5.1");
			exit(1);
		}
	}

	~DatabaseWorkerPool() {
		delete _connectionInfo;
	}

	bool Open(const std::string& infoString, vn::u8 async_threads) {
		_connectionInfo = new MySQLConnectionInfo(infoString);

		VN_LOG_INFO(
				"Opening DatabasePool . Asynchronous connections: "<< async_threads <<".");

		bool res = OpenConnections(async_threads);

		if (!res)
			return res;
		return res;

	}

	void Close() {
		VN_LOG_INFO("Closing down DatabasePool .");
		m_WorkPool.cleanup();
	}


	void AsyncQuery(PreparedStatement* stmt, AsyncObjectPtr obj,
			std::function<void(PreparedQueryResult)> doResulet = nullptr)
	{
		auto fn = [stmt](void * local, AsyncObjectPtr obj, std::function<void(PreparedQueryResult)> doResulet) {
					//db线程调用
					T* t = (T*)local;
					PreparedResultSet* ret = t->Query(stmt);

					delete stmt;
					//投掷到obj线程
					obj->async(obj, std::bind(std::move(doResulet), PreparedQueryResult(ret)));
				};
				m_WorkPool.post(std::bind(fn, std::placeholders::_1, obj, std::move(doResulet)));
	}

	void AsyncExecute(PreparedStatement* stmt, AsyncObjectPtr obj,
						std::function<void(bool)> doResulet = nullptr)
	{
				auto fn = [stmt](void * local, AsyncObjectPtr obj, std::function<void(bool)> doResulet) {
					T* t = (T*)local;
					bool ret = t->Execute(stmt);

					delete stmt;

					if(!obj || !doResulet) {
						return;
					}
					obj->async(obj,std::bind(std::move(doResulet), ret));
				};
				m_WorkPool.post(std::bind(fn, std::placeholders::_1, std::move(obj), std::move(doResulet)));
	}


	void Query(PreparedStatement* stmt, vn::RefCountedPtr<Session> sessionPtr,
			std::function<void(PreparedQueryResult)> doResulet = nullptr) {
		auto fn = [stmt](void * local, SessionPtr session, std::function<void(PreparedQueryResult)> doResulet) {
			T* t = (T*)local;
			PreparedResultSet* ret = t->Query(stmt);

			delete stmt;

		/*	if (!ret )
			{
				delete ret;
				return;
			}
*/
			if(!session || !doResulet) {
				return;
			}
			session->post(std::bind(std::move(doResulet), PreparedQueryResult(ret)));
		};
		m_WorkPool.post(std::bind(fn, std::placeholders::_1, std::move(sessionPtr), std::move(doResulet)));
	}

	PreparedQueryResult SynQuery(PreparedStatement* stmt) {

		PreparedQueryResult resultPrt= nullptr;
		auto fn = [stmt,&resultPrt](void * local) {
			T* t = (T*)local;

			PreparedResultSet* ret  = t->Query(stmt);
			resultPrt.reset(ret);
			delete stmt;
		};
		if(!m_WorkPool.send(std::move(fn))){
			VN_LOG_ERROR( "DB SynQuery Error!!!");
		}
		return resultPrt;
	}

	void Execute(PreparedStatement* stmt, vn::RefCountedPtr<Session> sessionPtr,
				std::function<void(bool)> doResulet = nullptr) {
		auto fn = [stmt](void * local, SessionPtr session, std::function<void(bool)> doResulet) {
			T* t = (T*)local;
			bool ret = t->Execute(stmt);

			delete stmt;

			if(!session || !doResulet) {
				return;
			}
			session->post(std::bind(std::move(doResulet), ret));
		};
		m_WorkPool.post(std::bind(fn, std::placeholders::_1, std::move(sessionPtr), std::move(doResulet)));
	}

	bool SynExecute(PreparedStatement* stmt) {
		bool ret = false;
		PreparedQueryResult resultPrt= nullptr;
		auto fn = [stmt,&ret](void * local) {
			T* t = (T*)local;
			ret = t->Execute(stmt);
			delete stmt;
		};
		if(!m_WorkPool.send(std::move(fn))){
			VN_LOG_ERROR( "DB SynExecute Error!!!");
		}
		return ret;
	}
	/**
	 Other
	 */

	PreparedStatement* GetPreparedStatement(vn::u32 index) {
		return new PreparedStatement(index);
	}

	void ActionQuery(PreparedStatement* stmt, int cbid) {
		auto fn = [stmt](void * local, int cbid) {
			T* t = (T*)local;
			PreparedResultSet* ret = t->Query(stmt);

			delete stmt;

			DBQueryNtf ntf(cbid,PreparedQueryResult(ret));
			SNotifyManager.DispatchNotify(ntf);

		};
		m_WorkPool.post(std::bind(fn, std::placeholders::_1, cbid));
	}
	void ActionExecute(PreparedStatement* stmt, int cbid) {
		auto fn = [stmt](void * local, int cbid) {
			T* t = (T*)local;
			bool ret = t->Execute(stmt);

			delete stmt;

			DBExecuteNtf ntf(cbid,ret);
			SNotifyManager.DispatchNotify(ntf);

		};
		m_WorkPool.post(std::bind(fn, std::placeholders::_1, cbid));
	}
private:
	bool OpenConnections(vn::u8 numConnections) {
		MySQLConnectionInfo* pConnectionInfo = _connectionInfo;
		std::function<bool(void *&)> init =
				[pConnectionInfo](void *& local)-> bool {
					T* t;
					t = new T(*pConnectionInfo);

					bool res = t->Open();

					if (res) {
						if (mysql_get_server_version( t->GetHandle()) < MIN_MYSQL_SERVER_VERSION) {
							VN_LOG_ERROR( "TrinityCore does not support MySQL versions below 5.1");
							res = false;
						}
						if (!res) {
							delete t;
							VN_LOG_ERROR( "DatabasePool NOT opened. There were errors opening the MySQL connections. Check your SQLDriverLogFile ");
						}
						local = t;
					}
					return res;
				};
		std::function<void(void *)> destroy = [](void * local) {
			T* t = (T*)local;
			t->Close();
		};
		size_t nRet = m_WorkPool.startup(numConnections, std::move(init), std::move(destroy));
		if (nRet != numConnections) {
			m_WorkPool.cleanup();
			return false;
		}

		return true;
	}

	char const* GetDatabaseName() const {
		return _connectionInfo->database.c_str();
	}

	MySQLConnectionInfo* _connectionInfo;

	vn::WorkPool m_WorkPool;
};

#endif
