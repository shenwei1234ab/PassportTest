//
//  vnLexer2Analizers.h
//  vnlib
//
//  Created by Wayne on 6/27/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef vnlib_vnLexer2Analizers_h
#define vnlib_vnLexer2Analizers_h

#include "vnLexer2.h"
#include "vnLexLR_State.h"

_vn_begin

namespace lex {
    
    class L2AC_Space : public Lexer2::AnalizerCreator {
    public:
        virtual void operator ()(std::vector<std::unique_ptr<Lexer2::Analizer>> &) const;
    };
    
    class L2AC_Comment : public Lexer2::AnalizerCreator {
    public:
        virtual void operator ()(std::vector<std::unique_ptr<Lexer2::Analizer>> &) const;
    };
    
    class L2AC_Identifier : public Lexer2::AnalizerCreator {
    public:
        L2AC_Identifier() = default;
        L2AC_Identifier(const LR_Terminal *keywords) : m_keywords(keywords) {}
        
        virtual void operator ()(std::vector<std::unique_ptr<Lexer2::Analizer>> &) const;
        
    protected:
        const LR_Terminal *m_keywords = nullptr;
    };
    
    class L2AC_String : public Lexer2::AnalizerCreator {
    public:
        virtual void operator ()(std::vector<std::unique_ptr<Lexer2::Analizer>> &) const;
    };
    
    class L2AC_Symbols : public Lexer2::AnalizerCreator {
    public:
        L2AC_Symbols() = default;
        L2AC_Symbols(const LR_Terminal *symbols) : m_symbols(symbols) {}
        
        virtual void operator ()(std::vector<std::unique_ptr<Lexer2::Analizer>> &) const;
        
    protected:
        const LR_Terminal *m_symbols = nullptr;
    };
    
    class L2AC_Number : public Lexer2::AnalizerCreator {
    public:
        virtual void operator ()(std::vector<std::unique_ptr<Lexer2::Analizer>> &) const;
    };
}

_vn_end

#endif
