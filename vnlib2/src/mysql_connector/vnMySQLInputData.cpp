//
//  vnMySQLInputData.cpp
//  vnlib
//
//  Created by Wayne on 7/10/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "vnMySQLInputData.h"

_vn_begin

MySQL_InputDataImpl * MySQL_InputDataImpl::create(MySQL_DataType type) {
    switch (type) {
        case kMySQL_null:
            return vnnew MySQL_InputData_null();
            
        case kMySQL_i8:
            return vnnew MySQL_InputData_i8();

        case kMySQL_u8:
            return vnnew MySQL_InputData_u8();
            
        case kMySQL_i16:
            return vnnew MySQL_InputData_i16();
            
        case kMySQL_u16:
            return vnnew MySQL_InputData_u16();
            
        case kMySQL_i32:
            return vnnew MySQL_InputData_i32();
            
        case kMySQL_u32:
            return vnnew MySQL_InputData_u32();
            
        case kMySQL_i64:
            return vnnew MySQL_InputData_i64();
            
        case kMySQL_u64:
            return vnnew MySQL_InputData_u64();
            
        case kMySQL_float:
            return vnnew MySQL_InputData_float();
            
        case kMySQL_double:
            return vnnew MySQL_InputData_double();
            
        case kMySQL_date:
            return vnnew MySQL_InputData_date();
            
        case kMySQL_time:
            return vnnew MySQL_InputData_time();
            
        case kMySQL_datetime:
            return vnnew MySQL_InputData_datetime();
            
        case kMySQL_timestamp:
            return vnnew MySQL_InputData_timestamp();
            
        case kMySQL_string:
            return vnnew MySQL_InputData_string();
            
        case kMySQL_blob:
            return vnnew MySQL_InputData_blob();

        default:
            break;
    }
    return nullptr;
}

_vn_end
