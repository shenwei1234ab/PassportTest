#include "../GSTransactionMgr.h"
#include "../../../passportsvr/transaction/notifymanager/GSNotifyHandler.h"

#include "../../../passportsvr/transaction/GSTransactionMgr.h"
#include "../../../passportsvr/transaction/notifymanager/GSNotifyDef.h"


int NtfHandleNotifyDBQuery(const INotify& ntf)
{
	return SGSTransactionMgr.Response((const ITransNotify&)ntf);
}


int NtfHandleNotifyDBExecute(const INotify& ntf)
{
	return SGSTransactionMgr.Response((const ITransNotify&)ntf);
}
