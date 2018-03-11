//
//  TableBase.h
//  tblcoder
//
//  Created by Wayne on 14-2-8.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __tblcoder__TableBase__
#define __tblcoder__TableBase__

#include "vnDataStream.h"
#include <vector>
#include <string.h>
namespace tbl {
    
    using namespace vn;
    
    enum FieldType {
        kFT_Invalid = 0,
        kFT_String = 1,
        kFT_Int = 2,
        kFT_UInt = 3,
        kFT_Float = 4

    };
    
#pragma pack(push, 1)
    
    union FieldFormat {
        struct {
            u8 type : 7;
            u8 key : 1;
        };
        u8 value;
    };
    
#pragma pack(pop)
    
    
    
    const u32 kTableFileHeader = (u32)('T') | ((u32)('B') << 8) | ((u32)('L') << 16);
    
    typedef std::vector<FieldFormat> RecordFormat;
    
    DataStream & operator <<(DataStream &stream, const RecordFormat &format);
    DataStream & operator >>(DataStream &stream, RecordFormat &format);
    
    
    
    struct Field {
        bool null;
    };
    
    struct Field_string : Field {
        str8 value;
		enum { type = kFT_String };
        int compare(const Field_string &other) const;
		int compare(const str8 &_value) const;
    };
    
    struct Field_int : Field {
        s32 value;
		enum { type = kFT_Int };
        int compare(const Field_int &other) const;
		int compare(s32 _value) const;
    };
    
    struct Field_uint : Field {
		enum { type = kFT_UInt };
        u32 value;
        int compare(const Field_uint &other) const;
		int compare(u32 _value) const;
    };
    
    struct Field_float : Field {
        f32 value;
		enum { type = kFT_Float };
        int compare(const Field_float &other) const;
		int compare(f32 _value) const;
    };
    
    DataStream & operator >>(DataStream &stream, Field_string &field);
    DataStream & operator >>(DataStream &stream, Field_int &field);
    DataStream & operator >>(DataStream &stream, Field_uint &field);
    DataStream & operator >>(DataStream &stream, Field_float &field);

	DataStream & operator <<(DataStream &stream, const Field_string &field);
	DataStream & operator <<(DataStream &stream, const Field_int &field);
	DataStream & operator <<(DataStream &stream, const Field_uint &field);
	DataStream & operator <<(DataStream &stream, const Field_float &field);
    
    
    u32 _read_z_u32(DataStream &stream);
    void _write_z_u32(DataStream &stream, u32 value);
    void _read_str8(DataStream &stream, str8 &value);
    void _write_str8(DataStream &stream, const str8 &value);
    template <typename T>
    void _read_z_u32(DataStream &stream, T &value) {
        value = (T)_read_z_u32(stream);
    }
    
}


#endif /* defined(__tblcoder__TableBase__) */
