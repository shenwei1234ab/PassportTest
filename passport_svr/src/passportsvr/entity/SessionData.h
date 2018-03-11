/*
 * Session.h
 *
 *  Created on: 2015年7月7日
 *      Author: leave
 */

#ifndef SRC_PASSPORTSVR_ENTITY_SESSIONDATA_H_
#define SRC_PASSPORTSVR_ENTITY_SESSIONDATA_H_





#include "vnRefCounted.h"

using namespace _vn_ns;

class SessionData : public RefCounted{
public:
//	u64 sessionid;
	u64 ppid;
	u32 gameid;
	u32 pptoken;
};

typedef RefCountedPtr<SessionData> SessionDataPtr;

#endif /* SRC_PASSPORTSVR_ENTITY_SESSIONDATA_H_ */
