//
//  vnVariable.cpp
//  vnlib
//
//  Created by Wayne on 5/14/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnVariable.h"

_vn_begin

const VariablePtr Variable::kNull = vnnew Variable();

str8 Variable_int32::desc() const {
	c8 buf[16];
	return {buf, (size_t)sprintf(buf, "%d", m_value)};
}

str8 Variable_int64::desc() const {
	c8 buf[16];
	return {buf, (size_t)sprintf(buf, "%lld", m_value)};
}

str8 Variable_float::desc() const {
	c8 buf[16];
	return {buf, (size_t)sprintf(buf, "%g", m_value)};
}

str8 Variable_double::desc() const {
	c8 buf[16];
	return {buf, (size_t)sprintf(buf, "%g", m_value)};
}

void Variable_object::save(DataStream &stream) const {
    stream << m_name << DataStream::ccu(m_attributes.size());
    for (auto it = m_attributes.begin(); it != m_attributes.end(); ++it) {
        stream << it->first;
        Variable::save(it->second.get(), stream);
    }
    stream << DataStream::ccu(m_children.size());
    for (auto it = m_children.begin(); it != m_children.end(); ++it) {
        Variable::save(it->get(), stream);
    }
}

void Variable_object::load(DataStream &stream) {
    size_t size;
    stream >> m_name >> DataStream::cu(size);
    str8 name;
    for (size_t i = 0; i < size; ++i) {
        stream >> name;
        m_attributes.insert(std::make_pair(name, Variable::create(stream)));
    }
    stream >> DataStream::cu(size);
    m_children.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        m_children.push_back(Variable::create(stream));
    }
}

void Variable_object::setAttribute(const str8 &name, VariablePtr value) {
    m_attributes[name] = std::move(value);
    
}

void Variable_object::eraseAttribute(const str8 &name) {
    m_attributes.erase(name);
}


VariablePtr  Variable_object::findAttribute(const str8 &name) const {
    auto it = m_attributes.find(name);
    if (it == m_attributes.end()) {
        return nullptr;
    }
    return it->second;
}

bool Variable_object::queryAttributeBool(const str8 &name, bool errval) const {
    auto it = m_attributes.find(name);
    if (it == m_attributes.end()) {
        return errval;
    }
    Variable_number *p = dynamic_cast<Variable_number *>(it->second.get());
    if (p) {
        return p->getBool();
    }
    return errval;
}

s32 Variable_object::queryAttributeInt32(const str8 &name, s32 errval) const {
    auto it = m_attributes.find(name);
    if (it == m_attributes.end()) {
        return errval;
    }
    Variable_number *p = dynamic_cast<Variable_number *>(it->second.get());
    if (p) {
        return p->getInt32();
    }
    return errval;
}

s64 Variable_object::queryAttributeInt64(const str8 &name, s64 errval) const {
    auto it = m_attributes.find(name);
    if (it == m_attributes.end()) {
        return errval;
    }
    Variable_number *p = dynamic_cast<Variable_number *>(it->second.get());
    if (p) {
        return p->getInt64();
    }
    return errval;
}

f32 Variable_object::queryAttributeFloat(const str8 &name, f32 errval) const {
    auto it = m_attributes.find(name);
    if (it == m_attributes.end()) {
        return errval;
    }
    Variable_number *p = dynamic_cast<Variable_number *>(it->second.get());
    if (p) {
        return p->getFloat();
    }
    return errval;
}

f64 Variable_object::queryAttributeDouble(const str8 &name, f64 errval) const {
    auto it = m_attributes.find(name);
    if (it == m_attributes.end()) {
        return errval;
    }
    Variable_number *p = dynamic_cast<Variable_number *>(it->second.get());
    if (p) {
        return p->getDouble();
    }
    return errval;
}

str8 Variable_object::queryAttributeString(const str8 &name, str8 errval) const {
    auto it = m_attributes.find(name);
    if (it == m_attributes.end()) {
        return std::move(errval);
    }
    Variable_string *p = dynamic_cast<Variable_string *>(it->second.get());
    if (p) {
        return p->value();
    }
    return std::move(errval);
}

RefCountedPtr<Variable_string> Variable_object::queryAttributeStringV(const str8 &name) const {
    auto it = m_attributes.find(name);
    if (it == m_attributes.end()) {
        return nullptr;
    }
    Variable_string *p = dynamic_cast<Variable_string *>(it->second.get());
    if (p) {
        return {p, true};
    }
    return nullptr;
}

RefCountedPtr<Variable_object> Variable_object::queryAttributeObject(const str8 &name) const {
    auto it = m_attributes.find(name);
    if (it == m_attributes.end()) {
        return nullptr;
    }
    Variable_object *p = dynamic_cast<Variable_object *>(it->second.get());
    if (p) {
        return RefCountedPtr<Variable_object>(p, true);
    }
    return nullptr;
}

bool Variable_object::empty() const {
    return m_children.empty();
}

size_t Variable_object::size() const {
    return m_children.size();
}

VariablePtr & Variable_object::at(size_t index) {
    return m_children.at(index);
}

const VariablePtr & Variable_object::at(size_t index) const {
    return m_children.at(index);
}

void Variable_object::add(const VariablePtr &variable) {
    m_children.push_back(variable);
}

void Variable_object::add(VariablePtr &&variable) {
    m_children.push_back(variable);
}

void Variable_object::erase(size_t index) {
    m_children.erase(m_children.begin() + index);
}

size_t Variable_object::findObjectIndex(size_t after) const {
    for (size_t i = after + 1; i < m_children.size(); ++i) {
        if (dynamic_cast<Variable_object *>(m_children[i].get())) {
            return i;
        }
    }
    return -1;
}

size_t Variable_object::findNamedObjectIndex(const str8 &name, size_t after) const {
    for (size_t i = after + 1; i < m_children.size(); ++i) {
        Variable_object *p;
        if ((p = dynamic_cast<Variable_object *>(m_children[i].get())) && p->m_name == name) {
            return i;
        }
    }
    return -1;
}

//////////////////////////////////////////////////////////////////////////


VariablePtr Variable::create(VariableType vt) {
    Variable *p = 0;
    switch (vt) {
        case kVT_null:
            return Variable::kNull;

        case kVT_bool:
            p = vnnew Variable_bool();
            break;
            
        case kVT_int32:
            p = vnnew Variable_int32();
            break;

        case kVT_int64:
            p = vnnew Variable_int64();
            break;

        case kVT_float:
            p = vnnew Variable_float();
            break;

        case kVT_double:
            p = vnnew Variable_double();
            break;

        case kVT_string:
            p = vnnew Variable_string();
            break;

        case kVT_id:
            p = vnnew Variable_id();
            break;

        case kVT_object:
            p = vnnew Variable_object();
            break;
        default:
            break;
    }
    return p;
}

VariablePtr Variable::create(DataStream &stream) {
    u8 vt = 0;
    stream >> vt;
    VariablePtr p = create((VariableType)vt);
    if (!p) {
        throw DataStream::ReadException();
    }
    p->load(stream);
    return p;
}

void Variable::save(const Variable *variable, DataStream &stream) {
    stream << (u8)variable->type();
    variable->save(stream);
}



_vn_end