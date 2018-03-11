#include "tranbase/transactionmgr/IAction.h"
#include "tranbase/logmgr/Log.h"
#include "tranbase/transactionmgr/Transaction.h"


//在Transaction中给Action申请一个CallbackID
int IAction::RequestCallbackId()
{
	return GetTransaction().RequestCallbackId(this);
}
int IActionReturnWait::Process()
{
	return 0;
}
int IActionReturnNext::Process()
{
	m_rTrans.DoActionEnd();
	return 0;
}
int IActionReturnFail::Process()
{
	m_rTrans.DoFail();
	return -1;
}
int IActionReturnEnd::Process()
{
	m_rTrans.DoForceEnd();
	return 0;
}
int IActionReturnRedo::Process()
{
	m_rTrans.DoRestart();
	return 0;	
}

IActionReturn& IAction::Start()
{
	IActionReturn& pretmp = PreStart();

	if( &pretmp == &Fail )
	{
		LOG_ERROR("Transaction %s, %p, Action %s, %p, PreStart %s", m_Transaction.GetString(), &m_Transaction, GetString(), this , pretmp.GetString());
	}
	else
	{
		LOG_DEBUG("Transaction %s, %p, Action %s, %p, PreStart %s", m_Transaction.GetString(), &m_Transaction, GetString(), this , pretmp.GetString());
	}

	if ( &pretmp != &Next )
	{
		if( &pretmp == &Wait )//prestart 不能wait做异步
			return Fail;
		return pretmp;
	}

	IActionReturn& tmp = OnStart();

	if( &tmp == &Fail )
	{
		LOG_ERROR("Transaction %s,%p,Action %s,%p,Start %s",m_Transaction.GetString(),&m_Transaction,GetString(),this ,tmp.GetString());
	}
	else
	{
		LOG_DEBUG("Transaction %s,%p,Action %s,%p,Start %s",m_Transaction.GetString(),&m_Transaction,GetString(),this ,tmp.GetString());
	}

	return tmp;
}

IActionReturn& IAction::Response(const ITransNotify& ntf)
{
	IActionReturn& tmp = OnAsynResponse(ntf);

	if( &tmp == &Fail )
	{
		LOG_ERROR("Transaction %s,Action %s,Ntf %s Response %s",m_Transaction.GetString(),GetString(),ntf.GetString(),tmp.GetString());
	}
	else
	{
		LOG_DEBUG("Transaction %s,Action %s,Ntf %s Response %s",m_Transaction.GetString(),GetString(),ntf.GetString(),tmp.GetString());
	}

	if( &tmp == &Next )
	{
		IActionReturn& posttmp = PostResponse(ntf);
		if( &posttmp == &Fail )
		{
			LOG_ERROR("Transaction %s,Action %s,Ntf %s PostResponse %s",m_Transaction.GetString(),GetString(),ntf.GetString(),posttmp.GetString());
		}
		else
		{
			LOG_DEBUG("Transaction %s,Action %s,Ntf %s PostResponse %s",m_Transaction.GetString(),GetString(),ntf.GetString(),posttmp.GetString());
		}
		return posttmp;
	}
	return tmp;
}
