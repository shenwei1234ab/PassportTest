//
//  vnCLikeScriptCompiler.cpp
//  vnlib
//
//  Created by Wayne on 15/6/28.
//  Copyright (c) 2015年 viichi.com. All rights reserved.
//

#include "vnCLikeScriptCompiler.h"
#include "vnLexer2Analizers.h"

#include "vnFileManager.h"

#include "vnLog.h"

#include <cstring>

_vn_begin

namespace clks {
#include "vnCLikeScriptStates.hpp"
}

CLikeScriptCompiler::CLikeScriptCompiler()
: m_lexer({
	lex::Lexer2::AnalizerCreatorRef(lex::L2AC_Space()),
	lex::Lexer2::AnalizerCreatorRef(lex::L2AC_Comment()),
	lex::Lexer2::AnalizerCreatorRef(lex::L2AC_Identifier(clks::g_keywordInfo)),
	lex::Lexer2::AnalizerCreatorRef(lex::L2AC_Number()),
	lex::Lexer2::AnalizerCreatorRef(lex::L2AC_String()),
	lex::Lexer2::AnalizerCreatorRef(lex::L2AC_Symbols(clks::g_symbolInfo))
})
, m_parser(clks::g_states) {
	
}


lex::LR_NodePtr CLikeScriptCompiler::parse(const FilePath &file) {
	FileStream *fs = FileManager::instance().open(file);
	if (!fs) {
		VN_LOG_ERROR("open file \"" << file.fullPath() << "\" failed!");
		return nullptr;
	}
	size_t size = (size_t)fs->size();
	if (!size) {
		fs->close();
		return nullptr;
	} else {
		c8 *ptr = vnmalloc(c8, size);
		if (fs->read(ptr, size) != size) {
			VN_LOG_ERROR("read file \"" << file.fullPath() << "\" failed!");
			fs->close();
			vnfree(ptr);
			return nullptr;
		}
		fs->close();
		
		c8 *txt = ptr;
		// utf8 bom
		if (strncmp(txt, "\xEF\xBB\xBF", 3) == 0) {
			txt += 3;
			size -= 3;
		}
		
		lex::Token::FilePtr tf = vnnew lex::Token::File();
		tf->name = file.fullPath();
		auto ret = this->parse(txt, size, tf);
		vnfree(ptr);
		return std::move(ret);
	}
	return nullptr;
}

lex::LR_NodePtr CLikeScriptCompiler::parse(const c8 *text, size_t length, lex::Token::FilePtr file) {
	lex::TokenList tokens;
	if (m_lexer.scan(text, length, file, tokens) < 0) {
		VN_LOG_ERROR("lexical error: " << m_lexer.error());
		return nullptr;
	}
	tokens.push_back(vnnew lex::Token(file));
	for (auto &token : tokens) {
		lex::LR_Parser::Result ret = m_parser.input(token);
		if (ret == lex::LR_Parser::kAccept) {
			break;
		}
		if (ret == lex::LR_Parser::kFailed) {
			VN_LOG_ERROR("syntax error: " << (file ? file->name.c_str() : "") << "(" << token->row + 1 << "): " << token->text);
			break;
		}
	}
	lex::LR_NodePtr root = m_parser.result();
	m_parser.reset();
	return std::move(root);
	
}


_vn_end
