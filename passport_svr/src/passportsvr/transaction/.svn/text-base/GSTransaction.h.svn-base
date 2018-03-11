/*******************************************************************************
@ ∞Ê»®À˘”–(C) Tencent 2012
********************************************************************************/
#ifndef __GSSVR_TRANSACTION_GSTRANSACTION_H__
#define __GSSVR_TRANSACTION_GSTRANSACTION_H__


#include "tranbase/transactionmgr/Transaction.h"
#include "GSAction.h"
#include "GSTransContext.h"
#include "macrodef/GamesvrMacro.h"
#define _DEFAULT_TRANS_TIME_SPAN_ 5 //seconds



class GSTestTransaction : public Transaction
{
public:
	GSTestTransaction(TransactionMgr& mgr ,  const GSTestContext& gictx  )
		:Transaction(mgr,_DEFAULT_TRANS_TIME_SPAN_)
		,m_GetInfoCtx(gictx)
	{

	}
	virtual uint32_t GetId() const { return MACROGAMESVR(TransactionTest); }
	virtual int OnFail();
	virtual int OnEnd();
protected:
	virtual void _AddAction();
	virtual void _AddPreAction();
	GSTestContext m_GetInfoCtx;
};


class GSTransactionUpdateToken : public Transaction
{
public:
	GSTransactionUpdateToken(TransactionMgr& mgr , uint64_t ppid,uint32_t pptoken)
		:Transaction(mgr,_DEFAULT_TRANS_TIME_SPAN_)
		,m_ppid(ppid),m_pptoken(pptoken)
	{

	}
	virtual uint32_t GetId() const { return MACROGAMESVR(TransactionUpdateToken); }
	virtual int OnFail();
	virtual int OnEnd();
protected:
	virtual void _AddAction();
	virtual void _AddPreAction();
	uint64_t m_ppid;
	uint32_t m_pptoken;
};
#endif // end of __GSSVR_TRANSACTION_GSTRANSACTION_H__
