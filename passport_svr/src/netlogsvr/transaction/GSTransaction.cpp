#include "GSTransaction.h"

#include <algorithm>

#include "GSAction.h"
#include "GSTransactionMgr.h"
#include "GSTransNtfDef.h"
#include "tranbase/transactionmgr/ActionAllocator.h"



void GSInsertTransaction::_AddAction()
{
	ACTIONALLOCMACRO(Insert,*this,m_GetInfoCtx);
}
void GSInsertTransaction::_AddPreAction()
{
}

int GSInsertTransaction::OnFail()
{
	//PkgCS::CSPKG pkg;
	//pkg.construct();
	//pkg.stBody.construct(PkgCS::CS_ACCOUNT_Insert_RES);
	//pkg.stBody.stCSInsertRes.stAccountStatus.iStatus = ACCOUNTSTATUS_ERROR;
	//return m_Context.SendCSMsg(pkg, PkgCS::CS_ACCOUNT_Insert_RES);
	return 0;
}
int GSInsertTransaction::OnEnd()
{

	return 0;
}

