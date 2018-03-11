/*
 * Player.h
 *
 *  Created on: 2015年6月25日
 *      Author: leave
 */

#ifndef SRC_PASSPORTSVR_ENTITY_PLAYER_H_
#define SRC_PASSPORTSVR_ENTITY_PLAYER_H_


#include "vnRefCounted.h"

using namespace _vn_ns;

class Player : public RefCounted{
public:
	u64 playerid;
	u64 ppid;
	u32 gameid;
	u32 zoneid;
	u32 diamond;
	u32 gen_balance;
	u32  first_save;
	u32 save_amt;
};

typedef RefCountedPtr<Player> PlayerPtr;

#endif /* SRC_PASSPORTSVR_ENTITY_PLAYER_H_ */
