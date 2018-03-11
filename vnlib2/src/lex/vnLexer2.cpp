//
//  vnLexer2.cpp
//  vnlib
//
//  Created by Wayne on 6/27/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "vnLexer2.h"

#include <sstream>

_vn_begin

namespace lex {
    
    Lexer2::Lexer2(std::initializer_list<AnalizerCreatorRef> creators) {
        for (auto &i : creators) {
			i.creator(m_analizers);
        }
    }
    
    Lexer2::Lexer2(const Lexer2 &rhs) {
        m_analizers.reserve(rhs.m_analizers.size());
        for (auto &i : rhs.m_analizers) {
            m_analizers.push_back(std::unique_ptr<Analizer>(i->clone()));
        }
    }
    
    Lexer2::Lexer2(Lexer2 &&rhs)
    : m_analizers(std::move(rhs.m_analizers)) {
        for (auto &i : m_analizers) {
            i->reset();
        }
    }
    
    void Lexer2::reset(Token::FilePtr file, int row, int col) {
        m_ctx.file = std::move(file);
        m_ctx.row = row;
        m_ctx.col = col;
        m_ctx.buffer.clear();
        m_tokens.clear();
        m_open.clear();
        m_close.clear();
        m_error.clear();
        m_errorMsg.clear();
        for (auto &i : m_analizers) {
            i->reset();
        }
    }
    
    bool Lexer2::input(c8 chr) {
        vnassert(chr != '\0');
        if (m_open.empty()) {
        l_input:
            for (auto &i : m_analizers) {
                if (i->begin(chr)) {
                    m_open.push_back(i.get());
                }
            }
            if (m_open.empty()) {
                std::ostringstream oss;
                if (m_ctx.file) {
                    oss << m_ctx.file->name.c_str();
                }
                oss << '(' << m_ctx.row + 1 << ':' << m_ctx.col + 1 << "): invalid character \'" << chr << "\'.";
                m_errorMsg = oss.str();
                return false;
            }
            m_ctx.buffer.push_back(chr);
            return true;
        }
        
        auto cur = &m_open.front();
        auto last = &m_open.back();
        while (cur <= last) {
            auto ret = (*cur)->input(chr);
            if (ret == Analizer::kAccept) {
                ++cur;
            } else if (ret == Analizer::kRefuse) {
                m_close.push_back(*cur);
                *cur = *last;
                --last;
                m_open.pop_back();
            } else {
                m_error.push_back(*cur);
                *cur = *last;
                --last;
                m_open.pop_back();
            }
        }
        if (m_open.empty()) {
            if (m_close.empty()) {
                m_errorMsg = m_error.back()->makeErrorMsg(m_ctx, chr);
                return false;
            }
            
            auto p = m_close.back();
            int row, col;
            str8 text {m_ctx.buffer.begin() + p->processed(), m_ctx.buffer.end()};
            
            p->end(m_ctx, m_tokens, row, col);
            
            m_ctx.buffer.clear();
            m_ctx.row = row;
            m_ctx.col = col;
            for (auto i : m_close) {
                i->reset();
            }
            m_close.clear();
            for (auto i : m_error) {
                i->reset();
            }
            m_error.clear();
            for (auto i : text) {
                if (!this->input(i)) {
                    return false;
                }
            }
            goto l_input;
        }
        m_ctx.buffer.push_back(chr);
        return true;
    }
    
    bool Lexer2::end(TokenList &result, int &row, int &col) {
        if (m_open.empty()) {
            m_errorMsg = "no input.";
            return false;
        }
        if (m_open.size() != 1) {
            std::ostringstream oss;
            if (m_ctx.file) {
                oss << m_ctx.file->name.c_str();
            }
            oss << '(' << m_ctx.row + 1 << ':' << m_ctx.col + 1 << "): can not parse the rest content.";
            m_errorMsg = oss.str();
            return false;
        }
        if (!m_open.back()->end(m_ctx, m_tokens, row, col)) {
            m_errorMsg = m_open.back()->makeErrorMsg(m_ctx, 0);
            return false;
        }
        result = std::move(m_tokens);
        return true;
    }
    
    ssize_t Lexer2::scan(const c8 *text, size_t length, Token::FilePtr file, TokenList &result) {
        this->reset(std::move(file));
        auto p = text;
        while (*p && length) {
            if (!this->input(*p++)) {
                return -1;
            }
            --length;
        }
        int row, col;
        if (!this->end(result, row, col)) {
            return -1;
        }
        return p - text;
    }
    
    const str8 & Lexer2::error() const {
        return m_errorMsg;
    }
    
}

_vn_end
