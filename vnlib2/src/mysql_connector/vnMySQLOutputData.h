//
//  vnMySQLOutputData.h
//  vnlib
//
//  Created by Wayne on 7/10/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __vnlib__vnMySQLOutputData__
#define __vnlib__vnMySQLOutputData__

#include "vnMySQLData.h"

#include <mysql.h>

_vn_begin

class MySQL_OutputDataImpl : public MySQL_OutputData {
public:
    virtual void init(MYSQL_BIND &bind) = 0;
    virtual void fetch(MYSQL_STMT *stmt, size_t index) {}
    
    virtual bool null() const { return m_null; }
    
    static MySQL_OutputDataImpl * create(enum_field_types type, unsigned int flags);
protected:
    my_bool m_null = 0;
    size_t m_length = 0;
};

template <typename T>
class MySQL_OutputData_Number : public MySQL_OutputDataImpl {
public:
    
    void _init(MYSQL_BIND &bind) {
        bind.is_null = &m_null;
        bind.length = &m_length;
        bind.buffer = &m_value;
    }
    
    virtual u8 get_u8() const {
        return (u8)m_value;
    }
    virtual s8 get_i8() const {
        return (s8)m_value;
    }
    virtual u16 get_u16() const {
        return (u16)m_value;
    }
    virtual s16 get_i16() const {
        return (s16)m_value;
    }
    virtual u32 get_u32() const {
        return (u32)m_value;
    }
    virtual s32 get_i32() const {
        return (s32)m_value;
    }
    virtual u64 get_u64() const {
        return (u64)m_value;
    }
    virtual s64 get_i64() const {
        return (s64)m_value;
    }
    virtual float get_float() const {
        return (float)m_value;
    }
    virtual double get_double() const {
        return (double)m_value;
    }
    
protected:
    T m_value;
};

class MySQL_OutputData_i8 : public MySQL_OutputData_Number<s8> {
public:
    virtual MySQL_DataType type() const { return kMySQL_i8; }
    
    virtual void init(MYSQL_BIND &bind) {
        this->_init(bind);
        bind.buffer_type = MYSQL_TYPE_TINY;
    }
};

class MySQL_OutputData_u8 : public MySQL_OutputData_Number<u8> {
public:
    virtual MySQL_DataType type() const { return kMySQL_u8; }
    
    virtual void init(MYSQL_BIND &bind) {
        this->_init(bind);
        bind.buffer_type = MYSQL_TYPE_TINY;
        bind.is_unsigned = 1;
    }
};

class MySQL_OutputData_i16 : public MySQL_OutputData_Number<s16> {
public:
    virtual MySQL_DataType type() const { return kMySQL_i16; }
    
    virtual void init(MYSQL_BIND &bind) {
        this->_init(bind);
        bind.buffer_type = MYSQL_TYPE_SHORT;
    }
};

class MySQL_OutputData_u16 : public MySQL_OutputData_Number<u16> {
public:
    virtual MySQL_DataType type() const { return kMySQL_u16; }
    
    virtual void init(MYSQL_BIND &bind) {
        this->_init(bind);
        bind.buffer_type = MYSQL_TYPE_SHORT;
        bind.is_unsigned = 1;
    }
};

class MySQL_OutputData_i32 : public MySQL_OutputData_Number<s32> {
public:
    virtual MySQL_DataType type() const { return kMySQL_i32; }
    
    virtual void init(MYSQL_BIND &bind) {
        this->_init(bind);
        bind.buffer_type = MYSQL_TYPE_LONG;
    }
};

class MySQL_OutputData_u32 : public MySQL_OutputData_Number<u32> {
public:
    virtual MySQL_DataType type() const { return kMySQL_u32; }
    
    virtual void init(MYSQL_BIND &bind) {
        this->_init(bind);
        bind.buffer_type = MYSQL_TYPE_LONG;
        bind.is_unsigned = 1;
    }
};

class MySQL_OutputData_i64 : public MySQL_OutputData_Number<s64> {
public:
    virtual MySQL_DataType type() const { return kMySQL_i64; }
    
    virtual void init(MYSQL_BIND &bind) {
        this->_init(bind);
        bind.buffer_type = MYSQL_TYPE_LONGLONG;
    }
};

class MySQL_OutputData_u64 : public MySQL_OutputData_Number<u64> {
public:
    virtual MySQL_DataType type() const { return kMySQL_u64; }
    
    virtual void init(MYSQL_BIND &bind) {
        this->_init(bind);
        bind.buffer_type = MYSQL_TYPE_LONGLONG;
        bind.is_unsigned = 1;
    }
};

class MySQL_OutputData_float : public MySQL_OutputData_Number<float> {
public:
    virtual MySQL_DataType type() const { return kMySQL_float; }
    
    virtual void init(MYSQL_BIND &bind) {
        this->_init(bind);
        bind.buffer_type = MYSQL_TYPE_FLOAT;
    }
};

class MySQL_OutputData_double : public MySQL_OutputData_Number<double> {
public:
    virtual MySQL_DataType type() const { return kMySQL_double; }
    
    virtual void init(MYSQL_BIND &bind) {
        this->_init(bind);
        bind.buffer_type = MYSQL_TYPE_DOUBLE;
    }
};

template <typename T>
class MySQL_OutputData_DateTime : public MySQL_OutputDataImpl {
public:
    void _init(MYSQL_BIND &bind) {
        bind.is_null = &m_null;
        bind.buffer = &m_time;
        bind.length = &m_length;
    }
    
protected:
    MYSQL_TIME m_time;
    T m_value;
};

class MySQL_OutputData_datetime : public MySQL_OutputData_DateTime<MySQL_DateTime> {
public:
    virtual MySQL_DataType type() const { return kMySQL_datetime; }
    
    virtual void init(MYSQL_BIND &bind) {
        this->_init(bind);
        bind.buffer_type = MYSQL_TYPE_DATETIME;
    }
    
    virtual void fetch(MYSQL_STMT *stmt, size_t index) {
        m_value.year = m_time.year;
        m_value.month = m_time.month;
        m_value.day = m_time.day;
        m_value.hour = m_time.hour;
        m_value.minite = m_time.minute;
        m_value.second = m_time.second;
    }
    
    virtual const MySQL_Time & get_time() const {
        return m_value;
    }
    
    virtual const MySQL_Date & get_date() const {
        return m_value;
    }
    
    virtual const MySQL_DateTime & get_datetime() const {
        return m_value;
    }
};

class MySQL_OutputData_date : public MySQL_OutputData_DateTime<MySQL_Date> {
public:
    virtual MySQL_DataType type() const { return kMySQL_date; }
    
    virtual void init(MYSQL_BIND &bind) {
        this->_init(bind);
        bind.buffer_type = MYSQL_TYPE_DATETIME;
    }
    
    virtual void fetch(MYSQL_STMT *stmt, size_t index) {
        m_value.year = m_time.year;
        m_value.month = m_time.month;
        m_value.day = m_time.day;
    }
    
    virtual const MySQL_Date & get_date() const {
        return m_value;
    }
};

class MySQL_OutputData_time : public MySQL_OutputData_DateTime<MySQL_Time> {
public:
    virtual MySQL_DataType type() const { return kMySQL_time; }
    
    virtual void init(MYSQL_BIND &bind) {
        this->_init(bind);
        bind.buffer_type = MYSQL_TYPE_DATETIME;
    }
    
    virtual void fetch(MYSQL_STMT *stmt, size_t index) {
        m_value.hour = m_time.hour;
        m_value.minite = m_time.minute;
        m_value.second = m_time.second;
    }
    
    virtual const MySQL_Time & get_time() const {
        return m_value;
    }
};

class MySQL_OutputData_timestamp : public MySQL_OutputData_DateTime<MySQL_DateTime> {
public:
    virtual MySQL_DataType type() const { return kMySQL_timestamp; }
    
    virtual void init(MYSQL_BIND &bind) {
        this->_init(bind);
        bind.buffer_type = MYSQL_TYPE_DATETIME;
    }
    
    virtual void fetch(MYSQL_STMT *stmt, size_t index) {
        m_value.year = m_time.year;
        m_value.month = m_time.month;
        m_value.day = m_time.day;
        m_value.hour = m_time.hour;
        m_value.minite = m_time.minute;
        m_value.second = m_time.second;
    }
    
    virtual const MySQL_Time & get_time() const {
        return m_value;
    }
    
    virtual const MySQL_Date & get_date() const {
        return m_value;
    }
    
    virtual const MySQL_DateTime & get_datetime() const {
        return m_value;
    }
};

class MySQL_OutputData_string : public MySQL_OutputDataImpl {
public:
    virtual MySQL_DataType type() const { return kMySQL_string; }
    
    virtual void init(MYSQL_BIND &bind) {
        bind.is_null = &m_null;
        bind.length = &m_length;
        bind.buffer = 0;
        bind.buffer_length = 0;
        bind.buffer_type = MYSQL_TYPE_STRING;
    }
    
    virtual void fetch(MYSQL_STMT *stmt, size_t index);
    
    virtual const str8 & get_string() const { return m_value; }
    
protected:
    str8 m_value;
};

class MySQL_OutputData_blob : public MySQL_OutputDataImpl {
public:
    virtual MySQL_DataType type() const { return kMySQL_blob; }
    
    virtual void init(MYSQL_BIND &bind) {
        bind.is_null = &m_null;
        bind.length = &m_length;
        bind.buffer = 0;
        bind.buffer_length = 0;
        bind.buffer_type = MYSQL_TYPE_BLOB;
    }
    
    virtual void fetch(MYSQL_STMT *stmt, size_t index);
    
    virtual const std::vector<u8> & get_blob() const { return m_value; }
    
protected:
    std::vector<u8> m_value;
};

_vn_end

#endif /* defined(__vnlib__vnMySQLOutputData__) */
