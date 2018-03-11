//
//  vnVariableJSON.h
//  vnlib
//
//  Created by Wayne on 10/15/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnVariableJSON_h
#define vnlib_vnVariableJSON_h

#include "vnLexer.h"
#include "vnLexLR_Parser.h"
#include "vnFilePath.h"
#include "vnVariable.h"

_vn_begin

class VariableJSON : public AllocatedObject {
public:
    VariableJSON();
    
    RefCountedPtr<Variable_object> parse(const FilePath &file);
    RefCountedPtr<Variable_object> parse(const c8 *text, size_t length, lex::Token::FilePtr file = nullptr);
    static str8 generate(const Variable_object *object);
    
protected:
	lex::LR_Parser m_parser;
    lex::Lexer m_lexer;

    
    static void _parse_object(lex::LR_Node *node, Variable_object *object);
    static void _parse_item_list(lex::LR_Node *node, Variable_object *object);
    static void _parse_item(lex::LR_Node *node, Variable_object *object);
    static VariablePtr _parse_number(lex::LR_Node *node, bool negative);
    static VariablePtr _parse_value(lex::LR_Node *node);
    static VariablePtr _parse_array(lex::LR_Node *node);
    static void _parse_value_list(lex::LR_Node *node, Variable_object *object);
    
    static void _gen_value(const Variable *var, std::ostringstream &oss);
    static void _gen_object(const Variable_object *object, std::ostringstream &oss);
    static void _gen_array(const Variable_object *object, std::ostringstream &oss);
};

_vn_end

#endif
