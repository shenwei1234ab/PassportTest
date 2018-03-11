//
//  vnCLikeScript_SyntaxTree.cpp
//  vnlib
//
//  Created by Wayne on 7/1/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "vnCLikeScript_SyntaxTree.h"

_vn_begin

namespace clks {
    
    void ST_Element::setFileInfo(const lex::Token &token) {
        file = token.file;
        file_row = token.row;
        file_col = token.col;
    }
    
}

_vn_end
