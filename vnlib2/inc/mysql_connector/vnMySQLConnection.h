//
//  vnMySQLConnection.h
//  vnlib
//
//  Created by Wayne on 7/7/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef vnlib_vnMySQLConnection_h
#define vnlib_vnMySQLConnection_h

#include "vnAllocatedObject.h"
#include "vnNonCopyable.h"

#include "vnMySQLData.h"

#include <initializer_list>

_vn_begin

class MySQL_Statement;

class MySQL_Connection : public AllocatedObject, public NonCopyable {
public:
    static void initialize();
    static void finalize();
    
    static void thread_init();
    static void thread_end();
    
    MySQL_Connection();
    ~MySQL_Connection();
    
    bool connect(const c8 *host, const c8 *user, const c8 *passwd, const c8 *db);
    bool ping();
    
    MySQL_Statement * createStatement(const c8 *stmt_str, size_t length, std::initializer_list<MySQL_DataType> paramTypes);
    
private:
    void * m_mysql;
};

typedef std::unique_ptr<MySQL_Connection> MySQL_ConnectionPtr;

_vn_end

#endif
