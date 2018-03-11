#include "GSTransaction.h"

#include <algorithm>

#include "GSAction.h"
#include "GSTransactionMgr.h"
#include "GSTransNtfDef.h"
#include "tranbase/transactionmgr/ActionAllocator.h"



void GSTestTransaction::_AddAction()
{
	ACTIONALLOCMACRO(TestInsert,*this,m_GetInfoCtx);
//	ACTIONALLOCMACRO(Test,*this,m_GetInfoCtx);
}
void GSTestTransaction::_AddPreAction()
{
}

int GSTestTransaction::OnFail()
{
	//PkgCS::CSPKG pkg;
	//pkg.construct();
	//pkg.stBody.construct(PkgCS::CS_ACCOUNT_Test_RES);
	//pkg.stBody.stCSTestRes.stAccountStatus.iStatus = ACCOUNTSTATUS_ERROR;
	//return m_Context.SendCSMsg(pkg, PkgCS::CS_ACCOUNT_Test_RES);
	return 0;
}
int GSTestTransaction::OnEnd()
{

	return 0;
}



void GSTransactionUpdateToken::_AddAction()
{
	ACTIONALLOCMACRO(UpdateToken,*this,m_ppid,m_pptoken);
}
void GSTransactionUpdateToken::_AddPreAction()
{
}

int GSTransactionUpdateToken::OnFail()
{
	//PkgCS::CSPKG pkg;
	//pkg.construct();
	//pkg.stBody.construct(PkgCS::CS_ACCOUNT_Test_RES);
	//pkg.stBody.stCSTestRes.stAccountStatus.iStatus = ACCOUNTSTATUS_ERROR;
	//return m_Context.SendCSMsg(pkg, PkgCS::CS_ACCOUNT_Test_RES);
	return 0;
}
int GSTransactionUpdateToken::OnEnd()
{

	return 0;
}

