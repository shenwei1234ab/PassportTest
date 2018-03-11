//
//  vnLexer.cpp
//  vnlib
//
//  Created by Wayne on 15/5/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnLexer.h"
#include "vnAssert.h"

#include <sstream>

_vn_begin

namespace lex {
    
    Lexer::Lexer(std::initializer_list<AnalizerPtr> args)
    : m_analizers(args) {
        
    }
    
    void Lexer::add(const AnalizerPtr &analizer) {
        vnassert(analizer);
        m_analizers.push_back(analizer);
    }
    
    void Lexer::add(AnalizerPtr &&analizer) {
        vnassert(analizer);
        m_analizers.push_back(analizer);
    }
    
    ssize_t Lexer::scan(const c8 *text, size_t length, const Token::FilePtr &file, TokenList &result) {
        result.clear();
        
        Context ctx;
        ctx.file = file;
        ctx.tokens = &result;
        
        const c8 *ptr;
        Analizers::iterator it = m_analizers.end();
        for (ptr = text; length && *ptr; ++ptr, --length) {
            ctx.chr = *ptr;
            if (it != m_analizers.end()) {
                Analizer::Result ret = (*it)->process(ctx);
                if (ret == Analizer::kAccept) {
                    continue;
                }
                if (ret == Analizer::kError) {
                    _makeErrorMsg(ctx);
                    return -1;
                }
                ctx.state = 0;
            }
            for (it = m_analizers.begin(); it != m_analizers.end(); ++it) {
                Analizer::Result ret = (*it)->process(ctx);
                if (ret == Analizer::kAccept) {
                    ctx.token_row = ctx.row;
                    break;
                }
            }
            if (it == m_analizers.end()) {
                _makeErrorMsg(ctx);
                return -1;
            }
        }
		if (it == m_analizers.end()) {
			return -1;
		}
        ctx.chr = 0;
        if ((*it)->process(ctx) == Analizer::kError) {
            _makeErrorMsg(ctx);
            return -1;
        }
        m_errorMsg.clear();
        return (ssize_t)(ptr - text);
    }
    
    const str8 & Lexer::error() const {
        return m_errorMsg;
    }
    
    void Lexer::_makeErrorMsg(const Context &ctx) {
        std::ostringstream oss;
        if (ctx.file) {
            oss << ctx.file->name.c_str();
        }
        oss << '(' << ctx.row + 1 << "): " << ctx.buf.c_str();
        m_errorMsg = oss.str();
    }

    
}



_vn_end
