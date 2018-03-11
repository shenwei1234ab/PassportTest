//
//  vnLexToken.h
//  vnlib
//
//  Created by Wayne on 15/5/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnLexToken_h
#define vnlib_vnLexToken_h

#include "vnRefCounted.h"

#include <list>

_vn_begin

namespace lex {
    
    struct Token : RefCounted {
        struct File : RefCounted {
            str8 name;
        };
        typedef RefCountedPtr<File> FilePtr;
        
        enum Type {
            kNull = 0,
            kIdentifier,
            kSymbol,
            kNumber,
            
            kCharacter,
            kInteger,
            kLongInteger,
            kFloat,
            kDouble,
            kString,
            
            kKeyword,
        };
        
        Token() = default;
        Token(const FilePtr &_file);
        Token(FilePtr &&_file);
        
        FilePtr file;
        int row = 0;
        int col = 0;
        
        Type type = kNull;
        str8 text;
        union {
            u32 int32;
            u64 int64 = 0;
            f32 float32;
            f64 float64;
        };
        
        Token & set(Type t);
        Token & set(Type t, const str8 &s);
        
    };
    
    
    typedef RefCountedPtr<Token> TokenPtr;
    typedef std::list<TokenPtr> TokenList;
}

_vn_end

#endif
