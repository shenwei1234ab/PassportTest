/*******************************************************************************
@ ��Ȩ����(C) Tencent 2012
********************************************************************************/
#ifndef __GSSVR_TRANSACTION_GSTRANSACTION_H__
#define __GSSVR_TRANSACTION_GSTRANSACTION_H__


#include "tranbase/transactionmgr/Transaction.h"
#include "GSAction.h"
#include "GSTransContext.h"
#include "macrodef/GamesvrMacro.h"
#define _DEFAULT_TRANS_TIME_SPAN_ 5 //seconds



class GSInsertTransaction : public Transaction
{
public:
	GSInsertTransaction(TransactionMgr& mgr ,  const GSLogContext& gictx  )
		:Transaction(mgr,_DEFAULT_TRANS_TIME_SPAN_)
		,m_GetInfoCtx(gictx)
	{

	}
	virtual uint32_t GetId() const { return MACROGAMESVR(TransactionInsert); }
	virtual int OnFail();
	virtual int OnEnd();
protected:
	virtual void _AddAction();
	virtual void _AddPreAction();
	GSLogContext m_GetInfoCtx;
};


#endif // end of __GSSVR_TRANSACTION_GSTRANSACTION_H__
