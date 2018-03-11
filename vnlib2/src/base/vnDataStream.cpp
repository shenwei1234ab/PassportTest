//
//  vnDataStream.cpp
//  vnlib
//
//  Created by Wayne on 14-5-10.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnDataStream.h"
#include "vnUnicode.h"

_vn_begin


DataStream & DataStream::operator >>(bool &value) {
	if (read(&value, sizeof(value)) != sizeof(value)) {
		throw ReadException();
	}
	return *this;
}

DataStream & DataStream::operator >>(s8 &value) {
	if (read(&value, sizeof(value)) != sizeof(value)) {
		throw ReadException();
	}
	return *this;
}

DataStream & DataStream::operator >>(u8 &value) {
	if (read(&value, sizeof(value)) != sizeof(value)) {
		throw ReadException();
	}
	return *this;
}

DataStream & DataStream::operator >>(s16 &value) {
	if (read(&value, sizeof(value)) != sizeof(value)) {
		throw ReadException();
	}
	return *this;
}

DataStream & DataStream::operator >>(u16 &value) {
	if (read(&value, sizeof(value)) != sizeof(value)) {
		throw ReadException();
	}
	return *this;
}

DataStream & DataStream::operator >>(s32 &value) {
	if (read(&value, sizeof(value)) != sizeof(value)) {
		throw ReadException();
	}
	return *this;
}

DataStream & DataStream::operator >>(u32 &value) {
	if (read(&value, sizeof(value)) != sizeof(value)) {
		throw ReadException();
	}
	return *this;
}

DataStream & DataStream::operator >>(s64 &value) {
	if (read(&value, sizeof(value)) != sizeof(value)) {
		throw ReadException();
	}
	return *this;
}

DataStream & DataStream::operator >>(u64 &value) {
	if (read(&value, sizeof(value)) != sizeof(value)) {
		throw ReadException();
	}
	return *this;
}

DataStream & DataStream::operator >>(f32 &value) {
	if (read(&value, sizeof(value)) != sizeof(value)) {
		throw ReadException();
	}
	return *this;
}

DataStream & DataStream::operator >>(f64 &value) {
	if (read(&value, sizeof(value)) != sizeof(value)) {
		throw ReadException();
	}
	return *this;
}

DataStream & DataStream::operator >>(str8 &value) {
	size_t len = (size_t)_readCompressedUInt(*this);
	value.resize(len);
	if (len) {
		*this >> buf(&value[0], len);
	}
	return *this;
}

DataStream & DataStream::operator >>(str16 &value) {
	str8 t;
	*this >> t;
	value.reserve(t.length());
	Unicode::convert(t.c_str(), t.length(), value);
	return *this;
}

DataStream & DataStream::operator >>(str32 &value) {
	str8 t;
	*this >> t;
	value.reserve(t.length());
	Unicode::convert(t.c_str(), t.length(), value);
	return *this;
}

DataStream & DataStream::operator >>(const buf &value) {
	if (read(value.ptr, value.size) != value.size) {
		throw ReadException();
	}
	return *this;
}

///////////////////////////////////////////////////////

DataStream & DataStream::operator <<(bool value) {
	if (write(&value, sizeof(value)) != sizeof(value)) {
		throw WriteException();
	}
	return *this;
}

DataStream & DataStream::operator <<(s8 value) {
	if (write(&value, sizeof(value)) != sizeof(value)) {
		throw WriteException();
	}
	return *this;
}

DataStream & DataStream::operator <<(u8 value) {
	if (write(&value, sizeof(value)) != sizeof(value)) {
		throw WriteException();
	}
	return *this;
}

DataStream & DataStream::operator <<(s16 value) {
	if (write(&value, sizeof(value)) != sizeof(value)) {
		throw WriteException();
	}
	return *this;
}

DataStream & DataStream::operator <<(u16 value) {
	if (write(&value, sizeof(value)) != sizeof(value)) {
		throw WriteException();
	}
	return *this;
}

DataStream & DataStream::operator <<(s32 value) {
	if (write(&value, sizeof(value)) != sizeof(value)) {
		throw WriteException();
	}
	return *this;
}

DataStream & DataStream::operator <<(u32 value) {
	if (write(&value, sizeof(value)) != sizeof(value)) {
		throw WriteException();
	}
	return *this;
}

DataStream & DataStream::operator <<(s64 value) {
	if (write(&value, sizeof(value)) != sizeof(value)) {
		throw WriteException();
	}
	return *this;
}

DataStream & DataStream::operator <<(u64 value) {
	if (write(&value, sizeof(value)) != sizeof(value)) {
		throw WriteException();
	}
	return *this;
}

DataStream & DataStream::operator <<(f32 value) {
	if (write(&value, sizeof(value)) != sizeof(value)) {
		throw WriteException();
	}
	return *this;
}

DataStream & DataStream::operator <<(f64 value) {
	if (write(&value, sizeof(value)) != sizeof(value)) {
		throw WriteException();
	}
	return *this;
}

DataStream & DataStream::operator <<(const str8 &value) {
	_writeCompressedUInt(*this, value.length());
	if (!value.empty()) {
		*this << cbuf(value.data(), value.length());
	}
	return *this;
}

DataStream & DataStream::operator <<(const str16 &value) {
	str8 t;
	t.reserve(value.length());
	Unicode::convert(value.c_str(), value.length(), t);
	return *this << t;
}

DataStream & DataStream::operator <<(const str32 &value) {
	str8 t;
	t.reserve(value.length());
	Unicode::convert(value.c_str(), value.length(), t);
	return *this << t;
}

DataStream & DataStream::operator <<(const cbuf &value) {
	if (write(value.ptr, value.size) != value.size) {
		throw WriteException();
	}
	return *this;
}

DataStream & DataStream::operator <<(const c8 *value) {
    size_t len = std::char_traits<c8>::length(value);
	_writeCompressedUInt(*this, len);
	if (len) {
		if (write(value, len) != len) {
			throw WriteException();
		}
	}
	return *this;
}

DataStream & DataStream::operator <<(const c16 *value) {
	size_t len = (u16)std::char_traits<c16>::length(value);
	str8 t;
	t.reserve(len);
	Unicode::convert(value, len, t);
	return *this << t;
}

DataStream & DataStream::operator <<(const c32 *value) {
	size_t len = std::char_traits<c32>::length(value);
	str8 t;
	t.reserve(len);
	Unicode::convert(value, len, t);
	return *this << t;
}


_vn_end
