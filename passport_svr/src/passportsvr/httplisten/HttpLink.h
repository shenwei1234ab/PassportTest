//
//  HttpLink.h
//  demo_svr
//
//  Created by Wayne on 8/12/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __demo_svr__HttpLink__
#define __demo_svr__HttpLink__

#include "../../passportsvr/entity/AsyncObject.h"
#include "vnLink.h"
#include "vnHttpRequestParser.h"

using namespace vn;

class HttpLink : public Link_TCP_Connection, public AsyncObject {
public:
    virtual void async(std::function<void()>);
protected:
    virtual void _init();
    virtual void _destroy();
    virtual void _disconnected();
    virtual void _sent(size_t bytes, void *tag);
    virtual void _received(LinkBufferPtr buffers);
    
    void doTestReq();
    void doXiaomiReq();
    void doHuaweiReq();
    void doLenovoReq();
    void do360Req();
    void doKuaiyongReq();
    void doUCReq();
    void doAnzhiReq();
    void doXmwanReq();
    void doBaiduReq();
    void doOppoReq();
    void doPptvReq();
    void doGioneeReq();
    void doFlymeReq();
    void doAppchinaReq();
    void doXYReq();
    void do19196Req();
    void do4399Req();
    void doI4Req();
    void doCcplayReq();
    void doCoolyunReq();
    void doVivoReq();
    void doTBTReq();
    void doHMW_AND_Req();
    void doHMW_IOS_Req();
    void doSougouReq();
    void doSinaReq();
    void doPpsReq();
    void doGfanReq();
    void doYoukuReq();
    void doIAppleReq();

    void doWandoujiaReq();
    void doMuzhiwanReq();
    void doDownjoyReq();

    void doQQReq();

    void doGuoPanAndReq();
    void doGuoPanIOSReq();
    void doPPIOSReq();
    void doXINMEI365Req();

    //登录验证
    void doVerify();

    HttpRequestParser m_parser;
    
    NetAddress m_peer;
};

#endif /* defined(__demo_svr__HttpLink__) */
