/************************************************************************ 
 * Copyright(C) Tencent 2011
 * 1.ʵ��ͬ�����첽���ַ������ڲ�֪ͨ����
 ***********************************************************************/
#ifndef __UTIL_NOTIFYMANAGER_NOTIFYMGR_H__
#define __UTIL_NOTIFYMANAGER_NOTIFYMGR_H__

#include <list>

#include "tranbase/notifymanager/NotifyDef.h"
#include "tranbase/Util.h"


#define _ADDNtfHandler(cmdid , handler) \
    m_NtfHandleMap.insert(std::make_pair(cmdid , handler ))

class NotifyManager
{
public:
    typedef int (*NtfHandle) (const INotify&);
    NotifyManager();
    virtual ~NotifyManager();
    int DispatchNotify(const INotify&);
	int Process();
protected:
	typedef std::list<INotify*> NotifyBuff;
	typedef HASHMULTIMAPDEFINE(int,NtfHandle) NtfHandleMap;
	NtfHandleMap m_NtfHandleMap;
	NotifyBuff m_NtfBuff;
	int _Dispatch(const INotify&);
};


#endif // end of __UTIL_NOTIFYMANAGER_NOTIFYMGR_H__
