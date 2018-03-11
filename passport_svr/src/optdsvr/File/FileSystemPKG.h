//
//  FileSystem.h
//  LOD
//
//  Created by Wayne on 13-6-3.
//  Copyright (c) 2013 secher.com. All rights reserved.
//

#ifndef __LOD__FileSystem__
#define __LOD__FileSystem__

#include "vnFileSystem.h"
#include "vnBuffer.h"
#include <mutex>
#include <string.h>
#include <vector>

class FileSystemPKG : public vn::FileSystem {
public:
    class FileInMemory : public vn::FileStream {
    public:
        FileInMemory(vn::Buffer *context, bool grab);
        virtual ~FileInMemory();
        virtual size_t read(void *buffer, size_t size) { return m_stream.read(buffer, size); }
        virtual size_t write(const void *buffer, size_t size) { return 0; }
        virtual bool seek(vn::s64 offset, SeekDir way) { return m_stream.seek(offset, way); }
        virtual vn::s64 tell() { return m_stream.tell(); }
        virtual vn::s64 size() { return m_size; }
        virtual void close();
    private:
        vn::BufferStream m_stream;
        vn::s64 m_size;
    };
    
    class File : public vn::FileStream {
    public:
        File(FileSystemPKG *fs, vn::u64 offset, vn::u64 size);
        virtual ~File();
        virtual size_t read(void *buffer, size_t size);
        virtual size_t write(const void *buffer, size_t size) { return 0; }
        virtual bool seek(vn::s64 offset, SeekDir way);
        virtual vn::s64 tell() { return m_current; }
        virtual vn::s64 size() { return m_size; }
        virtual void close();
    private:
        FileSystemPKG *m_fs;
        vn::u64 m_offset;
        vn::u64 m_size;
        vn::u64 m_current;

    };
    
    FileSystemPKG();
    virtual ~FileSystemPKG();
	virtual vn::FileStream * open(const vn::str8 &file, bool readOnly);
	virtual vn::FileStream * create(const vn::str8 &file, bool writeOnly) { return 0; }
    virtual bool remove(const vn::str8 &path) { return false; }
	virtual bool list(const vn::str8 &path, std::list<vn::str8> &result) { return false; }
	virtual bool mkdir(const vn::str8 &path) { return false; }
	virtual bool exist(const vn::str8 &path){return false;}
	virtual bool createDirectory(const vn::str8 &path) {return false;}
    virtual bool removeDirectory(const vn::str8 &path) {return false;}

    
    bool load(vn::FileSystem *base, const vn::str8 &fileName);
    
    size_t _read(vn::u64 offset, void *buffer, size_t size);
    
private:
    vn::FileStream *m_file;
    vn::BufferStream *m_stream;
    
    std::mutex m_lock;
    
    struct _FileInfo {
        vn::str8 name;
        vn::u64 offset;
        vn::u64 size;
    };
    
    typedef std::vector<_FileInfo> FileIndex;
    FileIndex m_files;
};

#endif /* defined(__LOD__FileSystem__) */
