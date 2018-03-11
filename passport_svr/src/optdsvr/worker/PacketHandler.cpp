//
//  PacketHandler.cpp
//  optd_srv
//
//  Created by Wayne on 6/19/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "../worker/PacketHandler.h"

void PacketHandler::postPacket(vn::PacketPtr &&packet) {
	  vnassert(queue());
	  //bind (类函数,调用者,参数（绑定）,)
	  queue()->runLoop().post(std::bind(&vn::Packet::process,std::move(packet),this));
}

void PacketHandler::notify_closed() {
    vnassert(queue());
    queue()->runLoop().post(std::bind(&PacketHandler::_closed, this));
}
