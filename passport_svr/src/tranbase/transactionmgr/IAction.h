/*******************************************************************************
* Copyright(C) Tencent 2012
********************************************************************************/
#ifndef SRC_UTILLOGIC_TRANSACTIONMGR_IACTION_H_
#define SRC_UTILLOGIC_TRANSACTIONMGR_IACTION_H_

#include "tranbase/transactionmgr/ITransNotify.h"

typedef uint64_t TActionToken;
class Transaction;

#define SEQUENCE true
#define CONCURRENT false



class Transaction;

class IActionReturn
{
public:
	IActionReturn(Transaction& trans)
		:m_rTrans(trans)
	{}
    virtual ~IActionReturn(){}
    virtual int Process() = 0;
	virtual const char* GetString() = 0;
protected:
	Transaction& m_rTrans;
};
//// 本活动已正常启动，需要继续等待异步返回
class IActionReturnWait: public IActionReturn
{
public:
	IActionReturnWait(Transaction& trans)
		:IActionReturn(trans)
	{}
    virtual ~IActionReturnWait(){}
    virtual int Process();
	virtual const char* GetString() { return "Wait"; }
};


class IActionReturnNext: public IActionReturn
{
public:
	IActionReturnNext(Transaction& trans)
		:IActionReturn(trans)
	{}
    virtual ~IActionReturnNext(){}
    virtual int Process();
	virtual const char* GetString() { return "Next"; }
};

class IActionReturnFail: public IActionReturn
{
public:
	IActionReturnFail(Transaction& trans)
		:IActionReturn(trans)
	{}
    virtual ~IActionReturnFail(){}
    virtual int Process();
	virtual const char* GetString() { return "Fail"; }
};
class IActionReturnEnd: public IActionReturn
{
public:
	IActionReturnEnd(Transaction& trans)
		:IActionReturn(trans)
	{}
    virtual ~IActionReturnEnd(){}
    virtual int Process();
	virtual const char* GetString() { return "End"; }
};
class IActionReturnRedo: public IActionReturn
{
public:
	IActionReturnRedo(Transaction& trans)
		:IActionReturn(trans)
	{}
    virtual ~IActionReturnRedo(){}
    virtual int Process();
	virtual const char* GetString() { return "Redo"; }
};

class IAction
{ 
public: 
	explicit IAction(Transaction& trans )
		:m_Transaction(trans)
		, Wait(trans)
		, Next(trans)
		, Fail(trans)
		, Redo(trans)
		, End(trans)
	{};
	virtual ~IAction(){}
    virtual uint32_t GetId() const = 0;
	const char* GetString() const {return GetMacroName(GetId());};
    void SetActionIdx(int32_t iIdx){m_ActionIdx = iIdx;}
    int32_t GetActionIdx(){return m_ActionIdx;}
	//取得包含action的事务
	virtual Transaction& GetTransaction() { return m_Transaction; }
	//重新执行事务前调用，以重置action状态
	virtual int Reset(){ return 0;}
	// 取得回调id
	int RequestCallbackId();
	IActionReturn& Start();
	IActionReturn& Response(const ITransNotify& ntf);
protected:
	// Start之前做相应定制
	virtual IActionReturn& PreStart() { return Next; }
	// Response之后做相应定制
	virtual IActionReturn& PostResponse(const ITransNotify& ntf) { return Next; }
	// 启动响应虚函数，活动启动时做相应定制
    virtual IActionReturn& OnStart() = 0;

    // 异步回调响应虚函数，活动在异步响应时做相应定制
    virtual IActionReturn& OnAsynResponse(const ITransNotify& ntf) = 0;
	Transaction& m_Transaction;
	int32_t m_ActionIdx;
	IActionReturnWait Wait;
	IActionReturnNext Next;
	IActionReturnFail Fail;
	IActionReturnRedo Redo;
	IActionReturnEnd End;
}; 


#endif // end of SRC_UTILLOGIC_TRANSACTIONMGR_IACTION_H_
