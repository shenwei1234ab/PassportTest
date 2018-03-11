/*
 * RsaKeyManager.cpp
 *
 *  Created on: 2015年9月18日
 *      Author: leave
 */

#include "../../passportsvr/manager/RsaKeyManager.h"

#include "../../passportsvr/Common.h"


bool RsaKeyManager::init(){
	m_keyPtrMap[PLATFORM_ID_TEST] = vn::OpenSSL_Key::create_public(TEST_PbuKey.data(), TEST_PbuKey.length());
	m_keyPtrMap[PLATFORM_ID_WANDOUJIA] = vn::OpenSSL_Key::create_public(WANDOUJIA_PublicKey.data(), WANDOUJIA_PublicKey.length());
	m_keyPtrMap[PLATFORM_ID_HUAWEI] = vn::OpenSSL_Key::create_public(HUAWEI_PubKey.data(), HUAWEI_PubKey.length());
	m_keyPtrMap[PLATFORM_ID_LENOVO] = vn::OpenSSL_Key::create_private(LENOVO_AppKey.data(), LENOVO_AppKey.length());
	m_keyPtrMap[PLATFORM_ID_KUAIYONG] = vn::OpenSSL_Key::create_public(KUAIYONG_PublicKey.data(), KUAIYONG_PublicKey.length());
	m_keyPtrMap[PLATFORM_ID_OPPO] = vn::OpenSSL_Key::create_public(OPPO_PubKey.data(), OPPO_PubKey.length());
	m_keyPtrMap[PLATFORM_ID_GIONEE] = vn::OpenSSL_Key::create_public(GIONEE_PublicKey.data(), GIONEE_PublicKey.length());
	m_keyPtrMap[PLATFORM_ID_I4] = vn::OpenSSL_Key::create_public(I4_PubKey.data(), I4_PubKey.length());
	m_keyPtrMap[PLATFORM_ID_PP_IOS] = vn::OpenSSL_Key::create_public(PP_PubKey.data(), PP_PubKey.length());

	for(auto key : m_keyPtrMap){
		if(key.second.get() == NULL){
			VN_LOG_FATAL("RSA key create error! platform id : "<<key.first);
			return false;
		}
	}
	return true;
}

vn::OpenSSL_KeyPtr RsaKeyManager::GetRsakey(int pfid){
	vn::OpenSSL_KeyPtr ret = m_keyPtrMap.at(pfid);
	assert(ret);
	return ret;
}
