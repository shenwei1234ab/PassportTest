//
//  vnVariable.h
//  vnlib
//
//  Created by Wayne on 5/14/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnVariable_h
#define vnlib_vnVariable_h

#include "vnRefCounted.h"
#include "vnDataStream.h"

#include <map>
#include <vector>
#include <functional>

_vn_begin

enum VariableType : u8 {
    kVT_null = 0,
    kVT_bool = 2,
    kVT_int32 = 3,
    kVT_int64 = 5,
    kVT_float = 9,
    kVT_double = 17,
    kVT_string = 32,
    kVT_id = 64,
    kVT_object = 128,
    kVT_reserved = 255
};

class Variable : public RefCounted {
public:
    virtual VariableType type() const { return kVT_null; }
    
    static const RefCountedPtr<Variable> kNull;
    
    virtual void save(DataStream &stream) const {}
    virtual void load(DataStream &stream) {}
	virtual str8 desc() const { return str8(); }
    
    static RefCountedPtr<Variable> create(VariableType vt);
    RefCountedPtr<Variable> create(DataStream &stream);
    static void save(const Variable *variable, DataStream &stream);
};

typedef RefCountedPtr<Variable> VariablePtr;

class Variable_number : public Variable {
public:
    virtual bool getBool() const = 0;
    virtual s32 getInt32() const = 0;
    virtual s64 getInt64() const = 0;
    virtual f32 getFloat() const = 0;
    virtual f64 getDouble() const = 0;
};

class Variable_bool : public Variable_number {
public:
    Variable_bool() = default;
    Variable_bool(bool _value) : m_value(_value) {}
    
    virtual VariableType type() const { return kVT_bool; }
    virtual bool getBool() const { return m_value; }
    virtual s32 getInt32() const { return m_value ? 1 : 0; }
    virtual s64 getInt64() const { return m_value ? 1 : 0; }
    virtual f32 getFloat() const { return m_value ? 1.f : 0; }
    virtual f64 getDouble() const { return m_value ? 1.0 : 0; }
    
    bool value() const { return m_value; }
    void setValue(bool _value) { m_value = _value; }
    
    virtual void save(DataStream &stream) const { stream << m_value; }
    virtual void load(DataStream &stream) { stream >> m_value; }
	virtual str8 desc() const { return m_value ? "true" : "false"; }
protected:
    bool m_value = false;
};

class Variable_int32 : public Variable_number {
public:
    Variable_int32() = default;
    Variable_int32(s32 _value) : m_value(_value) {}
    
    virtual VariableType type() const { return kVT_int32; }
    virtual bool getBool() const { return m_value ? true : false; }
    virtual s32 getInt32() const { return (s32)m_value; }
    virtual s64 getInt64() const { return (s64)m_value; }
    virtual f32 getFloat() const { return (f32)m_value; }
    virtual f64 getDouble() const { return (f64)m_value; }
    
    s32 value() const { return m_value; }
    void setValue(s32 _value) { m_value = _value; }

    virtual void save(DataStream &stream) const { stream << m_value; }
    virtual void load(DataStream &stream) { stream >> m_value; }
	virtual str8 desc() const;
protected:
    s32 m_value = 0;
};

class Variable_int64 : public Variable_number {
public:
    Variable_int64() = default;
    Variable_int64(s64 _value) : m_value(_value) {}
    
    virtual VariableType type() const { return kVT_int64; }
    virtual bool getBool() const { return m_value ? true : false; }
    virtual s32 getInt32() const { return (s32)m_value; }
    virtual s64 getInt64() const { return (s64)m_value; }
    virtual f32 getFloat() const { return (f32)m_value; }
    virtual f64 getDouble() const { return (f64)m_value; };
    
    s64 value() const { return m_value; }
    void setValue(s64 _value) { m_value = _value; }

    virtual void save(DataStream &stream) const { stream << m_value; }
    virtual void load(DataStream &stream) { stream >> m_value; }
    virtual str8 desc() const;
protected:
    s64 m_value = 0;
};

class Variable_float : public Variable_number {
public:
    Variable_float() = default;
    Variable_float(f32 _value) : m_value(_value) {}
    
    virtual VariableType type() const { return kVT_float; }
    virtual bool getBool() const { return m_value ? true : false; }
    virtual s32 getInt32() const { return (s32)m_value; }
    virtual s64 getInt64() const { return (s64)m_value; }
    virtual f32 getFloat() const { return (f32)m_value; }
    virtual f64 getDouble() const { return (f64)m_value; }
    
    f32 value() const { return m_value; }
    void setValue(f32 _value) { m_value = _value; }

    virtual void save(DataStream &stream) const { stream << m_value; }
    virtual void load(DataStream &stream) { stream >> m_value; }
	virtual str8 desc() const;
protected:
    f32 m_value = 0;
};

class Variable_double : public Variable_number {
public:
    Variable_double() = default;
    Variable_double(f64 _value) : m_value(_value) {}
    
    virtual VariableType type() const { return kVT_double; }
    virtual bool getBool() const { return m_value ? true : false; }
    virtual s32 getInt32() const { return (s32)m_value; }
    virtual s64 getInt64() const { return (s64)m_value; }
    virtual f32 getFloat() const { return (f32)m_value; }
    virtual f64 getDouble() const { return (f64)m_value; }
    
    f64 value() const { return m_value; }
    void setValue(f64 _value) { m_value = _value; }

    virtual void save(DataStream &stream) const { stream << m_value; }
    virtual void load(DataStream &stream) { stream >> m_value; }
    virtual str8 desc() const;
protected:
    f64 m_value = 0;
};

class Variable_string : public Variable {
public:
    Variable_string() = default;
    Variable_string(const str8 &_value) : m_value(_value) {}
    Variable_string(str8 &&_value) : m_value(std::move(_value)) {}
    
    virtual VariableType type() const { return kVT_string; }
    
    const str8 & value() const { return m_value; }
    void setValue(const str8 &_value) { m_value = _value; }
    void setValue(str8 &&_value) { m_value = std::move(_value); }

    virtual void save(DataStream &stream) const { stream << m_value; }
    virtual void load(DataStream &stream) { stream >> m_value; }
	virtual str8 desc() const { return m_value; }
protected:
    str8 m_value;
};

class Variable_id : public Variable_string {
public:
    Variable_id() = default;
    Variable_id(const str8 &_value) : Variable_string(_value) {}
    Variable_id(str8 &&_value) : Variable_string(std::move(_value)) {}
    
    virtual VariableType type() const { return kVT_id; }
};

class Variable_object : public Variable {
public:
    Variable_object() = default;
    Variable_object(const str8 &name) : m_name(name) {}
    Variable_object(str8 &&name) : m_name(name) {}
    virtual VariableType type() const { return kVT_object; }

    virtual void save(DataStream &stream) const;
    virtual void load(DataStream &stream);
	virtual str8 desc() const { return m_name; }

    const str8 & name() const { return m_name; }
    
    void setAttribute(const str8 &name, VariablePtr value);
    void eraseAttribute(const str8 &name);
    
    const std::map<str8, VariablePtr> & attributes() const { return m_attributes; }
    std::map<str8, VariablePtr> & attributes() { return m_attributes; }

    VariablePtr findAttribute(const str8 &name) const;
    bool queryAttributeBool(const str8 &name, bool errval = false) const;
    s32 queryAttributeInt32(const str8 &name, s32 errval = 0) const;
    s64 queryAttributeInt64(const str8 &name, s64 errval = 0) const;
    f32 queryAttributeFloat(const str8 &name, f32 errval = 0) const;
    f64 queryAttributeDouble(const str8 &name, f64 errval = 0) const;
    str8 queryAttributeString(const str8 &name, str8 errval = {}) const;
    str8 queryAttributeDesc(const str8 &name) const;
    RefCountedPtr<Variable_string> queryAttributeStringV(const str8 &name) const;
    RefCountedPtr<Variable_object> queryAttributeObject(const str8 &name) const;
    
    bool empty() const;
    size_t size() const;
    VariablePtr & at(size_t index);
    const VariablePtr & at(size_t index) const;
    
    void add(const VariablePtr &variable);
    void add(VariablePtr &&variable);
    void erase(size_t index);
    
    size_t findObjectIndex(size_t after = -1) const;
    size_t findNamedObjectIndex(const str8 &name, size_t after = -1) const;
    
protected:
    std::map<str8, VariablePtr> m_attributes;
    std::vector<VariablePtr> m_children;
    
    str8 m_name;
};

_vn_end

#endif
