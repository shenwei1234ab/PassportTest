//
//  vnVariableAccessor.h
//  vnlib
//
//  Created by Wayne on 14-9-6.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnVariableAccessor_h
#define vnlib_vnVariableAccessor_h

#include "vnVariable.h"

_vn_begin

namespace internal {
	
	template <class... Types>
	struct VariableTuple;

	template <class T>
	struct VariableTuple<T> {
		typedef RefCountedPtr<T> ValueType;
		
		ValueType value;
		
		bool fetch(const Variable_object *obj, size_t index) {
			value = obj->at(index).dyn_cast<T>();
            return (bool)value;
		}
	};

	template <class T, class... Tails>
	struct VariableTuple<T, Tails...> : VariableTuple<Tails...> {
		typedef RefCountedPtr<T> ValueType;
		typedef VariableTuple<Tails...> BaseType;
		
		ValueType value;
		
		bool fetch(const Variable_object *obj, size_t index) {
			value = obj->at(index).dyn_cast<T>();
			if (!value) {
				return false;
			}
			return BaseType::fetch(obj, index + 1);
		}
	};
	
	template <size_t index, class T>
	struct VariableTupleElement;
	
	template <class T>
	struct VariableTupleElement<0, T> {
		typedef typename T::ValueType ValueType;
		typedef T TupleType;
	};
	
	template <size_t index, class T>
	struct VariableTupleElement {
		typedef typename VariableTupleElement<index - 1, typename T::BaseType>::ValueType ValueType;
		typedef typename VariableTupleElement<index - 1, typename T::BaseType>::TupleType TupleType;
	};
}

template <class... Types>
class VariableAccessor : public AllocatedObject {
public:
	
	enum { kCount = sizeof...(Types) };
	
	
	VariableAccessor(const RefCountedPtr<Variable_object> &vobj, size_t offset = 0)
	: m_object(vobj), m_offset(offset) {}
	
	VariableAccessor(RefCountedPtr<Variable_object> &&vobj, size_t offset = 0)
	: m_object(std::move(vobj)), m_offset(offset) {}
	
	bool fetch() {
		size_t off = m_offset + kCount;
		if (!m_object || off > m_object->size()) {
			return false;
		}
		if (!m_variables.fetch(m_object.get(), m_offset)) {
			return false;
		}
		m_offset = off;
		return true;
	}
	
	template <size_t index>
	typename internal::VariableTupleElement<index, internal::VariableTuple<Types...>>::ValueType & at() {
		static_assert(index < kCount, "out of range.");
		typedef typename internal::VariableTupleElement<index, decltype(m_variables)>::TupleType TupleType;
		return m_variables.TupleType::value;
	}
	
	template <size_t index>
	const typename internal::VariableTupleElement<index, internal::VariableTuple<Types...>>::ValueType & at() const {
		static_assert(index < kCount, "out of range.");
		typedef typename internal::VariableTupleElement<index, decltype(m_variables)>::TupleType TupleType;
		return m_variables.TupleType::value;
	}

	
private:
	RefCountedPtr<Variable_object> m_object;
	size_t m_offset;
	internal::VariableTuple<Types...> m_variables;
};

_vn_end

#endif
