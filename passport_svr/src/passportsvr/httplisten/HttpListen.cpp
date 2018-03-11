//
//  HttpListen.cpp
//  demo_svr
//
//  Created by Wayne on 8/12/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "../../passportsvr/httplisten/HttpListen.h"

#include "../../passportsvr/httplisten/HttpLink.h"

void HttpListen::_init() {
    
}

void HttpListen::_destroy() {
    
}

Link_TCP_Connection * HttpListen::_accepted(const NetAddress &peer) {
    return vnnew HttpLink();
}