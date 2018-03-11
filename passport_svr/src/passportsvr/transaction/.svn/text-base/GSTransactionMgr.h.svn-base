/*******************************************************************************
@ ∞Ê»®À˘”–(C) Tencent 2012
********************************************************************************/
#ifndef __GAMESVR_TRANSACTION_GAMETRANSACTIONMGR_H__
#define __GAMESVR_TRANSACTION_GAMETRANSACTIONMGR_H__

#include "tranbase/transactionmgr/ActionAllocator.h"
#include "tranbase/transactionmgr/TransactionMgr.h"
#include "vnSingleton.h"


class GSTransactionMgr : public TransactionMgr, public vn::Singleton<GSTransactionMgr>
{ 
public: 
	GSTransactionMgr();
	virtual ~GSTransactionMgr();
protected: 
}; 

//typedef util::SingletonHolder<GSTransactionMgr> SGSTransactionMgr;
#define SGSTransactionMgr GSTransactionMgr::instance()

#endif // end of __GAMESVR_TRANSACTION_GAMETRANSACTIONMGR_H__

