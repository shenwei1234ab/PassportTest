//
//  ServerApp.h
//  passport_svr
//
//  Created by Wayne on 6/9/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __passport_svr__ServerApp__
#define __passport_svr__ServerApp__

#include "vnSingleton.h"
#include "vnRunLoop.h"

#include <set>

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <execinfo.h>

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)


using namespace _vn_ns;



struct ServerAppCreator;
class ServerApp : public Singleton<ServerApp, ServerAppCreator> {
public:
    virtual ~ServerApp();
    virtual bool initialize(int argc, const char *argv[]) = 0;
    virtual void update(f32 deltaTime) {} // seconds.
    virtual void finalize() = 0;
    
    int main(int argc, const char *argv[]);
    
    void quit();
    
//    timer_t createTimer(f32 periods, std::function<void()> fn, u32 repeats = 0); // repeats == -1 means infinite.
//    void destroyTimer(timer_t timer);
    
protected:
    RunLoop m_mainLoop;
    
private:
//    struct Timer : AllocatedObject {
//        f32 periods;
//        f32 timer;
//        u32 repeats;
//        std::function<void()> fn;
//
//        Timer(f32 _p, u32 _r, std::function<void()> &&_f) : periods(_p), timer(0), repeats(_r), fn(std::move(_f)) {}
//    };
//    std::set<Timer *> m_timers;
//    SpinLock m_timers_lock;
//
//    void _updateTimers(f32 dt);
};

struct ServerAppCreator {
    static ServerApp * (*&pfn())();
    static ServerApp * create();
    static void destroy(ServerApp *p);
};

#define DECL_SERVER_APP() \
    public:\
        static ServerApp * create();

#define IMPL_SERVER_APP(cls) \
    ServerApp * cls::create() { return vnnew cls(); }\
    struct _auto_svr_app_creator {\
        _auto_svr_app_creator() { ServerAppCreator::pfn() = &cls::create; }\
    } _auto_svr_app_creator_ins;


#endif /* defined(__passport_svr__ServerApp__) */
