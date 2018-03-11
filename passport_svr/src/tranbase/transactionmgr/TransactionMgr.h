/*******************************************************************************
* Copyright(C) Tencent 2012
********************************************************************************/
#ifndef SRC_UTILLOGIC_TRANSACTIONMGR_TRANSACTIONMGR_H_
#define SRC_UTILLOGIC_TRANSACTIONMGR_TRANSACTIONMGR_H_

#include <vector>
#include <mutex>
#include <assert.h>
#include <tr1/unordered_map>

#include "tranbase/transactionmgr/ITransNotify.h"
#include "tranbase/transactionmgr/Transaction.h"
#include "../Util.h"


class TransactionMgr
{ 
public: 
    // 目前暂时用map，今后换成mempool

	typedef HASHMAPDEFINE(uint64_t, Transaction*) TransactionMap;
	typedef std::vector<uint64_t> DelVector;
	typedef std::pair<Transaction*,IAction*> TransActPair;
	typedef HASHMAPDEFINE(int,TransActPair) CallbackMap;

	TransactionMgr(int max = 0 ,int transize = 0 , int maxaction = 0,int actionsize = 0);
	virtual ~TransactionMgr();

    // 响应回调接口，会通过ITransNotify的GetCBId来获取需要继续投递的活动
	virtual int Response(const ITransNotify& ntf);

    // 添加一个事务，并启动该事务
	virtual int AddTransaction(Transaction* trans );

    // 每个游戏主逻辑循环都要运行，会做事务的实际删除工作
    virtual void Process();

    
	Transaction* GetTransaction(uint64_t tid);
	
    // 无论事务状态，直接登记删除事务
    void DeleteTransaction(uint64_t tid);

	// 事务管理器内部分配回调ID
	int RequestCallbackId(Transaction* trans , IAction*);

    // 清理具体的某个callbackid
	void ClearCallback(int cbid);

	// 清理Transaction
	void FreeTrans(Transaction* trans);
	void FreeAction(IAction* action);
/*	template<typename Trans>
	void GetTransBuffer(Trans*& ptr)
	{
		ptr = (Trans*)malloc(sizeof(Trans)); //util::MemNew(ptr,1);
	}
	template<typename Action>
	void GetActionBuffer(Action*& ptr)
	{
		//ptr = util::MemNew(ptr,1);
		ptr = (Action*)malloc(sizeof(Action));
	}*/

	size_t GetTransCnt() const { return m_TransMap.size(); }
protected:
	int _GetCBId();
	void ClearCloseTrans();
	void ClearTrans();
	TransactionMap m_TransMap;
	int64_t m_TransID;
	DelVector m_DelVec;
	int m_CallbackId;
	CallbackMap m_CBMap;

	bool m_usepool;
	bool m_stop;//调用析构

	std::mutex m_TransMapLock;
};

#endif // end of SRC_UTILLOGIC_TRANSACTIONMGR_TRANSACTIONMGR_H_
