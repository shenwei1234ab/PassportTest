//
//  vnVariableScript.cpp
//  vnlib
//
//  Created by Wayne on 22/5/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnVariableScript.h"
#include "vnLexAnalizers.h"
#include "vnLog.h"
#include "vnFileManager.h"

#include <cstring>

_vn_begin

namespace internal {
#include "vnVariableScriptProductions.h"
#include "vnVariableScriptStates.hpp"
}

VariableScript::VariableScript()
: m_parser(internal::g_states)
#if VN_PLATFORM == VN_PLATFORM_WIN
, m_lexer(std::initializer_list<lex::Lexer::AnalizerPtr>{
    vnnew lex::LA_Space(),
    vnnew lex::LA_Identifier(internal::g_keywordInfo),
    vnnew lex::LA_Number(),
    vnnew lex::LA_Comment(),
    vnnew lex::LA_String(),
    vnnew lex::LA_Symbol(internal::g_symbolInfo)
})
#else
, m_lexer{
	vnnew lex::LA_Space(),
	vnnew lex::LA_Identifier(internal::g_keywordInfo),
	vnnew lex::LA_Number(),
	vnnew lex::LA_Comment(),
	vnnew lex::LA_String(),
	vnnew lex::LA_Symbol(internal::g_symbolInfo)
}
#endif
{
    
}

RefCountedPtr<Variable_object> VariableScript::parse(const FilePath &file) {
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
		ret = parse(txt, size, std::move(tf));
        vnfree(ptr);
    }
    return std::move(ret);
}

RefCountedPtr<Variable_object> VariableScript::parse(const c8 *text, size_t length, lex::Token::FilePtr file) {
    lex::TokenList tokens;
    if (m_lexer.scan(text, length, file, tokens) < 0) {
        VN_LOG_ERROR("lexical error: " << m_lexer.error());
        return RefCountedPtr<Variable_object>();
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
        _parse_object_member_list(root->child(0), ret.get());
        return std::move(ret);
    }
    return nullptr;
}


str8 VariableScript::generate(const Variable_object *object) {
    std::ostringstream oss;
    if (object) {
        _gen_object_members(object, 0, oss);
    }
    return std::move(oss.str());
}

void VariableScript::_parse_object_member_list(lex::LR_Node *node, Variable_object *object) {
    if (node->productionId() == internal::PROD_object_member_list_0) {
        _parse_object_member(node->child(0), object);
    } else {
        _parse_object_member_list(node->child(0), object);
        _parse_object_member(node->child(1), object);
    }
}

void VariableScript::_parse_object_member(lex::LR_Node *node, Variable_object *object) {
    if (node->productionId() == internal::PROD_object_member_0) {
        object->add(_parse_value(node->child(0)));
    } else {
        _parse_attribute(node->child(0), object);
    }
}

void VariableScript::_parse_attribute(lex::LR_Node *node, Variable_object *object) {
    object->setAttribute(node->child(0)->token()->text, _parse_value(node->child(2)));
}

VariablePtr VariableScript::_parse_value(lex::LR_Node *node) {
    switch (node->productionId()) {
        case internal::PROD_value_0:
            break;
            
        case internal::PROD_value_1:
            return vnnew Variable_bool(true);
            
        case internal::PROD_value_2:
            return vnnew Variable_bool(false);
            
        case internal::PROD_value_3:
            return _parse_number(node->child(0));
            
        case internal::PROD_value_4:
            return _parse_number(node->child(1));
            
        case internal::PROD_value_5:
            return _parse_minus_number(node->child(1));
            
        case internal::PROD_value_6:
            return vnnew Variable_string(node->child(0)->token()->text);
			
        case internal::PROD_value_7:
            return vnnew Variable_id(node->child(0)->token()->text);
            
        case internal::PROD_value_8:
            return _parse_object(node->child(0));
        
    }
    return Variable::kNull;
}

VariablePtr VariableScript::_parse_number(lex::LR_Node *node) {
    switch (node->productionId()) {
        case internal::PROD_number_0:
            return vnnew Variable_int32(node->child(0)->token()->int32);
            
        case internal::PROD_number_1:
            return vnnew Variable_int64(node->child(0)->token()->int64);
            
        case internal::PROD_number_2:
            return vnnew Variable_float(node->child(0)->token()->float32);
            
        case internal::PROD_number_3:
            return vnnew Variable_double(node->child(0)->token()->float64);
    }
    return Variable::kNull;
}

VariablePtr VariableScript::_parse_minus_number(lex::LR_Node *node) {
    switch (node->productionId()) {
        case internal::PROD_number_0:
            return vnnew Variable_int32(-(s32)node->child(0)->token()->int32);
            
        case internal::PROD_number_1:
            return vnnew Variable_int64(-(s64)node->child(0)->token()->int64);
            
        case internal::PROD_number_2:
            return vnnew Variable_float(-node->child(0)->token()->float32);
            
        case internal::PROD_number_3:
            return vnnew Variable_double(-node->child(0)->token()->float64);
    }
    return Variable::kNull;
}

VariablePtr VariableScript::_parse_object(lex::LR_Node *node) {
    Variable_object *object = 0;
    if (node->productionId() == internal::PROD_object_0) {
        lex::LR_Node *head = node->child(0);
        if (head->productionId() == internal::PROD_object_head_0) {
            object = vnnew Variable_object();
        } else {
            object = vnnew Variable_object(head->child(1)->token()->text);
        }
        _parse_object_body(node->child(1), object);
    } else {
        object = vnnew Variable_object();
        _parse_value_list(node->child(1), object);
    }
    return object;
}

void VariableScript::_parse_value_list(lex::LR_Node *node, Variable_object *object) {
    if (node->productionId() == internal::PROD_value_list_0) {
        object->add(_parse_value(node->child(0)));
    } else {
        _parse_value_list(node->child(0), object);
        object->add(_parse_value(node->child(1)));
    }
}

void VariableScript::_parse_object_body(lex::LR_Node *node, Variable_object *object) {
    if (node->productionId() == internal::PROD_object_body_1) {
        _parse_object_member_list(node->child(1), object);
    }
}

void VariableScript::_gen_object_members(const Variable_object *object, int tab, std::ostringstream &oss) {
	for (auto &i : object->attributes()) {
		oss << str8(tab * 4, ' ') << i.first << " = ";
		_gen_value(i.second.get(), tab, oss);
		oss << std::endl;
	}
    for (size_t i = 0, c = object->size(); i < c; ++i) {
        oss << str8(tab * 4, ' ');
        _gen_value(object->at(i).get(), tab, oss);
        oss << std::endl;
    }
}

void VariableScript::_gen_value(const vn::Variable *var, int tab, std::ostringstream &oss) {
    if (!var) {
        oss << "null";
        return ;
    }
    switch (var->type()) {
        case kVT_null:
            oss << "null";
            break;
            
        case kVT_bool:
            if (static_cast<const Variable_bool *>(var)->getBool()) {
                oss << "true";
            } else {
                oss << "false";
            }
            break;
        
        case kVT_int32:
            oss << static_cast<const Variable_int32 *>(var)->value();
            break;
            
        case kVT_int64:
            oss << static_cast<const Variable_int64 *>(var)->value() << 'L';
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
            
        case kVT_id: {
            auto p = static_cast<const Variable_id *>(var);
            if (p->value().empty()) {
                oss << "null_id";
            } else {
                oss << p->value();
            }
            break;
        }
            
        case kVT_object:
            _gen_object(static_cast<const Variable_object *>(var), tab, oss);
            break;
            
        default:
            oss << "null";
            break;
    }
}

void VariableScript::_gen_object(const vn::Variable_object *object, int tab, std::ostringstream &oss) {
    if (object->name().empty() && object->attributes().empty()) {
        if (object->size()) {
            oss << '[' << std::endl;
            for (size_t i = 0, c = object->size(); i < c; ++i) {
                oss << str8((tab + 1) * 4, ' ');
                _gen_value(object->at(i).get(), tab + 1, oss);
                oss << std::endl;
            }
            oss << str8(tab * 4, ' ') << ']';
        } else {
            oss << "[]";
        }
        return ;
    }
    oss << '@' << object->name() << " {";
    if (object->attributes().empty() && !object->size()) {
        oss << '}';
        return ;
    }
    oss << std::endl;
    _gen_object_members(object, tab + 1, oss);
    oss << str8(tab * 4, ' ') << '}';
}

_vn_end
