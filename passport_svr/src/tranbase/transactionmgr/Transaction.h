/*******************************************************************************
* Copyright(C) Tencent 2012
********************************************************************************/
#ifndef SRC_UTILLOGIC_TRANSACTIONMGR_TRANSACTION_H_
#define SRC_UTILLOGIC_TRANSACTIONMGR_TRANSACTION_H_

#include <set>

#include "tranbase/transactionmgr/TransTimer.h"
#include "tranbase/transactionmgr/IAction.h"
class TransactionMgr;


struct ActionOp
{
    ActionOp()
        :m_pAction(NULL)
    {

    }
    ActionOp(IAction* pAction)
        :m_pAction(pAction)
    {

    }
    IAction* m_pAction;
};
class Transaction
{ 
public: 
	typedef std::vector<int> CBVec;
    typedef std::vector<ActionOp> ActionOpVec;


	explicit Transaction(TransactionMgr& mgr,time_t span);
	virtual ~Transaction();
    virtual uint32_t GetId() const = 0;
	const char* GetString() const {return GetMacroName(GetId());};
    // 启动事务，当该事务被添加到TransactionMgr里时，被启动
    virtual int OnStart();

    // 事务超时接口
	virtual int OnTimeOut();

    // 事务失败
	virtual int OnFail();

    // 事务结束
	virtual int OnEnd();

    // 当某个活动结束时会通知
	virtual int OnActionEnd();
	
	// 重置事务状态时调用
	virtual int OnReset();

	// 事务结束时，不论成败都会调用
	virtual int OnExeTransEnd() { return 0; }

	// 事务结束时，不论成败都会调用
	virtual int OnExeTransFini() { return 0; }

	// 是否支持重入
	virtual bool Reentrant() { return true; }

	void StartTimer(){ m_TransTimer.StartTimer(); };
	void CloseTimer(){ m_TransTimer.CloseTimer(); };


    // 获取本事务的id
    uint64_t GetTransactionId() const { return m_iIdx; }
    // 为某个活动取得一个callbackid
	int RequestCallbackId(IAction* act);
    //清除前面的callback信息
	void ClearCallback();
    
    // Action添加到Transaction，按执行顺序依次添加
    size_t AddAction(IAction*);
    int ClearAction();
    IAction* GetAction( uint32_t idx);
    ActionOp* GetActionOp( uint32_t idx);
    // 开启下一个（或一组并行）事务
    int StartNextActions();

    int AddWaitingResActionCount();

    // 事务开始
    int DoStart();
    // 事务失败
    int DoFail();
    // 事务结束
    int DoEnd();
	// 事务强行结束，逻辑上判断失败了
	int DoForceEnd();
    // 事务超时
    int DoTimeOut();
    // 事务中一个活动结束
    int DoActionEnd();
	// 重置事务状态
	int DoRestart();

	// 设置结果
	virtual void SetResult(int);
	virtual int GetResult();

	void AddAllAction()
	{
		_AddPreAction();
		_AddAction();
		_AddPostAction();
	};
	//在调用action response之前调用这个函数
	virtual void PrepareResponse(){};

	TransactionMgr & GetMgr() const
	{
		return m_TransMgr;
	}

protected: 
	// 加入事务的主要action
	virtual void _AddAction(){};
	// 加入事务预处理action
	virtual void _AddPreAction(){};
	// 加入事务结束需要做的actoin
	virtual void _AddPostAction(){};
    //IActionReturn _StartSingleAction(uint32_t idx);
	TransactionMgr& m_TransMgr;
    int m_iNextPhysicIdx;
    //int m_iWaitingCount;
    uint64_t m_iIdx;
	CBVec m_CBVec;
	TransTimer m_TransTimer;
    ActionOpVec m_ActionOpVec;
}; 
#endif // end of SRC_UTILLOGIC_TRANSACTIONMGR_TRANSACTION_H_
