//
//  vnOpenSSL_Context.cpp
//  vnlib
//
//  Created by Wayne on 6/18/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "vnOpenSSL_Context.h"

_vn_begin

OpenSSL_Context::OpenSSL_Context() {
    SSL_library_init();
    m_ctx = SSL_CTX_new(SSLv23_client_method());
}

OpenSSL_Context::~OpenSSL_Context() {
    SSL_CTX_free(m_ctx);
}

SSL * OpenSSL_Context::createSSL() {
    return SSL_new(m_ctx);
}

_vn_end