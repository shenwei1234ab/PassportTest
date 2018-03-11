/************************************************************************
 * Copyright(C) Tencent 2011
 ***********************************************************************/
#include "tranbase/notifymanager/NotifyMgr.h"

#include "tranbase/logmgr/Log.h"
#include "tranbase/logmgr/Log.h"
#include "../Util.h"

NotifyManager::NotifyManager()
{
}
NotifyManager::~NotifyManager()
{
	for( NotifyBuff::iterator it = m_NtfBuff.begin() ; it != m_NtfBuff.end() ; ++it )
	{
		if( *it != NULL )
		{
			RELEASE((*it));
		}
	}
	m_NtfBuff.clear();
}

int Default_Handler(const INotify& notify)
{
    LOG_INFO("UnknowNotify Event %s"  , notify.GetString());
    return 0;
}


int NotifyManager::_Dispatch(const INotify& notify)
{
    NtfHandleMap::iterator it;
    std::pair<NtfHandleMap::iterator, NtfHandleMap::iterator> ret;
    ret = m_NtfHandleMap.equal_range(notify.GetId());

    int res = 0;

    if ( ret.first != ret.second )
    {
        for ( it = ret.first ; it != ret.second; ++it )
        {
            res |= (it->second)(notify);
        }
    }
    else
    {
        res |= Default_Handler(notify);
    }
    LOG_DEBUG("NotifyEvent %s Res[%d]!  " , notify.GetString() , res);
    return res;
}
int NotifyManager::DispatchNotify(const INotify& notify)
{
   if( notify.IsAsynNtf() )
   {
	   INotify* ntf = notify.Clone();
	   if( ntf == NULL )
	   {
			LOG_ERROR("NotifyEvent Ansy Fail! %s" , notify.GetString());
			return -1;
	   }
	   m_NtfBuff.push_back(ntf);
   }
   else
   {
		return _Dispatch(notify);
   }
   return 0;
}
int NotifyManager::Process()
{
	for( NotifyBuff::iterator it = m_NtfBuff.begin() ; it != m_NtfBuff.end() ; ++it )
	{
		if( *it != NULL )
		{
			_Dispatch(*(*it));
			RELEASE((*it));
		}
	}
	int size = (int)m_NtfBuff.size();
	m_NtfBuff.clear();
	return size;
}


