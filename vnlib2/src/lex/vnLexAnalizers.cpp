//
//  vnLexAnalizers.cpp
//  vnlib
//
//  Created by Wayne on 15/5/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnLexAnalizers.h"
#include "vnUnicode.h"

_vn_begin

namespace lex {
    
    Lexer::Analizer::Result LA_Space::process(Lexer::Context &context) {
        if (context.state == 0) {
            switch (context.chr) {
                case '\n':
                    ++context.row;
                case ' ':
                case '\t':
                    return kAccept;
                    
                case '\r':
                    context.state = 1;
                    return kAccept;
            }
        } else {
            ++context.row;
            switch (context.chr) {
                case '\n':
                case ' ':
                case '\t':
                    context.state = 0;
                case '\r':
                    return kAccept;
            }
        }
        return kRefuse;
    }
    
    Lexer::Analizer::Result LA_Comment::process(Lexer::Context &context) {
        switch (context.state) {
            case 0:
                if (context.chr == '/') {
                    context.state = 1;
                    return kAccept;
                }
                break;
                
            case 1:
                switch (context.chr) {
                    case '/':
                        context.state = 2;
                        return kAccept;
                    case '*':
                        context.state = 4;
                        return kAccept;
                }
                // TODO:
                return kError;
                
            case 2:
                switch (context.chr) {
                    case 0:
                        return kRefuse;
                    case '\n':
                        ++context.row;
                        context.state = 0;
                        break;
                    case '\r':
                        context.state = 3;
                        break;
                }
                return kAccept;
                
            case 3:
                ++context.row;
                context.state = 0;
                if (context.chr == '\n') {
                    return kAccept;
                }
                return kRefuse;
                
            case 4:
                switch (context.chr) {
                    case 0:
                        // TODO: EOF!
                        return kError;
                    case '\n':
                        ++context.row;
                        break;
                    case '\r':
                        context.state = 5;
                        break;
                    case '*':
                        context.state = 6;
                        break;
                }
                return kAccept;
                
            case 5:
                ++context.row;
                if (context.chr == 0) {
                    // TODO: EOF!
                    return kError;
                }
                if (context.chr != '\r') {
                    context.state = 4;
                }
                return kAccept;
                
            case 6:
                switch (context.chr) {
                    case 0:
                        // TODO: EOF!
                        return kError;
                    case '/':
                        context.state = 0;
                    case '*':
                        return kAccept;
                }
                context.state = 4;
                return kAccept;
        }
        
        return kRefuse;
    }
    
    LA_Identifier::LA_Identifier() {
        
    }
    
    LA_Identifier::LA_Identifier(const LR_Terminal *keywords) {
        for (; keywords->name; ++keywords) {
            m_keywords[keywords->name] = keywords->identity;
        }
    }
    
    Lexer::Analizer::Result LA_Identifier::process(Lexer::Context &context) {
        switch (context.state) {
            case 0:
                if (context.chr == '_' || isLetter(context.chr)) {
                    context.buf.clear();
                    context.buf.push_back(context.chr);
                    context.state = 1;
                    return kAccept;
                }
                break;
                
            case 1:
                if (context.chr == '_' || isLetter(context.chr) || isDigit(context.chr)) {
                    context.buf.push_back(context.chr);
                    return kAccept;
                } else {
                    Token *token = vnnew Token(context.file);
                    if (isKeyword(context.buf, token->int32)) {
                        token->set(Token::kKeyword, context.buf);
                    } else {
                        token->set(Token::kIdentifier, context.buf);
                    }
                    token->row = context.token_row;
                    context.tokens->push_back(token);
                }
                break;
                
        }
        return kRefuse;
    }
    
    bool LA_Identifier::isDigit(c8 chr) {
        return chr >= '0' && chr <= '9';
    }
    
    bool LA_Identifier::isLetter(c8 chr) {
        return (chr >= 'A' && chr <= 'Z') || (chr >= 'a' && chr <= 'z') || chr < 0;
    }
    
    bool LA_Identifier::isKeyword(const std::string &s, unsigned int &outId) {
        KeywordMap::iterator it = m_keywords.find(s);
        if (it == m_keywords.end()) {
            return false;
        }
        outId = it->second;
        return true;
    }
    
    void LA_Identifier::registerKeyword(const char *keyword, unsigned int kwId) {
        m_keywords[keyword] = kwId;
    }
    
    Lexer::Analizer::Result LA_String::process(Lexer::Context &context) {
        switch (context.state) {
            case 0:
                if (context.chr == '\"') {
                    context.state = 1;
                    context.buf.clear();
                    return kAccept;
                }
                break;
                
            case 1:
                switch (context.chr) {
                    case '\\':
                        context.state = 2;
                        break;
                    case '\"':
					{
						Token *token = vnnew Token(context.file);
						token->set(Token::kString, context.buf);
						token->row = context.token_row;
						context.tokens->push_back(token);
						context.state = 0;
					}
                        break;
                    case 0:
                        // TODO: EOF!
                        return kError;
                    case '\r':
                    case '\n':
                        // TODO: NEW LINE!
                        return kError;
                        
                    default:
                        context.buf.push_back(context.chr);
                        break;
                }
                return kAccept;
            case 2:
                switch (context.chr) {
                    case '0':
                        context.buf.push_back('\0');
                        break;
                    case 'b':
                        context.buf.push_back('\b');
                        break;
                    case 't':
                        context.buf.push_back('\t');
                        break;
                    case 'r':
                        context.buf.push_back('\r');
                        break;
                    case 'n':
                        context.buf.push_back('\n');
                        break;
                    case '\\':
                    case '\'':
                    case '\"':
                    case '/':
                        context.buf.push_back(context.chr);
                        break;
                    case 'x':
                    case 'X':
                        context.data = 0;
                        context.state = 3;
                        return kAccept;
                        
                    case 'u':
                        context.data = 0;
                        context.state = 6;
                        return kAccept;
                        
                    case 0:
                        // TODO: EOF!
                        return kError;
                        
                    default:
                        // TODO:
                        return kError;
                }
                context.state = 1;
                return kAccept;
            case 3:
                if (context.chr >= '0' && context.chr <= '9') {
                    context.data = context.data * 16 + context.chr - '0';
                } else if (context.chr >= 'A' && context.chr <= 'F') {
                    context.data = context.data * 16 + context.chr - 'A' + 10;
                } else if (context.chr >= 'a' && context.chr <= 'f') {
                    context.data = context.data * 16 + context.chr - 'a' + 10;
                } else {
                    // TODO:
                    return kError;
                }
                context.state = 4;
                return kAccept;
            case 4:
                if (context.chr >= '0' && context.chr <= '9') {
                    context.data = context.data * 16 + context.chr - '0';
                } else if (context.chr >= 'A' && context.chr <= 'F') {
                    context.data = context.data * 16 + context.chr - 'A' + 10;
                } else if (context.chr >= 'a' && context.chr <= 'f') {
                    context.data = context.data * 16 + context.chr - 'a' + 10;
                } else {
                    context.buf.push_back((c8)context.data);
                    context.state = 1;
                    return process(context);
                }
                return kAccept;
                
            case 6:
                if (context.chr >= '0' && context.chr <= '9') {
                    context.data = context.data * 16 + context.chr - '0';
                } else if (context.chr >= 'A' && context.chr <= 'F') {
                    context.data = context.data * 16 + context.chr - 'A' + 10;
                } else if (context.chr >= 'a' && context.chr <= 'f') {
                    context.data = context.data * 16 + context.chr - 'a' + 10;
                } else {
                    // TODO:
                    return kError;
                }
                context.state = 7;
                return kAccept;
            case 7:
                if (context.chr >= '0' && context.chr <= '9') {
                    context.data = context.data * 16 + context.chr - '0';
                } else if (context.chr >= 'A' && context.chr <= 'F') {
                    context.data = context.data * 16 + context.chr - 'A' + 10;
                } else if (context.chr >= 'a' && context.chr <= 'f') {
                    context.data = context.data * 16 + context.chr - 'a' + 10;
                } else {
                    // TODO:
                    return kError;
                }
                context.state = 8;
                return kAccept;
            case 8:
                if (context.chr >= '0' && context.chr <= '9') {
                    context.data = context.data * 16 + context.chr - '0';
                } else if (context.chr >= 'A' && context.chr <= 'F') {
                    context.data = context.data * 16 + context.chr - 'A' + 10;
                } else if (context.chr >= 'a' && context.chr <= 'f') {
                    context.data = context.data * 16 + context.chr - 'a' + 10;
                } else {
                    // TODO:
                    return kError;
                }
                context.state = 9;
                return kAccept;
                
            case 9: {
                if (context.chr >= '0' && context.chr <= '9') {
                    context.data = context.data * 16 + context.chr - '0';
                } else if (context.chr >= 'A' && context.chr <= 'F') {
                    context.data = context.data * 16 + context.chr - 'A' + 10;
                } else if (context.chr >= 'a' && context.chr <= 'f') {
                    context.data = context.data * 16 + context.chr - 'a' + 10;
                } else {
                    // TODO:
                    return kError;
                }
                str8 t;
                Unicode::convert((const c16 *)&context.data, 1, t);
                if (t.empty()) {
                    // TODO:
                    return kError;
                }
                context.buf += t;
                context.state = 1;
                return kAccept;
            }
        }
        return kRefuse;
    }
    
    str8 LA_String::convert(const str8 &s) {
        return convert(s.c_str(), s.length());
    }
    
    str8 LA_String::convert(const c8 *s, size_t length) {
        str8 ret;
        ret.reserve(length + 2);
        ret.push_back('\"');
        while (length--) {
            switch (*s) {
                case '\0':
                    ret += "\\0";
                    break;
                case '\b':
                    ret += "\\b";
                    break;
                case '\t':
                    ret += "\\t";
                    break;
                case '\r':
                    ret += "\\r";
                    break;
                case '\n':
                    ret += "\\n";
                    break;
                case '\'':
                    ret += "\\\'";
                    break;
                case '\"':
                    ret += "\\\"";
                    break;
                case '\\':
                    ret += "\\\\";
                    break;
                    
                default:
                    ret.push_back(*s);
            }
            ++s;
        }
        ret.push_back('\"');
        return std::move(ret);
    }
    
    Lexer::Analizer::Result LA_Character::process(Lexer::Context &context) {
        switch (context.state) {
            case 0:
                if (context.chr == '\'') {
                    context.state = 1;
                    context.buf.clear();
                    return kAccept;
                }
                break;
                
            case 1:
                switch (context.chr) {
                    case '\\':
                        context.state = 2;
                        break;
                        
                    case 0:
                        // TODO: EOF!
                        return kError;
                    case '\r':
                    case '\n':
                        // TODO: NEW LINE!
                        return kError;
                        
                    default:
                        context.buf.push_back(context.chr);
                        context.state = 5;
                        break;
                }
                return kAccept;
            case 2:
                switch (context.chr) {
                    case '0':
                        context.buf.push_back('\0');
                        break;
                    case 'b':
                        context.buf.push_back('\b');
                        break;
                    case 't':
                        context.buf.push_back('\t');
                        break;
                    case 'r':
                        context.buf.push_back('\r');
                        break;
                    case 'n':
                        context.buf.push_back('\n');
                        break;
                    case '\\':
                    case '\'':
                    case '\"':
                        context.buf.push_back(context.chr);
                        break;
                    case 'x':
                    case 'X':
                        context.data = 0;
                        context.state = 3;
                        return kAccept;
                        
                    case 0:
                        // TODO: EOF!
                        return kError;
                        
                    default:
                        // TODO:
                        return kError;
                }
                context.state = 5;
                return kAccept;
            case 3:
                if (context.chr >= '0' && context.chr <= '9') {
                    context.data = context.data * 16 + context.chr - '0';
                } else if (context.chr >= 'A' && context.chr <= 'F') {
                    context.data = context.data * 16 + context.chr - 'A' + 10;
                } else if (context.chr >= 'a' && context.chr <= 'f') {
                    context.data = context.data * 16 + context.chr - 'a' + 10;
                } else {
                    // TODO:
                    return kError;
                }
                context.state = 4;
                return kAccept;
            case 4:
                if (context.chr >= '0' && context.chr <= '9') {
                    context.data = context.data * 16 + context.chr - '0';
                } else if (context.chr >= 'A' && context.chr <= 'F') {
                    context.data = context.data * 16 + context.chr - 'A' + 10;
                } else if (context.chr >= 'a' && context.chr <= 'f') {
                    context.data = context.data * 16 + context.chr - 'a' + 10;
                } else {
                    context.buf.push_back((c8)context.data);
                    context.state = 5;
                    return process(context);
                }
                return kAccept;
            case 5:
                if (context.chr == '\'')
                {
                    Token *token = vnnew Token(context.file);
                    token->set(Token::kCharacter, context.buf);
                    token->row = context.token_row;
                    context.tokens->push_back(token);
                    context.state = 0;
                    return kAccept;
                }
                // TODO:
                return kError;
        }
        return kRefuse;
    }
    
    Lexer::Analizer::Result LA_CharacterEx::process(Lexer::Context &context) {
        switch (context.state) {
            case 0:
                if (context.chr == '\'') {
                    context.state = 1;
                    context.buf.clear();
                    return kAccept;
                }
                break;
                
            case 1:
                switch (context.chr) {
                    case '\\':
                        context.state = 2;
                        break;
                    case '\'': {
                        Token *token = vnnew Token(context.file);
                        token->set(Token::kString, context.buf);
                        token->row = context.token_row;
                        context.tokens->push_back(token);
                        context.state = 0;
                    } break;
                    case 0:
                        // TODO: EOF!
                        return kError;
                    case '\r':
                    case '\n':
                        // TODO: NEW LINE!
                        return kError;
                        
                    default:
                        context.buf.push_back(context.chr);
                        break;
                }
                return kAccept;
            case 2:
                switch (context.chr) {
                    case '0':
                        context.buf.push_back('\0');
                        break;
                    case 'b':
                        context.buf.push_back('\b');
                        break;
                    case 't':
                        context.buf.push_back('\t');
                        break;
                    case 'r':
                        context.buf.push_back('\r');
                        break;
                    case 'n':
                        context.buf.push_back('\n');
                        break;
                    case '\\':
                    case '\'':
                    case '\"':
                    case '/':
                        context.buf.push_back(context.chr);
                        break;
                    case 'x':
                    case 'X':
                        context.data = 0;
                        context.state = 3;
                        return kAccept;
                        
                    case 'u':
                        context.data = 0;
                        context.state = 6;
                        return kAccept;
                        
                    case 0:
                        // TODO: EOF!
                        return kError;
                        
                    default:
                        // TODO:
                        return kError;
                }
                context.state = 1;
                return kAccept;
            case 3:
                if (context.chr >= '0' && context.chr <= '9') {
                    context.data = context.data * 16 + context.chr - '0';
                } else if (context.chr >= 'A' && context.chr <= 'F') {
                    context.data = context.data * 16 + context.chr - 'A' + 10;
                } else if (context.chr >= 'a' && context.chr <= 'f') {
                    context.data = context.data * 16 + context.chr - 'a' + 10;
                } else {
                    // TODO:
                    return kError;
                }
                context.state = 4;
                return kAccept;
            case 4:
                if (context.chr >= '0' && context.chr <= '9') {
                    context.data = context.data * 16 + context.chr - '0';
                } else if (context.chr >= 'A' && context.chr <= 'F') {
                    context.data = context.data * 16 + context.chr - 'A' + 10;
                } else if (context.chr >= 'a' && context.chr <= 'f') {
                    context.data = context.data * 16 + context.chr - 'a' + 10;
                } else {
                    context.buf.push_back((c8)context.data);
                    context.state = 1;
                    return process(context);
                }
                return kAccept;
                
            case 6:
                if (context.chr >= '0' && context.chr <= '9') {
                    context.data = context.data * 16 + context.chr - '0';
                } else if (context.chr >= 'A' && context.chr <= 'F') {
                    context.data = context.data * 16 + context.chr - 'A' + 10;
                } else if (context.chr >= 'a' && context.chr <= 'f') {
                    context.data = context.data * 16 + context.chr - 'a' + 10;
                } else {
                    // TODO:
                    return kError;
                }
                context.state = 7;
                return kAccept;
            case 7:
                if (context.chr >= '0' && context.chr <= '9') {
                    context.data = context.data * 16 + context.chr - '0';
                } else if (context.chr >= 'A' && context.chr <= 'F') {
                    context.data = context.data * 16 + context.chr - 'A' + 10;
                } else if (context.chr >= 'a' && context.chr <= 'f') {
                    context.data = context.data * 16 + context.chr - 'a' + 10;
                } else {
                    // TODO:
                    return kError;
                }
                context.state = 8;
                return kAccept;
            case 8:
                if (context.chr >= '0' && context.chr <= '9') {
                    context.data = context.data * 16 + context.chr - '0';
                } else if (context.chr >= 'A' && context.chr <= 'F') {
                    context.data = context.data * 16 + context.chr - 'A' + 10;
                } else if (context.chr >= 'a' && context.chr <= 'f') {
                    context.data = context.data * 16 + context.chr - 'a' + 10;
                } else {
                    // TODO:
                    return kError;
                }
                context.state = 9;
                return kAccept;
                
            case 9: {
                if (context.chr >= '0' && context.chr <= '9') {
                    context.data = context.data * 16 + context.chr - '0';
                } else if (context.chr >= 'A' && context.chr <= 'F') {
                    context.data = context.data * 16 + context.chr - 'A' + 10;
                } else if (context.chr >= 'a' && context.chr <= 'f') {
                    context.data = context.data * 16 + context.chr - 'a' + 10;
                } else {
                    // TODO:
                    return kError;
                }
                str8 t;
                Unicode::convert((const c16 *)&context.data, 1, t);
                if (t.empty()) {
                    // TODO:
                    return kError;
                }
                context.buf += t;
                context.state = 1;
                return kAccept;
            }
        }
        return kRefuse;
    }
    
    
    Lexer::Analizer::Result LA_Number::process(Lexer::Context &context) {
        switch (context.state) {
            case 0:
                if (context.chr >= '0' && context.chr <= '9') {
                    context.buf.clear();
                    context.buf.push_back(context.chr);
                    context.state = (context.chr == '0' ? 1 : 4);
                    return kAccept;
                }
                break;
                
            case 1:
                if (context.chr == 'x' || context.chr == 'X') {
                    context.buf.push_back(context.chr);
                    context.state = 2;
                } else if (context.chr == '.') {
                    context.buf.push_back('.');
                    context.state = 5;
                } else if (context.chr >= '0' && context.chr <= '9') {
                    context.buf.push_back(context.chr);
                    context.state = 4;
                } else if (context.chr == 'l' || context.chr == 'L') {
                    Token *token = vnnew Token(context.file);
                    context.buf.push_back(context.chr);
                    token->set(Token::kLongInteger, context.buf).int64 = 0;
                    token->row = context.token_row;
                    context.tokens->push_back(token);
                    context.state = 9;
                } else if (context.chr == 'e' || context.chr == 'E') {
                    context.buf.push_back(context.chr);
                    context.state = 6;
                } else if (LA_Identifier::isLetter(context.chr) || context.chr == '_') {
                    // TODO:
                    return kError;
                } else {
                    Token *token = vnnew Token(context.file);
                    token->set(Token::kInteger, context.buf).int32 = 0;
                    token->row = context.token_row;
                    context.tokens->push_back(token);
                    return kRefuse;
                }
                return kAccept;
                
            case 2:
                if ((context.chr >= '0' && context.chr <= '9') || (context.chr >= 'A' && context.chr <= 'F') || (context.chr >= 'a' && context.chr <= 'f')) {
                    context.buf.push_back(context.chr);
                    context.state = 3;
                } else {
                    // TODO:
                    return kError;
                }
                return kAccept;
                
            case 3:
                if ((context.chr >= '0' && context.chr <= '9') || (context.chr >= 'A' && context.chr <= 'F') || (context.chr >= 'a' && context.chr <= 'f')) {
                    context.buf.push_back(context.chr);
                } else if (context.chr == 'l' || context.chr == 'L') {
                    Token *token = vnnew Token(context.file);
                    sscanf(context.buf.c_str() + 2, "%llx", &token->int64);
                    context.buf.push_back(context.chr);
                    token->set(Token::kLongInteger, context.buf);
                    token->row = context.token_row;
                    context.tokens->push_back(token);
                    context.state = 9;
                } else if (LA_Identifier::isLetter(context.chr) || context.chr == '_') {
                    // TODO:
                    return kError;
                } else {
                    Token *token = vnnew Token(context.file);
                    token->set(Token::kInteger, context.buf);
                    sscanf(context.buf.c_str() + 2, "%x", &token->int32);
                    token->row = context.token_row;
                    context.tokens->push_back(token);
                    return kRefuse;
                }
                return kAccept;
                
            case 4:
                if (context.chr == '.') {
                    context.buf.push_back('.');
                    context.state = 5;
                } else if (context.chr >= '0' && context.chr <= '9') {
                    context.buf.push_back(context.chr);
                } else if (context.chr == 'l' || context.chr == 'L') {
                    Token *token = vnnew Token(context.file);
                    sscanf(context.buf.c_str(), "%llu", &token->int64);
                    context.buf.push_back(context.chr);
                    token->set(Token::kLongInteger, context.buf);
                    token->row = context.token_row;
                    context.tokens->push_back(token);
                    context.state = 9;
                } else if (context.chr == 'e' || context.chr == 'E') {
                    context.buf.push_back(context.chr);
                    context.state = 6;
                } else if (LA_Identifier::isLetter(context.chr) || context.chr == '_') {
                    // TODO:
                    return kError;
                } else {
                    Token *token = vnnew Token(context.file);
                    token->set(Token::kInteger, context.buf);
                    sscanf(context.buf.c_str(), "%u", &token->int32);
                    token->row = context.token_row;
                    context.tokens->push_back(token);
                    return kRefuse;
                }
                return kAccept;
                
            case 5:
                if (context.chr >= '0' && context.chr <= '9') {
                    context.buf.push_back(context.chr);
                } else if (context.chr == 'e' || context.chr == 'E') {
                    context.buf.push_back(context.chr);
                    context.state = 6;
                } else if (context.chr == 'f' || context.chr == 'F') {
                    Token *token = vnnew Token(context.file);
                    sscanf(context.buf.c_str(), "%f", &token->float32);
                    context.buf.push_back(context.chr);
                    token->set(Token::kFloat, context.buf);
                    token->row = context.token_row;
                    context.tokens->push_back(token);
                    context.state = 9;
                } else if (LA_Identifier::isLetter(context.chr) || context.chr == '_') {
                    // TODO:
                    return kError;
                } else {
                    Token *token = vnnew Token(context.file);
                    token->set(Token::kDouble, context.buf);
                    sscanf(context.buf.c_str(), "%lf", &token->float64);
                    token->row = context.token_row;
                    context.tokens->push_back(token);
                    return kRefuse;
                }
                return kAccept;
                
            case 6:
                if (context.chr == '+' || context.chr == '-') {
                    context.buf.push_back(context.chr);
                    context.state = 7;
                } else if (context.chr >= '0' && context.chr <= '9') {
                    context.buf.push_back(context.chr);
                    context.state = 8;
                } else {
                    // TODO:
                    return kError;
                }
                return kAccept;
                
            case 7:
                if (context.chr >= '0' && context.chr <= '9') {
                    context.buf.push_back(context.chr);
                    context.state = 8;
                } else {
                    // TODO:
                    return kError;
                }
                return kAccept;
                
            case 8:
                if (context.chr >= '0' && context.chr <= '9') {
                    context.buf.push_back(context.chr);
                } else if (context.chr == 'f' || context.chr == 'F') {
                    Token *token = vnnew Token(context.file);
                    sscanf(context.buf.c_str(), "%e", &token->float32);
                    context.buf.push_back(context.chr);
                    token->set(Token::kFloat, context.buf);
                    token->row = context.token_row;
                    context.tokens->push_back(token);
                    context.state = 9;
                } else if (LA_Identifier::isLetter(context.chr) || context.chr == '_') {
                    // TODO:
                    return kError;
                } else {
                    Token *token = vnnew Token(context.file);
                    token->set(Token::kDouble, context.buf);
                    sscanf(context.buf.c_str(), "%le", &token->float64);
                    token->row = context.token_row;
                    context.tokens->push_back(token);
                    return kRefuse;
                }
                return kAccept;
                
            case 9:
                if (LA_Identifier::isLetter(context.chr) || LA_Identifier::isDigit(context.chr) || context.chr == '_') {
                    // TODO:
                    return kError;
                }
                break;
                
            default:
                break;
        }
        return kRefuse;
    }
    
    LA_Symbol::Symbol::~Symbol() {
        for (std::map<c8, Symbol *>::iterator it = symbols.begin(); it != symbols.end(); ++it) {
            delete it->second;
        }
    }
    
    LA_Symbol::LA_Symbol() {
        
    }
    
    LA_Symbol::LA_Symbol(const LR_Terminal *symbols) {
        for (;symbols->name; ++symbols) {
            registerSymbol(symbols->name, symbols->identity);
        }
    }
    
    LA_Symbol::~LA_Symbol() {
        for (SymbolMap::iterator it = m_symbols.begin(); it != m_symbols.end(); ++it) {
            delete it->second;
        }
        m_symbols.clear();
    }
    
    void LA_Symbol::registerSymbol(const char *symbol, unsigned int sId) {
        SymbolMap::iterator it = m_symbols.insert(std::make_pair(*symbol++, (Symbol *)0)).first;
        if (!it->second) {
            it->second = new Symbol;
        }
        while (*symbol) {
            it = it->second->symbols.insert(std::make_pair(*symbol++, (Symbol *)0)).first;
            if (!it->second) {
                it->second = new Symbol;
            }
        }
        it->second->sId = sId;
    }
    
    Lexer::Analizer::Result LA_Symbol::process(Lexer::Context &context) {
        switch (context.state) {
            case 0:
			{
				SymbolMap::iterator it = m_symbols.find(context.chr);
				if (it != m_symbols.end()) {
					context.state = 1;
					context.buf.clear();
					context.buf.push_back(context.chr);
					context.data = (long)it->second;
					return kAccept;
				}
			}
                break;
                
            case 1:
			{
				Symbol *p = (Symbol *)context.data;
				SymbolMap::iterator it = p->symbols.find(context.chr);
				if (it == p->symbols.end()) {
					if (p->sId == 0) {
						// TODO:
						return kError;
					}
					Token *token = vnnew Token(context.file);
					token->row = context.token_row;
					token->set(Token::kSymbol, context.buf).int32 = p->sId;
					context.tokens->push_back(token);
				} else {
					context.buf.push_back(context.chr);
					context.data = (long)it->second;
					return kAccept;
				}
			}
                break;
        }
        return kRefuse;
    }

    
}

_vn_end
