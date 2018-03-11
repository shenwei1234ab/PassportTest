//
//  vnLinkSSL.h
//  vnlib
//
//  Created by Wayne on 6/17/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef vnlib_vnLinkSSL_h
#define vnlib_vnLinkSSL_h

#include "vnLink.h"
#include "vnOpenSSL_BIO.h"

_vn_begin

class Link_TCP_Connection_SSL_Client : public Link_TCP_Connection, OpenSSL_BIO {
public:
    Link_TCP_Connection_SSL_Client();
    ~Link_TCP_Connection_SSL_Client();
    
    void ssl_send(const void *data, size_t size);
    void ssl_send(LinkBufferPtr buffers);
    void ssl_shutdown();
    
    bool ssl_connected() const;
    
    virtual int bio_write(const char *buf, int size);
    
protected:
    virtual void _ssl_connected() {}
    virtual void _ssl_error_connecting(int err) {}
    virtual void _ssl_received(LinkBufferPtr buffer) {}
    virtual void _ssl_disconnected() {}
    
    virtual void _connected();
    virtual void _received(LinkBufferPtr buffers);
    
private:
    bool m_ssl_connected = false;
    void *m_ssl = nullptr;
};

_vn_end

#endif
