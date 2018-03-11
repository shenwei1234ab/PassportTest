//
//  vnPacketScript.h
//  vnlib
//
//  Created by Wayne on 5/19/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnPacketScript_h
#define vnlib_vnPacketScript_h

#include "vnLexer.h"
#include "vnLexLR_Parser.h"
#include "vnFilePath.h"

_vn_begin

class PacketScript : public AllocatedObject {
public:
    enum DataType {
        kDT_void = 0,
        kDT_bool,
        kDT_s8,
        kDT_u8,
        kDT_s16,
        kDT_u16,
        kDT_s32,
        kDT_u32,
        kDT_s64,
        kDT_u64,
        kDT_f32,
        kDT_f64,
        kDT_int,
        kDT_uint,
        kDT_string,
        kDT_structure,
    };
    
    struct DataDesc {
		DataType type = kDT_void;
		bool isArray = false;
        size_t arraySize = 0;
		str8 typeName;
		str8 dataName;
        
        DataDesc() = default;
        DataDesc(const DataDesc &) = default;
        DataDesc(DataDesc &&s) : type(s.type), isArray(s.isArray), arraySize(s.arraySize), typeName(std::move(s.typeName)), dataName(std::move(s.dataName)) {}
	};
    
    struct StructDesc {
        str8 name;
        std::vector<DataDesc> members;
        
        StructDesc() = default;
        StructDesc(const StructDesc &) = default;
        StructDesc(StructDesc &&s) : name(std::move(s.name)), members(std::move(s.members)) {}
    };
    
    enum PacketType {
        kPT_packet = 0,
        kPT_request,
        kPT_response
    };
    
    struct PacketDesc : StructDesc {
        u32 pid = 0;
        DataDesc data;
        PacketType type = kPT_packet;
        PacketDesc() = default;
        PacketDesc(const PacketDesc &) = default;
        PacketDesc(PacketDesc &&s) : StructDesc(std::move(s)), pid(s.pid), data(std::move(s.data)), type(s.type) {}
    };
    
    PacketScript();
    
    bool parse(const FilePath &file, std::vector<StructDesc> &structs, std::vector<PacketDesc> &packets);
    
    bool parse(const c8 *text, size_t length, const lex::Token::FilePtr &file, std::vector<StructDesc> &structs, std::vector<PacketDesc> &packets);
    
protected:
    lex::Lexer m_lexer;
    lex::LR_Parser m_parser;

    std::list<StructDesc> m_structs;
    std::list<PacketDesc> m_packets;
    
    
    void _parse_root(lex::LR_Node *root);
    void _parse_definition(lex::LR_Node *node);
    void _parse_struct_def(lex::LR_Node *node);
    void _parse_packet_def(lex::LR_Node *node);
    static void _parse_struct_body(lex::LR_Node *node, StructDesc &desc);
    static void _parse_struct_member_list(lex::LR_Node *node, StructDesc &desc);
    static void _parse_struct_member(lex::LR_Node *node, StructDesc &desc);
    static void _parse_name_list(lex::LR_Node *node, std::list<str8> &names);
    static void _parse_type(lex::LR_Node *node, DataDesc &desc);
    static void _parse_base_type(lex::LR_Node *node, DataDesc &desc);
    static PacketType _parse_packet_type(lex::LR_Node *node);
};

_vn_end

#endif
