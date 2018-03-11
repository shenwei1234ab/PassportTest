//
//  vnOpenSSL_Context.h
//  vnlib
//
//  Created by Wayne on 6/18/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __vnlib__vnOpenSSL_Context__
#define __vnlib__vnOpenSSL_Context__

#include "vnSingleton.h"

#include <openssl/ssl.h>

_vn_begin

class OpenSSL_Context : public Singleton<OpenSSL_Context> {
public:
    OpenSSL_Context();
    ~OpenSSL_Context();
    
    SSL * createSSL();
    
private:
    SSL_CTX * m_ctx;
};

_vn_end

#endif /* defined(__vnlib__vnOpenSSL_Context__) */
