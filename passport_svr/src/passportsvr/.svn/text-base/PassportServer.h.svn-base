//
//  PassportServer.h
//  passport_svr
//
//  Created by Wayne on 6/9/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __passport_svr__PassportServer__
#define __passport_svr__PassportServer__

#include "framework/ServerApp.h"
#include "../passportsvr/httplisten/HttpListen.h"
#include "vnVariable.h"
#include "vnLink.h"

class PassportSession;

class PassportServer : public ServerApp {
    DECL_SERVER_APP()
public:
    virtual bool initialize(int argc, const char *argv[]) override;
    virtual void finalize() override;
    
protected:

    virtual void update(f32 deltaTime);
//    bool _loadConfig();
    
    
//    size_t m_thread_count = 0;
//    u16 m_client_port = 46001;
//    u16 m_admin_port = 46002;
//    u16 m_gamesvr_port = 46003;
//    u16 m_http_port = 46004;
//    bool m_console_mode = false;
    
    bool _console_command(RefCountedPtr<Variable_object> cmds);
    
	bool _startDB();
	bool _stopDB();
    LinkPool *m_linkPool = nullptr;


    //PassportSession * m_passprotConnecter = nullptr;

//    RefCountedPtr<Variable_object> m_config;

	Link_TCP_Listen *m_clientListen = nullptr; //客户端监听器
	Link_TCP_Listen *m_gamesvrListen = nullptr;//friendsvr，dealsvr监听器
	Link_TCP_Listen *m_adminListen = nullptr;//管理端口监听
   HttpListen m_http_listen ; //第三方回调监听器(http协议）

};
//#define sPassportServer PassportServer::instance()

#endif /* defined(__passport_svr__PassportServer__) */
