#include "GSAction.h"

#include "../database/PassPortDatabaseConnection.h"

extern PassPortDatabaseWorkerPool PassPortDatabase;

Test::Test(Transaction& trans, GSTestContext& ctx) :
		IAction(trans), m_Ctx(ctx) {

}
Test::~Test() {

}
// 启动响应虚函数，活动启动时做相应定制
IActionReturn& Test::OnStart() {
	m_cbid = RequestCallbackId();
	if (m_cbid < 0)
		return Fail;

	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_SEL_MAXPPID);
	PassPortDatabase.ActionQuery(stmt, m_cbid);
	return Wait;
}
// 异步回调响应虚函数，活动在异步响应时做相应定制
IActionReturn& Test::OnAsynResponse(const ITransNotify& ntf) {
	if (m_status == 0) {
		const DBQueryNtf& ltntf = static_cast<const DBQueryNtf&>(ntf);
		PreparedQueryResult aResult = ltntf.GetUserData();

		if (aResult && aResult->GetRowCount()) {
			do {
				VN_LOG_DEBUG("platformid : " << (*aResult)[0].GetUInt32()<< "  Max ppID : "<< ((*aResult)[1].GetUInt64()& 0xFFFFF));
			} while ((*aResult).NextRow());
		}
		return Next;
		PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(Test_INS_Test);
		stmt->setString(0, "test");
		stmt->setString(0, "test");
		PassPortDatabase.ActionExecute(stmt, m_cbid);
		m_status = 1;
		return Wait;
	} else if (m_status == 1) {
		const DBExecuteNtf& ltntf = static_cast<const DBExecuteNtf&>(ntf);
		bool result = ltntf.GetUserData();

		VN_LOG_DEBUG("Test_INS_Test result : " << result);
		return Next;
	}
	return Fail;
}




TestInsert::TestInsert(Transaction& trans, GSTestContext& ctx) :
		IAction(trans), m_Ctx(ctx) {

}
TestInsert::~TestInsert() {

}
// 启动响应虚函数，活动启动时做相应定制
IActionReturn& TestInsert::OnStart() {
	m_cbid = RequestCallbackId();
	if (m_cbid < 0)
		return Fail;

	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(Test_INS_Test);
	stmt->setString(0, "TestInsert");
	PassPortDatabase.ActionExecute(stmt, m_cbid);
	return Wait;
}
// 异步回调响应虚函数，活动在异步响应时做相应定制
IActionReturn& TestInsert::OnAsynResponse(const ITransNotify& ntf) {

	const DBExecuteNtf& ltntf = static_cast<const DBExecuteNtf&>(ntf);
	bool result = ltntf.GetUserData();

	VN_LOG_DEBUG("Test_INS_Test result : " << result);
	return Next;

}

UpdateToken::UpdateToken(Transaction& trans,  uint64_t ppid,uint32_t pptoken) :
		IAction(trans), m_ppid(ppid), m_pptoken(pptoken) {

}
UpdateToken::~UpdateToken() {

}
// 启动响应虚函数，活动启动时做相应定制
IActionReturn& UpdateToken::OnStart() {
	m_cbid = RequestCallbackId();
	if (m_cbid < 0)
		return Fail;

	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_UPD_ACCOUNT_TOKEN_BY_PPID);
	stmt->setString(0, "");
	stmt->setUInt64(1, m_ppid);
	PassPortDatabase.ActionExecute(stmt, m_cbid);
	return Wait;
}
// 异步回调响应虚函数，活动在异步响应时做相应定制
IActionReturn& UpdateToken::OnAsynResponse(const ITransNotify& ntf) {

	const DBExecuteNtf& ltntf = static_cast<const DBExecuteNtf&>(ntf);
	bool result = ltntf.GetUserData();

	VN_LOG_DEBUG("UpdateToken result : " << result);
	return Next;

}
