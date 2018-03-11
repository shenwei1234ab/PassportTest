/*******************************************************************************
@ ��Ȩ����(C) Tencent 2013
********************************************************************************/
#ifndef __UTIL_NOTIFYMANAGER_NOTIFYDEF_H__
#define __UTIL_NOTIFYMANAGER_NOTIFYDEF_H__

#include "tranbase/macrodef/UtilMacro.h"
#include "tranbase/Util.h"

#define CLONE(ptr,type)\
type* ptr=NULL;\
ptr = util::MemNew(ptr,1);\
ptr=new(ptr)type(*this);\
return ptr;

//#define RELEASE(ptr) util::MemDelete(ptr,1);
#define RELEASE(ptr) delete ptr;

class INotify
{
public:
	INotify(bool isAsyn)
		:m_bIsAsyn(isAsyn)
	{}
    virtual uint32_t GetId() const = 0;
	const char* GetString() const{ return GetMacroName(GetId()); };
	virtual ~INotify(){};
	bool IsAsynNtf() const { return m_bIsAsyn; }
	virtual INotify* Clone() const { return NULL; };
protected:
	bool m_bIsAsyn;
};


#endif // end of __UTIL_NOTIFYMANAGER_NOTIFYDEF_H__
