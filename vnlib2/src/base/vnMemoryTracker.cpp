//
//  vnMemoryTracker.cpp
//  vnlib
//
//  Created by Wayne on 5/7/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnMemoryTracker.h"

#ifdef VN_MEMORY_TRACKER

#include <sstream>
#include <iostream>
#include <fstream>
#include <mutex>

_vn_begin

MemoryTracker & MemoryTracker::instance() {
	static MemoryTracker s_ins;
	return s_ins;
}

MemoryTracker::MemoryTracker()
: m_totalAllocations(0)
, m_auto_report(true) {
	
}

MemoryTracker::~MemoryTracker() {
    if (m_auto_report) {
        reportLeaks();
    }
}

size_t MemoryTracker::totalMemoryAllocated() {
    return m_totalAllocations;
}

void MemoryTracker::setAutoReportLeaks(bool auto_report) {
    m_auto_report = auto_report;
}

bool MemoryTracker::isAutoReportLeaks() {
    return m_auto_report;
}

void MemoryTracker::setReportFile(const c8 *name) {
    if (name) {
        m_report_file = name;
    } else {
        m_report_file.clear();
    }
}

void MemoryTracker::recordAllocation(void *p, size_t size, const char *file, int line, int hint) {
    std::lock_guard<SpinLock> guard(m_lock);
	
	m_allocations[p] = Allocation(size, file, line, hint);
	m_totalAllocations += size;
}

void MemoryTracker::recordDeallocation(void *p) {
	std::lock_guard<SpinLock> guard(m_lock);
	
	auto it = m_allocations.find(p);
	if (it != m_allocations.end()) {
		m_totalAllocations -= it->second.size;
		m_allocations.erase(it);
	}
}

void MemoryTracker::reportLeaks() {
	std::ostringstream oss;
	if (m_allocations.empty()) {
		oss << "Memory: No memory leaks" << std::endl;
	} else {
		oss << "Memory: Detected memory leaks !!!" << std::endl;
		oss << "Memory: " << m_allocations.size() << " Allocation(s) with total " << m_totalAllocations << " bytes." << std::endl;
		oss << "Memory: Dumping allocations -> " << std::endl;
		for (auto &i : m_allocations) {
			Allocation &alloc = i.second;
			if (!alloc.file.empty()) {
				oss << alloc.file;
			} else {
				oss << "(unknown source)";
			}
			oss << "(" << alloc.line << "): {" << alloc.size << " bytes} hint_" << alloc.hint << std::endl;
		}
	}
    if (m_report_file.empty()) {
        std::cerr << oss.str().c_str();
    } else {
        std::ofstream of(m_report_file.c_str());
        of << oss.str().c_str();
    }
}

_vn_end

#endif