#include "GSAction.h"

#include "../database/NetLogDBConnection.h"
#include "../manager/configmgr/ConfigMgr.h"

extern NetLogDBWorkerPool NetLogDatabase;

Insert::Insert(Transaction& trans, GSLogContext& ctx) :
		IAction(trans), m_Ctx(ctx) {

}
Insert::~Insert() {

}

IActionReturn& Insert::OnStart() {
	m_cbid = RequestCallbackId();
	if (m_cbid < 0)
		return Fail;

	PreparedStatement *stmt = 0;
	int nStatementID = sConfigMgr.GetStatementID(m_Ctx.m_LogName);
	if(nStatementID < 0){
		VN_LOG_ERROR("can't find log name:"<<m_Ctx.m_LogName);
		return Fail;
	}
	stmt = NetLogDatabase.GetPreparedStatement(nStatementID);
	const std::vector<std::string>& cntxtVtr = m_Ctx.GetContextMap();
	int i = 0;
	for(auto arg : cntxtVtr){
		stmt->setString(i++, arg);
	}
	VN_LOG_DEBUG("receive log:"<<m_Ctx.m_LogName);
	NetLogDatabase.ActionExecute(stmt, m_cbid);

	return Wait;
}

IActionReturn& Insert::OnAsynResponse(const ITransNotify& ntf) {
	const DBExecuteNtf& ltntf = static_cast<const DBExecuteNtf&>(ntf);
	bool result = ltntf.GetUserData();

	VN_LOG_DEBUG("Test_INS_Test result : " << result);

	if (!result) {
		return Fail;
	}
	return Next;
}
