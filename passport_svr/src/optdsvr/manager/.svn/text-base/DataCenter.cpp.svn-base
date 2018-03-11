//
//  DataCenter.cpp
//  optd_srv
//
//  Created by Wayne on 6/20/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "DataCenter.h"
#include "WorkQueueManager.h"
#include "LoggedUserManager.h"

#include "DC_File.h"
#include "DC_DataBase.h"

DataCenter * DataCenterCreator::create() {
    //return DC_File::create();
	return DC_DataBase::create();
}

void DataCenterCreator::destroy(DataCenter *p) {
    //DC_File::destroy(p);
	return DC_DataBase::destroy(p);
}


void DataCenter::_addUser(const LoggedUserPtr &user) {
	optd::WorkQueue *wq = WorkQueueManager::instance().selectWorkQueue();
	if(!wq)
	{
		VN_LOG_ERROR("wq NULL");
		return ;
	}
    wq->attach(user);
    LoggedUserManager::instance().addUser(user);
}

void DataCenter::_removeUser(const LoggedUserPtr &user) {
    LoggedUserManager::instance().removeUser(user->GetUUId());
    optd::WorkQueue * wq = user->queue();
    wq->detach(user);
}
