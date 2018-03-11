//
//  PassportServer.cpp
//  passport_svr
//
//  Created by Wayne on 6/9/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#include "PassportServer.h"

#include "common/LogPrinter.h"
#include "database/PassPortDatabaseConnection.h"


#include "vnVariableScript.h"
#include "curl/CurlWrapper.h"
#include "curl/CurlManager.h"

#include <thread>
#include <iostream>

#include "adminlisten/AdminListen.h"
#include "clientlisten/ClientListen.h"
#include "gamesvrlisten/GamesvrListen.h"
#include "manager/DirtyManager.h"
#include "manager/DirtyManager2.h"
#include "manager/NicknameManager.h"
#include "manager/PlayerManager.h"
#include "manager/RsaKeyManager.h"
#include "manager/SessionManager.h"
#include "manager/TimerManager.h"
#include "tranbase/notifymanager/NotifyMgr.h"
#include "transaction/GSTransaction.h"
#include "transaction/GSTransactionMgr.h"
#include "transaction/GSTransContext.h"
#include "transaction/notifymanager/GSNotifyMgr.h"
#include "WorldManager.h"
#include "vnBase64.h"
#include "tool/crypt/Crypt.h"
#include "vnVariableAccessor.h"

PassPortDatabaseWorkerPool PassPortDatabase;

IMPL_SERVER_APP(PassportServer)

bool PassportServer::initialize(int argc, const char *argv[]) {
    if (!sWorldManager.loadBaseConfig()) {
        return false;
    }

    if (!sWorldManager.m_console_mode) {
        Log::instance().printer(vnnew LogPrinterFile("logs"));
    }
	if (!_startDB()) {
		return false;
	}
	sWorldManager.loadFromDB();
	sWorldManager.loadTemplet();

	if (!sRsaKeyManager.init())
	{
		VN_LOG_FATAL("RsaKeyManager init FAILED!");
		return false;
	}
	VN_LOG_DEBUG("RsaKeyManager init OK! ");

    
    m_linkPool = LinkPool::create(sWorldManager.m_thread_count);
	if (!m_linkPool) {
		return false;
	}
    VN_LOG_DEBUG("LinkPool init OK! ");

    sWorldManager.SetLinkPool(m_linkPool);

//	if (DirtyManager::instance().init() != 0) {
//		VN_LOG_FATAL("DirtyManager init FAILED!");
//		return false;
//	}
//	VN_LOG_DEBUG("DirtyManager init OK! ");

	if (DirtyManager2::instance().init() != true) {
		VN_LOG_FATAL("DirtyManager2 init FAILED!");
		return false;
	}
	VN_LOG_DEBUG("DirtyManager2 init OK! ");

	if (!NicknameManager::instance().init()) {
		VN_LOG_FATAL("NicknameManager init FAILED!");
		return false;
	}
	VN_LOG_DEBUG("NicknameManager init OK! ");

	sCurlManager.init(m_linkPool,90000);
	VN_LOG_DEBUG("CurlManager init OK! ");

	sPlayerManager.init();
	VN_LOG_DEBUG("PlayerManager init OK! ");

	m_clientListen = m_linkPool->listen(&ClientListen::create, NetAddress(0, sWorldManager.m_client_port), true);
	if (!m_clientListen) {
		VN_LOG_FATAL("listen *:" << sWorldManager.m_client_port << " FAILED!");
		return false;
	}
	VN_LOG_DEBUG("Client Listener init OK! ");

	m_adminListen = m_linkPool->listen(&AdminListen::create, NetAddress(0, sWorldManager.m_admin_port), true);
	if (!m_adminListen) {
		VN_LOG_FATAL("listen *:" << sWorldManager.m_admin_port << " FAILED!");
		return false;
	}
	VN_LOG_DEBUG("Admin Listener init OK! ");

	 m_gamesvrListen= m_linkPool->listen(&GamesvrListen::create, NetAddress(0, sWorldManager.m_gamesvr_port), true);
	if (!m_gamesvrListen) {
		VN_LOG_FATAL("listen *:" << sWorldManager.m_gamesvr_port << " FAILED!");
		return false;
	}
	sWorldManager.SetGamesvrListen(m_gamesvrListen);
	VN_LOG_DEBUG("Gamesvr Listener init OK! ");



	if (!m_linkPool->listen([this]() {return &m_http_listen;}, { 0, sWorldManager.m_http_port }, false)) {
		VN_LOG_ERROR("can NOT listen *:" << sWorldManager.m_http_port);
		return false;
	}
	VN_LOG_DEBUG("Http Listener init OK! ");

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

	VN_LOG_INFO("Passport start OK");
	return true;
}

void PassportServer::finalize() {
    if (m_clientListen) {
        m_clientListen->close();
    }
    if (m_adminListen) {
    	m_adminListen->close();
    }
    if (m_gamesvrListen) {
    	m_gamesvrListen->close();
    }
	m_http_listen.close();
    
    //SessionManager::instance().closeAll();
	sCurlManager.Destroy();
    if (m_linkPool) {
        m_linkPool->release();
    }
    _stopDB();
    VN_LOG_INFO("stop.");
}

/*
bool PassportServer::_loadConfig() {
    VariableScript vs;
    m_config = vs.parse({0, "config.var"});
    if (!m_config) {
        VN_LOG_FATAL("can NOT load config!");
        return false;
    }
    
    m_thread_count = (size_t)m_config->queryAttributeInt32("thread_count", (int)m_thread_count);
    m_client_port = (u16)m_config->queryAttributeInt32("client_port", (int)m_client_port);
    m_gamesvr_port = (u16)m_config->queryAttributeInt32("gamesvr_port", (int)m_gamesvr_port);
    m_admin_port = (u16)m_config->queryAttributeInt32("admin_port", (int)m_admin_port);
    m_console_mode = m_config->queryAttributeBool("console_mode", m_console_mode);
    m_http_port = (u16)m_config->queryAttributeInt32("http_port", (int)m_http_port);

    VariableAccessor<Variable_int32, Variable_bool, Variable_number> va(m_config->queryAttributeObject("return_money"));
    while (va.fetch()) {
    	auto	pfid = va.at<0>()->value();
    	auto isReturn = va.at<1>()->value();
    	auto persent = va.at<2>()->getFloat();
    	sWorldManager.m_moneyReturnInfos.insert({pfid, WorldManager::MoneyReturn(persent,isReturn)});

    	WorldManager::MoneyReturn& tmp = sWorldManager.m_moneyReturnInfos.at(pfid);
    	VN_LOG_DEBUG("pfid="<<pfid<<" isBeginReturn="<<tmp.isBeginReturn<<" persent="<<tmp.persent);
    }

    return true;
}*/
RandomEngine _randomEngine;
bool PassportServer::_console_command(RefCountedPtr<Variable_object> cmds) {
    auto cmd = cmds->at(0).dyn_cast<Variable_id>();
    if (!cmd) {
        return true;
    }
    if (cmd->value() == "exit") {
        quit();
        return false;
    }
    if (cmd->value() == "gettoken"){
    	u64 curTime = time(NULL);
    		if(curTime > sWorldManager.expires_in){
    			VN_LOG_DEBUG("expires_in:"<<sWorldManager.expires_in<<"  curTime:"<<curTime);
    			//token过期
    			//https://rest.gotye.com.cn/api/GetFriends
    			HttpRequestPtr aReqPtr(new HttpRequestC);
    			aReqPtr->host = "rest.gotye.com.cn";
    			aReqPtr->content = "/api/accessToken";
    			aReqPtr->isPost = true;
    			aReqPtr->isHttps = true;

    			aReqPtr->postUri = "{\n";
    			aReqPtr->postUri += "\"username\":\"";
    			aReqPtr->postUri += sWorldManager.GOTYE_UserName;
    			aReqPtr->postUri += "\",";
    			aReqPtr->postUri += "\"grant_type\":\"";
    			aReqPtr->postUri += "password";
    			aReqPtr->postUri += "\",";
    			aReqPtr->postUri += "\"password\":\"";
    			aReqPtr->postUri += sWorldManager.GOTYE_Password;
    			aReqPtr->postUri += "\",";
    			aReqPtr->postUri += "\n}";
    			aReqPtr->callBackFn = [](const HttpResponse& _httpResponse ) {
    				BufferPtr pBuffer = _httpResponse.content();
    				vn::RefCountedPtr<vn::Variable_object>	vJSONPtr = _getJsonObj(_httpResponse);
    				if (vJSONPtr.get()) {
    					sWorldManager. expires_in = vJSONPtr->queryAttributeInt64("expires_in",0);
    					if(sWorldManager.expires_in <=0) {
    						return;
    					} else {
    						sWorldManager.expires_in+=time(NULL);
    					}
    					sWorldManager.access_token = vJSONPtr->queryAttributeString("access_token","");
    				}
    			};
    		/*	uint64_t ps = getpagesize();
			uint64_t page = (uint64_t(&(aReqPtr->callBackFn)) & ~(ps -1));
			VN_LOG_DEBUG("callBackFn["<<(uint64_t)&(aReqPtr->callBackFn)<<"]  mprotect from ["<<page<<"]");
			if( mprotect((void*)page, ps, PROT_READ)) {
				handle_error("mprotect");
			}*/
    			sCurlManager.CurlSend(aReqPtr);
    		}
    }
   if (cmd->value() == "friend") {
	   //从亲加拉数据，插入数据库
	   PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(Test_SEL_ACCOUNT_ALL);
	   int cnt = 0;
	   	//同步查询
	   PreparedQueryResult result = PassPortDatabase.SynQuery(stmt);
	   if (result && result->GetRowCount())	{
			do {
				if(cnt>1){
					break;
				}
				u64 ppid = (*result)[0].GetUInt64();
				VN_LOG_DEBUG("index = "<<cnt++ << "   ppid="<<ppid);
				std::vector<u8> ppidVtr;
				ppidVtr.resize(sizeof(u64));
				memcpy(ppidVtr.data(), &ppid, sizeof(u64));
				str8 userAccount = Base64::encode(ppidVtr.data(), sizeof(u64));

				HttpRequestPtr aReqPtr(new HttpRequestC);
				aReqPtr->host = "rest.gotye.com.cn";
				aReqPtr->content = "/api/GetFriends";
				aReqPtr->isPost = true;
				aReqPtr->isHttps = true;
				aReqPtr->headMap["Authorization"] = "Bearer " + sWorldManager.access_token;
				//aReqPtr->headMap.insert(make_pair("Authorization",std::move(std::string("Bearer " + sWorldManager.access_token))));
				aReqPtr->postUri = "{\n";
				aReqPtr->postUri += "\"appkey\":\"";
				aReqPtr->postUri += sWorldManager.GOTYE_Appkey;
				aReqPtr->postUri += "\",";
				aReqPtr->postUri += "\"userAccount\":\"";
				aReqPtr->postUri += userAccount;
				aReqPtr->postUri += "\",";
				aReqPtr->postUri += "\"index\":";
				aReqPtr->postUri += "0";
				aReqPtr->postUri += ",";
				aReqPtr->postUri += "\"count\":";
				aReqPtr->postUri += sWorldManager.GOTYE_Max_Friend_Num;
				aReqPtr->postUri += "\n}";

				aReqPtr->callBackFn = [ppid](const HttpResponse& _httpResponse ) {
					BufferPtr pBuffer = _httpResponse.content();
					vn::RefCountedPtr<vn::Variable_object> vJSONPtr = _getJsonObj(_httpResponse);
					if (vJSONPtr.get()) {

						vn::RefCountedPtr<vn::Variable_object> entities = vJSONPtr->queryAttributeObject("entities");
						if (entities.get()) {
							for(size_t i =0; i< entities->size();i++) {
								VariablePtr pVar = entities->at(i);
								vn::RefCountedPtr<vn::Variable_object> entity = pVar.dyn_cast<Variable_object>();
								str8 friendaccount = entity->queryAttributeString("account","");
								std::vector<u8> ppidVtr = Base64::decode(friendaccount);
								if (ppidVtr.size() != sizeof(u64)) {
									VN_LOG_ERROR("input = "<<friendaccount << "   Base64::decode error!");
									break;
								}
								u64 friendppId = 0;
								memcpy(&friendppId, ppidVtr.data(), sizeof(u64));
								PreparedStatement*stmt = PassPortDatabase.GetPreparedStatement(CHAT_INS_FRIENDSLIST);
								stmt->setUInt64(0,ppid);
								stmt->setUInt64(1,friendppId);
								stmt->setUInt8(2, 1);
								PassPortDatabase.SynExecute(stmt);
							}
						}
					}
				};

//				uint64_t ps = getpagesize();
//				uint64_t page = (uint64_t(&(aReqPtr->callBackFn)) & ~(ps -1));
//				VN_LOG_DEBUG("callBackFn["<<(uint64_t)&(aReqPtr->callBackFn)<<"]  mprotect from ["<<page<<"]");
//				if( mprotect((void*)page, ps, PROT_READ)){
//						handle_error("mprotect");
//					}
				sCurlManager.CurlSend(aReqPtr);

			} while (result->NextRow());
	   }
    }
	if (cmd->value() == "encode") {
		/*str8 input = cmds->at(1).dyn_cast<Variable_string>()->value();
		u8 outbuff[input.length()/2];
		HexStrToByteArray(input,outbuff);
		VN_LOG_INFO(base64_encode((const char*)outbuff, sizeof(outbuff)));*/
		size_t cnt = cmds->size();
		for (size_t i = 1; i < cnt; i++) {
			s64 input = cmds->at(i).dyn_cast<Variable_int64>()->value();
			std::vector<u8> ppidVtr;
			ppidVtr.resize(sizeof(u64));
			memcpy(ppidVtr.data(), &input, sizeof(u64));
			str8 output = Base64::encode(ppidVtr.data(), sizeof(u64));
			VN_LOG_INFO(output);
		}
		return true;
	}
	if (cmd->value() == "decode") {
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
	/*	ppidVtr.push_back(0);
		std::string s = (const char *)ppidVtr.data();
		std::string s2;
		Crypt::Decrypt(s, s2);
		VN_LOG_INFO("Decrypt : "<< s2);*/
		return true;
	}
    if(cmd->value() == "rand"){

    	int num = cmds->at(1).dyn_cast<Variable_number>()->getInt32();
    	for(int i=0 ;i<num; i++){
    		VN_LOG_INFO("random : " << _randomEngine.rand32());
    	}
    }
	if (cmd->value() == "send2") {
		s32 count = cmds->at(1).dyn_cast<Variable_int32>()->value();
		s32 threadscount = cmds->at(2).dyn_cast<Variable_int32>()->value();
		std::function<void()> thread_task = [this,count]() {
			for (int i = 0; i < count; i++) {
				//https://rest.gotye.com.cn/api/GetFriends
				HttpRequestPtr aReqPtr(new HttpRequestC);
				aReqPtr->addr.from_string("61.135.169.121:443");
				aReqPtr->host = "www.baidu.com";
				aReqPtr->content = "/s?wd=qwe&rsv_spt=1&rsv_iqid=0xad7d4a690000e1f4&issp=1&f=8&rsv_bp=0&rsv_idx=2&ie=utf-8&tn=baiduhome_pg&rsv_enter=1&rsv_sug3=3&rsv_sug1=1";
				aReqPtr->isPost = false;
				aReqPtr->isHttps = true;

				aReqPtr->callBackFn = [](const HttpResponse& _httpResponse ) {
					BufferPtr pBuffer = _httpResponse.content();
					VN_LOG_DEBUG("receive size"<<pBuffer->size());
				};
				sCurlManager.CurlSend(aReqPtr);
			}
		};

		std::thread threads[threadscount];
		for (int i = 0; i < threadscount; i++) {
			threads[i] = std::thread(thread_task);
		}
		VN_LOG_DEBUG( "Done spawning threads! Now wait for them to join\n");
		for (auto& t : threads) {
			t.join();
		}
		VN_LOG_DEBUG( "All threads joined.\n");
		return true;
	}
    if(cmd->value() == "send"){
    	//https://rest.gotye.com.cn/api/GetFriends
		HttpRequestPtr aReqPtr(new HttpRequestC);
		//aReqPtr->addr.from_string("42.62.79.105:443");
		//aReqPtr->addr.port=443;
		aReqPtr->host = "rest.gotye.com.cn";
		aReqPtr->content = "/api/GetFriends";
		aReqPtr->isPost = true;
		aReqPtr->isHttps = true;
		aReqPtr->headMap["Authorization"] = "Bearer 6d3610fec67e614ef11f634aaae02e64";
		aReqPtr->postUri = "{\n";
		aReqPtr->postUri += "\"appkey\":\"";
		aReqPtr->postUri += "1490ac71-4d3a-43ae-9d09-32acfb6f6fad";
		aReqPtr->postUri += "\",";
		aReqPtr->postUri += "\"userAccount\":\"";
		aReqPtr->postUri += "123123";
		aReqPtr->postUri += "\",";
		aReqPtr->postUri += "\"index\":";
		aReqPtr->postUri += "0";
		aReqPtr->postUri += ",";
		aReqPtr->postUri += "\"count\":";
		aReqPtr->postUri += "10";
		aReqPtr->postUri += "\n}";

		aReqPtr->callBackFn = [](const HttpResponse& _httpResponse ) {
			BufferPtr pBuffer = _httpResponse.content();
			VN_LOG_DEBUG("Rev : "<<(const char *)pBuffer->lock());
			vn::RefCountedPtr<vn::Variable_object>	vJSONPtr = _getJsonObj(_httpResponse);
			if (vJSONPtr.get()) {

				/*vn::RefCountedPtr<vn::Variable_object>	entities = vJSONPtr->queryAttributeObject("entities");
				if (entities.get()) {
					VN_LOG_DEBUG("entities.get() ");
					for(size_t i =0; i< entities->size();i++){
						vn::RefCountedPtr<vn::Variable_object> entity =	entities->at(i);
					}

				}*/
			}
		};

		sCurlManager.CurlSend(aReqPtr);
		 return true;
    }

	if (cmd->value() == "dbtest") {
		s32 count = cmds->at(1).dyn_cast<Variable_int32>()->value();
		s32 threadscount = cmds->at(2).dyn_cast<Variable_int32>()->value();
		std::function<void()> thread_task = [this,count]() {
			for (int i = 0; i < count; i++) {
				GSTestContext ctx(123);
				TRANSACTIONALLOCMACRO(SGSTransactionMgr, GSTestTransaction, ctx);
			}
		};
		std::thread threads[threadscount];
				for (int i = 0; i < threadscount; i++) {
					threads[i] = std::thread(thread_task);
				}
				VN_LOG_DEBUG( "Done spawning threads! Now wait for them to join\n");
				for (auto& t : threads) {
					t.join();
				}
				VN_LOG_DEBUG( "All threads joined.\n");
	}
	if(cmd->value() == "test"){
		// 验证订单
		vn::VariableJSON m_variableJSON;
//		std::string value = "{\"status\":0,\"environment\":\"Production\",\"receipt\":{\"receipt_type\":\"Production\",\"adam_id\":1038332753,\"app_item_id\":1038332753,\"bundle_id\":\"com.longsgoo.icefire\",\"application_version\":\"1.3.1.697\",\"download_id\":73010402255493,\"version_exte     rnal_identifier\":815176621,\"receipt_creation_date\":\"2015-12-28 04:45:01 Etc/GMT\",\"receipt_creation_date_ms\":\"1451277901000\",\"receipt_creation_date_pst\":\"2015-12-27 20:45:01 America/Los_Angeles\",\"req     uest_date\":\"2015-12-28 04:45:03 Etc/GMT\",\"request_date_ms\":\"1451277903332\",\"request_date_pst\":\"2015-12-27 20:45:03 America/Los_Angeles\",\"original_purchase_date\":\"2015-12-18 03:56:04 Etc/GMT\",\"origin     al_purchase_date_ms\":\"1450410964000\",\"original_purchase_date_pst\":\"2015-12-17 19:56:04 America/Los_Angeles\",\"original_application_version\":\"1.3.1.697\",\"in_app\":[{\"quantity\":\"1\",\"product_id\":\"com.longsgoo.vd_10\",\"transaction_id\":\"330000125935025\",\"original_transaction_id\":\"330000125935025\",\"purchase_date\":\"2015-12-28 04:45:01 Etc/GMT\",\"purchase_date_ms\":\"14     51277901000\",\"purchase_date_pst\":\"2015-12-27 20:45:01 America/Los_Angeles\",\"original_purchase_date\":\"2015-12-28 04:45:01 Etc/GMT\",\"original_purchase_date_ms\":\"1451277901000\",\"original_purchase_date_     pst\":\"2015-12-27 20:45:01 America/Los_Angeles\",\"is_trial_period\":\"false\"}]}}";
		std::string value = "{\"status\":0,\"environment\":\"Production\",\"receipt\":{\"receipt_type\":\"Production\",\"adam_id\":1038332753,\"app_item_id\":1038332753,\"bundle_id\":\"com.longsgoo.icefire\",\"application_version\":\"1.3.1.697\",\"download_id\":31015150812063,\"version_external_identifier\":815176621,\"receipt_creation_date\":\"2015-12-30 07:02:26 Etc/GMT\",\"receipt_creation_date_ms\":\"1451458946000\",\"receipt_creation_date_pst\":\"2015-12-29 23:02:26 America/Los_Angeles\",\"request_date\":\"2015-12-30 07:16:50 Etc/GMT\",\"request_date_ms\":\"1451459810349\",\"request_date_pst\":\"2015-12-29 23:16:50 America/Los_Angeles\",\"original_purchase_date\":\"2015-12-24 16:23:27 Etc/GMT\",\"original_purchase_date_ms\":\"1450974207000\",\"original_purchase_date_pst\":\"2015-12-24 08:23:27 America/Los_Angeles\",\"original_application_version\":\"1.3.1.697\",\"in_app\":[]}}";
		vn::RefCountedPtr<vn::Variable_object> vJSONPtr = m_variableJSON.parse(value.c_str(), value.size());
		RefCountedPtr<Variable_object> receiptObj = vJSONPtr->queryAttributeObject("receipt");
		if (receiptObj ) {
			RefCountedPtr<Variable_object> in_app = receiptObj->queryAttributeObject("in_app");
			if(in_app && in_app->size()) {
				RefCountedPtr<Variable_object> app0 = in_app->at(0).dyn_cast<Variable_object>();
				if(app0) {
					std::string product_name = app0->queryAttributeString("product_id");
					for(auto item : sWorldManager.m_IOSProductTempletMap) {
						auto pProductTmpl = item.second;
						if(pProductTmpl && product_name == pProductTmpl->productName) {
							if(pProductTmpl->productId == 7) {
								//auto pProductTmpl = sWorldManager.m_IOSProductTempletMap[pOrder->productid];
								//result = handler->DeliverGoods(pOrder->orderid, pProductTmpl->price * 100, pOrder->receipt,OS_ID_IOS);
								VN_LOG_DEBUG( "ok\n");
							} else {
								//VN_LOG_WARN("app receipt productid is "<< pProductTmpl->productId <<" but order productid is "<< pOrder->productid);
								VN_LOG_DEBUG( "error:"<<pProductTmpl->productId);
							}
						}
					}
				}
			} else {
				VN_LOG_DEBUG( "error\n");
			}
		}
	}
	if(cmd->value() == "UserRegister"){
		std::string account;
		std::string password="1234";
		std::string phonenumber = "0000";
		for (int i = 100; i < 500; i++) {
			account = "test";
			account += std::to_string(i);

			PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_SEL_BY_NAME);
			stmt->setString(0, account);
			PreparedQueryResult aResult = PassPortDatabase.SynQuery(stmt);
			//结果未0
			if (aResult && aResult->GetRowCount()) {
				VN_LOG_DEBUG( "重复的用户名："<<account.c_str());
				break;
			}
			//密码加密
			std::string password_encrypt;
			if (!Crypt::Encrypt(password, password_encrypt)) {
				VN_LOG_DEBUG( "加密错误："<<account.c_str());
				break;
				break;
			}
			std::string s = Base64::encode(password_encrypt.c_str(), password_encrypt.length());

			//向数据库插入数据
			stmt = PassPortDatabase.GetPreparedStatement(PASSPORT_INS_ACCOUNT_LONGSGOO);
			stmt->setString(0, account);
			stmt->setString(1, s);
			stmt->setString(2, phonenumber);
			bool isExecute = PassPortDatabase.SynExecute(stmt);
			if(isExecute){
				VN_LOG_DEBUG( "注册成功："<<account.c_str());
			} else {
				VN_LOG_DEBUG( "注册失败："<<account.c_str());
			}

		}
	}
    return true;
}
bool PassportServer::_startDB(){
	 mysql_library_init(-1, NULL, NULL);
	 if (!PassPortDatabase.Open(sWorldManager.m_sDBInfo, sWorldManager.m_dbThreads)) {
		VN_LOG_FATAL("Cannot connect to database");
		return false;
	}
	return true;
}
bool PassportServer::_stopDB(){
	PassPortDatabase.Close();
	 mysql_library_end();
	return true;
}

void PassportServer::update(f32 deltaTime){
	SGSTransactionMgr.Process();
	SNotifyManager.Process();
	sTimerManager.updateTimers(deltaTime);
}
