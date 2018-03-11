/*
 * TimerManager.h
 *
 *  Created on: 2015年11月5日
 *      Author: leave
 */

#ifndef SRC_PASSPORTSVR_MANAGER_TIMERMANAGER_H_
#define SRC_PASSPORTSVR_MANAGER_TIMERMANAGER_H_

#include "vnSingleton.h"
#include "vnSpinLock.h"
#include "vnRefCounted.h"


#include <set>

typedef void * timer_handler;

class TimerManager: public vn::Singleton<TimerManager> {
public:
	TimerManager();
	virtual ~TimerManager();

	//创建定时器，回调函数在updateTimers所在的线程（主线程）执行，可以在回调函数中进行定时器本身的操作
	timer_handler createTimer(vn::f32 periods, std::function<void()> fn, vn::u32 repeats = 0); // repeats == -1 means infinite.

	//销毁定时器
	void destroyTimer(timer_handler timer);

	void updateTimers(vn::f32 dt);
protected:


	struct Timer: vn::RefCounted {
		vn::f32 periods;
		vn::f32 timer;
		vn::u32 repeats;
		std::function<void()> fn;

		Timer(vn::f32 _p, vn::u32 _r, std::function<void()> &&_f) :
				periods(_p), timer(0), repeats(_r), fn(std::move(_f)) {
		}
	};
	std::set<Timer *> m_timers;
	vn::SpinLock m_timers_lock;
};
#define sTimerManager TimerManager::instance()

#endif /* SRC_PASSPORTSVR_MANAGER_TIMERMANAGER_H_ */
