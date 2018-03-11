//
//  ConsoleApp.h
//  passport_svr
//
//  Created by Wayne on 6/12/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __passport_svr__ConsoleApp__
#define __passport_svr__ConsoleApp__

#include "framework/ServerApp.h"
#include "vnLink.h"

class ConsoleApp : public ServerApp {
    DECL_SERVER_APP()
public:
    virtual bool initialize(int argc, const char *argv[]);
    virtual void finalize();
private:
    LinkPool *m_pool = nullptr;
    Link_TCP_Connection *m_link = nullptr;
};

#endif /* defined(__passport_svr__ConsoleApp__) */
