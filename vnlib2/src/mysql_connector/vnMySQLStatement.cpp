//
//  vnMySQLStatement.cpp
//  vnlib
//
//  Created by Wayne on 7/7/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "vnMySQLStatement.h"
#include "vnLog.h"
#include "vnMySQLInputData.h"
#include "vnMySQLOutputData.h"

#include <mysql.h>

_vn_begin

MySQL_Statement::~MySQL_Statement() {
    if (m_paramBinds) {
        vnfree(m_paramBinds);
    }
    mysql_stmt_close((MYSQL_STMT *)m_stmt);
}

bool MySQL_Statement::execute(bool errlog) {
    bool bind_param = false;
    size_t index = 0;
    for (auto &i : m_inputs) {
        auto p = static_cast<MySQL_InputDataImpl *>(i.get());
        if (p->dirty()) {
            p->update(static_cast<MYSQL_BIND *>(m_paramBinds)[index]);
            bind_param = true;
        }
        ++index;
    }
    if (bind_param) {
        mysql_stmt_bind_param((static_cast<MYSQL_STMT *>(m_stmt)), static_cast<MYSQL_BIND *>(m_paramBinds));
    }
    if (mysql_stmt_execute((MYSQL_STMT *)m_stmt)) {
        if (errlog) {
            VN_LOG_ERROR("MySQL_Statement::execute() failed: " << mysql_stmt_error((MYSQL_STMT *)m_stmt));
        }
        return false;
    }
    return true;
}

bool MySQL_Statement::fetch() {
    auto ret = mysql_stmt_fetch(static_cast<MYSQL_STMT *>(m_stmt));
    if (ret == 1) {
        VN_LOG_ERROR("MySQL_Statement::execute() failed: " << mysql_stmt_error((MYSQL_STMT *)m_stmt));
        return false;
    } else if (ret == MYSQL_NO_DATA) {
        return false;
    }
    size_t index = 0;
    for (auto &i : m_outputs) {
        static_cast<MySQL_OutputDataImpl *>(i.get())->fetch(static_cast<MYSQL_STMT *>(m_stmt), index++);
    }
    return true;
}

void MySQL_Statement::free_result() {
    mysql_stmt_free_result(static_cast<MYSQL_STMT *>(m_stmt));
}

u64 MySQL_Statement::affected_rows() {
    return mysql_stmt_affected_rows(static_cast<MYSQL_STMT *>(m_stmt));
}

u64 MySQL_Statement::insert_id() {
    return mysql_stmt_insert_id(static_cast<MYSQL_STMT *>(m_stmt));
}

_vn_end