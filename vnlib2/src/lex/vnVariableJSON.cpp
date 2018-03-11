//
//  vnVariableJSON.cpp
//  vnlib
//
//  Created by Wayne on 10/15/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnVariableJSON.h"
#include "vnLexAnalizers.h"
#include "vnLog.h"
#include "vnFileManager.h"

#include <cstring>
#include <sstream>

_vn_begin

namespace internal {
#include "vnVariableJSONProductions.h"
#include "vnVariableJSONStates.hpp"
}

VariableJSON::VariableJSON()
: m_parser(internal::g_states)
#if VN_PLATFORM == VN_PLATFORM_WIN
, m_lexer(std::initializer_list<lex::Lexer::AnalizerPtr>{
		vnnew lex::LA_Space(),
		vnnew lex::LA_Identifier(internal::g_keywordInfo),
		vnnew lex::LA_Number(),
		vnnew lex::LA_Comment(),
		vnnew lex::LA_String(),
        vnnew lex::LA_CharacterEx(),
		vnnew lex::LA_Symbol(internal::g_symbolInfo)
})
#else
, m_lexer{
	vnnew lex::LA_Space(),
	vnnew lex::LA_Identifier(internal::g_keywordInfo),
	vnnew lex::LA_Number(),
	vnnew lex::LA_Comment(),
	vnnew lex::LA_String(),
    vnnew lex::LA_CharacterEx(),
	vnnew lex::LA_Symbol(internal::g_symbolInfo)
}
#endif
{
    
}

RefCountedPtr<Variable_object> VariableJSON::parse(const FilePath &file) {
    RefCountedPtr<Variable_object> ret;
    FileStream *fs = FileManager::instance().open(file);
    if (!fs) {
        VN_LOG_ERROR("open file \"" << file.fullPath() << "\" failed!");
        return RefCountedPtr<Variable_object>();
    }
    size_t size = (size_t)fs->size();
    if (!size) {
        fs->close();
        ret = vnnew Variable_object();
    } else {
        c8 *ptr = vnmalloc(c8, size);
        if (fs->read(ptr, size) != size) {
            VN_LOG_ERROR("read file \"" << file.fullPath() << "\" failed!");
            fs->close();
            vnfree(ptr);
            return RefCountedPtr<Variable_object>();
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
        ret = parse(txt, size, tf);
        vnfree(ptr);
    }
    return std::move(ret);
}

RefCountedPtr<Variable_object> VariableJSON::parse(const c8 *text, size_t length, lex::Token::FilePtr file) {
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
    if (root) {
        RefCountedPtr<Variable_object> ret = vnnew Variable_object();
        _parse_object(root->child(0), ret.get());
        return std::move(ret);
    }
    return nullptr;
    
}

void VariableJSON::_parse_object(lex::LR_Node *node, Variable_object *object) {
    switch (node->productionId()) {
        case internal::PROD_object_0:
            break;
            
        case internal::PROD_object_1:
            _parse_item_list(node->child(1), object);
            break;
    }
}

void VariableJSON::_parse_item_list(lex::LR_Node *node, Variable_object *object) {
    switch (node->productionId()) {
        case internal::PROD_item_list_0:
            _parse_item(node->child(0), object);
            break;
            
        case internal::PROD_item_list_1:
            _parse_item_list(node->child(0), object);
            _parse_item(node->child(2), object);
            break;
    }
}

void VariableJSON::_parse_item(lex::LR_Node *node, Variable_object *object) {
    object->setAttribute(node->child(0)->token()->text, _parse_value(node->child(2)));
}

VariablePtr VariableJSON::_parse_number(lex::LR_Node *node, bool negative) {
    VariablePtr ret;
    switch (node->productionId()) {
        case internal::PROD_number_0:
            ret = vnnew Variable_int32(node->child(0)->token()->int32 * (negative ? -1 : 1));
            break;
            
        case internal::PROD_number_1:
            ret = vnnew Variable_int64(node->child(0)->token()->int64 * (negative ? -1 : 1));
            break;
            
        case internal::PROD_number_2:
            ret = vnnew Variable_float(node->child(0)->token()->float32 * (negative ? -1 : 1));
            break;
            
        case internal::PROD_number_3:
            ret = vnnew Variable_double(node->child(0)->token()->float64 * (negative ? -1 : 1));
            break;
    }
    return std::move(ret);
}

VariablePtr VariableJSON::_parse_value(lex::LR_Node *node) {
    VariablePtr ret;
    switch (node->productionId()) {
        case internal::PROD_value_0:
            ret = Variable::kNull;
            break;
            
        case internal::PROD_value_1:
            ret = vnnew Variable_bool(true);
            break;
            
        case internal::PROD_value_2:
            ret = vnnew Variable_bool(false);
            break;
            
        case internal::PROD_value_3:
            ret = _parse_number(node->child(0), false);
            break;
        case internal::PROD_value_4:
            ret = _parse_number(node->child(1), false);
            break;
            
        case internal::PROD_value_5:
            ret = _parse_number(node->child(1), true);
            break;
            
        case internal::PROD_value_6:
            ret = vnnew Variable_string(node->child(0)->token()->text);
            break;
            
        case internal::PROD_value_7:
            ret = _parse_array(node->child(0));
            break;
            
        case internal::PROD_value_8: {
            Variable_object *object = vnnew Variable_object();
            _parse_object(node->child(0), object);
            ret = object;
            break;
        }
    }
    return std::move(ret);
}

VariablePtr VariableJSON::_parse_array(lex::LR_Node *node) {
    Variable_object *ret = vnnew Variable_object();
    switch (node->productionId()) {
        case internal::PROD_array_0:
            break;
            
        case internal::PROD_array_1:
            _parse_value_list(node->child(1), ret);
            break;
    }
    return ret;
}

void VariableJSON::_parse_value_list(lex::LR_Node *node, Variable_object *object) {
    switch (node->productionId()) {
        case internal::PROD_value_list_0:
            object->add(_parse_value(node->child(0)));
            break;
            
        case internal::PROD_value_list_1:
            _parse_value_list(node->child(0), object);
            object->add(_parse_value(node->child(2)));
            break;
    }
}

str8 VariableJSON::generate(const Variable_object *object) {
    if (!object) {
        return "{}";
    }
    std::ostringstream oss;
    _gen_object(object, oss);
    return oss.str();
}

void VariableJSON::_gen_object(const Variable_object *object, std::ostringstream &oss) {
    oss << '{';
    if (!object->attributes().empty()) {
        auto it = object->attributes().begin();
        oss << lex::LA_String::convert(it->first) << ':';
        _gen_value(it->second.get(), oss);
        for (++it; it != object->attributes().end(); ++it) {
            oss << ',';
            oss << lex::LA_String::convert(it->first) << ':';
            _gen_value(it->second.get(), oss);
        }
    }
    oss << '}';
}

void VariableJSON::_gen_value(const Variable *var, std::ostringstream &oss) {
    switch (var->type()) {
        case kVT_null:
            oss << "null";
            break;
            
        
        case kVT_bool:
            oss << (static_cast<const Variable_bool *>(var)->value() ? "true" : "false");
            break;
            
        case kVT_int32:
            oss << static_cast<const Variable_int32 *>(var)->value();
            break;
            
        case kVT_int64:
            oss << static_cast<const Variable_int64 *>(var)->value();
            break;
            
        case kVT_float:
            oss << static_cast<const Variable_float *>(var)->value();
            break;
            
        case kVT_double:
            oss << static_cast<const Variable_double *>(var)->value();
            break;
            
        case kVT_string:
            oss << lex::LA_String::convert(static_cast<const Variable_string *>(var)->value());
            break;
            
        case kVT_object: {
            auto obj = static_cast<const Variable_object *>(var);
            if (obj->attributes().empty()) {
                _gen_array(obj, oss);
            } else {
                _gen_object(obj, oss);
            }
            break;
        }
            
        default:
            break;
    }
}

void VariableJSON::_gen_array(const Variable_object *object, std::ostringstream &oss) {
    oss << '[';
    if (!object->empty()) {
        _gen_value(object->at(0).get(), oss);
        for (size_t i = 1; i < object->size(); ++i) {
            oss << ',';
            _gen_value(object->at(i).get(), oss);
        }
    }
    oss << ']';
}

_vn_end
