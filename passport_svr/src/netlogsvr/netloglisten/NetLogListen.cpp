/*
 * NetLogListen.cpp
 *
 *  Created on: 2015年11月9日
 *      Author: leave
 */

#include "NetLogListen.h"
#include "NetLogSession.h"
#include "netlogsvr/Protocols.h"

Link_TCP_Listen * NetLogListen::create() {
    return vnnew NetLogListen();
}

void NetLogListen::_destroy() {
    delete this;
}

Link_TCP_Connection * NetLogListen::_accepted(const NetAddress &peer) {
    return vnnew NetLogSession();
}
