
#include "PreLoggingUser.h"
#include "../manager/LoggedUserManager.h"
#include "../WorldManager.h"
#include "../manager/WorkQueueManager.h"
#include "../manager/DC_DataBase.h"
#include "AsyncTask.h"
#include "../PPLink/PPLinkManger.h"
#include "BSTool.h"
extern PassPortDatabaseWorkerPool PassPortDatabase;
PreLoggingUser::~PreLoggingUser() {
    if (m_client) {
        m_client->setPacketHandler(nullptr);
        m_client->close();
    }
}

PacketPtr PreLoggingUser::createRequest(u32 pid) {
    return PacketFactory<PreLoggingUser>::instance().createRequest(pid);
}


void PreLoggingUser::update(f32 deltaTime) {
    switch (m_state) {
        case kInitial:
            _updateInitial(deltaTime);
            break;
        case kNormal:
            break;
        case kClosed:
            break;
    }
}


void PreLoggingUser::DbOperFailHandl()
{
	pkt::res_login_failed res;
	m_client->sendPacket(res,Session::kClose);
}

void PreLoggingUser::_updateInitial(f32 deltaTime) {
    m_timer += deltaTime;
    return;
    if (m_timer >= 10.f) {
        m_client->close();
    }
}

void PreLoggingUser::_closed() {
    if (m_client) {
        m_client->setPacketHandler(nullptr);
        m_client = nullptr;
        m_state = kClosed;
        queue()->detach({this, true});
    }
}


VN_IMPL_PACKET_REQUEST2(PreLoggingUser,pkt,req_version)
{

	m_state = kNormal;
	u32 version = sWorldManager.m_version;
	u32 min_version = sWorldManager.m_min_version;
	if (pkt->data.data == version)
	{
		pkt::res_version_ok res;
		u8 mask = (u8)rand();
		res.data.data = mask;
		m_client->sendPacket(res);
		return;
	}
	if (pkt->data.data < min_version)
	{
		pkt::res_version_refused res;
		m_client->sendPacket(res, Session::kClose);
		return ;
	}
	if (pkt->data.data > version)
	{
		m_client->close();
		return ;
	}
	pkt::res_version_update res;
	res.data.version = version;
	res.data.file_size = sWorldManager.m_version_file_size;
	m_client->sendPacket(res);
	auto &vfd = sWorldManager.m_version_file_data;
	for (auto &i : vfd)
	{
		pkt::res_file_data pkt;
		pkt.data.data = i;
		m_client->sendPacket(pkt);
	}
	pkt::res_announcementId anPkt;
		// App::instance().announcement(pkt.data);
	sWorldManager.announcement(anPkt.data.data,NULL);
	 m_client->sendPacket(anPkt);
}


//异步请求登录
VN_IMPL_PACKET_REQUEST2(PreLoggingUser,pkt,req_login)
{
	return;
	//查询用户并且获取用户信息
	UUID uuid = {pkt->data.low, pkt->data.high};
	VN_LOG_DEBUG(uuid.to_string()<<"login");
	std::string strUUId = uuid.to_string();
	//todo 数据库查询(异步查询)
	DC_DataBase *pDataBase=(DC_DataBase *)&DataCenter::instance();
	pDataBase->AsyncLoadBaseInfo(strUUId,{this,true},[this,uuid](PreparedQueryResult result)
	{

		//用户不存在
		 if (!(result && result->GetRowCount()))
		{
			 VN_LOG_DEBUG("uuid:"<<uuid.to_string()<<"not found in database");
			pkt::res_login_failed res;
			m_client->sendPacket(res,Session::kClose);
			return ;
		}
		//存在多个用户
		if(result->GetRowCount()>1)
		{
			VN_LOG_ERROR("Exist ONE MORE USER");
			pkt::res_login_failed res;
			m_client->sendPacket(res,Session::kClose);
			return ;
		}
		LoggedUserPtr user = vnnew LoggedUser(uuid);
		//载入基本信息并得到上次保存到现在的时间间隔
		user->load(result);
		//同步查询hero(todo 改成异步)
		if(!((DC_DataBase *)&DataCenter::instance())->LoadHero(user))
		{
			DbOperFailHandl();
			return;
		}
		//开启并行任务
		AsyncTaskPtr task = vnnew AsyncTask();
		task->Init(10);

		((DC_DataBase *)&DataCenter::instance())->AsyncLoadForceGuide(user,{this,true},std::bind(&PreLoggingUser::_HandlAsyncLoadForceGuide,this, std::placeholders::_1,user,task));
		((DC_DataBase *)&DataCenter::instance())->AsyncLoadItemStore(user,{this,true},std::bind(&PreLoggingUser::_HandlAsyncLoadItemStore,this, std::placeholders::_1,user,task));
		((DC_DataBase *)&DataCenter::instance())->AsyncLoadLevels(user,{this,true},std::bind(&PreLoggingUser::_HandlAsyncLoadLevels,this, std::placeholders::_1,user,task));
		((DC_DataBase *)&DataCenter::instance())->AsyncLoadMines(user,{this,true},std::bind(&PreLoggingUser::_HandlAsyncLoadMines,this, std::placeholders::_1,user,task));
		((DC_DataBase *)&DataCenter::instance())->AsyncLoadMissions(user,{this,true},std::bind(&PreLoggingUser::_HandlAsyncLoadMissions,this, std::placeholders::_1,user,task));
		((DC_DataBase *)&DataCenter::instance())->AsyncLoadMonsterMap(user,{this,true},std::bind(&PreLoggingUser::_HandlAsyncLoadMonsterMap,this, std::placeholders::_1,user,task));
		((DC_DataBase *)&DataCenter::instance())->AsyncLoadPlatformStore(user,{this,true},std::bind(&PreLoggingUser::_HandlAsyncLoadPlatformStore,this, std::placeholders::_1,user,task));
		((DC_DataBase *)&DataCenter::instance())->AsyncLoadTechUpgrade(user,{this,true},std::bind(&PreLoggingUser::_HandlAsyncLoadTechUpgrade,this, std::placeholders::_1,user,task));
		((DC_DataBase *)&DataCenter::instance())->AsyncLoadTowerUpgrade(user,{this,true},std::bind(&PreLoggingUser::_HandlAsyncLoadTowerUpgrade,this, std::placeholders::_1,user,task));
		((DC_DataBase *)&DataCenter::instance())->AsyncLoadLevelGroup(user,{this,true},std::bind(&PreLoggingUser::_HandlAsyncLoadLevelGroup,this, std::placeholders::_1,user,task));
	});
}



//PPId 登录
//异步请求登录
VN_IMPL_PACKET_REQUEST2(PreLoggingUser,pkt,req_login_PPID)
{
	vn::RefCountedPtr<pkt::req_login_PPID> 	pktPtr(pkt,true);
	VN_LOG_DEBUG("uuid:"<<pktPtr->data.ppid<<"req_login_PPID");
	LoggedUserPtr user = LoggedUserManager::instance().findUser(pktPtr->data.ppid);
	//重复登录
	if(user.get())
	{
		VN_LOG_DEBUG("Other Login");
		if(!user->queue())
		{
			VN_LOG_ERROR("user:"<<user->GetUUId()<<"queue is empty");
			return;
		}
		ClientPtr client = m_client;
		client->setPacketHandler(nullptr);
		m_client = nullptr;
		user->queue()->runLoop().post(std::bind(&LoggedUser::login, user, client));
		queue()->detach({this, true});
		return;
	}
	VN_LOG_DEBUG("ppid:"<<pktPtr->data.ppid);
	PPLinkManger::CallbackFn callback = [this,pktPtr](vn::s32 result,std::string& context)
		{
			VN_LOG_DEBUG("result["<<result<<"]  context:"<<context);
			//解析context
			s32 nResultCode=0;
			vn::Variable_int32 type;
			std::string strKey = "ret";
			if(!ParseJson(&nResultCode,vn::kVT_int32,type,context,strKey))
			{
				VN_LOG_ERROR("Parase FAILED");
				return ;
			}
			VN_LOG_DEBUG("nResultCode:"<<nResultCode);
			//登录失败
			if(nResultCode < 0)
			{
				 VN_LOG_DEBUG("ppid:"<<pktPtr->data.ppid<<"Login failed"<<"nResultCode:"<<nResultCode);
				pkt::res_login_failed res;
				m_client->sendPacket(res,Session::kClose);
				return;
			}
			//登录成功查询余额
			VN_LOG_DEBUG("uuid:"<<pktPtr->data.ppid<<"login success");
			PPLinkManger::CallbackFn getBalCallFun = std::bind(&PreLoggingUser::_HandlGetBalance,this,std::placeholders::_1,std::placeholders::_2,pktPtr);
			//查询余额
			sPPLinkMgr.SendGetBalance({this,true},std::move(getBalCallFun),
					(char*)pktPtr->data.ppid.c_str());
		};
		//FriendsvrHandler.cpp ::req_Verify
		sPPLinkMgr.SendVerifyReq({this,true},std::move(callback),
				time(NULL),
				"test",
				"test",
				(char*)pkt->data.ppid.c_str(),
				(char*)pkt->data.accessToken.c_str()
		);
}
//DealsvrHandler.cpp::doPlayerGetBalance
void PreLoggingUser::_HandlGetBalance(vn::s32 result,std::string& context,vn::RefCountedPtr<pkt::req_login_PPID> 	pktPtr)
{
	//解析context
	s32 nResultCode=0;
	vn::Variable_int32 type;
	std::string strKey = "ret";
	if(!ParseJson(&nResultCode,vn::kVT_int32,type,context,strKey))
	{
		VN_LOG_ERROR("Parase FAILED");
		return ;
	}
	VN_LOG_DEBUG("nResultCode:"<<nResultCode);
	if( nResultCode <0 )
	{
		VN_LOG_ERROR("GetBalance FAILED");
		return ;
	}
	//获取金币
	s32 nBalance=0;
	//vn::Variable_int32 type;
	 strKey = "balance";
	if(!ParseJson(&nBalance,vn::kVT_int32,type,context,strKey))
	{
		VN_LOG_ERROR("Parase FAILED");
		return ;
	}
	VN_LOG_DEBUG("uuid:"<<pktPtr->data.ppid<<"balance is :"<<nBalance);

	//todo 数据库查询(异步查询)
	DC_DataBase *pDataBase=(DC_DataBase *)&DataCenter::instance();
	//
	pDataBase->AsyncLoadBaseInfo(pktPtr->data.ppid,{this,true},[this,pktPtr,nBalance](PreparedQueryResult result)
	{
		VN_LOG_DEBUG("uuid:"<<pktPtr->data.ppid<<"AsyncLoadBaseInfo success");
		//用户不存在
		 if (!(result && result->GetRowCount()))
		{
			 VN_LOG_DEBUG("start signin user");
			 //注册
			 _HandlSignIn(pktPtr->data.ppid,nBalance);
			return ;
		}
		//存在多个用户
		if(result->GetRowCount()>1)
		{
			VN_LOG_ERROR("Exist ONE MORE USER");
			pkt::res_login_failed res;
			m_client->sendPacket(res,Session::kClose);
			return ;
		}
		LoggedUserPtr user = vnnew LoggedUser(pktPtr->data.ppid);
		//载入基本信息并得到上次保存到现在的时间间隔
		user->load(result,nBalance);
		//同步查询hero(todo 改成异步)
		if(!((DC_DataBase *)&DataCenter::instance())->LoadHero(user))
		{
			 VN_LOG_DEBUG("load hero failed");
			DbOperFailHandl();
			return;
		}
		 VN_LOG_DEBUG("start Load other tables");
		//开启并行任务
		AsyncTaskPtr task = vnnew AsyncTask();
		task->Init(10);
		((DC_DataBase *)&DataCenter::instance())->AsyncLoadForceGuide(user,{this,true},std::bind(&PreLoggingUser::_HandlAsyncLoadForceGuide,this, std::placeholders::_1,user,task));
		((DC_DataBase *)&DataCenter::instance())->AsyncLoadItemStore(user,{this,true},std::bind(&PreLoggingUser::_HandlAsyncLoadItemStore,this, std::placeholders::_1,user,task));
		((DC_DataBase *)&DataCenter::instance())->AsyncLoadLevels(user,{this,true},std::bind(&PreLoggingUser::_HandlAsyncLoadLevels,this, std::placeholders::_1,user,task));
		((DC_DataBase *)&DataCenter::instance())->AsyncLoadMines(user,{this,true},std::bind(&PreLoggingUser::_HandlAsyncLoadMines,this, std::placeholders::_1,user,task));
		((DC_DataBase *)&DataCenter::instance())->AsyncLoadMissions(user,{this,true},std::bind(&PreLoggingUser::_HandlAsyncLoadMissions,this, std::placeholders::_1,user,task));
		((DC_DataBase *)&DataCenter::instance())->AsyncLoadMonsterMap(user,{this,true},std::bind(&PreLoggingUser::_HandlAsyncLoadMonsterMap,this, std::placeholders::_1,user,task));
		((DC_DataBase *)&DataCenter::instance())->AsyncLoadPlatformStore(user,{this,true},std::bind(&PreLoggingUser::_HandlAsyncLoadPlatformStore,this, std::placeholders::_1,user,task));
		((DC_DataBase *)&DataCenter::instance())->AsyncLoadTechUpgrade(user,{this,true},std::bind(&PreLoggingUser::_HandlAsyncLoadTechUpgrade,this, std::placeholders::_1,user,task));
		((DC_DataBase *)&DataCenter::instance())->AsyncLoadTowerUpgrade(user,{this,true},std::bind(&PreLoggingUser::_HandlAsyncLoadTowerUpgrade,this, std::placeholders::_1,user,task));
		((DC_DataBase *)&DataCenter::instance())->AsyncLoadLevelGroup(user,{this,true},std::bind(&PreLoggingUser::_HandlAsyncLoadLevelGroup,this, std::placeholders::_1,user,task));
	});
}



void PreLoggingUser::_HandlTaskFinish(AsyncTaskPtr task,LoggedUserPtr user)
{
	//判断任务是否完成
	int size = task->TaskSize();
	if(task->IsFinished())
	{
		VN_LOG_DEBUG("user:"<<user->GetUUId()<<"loadAllFinished");
		bool result = true;
		for(int i=0;i<size;++i)
		{
			if(task->at(i) == 0)
			{
				result = false;
				break;
			}
		}
		if(!result)
		{
			DbOperFailHandl();
			return;
		}
		else
		{
			//vn::u64 nowTime = time(NULL);
			//vn::u64 intrusionTimeInterval=nowTime-user->bsIntrusion().m_lastIntrusionTimeStamp;
			//计算入侵并保存
			_AsyncCalculateIntrusion(user,[this,user](bool ret)
			{
				if(!ret)
				{
					VN_LOG_ERROR("_AsyncCalculateIntrusion FAILED");
					DbOperFailHandl();
					return;
				}
				 //计算金矿产量并保存
				_AsyncCalculateMines(user,[this,user](bool ret)
				{
					if(!ret)
					{
						VN_LOG_ERROR("_AsyncCalculateMines FAILED");
						DbOperFailHandl();
						return;
					}
					//计算
					user->CalculateCoupon(user->m_lastCouponTimeStamp);
					//计算抽奖
					user->CalculateLottery(user->m_lastLotteryTimeStamp,user->m_lastLoginTimeStamp);
					//保存timer
					((DC_DataBase *)&DataCenter::instance())->AsyncUpdateLoginInfo({this,true},user,[this,user](bool ret){
							if(!ret)
							{
								VN_LOG_ERROR("AsyncUpdateTimer FAILED");
								DbOperFailHandl();
								return;
							}
							//test
							optd::WorkQueue * wq= NULL;
							if(!user->queue() )
							{
								wq = WorkQueueManager::instance().selectWorkQueue();
								wq->attach(user);
							}
							else
							{
								wq = user->queue();
							}
							if(!wq)
							{
								VN_LOG_ERROR("NoWorkQueue");
								return ;
							}
							LoggedUserManager::instance().addUser(user);
							ClientPtr client = m_client;
							client->setPacketHandler(nullptr);
							m_client = nullptr;
							wq->runLoop().post(std::bind(&LoggedUser::login, user, client));
							queue()->detach({this, true});
					});
				});
			});
		}
	}
}


//异步计算入侵数据并保存数据库
void 	PreLoggingUser::_AsyncCalculateIntrusion(LoggedUserPtr user,std::function<void(bool ret)> doResult)
{
	//不用计算了
	if(!user->bsIntrusion().Getrunning())
	{
		return	doResult(true);
	}
	vn::u64 curTimeStamp =time(NULL);
	vn::u64 timeInterval = curTimeStamp - user->bsIntrusion().m_lastIntrusionTimeStamp;
	//上次的时间
	f32 lastTimer = user->bsIntrusion().GetTimer();
	u32 instrusionInterval =sWorldManager.intrusion_interval();
	f32 totolTime = instrusionInterval-lastTimer+timeInterval;
	//计算能入侵多少次
	int  instrTimes =totolTime/instrusionInterval;
	//计算剩下的timer（要写入数据库）
	//f32		newTimer = instrusionInterval-(totolTime%instrusionInterval);
	f32		newTimer = instrusionInterval-(totolTime-instrTimes*instrusionInterval);
	//写入数据库
	VN_LOG_DEBUG("newTimer"<<newTimer);
	for(int i=instrTimes;i>0;--i)
	{
		//不用入侵
		if(!user->bsIntrusion().Getrunning())
		{
			continue;
		}
		else
		{
			if(!user->bsIntrusion().Sync_intrusion())
			{
				return doResult(false);
			}
		}
	}
	//保存
	user->bsIntrusion().Asyncsave({this,true},[doResult](bool ret)
	{
		if(!ret)
		{
			VN_LOG_ERROR("Intrusion update failed");
			return	doResult(false);
		}
		VN_LOG_DEBUG("intrusion calculate success");
		return doResult(true);
	});
}



//异步计算金矿数据并保存
void 	PreLoggingUser::_AsyncCalculateMines(LoggedUserPtr user,std::function<void(bool ret)> doResult)
{
	std::string strUUId =user->GetUUId();
	int taskNum =user->bsGoldMine().mines().size();
	AsyncTaskPtr mineTask = vnnew AsyncTask();
	mineTask->Init(taskNum);
	vn::u64 nowTime = time(NULL);
	 for (auto &i : user->bsGoldMine().mines())
	{
		//计算产量
		 u32 mindId = i.second.id;
		 u32 newNum = i.second.nowNum;
		 f32 newTimer = i.second.timer;
		 i.second.calcYield();
		 VN_LOG_DEBUG("UUID:"<<strUUId<<"minesid"<<i.second.id<<"yield"<<i.second.yield);
		 if(i.second.yield > 0.f)
		 {
			f32 growTime = 86400.f / i.second.yield;
			vn::u64 timeInterval = nowTime - i.second.lastTimestamp;
			u32 addNum =timeInterval/growTime;
			 newTimer = timeInterval - addNum*growTime;
			 newNum = i.second.nowNum+addNum;
			if(newNum >= i.second.maxNum)
			{
				newNum =  i.second.maxNum;
				newTimer = 0;
			}
			i.second.nowNum = newNum;
			i.second.timer = newTimer;
			((DC_DataBase *)&DataCenter::instance())->AsyncUpdateGoldMine(strUUId,mindId,newNum,newTimer,nowTime,{this,true},[this,mineTask,doResult,strUUId,mindId](bool ret)
			{
				if(!ret)
				{
					VN_LOG_ERROR("uuid:"<<strUUId<<"minId"<<mindId<<"calculate and save failed");
				}
				 mineTask->FinishTask(ret);
				 _HandlmineTask(mineTask,doResult);
			});
		 }
		 else
		 {
			 //不需要保存
			 mineTask->FinishTask(true);
			 _HandlmineTask(mineTask,doResult);
		 }
	}
}


void	PreLoggingUser::_HandlmineTask(AsyncTaskPtr mineTask,std::function<void(bool ret)> doResult)
{
	if(!mineTask->IsFinished())
	{
		return;
	}
	bool ret = true;
	for(int i=0;i<mineTask->TaskSize();++i)
	{
		if(!mineTask->at(i))
		{
			ret = false;
			break;
		}
	}
	VN_LOG_DEBUG("mines calculate success");
	return	doResult(ret);
}







void PreLoggingUser::_HandlAsyncLoadItemStore(PreparedQueryResult result,LoggedUserPtr user,AsyncTaskPtr task)
{
	int taskRet=1;
	int rowCount=result->GetRowCount();
	if(!(result && rowCount))
	{
		VN_LOG_ERROR("AsyncLoadItemStore FAILED");
		taskRet=0;
	}
	if(rowCount)
	{
		user->bsItemStore().load(result);
	}
	task->FinishTask(taskRet);
	VN_LOG_DEBUG("user:"<<user->GetUUId()<<"AsyncLoadItemStore Finished");
	_HandlTaskFinish(task,user);
}
void PreLoggingUser::_HandlAsyncLoadForceGuide(PreparedQueryResult result,LoggedUserPtr user,AsyncTaskPtr task)
{
	int taskRet=1;
	if(!result)
	{
		VN_LOG_ERROR("AsyncLoadForceGuide FAILED");
		taskRet = 0;
	}
	if(result->GetRowCount())
	{
		user->bsForceGuide().load(result);
	}
	task->FinishTask(taskRet);
	VN_LOG_DEBUG("user:"<<user->GetUUId()<<"AsyncLoadForceGuide Finished");
	_HandlTaskFinish(task,user);
}


void PreLoggingUser::_HandlAsyncLoadLevels(PreparedQueryResult result,LoggedUserPtr user,AsyncTaskPtr task)
{
	int taskRet=1;
	if(!result)
	{
		VN_LOG_ERROR("AsyncAsyncLoadLevels FAILED");
		taskRet = 0;
	}
	if(result->GetRowCount())
	{
		user->bsLevel().load(result);
	}
	task->FinishTask(taskRet);
	VN_LOG_DEBUG("user:"<<user->GetUUId()<<"AsyncLoadLevels Finished");
	_HandlTaskFinish(task,user);
}

void PreLoggingUser::_HandlAsyncLoadMines(PreparedQueryResult result,LoggedUserPtr user,AsyncTaskPtr task)
{
	int taskRet=1;
	int rowCount=result->GetRowCount();
	if(!(result && rowCount))
	{
		VN_LOG_ERROR("AsynLoadMines FAILED");
		taskRet=0;
	}
	if(rowCount)
	{
		user->bsGoldMine().load(result);
	}
	task->FinishTask(taskRet);
	VN_LOG_DEBUG("user:"<<user->GetUUId()<<"AsyncLoadMines Finished");
	_HandlTaskFinish(task,user);
}


void PreLoggingUser::_HandlAsyncLoadMissions(PreparedQueryResult result,LoggedUserPtr user,AsyncTaskPtr task)
{
	int taskRet=1;
	int rowCount=result->GetRowCount();
	if(!(result && rowCount))
	{
		VN_LOG_ERROR("AsynLoadMission FAILED");
		taskRet=0;
	}
	if(rowCount)
	{
		user->bsMission().load(result);
	}
	task->FinishTask(taskRet);
	VN_LOG_DEBUG("user:"<<user->GetUUId()<<"AsyncLoadMissions Finished");
	_HandlTaskFinish(task,user);
}
void PreLoggingUser::_HandlAsyncLoadMonsterMap(PreparedQueryResult result,LoggedUserPtr user,AsyncTaskPtr task)
{
	int taskRet=1;
	int rowCount=result->GetRowCount();
	if(!(result && rowCount))
	{
		VN_LOG_ERROR("AsynLoadMonsterMap FAILED");
		taskRet=0;
	}
	if(rowCount)
	{
		user->bsMonsterMap().load(result);
	}
	task->FinishTask(taskRet);
	VN_LOG_DEBUG("user:"<<user->GetUUId()<<"AsyncLoadMonsterMap Finished");
	_HandlTaskFinish(task,user);
}
void PreLoggingUser::_HandlAsyncLoadPlatformStore(PreparedQueryResult result,LoggedUserPtr user,AsyncTaskPtr task)
{
	int taskRet=1;
	int rowCount=result->GetRowCount();
	if(!(result && rowCount))
	{
		VN_LOG_ERROR("AsynLoadPlatformStore FAILED");
		taskRet=0;
	}
	if(rowCount)
	{
		user->bsPlatformStore().load(result);
	}
	task->FinishTask(taskRet);
	VN_LOG_DEBUG("user:"<<user->GetUUId()<<"AsyncLoadPlatformStore Finished");
	_HandlTaskFinish(task,user);
}
void PreLoggingUser::_HandlAsyncLoadTechUpgrade(PreparedQueryResult result,LoggedUserPtr user,AsyncTaskPtr task)
{
	int taskRet=1;
	int rowCount=result->GetRowCount();
	if(!(result && rowCount))
	{
		VN_LOG_ERROR("AsynLoadTechUpgrade FAILED");
		taskRet=0;
	}
	if(rowCount)
	{
		user->bsTechUpgrade().load(result);
	}
	task->FinishTask(taskRet);
	VN_LOG_DEBUG("user:"<<user->GetUUId()<<"AsyncLoadTechUpgrade Finished");
	_HandlTaskFinish(task,user);
}
void PreLoggingUser::_HandlAsyncLoadTowerUpgrade(PreparedQueryResult result,LoggedUserPtr user,AsyncTaskPtr task)
{
	int taskRet=1;
	int rowCount=result->GetRowCount();
	if(!(result && rowCount))
	{
		VN_LOG_ERROR("AsynLoadTowerUpgrade FAILED");
		taskRet=0;
	}
	if(rowCount)
	{
		user->bsTowerUpgrade().load(result);
	}
	task->FinishTask(taskRet);
	VN_LOG_DEBUG("user:"<<user->GetUUId()<<"AsyncLoadTowerUpgrade Finished");
	_HandlTaskFinish(task,user);
}

void PreLoggingUser::_HandlAsyncLoadLevelGroup(PreparedQueryResult result,LoggedUserPtr user,AsyncTaskPtr task)
{
/*	if(!result->GetRowCount())
	{
		task->FinishTask(true);
		_HandlTaskFinish(task,user);
	}
	else
	{*/
	int taskRet=1;
	if(!result)
	{
		VN_LOG_ERROR("AsyncLoadLevelGroup FAILED");
		taskRet = false;
	}
	user->bsLevelGroupMgr().load(result);
	task->FinishTask(taskRet);
	VN_LOG_DEBUG("user:"<<user->GetUUId()<<"AsyncLoadLevelGroup Finished");
	_HandlTaskFinish(task,user);
}
/*
int g_index=0;
void PreLoggingUser::AsyncTest()
{

	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(ASY_TEST);
	stmt->setInt32(0,g_index);
	PassPortDatabase.AsyncExecute(stmt,{this,true},[&](bool ret)
	{
		if(g_index>=4)
		{
			VN_LOG_DEBUG("LOOP END");
			return;
		}
		g_index++;
		this->AsyncTest();
	});
}

*/





void PreLoggingUser::AsyncInsertbaseinfo(LoggedUserPtr user,std::function<void(bool ret)>doResult)
{
		u64 curTime = time(NULL);
		PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(INSERT_BASEINFO);
		std::string strUUId =user->GetUUId();
		stmt->setString(0,strUUId);
		stmt->setInt32(1,user->bsPlayerBaseInfo().chip(0));
		stmt->setInt32(2,user->bsPlayerBaseInfo().chip(1));
		stmt->setInt32(3,user->bsPlayerBaseInfo().chip(2));
		stmt->setInt32(4,user->bsPlayerBaseInfo().money());
		stmt->setInt32(5,user->bsPlayerBaseInfo().diamond()) ;
		stmt->setUInt32(6,user->GetPurchaseDiamond());
		stmt->setUInt32(7,user->GetStamp());
		stmt->setInt32(8,user->GetCouponId());
		stmt->setFloat(9,user->GetCouponTimer());
		stmt->setUInt64(10,curTime);
		stmt->setInt32(11,user->bsHero().shovel()[0]);
		stmt->setInt32(12,user->bsHero().shovel()[1]);
		stmt->setInt32(13,user->bsHero().skull()[0]);
		stmt->setInt32(14,	user->bsHero().skull()[1]);
		stmt->setInt32(15,user->bsHero().watch()[0]);
		stmt->setInt32(16,user->bsHero().watch()[1]);
		stmt->setBool(17,user->bsIntrusion().Getrunning());
		stmt->setFloat(18,user->bsIntrusion().GetTimer());
		stmt->setUInt64(19,curTime);
		stmt->setInt32(20,user->bsRegistration().m_nums);
		stmt->setString(21,user->bsRegistration().m_firstDate.convertToString())	;
		stmt->setInt32(22,user->bsRegistration().m_group);
		stmt->setString(23,user->bsRegistration().m_lastDate.convertToString());
		stmt->setUInt32(24,user->GetLotteryDrawTimer());
		stmt->setInt32(26,user->GetLotteryState());
		stmt->setFloat(26,user->GetLotteyTimer());
		stmt->setString(27,"");
		stmt->setUInt64(28,curTime);
		stmt->setUInt64(29,curTime);
		PassPortDatabase.AsyncExecute(stmt,{this,true},[strUUId,doResult](bool ret)
		{
			if(!ret)
			{
				VN_LOG_ERROR("database insert  uuid: baseinfo"<<strUUId<<"failed");
			}
			VN_LOG_DEBUG("uuid:"<<strUUId<<"AsyncInsertbaseinfo finished ret:"<<ret);
			return doResult(ret);
		});

}

void PreLoggingUser::AsyncInsertHero(LoggedUserPtr user,std::function<void(bool ret)>doResult)
{
	std::string strUUId =user->GetUUId();
		BSHero bshero = user->bsHero();
		AsyncTaskPtr task = vnnew AsyncTask();
		int taskSize= 0;
		for(auto heroinfo : bshero.heroList())
		{
			taskSize ++;
			taskSize+=heroinfo.runeList.size();
		}
		task->Init(taskSize);
		for(auto heroinfo : bshero.heroList())
		{
			//产生一个Heroid
			UUID uuid;
			uuid.generate();
			std::string strheroid=uuid.to_string();
			PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(INSERT_BASEINFOHERO);
			//插入一个hero
			stmt->setString(0,strUUId);
			stmt->setString(1,strheroid);
			stmt->setUInt32(2,heroinfo.id);
			stmt->setUInt32(3,heroinfo.towerClass);
			stmt->setInt32(4,(s32)heroinfo.status);
			stmt->setUInt32(5,heroinfo.unlockRes);
			PassPortDatabase.AsyncExecute(stmt,{this,true},[strUUId,doResult,task](bool ret)
			{
				//插入hero失败
				if(!ret)
				{
					VN_LOG_ERROR("database insert BASEINFOHERO"<<strUUId<<"failed");
				}
				task->FinishTask(ret);
				if(task->IsFinished())
				{
					bool result = true;
					for(int i=0;i<task->TaskSize();++i)
					{
						if(!task->at(i))
						{
							result = false;
							break;
						}
					}
					VN_LOG_DEBUG("uuid:"<<strUUId<<"AsyncInsertHero finished ret:"<<result);
					return	 doResult(result);
				}
			});
			//在插入hero_runes;
			for(auto rune :heroinfo.runeList)
			{
				stmt=PassPortDatabase.GetPreparedStatement(INSERT_HERORUNES);
				stmt->setString(0,strheroid);
				stmt->setUInt32(1,rune.id);
				stmt->setInt32(2,rune.status);
				PassPortDatabase.AsyncExecute(stmt,{this,true},[strUUId,doResult,task](bool ret)
				{
					//插入hero失败
					if(!ret)
					{
						VN_LOG_ERROR("database insert HERORUNES"<<strUUId<<"failed");
					}
					task->FinishTask(ret);
					if(task->IsFinished())
					{
						bool result = true;
						for(int i=0;i<task->TaskSize();++i)
						{
							if(!task->at(i))
							{
								result = false;
								break;
							}
						}
						VN_LOG_DEBUG("uuid:"<<strUUId<<"AsyncInsertHero finished ret:"<<result);
						return	 doResult(result);
					}
				});
			}
		}
}







void PreLoggingUser::AsyncInsertItemStore(LoggedUserPtr user,std::function<void(bool ret)>doResult)
{
	std::string strUUId = user->GetUUId();
	BSItemStore itemstore = user->bsItemStore();
	AsyncTaskPtr task = vnnew AsyncTask();
	task->Init(itemstore.itemMap().size());
	for(auto &item:itemstore.itemMap())
	{
		PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(INSERT_BASEINFOITEMSTORE);
		uint itemid=item.first;
		uint itemstatus=item.second;
		stmt->setString(0,strUUId);
		stmt->setUInt32(1,itemid);
		stmt->setUInt32(2,itemstatus);
		PassPortDatabase.AsyncExecute(stmt,{this,true},[this,task,doResult,strUUId](bool ret)
		{
			task->FinishTask(ret);
			if(task->IsFinished())
			{
				bool result = true;
				for(int i=0;i<task->TaskSize();++i)
				{
					if(!task->at(i))
					{
						result = false;
						break;
					}
				}
				VN_LOG_DEBUG("uuid:"<<strUUId<<"AsyncInsertItemStore finished ret:"<<result);
				return	 doResult(result);
			}
		});
	}
}

void PreLoggingUser::AsyncInsertLevels(LoggedUserPtr user,std::function<void(bool ret)>doResult)
{
	std::string strUUId = user->GetUUId();
	//BSLevel bslevel = user->bsLevel();
	AsyncTaskPtr task = vnnew AsyncTask();
	task->Init(user->bsLevel().levelList().size());
	for (auto &levelinfo : user->bsLevel().levelList())
	{
		PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(INSERT_BASEINFOLEVELS);
		   s32 levelid=levelinfo.first;
		   bool instrusion=levelinfo.second->intrusion;
		   u32 instrTimes=levelinfo.second->intrusionTimes;
		   s32 scores=levelinfo.second->score;
		   s32 shovel=levelinfo.second->shovel;
		   s32 stars=levelinfo.second->starNum;
		   s32 watch=levelinfo.second->watch;
		   stmt->setString(0,strUUId);
		   stmt->setInt32(1,levelid);
		   stmt->setBool(2,instrusion);
		   stmt->setUInt32(3,instrTimes);
		   stmt->setInt32(4,scores);
		   stmt->setInt32(5,shovel);
		   stmt->setInt32(6,stars);
		   stmt->setInt32(7,watch);
		   PassPortDatabase.AsyncExecute(stmt,{this,true},[this,task,doResult,strUUId](bool ret)
			{
				task->FinishTask(ret);
				if(task->IsFinished())
				{
					bool result = true;
					for(int i=0;i<task->TaskSize();++i)
					{
						if(!task->at(i))
						{
							result = false;
							break;
						}
					}
					VN_LOG_DEBUG("uuid:"<<strUUId<<"AsyncInsertLevels finished ret:"<<result);
					return	 doResult(result);
				}
			});
	}
}

void PreLoggingUser::AsyncInsertMines(LoggedUserPtr user,std::function<void(bool ret)>doResult)
{
	std::string strUUId = user->GetUUId();
	AsyncTaskPtr task = vnnew AsyncTask();
	task->Init(user->bsGoldMine().mines().size());
	for (auto &i :user->bsGoldMine().mines())
	{
		auto &gm = i.second;
		PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(INSERT_BASEINFOMINES);
		stmt->setString(0,strUUId);
		stmt->setUInt32(1,gm.id);
		stmt->setUInt32(2,gm.nowNum);
		stmt->setFloat(3,gm.timer);
		vn::u64 curTime = time(NULL);
		stmt->setUInt64(4,curTime);
		PassPortDatabase.AsyncExecute(stmt,{this,true},[this,task,doResult,strUUId](bool ret)
		{
			task->FinishTask(ret);
			if(task->IsFinished())
			{
				bool result = true;
				for(int i=0;i<task->TaskSize();++i)
				{
					if(!task->at(i))
					{
						result = false;
						break;
					}
				}
				VN_LOG_DEBUG("uuid:"<<strUUId<<"AsyncInsertMines finished ret:"<<result);
				return	 doResult(result);
			}
		});
	}
}




void PreLoggingUser::AsyncInsertMissions(LoggedUserPtr user,std::function<void(bool ret)>doResult)
{
	std::string strUUId = user->GetUUId();
	AsyncTaskPtr task = vnnew AsyncTask();
	task->Init(user->bsMission().list().size());
	 for (auto &i :user->bsMission().list())
	{
		u32 missionid=i.first;
		bool missionstatus=i.second;
		PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(INSERT_BASEINFOMISSION);
		stmt->setString(0,strUUId);
		stmt->setUInt32(1,missionid);
		stmt->setBool(2,missionstatus);
		PassPortDatabase.AsyncExecute(stmt,{this,true},[this,task,doResult,strUUId](bool ret)
		{
			task->FinishTask(ret);
			if(task->IsFinished())
			{
				bool result = true;
					for(int i=0;i<task->TaskSize();++i)
					{
						if(!task->at(i))
						{
							result = false;
							break;
						}
					}
					VN_LOG_DEBUG("uuid:"<<strUUId<<"AsyncInsertMissions finished ret:"<<result);
					return	 doResult(result);

			}
		});
	}
}

void PreLoggingUser::AsyncInsertMonsterMap(LoggedUserPtr user,std::function<void(bool ret)>doResult)
{
	std::string strUUId = user->GetUUId();
	AsyncTaskPtr task = vnnew AsyncTask();
	task->Init(user->bsMonsterMap().monsterMap().size());
	for (auto &i :user->bsMonsterMap().monsterMap())
	{
		u32 monterid=i.first;
		bool monsterstatus=i.second;
		PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(INSERT_BASEINFOMONSTERMAP);
		stmt->setString(0,strUUId);
		stmt->setUInt32(1,monterid);
		stmt->setBool(2,monsterstatus);
		PassPortDatabase.AsyncExecute(stmt,{this,true},[this,task,doResult,strUUId](bool ret)
		{
			task->FinishTask(ret);
			if(task->IsFinished())
			{
				bool result = true;
				for(int i=0;i<task->TaskSize();++i)
				{
					if(!task->at(i))
					{
					result = false;
					break;
					}
				}
				VN_LOG_DEBUG("uuid:"<<strUUId<<"AsyncInsertMonsterMap finished ret:"<<result);
				return	 doResult(result);
			}

		});
	}
}


void PreLoggingUser::AsyncInsertPlatformStore(LoggedUserPtr user,std::function<void(bool ret)>doResult)
{
		std::string strUUId = user->GetUUId();
		auto &levelArray = user->bsPlatformStore().list();
		AsyncTaskPtr task = vnnew AsyncTask();
		task->Init(levelArray.size());
		for (uint i=0;i<levelArray.size();++i)
		{
			PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(INSERT_BASEINFPLATFORMSTORE);
			stmt->setString(0,strUUId);
			stmt->setUInt32(1,i);
			stmt->setUInt32(2,levelArray[i]);
			PassPortDatabase.AsyncExecute(stmt,{this,true},[this,task,doResult,strUUId](bool ret)
			{
				task->FinishTask(ret);
				if(task->IsFinished())
				{
					bool result = true;
					for(int i=0;i<task->TaskSize();++i)
					{
						if(!task->at(i))
						{
							result = false;
							break;
						}
					}
					VN_LOG_DEBUG("uuid:"<<strUUId<<"AsyncInsertPlatformStore finished ret:"<<result);
					return	 doResult(result);
				}
			});
		}
}



void  PreLoggingUser::AsyncInsertTechUpgrade(LoggedUserPtr user,std::function<void(bool ret)>doResult)
{
		std::string strUUId = user->GetUUId();
		auto &techVec=user->bsTechUpgrade().techList();
		AsyncTaskPtr task = vnnew AsyncTask();
		task->Init(techVec.size());
		for (uint i=0;i<techVec.size();++i)
		{
			PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(INSERT_BASEINFOTECHUPGRADE);
			stmt->setString(0,strUUId);
			stmt->setUInt32(1,i);
			stmt->setUInt32(2,techVec[i]);
			PassPortDatabase.AsyncExecute(stmt,{this,true},[this,task,doResult,strUUId](bool ret)
				{

					task->FinishTask(ret);
					if(task->IsFinished())
					{
						bool result = true;
						for(int i=0;i<task->TaskSize();++i)
						{
							if(!task->at(i))
							{
								result = false;
								break;
							}
						}
						VN_LOG_DEBUG("uuid:"<<strUUId<<"AsyncInsertTechUpgrade finished ret:"<<result);
						return	 doResult(result);
					}
				});
		}
}


void PreLoggingUser::AsyncInsertTowerUpgrade(LoggedUserPtr user,std::function<void(bool ret)>doResult)
{
	std::string strUUId= user->GetUUId();
	auto &towerVec=user->bsTowerUpgrade().towerList();
	AsyncTaskPtr task = vnnew AsyncTask();
	int taskSize= 0;
	for (uint i=0;i<towerVec.size();++i)
	{
		taskSize +=towerVec[i].size();
	/*	for(uint j=0;j<towerVec[i].size();++j)
		{
			taskSize++;
		}*/
	}
	task->Init(taskSize);
	for (uint i=0;i<towerVec.size();++i)
	{
		for(uint j=0;j<towerVec[i].size();++j)
		{
			PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(INSERT_BASEINFOTOWERUPGRADE);
			stmt->setString(0,strUUId);
			stmt->setUInt32(1,i);
			stmt->setUInt32(2,j);
			stmt->setUInt32(3,towerVec[i][j]);
			PassPortDatabase.AsyncExecute(stmt,{this,true},[this,task,doResult,strUUId](bool ret)
			{
				task->FinishTask(ret);
				if(task->IsFinished())
				{
					bool result = true;
					for(int i=0;i<task->TaskSize();++i)
					{
						if(!task->at(i))
						{
							result = false;
							break;
						}
					}
					VN_LOG_DEBUG("uuid:"<<strUUId<<"AsyncInsertTowerUpgrade finished ret:"<<result);
					return	 doResult(result);
				}
			});
		}
	}
}








void PreLoggingUser::_HandlAsyncCreateUser(bool ret,LoggedUserPtr user,AsyncTaskPtr createUserTask)
{
	createUserTask->FinishTask(ret);
	int size = createUserTask->TaskSize();
	if(!createUserTask->IsFinished())
	{
		return;
	}
	bool result = true;
	for(int i=0;i<size;++i)
	{
		if(createUserTask->at(i) == 0)
		{
			result = false;
			break;
		}
	}
	if(!result)
	{
		VN_LOG_ERROR("signin user failed");
		return;
	}
	VN_LOG_DEBUG("signin user successful");
	/*user->_markDirty();
	((DC_DataBase *)&DataCenter::instance())->_addUser(user);
	ClientPtr client = m_client;
	client->setPacketHandler(nullptr);
	m_client = nullptr;
	optd::WorkQueue *wq= user->queue();
	wq->runLoop().post(std::bind(&LoggedUser::signin, user, client));
	queue()->detach({this, true});*/
	optd::WorkQueue * wq= NULL;
	if(!user->queue() )
	{
		wq = WorkQueueManager::instance().selectWorkQueue();
		wq->attach(user);
	}
	else
	{
		wq = user->queue();
	}
	if(!wq)
	{
		VN_LOG_ERROR("NoWorkQueue");
		return ;
	}
	LoggedUserManager::instance().addUser(user);
	ClientPtr client = m_client;
	client->setPacketHandler(nullptr);
	m_client = nullptr;
	wq->runLoop().post(std::bind(&LoggedUser::login, user, client));
	queue()->detach({this, true});
}




//异步请求注册用户
/*
VN_IMPL_PACKET_REQUEST2(PreLoggingUser,pkt,req_siginin)
{
	UUID uuid;
	uuid.generate();
	_HandlSignIn(uuid.to_string());
}
*/

void	PreLoggingUser::_HandlSignIn(const std::string &strUUId,vn::s32 diamond)
{
		VN_LOG_DEBUG("SignIn user start"<<strUUId);
		LoggedUserPtr user = vnnew LoggedUser(strUUId);
		//获取插入数据库的初始数据
		user->reset_data(diamond);
		AsyncTaskPtr createUserTask = vnnew AsyncTask();
		createUserTask->Init(9);
		AsyncInsertbaseinfo(user,std::bind(&PreLoggingUser::_HandlAsyncCreateUser,this,std::placeholders::_1,user,createUserTask));
		AsyncInsertHero(user,std::bind(&PreLoggingUser::_HandlAsyncCreateUser,this,std::placeholders::_1,user,createUserTask));
		AsyncInsertItemStore(user,std::bind(&PreLoggingUser::_HandlAsyncCreateUser,this,std::placeholders::_1,user,createUserTask));
		AsyncInsertMines(user,std::bind(&PreLoggingUser::_HandlAsyncCreateUser,this,std::placeholders::_1,user,createUserTask));
		AsyncInsertMissions(user,std::bind(&PreLoggingUser::_HandlAsyncCreateUser,this,std::placeholders::_1,user,createUserTask));
		AsyncInsertMonsterMap(user,std::bind(&PreLoggingUser::_HandlAsyncCreateUser,this,std::placeholders::_1,user,createUserTask));
		AsyncInsertPlatformStore(user,std::bind(&PreLoggingUser::_HandlAsyncCreateUser,this,std::placeholders::_1,user,createUserTask));
		AsyncInsertTechUpgrade(user,std::bind(&PreLoggingUser::_HandlAsyncCreateUser,this,std::placeholders::_1,user,createUserTask));
		AsyncInsertTowerUpgrade(user,std::bind(&PreLoggingUser::_HandlAsyncCreateUser,this,std::placeholders::_1,user,createUserTask));
}




