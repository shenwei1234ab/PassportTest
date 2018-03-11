//
//  vnPacketScript.cpp
//  vnlib
//
//  Created by Wayne on 5/19/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnPacketScript.h"
#include "vnLexAnalizers.h"
#include "vnLog.h"
#include "vnFileManager.h"

#include <cstring>

_vn_begin

namespace internal {
#include "vnPacketScriptProductions.h"
#include "vnPacketScriptStates.hpp"
}

PacketScript::PacketScript()
: m_lexer({
    vnnew lex::LA_Space(),
    vnnew lex::LA_Identifier(internal::g_keywordInfo),
    vnnew lex::LA_Number(),
    vnnew lex::LA_Comment(),
    vnnew lex::LA_Symbol(internal::g_symbolInfo)
})
, m_parser(internal::g_states) {
    
}

bool PacketScript::parse(const FilePath &file, std::vector<StructDesc> &structs, std::vector<PacketDesc> &packets) {
    FileStream *fs = FileManager::instance().open(file);
    if (!fs) {
        VN_LOG_ERROR("open file \"" << file.fullPath() << "\" failed!");
        return false;
    }
    size_t size = (size_t)fs->size();
    if (!size) {
        structs.clear();
        packets.clear();
        fs->close();
        return true;
    }
    c8 *ptr = vnmalloc(c8, size);
    if (fs->read(ptr, size) != size) {
        VN_LOG_ERROR("read file \"" << file.fullPath() << "\" failed!");
        fs->close();
        vnfree(ptr);
        return false;
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
    
    bool ret = parse(txt, size, tf, structs, packets);
    
    vnfree(ptr);
    
    return ret;
}

bool PacketScript::parse(const c8 *text, size_t length, const lex::Token::FilePtr &file, std::vector<StructDesc> &structs, std::vector<PacketDesc> &packets) {
    
    lex::TokenList tokens;
    if (m_lexer.scan(text, length, file, tokens) < 0) {
        VN_LOG_ERROR("lexical error: " << m_lexer.error());
        return false;
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
    if (!root) {
        return false;
    }
    _parse_root(root.get());
    
    structs.clear();
    structs.reserve(m_structs.size());
    for (auto &x : m_structs) {
        structs.push_back(std::move(x));
    }
    packets.clear();
    packets.reserve(m_packets.size());
    for (auto &x : m_packets) {
        packets.push_back(std::move(x));
    }
    m_structs.clear();
    m_packets.clear();
    return true;
}

void PacketScript::_parse_root(lex::LR_Node *root) {
    switch (root->productionId()) {
        case internal::PROD_script_0:
            _parse_definition(root->child(0));
            break;
            
        case internal::PROD_script_1:
            _parse_root(root->child(0));
            _parse_definition(root->child(1));
            break;
    }
}

void PacketScript::_parse_definition(lex::LR_Node *node) {
    switch (node->productionId()) {
        case internal::PROD_definition_0:
            _parse_struct_def(node->child(0));
            break;
            
        case internal::PROD_definition_1:
            _parse_packet_def(node->child(0));
            break;
    }
}

void PacketScript::_parse_struct_def(lex::LR_Node *node) {
    StructDesc desc;
    desc.name = node->child(1)->token()->text;
    _parse_struct_body(node->child(2), desc);
    m_structs.push_back(std::move(desc));
}

void PacketScript::_parse_struct_body(lex::LR_Node *node, StructDesc &desc) {
    switch (node->productionId()) {
        case internal::PROD_struct_body_0:
            break;
            
        case internal::PROD_struct_body_1:
            _parse_struct_member_list(node->child(1), desc);
            break;
    }
}

void PacketScript::_parse_struct_member_list(lex::LR_Node *node, StructDesc &desc) {
    switch (node->productionId()) {
        case internal::PROD_struct_member_list_0:
            _parse_struct_member(node->child(0), desc);
            break;
            
        case internal::PROD_struct_member_list_1:
            _parse_struct_member_list(node->child(0), desc);
            _parse_struct_member(node->child(1), desc);
            break;
    }
}

void PacketScript::_parse_struct_member(lex::LR_Node *node, StructDesc &desc) {
    DataDesc dataType;
    _parse_type(node->child(0), dataType);
    
    std::list<str8> names;
    _parse_name_list(node->child(1), names);
    
    for (auto &x : names) {
        DataDesc member;
        member.typeName = dataType.typeName;
        member.type = dataType.type;
        member.isArray = dataType.isArray;
        member.arraySize = dataType.arraySize;
        member.dataName = std::move(x);
        desc.members.push_back(std::move(member));
    }
}

void PacketScript::_parse_name_list(lex::LR_Node *node, std::list<str8> &names) {
    switch (node->productionId()) {
        case internal::PROD_name_list_0:
            names.push_back(node->child(0)->token()->text);
            break;
            
        case internal::PROD_name_list_1:
            _parse_name_list(node->child(0), names);
            names.push_back(node->child(2)->token()->text);
            break;
    }
}

void PacketScript::_parse_type(lex::LR_Node *node, vn::PacketScript::DataDesc &desc) {
    switch (node->productionId()) {
        case internal::PROD_type_0:
            _parse_base_type(node->child(0), desc);
            break;
            
        case internal::PROD_type_1:
            _parse_base_type(node->child(2), desc);
            desc.isArray = true;
            break;
            
        case internal::PROD_type_2:
            _parse_base_type(node->child(2), desc);
            desc.isArray = true;
            desc.arraySize = node->child(4)->token()->int32;
            break;
    }
}

void PacketScript::_parse_base_type(lex::LR_Node *node, vn::PacketScript::DataDesc &desc) {
    switch (node->productionId()) {
        case internal::PROD_base_type_0:
            desc.type = kDT_bool;
            desc.typeName = "bool";
            break;
            
        case internal::PROD_base_type_1:
            desc.type = kDT_s8;
            desc.typeName = "s8";
            break;
            
        case internal::PROD_base_type_2:
            desc.type = kDT_u8;
            desc.typeName = "u8";
            break;
            
        case internal::PROD_base_type_3:
            desc.type = kDT_s16;
            desc.typeName = "s16";
            break;
            
        case internal::PROD_base_type_4:
            desc.type = kDT_u16;
            desc.typeName = "u16";
            break;
            
        case internal::PROD_base_type_5:
            desc.type = kDT_s32;
            desc.typeName = "s32";
            break;
            
        case internal::PROD_base_type_6:
            desc.type = kDT_u32;
            desc.typeName = "u32";
            break;
            
        case internal::PROD_base_type_7:
            desc.type = kDT_s64;
            desc.typeName = "s64";
            break;
            
        case internal::PROD_base_type_8:
            desc.type = kDT_u64;
            desc.typeName = "u64";
            break;
            
        case internal::PROD_base_type_9:
            desc.type = kDT_f32;
            desc.typeName = "f32";
            break;
            
        case internal::PROD_base_type_10:
            desc.type = kDT_f64;
            desc.typeName = "f64";
            break;
            
        case internal::PROD_base_type_11:
            desc.type = kDT_int;
            desc.typeName = "int";
            break;
            
        case internal::PROD_base_type_12:
            desc.type = kDT_uint;
            desc.typeName = "unsigned int";
            break;
            
        case internal::PROD_base_type_13:
            desc.type = kDT_string;
            desc.typeName = "str8";
            break;
            
        case internal::PROD_base_type_14:
            desc.type = kDT_structure;
            desc.typeName = node->child(0)->token()->text;
            break;
    }
}

void PacketScript::_parse_packet_def(lex::LR_Node *node) {
    PacketDesc desc;
    desc.pid = node->child(2)->token()->int32;
    desc.type = _parse_packet_type(node->child(0));
    switch (node->productionId()) {
        case internal::PROD_packet_def_0:
        case internal::PROD_packet_def_1:
            desc.name = node->child(4)->token()->text;
            break;
            
        case internal::PROD_packet_def_2:
        case internal::PROD_packet_def_3:
            desc.name = node->child(5)->token()->text;
            _parse_struct_body(node->child(4), desc);
            break;
            
        case internal::PROD_packet_def_4:
        case internal::PROD_packet_def_5:
            desc.name = node->child(6)->token()->text;
            _parse_type(node->child(4), desc.data);
            break;
    }
    m_packets.push_back(std::move(desc));
}

PacketScript::PacketType PacketScript::_parse_packet_type(lex::LR_Node *node) {
    switch (node->productionId()) {
        case internal::PROD_packet_type_1:
            return kPT_request;
        case internal::PROD_packet_type_2:
            return kPT_response;
        
    }
    return kPT_packet;
}


_vn_end
