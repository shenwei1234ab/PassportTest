/*
 * Player_Subscribe.h
 *
 *  Created on: 2015年10月13日
 *      Author: leave
 */

#ifndef SRC_PASSPORTSVR_ENTITY_PLAYER_SUBSCRIBE_H_
#define SRC_PASSPORTSVR_ENTITY_PLAYER_SUBSCRIBE_H_


#include "vnRefCounted.h"

using namespace _vn_ns;

class Player_Subscribe : public RefCounted{
public:
	u64 id = 0;
	u64 playerid = 0;
	std::string inner_productid;
	u64 begintime;
	u64 endtime;
	u32 grandtotal_opendays = 0;
};

typedef RefCountedPtr<Player_Subscribe> PlayerSubscribePtr;


#endif /* SRC_PASSPORTSVR_ENTITY_PLAYER_SUBSCRIBE_H_ */
