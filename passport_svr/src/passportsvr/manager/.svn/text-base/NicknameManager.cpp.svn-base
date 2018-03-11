/*
 * NicknameManager.cpp
 *
 *  Created on: 2015年8月21日
 *      Author: tanghailong
 */


#include "../../passportsvr/manager/NicknameManager.h"

#include "../database/PassPortDatabaseConnection.h"
#include "tool/TSTime.h"
#include <mutex>
#include "../../passportsvr/manager/DirtyManager2.h"
#include "common/Timer.h"

extern PassPortDatabaseWorkerPool PassPortDatabase;

bool NicknameManager::init() {
	srand((unsigned int)time(0));

	vn::u32 _s = getMSTime();
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(NICKNAME_SEL_ALL);
	PreparedQueryResult result = PassPortDatabase.SynQuery(stmt);
	if (!(result && result->GetRowCount())) {
		return false;
	}
	LOG_DEBUG("[%u ms] SynQuery: %s", getMSTimeDiff(_s, getMSTime()), "NICKNAME_SEL_ALL");

	m_nicknames.reserve(result->GetRowCount());

	do {
		auto name = (*result)[0].GetString();
		/*if (!DirtyManager2::instance().check(name)) {
		 PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(NICKNAME_DEL_NAME);
		 stmt->setString(0, name);
		 PassPortDatabase.Execute(stmt, nullptr, nullptr);
		 continue;
		 }*/
		if (!m_nicknames.insert(std::move(name)).second) {
			return false;
		}
	} while (result->NextRow());
	LOG_DEBUG("[%u ms] NicknameManager::init ok", getMSTimeDiff(_s, getMSTime()));

	return true;
}

str8 NicknameManager::getNickname() {
	std::lock_guard<ReadWriteLock> lck(m_lock);
	if(m_nicknames.empty()) {
		return {};
	}
	//to do random
	union {
		u16 w[2];
		u32 value;
	} index;

	index.w[0] = (u16)rand();
	index.w[1] = (u16)rand();
	index.value %= (u32)m_nicknames.size();
	auto it = m_nicknames.begin();
	while (index.value--) {
		++it;
	}
	return *it;
}

void NicknameManager::deleteNickname(const str8 &nickname) {
	m_lock.lock_write();
	if(m_nicknames.empty()) {
		m_lock.unlock();
		return ;
	}

	auto it = m_nicknames.find(nickname);
	if (it == m_nicknames.end()) {
		m_lock.unlock();
		return ;
	}

	m_nicknames.erase(it);
	m_lock.unlock();

	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(NICKNAME_DEL_NAME);
	stmt->setString(0, nickname);
	PassPortDatabase.Execute(stmt, nullptr, nullptr);
}

