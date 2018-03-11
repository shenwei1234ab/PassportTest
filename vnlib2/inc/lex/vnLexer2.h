//
//  vnLexer2.h
//  vnlib
//
//  Created by Wayne on 6/27/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef vnlib_vnLexer2_h
#define vnlib_vnLexer2_h

#include "vnLexToken.h"
#include <initializer_list>
#include <vector>
#include <memory>

_vn_begin

namespace lex {
    
    class Lexer2 : public AllocatedObject {
    public:
        
        struct Context {
            Token::FilePtr file;
            int row = 0, col = 0;
            str8 buffer;
        };
        
        class Analizer : public AllocatedObject {
        public:
            enum InputResult {
                kError = 0,
                kAccept,
                kRefuse
            };
            virtual ~Analizer() = default;
            virtual Analizer * clone() const = 0;
            virtual void reset() = 0;
            virtual bool begin(c8 chr) = 0;
            virtual InputResult input(c8 chr) = 0;
            virtual bool end(const Context &ctx, TokenList &tokens, int &row, int &col) = 0;
            virtual str8 makeErrorMsg(const Context &ctx, c8 chr) = 0;
            size_t processed() const { return m_processed; }
            
        protected:
            size_t m_processed = 0;
        };
        
        class AnalizerCreator : public AllocatedObject {
        public:
            virtual ~AnalizerCreator() = default;
            virtual void operator ()(std::vector<std::unique_ptr<Analizer>> &) const = 0;
        };
		
		struct AnalizerCreatorRef {
			const AnalizerCreator &creator;
			AnalizerCreatorRef(const AnalizerCreator &_creator) : creator(_creator) {}
		};
        
        Lexer2(std::initializer_list<AnalizerCreatorRef>);
        Lexer2(const Lexer2 &);
        Lexer2(Lexer2 &&);
        
        void reset(Token::FilePtr file, int row = 0, int col = 0);
        bool input(c8 chr);
        bool end(TokenList &result, int &row, int &col);
        
        ssize_t scan(const c8 *text, size_t length, Token::FilePtr file, TokenList &result);
        const str8 & error() const;
        
    protected:
        std::vector<std::unique_ptr<Analizer>> m_analizers;
        Context m_ctx;
        TokenList m_tokens;
        str8 m_errorMsg;
        std::vector<Analizer *> m_open, m_close, m_error;
    };
}

_vn_end

#endif
