/*
 * Account.h
 *
 *  Created on: 2015年8月17日
 *      Author: leave
 */

#ifndef SRC_PASSPORTSVR_ENTITY_ACCOUNT_H_
#define SRC_PASSPORTSVR_ENTITY_ACCOUNT_H_



#include "vnRefCounted.h"

using namespace _vn_ns;

class Account : public RefCounted{
public:
	u64 ppid;
	int platformid;
	str8 openid;
	str8 accesstoken;
	int os;
	str8 nickname;
};

typedef RefCountedPtr<Account> AccountPtr;


#endif /* SRC_PASSPORTSVR_ENTITY_ACCOUNT_H_ */
