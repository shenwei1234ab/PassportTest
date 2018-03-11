//
//  vnFilePath.h
//  vnlib
//
//  Created by Wayne on 5/12/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnFilePath_h
#define vnlib_vnFilePath_h

#include "vnAllocatedObject.h"

_vn_begin

struct FilePath : AllocatedObject {
    u32 fsId = 0;
    str8 fileName;
    
    FilePath() = default;
    FilePath(const FilePath &) = default;
    FilePath(FilePath &&other);
    FilePath(u32 _fsId, const str8 &_fileName);
    FilePath(u32 _fsId, str8 &&_fileName);
    FilePath(const c8 *fullPath);
    
    str8 fullPath() const;
    
    bool operator ==(const FilePath &rhs) const;
    bool operator !=(const FilePath &rhs) const;
    bool operator <(const FilePath &rhs) const;
    
    FilePath & operator =(const FilePath &rhs) = default;
    FilePath & operator =(FilePath &&rhs);
    FilePath & operator =(const c8 *fullPath);
};

_vn_end

#endif
