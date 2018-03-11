/*
 * NetLogListen.h
 *
 *  Created on: 2015年11月9日
 *      Author: leave
 */

#ifndef SRC_NETLOGSVR_NETLOGLISTEN_NETLOGLISTEN_H_
#define SRC_NETLOGSVR_NETLOGLISTEN_NETLOGLISTEN_H_

#include "vnLink.h"

using namespace _vn_ns;

class NetLogListen: public Link_TCP_Listen, public AllocatedObject {
public:
	virtual ~NetLogListen() = default;
	static Link_TCP_Listen * create();
protected:
	virtual void _destroy();
	virtual Link_TCP_Connection * _accepted(const NetAddress &peer);
};

#endif /* SRC_NETLOGSVR_NETLOGLISTEN_NETLOGLISTEN_H_ */
