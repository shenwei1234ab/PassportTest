#include "tranbase/transactionmgr/Transaction.h"
#include "tranbase/transactionmgr/TransTimer.h"


#define _DEFAULT_TRANS_TIME_SPAN_ 5 //seconds

TransTimer::TransTimer(Transaction& trans, time_t timespan) :
		m_Trans(trans), m_iTimerSpan(timespan), m_hander(NULL) {

}
bool TransTimer::_OnTick() {
	m_Trans.DoTimeOut();
	return true;
}
void TransTimer::CloseTimer() {
	//关闭定时器
	sTimerManager.destroyTimer(m_hander);
}
void TransTimer::StartTimer() {
	//开启定时器
	m_hander = sTimerManager.createTimer(_DEFAULT_TRANS_TIME_SPAN_,std::bind(&TransTimer::_OnTick,this),-1);
}
