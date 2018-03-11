//
//  ClientSession.h
//  passport_svr
//
//  Created by Wayne on 6/9/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __optd_svr__ClientSession__
#define __optd_svr__ClientSession__

#include "framework/Session.h"

#include "vnPacketFactory.h"
#include <memory>
#include "common/Util.h"
#include "db/QueryResult.h"
#include <functional>

#include "../../optdsvr/Protocols.h"
#include "vnVariableJSON.h"
#include "vnSpinLock.h"
#include "vnHttpResponse.h"
//#include "tool/TSTime.h"
std::string _getContent(const vn::HttpResponse& _httpResponse);
vn::RefCountedPtr<vn::Variable_object> _getJsonObj(const vn::HttpResponse& _httpResponse);

class PacketHandler;

class ClientSession : public Session {
    VN_DECL_PACKET_HANDLER()
public:
    virtual vn::PacketPtr createRequest(vn::u32 pid);
    enum State {
    	kUnlogin,
		kLogin,
     };

    void setPacketHandler(PacketHandler *handler);
protected:
    void _init();
    void _destroy();
    vn::SpinLock m_lock;
    vn::u64 m_ppId = 0;
    vn::u32 m_tmpToken = 0;
    vn::u32 m_gameId = 0;
    vn::u64 m_playerId = 0;

    State m_state = kUnlogin;
    PacketHandler *m_handler = nullptr;
    void _setLoginState();

};

typedef vn::RefCountedPtr<ClientSession> ClientSessionPtr;

#endif
