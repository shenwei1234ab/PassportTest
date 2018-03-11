//
//  vnLexToken.cpp
//  vnlib
//
//  Created by Wayne on 15/5/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnLexToken.h"

_vn_begin

namespace lex {
    Token::Token(const FilePtr &_file)
    : file(_file) {
        
    }
    
    Token::Token(FilePtr &&_file)
    : file(std::move(_file)) {
        
    }
    
    Token & Token::set(Type t) {
        type = t;
        return *this;
    }
    
    Token & Token::set(Type t, const str8 &s) {
        type = t;
        text = s;
        return *this;
    }
}


_vn_end
