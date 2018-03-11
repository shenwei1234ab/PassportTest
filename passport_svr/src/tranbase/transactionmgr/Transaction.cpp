#include "tranbase/logmgr/Log.h"
#include "tranbase/transactionmgr/Transaction.h"

#include "tranbase/logmgr/Log.h"
#include "tranbase/transactionmgr/ActionAllocator.h"
#include "tranbase/transactionmgr/TransactionMgr.h"

Transaction::Transaction(TransactionMgr& mgr,time_t span)
:m_TransMgr(mgr)
,m_iNextPhysicIdx(0)
,m_iIdx((uint64_t)this)
,m_TransTimer(*this,span)
{
	m_CBVec.reserve(10);
}
Transaction::~Transaction()
{
    ClearCallback();
    ClearAction();
	CloseTimer();
}
int Transaction::OnStart()
{
    return 0;
}

int Transaction::OnEnd()
{
    return 0;
}

int Transaction::OnFail()
{
    return 0;
}

int Transaction::OnActionEnd()
{
 
    return 0;
}
int Transaction::OnReset()
{
	return 0;
}
int Transaction::OnTimeOut()
{
    return 0;   
}
int Transaction::DoForceEnd()
{
	OnExeTransEnd();
    OnFail();
	//LOG_DEBUG("Transaction DoFail Clear CB %s Ptr[%lu]",GetString() , (uint64_t)this);
	ClearCallback();
	OnExeTransFini();
	LOG_DEBUG("Transaction DoForceEnd %s Ptr[%p]",GetString() , this);
	m_TransMgr.DeleteTransaction(GetTransactionId());
	return 0;
}

int Transaction::DoFail()
{
	OnExeTransEnd();
    OnFail();
	//LOG_DEBUG("Transaction DoFail Clear CB %s Ptr[%lu]",GetString() , (uint64_t)this);
	ClearCallback();
	OnExeTransFini();
	LOG_FATAL("TransactionDoFail Idx[%d] %s Ptr[%p] " , m_iNextPhysicIdx, GetString() , this);
	m_TransMgr.DeleteTransaction(GetTransactionId());

	return 0;
}
int Transaction::DoEnd()
{
	OnExeTransEnd();
    OnEnd();
	//LOG_DEBUG("Transaction DoEnd Clear CB %s Ptr[%lu]",GetString() , (uint64_t)this);
	ClearCallback();
	OnExeTransFini();
	LOG_DEBUG("Transaction DoEnd %s Ptr[%p]",GetString() , this);
	m_TransMgr.DeleteTransaction(GetTransactionId());
	return 0;
}

//将Action插入Transaction中，并申请一个回调ID
int Transaction::RequestCallbackId(IAction* act)
{
	int id = m_TransMgr.RequestCallbackId(this,act);
	if( id >= 0 )
	{
		m_CBVec.push_back(id);
	}
	return id;
}

void Transaction::ClearCallback()
{
	LOG_DEBUG("Transaction Clear CB Ptr[%p]", this);
	for( CBVec::iterator it = m_CBVec.begin() ; it != m_CBVec.end() ;  ++it )
	{
		m_TransMgr.ClearCallback(*it);
	}
	m_CBVec.clear();	
}

int Transaction::DoStart()
{
    LOG_DEBUG("Transaction %s,%p Start" , GetString(),this );
    OnStart();
    StartNextActions();
    return 0;
}

size_t Transaction::AddAction( IAction* pAction)
{
    if(pAction == NULL){return 0;}

	LOG_DEBUG("Transaction %s Ptr[%p] Add Action %s Ptr[%p]", GetString() , this, pAction->GetString() , pAction);

    ActionOp op(pAction);
    m_ActionOpVec.push_back(op);
    pAction->SetActionIdx(m_ActionOpVec.size() - 1);
    return m_ActionOpVec.size();
}

int Transaction::ClearAction()
{
	LOG_DEBUG("Transaction Ptr[%p] Clear Action ", this);

    for( ActionOpVec::iterator it = m_ActionOpVec.begin() ; it != m_ActionOpVec.end() ; ++it )
    {
		if( it->m_pAction != NULL )
		{
			LOG_DEBUG("Transaction Clear Action %s Ptr[%p]", it->m_pAction->GetString() , it->m_pAction);

			m_TransMgr.FreeAction(it->m_pAction);
		}
    }
    m_ActionOpVec.clear();
    return 0;
}


ActionOp* Transaction::GetActionOp( uint32_t idx )
{
    if(  idx < m_ActionOpVec.size() )
    {
        return &m_ActionOpVec[idx];
    }
    return NULL;
}


IAction* Transaction::GetAction( uint32_t idx)
{
    if(  idx < m_ActionOpVec.size() )
    {
        return m_ActionOpVec[idx].m_pAction;
    }
    return NULL;
}

int Transaction::StartNextActions()
{
    if(m_iNextPhysicIdx >= (int)m_ActionOpVec.size())
    {
        return DoEnd();
    }
	//LOG_DEBUG("Transaction StartNextAction Clear CB %s Ptr[%lu]",GetString() , (uint64_t)this);
    ClearCallback();

    ActionOp *pOp = &m_ActionOpVec[m_iNextPhysicIdx++];
    if( pOp == NULL || pOp->m_pAction == NULL)
    {
		LOG_ERROR("Transaction %s Ptr[%p] StartNextAction NULL",GetString(),this);
        DoFail();
        return -1;
    }
	LOG_DEBUG("Transaction %s Ptr[%p] StartNextAction %s Ptr[%p]",GetString(),this,pOp->m_pAction->GetString() , pOp->m_pAction);
	return pOp->m_pAction->Start().Process();

}

int Transaction::DoActionEnd()
{
	LOG_DEBUG("Transaction %s Ptr[%p] EndAction Idx[%d] ",GetString(),this, m_iNextPhysicIdx );
    OnActionEnd();
    StartNextActions();
    return 0;
}

int Transaction::DoTimeOut()
{
    LOG_ERROR("Transaction %s,%p TimeOut Idx[%d]",GetString(),this, m_iNextPhysicIdx );
    OnTimeOut();
    // 已经出错，但还是应该做些清理工作
    return DoFail();
}
int Transaction::DoRestart()
{
	LOG_DEBUG("Transaction DoRestart %s Ptr[%p] Idx[%d]",GetString(),this, m_iNextPhysicIdx);
	int ret = 0;
	for(ActionOpVec::iterator it = m_ActionOpVec.begin() ; it != m_ActionOpVec.end() ; ++it)
	{
		if( it->m_pAction != NULL )
			ret |= it->m_pAction->Reset();
	}
	ret |= OnReset();
	LOG_DEBUG("Transaction DoRestart Clear CB %s Ptr[%p]",GetString() , this);
	ClearCallback();
	m_iNextPhysicIdx = 0;
	DoStart();
	return ret;
}

// 设置结果
void Transaction::SetResult(int)	
{
}

int Transaction::GetResult()
{
	return -1;
}
