//
//  vnLexer2Analizers.cpp
//  vnlib
//
//  Created by Wayne on 6/27/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "vnLexer2Analizers.h"

#include <sstream>
#include <map>

_vn_begin

namespace lex {
    
    class L2A_Space : public Lexer2::Analizer {
    public:
        virtual Analizer * clone() const {
            return vnnew L2A_Space();
        }
        
        virtual void reset() {
            m_row = 0;
            m_col = 0;
            m_processed = 0;
        }
        
        virtual bool begin(c8 chr) {
            switch (chr) {
                case ' ':
                case '\t':
                case '\r':
                    m_col = 1;
                    m_processed = 1;
                    return true;
                    
                case '\n':
                    m_row = 1;
                    m_processed = 1;
                    return true;
            }
            return false;
        }
        
        virtual InputResult input(c8 chr) {
            switch (chr) {
                case ' ':
                case '\t':
                case '\r':
                    ++m_col;
                    break;
                    
                case '\n':
                    m_col = 0;
                    ++m_row;
                    break;
                    
                default:
                    return kRefuse;
            }
            ++m_processed;
            return kAccept;
        }
        
        virtual bool end(const Lexer2::Context &ctx, TokenList &, int &row, int &col) {
            if (m_row) {
                row = ctx.row + m_row;
                col = m_col;
            } else {
                row = ctx.row;
                col = ctx.col + m_col;
            }
            return true;
        }
        
        virtual str8 makeErrorMsg(const Lexer2::Context &ctx, c8 chr) {
            return {};
        }
        
    private:
        int m_row = 0;
        int m_col = 0;
    };
    
    void L2AC_Space::operator ()(std::vector<std::unique_ptr<Lexer2::Analizer>> &analizers) const {
        analizers.push_back(std::unique_ptr<Lexer2::Analizer>(vnnew L2A_Space()));
    }
    
    
    class L2A_Comment : public Lexer2::Analizer {
    public:
        virtual Analizer * clone() const {
            return vnnew L2A_Comment();
        }
        
        virtual void reset() {
            m_row = 0;
            m_col = 0;
            m_state = kInitial;
            m_processed = 0;
        }
        
        virtual bool begin(c8 chr) {
            if (chr == '/') {
                m_processed = 1;
                m_col = 1;
                return true;
            }
            return false;
        }
        
        virtual InputResult input(c8 chr) {
            switch (m_state) {
                case kInitial:
                    if (chr == '/') {
                        m_state = kType1;
                        m_col = 2;
                        m_processed = 2;
                        return kAccept;
                    } else if (chr == '*') {
                        m_col = 2;
                        m_state = kType2;
                        m_processed = 2;
                        return kAccept;
                    }
                    return kError;
                    
                case kType1:
                    if (chr == '\n') {
                        ++m_row;
                        m_col = 0;
                        m_state = kFinish;
                    } else {
                        ++m_col;
                    }
                    ++m_processed;
                    return kAccept;
                    
                case kFinish:
                    if (chr == '/') {
                        ++m_col;
                        ++m_processed;
                        m_state = kCheck;
                        return kAccept;
                    }
                    break;
                    
                case kCheck:
                    if (chr == '/') {
                        m_state = kType1;
                        ++m_col;
                        ++m_processed;
                        return kAccept;
                    } else if (chr == '*') {
                        ++m_col;
                        m_state = kType2;
                        ++m_processed;
                        return kAccept;
                    }
                    break;
                    
                case kType2:
                    ++m_processed;
                    if (chr == '*') {
                        ++m_col;
                        m_state = kType2_1;
                    } else if (chr == '\n') {
                        m_col = 0;
                        ++m_row;
                    }
                    return kAccept;
                    
                case kType2_1:
                    ++m_processed;
                    if (chr == '/') {
                        ++m_col;
                        m_state = kFinish;
                    } else if (chr == '\n') {
                        m_col = 0;
                        ++m_row;
                        m_state = kType2;
                    } else if (chr != '*') {
                        ++m_col;
                        m_state = kType2;
                    } else {
                        ++m_col;
                    }
                    return kAccept;
            }
            return kRefuse;
        }
        
        virtual bool end(const Lexer2::Context &ctx, TokenList &, int &row, int &col) {
            if (m_row) {
                row = ctx.row + m_row;
                col = m_col;
            } else {
                row = ctx.row;
                col = ctx.col + m_col;
            }
            return true;
        }
        
        virtual str8 makeErrorMsg(const Lexer2::Context &ctx, c8 chr) {
            return {};
        }
        
    private:
        int m_row = 0;
        int m_col = 0;
        enum {
            kInitial = 0,
            kType1,
            kFinish,
            kCheck,
            kType2,
            kType2_1
        } m_state = kInitial;
    };
    
    void L2AC_Comment::operator ()(std::vector<std::unique_ptr<Lexer2::Analizer>> &analizers) const {
        analizers.push_back(std::unique_ptr<Lexer2::Analizer>(vnnew L2A_Comment()));
    }
    
    class L2A_Identifier : public Lexer2::Analizer {
    public:
        L2A_Identifier(const LR_Terminal *keywords) {
            if (keywords) {
                for (; keywords->name; ++keywords) {
                    m_keywords[keywords->name] = keywords->identity;
                }
            }
        }
        
        L2A_Identifier(const L2A_Identifier &rhs)
        : m_keywords(rhs.m_keywords) {}
        
        virtual Analizer * clone() const {
            return vnnew L2A_Identifier(*this);
        }
        
        virtual void reset() {
            m_processed = 0;
        }
        
        static bool isLetter(c8 chr) {
            return (chr >= 'A' && chr <= 'Z') || (chr >= 'a' && chr <= 'z') || chr < 0;
        }
        
        static bool isDigit(c8 chr) {
            return chr >= '0' && chr <= '9';
        }
        
        bool isKeyword(const std::string &s, unsigned int &outId) {
            auto it = m_keywords.find(s);
            if (it == m_keywords.end()) {
                return false;
            }
            outId = it->second;
            return true;
        }
        
        virtual bool begin(c8 chr) {
            if (chr == '_' || isLetter(chr)) {
                m_processed = 1;
                return true;
            }
            return false;
        }
        
        virtual InputResult input(c8 chr) {
            if (chr == '_' || isLetter(chr) || isDigit(chr)) {
                ++m_processed;
                return kAccept;
            }
            return kRefuse;
        }
        
        virtual bool end(const Lexer2::Context &ctx, TokenList &tokens, int &row, int &col) {
            Token *token = vnnew Token(ctx.file);
            token->text = ctx.buffer.substr(0, m_processed);
            if (isKeyword(token->text, token->int32)) {
                token->set(Token::kKeyword);
            } else {
                token->set(Token::kIdentifier);
            }
            token->row = ctx.row;
            token->col = ctx.col;
            
            tokens.push_back(token);
            
            row = ctx.row;
            col = ctx.col + (int)m_processed;
            return true;
        }
        
        virtual str8 makeErrorMsg(const Lexer2::Context &ctx, c8 chr) {
            return {};
        }
        
    private:
        std::map<str8, u32> m_keywords;
    };
    
    void L2AC_Identifier::operator ()(std::vector<std::unique_ptr<Lexer2::Analizer>> &analizers) const {
        analizers.push_back(std::unique_ptr<Lexer2::Analizer>(vnnew L2A_Identifier(m_keywords)));
    }
    
    class L2A_String : public Lexer2::Analizer {
    public:
        
        virtual Analizer * clone() const {
            return vnnew L2A_String();
        }
        
        virtual void reset() {
            m_processed = 0;
            m_state = kDefault;
            m_data = 0;
            m_buffer.clear();
        }
        
        
        virtual bool begin(c8 chr) {
            if (chr == '\"') {
                m_processed = 1;
                return true;
            }
            return false;
        }
        
        virtual InputResult input(c8 chr) {
            switch (m_state) {
                case kDefault:
                    if (chr == '\\') {
                        m_state = kS;
                    } else if (chr == '\"') {
                        m_state = kFinish;
                    } else if (chr == '\r' || chr == '\n') {
                        m_state = kInvalid;
                        return kError;
                    } else {
                        m_buffer.push_back(chr);
                    }
                    break;
                    
                case kS:
                    switch (chr) {
                        case '0':
                            m_buffer.push_back('\0');
                            break;
                        case 'b':
                            m_buffer.push_back('\b');
                            break;
                        case 't':
                            m_buffer.push_back('\t');
                            break;
                        case 'r':
                            m_buffer.push_back('\r');
                            break;
                        case 'n':
                            m_buffer.push_back('\n');
                            break;
                        case '\\':
                        case '\'':
                        case '\"':
                            m_buffer.push_back(chr);
                            break;
                        case 'x':
                        case 'X':
                            m_data = 0;
                            m_state = kX1;
                            break;
                            
                        default:
                            m_state = kInvalidS;
                            return kError;
                            
                    }
                    break;
                    
                case kX1:
                    if (chr >= '0' && chr <= '9') {
                        m_data = chr - '0';
                    } else if (chr >= 'A' && chr <= 'F') {
                        m_data = chr - 'A' + 10;
                    } else if (chr >= 'a' && chr <= 'f') {
                        m_data = chr - 'a' + 10;
                    } else {
                        m_state = kInvalidX;
                        return kError;
                    }
                    m_state = kX2;
                    break;
                    
                case kX2:
                    if (chr >= '0' && chr <= '9') {
                        m_data = m_data * 16 + chr - '0';
                    } else if (chr >= 'A' && chr <= 'F') {
                        m_data = m_data * 16 + chr - 'A' + 10;
                    } else if (chr >= 'a' && chr <= 'f') {
                        m_data = m_data * 16 + chr - 'a' + 10;
                    } else {
                        m_state = kInvalidX;
                        return kError;
                    }
                    m_buffer.push_back((c8)m_data);
                    m_state = kDefault;
                    break;
                    
                case kFinish:
                    return kRefuse;
                    
                case kInvalid:
                case kInvalidS:
                case kInvalidX:
                    return kError;
                    
            }
            ++m_processed;
            return kAccept;
        }
        
        virtual bool end(const Lexer2::Context &ctx, TokenList &tokens, int &row, int &col) {
            if (m_state != kFinish) {
                return false;
            }
            Token *token = vnnew Token(ctx.file);
            token->set(Token::kString, m_buffer);
            token->row = ctx.row;
            token->col = ctx.col;
            tokens.push_back(token);
            row = ctx.row;
            col = ctx.col + (int)m_processed;
            return true;
        }
        
        virtual str8 makeErrorMsg(const Lexer2::Context &ctx, c8 chr) {
            std::ostringstream oss;
            if (ctx.file) {
                oss << ctx.file->name.c_str();
            }
            oss << '(' << ctx.row + 1 << ':' << ctx.col + 1 << "): invalid string.";
            return oss.str();
        }
        
    private:
        int m_data = 0;
        str8 m_buffer;
        enum {
            kDefault = 0,
            kS,
            kX1,
            kX2,
            kFinish,
            kInvalid,
            kInvalidS,
            kInvalidX,
        } m_state = kDefault;
    };

    void L2AC_String::operator ()(std::vector<std::unique_ptr<Lexer2::Analizer>> &analizers) const {
        analizers.push_back(std::unique_ptr<Lexer2::Analizer>(vnnew L2A_String()));
    }
    
    class L2A_Symbol : public Lexer2::Analizer {
    public:
        L2A_Symbol(str8 symbol, u32 sId) : m_symbol(std::move(symbol)), m_id(sId) {}
        
        virtual Analizer * clone() const {
            return vnnew L2A_Symbol(m_symbol, m_id);
        }
        
        virtual void reset() {
            m_processed = 0;
        }
        
        virtual bool begin(c8 chr) {
            if (m_symbol.front() != chr) {
                return false;
            }
            m_processed = 1;
            return true;
        }
        
        virtual InputResult input(c8 chr) {
            if (m_processed == m_symbol.length()) {
                return kRefuse;
            }
            if (m_symbol[m_processed] != chr) {
                return kError;
            }
            ++m_processed;
            return kAccept;
        }
        
        virtual bool end(const Lexer2::Context &ctx, TokenList &tokens, int &row, int &col) {
            if (m_processed != m_symbol.length()) {
                return false;
            }
            Token *token = vnnew Token(ctx.file);
            token->row = ctx.row;
            token->col = ctx.col;
            token->set(Token::kSymbol, m_symbol).int32 = m_id;
            tokens.push_back(token);
            
            row = ctx.row;
            col = ctx.col + (int)m_processed;
            
            return true;
        }
        
        virtual str8 makeErrorMsg(const Lexer2::Context &ctx, c8 chr) {
            std::ostringstream oss;
            if (ctx.file) {
                oss << ctx.file->name.c_str();
            }
            oss << '(' << ctx.row + 1 << ':' << ctx.col + 1 << "): invalid symbol \"" << ctx.buffer.c_str() << "\".";
            return oss.str();
        }
        
    private:
        str8 m_symbol;
        u32 m_id = 0;
    };
    
    void L2AC_Symbols::operator ()(std::vector<std::unique_ptr<Lexer2::Analizer>> &analizers) const {
        if (!m_symbols) {
            return ;
        }
        for (auto p = m_symbols; p->name; ++p) {
            analizers.push_back(std::unique_ptr<Lexer2::Analizer>(vnnew L2A_Symbol(p->name, p->identity)));
        }
    }
    
    
    class L2A_Number : public Lexer2::Analizer {
    public:
        virtual Analizer * clone() const {
            return vnnew L2A_Number();
        }
        
        virtual void reset() {
            m_state = kZero;
            m_processed = 0;
        }
        
        virtual bool begin(c8 chr) {
            if (chr < '0' || chr > '9') {
                return false;
            }
            m_processed = 1;
            if (chr != '0') {
                m_state = kNormal;
            }
            return true;
        }
        
        virtual InputResult input(c8 chr) {
            switch (m_state) {
                case kZero:
                    if (chr == 'x' || chr == 'X') {
                        m_state = kHex1;
                    } else if (chr == '.') {
                        m_state = kFloat;
                    } else if (chr >= '0' && chr <= '9') {
                        m_state = kNormal;
                    } else if (chr == 'l' || chr == 'L') {
                        m_state = kFinish_Long;
                    } else if (chr == 'e' || chr == 'E') {
                        m_state = kFloatE1;
                    } else if (L2A_Identifier::isLetter(chr) || chr == '_') {
                        m_state = kInvalid;
                        return kError;
                    } else {
                        m_state = kFinish_Int;
                        return kRefuse;
                    }
                    break;
                    
                case kNormal:
                    if (chr == '.') {
                        m_state = kFloat;
                    } else if (chr == 'l' || chr == 'L') {
                        m_state = kFinish_Long;
                    } else if (chr == 'e' || chr == 'E') {
                        m_state = kFloatE1;
                    } else if (L2A_Identifier::isLetter(chr) || chr == '_') {
                        m_state = kInvalid;
                        return kError;
                    } else if (chr < '0' || chr > '9') {
                        m_state = kFinish_Int;
                        return kRefuse;
                    }
                    break;
                    
                case kHex1:
                    if ((chr < '0' || chr > '9') && (chr < 'A' || chr > 'F') && (chr < 'a' || chr > 'f')) {
                        m_state = kInvalidHex;
                        return kError;
                    }
                    m_state = kHex2;
                    break;
                    
                case kHex2:
                    if (chr == 'l' || chr == 'L') {
                        m_state = kFinish_LongHex;
                    } else if ((chr < '0' || chr > '9') && (chr < 'A' || chr > 'F') && (chr < 'a' || chr > 'f')) {
                        if (L2A_Identifier::isLetter(chr) || chr == '_') {
                            m_state = kInvalid;
                            return kError;
                        }
                        m_state = kFinish_Hex;
                        return kRefuse;
                    }
                    break;
                    
                case kFloat:
                    if (chr == 'f' || chr == 'F') {
                        m_state = kFinish_Float;
                    } else if (chr == 'e' || chr == 'E') {
                        m_state = kFloatE1;
                    } else if (L2A_Identifier::isLetter(chr) || chr == '_') {
                        m_state = kInvalid;
                        return kError;
                    } else if (chr < '0' || chr > '9') {
                        m_state = kFinish_Double;
                        return kRefuse;
                    }
                    break;
                    
                case kFloatE1:
                    if (chr == '+' || chr == '-') {
                        m_state = kFloatE2;
                    } else if (chr >= '0' && chr <= '9') {
                        m_state = kFloatE3;
                    } else {
                        m_state = kInvalidFloatE;
                        return kError;
                    }
                    break;
                
                case kFloatE2:
                    if (chr < '0' || chr > '9') {
                        m_state = kInvalidFloatE;
                        return kError;
                    }
                    m_state = kFloatE3;
                    break;
                    
                case kFloatE3:
                    if (chr == 'f' || chr == 'F') {
                        m_state = kFinish_FloatE;
                    } else if (L2A_Identifier::isLetter(chr) || chr == '_') {
                        m_state = kInvalid;
                        return kError;
                    } else if (chr < '0' || chr > '9') {
                        m_state = kFinish_DoubleE;
                        return kRefuse;
                    }
                    break;
                    
                case kFinish_Float:
                case kFinish_FloatE:
                case kFinish_LongHex:
                case kFinish_Long:
                    if (L2A_Identifier::isDigit(chr) || L2A_Identifier::isLetter(chr) || chr == '_') {
                        m_state = kInvalid;
                        return kError;
                    }
                    return kRefuse;
                    
                case kFinish_DoubleE:
                case kFinish_Double:
                case kFinish_Int:
                case kFinish_Hex:
                    
                case kInvalid:
                case kInvalidHex:
                case kInvalidFloatE:
                    
                    return kError;
                    
                
            }
            ++m_processed;
            return kAccept;
        }
        
        virtual bool end(const Lexer2::Context &ctx, TokenList &tokens, int &row, int &col) {
            Token *p = nullptr;
            switch (m_state) {
                case kZero:
                    p = _createToken(ctx);
                    p->set(Token::kInteger).int32 = 0;
                    break;
                    
                case kNormal:
                case kFinish_Int:
                    p = _createToken(ctx);
                    p->set(Token::kInteger);
                    sscanf(p->text.c_str(), "%d", &p->int32);
                    break;
                    
                case kFinish_Long:
                    p = _createToken(ctx);
                    p->set(Token::kLongInteger);
                    sscanf(p->text.c_str(), "%lld", &p->int64);
                    break;
                    
                case kHex2:
                case kFinish_Hex:
                    p = _createToken(ctx);
                    p->set(Token::kInteger);
                    sscanf(p->text.c_str(), "%x", &p->int32);
                    break;
                    
                case kFinish_LongHex:
                    p = _createToken(ctx);
                    p->set(Token::kInteger);
                    sscanf(p->text.c_str(), "%llx", &p->int64);
                    break;
                    
                case kFinish_Float:
                    p = _createToken(ctx);
                    p->set(Token::kFloat);
                    sscanf(p->text.c_str(), "%f", &p->float32);
                    break;
                    
                case kFloat:
                case kFinish_Double:
                    p = _createToken(ctx);
                    p->set(Token::kDouble);
                    sscanf(p->text.c_str(), "%lf", &p->float64);
                    break;
                    
                case kFinish_FloatE:
                    p = _createToken(ctx);
                    p->set(Token::kFloat);
                    sscanf(p->text.c_str(), "%e", &p->float32);
                    break;
                    
                case kFloatE3:
                case kFinish_DoubleE:
                    p = _createToken(ctx);
                    p->set(Token::kFloat);
                    sscanf(p->text.c_str(), "%le", &p->float64);
                    break;
                    
                
                    
                default:
                    return false;
            }
            tokens.push_back(p);
            row = ctx.row;
            col = ctx.col + (int)m_processed;
            return true;
        }
        
        virtual str8 makeErrorMsg(const Lexer2::Context &ctx, c8 chr) {
            std::ostringstream oss;
            if (ctx.file) {
                oss << ctx.file->name.c_str();
            }
            oss << '(' << ctx.row + 1 << ':' << ctx.col + 1 << "): invalid number \"" << ctx.buffer.c_str() << "\".";
            return oss.str();
        }
        
    private:
        enum {
            kZero = 0,
            kNormal,
            kHex1,
            kHex2,
            kFloat,
            kFloatE1,
            kFloatE2,
            kFloatE3,
            kFinish_Int,
            kFinish_Long,
            kFinish_Hex,
            kFinish_LongHex,
            kFinish_Float,
            kFinish_FloatE,
            kFinish_Double,
            kFinish_DoubleE,
            kInvalid,
            kInvalidHex,
            kInvalidFloatE
        } m_state = kZero;
        
        Token * _createToken(const Lexer2::Context &ctx) {
            auto p = vnnew Token();
            p->text = ctx.buffer.substr(0, m_processed);
            p->row = ctx.row;
            p->col = ctx.col;
            return p;
        }
    };
    
    void L2AC_Number::operator ()(std::vector<std::unique_ptr<Lexer2::Analizer>> &analizers) const {
        analizers.push_back(std::unique_ptr<Lexer2::Analizer>(vnnew L2A_Number()));
    }
}

_vn_end
