//
//  vnFileManager.h
//  vnlib
//
//  Created by Wayne on 5/12/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnFileManager_h
#define vnlib_vnFileManager_h

#include "vnSingleton.h"
#include "vnFileSystem.h"
#include "vnFilePath.h"
#include <vector>

#include <unordered_map>


_vn_begin

class FileManager : public Singleton<FileManager> {
	friend struct CreateUsingNew<FileManager>;
public:
    FileStream * open(const FilePath &path, bool readOnly = true);
    FileStream * create(const FilePath &path, bool writeOnly = true);
    bool exist(const FilePath &path);
    bool remove(const FilePath &path);
    bool list(const FilePath &path, std::list<str8> &result);
    bool createDirectory(const FilePath &path);
    bool removeDirectory(const FilePath &path);
    
	FileSystemPtr findFileSystem(u32 fsId);
	void setFileSystem(u32 fsId, const FileSystemPtr &fs);
    void setFileSystem(u32 fsId, FileSystemPtr &&fs);
    std::vector<u32> listFileSystemId();
private:
	FileManager();
    std::unordered_map<u32, FileSystemPtr> m_systems;
};

_vn_end

#endif
