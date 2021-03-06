/*
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _PREPAREDSTATEMENT_H
#define _PREPAREDSTATEMENT_H

#include <future>
#include "SQLOperation.h"

#ifdef __APPLE__
#undef TYPE_BOOL
#endif

//- Union for data buffer (upper-level bind -> queue -> lower-level bind)
union PreparedStatementDataUnion
{
    bool boolean;
    vn::u8 ui8;
    vn::s8 i8;
    vn::u16 ui16;
    vn::s16 i16;
    vn::u32 ui32;
    vn::s32 i32;
    vn::u64 ui64;
    vn::s64 i64;
    float f;
    double d;
};

//- This enum helps us differ data held in above union
enum PreparedStatementValueType
{
    TYPE_BOOL,
    TYPE_UI8,
    TYPE_UI16,
    TYPE_UI32,
    TYPE_UI64,
    TYPE_I8,
    TYPE_I16,
    TYPE_I32,
    TYPE_I64,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_STRING,
    TYPE_BINARY,
    TYPE_NULL
};

struct PreparedStatementData
{
    PreparedStatementDataUnion data;
    PreparedStatementValueType type;
    std::vector<vn::u8> binary;
};

//- Forward declare
class MySQLPreparedStatement;

//- Upper-level class that is used in code
class PreparedStatement
{
    friend class PreparedStatementTask;
    friend class MySQLPreparedStatement;
    friend class MySQLConnection;

    public:
        explicit PreparedStatement(vn::u32 index);
        ~PreparedStatement();

        void setBool(const vn::u32 index, const bool value);
        void setUInt8(const vn::u32 index, const vn::u8 value);
        void setUInt16(const vn::u32 index, const vn::u16 value);
        void setUInt32(const vn::u32 index, const vn::u32 value);
        void setUInt64(const vn::u32 index, const vn::u64 value);
        void setInt8(const vn::u32 index, const vn::s8 value);
        void setInt16(const vn::u32 index, const vn::s16 value);
        void setInt32(const vn::u32 index, const vn::s32 value);
        void setInt64(const vn::u32 index, const vn::s64 value);
        void setFloat(const vn::u32 index, const float value);
        void setDouble(const vn::u32 index, const double value);
        void setString(const vn::u32 index, const std::string& value);
        void setBinary(const vn::u32 index, const std::vector<vn::u8>& value);
        void setNull(const vn::u32 index);

    protected:
        void BindParameters();

    protected:
        MySQLPreparedStatement* m_stmt;
        vn::u32 m_index;
        std::vector<PreparedStatementData> statement_data;    //- Buffer of parameters, not tied to MySQL in any way yet

        PreparedStatement(PreparedStatement const& right) = delete;
        PreparedStatement& operator=(PreparedStatement const& right) = delete;
};

//- Class of which the instances are unique per MySQLConnection
//- access to these class objects is only done when a prepared statement task
//- is executed.
class MySQLPreparedStatement
{
    friend class MySQLConnection;
    friend class PreparedStatement;

    public:
        MySQLPreparedStatement(MYSQL_STMT* stmt);
        ~MySQLPreparedStatement();

        void setBool(const vn::u32 index, const bool value);
        void setUInt8(const vn::u32 index, const vn::u8 value);
        void setUInt16(const vn::u32 index, const vn::u16 value);
        void setUInt32(const vn::u32 index, const vn::u32 value);
        void setUInt64(const vn::u32 index, const vn::u64 value);
        void setInt8(const vn::u32 index, const vn::s8 value);
        void setInt16(const vn::u32 index, const vn::s16 value);
        void setInt32(const vn::u32 index, const vn::s32 value);
        void setInt64(const vn::u32 index, const vn::s64 value);
        void setFloat(const vn::u32 index, const float value);
        void setDouble(const vn::u32 index, const double value);
        void setBinary(const vn::u32 index, const std::vector<vn::u8>& value, bool isString);
        void setNull(const vn::u32 index);

    protected:
        MYSQL_STMT* GetSTMT() { return m_Mstmt; }
        MYSQL_BIND* GetBind() { return m_bind; }
        PreparedStatement* m_stmt;
        void ClearParameters();
        bool CheckValidIndex(vn::u32 index);
        std::string getQueryString(std::string const& sqlPattern) const;

    private:
        void setValue(MYSQL_BIND* param, enum_field_types type, const void* value, vn::u32 len, bool isUnsigned);

    private:
        MYSQL_STMT* m_Mstmt;
        vn::u32 m_paramCount;
        std::vector<bool> m_paramsSet;
        MYSQL_BIND* m_bind;

        MySQLPreparedStatement(MySQLPreparedStatement const& right) = delete;
        MySQLPreparedStatement& operator=(MySQLPreparedStatement const& right) = delete;
};

typedef std::future<PreparedQueryResult> PreparedQueryResultFuture;
typedef std::promise<PreparedQueryResult> PreparedQueryResultPromise;

//- Lower-level class, enqueuable operation
class PreparedStatementTask : public SQLOperation
{
    public:
        PreparedStatementTask(PreparedStatement* stmt, bool async = false);
        ~PreparedStatementTask();

        bool Execute() override;
        PreparedQueryResultFuture GetFuture() { return m_result->get_future(); }

    protected:
        PreparedStatement* m_stmt;
        bool m_has_result;
        PreparedQueryResultPromise* m_result;
};
#endif
