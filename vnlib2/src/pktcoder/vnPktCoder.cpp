//
//  vnPktCoder.cpp
//  vnlib
//
//  Created by Wayne on 5/19/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnPacketScript.h"
#include "vnFileManager.h"

#include <iostream>
#include <sstream>
#include <cstring>

using namespace _vn_ns;

str8 generate_cpp(const std::vector<PacketScript::StructDesc> &structs, const std::vector<PacketScript::PacketDesc> &packets);
str8 generate_csharp(const std::vector<PacketScript::StructDesc> &structs, const std::vector<PacketScript::PacketDesc> &packets);

enum CodeType {
    kCT_cpp = 0,
    kCT_csharp
};

str8 g_procotolName;

int main(int argc, char *argv[]) {
    
    if (argc < 2) {
    l_error_usage:
        std::cout << "usage: vnpktcoder [options] input-file [output-path]" << std::endl;
        return -1;
    }
    CodeType codeType = kCT_cpp;
    
    int argi = 1;
    if (*argv[argi] == '-') {
        auto p = argv[argi] + 1;
        ++argi;
        if (!strcmp(p, "cs")) {
            codeType = kCT_csharp;
        } else {
            std::cout << "error: invalid option \"" << p << "\"" << std::endl;
            return -1;
         }
    }
    
    if (argi == argc) {
        goto l_error_usage;
    }
    
    FilePath inputFile;
    inputFile.fileName = argv[argi++];
    str8 outputFileName;
    
    size_t idx = inputFile.fileName.find_last_of("/\\");
    if (idx == str8::npos) {
        outputFileName = inputFile.fileName;
    } else {
        outputFileName = inputFile.fileName.substr(idx + 1);
    }
    idx = outputFileName.find_last_of('.');
    if (idx != str8::npos) {
        outputFileName.erase(outputFileName.begin() + idx, outputFileName.end());
    }
    g_procotolName = outputFileName;
    switch (codeType) {
        case kCT_cpp:
            outputFileName += ".hpp";
            break;
            
        case kCT_csharp:
            outputFileName += ".cs";
            break;
    }
    
    FilePath outputFile;
    
    if (argi < argc) {
        outputFile.fileName = argv[argi];
        if (!outputFile.fileName.empty()) {
            if (outputFile.fileName.back() != '/' && outputFile.fileName.back() != '\\') {
                outputFile.fileName.push_back('/');
            }
        }
    }
    
    outputFile.fileName += outputFileName;
    
    std::vector<PacketScript::StructDesc> structs;
    std::vector<PacketScript::PacketDesc> packets;
    
    PacketScript ps;
    if (!ps.parse(inputFile, structs, packets)) {
        return -1;
    }
    
    str8 text;
    
    switch (codeType) {
        case kCT_cpp:
            text = generate_cpp(structs, packets);
            break;
            
        case kCT_csharp:
            text = generate_csharp(structs, packets);
            break;
    }

    
    std::cout << outputFileName;
    
    FileStream *fs = FileManager::instance().open(outputFile);
    if (fs) {
        str8 t;
        t.resize((size_t)fs->size());
        if (!t.empty()) {
            *fs >> DataStream::buf(const_cast<c8 *>(t.data()), t.size());
        }
        fs->close();
        if (t == text) {
            std::cout << " - [not changed]" << std::endl;
            return 0;
        }
    }
    fs = FileManager::instance().create(outputFile);
    if (!fs) {
        std::cout << " - [open failed]" << std::endl;
        return -1;
    }
    *fs << DataStream::cbuf(text.data(), text.length());
    fs->close();
    std::cout << " - [ok]" << std::endl;
    return 0;
}

#define indent str8(tab * 4, ' ')

//str8 generate_cpp_struct_members(const PacketScript::StructDesc &desc, int tab) {
//    std::ostringstream oss;
//    for (auto &i : desc.members) {
//        oss << indent;
//        if (i.isArray) {
//            if (i.arraySize) {
//                oss << "std::array<" << i.typeName << ", " << i.arraySize << ">";
//            } else {
//                oss << "std::vector<" << i.typeName << ">";
//            }
//        } else {
//            oss << i.typeName;
//        }
//        oss << " " << i.dataName << ";" << std::endl;
//    }
//    return oss.str();
//}

str8 generate_cpp_struct_function(const PacketScript::StructDesc &desc, int tab) {
    std::ostringstream oss;
    oss << indent << "void _save(Variable_object &_vobj) const {" << std::endl;
    ++tab;
    for (auto &i : desc.members) {
        oss << indent << "PacketDataHelper<" << i.typeName << ">::save(_vobj, \"" << i.dataName << "\", " << i.dataName << ");" << std::endl;
    }
    --tab;
    oss << indent << "}" << std::endl;
    oss << indent << "void _load(Variable_object &_vobj) {" << std::endl;
    ++tab;
    for (auto &i : desc.members) {
        oss << indent << "PacketDataHelper<" << i.typeName << ">::load(_vobj, \"" << i.dataName << "\", " << i.dataName << ");" << std::endl;
    }
    --tab;
    oss << indent << "}" << std::endl;
    return oss.str();
}

str8 generate_cpp(const std::vector<PacketScript::StructDesc> &structs, const std::vector<PacketScript::PacketDesc> &packets) {
    std::ostringstream oss;
    
    oss << "// generated by pktcoder." << std::endl;
    
    for (auto &i : structs) {
        bool hasArray = false;
        oss << "struct " << i.name << " {" << std::endl;
        for (auto &j : i.members) {
            oss << str8(4, ' ');
            if (j.isArray) {
                hasArray = true;
                if (j.arraySize) {
                    oss << "std::array<" << j.typeName << ", " << j.arraySize << ">";
                } else {
                    oss << "std::vector<" << j.typeName << ">";
                }
            } else {
                oss << j.typeName;
            }
            oss << " " << j.dataName << ";" << std::endl;
        }
        oss << generate_cpp_struct_function(i, 1);
        oss << "};" << std::endl;
        oss << "inline DataStream & operator <<(DataStream &s, const " << i.name << " &v) {" << std::endl;
        for (auto &j : i.members) {
            if (j.isArray) {
                if (!j.arraySize) {
                    oss << str8(4, ' ') << "s << DataStream::ccu(v." << j.dataName << ".size());" << std::endl;
                }
                if (j.type == PacketScript::kDT_int) {
                    oss << str8(4, ' ') << "for (auto &i : v." << j.dataName << ") s << DataStream::cci(i);" << std::endl;
                } else if (j.type == PacketScript::kDT_uint) {
                    oss << str8(4, ' ') << "for (auto &i : v." << j.dataName << ") s << DataStream::ccu(i);" << std::endl;
                } else if (j.type == PacketScript::kDT_bool || j.type == PacketScript::kDT_string || j.type == PacketScript::kDT_structure) {
                    oss << str8(4, ' ') << "for (auto &i : v." << j.dataName << ") s << i;" << std::endl;
                } else {
                    if (j.arraySize) {
                        oss << str8(4, ' ') << "s << DataStream::cbuf(v." << j.dataName << ".data(), sizeof(" << j.typeName << ") * " << j.arraySize << ");" << std::endl;
                    } else {
                        oss << str8(4, ' ') << "if (!v." << j.dataName << ".empty()) s << DataStream::cbuf(v." << j.dataName << ".data(), v." << j.dataName << ".size() * sizeof(" << j.typeName << "));" << std::endl;
                    }
                    
                }
            } else {
                if (j.type == PacketScript::kDT_int) {
                    oss << str8(4, ' ') << "s << DataStream::cci(v." << j.dataName << ");" << std::endl;
                } else if (j.type == PacketScript::kDT_uint) {
                    oss << str8(4, ' ') << "s << DataStream::ccu(v." << j.dataName << ");" << std::endl;
                } else {
                    oss << str8(4, ' ') << "s << v." << j.dataName << ";" << std::endl;
                }
            }
        }
        oss << str8(4, ' ') << "return s;" << std::endl;
        oss << "}" << std::endl;
        oss << "inline DataStream & operator >>(DataStream &s, " << i.name << " &v) {" << std::endl;
        if (hasArray) {
            oss << str8(4, ' ') << "size_t c; (void)c;" << std::endl;
        }
        for (auto &j : i.members) {
            if (j.isArray) {
                if (!j.arraySize) {
                    oss << str8(4, ' ') << "s >> DataStream::cu(c);" << std::endl;
                    oss << str8(4, ' ') << "v." << j.dataName << ".resize(c);" << std::endl;
                }
                if (j.type == PacketScript::kDT_int) {
                    oss << str8(4, ' ') << "for (auto &i : v." << j.dataName << ") s >> DataStream::ci(i);" << std::endl;
                } else if (j.type == PacketScript::kDT_uint) {
                    oss << str8(4, ' ') << "for (auto &i : v." << j.dataName << ") s >> DataStream::cu(i);" << std::endl;
                } else if (j.type == PacketScript::kDT_bool || j.type == PacketScript::kDT_string || j.type == PacketScript::kDT_structure) {
                    oss << str8(4, ' ') << "for (auto &i : v." << j.dataName << ") s >> i;" << std::endl;
                } else {
                    if (j.arraySize) {
                        oss << str8(4, ' ') << "s >> DataStream::buf(v." << j.dataName << ".data(), sizeof(" << j.typeName << ") * " << j.arraySize << ");" << std::endl;
                    } else {
                        oss << str8(4, ' ') << "if (c) s >> DataStream::buf(v." << j.dataName << ".data(), c * sizeof(" << j.typeName << "));" << std::endl;
                    }
                }
            } else {
                if (j.type == PacketScript::kDT_int) {
                    oss << str8(4, ' ') << "s >> DataStream::ci(v." << j.dataName << ");" << std::endl;
                } else if (j.type == PacketScript::kDT_uint) {
                    oss << str8(4, ' ') << "s >> DataStream::cu(v." << j.dataName << ");" << std::endl;
                } else {
                    oss << str8(4, ' ') << "s >> v." << j.dataName << ";" << std::endl;
                }
            }
        }
        oss << str8(4, ' ') << "return s;" << std::endl;
        oss << "}" << std::endl;
    }
    
    for (auto &i : packets) {
        oss << "class " << i.name << " : public Packet {" << std::endl;
        oss << "public:" << std::endl;
        oss << str8(4, ' ') << "enum { ID = " << i.pid << " };" << std::endl;
        oss << str8(4, ' ') << "virtual u32 pid() const { return ID; }" << std::endl;
        
        if (i.data.type != PacketScript::kDT_void) {
            oss << str8(4, ' ');
            if (i.data.isArray) {
                if (i.data.arraySize) {
                    oss << "std::array<" << i.data.typeName << ", " << i.data.arraySize << ">";
                } else {
                    oss << "std::vector<" << i.data.typeName << ">";
                }
            } else {
                oss << i.data.typeName;
            }
            oss << " data;" << std::endl;
            oss << str8(4, ' ') << "virtual void save(DataStream &s) const {" << std::endl;
            if (i.data.isArray) {
                if (!i.data.arraySize) {
                    oss << str8(8, ' ') << "s << DataStream::ccu(data.size());" << std::endl;
                }
                if (i.data.type == PacketScript::kDT_int) {
                    oss << str8(8, ' ') << "for (auto &i : data) s << DataStream::cci(i);" << std::endl;
                } else if (i.data.type == PacketScript::kDT_uint) {
                    oss << str8(8, ' ') << "for (auto &i : data) s << DataStream::ccu(i);" << std::endl;
                } else if (i.data.type == PacketScript::kDT_bool || i.data.type == PacketScript::kDT_string || i.data.type == PacketScript::kDT_structure) {
                    oss << str8(8, ' ') << "for (auto &i : data) s << i;" << std::endl;
                } else {
                    if (i.data.arraySize) {
                        oss << str8(8, ' ') << "s << DataStream::cbuf(data.data(), sizeof(" << i.data.typeName << ") * " << i.data.arraySize << ");" << std::endl;
                    } else {
                        oss << str8(8, ' ') << "if (!data.empty()) s << DataStream::cbuf(data.data(), data.size() * sizeof(" << i.data.typeName << "));" << std::endl;
                    }
                }
            } else {
                if (i.data.type == PacketScript::kDT_int) {
                    oss << str8(8, ' ') << "s << DataStream::cci(data);" << std::endl;
                } else if (i.data.type == PacketScript::kDT_uint) {
                    oss << str8(8, ' ') << "s << DataStream::ccu(data);" << std::endl;
                } else {
                    oss << str8(8, ' ') << "s << data;" << std::endl;
                }
            }
            oss << str8(4, ' ') << "}" << std::endl;
            oss << str8(4, ' ') << "virtual void load(DataStream &s) {" << std::endl;
            if (i.data.isArray) {
                if (!i.data.arraySize) {
                    oss << str8(8, ' ') << "size_t c;" << std::endl;
                    oss << str8(8, ' ') << "s >> DataStream::cu(c);" << std::endl;
                    oss << str8(8, ' ') << "data.resize(c);" << std::endl;
                }
                if (i.data.type == PacketScript::kDT_int) {
                    oss << str8(8, ' ') << "for (auto &i : data) s >> DataStream::ci(i);" << std::endl;
                } else if (i.data.type == PacketScript::kDT_uint) {
                    oss << str8(8, ' ') << "for (auto &i : data) s >> DataStream::cu(i);" << std::endl;
                } else if (i.data.type == PacketScript::kDT_bool || i.data.type == PacketScript::kDT_string || i.data.type == PacketScript::kDT_structure) {
                    oss << str8(8, ' ') << "for (auto &i : data) s >> i;" << std::endl;
                } else {
                    if (i.data.arraySize) {
                        oss << str8(8, ' ') << "s >> DataStream::buf(data.data(), sizeof(" << i.data.typeName << ") * " << i.data.arraySize << ");" << std::endl;
                    } else {
                        oss << str8(8, ' ') << "if (c) s >> DataStream::buf(data.data(), c * sizeof(" << i.data.typeName << "));" << std::endl;
                    }
                }
            } else {
                if (i.data.type == PacketScript::kDT_int) {
                    oss << str8(8, ' ') << "s >> DataStream::ci(data);" << std::endl;
                } else if (i.data.type == PacketScript::kDT_uint) {
                    oss << str8(8, ' ') << "s >> DataStream::cu(data);" << std::endl;
                } else {
                    oss << str8(8, ' ') << "s >> data;" << std::endl;
                }
            }
            oss << str8(4, ' ') << "}" << std::endl;
            oss << str8(4, ' ') << "virtual void save(Variable_object &vobj) const {" << std::endl;
            oss << str8(8, ' ') << "PacketDataHelper<" << i.data.typeName << ">::save(vobj, \"data\", data);" << std::endl;
            oss << str8(4, ' ') << "}" << std::endl;
            oss << str8(4, ' ') << "virtual void load(Variable_object &vobj) {" << std::endl;
            oss << str8(8, ' ') << "PacketDataHelper<" << i.data.typeName << ">::load(vobj, \"data\", data);" << std::endl;
            oss << str8(4, ' ') << "}" << std::endl;
            
        } else if (!i.members.empty()) {
            
            bool hasArray = false;
            oss << str8(4, ' ') << "struct _Data {" << std::endl;
            for (auto &j : i.members) {
                oss << str8(8, ' ');
                if (j.isArray) {
                    hasArray = true;
                    if (j.arraySize) {
                        oss << "std::array<" << j.typeName << ", " << j.arraySize << ">";
                    } else {
                        oss << "std::vector<" << j.typeName << ">";
                    }
                } else {
                    oss << j.typeName;
                }
                oss << " " << j.dataName << ";" << std::endl;
            }
            oss << generate_cpp_struct_function(i, 2);
            oss << str8(4, ' ') << "} data;" << std::endl;
            oss << str8(4, ' ') << "virtual void save(DataStream &s) const {" << std::endl;
            oss << str8(8, ' ') << "auto &v = data;" << std::endl;
            for (auto &j : i.members) {
                if (j.isArray) {
                    if (!j.arraySize) {
                        oss << str8(8, ' ') << "s << DataStream::ccu(v." << j.dataName << ".size());" << std::endl;
                    }
                    if (j.type == PacketScript::kDT_int) {
                        oss << str8(8, ' ') << "for (auto &i : v." << j.dataName << ") s << DataStream::cci(i);" << std::endl;
                    } else if (j.type == PacketScript::kDT_uint) {
                        oss << str8(8, ' ') << "for (auto &i : v." << j.dataName << ") s << DataStream::ccu(i);" << std::endl;
                    } else if (j.type == PacketScript::kDT_bool || j.type == PacketScript::kDT_string || j.type == PacketScript::kDT_structure) {
                        oss << str8(8, ' ') << "for (auto &i : v." << j.dataName << ") s << i;" << std::endl;
                    } else {
                        if (j.arraySize) {
                            oss << str8(8, ' ') << "s << DataStream::cbuf(v." << j.dataName << ".data(), sizeof(" << j.typeName << ") * " << j.arraySize << ");" << std::endl;
                        } else {
                            oss << str8(8, ' ') << "if (!v." << j.dataName << ".empty()) s << DataStream::cbuf(v." << j.dataName << ".data(), v." << j.dataName << ".size() * sizeof(" << j.typeName << "));" << std::endl;
                        }
                        
                    }
                } else {
                    if (j.type == PacketScript::kDT_int) {
                        oss << str8(8, ' ') << "s << DataStream::cci(v." << j.dataName << ");" << std::endl;
                    } else if (j.type == PacketScript::kDT_uint) {
                        oss << str8(8, ' ') << "s << DataStream::ccu(v." << j.dataName << ");" << std::endl;
                    } else {
                        oss << str8(8, ' ') << "s << v." << j.dataName << ";" << std::endl;
                    }
                }
            }
            oss << str8(4, ' ') << "}" << std::endl;
            oss << str8(4, ' ') << "virtual void load(DataStream &s) {" << std::endl;
            oss << str8(8, ' ') << "auto &v = data;" << std::endl;
            if (hasArray) {
                oss << str8(8, ' ') << "size_t c; (void)c;" << std::endl;
            }
            for (auto &j : i.members) {
                if (j.isArray) {
                    if (!j.arraySize) {
                        oss << str8(8, ' ') << "s >> DataStream::cu(c);" << std::endl;
                        oss << str8(8, ' ') << "v." << j.dataName << ".resize(c);" << std::endl;
                    }
                    if (j.type == PacketScript::kDT_int) {
                        oss << str8(8, ' ') << "for (auto &i : v." << j.dataName << ") s >> DataStream::ci(i);" << std::endl;
                    } else if (j.type == PacketScript::kDT_uint) {
                        oss << str8(8, ' ') << "for (auto &i : v." << j.dataName << ") s >> DataStream::cu(i);" << std::endl;
                    } else if (j.type == PacketScript::kDT_bool || j.type == PacketScript::kDT_string || j.type == PacketScript::kDT_structure) {
                        oss << str8(8, ' ') << "for (auto &i : v." << j.dataName << ") s >> i;" << std::endl;
                    } else {
                        if (j.arraySize) {
                            oss << str8(8, ' ') << "s >> DataStream::buf(v." << j.dataName << ".data(), sizeof(" << j.typeName << ") * " << j.arraySize << ");" << std::endl;
                        } else {
                            oss << str8(8, ' ') << "if (c) s >> DataStream::buf(v." << j.dataName << ".data(), c * sizeof(" << j.typeName << "));" << std::endl;
                        }
                    }
                } else {
                    if (j.type == PacketScript::kDT_int) {
                        oss << str8(8, ' ') << "s >> DataStream::ci(v." << j.dataName << ");" << std::endl;
                    } else if (j.type == PacketScript::kDT_uint) {
                        oss << str8(8, ' ') << "s >> DataStream::cu(v." << j.dataName << ");" << std::endl;
                    } else {
                        oss << str8(8, ' ') << "s >> v." << j.dataName << ";" << std::endl;
                    }
                }
            }
            oss << str8(4, ' ') << "}" << std::endl;
            oss << str8(4, ' ') << "virtual void save(Variable_object &vobj) const { data._save(vobj); }" << std::endl;
            oss << str8(4, ' ') << "virtual void load(Variable_object &vobj) { data._load(vobj); }" << std::endl;
        }
        
//        if (i.body.type != PacketScript::kDT_void) {
//            oss << str8(4, ' ') << "virtual void save(DataStream &s) const {" << std::endl;
//            auto &j = i.body;
//            if (j.isArray) {
//                oss << str8(8, ' ') << "s << DataStream::ccu(data.size());" << std::endl;
//                if (j.type == PacketScript::kDT_int) {
//                    oss << str8(8, ' ') << "for (auto &i : data) s << DataStream::cci(i);" << std::endl;
//                } else if (j.type == PacketScript::kDT_uint) {
//                    oss << str8(8, ' ') << "for (auto &i : data) s << DataStream::ccu(i);" << std::endl;
//                } else if (j.type == PacketScript::kDT_bool || j.type == PacketScript::kDT_string || j.type == PacketScript::kDT_structure) {
//                    oss << str8(8, ' ') << "for (auto &i : data) s << i;" << std::endl;
//                } else {
//                    oss << str8(8, ' ') << "if (!data.empty()) s << DataStream::cbuf(data.data(), data.size() * sizeof(" << j.typeName << "));" << std::endl;
//                }
//            } else {
//                if (j.type == PacketScript::kDT_int) {
//                    oss << str8(8, ' ') << "s << DataStream::cci(data);" << std::endl;
//                } else if (j.type == PacketScript::kDT_uint) {
//                    oss << str8(8, ' ') << "s << DataStream::ccu(data);" << std::endl;
//                } else {
//                    oss << str8(8, ' ') << "s << data;" << std::endl;
//                }
//            }
//            oss << str8(4, ' ') << "}" << std::endl;
//            
//            oss << str8(4, ' ') << "virtual void load(DataStream &s) {" << std::endl;
//            if (j.isArray) {
//                oss << str8(8, ' ') << "size_t c;" << std::endl;
//                oss << str8(8, ' ') << "s >> DataStream::cu(c);" << std::endl;
//                oss << str8(8, ' ') << "data.resize(c);" << std::endl;
//                if (j.type == PacketScript::kDT_int) {
//                    oss << str8(8, ' ') << "for (auto &i : data) s >> DataStream::ci(i);" << std::endl;
//                } else if (j.type == PacketScript::kDT_uint) {
//                    oss << str8(8, ' ') << "for (auto &i : data) s >> DataStream::cu(i);" << std::endl;
//                } else if (j.type == PacketScript::kDT_bool || j.type == PacketScript::kDT_string || j.type == PacketScript::kDT_structure) {
//                    oss << str8(8, ' ') << "for (auto &i : data) s >> i;" << std::endl;
//                } else {
//                    oss << str8(12, ' ') << "if (c) s >> DataStream::buf(data.data(), c * sizeof(" << j.typeName << "));" << std::endl;
//                }
//            } else {
//                if (j.type == PacketScript::kDT_int) {
//                    oss << str8(8, ' ') << "s >> DataStream::ci(data);" << std::endl;
//                } else if (j.type == PacketScript::kDT_uint) {
//                    oss << str8(8, ' ') << "s >> DataStream::cu(data);" << std::endl;
//                } else {
//                    oss << str8(8, ' ') << "s >> data;" << std::endl;
//                }
//            }
//            oss << str8(4, ' ') << "}" << std::endl;
//            
//            if (j.isArray) {
//                oss << str8(4, ' ') << "std::vector<" << j.typeName << "> data;" << std::endl;
//            } else {
//                oss << str8(4, ' ') << j.typeName << " data;" << std::endl;
//            }
//        }
        oss << "};" << std::endl;
    }
    oss << "template <class _Handler>" << std::endl;
    oss << "void _reg_" << g_procotolName << "_requests() {" << std::endl;
    oss << str8(4, ' ') << "auto &_ins = PacketFactory<_Handler>::instance();" << std::endl;
    for (auto &i : packets) {
        if (i.type == PacketScript::kPT_request) {
            oss << str8(4, ' ') << "_ins.registerRequestNoProcess<" << i.name << ">();" << std::endl;
        }
    }
    oss << "}" << std::endl;
    oss << "template <class _Handler>" << std::endl;
    oss << "void _reg_" << g_procotolName << "_responses() {" << std::endl;
    oss << str8(4, ' ') << "auto &_ins = PacketFactory<_Handler>::instance();" << std::endl;
    for (auto &i : packets) {
        if (i.type == PacketScript::kPT_response) {
            oss << str8(4, ' ') << "_ins.registerResponseNoProcess<" << i.name << ">();" << std::endl;
        }
    }
    oss << "}" << std::endl;
    return oss.str();
}