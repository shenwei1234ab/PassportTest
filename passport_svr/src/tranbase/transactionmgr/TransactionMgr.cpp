#include "tranbase/logmgr/Log.h"
#include "tranbase/transactionmgr/TransactionMgr.h"

#include "tranbase/logmgr/Log.h"
#include "tranbase/transactionmgr/ActionAllocator.h"

TransactionMgr::TransactionMgr(int /*max*/,int transize, int maxaction ,int actionsize )
:m_TransID(-1)
,m_CallbackId(0)
{
	m_stop = false;
	m_usepool = false;
}
TransactionMgr::~TransactionMgr()
{
	m_stop = true;
	ClearTrans();
	//m_TransPool.Free();
	//m_ActionPool.Free();
}

void TransactionMgr::ClearTrans()
{
	LOG_DEBUG("TransactionMgr ClearTrans");
	for( TransactionMap::iterator it = m_TransMap.begin() ; it != m_TransMap.end() ; ++it )
	{
		FreeTrans(it->second);
	}
	m_TransMap.clear();
}
void TransactionMgr::FreeAction(IAction* action)
{

	ACTIONDEALLOCMACRO(action);

}
void TransactionMgr::FreeTrans(Transaction* trans)
{
	LOG_DEBUG("TransactionMgr FreeTrans [%s,%p]",trans->GetString() ,trans );
	trans->CloseTimer();

	ACTIONDEALLOCMACRO(trans);

}

int TransactionMgr::AddTransaction(Transaction* trans)
{
	if(m_stop) return -1;
	//ClearCloseTrans();
    if(NULL == trans)
    {
        LOG_DEBUG("TransactionMgr AddTransaction NULL == trans Ptr[%p]",trans);
        return -1;
    }
	if( GetTransaction(trans->GetTransactionId()) == NULL )
	{
		std::lock_guard<std::mutex> lck (m_TransMapLock);
		LOG_DEBUG("TransactionMgr AddTransaction %s Ptr[%p] ,Mgr Trans Cnt [%lu]",trans->GetString() ,trans , m_TransMap.size());
		m_TransMap[trans->GetTransactionId()] = trans;
		trans->StartTimer();
		trans->DoStart();
		return trans->GetTransactionId();
	}
	else
	{
		LOG_DEBUG("TransactionMgr AddTransaction Conflict %s Ptr[%p]", trans->GetString() , trans);
        return -1;
	}
	return 0;
}
Transaction* TransactionMgr::GetTransaction(uint64_t tid)
{
	TransactionMap::iterator it = m_TransMap.find(tid);
	if( it != m_TransMap.end() )
	{
		return it->second;
	}
	return NULL;
}
void TransactionMgr::DeleteTransaction(uint64_t tid)
{
	Transaction* trans = GetTransaction(tid);
	if( trans != NULL )
	{
		trans->CloseTimer();
		m_DelVec.push_back(tid);
	}
}
void TransactionMgr::Process()
{
	ClearCloseTrans();
}

void TransactionMgr::ClearCloseTrans()
{
	if(!m_stop)
	{
		std::lock_guard<std::mutex> lck (m_TransMapLock);
		for( DelVector::iterator it = m_DelVec.begin() ; it != m_DelVec.end() ; ++it )
		{
			TransactionMap::iterator mit = m_TransMap.find(*it);
			if( mit != m_TransMap.end() )
			{
				FreeTrans(mit->second);
				m_TransMap.erase(mit);
			}
		}
	}
	m_DelVec.clear();
}

int TransactionMgr::_GetCBId()
{
	if( m_CallbackId < 1 )
	{
		m_CallbackId = 1;
	}

    //int 越界了情况
	CallbackMap::iterator it = m_CBMap.find(m_CallbackId);
	int cnt = 0;
	while( it != m_CBMap.end() )
	{
		++cnt;
		if( cnt >= 100 ) { return -1;}
		++m_CallbackId;
		if( m_CallbackId < 1 ) { m_CallbackId = 1; }
		it = m_CBMap.find(m_CallbackId);
	}
	
	return m_CallbackId++;
}
/**
插入一个Transaction和Action的pair,并设置CallBackId
*/
int TransactionMgr::RequestCallbackId(Transaction* trans , IAction* act)
{
    if(trans == NULL || act == NULL)
    {
        LOG_DEBUG("TransactionMgr trans == NULL || act == NULL ");
        return -1;
    }
	int id = _GetCBId();
	LOG_DEBUG("TransactionMgr RequestCallbackId %s Ptr[%p] %s CB[%d] ", trans->GetString(),trans,act->GetString() ,id);
	if( id < 0  ) 
	{
		return -1; 
	}
	if( m_CBMap.insert(std::make_pair(id,std::make_pair(trans,act))).second )
	{
		return id;
	}
	LOG_DEBUG("TransactionMgr RequestCallbackId Conflict %s Ptr[%p] %s CB[%d] ", trans->GetString(),trans,act->GetString(),id);
	return -1;
}

void TransactionMgr::ClearCallback(int cbid)
{
	LOG_TRACE("TransactionMgr ClearCallback %d ",cbid);
	m_CBMap.erase(cbid);
}

int TransactionMgr::Response(const ITransNotify& ntf)
{
	CallbackMap::iterator it = m_CBMap.find(ntf.GetCBId());
	if( it == m_CBMap.end() )
	{
    	LOG_ERROR("TransactionMgr Response NotFound %s CB[%d]",
            ntf.GetString(), ntf.GetCBId());
	    return -1;
	}

    if(it->second.first == NULL || it->second.second == NULL)
    {
		LOG_ERROR("TransactionMgr Response NULL %s CB[%d]",
            ntf.GetString(), ntf.GetCBId());
		return -1;
    }

	Transaction* pTrans = it->second.first;
    IAction* pAction = it->second.second;
	LOG_DEBUG("TransactionMgr Response %s CB[%d] trans:%s action:%s",
        ntf.GetString() , 
        ntf.GetCBId(),
        pTrans->GetString(), 
        pAction->GetString()
        );
	pTrans->PrepareResponse();
	return pAction->Response(ntf).Process();
}
