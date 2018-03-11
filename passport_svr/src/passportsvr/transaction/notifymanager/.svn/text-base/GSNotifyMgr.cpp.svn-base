#include "../../../passportsvr/transaction/notifymanager/GSNotifyMgr.h"

#include "../../../passportsvr/transaction/macrodef/GamesvrMacro.h"
#include "../../../passportsvr/transaction/notifymanager/GSNotifyHandler.h"

GSNotifyMgr::GSNotifyMgr()
:NotifyManager()
{
	_ADDNtfHandler(MACROUTIL(NotifyDBQuery),&NtfHandleNotifyDBQuery);
	_ADDNtfHandler(MACROUTIL(NotifyDBExecute),&NtfHandleNotifyDBExecute);

}


