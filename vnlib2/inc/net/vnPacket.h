//
//  vnPacket.h
//  vnlib
//
//  Created by Wayne on 14-5-15.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnPacket_h
#define vnlib_vnPacket_h

#include "vnRefCounted.h"
#include "vnDataStream.h"
#include "vnVariable.h"
#include "vnVariableAccessor.h"

#include <vector>
#include <array>

_vn_begin

class Packet : public RefCounted {
public:
	virtual u32 pid() const = 0;
	virtual void save(DataStream &stream) const {}
	virtual void load(DataStream &stream) {}
	virtual void process(void *param) {}
    
    virtual void save(Variable_object &vobj) const {}
    virtual void load(Variable_object &vobj) {}
};

typedef RefCountedPtr<Packet> PacketPtr;

template <typename T>
struct PacketDataHelper {
    static void save(Variable_object &vobj, const str8 &name, const T &value) {
        auto p = vnnew Variable_object();
        vobj.setAttribute(name, p);
        value._save(*p);
    }
    
    static void load(Variable_object &vobj, const str8 &name, T &value) {
        auto p = vobj.queryAttributeObject(name);
        if (p) {
            value._load(*p);
        }
    }
    
    static void save(Variable_object &vobj, const str8 &name, const std::vector<T> &value) {
        auto p = vnnew Variable_object();
        vobj.setAttribute(name, p);
        for (auto &v : value) {
            auto q = vnnew Variable_object();
            p->add(q);
            v._save(*q);
        }
    }
    
    static void load(Variable_object &vobj, const str8 &name, std::vector<T> &value) {
        VariableAccessor<Variable_object> va(vobj.queryAttributeObject(name));
        while (va.fetch()) {
            value.push_back(T());
            value.back()._load(*va.at<0>());
        }
    }
    
    template <size_t size>
    static void save(Variable_object &vobj, const str8 &name, const std::array<T, size> &value) {
        auto p = vnnew Variable_object();
        vobj.setAttribute(name, p);
        for (auto &v : value) {
            auto q = vnnew Variable_object();
            p->add(q);
            v._save(*q);
        }
    }
    
    template <size_t size>
    static void load(Variable_object &vobj, const str8 &name, std::array<T, size> &value) {
        VariableAccessor<Variable_object> va(vobj.queryAttributeObject(name));
        Variable_object empty;
        for (auto &i : value) {
            if (va.fetch()) {
                i._load(*va.at<0>());
            } else {
                i._load(empty);
            }
        }
    }
};

template <>
struct PacketDataHelper<bool> {
    static void save(Variable_object &vobj, const str8 &name, bool value) {
        vobj.setAttribute(name, vnnew Variable_bool(value));
    }
    static void load(Variable_object &vobj, const str8 &name, bool &value) {
        value = vobj.queryAttributeBool(name);
    }
    
    static void save(Variable_object &vobj, const str8 &name, const std::vector<bool> &value) {
        auto p = vnnew Variable_object();
        vobj.setAttribute(name, p);
        for (auto v : value) {
            p->add(vnnew Variable_bool(v));
        }
    }
    
    static void load(Variable_object &vobj, const str8 &name, std::vector<bool> &value) {
        VariableAccessor<Variable_bool> va(vobj.queryAttributeObject(name));
        while (va.fetch()) {
            value.push_back(va.at<0>()->value());
        }
    }
    
    template <size_t size>
    static void save(Variable_object &vobj, const str8 &name, const std::array<bool, size> &value) {
        auto p = vnnew Variable_object();
        vobj.setAttribute(name, p);
        for (auto v : value) {
            p->add(vnnew Variable_bool(v));
        }
    }
    
    template <size_t size>
    static void load(Variable_object &vobj, const str8 &name, std::array<bool, size> &value) {
        VariableAccessor<Variable_bool> va(vobj.queryAttributeObject(name));
        for (auto &i : value) {
            if (va.fetch()) {
                i = va.at<0>()->value();
            } else {
                i = false;
            }
        }
    }
};

template <>
struct PacketDataHelper<u8> {
    static void save(Variable_object &vobj, const str8 &name, u8 value) {
        vobj.setAttribute(name, vnnew Variable_int32(value));
    }
    static void load(Variable_object &vobj, const str8 &name, u8 &value) {
        value = (u8)vobj.queryAttributeInt32(name);
    }
    
    static void save(Variable_object &vobj, const str8 &name, const std::vector<u8> &value) {
        auto p = vnnew Variable_object();
        vobj.setAttribute(name, p);
        for (auto v : value) {
            p->add(vnnew Variable_int32(v));
        }
    }
    
    static void load(Variable_object &vobj, const str8 &name, std::vector<u8> &value) {
        VariableAccessor<Variable_int32> va(vobj.queryAttributeObject(name));
        while (va.fetch()) {
            value.push_back((u8)va.at<0>()->value());
        }
    }
    
    template <size_t size>
    static void save(Variable_object &vobj, const str8 &name, const std::array<u8, size> &value) {
        auto p = vnnew Variable_object();
        vobj.setAttribute(name, p);
        for (auto v : value) {
            p->add(vnnew Variable_int32(v));
        }
    }
    
    template <size_t size>
    static void load(Variable_object &vobj, const str8 &name, std::array<u8, size> &value) {
        VariableAccessor<Variable_int32> va(vobj.queryAttributeObject(name));
        for (auto &i : value) {
            if (va.fetch()) {
                i = (u8)va.at<0>()->value();
            } else {
                i = 0;
            }
        }
    }

};

template <>
struct PacketDataHelper<s8> {
    static void save(Variable_object &vobj, const str8 &name, s8 value) {
        vobj.setAttribute(name, vnnew Variable_int32(value));
    }
    static void load(Variable_object &vobj, const str8 &name, s8 &value) {
        value = (s8)vobj.queryAttributeInt32(name);
    }
    
    static void save(Variable_object &vobj, const str8 &name, const std::vector<s8> &value) {
        auto p = vnnew Variable_object();
        vobj.setAttribute(name, p);
        for (auto v : value) {
            p->add(vnnew Variable_int32(v));
        }
    }
    
    static void load(Variable_object &vobj, const str8 &name, std::vector<s8> &value) {
        VariableAccessor<Variable_int32> va(vobj.queryAttributeObject(name));
        while (va.fetch()) {
            value.push_back((s8)va.at<0>()->value());
        }
    }
    
    template <size_t size>
    static void save(Variable_object &vobj, const str8 &name, const std::array<s8, size> &value) {
        auto p = vnnew Variable_object();
        vobj.setAttribute(name, p);
        for (auto v : value) {
            p->add(vnnew Variable_int32(v));
        }
    }
    
    template <size_t size>
    static void load(Variable_object &vobj, const str8 &name, std::array<s8, size> &value) {
        VariableAccessor<Variable_int32> va(vobj.queryAttributeObject(name));
        for (auto &i : value) {
            if (va.fetch()) {
                i = (s8)va.at<0>()->value();
            } else {
                i = 0;
            }
        }
    }
};

template <>
struct PacketDataHelper<u16> {
    static void save(Variable_object &vobj, const str8 &name, u16 value) {
        vobj.setAttribute(name, vnnew Variable_int32(value));
    }
    static void load(Variable_object &vobj, const str8 &name, u16 &value) {
        value = (u16)vobj.queryAttributeInt32(name);
    }
    
    static void save(Variable_object &vobj, const str8 &name, const std::vector<u16> &value) {
        auto p = vnnew Variable_object();
        vobj.setAttribute(name, p);
        for (auto v : value) {
            p->add(vnnew Variable_int32(v));
        }
    }
    
    static void load(Variable_object &vobj, const str8 &name, std::vector<u16> &value) {
        VariableAccessor<Variable_int32> va(vobj.queryAttributeObject(name));
        while (va.fetch()) {
            value.push_back((u16)va.at<0>()->value());
        }
    }
    
    template <size_t size>
    static void save(Variable_object &vobj, const str8 &name, const std::array<u16, size> &value) {
        auto p = vnnew Variable_object();
        vobj.setAttribute(name, p);
        for (auto v : value) {
            p->add(vnnew Variable_int32(v));
        }
    }
    
    template <size_t size>
    static void load(Variable_object &vobj, const str8 &name, std::array<u16, size> &value) {
        VariableAccessor<Variable_int32> va(vobj.queryAttributeObject(name));
        for (auto &i : value) {
            if (va.fetch()) {
                i = (u16)va.at<0>()->value();
            } else {
                i = 0;
            }
        }
    }
};

template <>
struct PacketDataHelper<s16> {
    static void save(Variable_object &vobj, const str8 &name, s16 value) {
        vobj.setAttribute(name, vnnew Variable_int32(value));
    }
    static void load(Variable_object &vobj, const str8 &name, s16 &value) {
        value = (s16)vobj.queryAttributeInt32(name);
    }
    
    static void save(Variable_object &vobj, const str8 &name, const std::vector<s16> &value) {
        auto p = vnnew Variable_object();
        vobj.setAttribute(name, p);
        for (auto v : value) {
            p->add(vnnew Variable_int32(v));
        }
    }
    
    static void load(Variable_object &vobj, const str8 &name, std::vector<s16> &value) {
        VariableAccessor<Variable_int32> va(vobj.queryAttributeObject(name));
        while (va.fetch()) {
            value.push_back((s16)va.at<0>()->value());
        }
    }
    
    template <size_t size>
    static void save(Variable_object &vobj, const str8 &name, const std::array<s16, size> &value) {
        auto p = vnnew Variable_object();
        vobj.setAttribute(name, p);
        for (auto v : value) {
            p->add(vnnew Variable_int32(v));
        }
    }
    
    template <size_t size>
    static void load(Variable_object &vobj, const str8 &name, std::array<s16, size> &value) {
        VariableAccessor<Variable_int32> va(vobj.queryAttributeObject(name));
        for (auto &i : value) {
            if (va.fetch()) {
                i = (s16)va.at<0>()->value();
            } else {
                i = 0;
            }
        }
    }
};

template <>
struct PacketDataHelper<u32> {
    static void save(Variable_object &vobj, const str8 &name, u32 value) {
        vobj.setAttribute(name, vnnew Variable_int32(value));
    }
    static void load(Variable_object &vobj, const str8 &name, u32 &value) {
        value = (u32)vobj.queryAttributeInt32(name);
    }
    
    static void save(Variable_object &vobj, const str8 &name, const std::vector<u32> &value) {
        auto p = vnnew Variable_object();
        vobj.setAttribute(name, p);
        for (auto v : value) {
            p->add(vnnew Variable_int32(v));
        }
    }
    
    static void load(Variable_object &vobj, const str8 &name, std::vector<u32> &value) {
        VariableAccessor<Variable_int32> va(vobj.queryAttributeObject(name));
        while (va.fetch()) {
            value.push_back((u32)va.at<0>()->value());
        }
    }
    
    template <size_t size>
    static void save(Variable_object &vobj, const str8 &name, const std::array<u32, size> &value) {
        auto p = vnnew Variable_object();
        vobj.setAttribute(name, p);
        for (auto v : value) {
            p->add(vnnew Variable_int32(v));
        }
    }
    
    template <size_t size>
    static void load(Variable_object &vobj, const str8 &name, std::array<u32, size> &value) {
        VariableAccessor<Variable_int32> va(vobj.queryAttributeObject(name));
        for (auto &i : value) {
            if (va.fetch()) {
                i = (u32)va.at<0>()->value();
            } else {
                i = 0;
            }
        }
    }
};

template <>
struct PacketDataHelper<s32> {
    static void save(Variable_object &vobj, const str8 &name, s32 value) {
        vobj.setAttribute(name, vnnew Variable_int32(value));
    }
    static void load(Variable_object &vobj, const str8 &name, s32 &value) {
        value = (s32)vobj.queryAttributeInt32(name);
    }
    
    static void save(Variable_object &vobj, const str8 &name, const std::vector<s32> &value) {
        auto p = vnnew Variable_object();
        vobj.setAttribute(name, p);
        for (auto v : value) {
            p->add(vnnew Variable_int32(v));
        }
    }
    
    static void load(Variable_object &vobj, const str8 &name, std::vector<s32> &value) {
        VariableAccessor<Variable_int32> va(vobj.queryAttributeObject(name));
        while (va.fetch()) {
            value.push_back(va.at<0>()->value());
        }
    }
    
    template <size_t size>
    static void save(Variable_object &vobj, const str8 &name, const std::array<s32, size> &value) {
        auto p = vnnew Variable_object();
        vobj.setAttribute(name, p);
        for (auto v : value) {
            p->add(vnnew Variable_int32(v));
        }
    }
    
    template <size_t size>
    static void load(Variable_object &vobj, const str8 &name, std::array<s32, size> &value) {
        VariableAccessor<Variable_int32> va(vobj.queryAttributeObject(name));
        for (auto &i : value) {
            if (va.fetch()) {
                i = va.at<0>()->value();
            } else {
                i = 0;
            }
        }
    }
};

template <>
struct PacketDataHelper<u64> {
    static void save(Variable_object &vobj, const str8 &name, u64 value) {
        vobj.setAttribute(name, vnnew Variable_int64(value));
    }
    static void load(Variable_object &vobj, const str8 &name, u64 &value) {
        value = (u64)vobj.queryAttributeInt64(name);
    }
    
    static void save(Variable_object &vobj, const str8 &name, const std::vector<u64> &value) {
        auto p = vnnew Variable_object();
        vobj.setAttribute(name, p);
        for (auto v : value) {
            p->add(vnnew Variable_int64(v));
        }
    }
    
    static void load(Variable_object &vobj, const str8 &name, std::vector<u64> &value) {
        VariableAccessor<Variable_number> va(vobj.queryAttributeObject(name));
        while (va.fetch()) {
            value.push_back((u64)va.at<0>()->getInt64());
        }
    }
    
    template <size_t size>
    static void save(Variable_object &vobj, const str8 &name, const std::array<u64, size> &value) {
        auto p = vnnew Variable_object();
        vobj.setAttribute(name, p);
        for (auto v : value) {
            p->add(vnnew Variable_int64(v));
        }
    }
    
    template <size_t size>
    static void load(Variable_object &vobj, const str8 &name, std::array<u64, size> &value) {
        VariableAccessor<Variable_number> va(vobj.queryAttributeObject(name));
        for (auto &i : value) {
            if (va.fetch()) {
                i = (u64)va.at<0>()->getInt64();
            } else {
                i = 0;
            }
        }
    }
};

template <>
struct PacketDataHelper<s64> {
    static void save(Variable_object &vobj, const str8 &name, s64 value) {
        vobj.setAttribute(name, vnnew Variable_int64(value));
    }
    static void load(Variable_object &vobj, const str8 &name, s64 &value) {
        value = vobj.queryAttributeInt64(name);
    }
    
    static void save(Variable_object &vobj, const str8 &name, const std::vector<s64> &value) {
        auto p = vnnew Variable_object();
        vobj.setAttribute(name, p);
        for (auto v : value) {
            p->add(vnnew Variable_int64(v));
        }
    }
    
    static void load(Variable_object &vobj, const str8 &name, std::vector<s64> &value) {
        VariableAccessor<Variable_number> va(vobj.queryAttributeObject(name));
        while (va.fetch()) {
            value.push_back(va.at<0>()->getInt64());
        }
    }
    
    template <size_t size>
    static void save(Variable_object &vobj, const str8 &name, const std::array<s64, size> &value) {
        auto p = vnnew Variable_object();
        vobj.setAttribute(name, p);
        for (auto v : value) {
            p->add(vnnew Variable_int64(v));
        }
    }
    
    template <size_t size>
    static void load(Variable_object &vobj, const str8 &name, std::array<s64, size> &value) {
        VariableAccessor<Variable_number> va(vobj.queryAttributeObject(name));
        for (auto &i : value) {
            if (va.fetch()) {
                i = va.at<0>()->getInt64();
            } else {
                i = 0;
            }
        }
    }
};

template <>
struct PacketDataHelper<f32> {
    static void save(Variable_object &vobj, const str8 &name, f32 value) {
        vobj.setAttribute(name, vnnew Variable_float(value));
    }
    static void load(Variable_object &vobj, const str8 &name, f32 &value) {
        value = vobj.queryAttributeFloat(name);
    }
    
    static void save(Variable_object &vobj, const str8 &name, const std::vector<f32> &value) {
        auto p = vnnew Variable_object();
        vobj.setAttribute(name, p);
        for (auto v : value) {
            p->add(vnnew Variable_float(v));
        }
    }
    
    static void load(Variable_object &vobj, const str8 &name, std::vector<f32> &value) {
        VariableAccessor<Variable_number> va(vobj.queryAttributeObject(name));
        while (va.fetch()) {
            value.push_back(va.at<0>()->getFloat());
        }
    }
    
    template <size_t size>
    static void save(Variable_object &vobj, const str8 &name, const std::array<f32, size> &value) {
        auto p = vnnew Variable_object();
        vobj.setAttribute(name, p);
        for (auto v : value) {
            p->add(vnnew Variable_float(v));
        }
    }
    
    template <size_t size>
    static void load(Variable_object &vobj, const str8 &name, std::array<f32, size> &value) {
        VariableAccessor<Variable_number> va(vobj.queryAttributeObject(name));
        for (auto &i : value) {
            if (va.fetch()) {
                i = va.at<0>()->getFloat();
            } else {
                i = 0;
            }
        }
    }
};

template <>
struct PacketDataHelper<f64> {
    static void save(Variable_object &vobj, const str8 &name, f64 value) {
        vobj.setAttribute(name, vnnew Variable_double(value));
    }
    static void load(Variable_object &vobj, const str8 &name, f64 &value) {
        value = vobj.queryAttributeDouble(name);
    }
    
    static void save(Variable_object &vobj, const str8 &name, const std::vector<f64> &value) {
        auto p = vnnew Variable_object();
        vobj.setAttribute(name, p);
        for (auto v : value) {
            p->add(vnnew Variable_double(v));
        }
    }
    
    static void load(Variable_object &vobj, const str8 &name, std::vector<f64> &value) {
        VariableAccessor<Variable_number> va(vobj.queryAttributeObject(name));
        while (va.fetch()) {
            value.push_back(va.at<0>()->getDouble());
        }
    }
    
    template <size_t size>
    static void save(Variable_object &vobj, const str8 &name, const std::array<f64, size> &value) {
        auto p = vnnew Variable_object();
        vobj.setAttribute(name, p);
        for (auto v : value) {
            p->add(vnnew Variable_double(v));
        }
    }
    
    template <size_t size>
    static void load(Variable_object &vobj, const str8 &name, std::array<f64, size> &value) {
        VariableAccessor<Variable_number> va(vobj.queryAttributeObject(name));
        for (auto &i : value) {
            if (va.fetch()) {
                i = va.at<0>()->getDouble();
            } else {
                i = 0;
            }
        }
    }
};

template <>
struct PacketDataHelper<str8> {
    static void save(Variable_object &vobj, const str8 &name, const str8 &value) {
        vobj.setAttribute(name, vnnew Variable_string(value));
    }
    static void load(Variable_object &vobj, const str8 &name, str8 &value) {
        value = vobj.queryAttributeString(name);
    }
    
    static void save(Variable_object &vobj, const str8 &name, const std::vector<str8> &value) {
        auto p = vnnew Variable_object();
        vobj.setAttribute(name, p);
        for (auto &v : value) {
            p->add(vnnew Variable_string(v));
        }
    }
    
    static void load(Variable_object &vobj, const str8 &name, std::vector<str8> &value) {
        VariableAccessor<Variable_string> va(vobj.queryAttributeObject(name));
        while (va.fetch()) {
            value.push_back(va.at<0>()->value());
        }
    }
    
    template <size_t size>
    static void save(Variable_object &vobj, const str8 &name, const std::array<str8, size> &value) {
        auto p = vnnew Variable_object();
        vobj.setAttribute(name, p);
        for (auto &v : value) {
            p->add(vnnew Variable_string(v));
        }
    }
    
    template <size_t size>
    static void load(Variable_object &vobj, const str8 &name, std::array<str8, size> &value) {
        VariableAccessor<Variable_string> va(vobj.queryAttributeObject(name));
        for (auto &i : value) {
            if (va.fetch()) {
                i = va.at<0>()->value();
            } else {
                break;
            }
        }
    }
};

_vn_end

#endif
