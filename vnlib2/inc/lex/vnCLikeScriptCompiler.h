//
//  vnCLikeScriptCompiler.h
//  vnlib
//
//  Created by Wayne on 15/6/28.
//  Copyright (c) 2015年 viichi.com. All rights reserved.
//

#ifndef vnlib_vnCLikeScriptCompiler_h
#define vnlib_vnCLikeScriptCompiler_h

#include "vnLexer2.h"
#include "vnLexLR_Parser.h"

#include "vnFilePath.h"

_vn_begin

class CLikeScriptCompiler : public AllocatedObject {
public:
	CLikeScriptCompiler();
	
	
	lex::LR_NodePtr parse(const FilePath &file);
	lex::LR_NodePtr parse(const c8 *text, size_t length, lex::Token::FilePtr file = nullptr);
	
protected:
	lex::Lexer2 m_lexer;
	lex::LR_Parser m_parser;
};

_vn_end

#endif
