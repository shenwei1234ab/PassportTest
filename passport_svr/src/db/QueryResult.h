/*
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#ifndef QUERYRESULT_H
#define QUERYRESULT_H

#include <memory>
#include "Field.h"
#include <mysql.h>

class ResultSet
{
    public:
        ResultSet(MYSQL_RES* result, MYSQL_FIELD* fields, vn::u64 rowCount, vn::u32 fieldCount);
        ~ResultSet();

        bool NextRow();
        vn::u64 GetRowCount() const { return _rowCount; }
        vn::u32 GetFieldCount() const { return _fieldCount; }

        Field* Fetch() const { return _currentRow; }
        const Field & operator [] (vn::u32 index) const
        {
            assert(index < _fieldCount);
            return _currentRow[index];
        }

    protected:
        vn::u64 _rowCount;
        Field* _currentRow;
        vn::u32 _fieldCount;

    private:
        void CleanUp();
        MYSQL_RES* _result;
        MYSQL_FIELD* _fields;

        ResultSet(ResultSet const& right) = delete;
        ResultSet& operator=(ResultSet const& right) = delete;
};

typedef std::shared_ptr<ResultSet> QueryResult;

class PreparedResultSet
{
    public:
        PreparedResultSet(MYSQL_STMT* stmt, MYSQL_RES* result, vn::u64 rowCount, vn::u32 fieldCount);
        ~PreparedResultSet();

        bool NextRow();
        vn::u64 GetRowCount() const { return m_rowCount; }
        vn::u32 GetFieldCount() const { return m_fieldCount; }

        Field* Fetch() const
        {
            assert(m_rowPosition < m_rowCount);
            return m_rows[vn::u32(m_rowPosition)];
        }

        const Field & operator [] (vn::u32 index) const
        {
            assert(m_rowPosition < m_rowCount);
            assert(index < m_fieldCount);
            return m_rows[vn::u32(m_rowPosition)][index];
        }

    protected:
        std::vector<Field*> m_rows;
        vn::u64 m_rowCount;
        vn::u64 m_rowPosition;
        vn::u32 m_fieldCount;

    private:
        MYSQL_BIND* m_rBind;
        MYSQL_STMT* m_stmt;
        MYSQL_RES* m_res;

        my_bool* m_isNull;
        unsigned long* m_length;

        void FreeBindBuffer();
        void CleanUp();
        bool _NextRow();

        PreparedResultSet(PreparedResultSet const& right) = delete;
        PreparedResultSet& operator=(PreparedResultSet const& right) = delete;
};

typedef std::shared_ptr<PreparedResultSet> PreparedQueryResult;

#endif

