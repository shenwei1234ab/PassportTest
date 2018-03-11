//
//  ServerData.h
//  optd_srv
//
//  Created by Wayne on 12/16/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__ServerData__
#define __optd_srv__ServerData__

#include "DateTime.h"

#include "vnDataStream.h"
#include "vnVariable.h"

struct ServerData : vn::AllocatedObject {
    Date last_registration_date;
    vn::u32 announcementId = 0;
    vn::str8 announcementContent;
    
    void save(vn::DataStream &) const;
    void load(vn::DataStream &);
    
    void save(vn::Variable_object &obj) const;
    void load(vn::Variable_object &obj);
};

#endif /* defined(__optd_srv__ServerData__) */
