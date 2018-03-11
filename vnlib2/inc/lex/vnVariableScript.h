//
//  vnVariableScript.h
//  vnlib
//
//  Created by Wayne on 22/5/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnVariableScript_h
#define vnlib_vnVariableScript_h

#include "vnLexer.h"
#include "vnLexLR_Parser.h"
#include "vnFilePath.h"
#include "vnVariable.h"

#include <sstream>

_vn_begin

class VariableScript : public AllocatedObject {
public:
    VariableScript();
    
    RefCountedPtr<Variable_object> parse(const FilePath &file);
    RefCountedPtr<Variable_object> parse(const c8 *text, size_t length, lex::Token::FilePtr file);
    
    static str8 generate(const Variable_object *object);
    
protected:
	lex::LR_Parser m_parser;
    lex::Lexer m_lexer;

    
    static void _parse_object_member_list(lex::LR_Node *node, Variable_object *object);
    static void _parse_object_member(lex::LR_Node *node, Variable_object *object);
    static void _parse_attribute(lex::LR_Node *node, Variable_object *object);
    static VariablePtr _parse_value(lex::LR_Node *node);
    static VariablePtr _parse_number(lex::LR_Node *node);
    static VariablePtr _parse_minus_number(lex::LR_Node *node);
    static VariablePtr _parse_object(lex::LR_Node *node);
    static void _parse_value_list(lex::LR_Node *node, Variable_object *object);
    static void _parse_object_body(lex::LR_Node *node, Variable_object *object);
    
    static void _gen_object_members(const Variable_object *object, int tab, std::ostringstream &oss);
    static void _gen_value(const Variable *var, int tab, std::ostringstream &oss);
    static void _gen_object(const Variable_object *object, int tab, std::ostringstream &oss);
};

_vn_end

#endif
