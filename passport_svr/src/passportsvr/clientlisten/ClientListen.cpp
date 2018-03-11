//
//  ClientListen.cpp
//  passport_svr
//
//  Created by Wayne on 6/9/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "../../passportsvr/clientlisten/ClientListen.h"

#include "../../passportsvr/clientlisten/ClientSession.h"


Link_TCP_Listen * ClientListen::create() {
    return vnnew ClientListen();
}

void ClientListen::_destroy() {
    delete this;
}

Link_TCP_Connection * ClientListen::_accepted(const NetAddress &peer) {
    return vnnew ClientSession();
}
