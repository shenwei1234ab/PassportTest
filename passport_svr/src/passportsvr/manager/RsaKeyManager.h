/*
 * RsaKeyManager.h
 *
 *  Created on: 2015年9月18日
 *      Author: leave
 */

#ifndef SRC_PASSPORTSVR_MANAGER_RSAKEYMANAGER_H_
#define SRC_PASSPORTSVR_MANAGER_RSAKEYMANAGER_H_

#include "vnSingleton.h"
#include <map>
#include "common/Util.h"
#include "vnOpenSSL_Verifier.h"



class RsaKeyManager : public vn::Singleton<RsaKeyManager> {
public:
	bool init();

	vn::OpenSSL_KeyPtr GetRsakey(int pfid);

protected:
	std::map<int,vn::OpenSSL_KeyPtr> m_keyPtrMap;
};

#define sRsaKeyManager RsaKeyManager::instance()
#endif /* SRC_PASSPORTSVR_MANAGER_RSAKEYMANAGER_H_ */
