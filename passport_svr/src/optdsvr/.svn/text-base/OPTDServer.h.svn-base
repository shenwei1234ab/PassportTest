//
//  PassportServer.h
//  passport_svr
//
//  Created by Wayne on 6/9/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __optd_svr__OPTDServer__
#define __optd_svr__OPTDServer__

#include "framework/ServerApp.h"
#include "vnVariable.h"
#include "vnLink.h"

class PassportSession;

class OPTDServer : public ServerApp {
    DECL_SERVER_APP()
public:
    virtual bool initialize(int argc, const char *argv[]) override;
    virtual void finalize() override;
    
protected:
    virtual void update(f32 deltaTime);
    bool _console_command(RefCountedPtr<Variable_object> cmds);

	bool _startDB();
	bool _stopDB();
    LinkPool *m_linkPool = nullptr;
	Link_TCP_Listen *m_clientListen = nullptr; //客户端监听器

private:





};
#endif
