//
//  ConsoleApp.cpp
//  passport_svr
//
//  Created by Wayne on 6/12/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "ConsoleApp.h"
#include <iostream>
#include <thread>
#include <cstring>

#include "vnPackingStream.h"
#include "vnUnpackingStream.h"
#include "vnVariableScript.h"

IMPL_SERVER_APP(ConsoleApp)

class Link : public Link_TCP_Connection, public AllocatedObject {
public:
    virtual ~Link() = default;
protected:
    virtual void _init() {
    
    }
    
    virtual void _destroy() {
        delete this;
    }
    
    virtual void _connected() {
        std::cout << "[connected]" << std::endl;
//        m_packing << DataStream::ccu(0);
//        this->send(m_packing.pack());
        std::thread th([this](){
            u32 pid = 0;
            int state = 0;
            str8 txt;
            c8 buf[1024];
            VariableScript vs;
            for (;;) {
                std::cin.getline(buf, sizeof(buf));
                if (state == 0) {
                    if (!strcmp(buf, "exit")) {
                        ServerApp::instance().quit();
                        return ;
                    }
                    if (!strcmp(buf, "send")) {
                        state = 1;
                        std::cout << "[input pid]" << std::endl;
                    }
                } else if (state == 1) {
                    if (!sscanf(buf, "%u", &pid)) {
                        std::cout << "[invalid input]" << std::endl;
                    } else {
                        state = 2;
                        std::cout << "[input content]" << std::endl;
                    }
                } else if (state == 2) {
                    if (*buf) {
                        txt += '\n';
                        txt += buf;
                    } else {
                        state = 0;
                        auto pobj = vs.parse(txt.c_str(), txt.length(), nullptr);
                        txt.clear();
                        if (pobj) {
                            std::cout << "[send packet]" << std::endl;
                            m_packing << DataStream::ccu(pid);
                            pobj->save(m_packing);
                            this->send(m_packing.pack());
                        }
                    }
                }
            }
        });
        th.detach();
    }
    
    virtual void _error_connecting() {
        std::cout << "[connect failed]" << std::endl;
        ServerApp::instance().quit();
    }
    
    virtual void _disconnected() {
        std::cout << "[disconnected]" << std::endl;
        ServerApp::instance().quit();
    }
    
    virtual void _received(LinkBufferPtr buffers) {
        m_unpacking.append(std::move(buffers));
        while (m_unpacking.ready()) {
            u32 pid;
            try {
                m_unpacking >> DataStream::cu(pid);
                Variable_object vobj;
                vobj.load(m_unpacking);
                
                std::cout << "[packet received: " << pid << "]" << std::endl;
                std::cout << VariableScript::generate(&vobj).c_str() << std::endl;
                
            } catch (DataStream::Exception &) {
                
            }
        }
    }

    PackingStream m_packing;
    UnpackingStream m_unpacking;
};


bool ConsoleApp::initialize(int argc, const char *argv[]) {
    if (argc < 2) {
        std::cout << "usage: console ip:port" << std::endl;
        return false;
    }
    NetAddress addr;
    if (!addr.from_string(argv[1])) {
        std::cout << "invalid NetAddress!" << std::endl;
        return false;
    }
    m_pool = LinkPool::create();
    
    std::cout << "[connecting " << addr.to_string().c_str() << "]" << std::endl;
    m_link = m_pool->connect([](){return vnnew Link();}, addr);
    
    return true;
}

void ConsoleApp::finalize() {
    if (m_link) {
        m_link->close();
    }
    if (m_pool) {
        m_pool->release();
    }
}
