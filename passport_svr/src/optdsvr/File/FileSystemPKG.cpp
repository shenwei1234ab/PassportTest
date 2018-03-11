//
//  FileSystem.cpp
//  LOD
//
//  Created by Wayne on 13-6-3.
//  Copyright (c) 2013年 secher.com. All rights reserved.
//

#include "FileSystemPKG.h"


FileSystemPKG::FileInMemory::FileInMemory(vn::Buffer *context, bool grab)
: m_stream(context)
, m_size(context->size()) {

}

FileSystemPKG::FileInMemory::~FileInMemory() {
	
}

void FileSystemPKG::FileInMemory::close() {
    delete this;
}

FileSystemPKG::File::File(FileSystemPKG *fs, vn::u64 offset, vn::u64 size)
: m_fs(fs)
, m_offset(offset)
, m_size(size)
, m_current(0) {
    m_fs->grab();
}

FileSystemPKG::File::~File() {
    m_fs->drop();
}

size_t FileSystemPKG::File::read(void *buffer, size_t size) {
    vn::u64 t = m_current + size;
    if (t > m_size) {
        size -= t - m_size;
    }
    size_t ret = m_fs->_read(m_offset + m_current, buffer, size);
    m_current += ret;
    return ret;
}

bool FileSystemPKG::File::seek(vn::s64 offset, SeekDir mode) {
    vn::s64 cur;
    switch (mode) {
        case vn::DataStream::kBegin:
            cur = offset;
            break;
        case vn::DataStream::kCurrent:
            cur = (vn::s64)m_current + offset;
            break;
        case vn::DataStream::kEnd: {
            cur = (vn::s64)m_size + offset;
            break;
            
        }
    }
    //fix
    if (cur < 0 || cur > (vn::s64)m_size) {
        return false;
    }
    m_current = cur;
    return true;
}

void FileSystemPKG::File::close() {
    delete this;
}

FileSystemPKG::FileSystemPKG()
: m_file(0) {
    
}

FileSystemPKG::~FileSystemPKG() {
    if (m_file) {
        m_file->close();
        m_file = 0;
    }
}

bool FileSystemPKG::load(vn::FileSystem *base, const vn::str8 &fileName) {
    if (!base) {
        return false;
    }
#if VN_PLATFORM != VN_PLATFORM_ANDROID
	m_file = base->open(fileName);
#else
    if (fileName.length() >= 4 && strcmp(fileName.c_str() + fileName.length() - 4, ".pkg") == 0) {
        m_file = base->open(fileName + ".jet");
    } else {
        m_file = base->open(fileName);
    }
#endif
	if (!m_file) {
		return false;
	}
    try {
        vn::c8 head[5] = {};
        *m_file >> vn::DataStream::buf(head, 4);
        if (strcmp(head, "VPKG")) {
            m_file->close();
            m_file = 0;
            return false;
        }
        vn::u32 count;
        *m_file >> count;
        m_files.resize(count);
        for (vn::u32 i = 0; i < count; ++i) {
            vn::u16 len;
            *m_file >> len;
            _FileInfo &info = m_files[i];
            if (len) {
                info.name.resize(len);
                *m_file >> vn::DataStream::buf(const_cast<vn::c8 *>(info.name.c_str()), len);
            }
            *m_file >> info.offset >> info.size;
        }
    } catch (vn::DataStream::Exception &) {
        m_file->close();
        m_file = 0;
        return false;
    }
	return true;
}

vn::FileStream * FileSystemPKG::open(const vn::str8 &file, bool readOnly) {
	if (!readOnly) {
		return 0;
	}
	int low = 0, high = (int)m_files.size() - 1;
	while (low <= high) {
		int mid = (low + high) / 2;
		const _FileInfo &info = m_files[mid];
		int ret = strcmp(file.c_str(), info.name.c_str());
		if (ret == 0) {
            //vn::FixedBuffer *buf = vnnew vn::FixedBuffer((size_t)info.size);
            //_read(info.offset, buf->lock(), (size_t)info.size);
            //buf->unlock();
			//return vnnew FileInMemory(buf, false);
            return vnnew File(this, info.offset, info.size);
		}
		if (ret > 0) {
			low = mid + 1;
		} else {
			high = mid - 1;
		}
	}
	return 0;
}

size_t FileSystemPKG::_read(vn::u64 offset, void *buffer, size_t size) {
    std::lock_guard<std::mutex> lck(m_lock);
    m_file->seek(offset);
    return m_file->read(buffer, size);
}

