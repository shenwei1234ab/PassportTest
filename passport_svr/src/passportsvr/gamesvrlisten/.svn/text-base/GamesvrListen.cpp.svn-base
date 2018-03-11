/*
 * GamesvrListen.cpp
 *
 *  Created on: 2015年6月9日
 *      Author: leave
 */

#include "../../passportsvr/gamesvrlisten/GamesvrListen.h"

#include "../../passportsvr/gamesvrlisten/GamesvrSession.h"


Link_TCP_Listen * GamesvrListen::create() {
    return vnnew GamesvrListen();
}

void GamesvrListen::_destroy() {
    delete this;
}

Link_TCP_Connection * GamesvrListen::_accepted(const NetAddress &peer) {
    return vnnew GamesvrSession();
}
