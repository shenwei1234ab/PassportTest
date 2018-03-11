//
//  TableBase.cpp
//  tblcoder
//
//  Created by Wayne on 14-2-8.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "TableBase.h"

namespace tbl {
    


    DataStream & operator <<(DataStream &stream, const RecordFormat &format) {
        _write_z_u32(stream, (u32)format.size());
        for (RecordFormat::const_iterator it = format.begin(); it != format.end(); ++it) {
            stream << it->value;
        }
        return stream;
    }

    DataStream & operator >>(DataStream &stream, RecordFormat &format) {
        size_t size;
        _read_z_u32(stream, size);
        format.resize(size);
        for (RecordFormat::iterator it = format.begin(); it != format.end(); ++it) {
            stream >> it->value;
        }
        return stream;
    }

    DataStream & operator >>(DataStream &stream, Field_string &field) {
		stream >> field.null;
		if (!field.null) {
			_read_str8(stream, field.value);
		}
		return stream;
    }
    
    DataStream & operator >>(DataStream &stream, Field_int &field) {
		stream >> field.null;
		if (!field.null) {
			stream >> field.value;
		} else {
			field.value = 0;
		}
		return stream;
    }
    
    DataStream & operator >>(DataStream &stream, Field_uint &field) {
		stream >> field.null;
		if (!field.null) {
			stream >> field.value;
		} else {
			field.value = 0;
		}
		return stream;
    }
    
    DataStream & operator >>(DataStream &stream, Field_float &field) {
		stream >> field.null;
		if (!field.null) {
			stream >> field.value;
		} else {
			field.value = 0;
		}
		return stream;
    }

	DataStream & operator <<(DataStream &stream, const Field_string &field) {
		stream << field.null;
		if (!field.null) {
			_write_str8(stream, field.value);
		}
		return stream;
	}

	DataStream & operator <<(DataStream &stream, const Field_int &field) {
		stream << field.null;
		if (!field.null) {
			stream << field.value;
		}
		return stream;
	}

	DataStream & operator <<(DataStream &stream, const Field_uint &field) {
		stream << field.null;
		if (!field.null) {
			stream << field.value;
		}
		return stream;
	}

	DataStream & operator <<(DataStream &stream, const Field_float &field) {
		stream << field.null;
		if (!field.null) {
			stream << field.value;
		}
		return stream;
	}

    int Field_string::compare(const Field_string &other) const {
        return strcmp(value.c_str(), other.value.c_str());
    }

	int Field_string::compare(const str8 &_value) const {
		return strcmp(value.c_str(), _value.c_str());
	}
    
    int Field_int::compare(const Field_int &other) const {
        return (value == other.value ? 0 :(value < other.value ? -1 : 1));
    }

	int Field_int::compare(s32 _value) const {
		return (value == _value ? 0 : (value < _value ? -1 : 1));
	}
    
	int Field_uint::compare(const Field_uint &other) const {
		return (value == other.value ? 0 : (value < other.value ? -1 : 1));
	}

    int Field_uint::compare(u32 _value) const {
		return (value == _value ? 0 : (value < _value ? -1 : 1));
    }
    
	int Field_float::compare(const Field_float &other) const {
		return (value == other.value ? 0 : (value < other.value ? -1 : 1));
	}

    int Field_float::compare(f32 _value) const {
		return (value == _value ? 0 : (value < _value ? -1 : 1));
    }
    
    u32 _read_z_u32(DataStream &stream) {
        u8 lead;
        u32 value = 0;
        stream >> lead;
        uint num = 0;
        if ((lead & 0x80) == 0) {
            value = lead;
        } else if ((lead & 0x40) == 0) {
            num = 1;
            value = lead & 0x3F;
        } else if ((lead & 0x20) == 0) {
            num = 2;
            value = lead & 0x1F;
        } else if ((lead & 0x10) == 0) {
            num = 3;
            value = lead & 0x0F;
        }
        if (num) {
            u8 tail[3];
            if (stream.read(tail, num) != num) {
                throw DataStream::ReadException();
            }
            for (uint i = 0; i < num; ++i) {
                value <<= 8;
                value |= tail[i];
            }
        }
        return value;
    }
    
    void _write_z_u32(DataStream &stream, u32 value) {
        u8 buf[4] = {};
        uint num = 1;
        if (value < 0x80) {
            buf[0] = (u8)value;
        } else if (value < 0x4000) {
            buf[0] = (u8)((value >> 8) | 0x80);
            buf[1] = (u8)value;
            num = 2;
        } else if (value < 0x200000) {
            buf[0] = (u8)((value >> 16) | 0xC0);
            buf[1] = (u8)(value >> 8);
            buf[2] = (u8)value;
            num = 3;
        } else {
            buf[0] = (u8)((value >> 24) | 0xE0);
            buf[1] = (u8)(value >> 16);
            buf[2] = (u8)(value >> 8);
            buf[3] = (u8)value;
            num = 4;
        }
        if (stream.write(buf, num) != num) {
            throw DataStream::WriteException();
        }
    }
    
    void _read_str8(DataStream &stream, str8 &value) {
        u32 size = _read_z_u32(stream);
        if (size) {
            value.resize(size);
            stream >> DataStream::buf(const_cast<c8 *>(value.c_str()), size);
        } else {
            value.clear();
        }
    }
    
    void _write_str8(DataStream &stream, const str8 &value) {
        _write_z_u32(stream, (u32)value.size());
        if (!value.empty()) {
            stream << DataStream::cbuf(value.c_str(), value.size());
        }
    }
    
}
