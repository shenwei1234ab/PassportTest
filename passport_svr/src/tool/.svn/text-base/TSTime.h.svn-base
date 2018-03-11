/*
 * TSTime.h
 *
 *  Created on: 2015年9月24日
 *      Author: tanghailong
 */

#ifndef SRC_TOOL_TSTIME_H_
#define SRC_TOOL_TSTIME_H_

#include <chrono>
#include "vnLog.h"
#include "vnSingleton.h"

#include <map>
#include <thread>

using namespace std::chrono;

class TSTime : public vn::Singleton<TSTime> {
public:

	void begin() {
		m_times[std::this_thread::get_id()] = high_resolution_clock::now();
	}

	void end() {
		auto it = m_times.find(std::this_thread::get_id());
		if (it == m_times.end()) {
			return ;
		}
		auto now = high_resolution_clock::now();
		duration<double> time_span = duration_cast<duration<double>>(now - it->second);

		VN_LOG_DEBUG("cost time:" << time_span.count());
	}

private:
	std::map<std::thread::id, high_resolution_clock::time_point> m_times;
};


#endif /* SRC_TOOL_TSTIME_H_ */
