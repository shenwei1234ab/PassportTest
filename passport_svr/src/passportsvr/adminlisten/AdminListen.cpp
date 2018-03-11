/*
 * AdminListen.cpp
 *
 *  Created on: 2015年6月9日
 *      Author: leave
 */

#include "../../passportsvr/adminlisten/AdminListen.h"

#include "../../passportsvr/adminlisten/AdminSession.h"

Link_TCP_Listen * AdminListen::create() {
    return vnnew AdminListen();
}

void AdminListen::_destroy() {
    delete this;
}

Link_TCP_Connection * AdminListen::_accepted(const NetAddress &peer) {
    return vnnew AdminSession();
}
