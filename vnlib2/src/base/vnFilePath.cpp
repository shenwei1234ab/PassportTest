//
//  vnFilePath.cpp
//  vnlib
//
//  Created by Wayne on 5/12/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnFilePath.h"
#include "vnAssert.h"
#include <sstream>

_vn_begin

FilePath::FilePath(FilePath &&other)
: fsId(other.fsId)
, fileName(std::move(other.fileName)) {
    
}

FilePath::FilePath(u32 _fsId, const str8 &_fileName)
: fsId(_fsId)
, fileName(_fileName) {
    
}

FilePath::FilePath(u32 _fsId, str8 &&_fileName)
: fsId(_fsId)
, fileName(std::move(_fileName)) {
    
}

FilePath::FilePath(const c8 *fullPath) {
    *this = fullPath;
}

str8 FilePath::fullPath() const {
    std::ostringstream oss;
    oss << fsId << ":/" << fileName.c_str();
    return oss.str();
}

bool FilePath::operator ==(const FilePath &rhs) const {
    return fsId == rhs.fsId && fileName == rhs.fileName;
}

bool FilePath::operator !=(const FilePath &rhs) const {
    return fsId != rhs.fsId || fileName != rhs.fileName;
}

bool FilePath::operator <(const FilePath &rhs) const {
    if (fsId < rhs.fsId) {
        return true;
    } else if (fsId == rhs.fsId) {
        return fileName < rhs.fileName;
    }
    return false;
}

FilePath & FilePath::operator =(FilePath &&rhs) {
    fsId = rhs.fsId;
    fileName = std::move(rhs.fileName);
    return *this;
}

FilePath & FilePath::operator =(const c8 *fullPath) {
    vnassert(fullPath);
    int off = 0;
    if (sscanf(fullPath, "%u:/%n", &fsId, &off) && off) {
        fileName = fullPath + off;
    }
    return *this;
}

_vn_end
