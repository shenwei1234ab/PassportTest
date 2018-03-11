/*******************************************************************************
@ 版权所有(C) Tencent 2012
********************************************************************************/
#ifndef __GAMESVR_TRANSACTION_GAMEACTION_H__
#define __GAMESVR_TRANSACTION_GAMEACTION_H__

#include "tranbase/transactionmgr/IAction.h"
#include "GSTransContext.h"
#include "macrodef/GamesvrMacro.h"

class Test : public IAction
{
public: 
	Test(Transaction& trans, GSTestContext& ctx);
	virtual ~Test();
	virtual uint32_t GetId() const { return MACROGAMESVR(ActionTest); }
    // 启动响应虚函数，活动启动时做相应定制
    virtual IActionReturn& OnStart();
    // 异步回调响应虚函数，活动在异步响应时做相应定制
    virtual IActionReturn& OnAsynResponse(const ITransNotify& ntf);
protected:
	GSTestContext& m_Ctx;
	int m_status = 0;
	int m_cbid = 0;
};

class TestInsert : public IAction
{
public:
	TestInsert(Transaction& trans, GSTestContext& ctx);
	virtual ~TestInsert();
	virtual uint32_t GetId() const { return MACROGAMESVR(ActionTestInsert); }
    // 启动响应虚函数，活动启动时做相应定制
    virtual IActionReturn& OnStart();
    // 异步回调响应虚函数，活动在异步响应时做相应定制
    virtual IActionReturn& OnAsynResponse(const ITransNotify& ntf);
protected:
	GSTestContext& m_Ctx;
	int m_status = 0;
	int m_cbid = 0;
};

class UpdateToken : public IAction
{
public:
	UpdateToken(Transaction& trans,   uint64_t ppid,uint32_t pptoken);
	virtual ~UpdateToken();
	virtual uint32_t GetId() const { return MACROGAMESVR(ActionUpdateToken); }
    // 启动响应虚函数，活动启动时做相应定制
    virtual IActionReturn& OnStart();
    // 异步回调响应虚函数，活动在异步响应时做相应定制
    virtual IActionReturn& OnAsynResponse(const ITransNotify& ntf);
protected:
	int m_status = 0;
	int m_cbid = 0;
	uint64_t m_ppid;
	uint32_t m_pptoken;
};


#endif // end of __GAMESVR_TRANSACTION_GAMEACTION_H__
