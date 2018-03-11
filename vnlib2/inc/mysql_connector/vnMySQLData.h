//
//  vnMySQLDataType.h
//  vnlib
//
//  Created by Wayne on 7/8/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef vnlib_vnMySQLData_h
#define vnlib_vnMySQLData_h

#include "vnAllocatedObject.h"

#include <vector>
#include <memory>

_vn_begin

enum MySQL_DataType {
    kMySQL_null = 0,
    kMySQL_i8,
    kMySQL_u8,
    kMySQL_i16,
    kMySQL_u16,
    kMySQL_i32,
    kMySQL_u32,
    kMySQL_i64,
    kMySQL_u64,
    kMySQL_float,
    kMySQL_double,
    kMySQL_date,
    kMySQL_time,
    kMySQL_datetime,
    kMySQL_timestamp,
    kMySQL_string,
    kMySQL_blob
};

struct MySQL_Date {
    u32 day;
    u32 month;
    u32 year;
    
    bool operator ==(const MySQL_Date &rhs) const {
        return year == rhs.year && month == rhs.month && day == rhs.day;
    }
    
    bool operator !=(const MySQL_Date &rhs) const {
        return year != rhs.year || month != rhs.month || day != rhs.day;
    }
    
    bool operator <(const MySQL_Date &rhs) const {
        if (year < rhs.year) {
            return true;
        } else if (year == rhs.year) {
            if (month < rhs.month) {
                return true;
            } else if (month == rhs.month && day < rhs.day) {
                return true;
            }
        }
        return false;
    }
};

struct MySQL_Time {
    u32 second;
    u32 minite;
    u32 hour;
    
    bool operator ==(const MySQL_Time &rhs) const {
        return hour == rhs.hour && minite == rhs.minite && second == rhs.second;
    }
    
    bool operator !=(const MySQL_Time &rhs) const {
        return hour != rhs.hour || minite != rhs.minite || second != rhs.second;
    }
    
    bool operator <(const MySQL_Time &rhs) const {
        if (hour < rhs.hour) {
            return true;
        } else if (hour == rhs.hour) {
            if (minite < rhs.minite) {
                return true;
            } else if (minite == rhs.minite && second < rhs.second) {
                return true;
            }
        }
        return false;
    }
};

struct MySQL_DateTime : MySQL_Time, MySQL_Date {
    bool operator ==(const MySQL_DateTime &rhs) const {
        return MySQL_Date::operator==(rhs) && MySQL_Time::operator==(rhs);
    }
    
    bool operator !=(const MySQL_DateTime &rhs) const {
        return MySQL_Date::operator!=(rhs) && MySQL_Time::operator!=(rhs);
    }
    
    bool operator <(const MySQL_DateTime &rhs) const {
        if (MySQL_Date::operator<(rhs)) {
            return true;
        } else if (MySQL_Date::operator==(rhs) && MySQL_Time::operator<(rhs)) {
            return true;
        }
        return false;
    }
};

struct MySQL_Exception {};

struct MySQL_Exception_InvalidDataType : MySQL_Exception {};

class MySQL_InputData : public AllocatedObject {
public:
    virtual ~MySQL_InputData() = default;
    virtual MySQL_DataType type() const = 0;
    
    virtual void set_null() = 0;
    virtual void set(u8 val) { throw MySQL_Exception_InvalidDataType(); }
    virtual void set(s8 val) { throw MySQL_Exception_InvalidDataType(); }
    virtual void set(u16 val) { throw MySQL_Exception_InvalidDataType(); }
    virtual void set(s16 val) { throw MySQL_Exception_InvalidDataType(); }
    virtual void set(u32 val) { throw MySQL_Exception_InvalidDataType(); }
    virtual void set(s32 val) { throw MySQL_Exception_InvalidDataType(); }
    virtual void set(u64 val) { throw MySQL_Exception_InvalidDataType(); }
    virtual void set(s64 val) { throw MySQL_Exception_InvalidDataType(); }
    virtual void set(float val) { throw MySQL_Exception_InvalidDataType(); }
    virtual void set(double val) { throw MySQL_Exception_InvalidDataType(); }
    virtual void set(str8 val) { throw MySQL_Exception_InvalidDataType(); }
    virtual void set_external_string(const c8 *ptr, size_t length) { throw MySQL_Exception_InvalidDataType(); }
    virtual void set(std::vector<u8> val) { throw MySQL_Exception_InvalidDataType(); }
    virtual void set_external_blob(const void *ptr, size_t length) { throw MySQL_Exception_InvalidDataType(); }
    virtual void set(const MySQL_Time &val) { throw MySQL_Exception_InvalidDataType(); }
    virtual void set(const MySQL_Date &val) { throw MySQL_Exception_InvalidDataType(); }
    virtual void set(const MySQL_DateTime &val) { throw MySQL_Exception_InvalidDataType(); }
    
};

typedef std::unique_ptr<MySQL_InputData> MySQL_InputDataPtr;

class MySQL_OutputData : public AllocatedObject {
public:
    virtual ~MySQL_OutputData() = default;
    virtual MySQL_DataType type() const = 0;
    
    virtual bool null() const = 0;
    virtual u8 get_u8() const { throw MySQL_Exception_InvalidDataType(); }
    virtual s8 get_i8() const { throw MySQL_Exception_InvalidDataType(); }
    virtual u16 get_u16() const { throw MySQL_Exception_InvalidDataType(); }
    virtual s16 get_i16() const { throw MySQL_Exception_InvalidDataType(); }
    virtual u32 get_u32() const { throw MySQL_Exception_InvalidDataType(); }
    virtual s32 get_i32() const { throw MySQL_Exception_InvalidDataType(); }
    virtual u64 get_u64() const { throw MySQL_Exception_InvalidDataType(); }
    virtual s64 get_i64() const { throw MySQL_Exception_InvalidDataType(); }
    virtual float get_float() const { throw MySQL_Exception_InvalidDataType(); }
    virtual double get_double() const { throw MySQL_Exception_InvalidDataType(); }
    virtual const str8 & get_string() const { throw MySQL_Exception_InvalidDataType(); }
    virtual const std::vector<u8> & get_blob() const { throw MySQL_Exception_InvalidDataType(); }
    virtual const MySQL_Time & get_time() const { throw MySQL_Exception_InvalidDataType(); }
    virtual const MySQL_Date & get_date() const { throw MySQL_Exception_InvalidDataType(); }
    virtual const MySQL_DateTime & get_datetime() const { throw MySQL_Exception_InvalidDataType(); }
};

typedef std::unique_ptr<MySQL_OutputData> MySQL_OutputDataPtr;

_vn_end

#endif
