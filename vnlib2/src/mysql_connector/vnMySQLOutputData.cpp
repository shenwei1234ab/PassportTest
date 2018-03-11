//
//  vnMySQLOutputData.cpp
//  vnlib
//
//  Created by Wayne on 7/10/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "vnMySQLOutputData.h"

#include <cstring>

_vn_begin

void MySQL_OutputData_string::fetch(MYSQL_STMT *stmt, size_t index) {
    if (m_null || !m_length) {
        m_value.clear();
        return ;
    }
    m_value.resize(m_length);
    MYSQL_BIND bnd;
    memset(&bnd, 0, sizeof(bnd));
    bnd.buffer_type = MYSQL_TYPE_STRING;
    bnd.buffer = const_cast<c8 *>(m_value.data());
    bnd.buffer_length = m_length;
    bnd.length = &m_length;
    bnd.is_null = &m_null;
    if (mysql_stmt_fetch_column(stmt, &bnd, (unsigned int)index, 0)) {
        m_value.clear();
    }
}

void MySQL_OutputData_blob::fetch(MYSQL_STMT *stmt, size_t index) {
    if (m_null || !m_length) {
        m_value.clear();
        return ;
    }
    m_value.resize(m_length);
    MYSQL_BIND bnd;
    memset(&bnd, 0, sizeof(bnd));
    bnd.buffer_type = MYSQL_TYPE_BLOB;
    bnd.buffer = m_value.data();
    bnd.buffer_length = m_length;
    bnd.length = &m_length;
    bnd.is_null = &m_null;
    if (mysql_stmt_fetch_column(stmt, &bnd, (unsigned int)index, 0)) {
        m_value.clear();
    }
}

MySQL_OutputDataImpl * MySQL_OutputDataImpl::create(enum_field_types type, unsigned int flags) {
    switch (type) {
        case MYSQL_TYPE_TINY:
            if (flags & UNSIGNED_FLAG) {
                return vnnew MySQL_OutputData_u8();
            }
            return vnnew MySQL_OutputData_i8();
            
        case MYSQL_TYPE_SHORT:
            if (flags & UNSIGNED_FLAG) {
                return vnnew MySQL_OutputData_u16();
            }
            return vnnew MySQL_OutputData_i16();
            
        case MYSQL_TYPE_INT24:
        case MYSQL_TYPE_LONG:
            if (flags & UNSIGNED_FLAG) {
                return vnnew MySQL_OutputData_u32();
            }
            return vnnew MySQL_OutputData_i32();
            
        case MYSQL_TYPE_LONGLONG:
            if (flags & UNSIGNED_FLAG) {
                return vnnew MySQL_OutputData_u64();
            }
            return vnnew MySQL_OutputData_i64();
            
        case MYSQL_TYPE_FLOAT:
            return vnnew MySQL_OutputData_float();
            
        case MYSQL_TYPE_DOUBLE:
            return vnnew MySQL_OutputData_double();
            
        case MYSQL_TYPE_TIME:
            return vnnew MySQL_OutputData_time();
            
        case MYSQL_TYPE_DATE:
            return vnnew MySQL_OutputData_date();
            
        case MYSQL_TYPE_DATETIME:
            return vnnew MySQL_OutputData_datetime();
            
        case MYSQL_TYPE_TIMESTAMP:
            return vnnew MySQL_OutputData_timestamp();
            
        case MYSQL_TYPE_NEWDECIMAL:
        case MYSQL_TYPE_STRING:
        case MYSQL_TYPE_VAR_STRING:
            return vnnew MySQL_OutputData_string();
            
        case MYSQL_TYPE_TINY_BLOB:
        case MYSQL_TYPE_BLOB:
        case MYSQL_TYPE_MEDIUM_BLOB:
        case MYSQL_TYPE_LONG_BLOB:
        case MYSQL_TYPE_BIT:
            return vnnew MySQL_OutputData_blob();
            
        default:
            break;
    }
    return nullptr;
}

_vn_end