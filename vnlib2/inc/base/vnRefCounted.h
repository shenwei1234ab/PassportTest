//
//  vnRefCounted.h
//  vnlib
//
//  Created by Wayne on 5/7/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnRefCounted_h
#define vnlib_vnRefCounted_h

#include "vnAllocatedObject.h"
#include "vnAssert.h"
#include <atomic>

_vn_begin

class RefCounted;
int ref_count(RefCounted *obj);

class RefCounted : public AllocatedObject {
    friend int ref_count(RefCounted *obj);
public:
    RefCounted() = default;
    RefCounted(const RefCounted &) {}
	virtual ~RefCounted() = default;
	
	RefCounted & operator =(const RefCounted &) { return *this; }
	
	void grab() const { ++m_refCount; }
	
	bool drop() const {
        vnassert(m_refCount);
		if (!--m_refCount) {
			_release();
			return true;
		}
		return false;
	}
	
protected:
	virtual void _release() const { delete this; }
	
private:
    mutable std::atomic<int> m_refCount {1};
};

inline int ref_count(RefCounted *obj) {
    return obj->m_refCount;
}

template <class _RefCounted>
class RefCountedPtr {
public:
	RefCountedPtr() : m_pointee(0) {}
    RefCountedPtr(std::nullptr_t) : m_pointee(0) {}
	RefCountedPtr(_RefCounted *ptr) : m_pointee(ptr) {}
    RefCountedPtr(_RefCounted *ptr, bool grab) : m_pointee(ptr) {
        if (ptr && grab) { ptr->grab(); }
    }
	RefCountedPtr(const RefCountedPtr &rhs) : m_pointee(rhs.m_pointee) {
		if (m_pointee) {
			m_pointee->grab();
		}
	}
    RefCountedPtr(RefCountedPtr &&rhs) : m_pointee(rhs.m_pointee) {
        rhs.m_pointee = 0;
	}
	
	template <class T>
	RefCountedPtr(const RefCountedPtr<T> &rhs) : m_pointee(rhs.get()) {
		if (m_pointee) {
			m_pointee->grab();
		}
	}
    
    template <class T>
	RefCountedPtr(RefCountedPtr<T> &&rhs) : m_pointee(rhs.release()) {
        
	}
	
	~RefCountedPtr() {
		reset();
	}
	
	void swap(RefCountedPtr &rhs) {
		_RefCounted *t = rhs.m_pointee;
		rhs.m_pointee = m_pointee;
		m_pointee = t;
	}
	
	RefCountedPtr & operator =(const RefCountedPtr &rhs) {
		if (rhs.m_pointee != m_pointee) {
			RefCountedPtr t(rhs);
			swap(t);
		}
		return *this;
	}
    
    RefCountedPtr & operator =(RefCountedPtr &&rhs) {
        swap(rhs);
        return *this;
    }
	
	RefCountedPtr & operator =(_RefCounted *rhs) {
		RefCountedPtr t(rhs);
		swap(t);
		return *this;
	}
    
    RefCountedPtr & operator =(decltype(nullptr)) {
        reset();
		return *this;
    }
    
    
    bool operator ==(const RefCountedPtr &rhs) const {
        return m_pointee == rhs.m_pointee;
    }
    
    bool operator !=(const RefCountedPtr &rhs) const {
        return m_pointee != rhs.m_pointee;
    }
    
    bool operator <(const RefCountedPtr &rhs) const {
        return m_pointee < rhs.m_pointee;
    }
    
    operator bool() const {
        return m_pointee != nullptr;
    }
	
	_RefCounted * operator ->() const {
		return m_pointee;
	}
	
	_RefCounted & operator *() const {
		return *m_pointee;
	}
	
	_RefCounted * get() const {
		return m_pointee;
	}
	
	s32 use_count() const {
		if (!m_pointee) {
			return 0;
		}
		return ref_count(m_pointee);
	}
    
    bool unique() const {
        if (!m_pointee) {
            return false;
        }
        return ref_count(m_pointee) == 1;
    }
	
	template <class T>
	RefCountedPtr<T> cast() const {
		if (!m_pointee) {
			return RefCountedPtr<T>();
		}
		T *p = static_cast<T *>(m_pointee);
		p->grab();
		return p;
	}
	
	template <class T>
	RefCountedPtr<T> dyn_cast() const {
		T *p = dynamic_cast<T *>(m_pointee);
		if (p) {
			p->grab();
		}
		return p;
	}
	
	void reset() {
		if (m_pointee) {
			m_pointee->drop();
			m_pointee = 0;
		}
	}
	
	void reset(_RefCounted *ptr, bool grab) {
		if (ptr && grab) {
			ptr->grab();
		}
		if (m_pointee) {
			m_pointee->drop();
		}
		m_pointee = ptr;
	}
    
    _RefCounted * release() {
        _RefCounted *p = m_pointee;
        m_pointee = 0;
        return p;
    }
    
private:
	_RefCounted *m_pointee;
    
};


_vn_end


#endif
