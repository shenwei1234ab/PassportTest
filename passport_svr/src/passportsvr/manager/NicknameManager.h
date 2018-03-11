/*
 * NicknameManager.h
 *
 *  Created on: 2015年8月21日
 *      Author: tanghailong
 */

#ifndef SRC_PASSPORTSVR_MANAGER_NICKNAMEMANAGER_H_
#define SRC_PASSPORTSVR_MANAGER_NICKNAMEMANAGER_H_

#include "vnSingleton.h"
#include "vnReadWriteLock.h"

#include <unordered_set>

using namespace _vn_ns;


class NicknameManager : public Singleton<NicknameManager>{
public:
	bool init();
	str8 getNickname();
	void deleteNickname(const str8& nickname);
private:
	std::unordered_set<str8> m_nicknames;
	ReadWriteLock m_lock;
};

#endif /* SRC_PASSPORTSVR_MANAGER_NICKNAMEMANAGER_H_ */
