/*******************************************************************************
* Copyright(C) Tencent 2012
********************************************************************************/
#ifndef SRC_UTILLOGIC_TRANSACTIONMGR_ITRANSNOTIFY_H_
#define SRC_UTILLOGIC_TRANSACTIONMGR_ITRANSNOTIFY_H_

#include "tranbase/notifymanager/NotifyDef.h"



// 用于给每个action投递消息的统一接口
class ITransNotify : public INotify
{
public:
    ITransNotify(int cbid , bool isasyn = false)
        :INotify(isasyn)
		,m_iCallBackId(cbid)
    {

    }
	virtual ~ITransNotify(){};
    virtual uint32_t GetId() const = 0;
	const char* GetString() const {return GetMacroName(GetId());};
    virtual int GetCBId() const { return m_iCallBackId;}
protected: 
    int m_iCallBackId;
};

#endif // end of SRC_UTILLOGIC_TRANSACTIONMGR_ITRANSNOTIFY_H_
