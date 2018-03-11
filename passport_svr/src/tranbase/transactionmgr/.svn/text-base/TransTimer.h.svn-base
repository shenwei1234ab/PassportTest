/*******************************************************************************
 @ ∞Ê»®À˘”–(C) Tencent 2012
 ********************************************************************************/
#ifndef __UTIL_TRANSACTIONMGR_TRANSTIMER_H__
#define __UTIL_TRANSACTIONMGR_TRANSTIMER_H__

#include "time.h"

#include "manager/TimerManager.h"

class Transaction;

class TransTimer {
public:
	TransTimer(Transaction& trans, time_t timespan);
	virtual ~TransTimer() {
	}
	void CloseTimer();
	void StartTimer();
protected:
	virtual bool _OnTick();
	Transaction& m_Trans;
	int m_iTimerSpan;

	timer_handler m_hander;
};
#endif // end of __UTIL_TRANSACTIONMGR_TRANSTIMER_H__
