/*
 * AdminListen.h
 *
 *  Created on: 2015年6月9日
 *      Author: leave
 */

#ifndef SRC_SERVER_ADMINLISTEN_H_
#define SRC_SERVER_ADMINLISTEN_H_

#include "vnLink.h"

using namespace _vn_ns;

class AdminListen  : public Link_TCP_Listen, public AllocatedObject{
public:
    virtual ~AdminListen() = default;
    static Link_TCP_Listen * create();
protected:
    virtual void _destroy();
    virtual Link_TCP_Connection * _accepted(const NetAddress &peer);
};

#endif /* SRC_SERVER_ADMINLISTEN_H_ */
