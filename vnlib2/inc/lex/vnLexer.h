//
//  vnLexer.h
//  vnlib
//
//  Created by Wayne on 15/5/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnLexer_h
#define vnlib_vnLexer_h

#include "vnLexToken.h"
#include <vector>
#include <initializer_list>

_vn_begin

namespace lex {
    
    class Lexer : public RefCounted {
    public:
        struct Context;
        class Analizer : public RefCounted {
        public:
            enum Result {
                kError = 0,
                kAccept,
                kRefuse
            };
            virtual Result process(Context &context) = 0;
        };
        typedef RefCountedPtr<Analizer> AnalizerPtr;
        
        struct Context {
            Token::FilePtr file;
            char chr = 0;
            int row = 0;
            int token_row = 0;
            str8 buf;
            int state = 0;
            long data = 0;
            TokenList *tokens = 0;
        };
        
        Lexer() = default;
        Lexer(std::initializer_list<AnalizerPtr>);
        
        void add(const AnalizerPtr &analizer);
        void add(AnalizerPtr &&analizer);
        
        ssize_t scan(const c8 *text, size_t length, const Token::FilePtr &file, TokenList &result);
        
        const str8 & error() const;
        
    private:
        typedef std::vector<AnalizerPtr> Analizers;
        Analizers m_analizers;
        void _makeErrorMsg(const Context &ctx);
        
        str8 m_errorMsg;
    };
    
}

_vn_end

#endif
