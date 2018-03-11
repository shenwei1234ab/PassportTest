/*
 * Order.h
 *
 *  Created on: 2015年6月25日
 *      Author: leave
 */

#ifndef SRC_PASSPORTSVR_ENTITY_ORDER_H_
#define SRC_PASSPORTSVR_ENTITY_ORDER_H_


#include "vnRefCounted.h"

using namespace _vn_ns;

class Order : public RefCounted{
public:
	u64 orderid;
	u64 ppid;
	u64 playerid;
	u32 gameid;
	u32 zoneid;
	u32 productid;
	u32 platformid;
	u32 status;
	str8 receipt;
};

typedef RefCountedPtr<Order> OrderPtr;

#endif /* SRC_PASSPORTSVR_ENTITY_ORDER_H_ */
