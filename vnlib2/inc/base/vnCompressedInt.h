//
//  vnCompressedInt.h
//  vnlib
//
//  Created by Wayne on 14-5-10.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnCompressedInt_h
#define vnlib_vnCompressedInt_h

#include "../vnDefine.h"

_vn_begin

class DataStream;

s64 _readCompressedInt(DataStream &stream);
void _writeCompressedInt(DataStream &stream, s64 value);

u64 _readCompressedUInt(DataStream &stream);
void _writeCompressedUInt(DataStream &stream, u64 value);

size_t _compressedDataSizeInt(s64 value);
size_t _compressedDataSizeUInt(u64 value);
size_t _compressedDataSizeLeadByte(u8 lead);

template <typename T>
struct CompressedInt;

template <typename T>
struct CompressedUInt;

template <typename _Int>
struct CompressedInt {
	_Int value;
	CompressedInt(_Int _v) : value(_v) {}
};

template <typename _Int>
struct CompressedInt<_Int &> {
	_Int &value;
	CompressedInt(_Int &_v) : value(_v) {}
};

template <typename _UInt>
struct CompressedUInt {
	_UInt value;
	CompressedUInt(_UInt _v) : value(_v) {}
};

template <typename _UInt>
struct CompressedUInt<_UInt &> {
	_UInt &value;
	CompressedUInt(_UInt &_v) : value(_v) {}
};



_vn_end


#endif
