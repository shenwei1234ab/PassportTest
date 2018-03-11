/*
 * DirtyManager2.cpp
 *
 *  Created on: 2015年9月27日
 *      Author: tanghailong
 */

#include "../../passportsvr/manager/DirtyManager2.h"

#include "../database/PassPortDatabaseConnection.h"

extern PassPortDatabaseWorkerPool PassPortDatabase;


DirtyManager2::~DirtyManager2() {
	// TODO Auto-generated destructor stub
}

bool DirtyManager2::init() {
	//加载数据库
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(DIRTY_SEL_ALL);
	//同步查询
	PreparedQueryResult result = PassPortDatabase.SynQuery(stmt);
	if (!(result && result->GetRowCount()))	{
		return false;
	}
	do {
			//int ret = LoadChar((*result)[0].GetString().c_str());
		auto s = (*result)[0].GetString();
		filter.add(s.c_str(), s.length());
			//m_keyWords.push_back((*result)[0].GetString());
	} while (result->NextRow());
	return true;
}

bool DirtyManager2::check(const str8 &s) {
	return filter.check(s.c_str(), s.length());
}

