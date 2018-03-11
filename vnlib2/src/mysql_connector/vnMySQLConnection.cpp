//
//  vnMySQLConnection.cpp
//  vnlib
//
//  Created by Wayne on 7/7/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "vnMySQLConnection.h"
#include "vnLog.h"

#include "vnMySQLStatement.h"
#include "vnMySQLInputData.h"
#include "vnMySQLOutputData.h"

#include <mysql.h>

_vn_begin

void MySQL_Connection::initialize() {
    mysql_library_init(0, 0, 0);
}

void MySQL_Connection::finalize() {
    mysql_library_end();
}

void MySQL_Connection::thread_init() {
    mysql_thread_init();
}

void MySQL_Connection::thread_end() {
    mysql_thread_end();
}

MySQL_Connection::MySQL_Connection() {
    m_mysql = mysql_init(0);
}

MySQL_Connection::~MySQL_Connection() {
    mysql_close((MYSQL *)m_mysql);
}

bool MySQL_Connection::connect(const c8 *host, const c8 *user, const c8 *passwd, const c8 *db) {
    my_bool flag = 0;
    mysql_options((MYSQL *)m_mysql, MYSQL_REPORT_DATA_TRUNCATION, &flag);
    
    if (!mysql_real_connect((MYSQL *)m_mysql, host, user, passwd, db, 0, 0, 0)) {
        VN_LOG_ERROR("MySQLConnection::connect() failed: " << mysql_error((MYSQL *)m_mysql));
        return false;
    }

    return true;
}

bool MySQL_Connection::ping() {
    return mysql_ping((MYSQL *)m_mysql) == 0;
}

MySQL_Statement * MySQL_Connection::createStatement(const c8 *stmt_str, size_t length, std::initializer_list<MySQL_DataType> paramTypes) {
    auto stmt = mysql_stmt_init((MYSQL *)m_mysql);
    if (!stmt) {
        VN_LOG_ERROR("MySQLConnection::createStatement() init failed: " << mysql_error((MYSQL *)m_mysql));
        return nullptr;
    }
    if (mysql_stmt_prepare(stmt, stmt_str, length)) {
        VN_LOG_ERROR("MySQLConnection::createStatement() prepare failed: " << mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return nullptr;
    }
    
    auto paramCount = mysql_stmt_param_count(stmt);
    if (paramCount != paramTypes.size()) {
        VN_LOG_ERROR("MySQLConnection::createStatement() failed: paramTypes.size() != " << paramCount);
        mysql_stmt_close(stmt);
        return nullptr;
    }
    MYSQL_BIND *paramBinds = 0;
    std::vector<MySQL_InputDataPtr> inputs;
    if (paramCount) {
        auto total = paramCount * sizeof(MYSQL_BIND);
        paramBinds = (MYSQL_BIND *)Memory::allocate(total, __FILE__, __LINE__, 0);
        memset(paramBinds, 0, total);
        inputs.resize(paramCount);
        size_t i = 0;
        for (auto it = paramTypes.begin(); it != paramTypes.end(); ++it, ++i) {
            auto p = MySQL_InputDataImpl::create(*it);
            if (!p) {
                VN_LOG_ERROR("MySQLConnection::createStatement() failed: invalid param type " << (u32)*it);
                Memory::free(paramBinds);
                mysql_stmt_close(stmt);
                return nullptr;
            }
            inputs[i].reset(p);
            p->init(paramBinds[i]);
        }
        if (mysql_stmt_bind_param(stmt, paramBinds)) {
            VN_LOG_ERROR("MySQLConnection::createStatement() bind_param failed: " << mysql_stmt_error(stmt));
            Memory::free(paramBinds);
            mysql_stmt_close(stmt);
            return nullptr;
        }
    }
    
    std::vector<MySQL_OutputDataPtr> outputs;
    
    auto rs = mysql_stmt_result_metadata(stmt);
    if (rs) {
        auto numFields = mysql_num_fields(rs);
        if (numFields) {
            auto fields = mysql_fetch_fields(rs);
            
            auto total = numFields * sizeof(MYSQL_BIND);
            auto fieldBinds = (MYSQL_BIND *)Memory::allocate(total, __FILE__, __LINE__, 0);
            memset(fieldBinds, 0, total);
            
            outputs.resize(numFields);
            for (unsigned int i = 0; i < numFields; ++i) {
                auto p = MySQL_OutputDataImpl::create(fields[i].type, fields[i].flags);
                if (!p) {
                    VN_LOG_ERROR("MySQLConnection::createStatement() failed: can not create output data");
                    Memory::free(fieldBinds);
                    if (paramBinds) {
                        Memory::free(paramBinds);
                    }
                    mysql_free_result(rs);
                    mysql_stmt_close(stmt);
                    return nullptr;
                }
                p->init(fieldBinds[i]);
                outputs[i].reset(p);
            }
            
            if (mysql_stmt_bind_result(stmt, fieldBinds)) {
                VN_LOG_ERROR("MySQLConnection::createStatement() bind_result failed: " << mysql_stmt_error(stmt));
                Memory::free(fieldBinds);
                if (paramBinds) {
                    Memory::free(paramBinds);
                }
                mysql_free_result(rs);
                mysql_stmt_close(stmt);
                return nullptr;
            }
            Memory::free(fieldBinds);
        }
        
        mysql_free_result(rs);
    }
    
    
    auto ret = vnnew MySQL_Statement();
    ret->m_stmt = stmt;
    ret->m_paramBinds = paramBinds;
    ret->m_inputs = std::move(inputs);
    ret->m_outputs = std::move(outputs);
    
    return ret;
}



_vn_end
