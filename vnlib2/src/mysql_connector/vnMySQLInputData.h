//
//  vnMySQLDataImpl.h
//  vnlib
//
//  Created by Wayne on 7/10/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __vnlib__vnMySQLInputData__
#define __vnlib__vnMySQLInputData__

#include "vnMySQLData.h"

#include <cstring>

#include <mysql.h>

_vn_begin

class MySQL_InputDataImpl : public MySQL_InputData {
public:
    virtual void init(MYSQL_BIND &bind) = 0;
    virtual void update(MYSQL_BIND &bind) = 0;
    
    bool dirty() const { return m_dirty; }
    
    static MySQL_InputDataImpl * create(MySQL_DataType type);
protected:
    bool m_dirty = false;
};

class MySQL_InputData_null : public MySQL_InputDataImpl {
public:
    virtual MySQL_DataType type() const { return kMySQL_null; }
    
    virtual void init(MYSQL_BIND &bind) {
        bind.buffer_type = MYSQL_TYPE_NULL;
    }
    
    void update(MYSQL_BIND &bind) {}
    
    virtual void set_null() {}
};

template <typename T>
class MySQL_InputData_Number : public MySQL_InputDataImpl {
public:
    void _init(MYSQL_BIND &bind) {
        bind.is_null = &m_null;
        bind.buffer = &m_value;
    }
    
    void update(MYSQL_BIND &bind) {}
    
    virtual void set_null() { m_null = 1; }
    virtual void set(u8 val) {
        m_null = 0;
        m_value = (T)val;
    }
    virtual void set(s8 val) {
        m_null = 0;
        m_value = (T)val;
    }
    virtual void set(u16 val) {
        m_null = 0;
        m_value = (T)val;
    }
    virtual void set(s16 val) {
        m_null = 0;
        m_value = (T)val;
    }
    virtual void set(u32 val) {
        m_null = 0;
        m_value = (T)val;
    }
    virtual void set(s32 val) {
        m_null = 0;
        m_value = (T)val;
    }
    virtual void set(u64 val) {
        m_null = 0;
        m_value = (T)val;
    }
    virtual void set(s64 val) {
        m_null = 0;
        m_value = (T)val;
    }
    virtual void set(float val) {
        m_null = 0;
        m_value = (T)val;
    }
    virtual void set(double val) {
        m_null = 0;
        m_value = (T)val;
    }
protected:
    T m_value = 0;
    my_bool m_null = 1;
};

class MySQL_InputData_i8 : public MySQL_InputData_Number<s8> {
public:
    virtual MySQL_DataType type() const { return kMySQL_i8; }
    
    virtual void init(MYSQL_BIND &bind) {
        this->_init(bind);
        bind.buffer_type = MYSQL_TYPE_TINY;
    }
};

class MySQL_InputData_u8 : public MySQL_InputData_Number<u8> {
public:
    virtual MySQL_DataType type() const { return kMySQL_u8; }
    
    virtual void init(MYSQL_BIND &bind) {
        this->_init(bind);
        bind.buffer_type = MYSQL_TYPE_TINY;
        bind.is_unsigned = 1;
    }
};

class MySQL_InputData_i16 : public MySQL_InputData_Number<s16> {
public:
    virtual MySQL_DataType type() const { return kMySQL_i16; }
    
    virtual void init(MYSQL_BIND &bind) {
        this->_init(bind);
        bind.buffer_type = MYSQL_TYPE_SHORT;
    }
};

class MySQL_InputData_u16 : public MySQL_InputData_Number<u16> {
public:
    virtual MySQL_DataType type() const { return kMySQL_u16; }
    
    virtual void init(MYSQL_BIND &bind) {
        this->_init(bind);
        bind.buffer_type = MYSQL_TYPE_SHORT;
        bind.is_unsigned = 1;
    }
};

class MySQL_InputData_i32 : public MySQL_InputData_Number<s32> {
public:
    virtual MySQL_DataType type() const { return kMySQL_i32; }
    
    virtual void init(MYSQL_BIND &bind) {
        this->_init(bind);
        bind.buffer_type = MYSQL_TYPE_LONG;
    }
};

class MySQL_InputData_u32 : public MySQL_InputData_Number<u32> {
public:
    virtual MySQL_DataType type() const { return kMySQL_u32; }
    
    virtual void init(MYSQL_BIND &bind) {
        this->_init(bind);
        bind.buffer_type = MYSQL_TYPE_LONG;
        bind.is_unsigned = 1;
    }
};

class MySQL_InputData_i64 : public MySQL_InputData_Number<s64> {
public:
    virtual MySQL_DataType type() const { return kMySQL_i64; }
    
    virtual void init(MYSQL_BIND &bind) {
        this->_init(bind);
        bind.buffer_type = MYSQL_TYPE_LONGLONG;
    }
};

class MySQL_InputData_u64 : public MySQL_InputData_Number<u64> {
public:
    virtual MySQL_DataType type() const { return kMySQL_u64; }
    
    virtual void init(MYSQL_BIND &bind) {
        this->_init(bind);
        bind.buffer_type = MYSQL_TYPE_LONGLONG;
        bind.is_unsigned = 1;
    }
};

class MySQL_InputData_float : public MySQL_InputData_Number<float> {
public:
    virtual MySQL_DataType type() const { return kMySQL_float; }
    
    virtual void init(MYSQL_BIND &bind) {
        this->_init(bind);
        bind.buffer_type = MYSQL_TYPE_FLOAT;
    }
};

class MySQL_InputData_double : public MySQL_InputData_Number<double> {
public:
    virtual MySQL_DataType type() const { return kMySQL_double; }
    
    virtual void init(MYSQL_BIND &bind) {
        this->_init(bind);
        bind.buffer_type = MYSQL_TYPE_DOUBLE;
    }
};

class MySQL_InputData_string : public MySQL_InputDataImpl {
public:
    virtual MySQL_DataType type() const { return kMySQL_string; }
    
    virtual void init(MYSQL_BIND &bind) {
        bind.buffer_type = MYSQL_TYPE_STRING;
        bind.buffer = const_cast<c8 *>(m_value.c_str());
        bind.is_null = &m_null;
    }
    
    virtual void update(MYSQL_BIND &bind) {
        m_dirty = false;
        if (m_external) {
            bind.buffer = const_cast<c8 *>(m_external_ptr);
            bind.buffer_length = m_external_length;
        } else {
            bind.buffer = const_cast<c8 *>(m_value.c_str());
            bind.buffer_length = m_value.length();
        }
    }
    
    virtual void set_null() { m_null = 1; }
    
    virtual void set(str8 val) {
        m_dirty = true;
        m_null = 0;
        m_external = false;
        m_value = std::move(val);
    }
    
    virtual void set_external_string(const c8 *ptr, size_t length) {
        m_dirty = true;
        m_null = 0;
        m_external = true;
        m_external_ptr = ptr;
        m_external_length = length;
    }
    
protected:
    str8 m_value;
    const c8 *m_external_ptr = 0;
    size_t m_external_length = 0;
    bool m_external = false;
    my_bool m_null = 1;
};

class MySQL_InputData_blob : public MySQL_InputDataImpl {
public:
    virtual MySQL_DataType type() const { return kMySQL_blob; }
    
    virtual void init(MYSQL_BIND &bind) {
        bind.buffer_type = MYSQL_TYPE_BLOB;
        bind.is_null = &m_null;
    }
    
    virtual void update(MYSQL_BIND &bind) {
        m_dirty = false;
        if (m_external) {
            bind.buffer = const_cast<void *>(m_external_ptr);
            bind.buffer_length = m_external_length;
        } else {
            bind.buffer = m_value.data();
            bind.buffer_length = m_value.size();
        }
    }
    
    virtual void set_null() { m_null = 1; }
    
    virtual void set(str8 val) {
        m_dirty = true;
        m_null = 0;
        m_external = false;
        m_value.reserve(val.length());
        m_value.assign(val.begin(), val.end());
    }
    
    virtual void set_external_string(const c8 *ptr, size_t length) {
        m_dirty = true;
        m_null = 0;
        m_external = true;
        m_external_ptr = ptr;
        m_external_length = length;
    }
    
    virtual void set(std::vector<u8> val) {
        m_dirty = true;
        m_null = 0;
        m_external = false;
        m_value = std::move(val);
    }
    
    virtual void set_external_blob(const void *ptr, size_t length) {
        m_dirty = true;
        m_null = 0;
        m_external = true;
        m_external_ptr = ptr;
        m_external_length = length;
    }
    
protected:
    std::vector<u8> m_value;
    const void *m_external_ptr = 0;
    size_t m_external_length = 0;
    bool m_external = false;
    my_bool m_null = 1;
};

class MySQL_InputData_datetime : public MySQL_InputDataImpl {
public:
    MySQL_InputData_datetime() {
        memset(&m_value, 0, sizeof(MYSQL_TIME));
    }
    
    void _init(MYSQL_BIND &bind) {
        bind.buffer = &m_value;
        bind.is_null = &m_null;
    }
    
    virtual MySQL_DataType type() const { return kMySQL_datetime; }
    
    virtual void init(MYSQL_BIND &bind) {
        this->_init(bind);
        bind.buffer_type = MYSQL_TYPE_DATETIME;
    }
    
    virtual void update(MYSQL_BIND &bind) {}
    
    virtual void set_null() { m_null = 1; }
    
    virtual void set(const MySQL_Time &val) {
        m_null = 0;
        m_value.hour = val.hour;
        m_value.minute = val.minite;
        m_value.second = val.second;
    }
    
    virtual void set(const MySQL_Date &val) {
        m_null = 0;
        m_value.year = val.year;
        m_value.month = val.year;
        m_value.day = val.day;
    }
    
    virtual void set(const MySQL_DateTime &val) {
        m_null = 0;
        m_value.year = val.year;
        m_value.month = val.year;
        m_value.day = val.day;
        m_value.hour = val.hour;
        m_value.minute = val.minite;
        m_value.second = val.second;
    }
    
protected:
    my_bool m_null = 1;
    MYSQL_TIME m_value;
};

class MySQL_InputData_date : public MySQL_InputData_datetime {
public:
    virtual MySQL_DataType type() const { return kMySQL_date; }
    
    virtual void init(MYSQL_BIND &bind) {
        this->_init(bind);
        bind.buffer_type = MYSQL_TYPE_DATE;
    }
};

class MySQL_InputData_time : public MySQL_InputData_datetime {
public:
    virtual MySQL_DataType type() const { return kMySQL_time; }
    
    virtual void init(MYSQL_BIND &bind) {
        this->_init(bind);
        bind.buffer_type = MYSQL_TYPE_TIME;
    }
};

class MySQL_InputData_timestamp : public MySQL_InputData_datetime {
public:
    virtual MySQL_DataType type() const { return kMySQL_timestamp; }
    
    virtual void init(MYSQL_BIND &bind) {
        this->_init(bind);
        bind.buffer_type = MYSQL_TYPE_TIMESTAMP;
    }
};


_vn_end

#endif /* defined(__vnlib__vnMySQLDataImpl__) */
