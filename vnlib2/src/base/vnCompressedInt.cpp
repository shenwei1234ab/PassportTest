//
//  vnCompressedInt.cpp
//  vnlib
//
//  Created by Wayne on 14-5-10.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnCompressedInt.h"
#include "vnDataStream.h"

#include <cstring>

_vn_begin

//1 byte (7 bits)
//0XXX XXXX
//0x7F
//
//2 bytes (14 bits)
//10XX XXXX | XXXX XXXX
//0x3FFF
//
//3 bytes (21 bits)
//110X XXXX | XXXX XXXX | XXXX XXXX
//0x1FFFFF
//
//4 bytes (28 bits)
//1110 XXXX | XXXX XXXX | XXXX XXXX | XXXX XXXX
//0xFFFFFFF
//
//5 bytes (35 bits)
//1111 0XXX | XXXX XXXX | XXXX XXXX | XXXX XXXX | XXXX XXXX
//0x7FFFFFFFF
//
//6 bytes (42 bits)
//1111 10XX | XXXX XXXX | XXXX XXXX | XXXX XXXX | XXXX XXXX | XXXX XXXX
//0x3FFFFFFFFFF
//
//7 bytes (49 bits)
//1111 110X | XXXX XXXX | XXXX XXXX | XXXX XXXX | XXXX XXXX | XXXX XXXX | XXXX XXXX
//0x1FFFFFFFFFFFF
//
//8 bytes (56 bits)
//1111 1110 | XXXX XXXX | XXXX XXXX | XXXX XXXX | XXXX XXXX | XXXX XXXX | XXXX XXXX | XXXX XXXX
//0xFFFFFFFFFFFFFF
//
//9 bytes (64 bits)
//1111 1111 | XXXX XXXX | XXXX XXXX | XXXX XXXX | XXXX XXXX | XXXX XXXX | XXXX XXXX | XXXX XXXX | XXXX XXXX
//0xFFFFFFFFFFFFFFFF


s64 _readCompressedInt(DataStream &stream) {
	u8 lead;
	u8 tails[8];
	stream >> lead;
	u64 ret;
	u8 t = ~lead;
	if (t & 0x80) {
		ret = lead;
		if (ret & (1LL << 6)) {
			ret |= ~((1LL << 7) - 1);
		}
	} else if (t & 0x40) {
		stream >> DataStream::buf(tails, 1);
		ret = lead & 0x3F;
		ret |= (u64)tails[0] << 6;
		if (ret & (1LL << 13)) {
			ret |= ~((1LL << 14) - 1);
		}
	} else if (t & 0x20) {
		stream >> DataStream::buf(tails, 2);
		ret = lead & 0x1F;
		ret |= (u64)tails[0] << 5;
		ret |= (u64)tails[1] << 13;
		if (ret & (1LL << 20)) {
			ret |= ~((1LL << 21) - 1);
		}
	} else if (t & 0x10) {
		stream >> DataStream::buf(tails, 3);
		ret = lead & 0xF;
		ret |= (u64)tails[0] << 4;
		ret |= (u64)tails[1] << 12;
		ret |= (u64)tails[2] << 20;
		if (ret & (1LL << 27)) {
			ret |= ~((1LL << 28) - 1);
		}
	} else if (t & 0x8) {
		stream >> DataStream::buf(tails, 4);
		ret = lead & 0x7;
		ret |= (u64)tails[0] << 3;
		ret |= (u64)tails[1] << 11;
		ret |= (u64)tails[2] << 19;
		ret |= (u64)tails[3] << 27;
		if (ret & (1LL << 34)) {
			ret |= ~((1LL << 35) - 1);
		}
	} else if (t & 0x4) {
		stream >> DataStream::buf(tails, 5);
		ret = lead & 0x3;
		ret |= (u64)tails[0] << 2;
		ret |= (u64)tails[1] << 10;
		ret |= (u64)tails[2] << 18;
		ret |= (u64)tails[3] << 26;
		ret |= (u64)tails[4] << 34;
		if (ret & (1LL << 41)) {
			ret |= ~((1LL << 42) - 1);
		}
	} else if (t & 0x2) {
		stream >> DataStream::buf(tails, 6);
		ret = lead & 0x1;
		ret |= (u64)tails[0] << 1;
		ret |= (u64)tails[1] << 9;
		ret |= (u64)tails[2] << 17;
		ret |= (u64)tails[3] << 25;
		ret |= (u64)tails[4] << 33;
		ret |= (u64)tails[5] << 41;
		if (ret & (1LL << 48)) {
			ret |= ~((1LL << 49) - 1);
		}
	} else if (t & 0x1) {
		ret = 0;
		stream >> DataStream::buf(&ret, 7);
		if (ret & (1LL << 55)) {
			ret |= ~((1LL << 56) - 1);
		}
	} else {
		stream >> ret;
	}
	return ret;
}

void _writeCompressedInt(DataStream &stream, s64 value) {
	size_t size;
	u8 data[9];
	if (value <= 0x3FLL && value > -0x40LL) {
		size = 1;
		data[0] = (u8)(value & 0x7F);
	} else if (value <= 0x1FFFLL && value >= -0x2000LL) {
		size = 2;
		data[0] = (u8)(0x80 | (value & 0x3F));
		data[1] = (u8)(value >> 6);
	} else if (value <= 0xFFFFFLL && value >= -0x100000LL) {
		size = 3;
		data[0] = (u8)(0xC0 | (value & 0x1F));
		data[1] = (u8)(value >> 5);
		data[2] = (u8)(value >> 13);
	} else if (value <= 0x7FFFFFFLL && value >= -0x8000000LL) {
		size = 4;
		data[0] = (u8)(0xE0 | (value & 0xF));
		data[1] = (u8)(value >> 4);
		data[2] = (u8)(value >> 12);
		data[3] = (u8)(value >> 20);
	} else if (value <= 0x3FFFFFFFFLL && value >= -0x400000000LL) {
		size = 5;
		data[0] = (u8)(0xF0 | (value & 0x7));
		data[1] = (u8)(value >> 3);
		data[2] = (u8)(value >> 11);
		data[3] = (u8)(value >> 19);
		data[4] = (u8)(value >> 27);
	} else if (value <= 0x1FFFFFFFFFFLL && value >= -0x20000000000LL) {
		size = 6;
		data[0] = (u8)(0xF8 | (value & 0x3));
		data[1] = (u8)(value >> 2);
		data[2] = (u8)(value >> 10);
		data[3] = (u8)(value >> 18);
		data[4] = (u8)(value >> 26);
		data[5] = (u8)(value >> 34);
	} else if (value <= 0xFFFFFFFFFFFFLL && value >= -0x1000000000000LL) {
		size = 7;
		data[0] = (u8)(0xFC | (value & 0x1));
		data[1] = (u8)(value >> 1);
		data[2] = (u8)(value >> 9);
		data[3] = (u8)(value >> 17);
		data[4] = (u8)(value >> 25);
		data[5] = (u8)(value >> 33);
		data[6] = (u8)(value >> 41);
	} else if (value <= 0x7FFFFFFFFFFFFFLL && value >= -0x80000000000000LL) {
		size = 8;
		data[0] = 0xFE;
		memcpy(data + 1, &value, 7);
	} else {
		size = 9;
		data[0] = 0xFF;
		*(u64 *)(data + 1) = value;
	}
	stream << DataStream::cbuf(data, size);
}

u64 _readCompressedUInt(DataStream &stream) {
	u8 lead;
	u8 tails[8];
	stream >> lead;
	u64 ret;
	u8 t = ~lead;
	if (t & 0x80) {
		ret = lead;
	} else if (t & 0x40) {
		stream >> DataStream::buf(tails, 1);
		ret = lead & 0x3F;
		ret |= (u64)tails[0] << 6;
	} else if (t & 0x20) {
		stream >> DataStream::buf(tails, 2);
		ret = lead & 0x1F;
		ret |= (u64)tails[0] << 5;
		ret |= (u64)tails[1] << 13;
	} else if (t & 0x10) {
		stream >> DataStream::buf(tails, 3);
		ret = lead & 0xF;
		ret |= (u64)tails[0] << 4;
		ret |= (u64)tails[1] << 12;
		ret |= (u64)tails[2] << 20;
	} else if (t & 0x8) {
		stream >> DataStream::buf(tails, 4);
		ret = lead & 0x7;
		ret |= (u64)tails[0] << 3;
		ret |= (u64)tails[1] << 11;
		ret |= (u64)tails[2] << 19;
		ret |= (u64)tails[3] << 27;
	} else if (t & 0x4) {
		stream >> DataStream::buf(tails, 5);
		ret = lead & 0x3;
		ret |= (u64)tails[0] << 2;
		ret |= (u64)tails[1] << 10;
		ret |= (u64)tails[2] << 18;
		ret |= (u64)tails[3] << 26;
		ret |= (u64)tails[4] << 34;
	} else if (t & 0x2) {
		stream >> DataStream::buf(tails, 6);
		ret = lead & 0x1;
		ret |= (u64)tails[0] << 1;
		ret |= (u64)tails[1] << 9;
		ret |= (u64)tails[2] << 17;
		ret |= (u64)tails[3] << 25;
		ret |= (u64)tails[4] << 33;
		ret |= (u64)tails[5] << 41;
	} else if (t & 0x1) {
		ret = 0;
		stream >> DataStream::buf(&ret, 7);
	} else {
		stream >> ret;
	}
	return ret;
}

void _writeCompressedUInt(DataStream &stream, u64 value) {
	size_t size;
	u8 data[9];
	if (value <= 0x7FLL) {
		size = 1;
		data[0] = (u8)value;
	} else if (value <= 0x3FFFLL) {
		size = 2;
		data[0] = (u8)(0x80 | (value & 0x3F));
		data[1] = (u8)(value >> 6);
	} else if (value <= 0x1FFFFFLL) {
		size = 3;
		data[0] = (u8)(0xC0 | (value & 0x1F));
		data[1] = (u8)(value >> 5);
		data[2] = (u8)(value >> 13);
	} else if (value <= 0xFFFFFFFLL) {
		size = 4;
		data[0] = (u8)(0xE0 | (value & 0xF));
		data[1] = (u8)(value >> 4);
		data[2] = (u8)(value >> 12);
		data[3] = (u8)(value >> 20);
	} else if (value <= 0x7FFFFFFFFLL) {
		size = 5;
		data[0] = (u8)(0xF0 | (value & 0x7));
		data[1] = (u8)(value >> 3);
		data[2] = (u8)(value >> 11);
		data[3] = (u8)(value >> 19);
		data[4] = (u8)(value >> 27);
	} else if (value <= 0x3FFFFFFFFFFLL) {
		size = 6;
		data[0] = (u8)(0xF8 | (value & 0x3));
		data[1] = (u8)(value >> 2);
		data[2] = (u8)(value >> 10);
		data[3] = (u8)(value >> 18);
		data[4] = (u8)(value >> 26);
		data[5] = (u8)(value >> 34);
	} else if (value <= 0x1FFFFFFFFFFFFLL) {
		size = 7;
		data[0] = (u8)(0xFC | (value & 0x1));
		data[1] = (u8)(value >> 1);
		data[2] = (u8)(value >> 9);
		data[3] = (u8)(value >> 17);
		data[4] = (u8)(value >> 25);
		data[5] = (u8)(value >> 33);
		data[6] = (u8)(value >> 41);
	} else if (value <= 0xFFFFFFFFFFFFFFLL) {
		size = 8;
		data[0] = 0xFE;
		memcpy(data + 1, &value, 7);
	} else {
		size = 9;
		data[0] = 0xFF;
		*(u64 *)(data + 1) = value;
	}
	stream << DataStream::cbuf(data, size);
}

size_t _compressedDataSizeInt(s64 value) {
	if (value <= 0x3FLL && value > -0x40LL) {
		return 1;
	}
	if (value <= 0x1FFFLL && value >= -0x2000LL) {
		return 2;
	}
	if (value <= 0xFFFFFLL && value >= -0x100000LL) {
		return 3;
	}
	if (value <= 0x7FFFFFFLL && value >= -0x8000000LL) {
		return 4;
	}
	if (value <= 0x3FFFFFFFFLL && value >= -0x400000000LL) {
		return 5;
	}
	if (value <= 0x1FFFFFFFFFFLL && value >= -0x20000000000LL) {
		return 6;
	}
	if (value <= 0xFFFFFFFFFFFFLL && value >= -0x1000000000000LL) {
		return 7;
	}
	if (value <= 0x7FFFFFFFFFFFFFLL && value >= -0x80000000000000LL) {
		return 8;
	}
	return 9;
}


size_t _compressedDataSizeUInt(u64 value) {
	if (value <= 0x7FLL) {
		return 1;
	}
	if (value <= 0x3FFFLL) {
		return 2;
	}
	if (value <= 0x1FFFFFLL) {
		return 3;
	}
	if (value <= 0xFFFFFFFLL) {
		return 4;
	}
	if (value <= 0x7FFFFFFFFLL) {
		return 5;
	}
	if (value <= 0x3FFFFFFFFFFLL) {
		return 6;
	}
	if (value <= 0x1FFFFFFFFFFFFLL) {
		return 7;
	}
	if (value <= 0xFFFFFFFFFFFFFFLL) {
		return 8;
	}
	return 9;
}

size_t _compressedDataSizeLeadByte(u8 lead) {
	u8 t = ~lead;
	if (t & 0x80) {
		return 1;
	}
	if (t & 0x40) {
		return 2;
	}
	if (t & 0x20) {
		return 3;
	}
	if (t & 0x10) {
		return 4;
	}
	if (t & 0x8) {
		return 5;
	}
	if (t & 0x4) {
		return 6;
	}
	if (t & 0x2) {
		return 7;
	}
	if (t & 0x1) {
		return 8;
	}
	return 9;
}


_vn_end
