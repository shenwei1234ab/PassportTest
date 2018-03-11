/*******************************************************************************
* Copyright(C) Tencent 2012
********************************************************************************/
#ifndef SRC_UTILLOGIC_TRANSACTIONMGR_ACTIONALLOCATOR_H_
#define SRC_UTILLOGIC_TRANSACTIONMGR_ACTIONALLOCATOR_H_



#define TEMPLATEACTIONALLOCMACRO(isSeq,type, temp,...) type<temp>* p##type##temp = NULL; m_TransMgr.GetActionBuffer(p##type##temp); \
                                                 p##type##temp = new (p##type##temp) type<temp>(__VA_ARGS__); AddAction(p##type##temp, isSeq)

#define ACTIONALLOCMACRO(type,...) \
	do \
	{ \
		type* p##type = new type(__VA_ARGS__); \
		AddAction(p##type); \
	} while(0)

#if defined(WIN32) || defined(_WIN32)
#define TRANSACTIONALLOCMACRO( mgr,type, ...) \
{ \
	type* p##type = util::MemNew(p##type, 1); \
	p##type = new (p##type) type(*(mgr),__VA_ARGS__); \
	p##type->AddAllAction(); \
	int _result, _dumy = 0; \
	if ( ! mgr.AddTransaction(p##type) ) \
{ \
	p##type->ClearAction(); \
	mgr.FreeTrans(p##type); \
	_dumy = -1; \
} \
	_result = _dumy; \
}
#else
#define TRANSACTIONALLOCMACRO( mgr,type, ...) \
	({ \
	int _result, _dumy = -1; \
	type* p##type = new type(mgr,__VA_ARGS__); \
	p##type->AddAllAction(); \
	if ( ! mgr.AddTransaction(p##type) ) \
	{ \
		p##type->ClearAction(); \
		mgr.FreeTrans(p##type); \
		_dumy = -1; \
	} \
	else \
	{\
		_dumy = 0;\
	}\
	_result = _dumy; \
})
#endif

//#define ACTIONDEALLOCMACRO(ptr) util::MemDelete(ptr,1)
#define ACTIONDEALLOCMACRO(ptr) delete ptr

#endif // end of SRC_UTILLOGIC_TRANSACTIONMGR_ACTIONALLOCATOR_H_
