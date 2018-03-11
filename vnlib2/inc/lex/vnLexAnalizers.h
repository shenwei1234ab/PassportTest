//
//  vnLexAnalizers.h
//  vnlib
//
//  Created by Wayne on 15/5/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnLexAnalizers_h
#define vnlib_vnLexAnalizers_h

#include "vnLexer.h"
#include "vnLexLR_State.h"

#include <map>

_vn_begin

namespace lex {
    
    class LA_Space : public Lexer::Analizer {
    public:
        virtual Result process(Lexer::Context &context);
    };
    
    class LA_Identifier : public Lexer::Analizer {
    public:
        LA_Identifier();
        LA_Identifier(const LR_Terminal *keywords);
        virtual Result process(Lexer::Context &context);
        static bool isDigit(c8 chr);
        static bool isLetter(c8 chr);
        bool isKeyword(const std::string &s, u32 &outId);
        void registerKeyword(const c8 *keyword, u32 kwId);
    private:
        typedef std::map<str8, u32> KeywordMap;
        KeywordMap m_keywords;
    };
    
    class LA_Character : public Lexer::Analizer {
    public:
        virtual Result process(Lexer::Context &context);
    };
    
    class LA_CharacterEx : public Lexer::Analizer {
    public:
        virtual Result process(Lexer::Context &context);
    };
    
    class LA_String : public Lexer::Analizer {
    public:
        virtual Result process(Lexer::Context &context);
        static str8 convert(const str8 &s);
        static str8 convert(const c8 *s, size_t length);
    };
    
    class LA_Number : public Lexer::Analizer {
    public:
        virtual Result process(Lexer::Context &context);
    };
    
    class LA_Comment : public Lexer::Analizer {
    public:
        virtual Result process(Lexer::Context &context);
    };
    
    class LA_Symbol : public Lexer::Analizer {
    public:
        LA_Symbol();
        LA_Symbol(const LR_Terminal *symbols);
        virtual ~LA_Symbol();
        virtual Result process(Lexer::Context &context);
        void registerSymbol(const c8 *symbol, u32 sId);
    private:
        struct Symbol;
        typedef std::map<c8, Symbol *> SymbolMap;
        struct Symbol {
            u32 sId = 0;
            SymbolMap symbols;
            ~Symbol();
        };
        
        SymbolMap m_symbols;
    };

    
}

_vn_end


#endif
