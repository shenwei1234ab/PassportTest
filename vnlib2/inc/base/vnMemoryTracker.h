//
//  vnMemoryTracker.h
//  vnlib
//
//  Created by Wayne on 5/7/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnMemoryTracker_h
#define vnlib_vnMemoryTracker_h

#include "vnSpinLock.h"

#ifdef VN_MEMORY_TRACKER

#include <map>

_vn_begin

class MemoryTracker : public NonCopyable {
public:
    static MemoryTracker & instance();
    
    ~MemoryTracker();
    
	void recordAllocation(void *p, size_t size, const char *file = 0, int line = 0, int hint = 0);
	void recordDeallocation(void *p);
	
	size_t totalMemoryAllocated();
	void reportLeaks();
    
    void setAutoReportLeaks(bool auto_report);
    bool isAutoReportLeaks();
    
    void setReportFile(const c8 *name);
private:
    struct Allocation {
		size_t size = 0;
		str8 file;
		int line = 0;
		int hint = 0;
		
		Allocation() = default;
		Allocation(size_t s, const char *f, int l, int h ) : size(s), line(l), hint(h) { if (f) file = f; }
	};
	typedef std::map<void *, Allocation> AllocationMap;
    
	MemoryTracker();
    
	size_t m_totalAllocations;
	AllocationMap m_allocations;
	SpinLock m_lock;
    bool m_auto_report;
    str8 m_report_file;
};

_vn_end

#endif

#endif
