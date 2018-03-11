//
//  PassportServer.cpp
//  passport_svr
//
//  Created by Wayne on 6/9/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "common/LogPrinter.h"
#include "database/PassPortDatabaseConnection.h"


#include "vnVariableScript.h"


#include <thread>
#include <iostream>


#include "clientlisten/ClientListen.h"
#include "OPTDServer.h"

#include "WorldManager.h"
#include "manager/WorkQueueManager.h"
#include "manager/DC_File.h"
#include "vnBase64.h"
#include "tool/crypt/Crypt.h"
#include "vnVariableAccessor.h"
#include "vnRefCounted.h"
#include "PPLink/PPLinkManger.h"

#include <list>

PassPortDatabaseWorkerPool PassPortDatabase;

IMPL_SERVER_APP(OPTDServer)




bool OPTDServer::initialize(int argc, const char *argv[])
{
	//载入配置文件
    if (!sWorldManager.loadBaseConfig())
    {
    	VN_LOG_ERROR("loadBaseConfig failed");
    	return false;
    }
//    if(!sWorldManager.loadVersionData())
//    {
//    	VN_LOG_ERROR("loadVersionData failed");
//    	 return false;
//    }
    if(!sWorldManager.loadTBL())
     {
    	VN_LOG_ERROR("loadTBL failed");
       	 return false;
    }
    //开启work线程
    WorkQueueManager::instance().init(sWorldManager.m_num_pool_threads);


    //开启db线程todo..
	if (!_startDB()) {
		return false;
	}


    if (!sWorldManager.m_console_mode) {
        Log::instance().printer(vnnew LogPrinterFile("logs"));
    }
    //
    if (!DataCenter::instance().init()) {
        VN_LOG_ERROR("data center initialize failed!");
        return false;
    }

    if (!DataCenter::instance().loadServerData(sWorldManager.m_data)) {
        VN_LOG_ERROR("load server data failed!");
    }






	//VN_LOG_DEBUG("RsaKeyManager init OK! ");

    
    m_linkPool = LinkPool::create(sWorldManager.m_num_pool_threads);
	if (!m_linkPool) {
		return false;
	}
    VN_LOG_DEBUG("LinkPool init OK! ");

    sWorldManager.SetLinkPool(m_linkPool);




	//sPlayerManager.init();
	//VN_LOG_DEBUG("PlayerManager init OK! ");

	m_clientListen = m_linkPool->listen(&ClientListen::create, NetAddress(0,sWorldManager.m_listening_port), true);
	if (!m_clientListen) {
		VN_LOG_FATAL("listen *:" << sWorldManager.m_listening_port << " FAILED!");
		return false;
	}
	VN_LOG_DEBUG("Client Listener init OK! ");

	sPPLinkMgr.init(m_linkPool);

	//开启console线程
    if (sWorldManager.m_console_mode) {
		std::thread th([this]() {
			str8 line;
			VariableScript vs;
			for (;;) {
				std::getline(std::cin, line);
				if (!line.empty()) {
					auto cmds = vs.parse(line.c_str(), line.length(), nullptr);
					if (cmds && !cmds->empty()) {
						if (!_console_command(std::move(cmds))) {
							break;
						}
					} else {
						VN_LOG_ERROR("Invalid Console Command(s)!");
					}
				}
			}
		});
		th.detach();
	}

	VN_LOG_INFO("OPTD SVR start OK");
	return true;
}











void OPTDServer::finalize() {
    if (m_clientListen) {
        m_clientListen->close();
    }
    sPPLinkMgr.finalize();
	//sCurlManager.Destroy();
    if (m_linkPool) {
        m_linkPool->release();
    }

    _stopDB();
    VN_LOG_INFO("stop.");
}


bool OPTDServer::_console_command(RefCountedPtr<Variable_object> cmds) {
	auto cmd = cmds->at(0).dyn_cast<Variable_id>();
	if (!cmd) {
		return true;
	}
	if (cmd->value() == "exit") {
		quit();
		return false;
	}
	if (cmd->value() == "verify") {
		str8 ppid = cmds->at(1).dyn_cast<Variable_string>()->value();
		str8 pptoken = cmds->at(1).dyn_cast<Variable_string>()->value();
		PPLinkManger::CallbackFn callback = [](vn::s32 result,std::string& context) {
			VN_LOG_DEBUG("result["<<result<<"]  context:"<<context);
		};
		sPPLinkMgr.SendVerifyReq(std::move(callback),
				time(NULL),
				"1",
				"",
				(char*)ppid.c_str(),
				(char*)pptoken.c_str()
		);
	}
	if (cmd->value() == "encode")
	{
			/*str8 input = cmds->at(1).dyn_cast<Variable_string>()->value();
			u8 outbuff[input.length()/2];
			HexStrToByteArray(input,outbuff);
			VN_LOG_INFO(base64_encode((const char*)outbuff, sizeof(outbuff)));*/
			size_t cnt = cmds->size();
			for (size_t i = 1; i < cnt; i++)
			{
				s64 input = cmds->at(i).dyn_cast<Variable_int64>()->value();
				std::vector<u8> ppidVtr;
				ppidVtr.resize(sizeof(u64));
				memcpy(ppidVtr.data(), &input, sizeof(u64));
				str8 output = Base64::encode(ppidVtr.data(), sizeof(u64));
				VN_LOG_INFO(output);
			}
	}
		if (cmd->value() == "decode")
		{
			/*str8 input = cmds->at(1).dyn_cast<Variable_string>()->value();
			char outbuff[input.length()*4/3];
			int len =base64_decode(const_cast<char*>(input.c_str()), input.length(),outbuff);
			VN_LOG_INFO(ByteArrayToHexStr((const u8*)outbuff,len));*/
			size_t cnt = cmds->size();
			for (size_t i = 1; i < cnt; i++) {
				str8 input = cmds->at(i).dyn_cast<Variable_string>()->value();
				std::vector<u8> ppidVtr = Base64::decode(input);
				if (ppidVtr.size() != sizeof(u64)) {
					VN_LOG_ERROR("input = "<<input << "   Base64::decode error!");
					return true;
				}
				u64 openId = 0;
				memcpy(&openId, ppidVtr.data(), sizeof(u64));
				//VN_LOG_INFO(input<<" "<< openId);
				VN_LOG_INFO(openId);
			}
		}
    return true;
}
bool OPTDServer::_startDB(){
	 mysql_library_init(-1, NULL, NULL);
	 if (!PassPortDatabase.Open(sWorldManager.m_sDBInfo, sWorldManager.m_dbThreads)) {
		VN_LOG_FATAL("Cannot connect to database");
		return false;
	}
	return true;
}
bool OPTDServer::_stopDB(){
	PassPortDatabase.Close();
	 mysql_library_end();
	return true;
}

void OPTDServer::update(f32 deltaTime){
	sWorldManager.update(deltaTime);
	sPPLinkMgr.update(deltaTime);
	//SGSTransactionMgr.Process();
	//SNotifyManager.Process();
	//sTimerManager.updateTimers(deltaTime);
}
