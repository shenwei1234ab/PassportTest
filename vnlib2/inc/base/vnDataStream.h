//
//  vnDataStream.h
//  vnlib
//
//  Created by Wayne on 14-5-10.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnDataStream_h
#define vnlib_vnDataStream_h

#include "vnNonCopyable.h"
#include "vnCompressedInt.h"

_vn_begin

class DataStream : public NonCopyable {
public:
	struct Exception {
		
	};
	
	struct ReadException : Exception {
		
	};
	
	struct WriteException : Exception {
		
	};
	
	struct buf {
		void *ptr;
		size_t size;
		buf(void *_p, size_t _s) : ptr(_p), size(_s) {}
	};
	
	struct cbuf {
		const void *ptr;
		size_t size;
		cbuf(const void *_p, size_t _s) : ptr(_p), size(_s) {}
	};
	
	template <typename T>
	static CompressedInt<T> cci(T v) { return CompressedInt<T>(v); }
	
	template <typename T>
	static CompressedUInt<T> ccu(T v) { return CompressedUInt<T>(v); }
	
	template <typename T>
	static CompressedInt<T &> ci(T &v) { return CompressedInt<T &>(v); }
	
	template <typename T>
	static CompressedUInt<T &> cu(T &v) { return CompressedUInt<T &>(v); }
	
	enum SeekDir {
		kBegin = SEEK_SET,
		kCurrent = SEEK_CUR,
		kEnd = SEEK_END
	};
	
	virtual size_t read(void *buffer, size_t size) = 0;
	virtual size_t write(const void *buffer, size_t size) = 0;
	virtual bool seek(s64 offset, SeekDir way = kBegin) = 0;
	virtual s64 tell() = 0;
	
	DataStream & operator >>(bool &value);
	DataStream & operator >>(s8 &value);
	DataStream & operator >>(u8 &value);
	DataStream & operator >>(s16 &value);
	DataStream & operator >>(u16 &value);
	DataStream & operator >>(s32 &value);
	DataStream & operator >>(u32 &value);
	DataStream & operator >>(s64 &value);
	DataStream & operator >>(u64 &value);
	DataStream & operator >>(f32 &value);
	DataStream & operator >>(f64 &value);
	DataStream & operator >>(str8 &value);
	DataStream & operator >>(str16 &value);
	DataStream & operator >>(str32 &value);
	DataStream & operator >>(const buf &value);
	
	template <typename T>
	DataStream & operator >>(const CompressedInt<T &> &value) {
		value.value = (T)_readCompressedInt(*this);
		return *this;
	}
	
	template <typename T>
	DataStream & operator >>(const CompressedUInt<T &> &value) {
		value.value = (T)_readCompressedUInt(*this);
		return *this;
	}
	
	DataStream & operator <<(bool value);
	DataStream & operator <<(s8 value);
	DataStream & operator <<(u8 value);
	DataStream & operator <<(s16 value);
	DataStream & operator <<(u16 value);
	DataStream & operator <<(s32 value);
	DataStream & operator <<(u32 value);
	DataStream & operator <<(s64 value);
	DataStream & operator <<(u64 value);
	DataStream & operator <<(f32 value);
	DataStream & operator <<(f64 value);
	DataStream & operator <<(const str8 &value);
	DataStream & operator <<(const str16 &value);
	DataStream & operator <<(const str32 &value);
	DataStream & operator <<(const buf &value);
	DataStream & operator <<(const cbuf &value);
	
	DataStream & operator <<(const c8 *value);
	DataStream & operator <<(const c16 *value);
	DataStream & operator <<(const c32 *value);
	
	template <typename T>
	DataStream & operator <<(const CompressedInt<T> &value) {
		_writeCompressedInt(*this, (long long)value.value);
		return *this;
	}
	
	template <typename T>
	DataStream & operator <<(const CompressedUInt<T> &value) {
		_writeCompressedUInt(*this, (unsigned long long)value.value);
		return *this;
	}
	
};

_vn_end


#endif
