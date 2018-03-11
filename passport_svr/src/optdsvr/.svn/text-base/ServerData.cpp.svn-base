//
//  ServerData.cpp
//  optd_srv
//
//  Created by Wayne on 12/16/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "ServerData.h"

using namespace vn;
void ServerData::save(vn::DataStream &s) const {
    s << last_registration_date.date;
    s << announcementId << announcementContent;
}

void ServerData::load(vn::DataStream &s) {
    s >> last_registration_date.date;
    s >> announcementId >> announcementContent;
}

void ServerData::save(vn::Variable_object &obj) const {
    obj.setAttribute("last_registration_date", vnnew Variable_string(last_registration_date.convertToString()));
    obj.setAttribute("announcement_id", vnnew Variable_int32(announcementId));
    obj.setAttribute("announcement_content", vnnew Variable_string(announcementContent));
}

void ServerData::load(vn::Variable_object &obj) {
    auto vstr = obj.queryAttributeStringV("last_registration_date");
    if (vstr) {
        last_registration_date.convertFromString(vstr->value());
    }
    announcementId = obj.queryAttributeInt32("announcement_id", announcementId);
    vstr = obj.queryAttributeStringV("announcement_content");
    if (vstr) {
        announcementContent = vstr->value();
    }
}
