//
//  ClientSession.h
//  passport_svr
//
//  Created by Wayne on 6/9/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __optd_svr__Client__
#define __optd_svr__Client__

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

class Client : public Session {
public:
    //virtual vn::PacketPtr createRequest(vn::u32 pid);
    enum State {
    	kUnlogin,
		kLogin,
     };
    void setPacketHandler(PacketHandler *handler);
protected:
    virtual void _received(vn::LinkBufferPtr buffers)override;
    virtual void _connected()override;
    virtual void _disconnected()override;

    void _init();
    void _destroy();
    vn::SpinLock m_lock;
    vn::u64 m_ppId = 0;
    vn::u32 m_tmpToken = 0;
    vn::u32 m_gameId = 0;
    vn::u64 m_playerId = 0;
   // std::string m_nickname;
    //PlayerHandlerPtr m_playerHandler;

    State m_state = kUnlogin;
    PacketHandler *m_handler = nullptr;
    void _setLoginState();


private:
    vn::u8 m_bufferMask = 0;
    void _mask(const vn::LinkBufferPtr &buffer);
    void _bad_data();

};

typedef vn::RefCountedPtr<Client> ClientPtr;

#endif /* defined(__passport_svr__ClientSession__) */
