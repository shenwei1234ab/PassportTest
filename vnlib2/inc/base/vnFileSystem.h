//
//  vnFileSystem.h
//  vnlib
//
//  Created by Wayne on 5/12/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnFileSystem_h
#define vnlib_vnFileSystem_h

#include "vnRefCounted.h"
#include "vnDataStream.h"
#include <list>

_vn_begin

class FileStream : public DataStream, public AllocatedObject {
public:
	virtual s64 size() = 0;
	virtual void close() = 0;
};

class FileSystem : virtual public RefCounted {
public:
	virtual FileStream * open(const str8 &file, bool readOnly = true) = 0;
	virtual FileStream * create(const str8 &file, bool writeOnly = true) = 0;
    virtual bool exist(const str8 &path) = 0;
	virtual bool remove(const str8 &path) = 0;
	virtual bool list(const str8 &path, std::list<str8> &result) = 0;
	virtual bool createDirectory(const str8 &path) = 0;
    virtual bool removeDirectory(const str8 &path) = 0;
};

typedef RefCountedPtr<FileSystem> FileSystemPtr;

class FileSystemOS : public FileSystem {
public:
    FileSystemOS() = default;
	FileSystemOS(const str8 &path);
    FileSystemOS(str8 &&path);
	virtual FileStream * open(const str8 &file, bool readOnly = true);
	virtual FileStream * create(const str8 &file, bool writeOnly = true);
    virtual bool exist(const str8 &path);
	virtual bool remove(const str8 &path);
	virtual bool list(const str8 &path, std::list<str8> &result);
    virtual bool createDirectory(const str8 &path);
    virtual bool removeDirectory(const str8 &path);
protected:
	str8 m_path;
};

_vn_end

#endif
