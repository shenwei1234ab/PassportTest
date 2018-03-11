/*******************************************************************************
@ ��Ȩ����(C) Tencent 2012
********************************************************************************/
#ifndef __GAMESVR_TRANSACTION_GAMEACTION_H__
#define __GAMESVR_TRANSACTION_GAMEACTION_H__

#include "tranbase/transactionmgr/IAction.h"
#include "GSTransContext.h"
#include "macrodef/GamesvrMacro.h"

class Insert : public IAction
{
public: 
	Insert(Transaction& trans, GSLogContext& ctx);
	virtual ~Insert();
	virtual uint32_t GetId() const { return MACROGAMESVR(ActionInsert); }

    virtual IActionReturn& OnStart();

    virtual IActionReturn& OnAsynResponse(const ITransNotify& ntf);
protected:
    GSLogContext& m_Ctx;
	int m_status = 0;
	int m_cbid = 0;
};




#endif // end of __GAMESVR_TRANSACTION_GAMEACTION_H__
