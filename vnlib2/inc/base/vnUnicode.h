//
//  vnUnicode.h
//  vnlib
//
//  Created by Wayne on 14-5-11.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnUnicode_h
#define vnlib_vnUnicode_h

#include "../vnDefine.h"

_vn_begin

class Unicode {
public:
	static ssize_t convert(const c8 *src, size_t src_len, c32 *dest);
	static ssize_t convert(const c8 *src, size_t src_len, c32 *dest, size_t dest_len);
	static ssize_t convert(const c8 *src, size_t src_len, str32 &dest);
    static ssize_t convert(const str8 &src, str32 &dest) {
        return convert(src.c_str(), src.length(), dest);
    }
	
	static ssize_t convert(const c8 *src, size_t src_len, c16 *dest);
	static ssize_t convert(const c8 *src, size_t src_len, c16 *dest, size_t dest_len);
	static ssize_t convert(const c8 *src, size_t src_len, str16 &dest);
    static ssize_t convert(const str8 &src, str16 &dest) {
        return convert(src.c_str(), src.length(), dest);
    }
	
	static ssize_t convert(const c16 *src, size_t src_len, c8 *dest);
	static ssize_t convert(const c16 *src, size_t src_len, c8 *dest, size_t dest_len);
	static ssize_t convert(const c16 *src, size_t src_len, str8 &dest);
    static ssize_t convert(const str16 &src, str8 &dest) {
        return convert(src.c_str(), src.length(), dest);
    }
	
	static ssize_t convert(const c16 *src, size_t src_len, c32 *dest);
	static ssize_t convert(const c16 *src, size_t src_len, c32 *dest, size_t dest_len);
	static ssize_t convert(const c16 *src, size_t src_len, str32 &dest);
    static ssize_t convert(const str16 &src, str32 &dest) {
        return convert(src.c_str(), src.length(), dest);
    }
    
	static ssize_t convert(const c32 *src, size_t src_len, c8 *dest);
	static ssize_t convert(const c32 *src, size_t src_len, c8 *dest, size_t dest_len);
	static ssize_t convert(const c32 *src, size_t src_len, str8 &dest);
    static ssize_t convert(const str32 &src, str8 &dest) {
        return convert(src.c_str(), src.length(), dest);
    }
	
	static ssize_t convert(const c32 *src, size_t src_len, c16 *dest);
	static ssize_t convert(const c32 *src, size_t src_len, c16 *dest, size_t dest_len);
	static ssize_t convert(const c32 *src, size_t src_len, str16 &dest);
    static ssize_t convert(const str32 &src, str16 &dest) {
        return convert(src.c_str(), src.length(), dest);
    }
};

_vn_end

#endif
