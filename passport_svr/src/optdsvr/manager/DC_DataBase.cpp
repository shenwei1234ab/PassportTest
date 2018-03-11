#include "vnLog.h"
#include "DC_DataBase.h"
#include "../database/PassPortDatabaseConnection.h"
#include "vnVariableScript.h"
#include "vnVariableAccessor.h"
#include "BSFactory.h"
#include "../Log/DC_DataBase_EventLog.h"
extern PassPortDatabaseWorkerPool PassPortDatabase;

DataCenter * DC_DataBase::create() {
    return vnnew DC_DataBase();
}

void DC_DataBase::destroy(DataCenter *p) {
    delete (DC_DataBase *)p;
}



bool DC_DataBase::init()
{
	//todo...改成异步
	//查询所有的用户基本信息(数据库同步查询)
	/*PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(SEL_BASEINFO);
	//同步查询
	PreparedQueryResult result = PassPortDatabase.SynQuery(stmt);
	//查询失败或者查询结果为0
	if (!(result && result->GetRowCount()))
	{
		VN_LOG_ERROR("LOAD ALL BASE_INFO failed ");
		return false;
	}
	//获取每一行
	do {
			//获取第一列
			std::string strUUid = (*result)[0].GetString();
			VN_LOG_DEBUG("user"<<strUUid<<"load");
			UUID uuid(strUUid);
			if(uuid)
			{
				//填充到内存
				DBTable_baseinfo info;
				info.chip[0] = (*result)[1].GetInt32();
				info.chip[1] = (*result)[2].GetInt32();
				info.chip[2] = (*result)[3].GetInt32();
				info.money = (*result)[4].GetInt32();
				info.diamonds = (*result)[5].GetInt32();
				info.purchasedDiamonds=(*result)[6].GetInt32();
				info.stamp = (*result)[7].GetInt32();
				info.coupon_id = (*result)[8].GetInt32();
				info.coupon_timer=(*result)[9].GetInt32();
				info.shovel0=(*result)[10].GetInt32();
				info.shovel1=(*result)[11].GetInt32();
				info.skull0=(*result)[12].GetInt32();
				info.skull1=(*result)[13].GetInt32();
				info.watch0=(*result)[14].GetInt32();
				info.watch1=(*result)[15].GetInt32();
				info.running=(*result)[16].GetBool();
				info.timer=(*result)[17].GetFloat();
				info.count=(*result)[18].GetInt32();
				info.first_date=(*result)[19].GetString();
				info.group=(*result)[20].GetInt32();
				info.last_date=(*result)[21].GetString();
				info.lottery_draw=(*result)[22].GetInt32();
				info.lottery_state=(*result)[23].GetInt32();
				info.lottery_timer=(*result)[24].GetFloat();
				LoggedUserPtr user = vnnew LoggedUser(uuid);
				user->load(info);
				//查询用户的其他消息
				_LoadForceGuide(user);
				_LoadHero(user);
				 if (user->restart())
				 {
					  _addUser(user);
				 }
			}
		} while (result->NextRow());*/
	//读取当前目录下的server.var
	 m_fs = FileManager::instance().findFileSystem(1001);
	 if (!m_fs)
	 {
	        m_fs = vnnew FileSystemOS();
	  }
	auto fs = m_fs->open("server.var");
		if (fs) {
			size_t size = (size_t)fs->size();
			if (size) {
				c8 *buf = vnmalloc(c8, size);
				size = fs->read(buf, size);
				if (size) {
					VariableScript vs;
					auto obj = vs.parse(buf, size, nullptr);
					if (obj) {
						m_serverData.load(*obj);
					}
				}
				vnfree(buf);
			}
			fs->close();
		}
		_init_redeem_packets();
		return true;
}


void  _AsyncInsertbaseinfo(std::string &strUUid,AsyncObjectPtr obj,std::function<void(bool ret)>doResult)
{
	 	 DBTable_baseinfo info;
	    info.Init();
	    PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(INSERT_BASEINFO);
	    stmt->setString(0,strUUid);
	    stmt->setInt32(1,info.chip[0]);
	    stmt->setInt32(2,info.chip[1]);
	    stmt->setInt32(3,info.chip[2]);
	    stmt->setInt32(4,info.money);
	    stmt->setInt32(5,info.diamonds) ;
	    stmt->setUInt32(6,info.purchasedDiamonds);
	    stmt->setUInt32(7,info.stamp);
	    stmt->setInt32(	8,info.coupon_id);
	    stmt->setFloat(9,info.coupon_timer);
	    stmt->setInt32(10,info.shovel0);
	    stmt->setInt32(11,info.shovel1);
	    stmt->setInt32(12,info.skull0);
	    stmt->setInt32(13,	info.skull1);
	    stmt->setInt32(14,info.watch0);
	    stmt->setInt32(15,info.watch1);
	    stmt->setBool(16,info.running);
	    stmt->setFloat(17,info.timer);
	    stmt->setInt32(18,info.count);
	    stmt->setString(19,info.first_date)	;
	    stmt->setInt32(20,info.group);
	    stmt->setString(21,info.last_date);
	    stmt->setInt32(22,info.lottery_draw);
	    stmt->setInt32(23,info.lottery_state);
	    stmt->setFloat(24,info.lottery_timer);
	  PassPortDatabase.AsyncExecute(stmt,obj,doResult);
	/*    if(!PassPortDatabase.SynExecute(stmt))
	    {
	    	VN_LOG_ERROR("database insert "<<strUUid<<"failed");
	    	return false;
	    }
	    return true;*/
}


void DC_DataBase::_init_redeem_packets() {
    auto fs = m_fs->open("redeem_packets.var");
    if (fs) {
        size_t size = (size_t)fs->size();
        if (size) {
            c8 *buf = vnmalloc(c8, size);
            size = fs->read(buf, size);
            if (size) {
                VariableScript vs;
                auto obj = vs.parse(buf, size, nullptr);
                if (obj) {
                    VariableAccessor<Variable_object> va(obj->queryAttributeObject("packets"));
                    while (va.fetch()) {
                        auto i = va.at<0>();
                        u32 codeId = i->queryAttributeInt32("id");
                        if (codeId) {
                            auto p = vnnew RedeemPacket();
                            p->codeId = i->queryAttributeInt32("id");
                            p->category = i->queryAttributeInt32("category");
                            p->money = i->queryAttributeInt32("money");
                            p->diamonds = i->queryAttributeInt32("diamonds");
                            m_redeemPackets[p->codeId] = p;
                        }
                    }

                    VariableAccessor<Variable_int32, Variable_string> vb(obj->queryAttributeObject("categories"));
                    while (vb.fetch()) {
                        m_redeemPacketCategories[vb.at<0>()->getInt32()] = vb.at<1>()->value();
                    }
                }
            }
            vnfree(buf);
        }
        fs->close();
    }
}

void DC_DataBase::_flush_redeem_packets() {
    auto fs = m_fs->create("redeem_packets.var");
    if (fs) {
        Variable_object obj;
        RefCountedPtr<Variable_object> packets = vnnew Variable_object();
        m_redeemPacketPacketsLock.lock();
        for (auto &i : m_redeemPackets) {
            RefCountedPtr<Variable_object> p = vnnew Variable_object();
            p->setAttribute("id", vnnew Variable_int32(i.second->codeId));
            p->setAttribute("category", vnnew Variable_int32(i.second->category));
            p->setAttribute("money", vnnew Variable_int32(i.second->money));
            p->setAttribute("diamonds", vnnew Variable_int32(i.second->diamonds));
            packets->add(std::move(p));
        }
        m_redeemPacketPacketsLock.unlock();
        obj.setAttribute("packets", std::move(packets));

        RefCountedPtr<Variable_object> categories = vnnew Variable_object();
        m_redeemPacketCategoriesLock.lock();
        for (auto &i : m_redeemPacketCategories) {
            categories->add(vnnew Variable_int32(i.first));
            categories->add(vnnew Variable_string(i.second));
        }
        m_redeemPacketCategoriesLock.unlock();
        obj.setAttribute("categories", std::move(categories));
        auto txt = VariableScript::generate(&obj);
        fs->write(txt.c_str(), txt.length());
        fs->close();
    }
}



////////////////////////////////////////////////////ForceGuide
bool DC_DataBase::_InsertForceGuide(LoggedUserPtr user)
{
	std::string strUUId = user->GetUUId();
	BSForceGuide forceguide = user->bsForceGuide();
	auto & idSet=forceguide.disableList();
	if(idSet.size()<=0)
	{
	 //不用插入
	 return true;
	}
	for(auto &guideid : idSet)
	{
	//插入forceguide
	PreparedStatement* stmt=PassPortDatabase.GetPreparedStatement(INSERT_BASEINFOFORCEGUIDE);
	stmt->setString(0,strUUId);
	stmt->setUInt32(1,guideid);
	if(!PassPortDatabase.SynExecute(stmt))
	{
		VN_LOG_ERROR("database insert baseinforceguide"<<strUUId<<"failed");
		return false;
	}
	}
	return true;
}









void  DC_DataBase::AsyncLoadForceGuide(LoggedUserPtr user,vn::RefCountedPtr<AsyncObject> asyncObject,std::function<void (PreparedQueryResult )> doResult)
{
	std::string stdUUid=user->GetUUId();
	//异步查询用户基本信息 todo
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(SEL_BASEINFOFORCEGUIDE);
	 stmt->setString(0,stdUUid);
	PassPortDatabase.AsyncQuery(stmt, asyncObject,doResult);
}






bool DC_DataBase::_LoadForceGuide(LoggedUserPtr user)
{
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(SEL_BASEINFOFORCEGUIDE);
	std::string strUUid = user->GetUUId();
	stmt->setString(0,strUUid);
	//同步查询
	PreparedQueryResult result = PassPortDatabase.SynQuery(stmt);
	if (!(result && result->GetRowCount()))
	{
		VN_LOG_DEBUG("LOAD "<<strUUid<<"HERO_BASEINFOHERO failed");
		return true;
	}
	//获取每一行
	user->bsForceGuide().load(result);
	return true;
}




/////////////////////////////////////Hero
bool DC_DataBase::_InsertHero(LoggedUserPtr user)
{
	std::string strUUId =user->GetUUId();
	BSHero bshero = user->bsHero();
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
		if(!PassPortDatabase.SynExecute(stmt))
		{
			//插入hero失败
			VN_LOG_ERROR("database insert BASEINFOHERO"<<strUUId<<"failed");
			return false;
		}
		//在插入hero_runes;
		for(auto rune :heroinfo.runeList)
		{
			stmt=PassPortDatabase.GetPreparedStatement(INSERT_HERORUNES);
			stmt->setString(0,strheroid);
			stmt->setUInt32(1,rune.id);
			stmt->setInt32(2,rune.status);
			if(!PassPortDatabase.SynExecute(stmt))
			{
				VN_LOG_ERROR("database insert HERORUNES"<<strUUId<<"failed");
				return false;
			}
		}
	}
	return true;
}






void DC_DataBase::Async_InsertHero(AsyncObjectPtr obj,LoggedUserPtr user,std::function<void(bool ret)>doResult)
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
		PassPortDatabase.AsyncExecute(stmt,obj,[strUUId,doResult,task](bool ret)
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
			PassPortDatabase.AsyncExecute(stmt,obj,[strUUId,doResult,task](bool ret)
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
					return	 doResult(result);
				}
			});
		}
	}
}











bool DC_DataBase::_LoadHero(std::string &strUUId,BSHero &hero)
{
		PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(SEL_BASEINFOHERO);
		stmt->setString(0,strUUId);
		//同步查询
		PreparedQueryResult heroResult = PassPortDatabase.SynQuery(stmt);
		if (!(heroResult && heroResult->GetRowCount()))
		{
			VN_LOG_ERROR("LOAD HERO_BASEINFOHERO failed ");
			return false;
		}
		do
		{
			BSHero::HeroInfo info;
			//获取heroid 主键
			std::string heroid=(*heroResult)[0].GetString();
			info.id =(*heroResult)[1].GetUInt32();
			info.towerClass =(*heroResult)[2].GetUInt32();
			info.status =(BSHero::Status)(*heroResult)[3].GetInt32();
			info.unlockRes=(*heroResult)[4].GetUInt32();
			//查询heroid对应的runes
			stmt = PassPortDatabase.GetPreparedStatement(SEL_HERORUNES);
			stmt->setString(0,heroid);
			PreparedQueryResult result = PassPortDatabase.SynQuery(stmt);
			if  (!(result && result->GetRowCount()))
			{
				VN_LOG_ERROR("LOAD HERORUNES failed ");
				return false;
			}
			do
			{
				BSHero::Rune rune;
				rune.id= (*result)[0].GetUInt32();
				rune.status = (BSHero::Status)(*result)[1].GetInt32();
				info.runeList.push_back(rune);
			}while(result->NextRow());
			hero.load(info);
		}while(heroResult->NextRow());
		//user->load(dbhero);
		return true;
}


//////////////////////////////////////////////////////ItemStore

bool DC_DataBase::_InsertItemStore(LoggedUserPtr user)
{
	std::string strUUId = user->GetUUId();
	BSItemStore itemstore = user->bsItemStore();
	for(auto &item:itemstore.itemMap())
	{
		PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(INSERT_BASEINFOITEMSTORE);
		uint itemid=item.first;
		uint itemstatus=item.second;
		stmt->setString(0,strUUId);
		stmt->setUInt32(1,itemid);
		stmt->setUInt32(2,itemstatus);
		if(!PassPortDatabase.SynExecute(stmt))
		{
			//插入hero失败
			VN_LOG_ERROR(strUUId<<":InsertItemStore "<<"failed");
			return false;
		}
	}
	return true;
}

void DC_DataBase::Async_InsertItemStore(AsyncObjectPtr obj,LoggedUserPtr user,std::function<void(bool ret)>doResult)
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
		PassPortDatabase.AsyncExecute(stmt,obj,[this,task,doResult](bool ret)
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
				return	 doResult(result);
			}

		});
	}
}












bool DC_DataBase::_LoadItemStore(std::string &strUUId,BSItemStore &itemStore)
{
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(SEL_BASEINFOITEMSTORE);
	stmt->setString(0,strUUId);
	//同步查询
	PreparedQueryResult result = PassPortDatabase.SynQuery(stmt);
	//查不到指定UUid对应的hero数据
	if (!(result && result->GetRowCount()))
	{
		VN_LOG_ERROR("_LoadItemStore failed ");
		return false;
	}
	itemStore.load(result);
	return true;
}


void  DC_DataBase::AsyncLoadItemStore(LoggedUserPtr user,vn::RefCountedPtr<AsyncObject> asyncObject,std::function<void (PreparedQueryResult )> doResult)
{
	std::string stdUUid=user->GetUUId();
	//异步查询用户基本信息 todo
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(SEL_BASEINFOITEMSTORE);
	 stmt->setString(0,stdUUid);
	PassPortDatabase.AsyncQuery(stmt, asyncObject,doResult);
}



void  DC_DataBase::AsyncLoadLevels(LoggedUserPtr user,vn::RefCountedPtr<AsyncObject> asyncObject,std::function<void (PreparedQueryResult )> doResult)
{
	std::string stdUUid=user->GetUUId();
	//异步查询用户基本信息 todo
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(SEL_BASEINFOLEVELS);
	 stmt->setString(0,stdUUid);
	PassPortDatabase.AsyncQuery(stmt, asyncObject,doResult);
}


void  DC_DataBase::AsyncLoadMines(LoggedUserPtr user,vn::RefCountedPtr<AsyncObject> asyncObject,std::function<void (PreparedQueryResult )> doResult)
{
	//
	std::string stdUUid=user->GetUUId();
	//异步查询用户基本信息 todo
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(SEL_BASEINFOMINES);
	 stmt->setString(0,stdUUid);
	PassPortDatabase.AsyncQuery(stmt, asyncObject,doResult);
}


void  DC_DataBase::AsyncLoadMissions(LoggedUserPtr user,vn::RefCountedPtr<AsyncObject> asyncObject,std::function<void (PreparedQueryResult )> doResult)
{
	std::string stdUUid=user->GetUUId();
	//异步查询用户基本信息 todo
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(SEL_BASEINFOMISSION);
	 stmt->setString(0,stdUUid);
	PassPortDatabase.AsyncQuery(stmt, asyncObject,doResult);
}


void  DC_DataBase::AsyncLoadMonsterMap(LoggedUserPtr user,vn::RefCountedPtr<AsyncObject> asyncObject,std::function<void (PreparedQueryResult )> doResult)
{
	std::string stdUUid=user->GetUUId();
	//异步查询用户基本信息 todo
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(SEL_BASEINFOMONSTERMAP);
	 stmt->setString(0,stdUUid);
	PassPortDatabase.AsyncQuery(stmt, asyncObject,doResult);
}


void  DC_DataBase::AsyncLoadPlatformStore(LoggedUserPtr user,vn::RefCountedPtr<AsyncObject> asyncObject,std::function<void (PreparedQueryResult )> doResult)
{
	std::string stdUUid=user->GetUUId();
	//异步查询用户基本信息 todo
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(SEL_BASEINFPLATFORMSTORE);
	 stmt->setString(0,stdUUid);
	PassPortDatabase.AsyncQuery(stmt, asyncObject,doResult);
}

void  DC_DataBase::AsyncLoadTechUpgrade(LoggedUserPtr user,vn::RefCountedPtr<AsyncObject> asyncObject,std::function<void (PreparedQueryResult )> doResult)
{
	std::string stdUUid=user->GetUUId();
	//异步查询用户基本信息 todo
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(SEL_BASEINFOTECHUPGRADE);
	 stmt->setString(0,stdUUid);
	PassPortDatabase.AsyncQuery(stmt, asyncObject,doResult);
}


void  DC_DataBase::AsyncLoadTowerUpgrade(LoggedUserPtr user,vn::RefCountedPtr<AsyncObject> asyncObject,std::function<void (PreparedQueryResult )> doResult)
{
	std::string stdUUid=user->GetUUId();
	//异步查询用户基本信息 todo
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(SEL_BASEINFOTOWERUPGRADE);
	 stmt->setString(0,stdUUid);
	PassPortDatabase.AsyncQuery(stmt, asyncObject,doResult);
}


void  DC_DataBase::AsyncLoadLevelGroup(LoggedUserPtr user,vn::RefCountedPtr<AsyncObject> asyncObject,std::function<void (PreparedQueryResult )> doResult)
{
	std::string stdUUid=user->GetUUId();
	//异步查询用户基本信息 todo
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(SEL_BASEINFOLEVELGROUPS);
	 stmt->setString(0,stdUUid);
	PassPortDatabase.AsyncQuery(stmt, asyncObject,doResult);
}

void  DC_DataBase::AsyncLoadLevelGroupLevels(vn::u32 levelId,vn::RefCountedPtr<AsyncObject> asyncObject,std::function<void (PreparedQueryResult )> doResult)
{
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(SEL_LEVELGROUPSLEVELS);
	 stmt->setUInt32(0,levelId);
	PassPortDatabase.AsyncQuery(stmt, asyncObject,doResult);
}

PreparedQueryResult  DC_DataBase::LoadLevelGroupLevels(vn::u32 groupId)
{
	//异步查询用户基本信息 todo
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(SEL_LEVELGROUPSLEVELS);
	 stmt->setUInt32(0,groupId);
	 //同步查询
	PreparedQueryResult result = PassPortDatabase.SynQuery(stmt);
	return result;
}



//////////////////////////////////////////////////////////////////////////////
bool DC_DataBase::_InsertLevels(LoggedUserPtr user)
{
	std::string strUUId = user->GetUUId();
	//BSLevel bslevel = user->bsLevel();
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
		   if(!PassPortDatabase.SynExecute(stmt))
			{
				//插入失败
				VN_LOG_ERROR(strUUId<<":_InsertLevels "<<"failed");
				return false;
			}
	}
	return true;
}



void DC_DataBase::Async_InsertLevels(AsyncObjectPtr obj,LoggedUserPtr user,std::function<void(bool ret)>doResult)
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
		   PassPortDatabase.AsyncExecute(stmt,obj,[this,task,doResult](bool ret)
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
					return	 doResult(result);
				}

			});
	}
}








bool DC_DataBase::_LoadLevels(LoggedUserPtr user)
{
	std::string strUUId=user->GetUUId();
	auto &levelMap=user->bsLevel().levelList();
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(SEL_BASEINFOLEVELS);
	stmt->setString(0,strUUId);
	//同步查询
	PreparedQueryResult result = PassPortDatabase.SynQuery(stmt);
	//查不到指定UUid对应的hero数据
	if (!(result && result->GetRowCount()))
	{
		VN_LOG_ERROR("_LoadLevels failed ");
		return true;
	}
	do
	{
		u32 levelid=(*result)[0].GetInt32();
		bool instrusion=(*result)[1].GetBool();
		u32 instrTimes=(*result)[2].GetUInt32();
		s32 scores=(*result)[3].GetInt32();
		s32 shovel=(*result)[4].GetInt32();
		s32 stars=(*result)[5].GetInt32();
		s32 watch=(*result)[6].GetInt32();
		BSLevel::LevelInfo *li=vnnew BSLevel::LevelInfo;
		li->shovel = shovel;
		li->watch = watch;
		li->score = scores;
		li->starNum = stars;
		auto info = BSFactory::instance().find_level(levelid);
		if (info)
		{
			auto li = vnnew BSLevel::LevelInfo();
			li->level = info;
			li->starNum = stars;
			li->score = scores;
			li->shovel = shovel;
			li->watch = watch;
			li->intrusion = instrusion;
			li->intrusionTimes = instrTimes;
			//user->bsLevel().levelList().insert(std::make_pair(levelid,li));
			levelMap[levelid] = li;
			if (li->intrusion)
			{
				user->bsGoldMine()._setStatus(info->f_MineId.value, levelid, false, 1);
			} else if (li->starNum == 3)
			{
				user->bsGoldMine()._setStatus(info->f_MineId.value, levelid, false, 2);
			}
		}
	}while(result->NextRow());
	return true;
}



bool DC_DataBase::_InsertMines(LoggedUserPtr user)
{
	std::string strUUId = user->GetUUId();
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
			if(!PassPortDatabase.SynExecute(stmt))
			{
				//插入失败
				VN_LOG_ERROR(strUUId<<":_InsertMines "<<"failed");
				return false;
			}
		}
	return true;
}


void DC_DataBase::Async_InsertMines(AsyncObjectPtr obj,LoggedUserPtr user,std::function<void(bool ret)>doResult)
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
		PassPortDatabase.AsyncExecute(stmt,obj,[this,task,doResult](bool ret)
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
				return	 doResult(result);
			}
		});
	}
}



bool DC_DataBase::_LoadMines(std::string &strUUId,BSGoldMine &goldMine)
{
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(SEL_BASEINFOMINES);
	stmt->setString(0,strUUId);
	//同步查询
	PreparedQueryResult result = PassPortDatabase.SynQuery(stmt);
	//查不到指定UUid对应的hero数据
	if (!(result && result->GetRowCount()))
	{
		VN_LOG_ERROR("_LoadLevels failed ");
		return false;
	}
	goldMine.load(result);
	return true;
}





bool DC_DataBase::_InsertMissions(LoggedUserPtr user)
{
	std::string strUUId = user->GetUUId();
	 for (auto &i :user->bsMission().list())
	{
		u32 missionid=i.first;
		bool missionstatus=i.second;
		PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(INSERT_BASEINFOMISSION);
		stmt->setString(0,strUUId);
		stmt->setUInt32(1,missionid);
		stmt->setBool(2,missionstatus);
		if(!PassPortDatabase.SynExecute(stmt))
		{
			//插入失败
			VN_LOG_ERROR(strUUId<<":_InsertMissions "<<"failed");
			return false;
		}
	}
	return true;
}



void DC_DataBase::Async_InsertMissions(AsyncObjectPtr obj,LoggedUserPtr user,std::function<void(bool ret)>doResult)
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
		PassPortDatabase.AsyncExecute(stmt,obj,[this,task,doResult](bool ret)
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
					return	 doResult(result);
			}

		});
	}
}





bool DC_DataBase::_LoadMissions(std::string &strUUId,BSMission &bsMission)
{
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(SEL_BASEINFOMISSION);
	stmt->setString(0,strUUId);
	//同步查询
	PreparedQueryResult result = PassPortDatabase.SynQuery(stmt);
	//查不到指定UUid对应的hero数据
	if (!(result && result->GetRowCount()))
	{
		VN_LOG_ERROR("_LoadMissions failed ");
		return false;
	}
	bsMission.load(result);
	/*do
	{
		  u32 missionId = (*result)[0].GetUInt32();
		  bool missionstatus = (*result)[1].GetBool();
		  auto it = rewards.find(missionId);
		 if (it != rewards.end())
		 {
			 it->second = missionstatus;
		 }
		 else
		 {
			 it->second = false;
		 }
	}while(result->NextRow());*/
	return true;
}







bool DC_DataBase::_InsertMonsterMap(LoggedUserPtr user)
{
	std::string strUUId = user->GetUUId();
	 for (auto &i :user->bsMonsterMap().monsterMap())
		{
			u32 monterid=i.first;
			bool monsterstatus=i.second;
			PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(INSERT_BASEINFOMONSTERMAP);
			stmt->setString(0,strUUId);
			stmt->setUInt32(1,monterid);
			stmt->setBool(2,monsterstatus);
			if(!PassPortDatabase.SynExecute(stmt))
			{
				//插入失败
				VN_LOG_ERROR(strUUId<<":_InsertMonsterMap "<<"failed");
				return false;
			}
		}
		return true;
}



void DC_DataBase::Async_InsertMonsterMap(AsyncObjectPtr obj,LoggedUserPtr user,std::function<void(bool ret)>doResult)
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
		PassPortDatabase.AsyncExecute(stmt,obj,[this,task,doResult](bool ret)
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
				return	 doResult(result);
			}

		});
	}
}



bool DC_DataBase::_LoadMonsterMap(std::string &strUUId,BSMonsterMap &monsterMap)
{
		//auto &monsterMap = user->bsMonsterMap().monsterMap();
		PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(SEL_BASEINFOMONSTERMAP);
		stmt->setString(0,strUUId);
		//同步查询
		PreparedQueryResult result = PassPortDatabase.SynQuery(stmt);
		//查不到指定UUid对应的数据
		if (!(result && result->GetRowCount()))
		{
			VN_LOG_ERROR("_LoadMonsterMap failed ");
			return false;
		}
		monsterMap.load(result);
	/*	do
		{
			  s32 monterid = (*result)[0].GetInt32();
			  bool monsterstatus = (*result)[1].GetBool();
			  monsterMap[monterid] = monsterstatus;
		}while(result->NextRow());*/
		return true;
}






bool	DC_DataBase::_InsertPlatformStore(LoggedUserPtr user)
{
		std::string strUUId = user->GetUUId();
		auto &levelArray = user->bsPlatformStore().list();
		for (uint i=0;i<levelArray.size();++i)
		{
			PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(INSERT_BASEINFPLATFORMSTORE);
			stmt->setString(0,strUUId);
			stmt->setUInt32(1,i);
			stmt->setUInt32(2,levelArray[i]);
			if(!PassPortDatabase.SynExecute(stmt))
			{
				//插入失败
				VN_LOG_ERROR(strUUId<<":_InsertPlatformStore "<<"failed");
				return false;
			}
		}
		return true;
}



void DC_DataBase::Async_InsertPlatformStore(AsyncObjectPtr obj,LoggedUserPtr user,std::function<void(bool ret)>doResult)
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
			PassPortDatabase.AsyncExecute(stmt,obj,[this,task,doResult](bool ret)
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
					return	 doResult(result);
				}

			});
		}
}




bool DC_DataBase::_LoadPlatformStore(std::string &strUUId ,BSPlatformStore &platStore)
{
		//auto &platformArray = user->bsPlatformStore().list();
		PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(SEL_BASEINFPLATFORMSTORE);
		stmt->setString(0,strUUId);
		//同步查询
		PreparedQueryResult result = PassPortDatabase.SynQuery(stmt);
		//查不到指定UUid对应的数据
		if (!(result && result->GetRowCount()))
		{
			VN_LOG_ERROR("_LoadPlatformStore failed ");
			return false;
		}
		platStore.load(result);
		/*for(auto &i:platformArray)
		{
			i= (*result)[0].GetUInt32();
			if(!result->NextRow())
			{
				break;
			}
		}*/
		return true;
}




bool DC_DataBase::_InsertTechUpgrade(LoggedUserPtr user)
{
	std::string strUUId = user->GetUUId();
	//BSTechUpgrade bstechgrade=user->bsTechUpgrade();
		auto &techVec=user->bsTechUpgrade().techList();
		for (uint i=0;i<techVec.size();++i)
		{
			PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(INSERT_BASEINFOTECHUPGRADE);
			stmt->setString(0,strUUId);
			stmt->setUInt32(1,i);
			stmt->setUInt32(2,techVec[i]);
			if(!PassPortDatabase.SynExecute(stmt))
			{
				//插入失败
				VN_LOG_ERROR(strUUId<<":_InsertTechUpgrade "<<"failed");
				return false;
			}
		}
		return true;
}

void  DC_DataBase::Async_InsertTechUpgrade(AsyncObjectPtr obj,LoggedUserPtr user,std::function<void(bool ret)>doResult)
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
			PassPortDatabase.AsyncExecute(stmt,obj,[this,task,doResult](bool ret)
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
						return	 doResult(result);

					}

				});
		}
}


bool DC_DataBase::_LoadTechUpgrade(std::string &strUUId,BSTechUpgrade &bsTech)
{
		//auto &techVec=user->bsTechUpgrade().techList();
		PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(SEL_BASEINFOTECHUPGRADE);
		stmt->setString(0,strUUId);
		//同步查询
		PreparedQueryResult result = PassPortDatabase.SynQuery(stmt);
		//查不到指定UUid对应的数据
		if (!(result && result->GetRowCount()))
		{
			VN_LOG_ERROR("_LoadPlatformStore failed ");
			return false;
		}
		bsTech.load( result);
		/*for(uint i=0;i<techVec.size()&&i<BSTechUpgrade::kTechCount;++i)
		{
			u32 techstatus= (*result)[0].GetUInt32();
			techVec.push_back(techstatus);
			if(!result->NextRow())
			{
				break;
			}
		}*/
		return true;
}





bool DC_DataBase::_InsertTowerUpgrade(LoggedUserPtr user)
{
	std::string strUUId= user->GetUUId();
	//BSTowerUpgrade bstowergrade=user->bsTowerUpgrade();
		auto &towerVec=user->bsTowerUpgrade().towerList();
		for (uint i=0;i<towerVec.size();++i)
		{
			for(uint j=0;j<towerVec[i].size();++j)
			{
				PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(INSERT_BASEINFOTOWERUPGRADE);
				stmt->setString(0,strUUId);
				stmt->setUInt32(1,i);
				stmt->setUInt32(2,j);
				stmt->setUInt32(3,towerVec[i][j]);
				if(!PassPortDatabase.SynExecute(stmt))
				{
					//插入失败
					VN_LOG_ERROR(strUUId<<":_InsertTechUpgrade "<<"failed");
					return false;
				}
			}
		}
		return true;
}

void DC_DataBase::Async_InsertTowerUpgrade(AsyncObjectPtr obj,LoggedUserPtr user,std::function<void(bool ret)>doResult)
{
	std::string strUUId= user->GetUUId();
	auto &towerVec=user->bsTowerUpgrade().towerList();
	AsyncTaskPtr task = vnnew AsyncTask();
	int taskSize= 0;
	for (uint i=0;i<towerVec.size();++i)
	{
		for(uint j=0;j<towerVec[i].size();++j)
		{
			taskSize++;
		}
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
			PassPortDatabase.AsyncExecute(stmt,obj,[this,task,doResult](bool ret)
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
					return	 doResult(result);
				}

			});
		}
	}
}

bool DC_DataBase::_LoadTowerUpgrade(std::string &strUUId,BSTowerUpgrade &bstowergrade)
{
		PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(SEL_BASEINFOTOWERUPGRADE);
		stmt->setString(0,strUUId);
		//同步查询
		PreparedQueryResult result = PassPortDatabase.SynQuery(stmt);
		//查不到指定UUid对应的数据
		if (!(result && result->GetRowCount()))
		{
			VN_LOG_ERROR("_LoadPlatformStore failed ");
			return false;
		}
		//判断数据有效性
		if(result->GetRowCount()!=(BSTowerUpgrade::kNumClass * BSTowerUpgrade::kNumTowerInClass))
		{
			VN_LOG_ERROR("_LoadPlatformStore data unvalid ");
			return false;
		}
		bstowergrade.load(result);
		return true;
}

//update



/*void DC_DataBase::AsyncUpdateBaseInfo(AsyncObjectPtr asyncObject,LoggedUserPtr pUser,std::function<void(bool)> doResulet)
{
	std::string stdUUId = pUser->GetUUId();
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_BASEINFO_BASEINFO);
	stmt->setInt32(0,pUser->bsPlayerBaseInfo().chip(0));
	stmt->setInt32(1,pUser->bsPlayerBaseInfo().chip(1));
	stmt->setInt32(2,pUser->bsPlayerBaseInfo().chip(2));
	stmt->setInt32(3,pUser->bsPlayerBaseInfo().money());
	stmt->setInt32(4,pUser->bsPlayerBaseInfo().diamond()) ;
	stmt->setUInt32(5,pUser->GetPurchaseDiamond());
	stmt->setUInt32(6,pUser->GetStamp());
	stmt->setInt32(7,pUser->bsHero().shovel()[0]);
	stmt->setInt32(8,pUser->bsHero().shovel()[1]);
	stmt->setInt32(9,pUser->bsHero().skull()[0]);
	stmt->setInt32(10,	pUser->bsHero().skull()[1]);
	stmt->setInt32(11,pUser->bsHero().watch()[0]);
	stmt->setInt32(12,pUser->bsHero().watch()[1]);
	stmt->setString(13,pUser->GetUUId());
	PassPortDatabase.AsyncExecute(stmt,asyncObject,doResulet);
}*/

void DC_DataBase::AsyncUpdateBaseInfo_Baseinfo(AsyncObjectPtr asyncObject,std::string &strUUid,s32 clip0,s32 clip1,s32 clip2,s32 money,s32 diamond,u32 purchaseDiamond,std::function<void(bool)> doResulet)
{
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_BASEINFO_BASEINFO);
	stmt->setInt32(0,clip0);
	stmt->setInt32(1,clip1);
	stmt->setInt32(2,clip2);
	stmt->setInt32(3,money);
	stmt->setInt32(4,diamond) ;
	stmt->setUInt32(5,purchaseDiamond);
	PassPortDatabase.AsyncExecute(stmt,asyncObject,doResulet);
}

void DC_DataBase::AsyncUpdateBaseInfo_Hero(AsyncObjectPtr asyncObject,std::string &strUUid,s32 shovel0,s32 shovel1,s32 skill0,s32 skill1,s32 watch0,s32 watch1,std::function<void(bool)> doResulet)
{
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_BASEINFO_HERO);
	stmt->setInt32(0,shovel0);
	stmt->setInt32(1,shovel1);
	stmt->setInt32(2,skill0);
	stmt->setInt32(3,	skill1);
	stmt->setInt32(4,watch0);
	stmt->setInt32(5,watch1);
	PassPortDatabase.AsyncExecute(stmt,asyncObject,doResulet);
}


void DC_DataBase::AsyncUpdateBaseInfo_Registion(AsyncObjectPtr asyncObject,std::string &strUUid,std::string &strfirstDate,std::string &strlastDate,s32 count,s32 group,std::function<void(bool)> doResulet)
{
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_BASEINFO_REGISTRATION);
	stmt->setString(0,strfirstDate);
	stmt->setString(1,strlastDate);
	stmt->setInt32(2,count);
	stmt->setInt32(3,group);
	stmt->setString(4,strUUid);
	PassPortDatabase.AsyncExecute(stmt,asyncObject,doResulet);
}


void DC_DataBase::AsyncUpdateBaseInfo_Lottery(AsyncObjectPtr asyPtr,std::string &strUUid,u32 lotteryDraw,s32 lotteryState,f32 lotteryTimer,std::string &lotteryPool,vn::u64 timestamp,std::function<void(bool)> doResulet)
{
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_BASEINFO_LOTTERY);
	stmt->setUInt32(0,lotteryDraw);
	stmt->setInt32(1,lotteryState);
	stmt->setFloat(2,lotteryTimer);
	stmt->setString(3,lotteryPool);
	stmt->setUInt64(4,timestamp);
	stmt->setString(5,strUUid);
	PassPortDatabase.AsyncExecute(stmt,asyPtr,doResulet);
}

void DC_DataBase::AsyncUpdateBaseInfo_Coupon(AsyncObjectPtr asyPtr,std::string &strUUid,s32 couponId,f32 couponTimer,vn::u64 timeStamp,std::function<void(bool)> doResulet)
{
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_BASEINFO_COUPON);
	stmt->setInt32(0,couponId);
	stmt->setFloat(1,couponTimer);
	stmt->setUInt64(2,timeStamp);
	stmt->setString(3,strUUid);
	PassPortDatabase.AsyncExecute(stmt,asyPtr,doResulet);
}
















//跟新基本信息和intrusion和lottery和coupon
void DC_DataBase::AsyncUpdateBaseInfoAll(AsyncObjectPtr asyncObject,LoggedUserPtr pUser,std::function<void(bool)> doResulet)
{
	vn::u64 timeStamp = time(NULL);
	std::string strUUId = pUser->GetUUId();
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_BASEINFO_ALL);
	stmt->setInt32(0,pUser->bsPlayerBaseInfo().chip(0));
	stmt->setInt32(1,pUser->bsPlayerBaseInfo().chip(1));
	stmt->setInt32(2,pUser->bsPlayerBaseInfo().chip(2));
	stmt->setInt32(3,pUser->bsPlayerBaseInfo().money());
	stmt->setInt32(4,pUser->bsPlayerBaseInfo().diamond()) ;
	stmt->setUInt32(5,pUser->GetPurchaseDiamond());
	stmt->setUInt32(6,pUser->GetStamp());
	stmt->setInt32(7,pUser->GetCouponId());
	stmt->setFloat(8,pUser->GetCouponTimer());
	stmt->setUInt64(9,timeStamp);
	stmt->setInt32(10,pUser->bsHero().shovel()[0]);
	stmt->setInt32(11,pUser->bsHero().shovel()[1]);
	stmt->setInt32(12,pUser->bsHero().skull()[0]);
	stmt->setInt32(13,	pUser->bsHero().skull()[1]);
	stmt->setInt32(14,pUser->bsHero().watch()[0]);
	stmt->setInt32(15,pUser->bsHero().watch()[1]);
	stmt->setBool(16,pUser->bsIntrusion().Getrunning());
	stmt->setFloat(17,pUser->bsIntrusion().GetTimer());
	stmt->setUInt64(18,timeStamp);
	stmt->setInt32(19,pUser->GetLotteryDrawTimer());
	stmt->setInt32(20,pUser->GetLotteryState());
	stmt->setFloat(21,pUser->GetLotteyTimer());
	std::string strLotteryPool;
	pUser->GetLotteryPool(strLotteryPool);
	stmt->setString(22,strLotteryPool);
	stmt->setUInt64(23,timeStamp);
	stmt->setString(24,strUUId);
	PassPortDatabase.AsyncExecute(stmt,asyncObject,doResulet);
}


void DC_DataBase::AsyncUpdateIntrusion(std::string &strUUId,bool running,f32 timer,vn::u64 timestamp,AsyncObjectPtr asyncObject,std::function<void(bool)> doResulet)
{
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_BASEINFO_INTRUSION);
	stmt->setBool(0,running);
	stmt->setFloat(1,timer);
	stmt->setUInt64(2,timestamp);
	stmt->setString(3,strUUId);
	PassPortDatabase.AsyncExecute(stmt,asyncObject,doResulet);
}



void DC_DataBase::AsyncUpdateTimeStamp(AsyncObjectPtr asyncObject,std::string &strUUId,u32 stamp,std::function<void(bool)> doResulet)
{
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_TIMESTAMP);
	stmt->setUInt32(0,stamp);
	stmt->setString(1,strUUId);
	PassPortDatabase.AsyncExecute(stmt,asyncObject,doResulet);
}


void DC_DataBase::AsyncUpdateLoginInfo(AsyncObjectPtr asyPtr,LoggedUserPtr pUser,std::function<void(bool)> doResulet)
{
	std::string strUUId = pUser->GetUUId();
	vn::u64 curTime = time(NULL);
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_LOGIN);
	stmt->setInt32(0,pUser->GetCouponId());
	stmt->setFloat(1,pUser->GetCouponTimer());
	stmt->setUInt64(2,curTime);
	stmt->setInt32(3,pUser->GetLotteryDrawTimer());
	stmt->setInt32(4,pUser->GetLotteryState());
	stmt->setFloat(5,pUser->GetLotteyTimer());
	std::string strLotteryPool;
	pUser->GetLotteryPool(strLotteryPool);
	stmt->setString(6,strLotteryPool);
	stmt->setUInt64(7,curTime);
	stmt->setUInt64(8,curTime);
	stmt->setString(9,strUUId);
	PassPortDatabase.AsyncExecute(stmt,asyPtr,doResulet);
}


/*
void DC_DataBase::AsyncUpdateCoupon(AsyncObjectPtr asyPtr,LoggedUserPtr pUser,std::function<void(bool)> doResulet)
{
	vn::u64 curTime = time(NULL);
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_COUPON);
	stmt->setInt32(0,pUser->GetCouponId());
	stmt->setFloat(1,pUser->GetCouponTimer());
	stmt->setUInt64(2,curTime);
	PassPortDatabase.AsyncExecute(stmt,asyPtr,doResulet);
}
*/



/*
void DC_DataBase::AsyncUpdateLottery(AsyncObjectPtr asyPtr,LoggedUserPtr pUser,std::function<void(bool)> doResulet)
{
	vn::u64 curTime = time(NULL);
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_LOTTERY);
	stmt->setInt32(0,pUser->GetLotteryDrawTimer());
	stmt->setInt32(1,pUser->GetLotteryState());
	stmt->setFloat(2,pUser->GetLotteyTimer());
	std::string strLotteryPool;
	pUser->GetLotteryPool(strLotteryPool);
	stmt->setString(3,strLotteryPool);
	stmt->setUInt64(4,curTime);
	PassPortDatabase.AsyncExecute(stmt,asyPtr,doResulet);
}
*/





void DC_DataBase::AsyncUpdateMission(AsyncObjectPtr asyObj,std::string &strUUId,u32 missionId,bool missionStatus,std::function<void(bool)> doResulet)
{
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_BASEINFOMISSION);
	stmt->setBool(0,missionStatus);
	stmt->setString(1,strUUId);
	stmt->setUInt32(2,missionId);
	PassPortDatabase.AsyncExecute(stmt,asyObj,doResulet);
}




void DC_DataBase::AsyncInsertForceGuide(LoggedUserPtr pUser,u32 guideid,std::function<void(bool)> doResulet)
{
	std::string strUUId =pUser->GetUUId();
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(INSERT_BASEINFOFORCEGUIDE);
	stmt->setString(0,strUUId);
	stmt->setUInt32(1,guideid);
	PassPortDatabase.AsyncExecute(stmt,pUser,doResulet);
}



void DC_DataBase::AsyncUpdateMonsterMap(LoggedUserPtr pUser,u32 monsterid,bool monsterStatus,std::function<void(bool)> doResulet)
{
	std::string strUUId =pUser->GetUUId();
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_BASEINFOMONSTERMAP);
	stmt->setBool(0,monsterStatus);
	stmt->setString(1,strUUId);
	stmt->setUInt32(2,monsterid);
	PassPortDatabase.AsyncExecute(stmt,pUser,doResulet);
}




/*void DC_DataBase::AsyncAddMoney(LoggedUser *pUser,std::string &strUUId,s32 nowMoney,std::function<void(bool)> doResulet)
{
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_ADDMONEY);
	stmt->setInt32(0,nowMoney);
	stmt->setString(1,strUUId);
	PassPortDatabase.AsyncExecute(stmt,{pUser,true},doResulet);
}*/




/*void DC_DataBase::AsyncAddDiamond(LoggedUser *pUser,std::string &strUUId,s32 nowDiamond,std::function<void(bool)> doResulet)
{
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_ADDDIAMOND);
	stmt->setInt32(0,nowDiamond);
	stmt->setString(1,strUUId);
	PassPortDatabase.AsyncExecute(stmt,{pUser,true},doResulet);
}*/

void DC_DataBase::AsyncUpdateItemStore(LoggedUserPtr pUser,u32 itemId,u32 itemNum,std::function<void(bool)> doResulet)
{
	std::string strUUId =pUser->GetUUId();
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_BASEINFOITEMSTORE);
	stmt->setUInt32(0,itemNum);
	stmt->setString(1,strUUId);
	stmt->setUInt32(2,itemId);
	PassPortDatabase.AsyncExecute(stmt,pUser,doResulet);
}







void DC_DataBase::AsyncInsertLevel(AsyncObjectPtr obj,std::string &strUUId,u32 levelId,bool intrusion,u32 intrusiontimes,u32 starNum,s32 score,bool shovel,bool watch,std::function<void(bool)> doResulet)
{
	//insert into baseinfo_levels(UUId,levelid,intrusion,intrusion_times,score,shovel,stars,watch)values(?,?,?,?,?,?,?,?)
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(INSERT_BASEINFOLEVELS);
	stmt->setString(0,strUUId);
	stmt->setUInt32(1,levelId);
	stmt->setBool(2,intrusion);
	stmt->setUInt32(3,intrusiontimes);
	stmt->setUInt32(4,score);
	stmt->setBool(5,shovel);
	stmt->setUInt32(6,starNum);
	stmt->setBool(7,watch);
	PassPortDatabase.AsyncExecute(stmt,obj,doResulet);
}



void DC_DataBase::AsyncInsertLevelGroups(LoggedUserPtr pUser,u32 groupId,bool pressed,u32 intrusionCount,u32 rewardsCount,std::function<void(bool)> doResulet)
{
	std::string strUUId=pUser->GetUUId();
	//insert into baseinfo_levels(UUId,levelid,intrusion,intrusion_times,score,shovel,stars,watch)values(?,?,?,?,?,?,?,?)
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(INSERT_BASEINFOLEVELGROUPS);
	stmt->setString(0,strUUId);
	stmt->setUInt32(1,groupId);
	stmt->setBool(2,pressed);
	stmt->setUInt32(3,intrusionCount);
	stmt->setUInt32(4,rewardsCount);
	PassPortDatabase.AsyncExecute(stmt,pUser,doResulet);
}


void DC_DataBase::AsyncUpdateLevelGroups(AsyncObjectPtr asyncObjPtr,LoggedUserPtr user,u32 groupId,bool pressed,u32 intrusionCount,u32 rewardsCount,std::function<void(bool)> doResulet)
{
	std::string strUUId =user->GetUUId();
	//insert into baseinfo_levels(UUId,levelid,intrusion,intrusion_times,score,shovel,stars,watch)values(?,?,?,?,?,?,?,?)
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_BASEINFOLEVELGROUPS);
	stmt->setBool(0,pressed);
	stmt->setUInt32(1,intrusionCount);
	stmt->setUInt32(2,rewardsCount);
	stmt->setString(3,strUUId);
	stmt->setUInt32(4,groupId);
	PassPortDatabase.AsyncExecute(stmt,asyncObjPtr,doResulet);
}

bool DC_DataBase::SyncUpdateLevelGroups(std::string &strUUId,u32 groupId,bool pressed,u32 intrusionCount,u32 rewardsCount)
{
	//insert into baseinfo_levels(UUId,levelid,intrusion,intrusion_times,score,shovel,stars,watch)values(?,?,?,?,?,?,?,?)
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_BASEINFOLEVELGROUPS);
	stmt->setBool(0,pressed);
	stmt->setUInt32(1,intrusionCount);
	stmt->setUInt32(2,rewardsCount);
	stmt->setString(3,strUUId);
	stmt->setUInt32(4,groupId);
	return PassPortDatabase.SynExecute(stmt);
}





void DC_DataBase::AsyncInsertLevelGroup_Levels(LoggedUserPtr pUser,u32 groupId,u32 levelId,std::string heros,std::string platforms, bool resolved,bool shovel ,u32 stars,u8 passed,bool watch,std::function<void(bool ret)> doResulet)
{
	std::string strUUId=pUser->GetUUId();
	//insert into baseinfo_levels(UUId,levelid,intrusion,intrusion_times,score,shovel,stars,watch)values(?,?,?,?,?,?,?,?)
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(INSERT_LEVELGROUPS_LEVELS);
	stmt->setUInt32(0,groupId);
	stmt->setUInt32(1,levelId);
	stmt->setString(2,heros);
	stmt->setString(3,platforms);
	stmt->setBool(4,resolved);
	stmt->setBool(5,shovel);
	stmt->setUInt32(6,stars);
	stmt->setUInt8(7,passed);
	stmt->setBool(8,watch);
	PassPortDatabase.AsyncExecute(stmt,pUser,doResulet);
}



void DC_DataBase::AsyncUpdateLevelGroup_Levels(AsyncObjectPtr asyncObjPtr,LoggedUserPtr user,u32 groupId,u32 levelId,std::string heros,std::string platforms, bool resolved,bool shovel ,u32 stars,u8 passed,bool watch,std::function<void(bool ret)> doResulet)
{
	std::string strUUId = user->GetUUId();
	//insert into baseinfo_levels(UUId,levelid,intrusion,intrusion_times,score,shovel,stars,watch)values(?,?,?,?,?,?,?,?)
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_LEVELGROUPSLEVELS);
	stmt->setString(0,heros);
	stmt->setString(1,platforms);
	stmt->setBool(2,resolved);
	stmt->setBool(3,shovel);
	stmt->setUInt32(4,stars);
	stmt->setUInt8(5,passed);
	stmt->setBool(6,watch);
	stmt->setUInt32(7,groupId);
	stmt->setUInt32(8,levelId);
	PassPortDatabase.AsyncExecute(stmt,asyncObjPtr,doResulet);
}




bool DC_DataBase::SyncUpdateLevelGroup_Levels(std::string &strUUId,u32 groupId,u32 levelId,std::string &heros,std::string &platforms, bool resolved,bool shovel ,u32 stars,u8 passed,bool watch)
{

	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_LEVELGROUPSLEVELS);
	stmt->setString(0,heros);
	stmt->setString(1,platforms);
	stmt->setBool(2,resolved);
	stmt->setBool(3,shovel);
	stmt->setUInt32(4,stars);
	stmt->setUInt8(5,passed);
	stmt->setBool(6,watch);
	stmt->setUInt32(7,groupId);
	stmt->setUInt32(8,levelId);
	return PassPortDatabase.SynExecute(stmt);
}

















void DC_DataBase::AsyncUpdateLevel(AsyncObjectPtr asyncObjPtr,std::string &strUUId,u32 levelId,bool intrusion,u32 intrusiontimes,u32 starNum,s32 score,bool shovel,bool watch,std::function<void(bool)> doResulet)
{
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_BASEINFOLEVELS);
	stmt->setBool(0,intrusion);
	stmt->setUInt32(1,intrusiontimes);
	stmt->setUInt32(2,score);
	stmt->setBool(3,shovel);
	stmt->setUInt32(4,starNum);
	stmt->setBool(5,watch);
	stmt->setString(6,strUUId);
	stmt->setUInt32(7,levelId);
	PassPortDatabase.AsyncExecute(stmt,asyncObjPtr,doResulet);
}


bool DC_DataBase::SyncUpdateLevel(std::string &strUUId,u32 levelId,bool intrusion,u32 intrusiontimes,u32 starNum,s32 score,bool shovel,bool watch)
{
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_BASEINFOLEVELS);
	stmt->setBool(0,intrusion);
	stmt->setUInt32(1,intrusiontimes);
	stmt->setUInt32(2,score);
	stmt->setBool(3,shovel);
	stmt->setUInt32(4,starNum);
	stmt->setBool(5,watch);
	stmt->setString(6,strUUId);
	stmt->setUInt32(7,levelId);
	return PassPortDatabase.SynExecute(stmt);
}












void DC_DataBase::AsyncUpdateTech(LoggedUserPtr pUser,u32 techIndex,u32 techStatus,std::function<void(bool)> doResulet)
{
	std::string strUUId=pUser->GetUUId();
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_BASEINFOTECHUPGRADE);
	stmt->setUInt32(0,techStatus);
	stmt->setString(1,strUUId);
	stmt->setUInt32(2,techIndex);
	PassPortDatabase.AsyncExecute(stmt,pUser,doResulet);
}



void DC_DataBase::AsyncUpdateTower(LoggedUserPtr pUser,u32 classIndex,u32 inclassIndex,u32 upgradeValue,std::function<void(bool)> doResulet)
{
	std::string strUUId=pUser->GetUUId();
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_BASEINFOTOWERUPGRADE);
	stmt->setUInt32(0,upgradeValue);
	stmt->setString(1,strUUId);
	stmt->setUInt32(2,classIndex);
	stmt->setUInt32(3,inclassIndex);
	PassPortDatabase.AsyncExecute(stmt,pUser,doResulet);
}






bool DC_DataBase::UpdateRunesStatus(u32 runId,s32 runstatus,std::string &heroId)
{
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_RUNESTATUS);
	stmt->setInt32(0,runstatus);
	stmt->setString(1,heroId);
	stmt->setUInt32(2,runId);
	return PassPortDatabase.SynExecute(stmt);
}



void DC_DataBase::AsyncUpdateRunesStatus(u32 runId,s32 runstatus,LoggedUserPtr pUser,std::string &heroId,std::function<void(bool)>doResult)
{
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_RUNESTATUS);
	stmt->setInt32(0,runstatus);
	stmt->setString(1,heroId);
	stmt->setUInt32(2,runId);
	return PassPortDatabase.AsyncExecute(stmt,pUser,doResult);
}



void DC_DataBase::AsyncUpdateGoldMine(std::string &strUUId,u32 id, u32 num,f32 timer,vn::u64 timeStamp,AsyncObjectPtr asyncObjectPtr,std::function<void(bool)>doResult)
{
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_BASEINFOMINES);
	stmt->setUInt32(0,num);
	stmt->setFloat(1,timer);
	stmt->setUInt64(2,timeStamp);
	stmt->setString(3,strUUId);
	stmt->setUInt32(4,id);
	return PassPortDatabase.AsyncExecute(stmt,asyncObjectPtr,doResult);
}















void  DC_DataBase::AsyncLoadHeroIdbyUUIdTowerClass(LoggedUserPtr user,u32 towerClass,std::function<void(PreparedQueryResult)> doResulet)
{
	std::string strUUId = user->GetUUId();
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(FIND_HEROID);
	stmt->setString(0,strUUId);
	stmt->setUInt32(1,towerClass);
	//同步查询
	PassPortDatabase.AsyncQuery(stmt,user,doResulet);
}

PreparedQueryResult DC_DataBase::LoadHeroIdbyUUIdTowerClass(std::string &strUUId,u32 towerClass)
{
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(FIND_HEROID);
	stmt->setString(0,strUUId);
	stmt->setUInt32(1,towerClass);
	//同步查询
	PreparedQueryResult result = PassPortDatabase.SynQuery(stmt);
	return result;
}



bool DC_DataBase::UpdateItemStore(std::string &strUUId,u32 itemId,u32 itemNum)
{
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_BASEINFOITEMSTORE);
	stmt->setUInt32(0,itemNum);
	stmt->setString(1,strUUId);
	stmt->setUInt32(2,itemId);
	return PassPortDatabase.SynExecute(stmt);
}


bool DC_DataBase::InsertItemStore(std::string &strUUId,u32 itemId,u32 itemNum)
{
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(INSERT_BASEINFOITEMSTORE);
	stmt->setString(0,strUUId);
	stmt->setUInt32(1,itemId);
	stmt->setUInt32(2,itemNum);
	return PassPortDatabase.SynExecute(stmt);
}

bool DC_DataBase::UpdateMoney(std::string &strUUId,s32 money)
{
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_ADDMONEY);
	stmt->setInt32(0,money);
	stmt->setString(1,strUUId);
	return PassPortDatabase.SynExecute(stmt);
}



bool DC_DataBase::UpdateDiamond(std::string &strUUId,s32 diamond)
{
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_ADDDIAMOND);
	stmt->setInt32(0,diamond);
	stmt->setString(1,strUUId);
	return PassPortDatabase.SynExecute(stmt);
}




bool DC_DataBase::UpdateClip(std::string &strUUId,s32 clip0,s32 clip1,s32 clip2)
{
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_CLIP);
	stmt->setInt32(0,clip0);
	stmt->setInt32(1,clip1);
	stmt->setInt32(2,clip2);
	stmt->setString(3,strUUId);
	return PassPortDatabase.SynExecute(stmt);
}




bool DC_DataBase::LoadHero(LoggedUserPtr user)
{
	std::string strUUId = user->GetUUId();

	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(SEL_BASEINFOHERO);
	stmt->setString(0,strUUId);
	//同步查询
	PreparedQueryResult heroResult = PassPortDatabase.SynQuery(stmt);
	if (!(heroResult && heroResult->GetRowCount()))
	{
		VN_LOG_ERROR("LOAD HERO_BASEINFOHERO failed ");
		return false;
	}
	do
	{
		BSHero::HeroInfo info;
		//获取heroid 主键
		std::string heroid=(*heroResult)[0].GetString();
		info.id =(*heroResult)[1].GetUInt32();
		info.towerClass =(*heroResult)[2].GetUInt32();
		info.status =(BSHero::Status)(*heroResult)[3].GetInt32();
		info.unlockRes=(*heroResult)[4].GetUInt32();
		//查询heroid对应的runes
		stmt = PassPortDatabase.GetPreparedStatement(SEL_HERORUNES);
		stmt->setString(0,heroid);
		PreparedQueryResult result = PassPortDatabase.SynQuery(stmt);
		if  (!(result && result->GetRowCount()))
		{
			VN_LOG_ERROR("LOAD HERORUNES failed ");
			return false;
		}
		do
		{
			BSHero::Rune rune;
			rune.id= (*result)[0].GetUInt32();
			rune.status = (BSHero::Status)(*result)[1].GetInt32();
			info.runeList.push_back(rune);
		}while(result->NextRow());
		user->bsHero().load(info);
	}while(heroResult->NextRow());
	//user->load(dbhero);
	return true;
}


bool DC_DataBase::UpdateHeroStatus(u32 status,LoggedUserPtr user,u32 id)
{
	std::string strUUId = user->GetUUId();
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_HEROSTATUS);
	stmt->setUInt32(0,status);
	stmt->setString(1,strUUId);
	stmt->setUInt32(2,id);
	return PassPortDatabase.SynExecute(stmt);
}


//升级因熊
void DC_DataBase::AsyncupgradeHero(AsyncObjectPtr ptr,std::string &strUUId,u32 id,s32 status,u32 towerclass,std::function<void(bool)> doResulet)
{
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_UPGRADEHERO);
	stmt->setUInt32(0,id);
	stmt->setInt32(1,status);
	stmt->setString(2,strUUId);
	stmt->setUInt32(3,towerclass);
	PassPortDatabase.AsyncExecute(stmt,ptr,doResulet);
}


void DC_DataBase::AsyncUpdatePlatform(AsyncObjectPtr asyObj,std::string &strUUId,u32 category,u32 level,std::function<void(bool)> doResulet)
{
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(UP_BASEINFPLATFORMSTORE);
	stmt->setUInt32(0,level);
	stmt->setString(1,strUUId);
	stmt->setUInt32(2,category);
	PassPortDatabase.AsyncExecute(stmt,asyObj,doResulet);
}





void DC_DataBase::AsyncInsertLog(AsyncObjectPtr asyObj,std::string &strUUId,u32 eventType,std::string &strMessage,std::function<void(bool)> doResulet)
{
	PreparedStatement*  stmt=PassPortDatabase.GetPreparedStatement(INSERT_NETLOG);
	stmt->setString(0,strUUId);
	stmt->setUInt32(1,eventType);
	stmt->setString(2,strMessage);
	vn::u64 curTime = time(NULL);
	stmt->setUInt64(3,curTime);
	PassPortDatabase.AsyncExecute(stmt,asyObj,doResulet);
}




void DC_DataBase::AsyncCreateUser(AsyncObjectPtr obj,std::function<void (bool ret,  LoggedUserPtr user)>doResult)
{
    UUID uuid;
    uuid.generate();
    LoggedUserPtr user = vnnew LoggedUser(uuid);
    //获取插入数据库的初始数据
    user->reset_data();
    std::string strUUId = uuid.to_string();
   /* _AsyncInsertbaseinfo(strUUid,obj,[this,doResult,strUUid,user](bool ret)
	{
		if(!ret)
		{
			VN_LOG_ERROR("create user"<<strUUid<<"failed");
			return doResult(ret,user);
		}
		if(!_InsertForceGuide(user)||!_InsertHero(user)||!_InsertItemStore(user)||!_InsertLevels(user)||!_InsertMines(user)||!_InsertMissions(user)||!_InsertMonsterMap(user)||!_InsertPlatformStore(user)||!_InsertTechUpgrade(user)||!_InsertTowerUpgrade(user))
		{
		  return doResult(ret,user);
		}
		user->_markDirty();
		_addUser(user);
		return doResult(ret,user);
	});*/
AsyncTaskPtr createUserTask = vnnew AsyncTask();
createUserTask->Init(9);
/*_AsyncInsertbaseinfo(strUUId,obj,std::bind(&PreLoggingUser::_HandlAsyncCreateUser,obj.get(),std::placeholders::_1,createUserTask,user,doResult));
Async_InsertHero(obj,user,std::bind(&PreLoggingUser::_HandlAsyncCreateUser,obj.get(),std::placeholders::_1,createUserTask,user,doResult));
Async_InsertItemStore(obj,user,std::bind(&PreLoggingUser::_HandlAsyncCreateUser,obj.get(),std::placeholders::_1,createUserTask,user,doResult));
Async_InsertMines(obj,user,std::bind(&PreLoggingUser::_HandlAsyncCreateUser,obj.get(),std::placeholders::_1,createUserTask,user,doResult));
Async_InsertMissions(obj,user,std::bind(&PreLoggingUser::_HandlAsyncCreateUser,obj.get(),std::placeholders::_1,createUserTask,user,doResult));
Async_InsertMonsterMap(obj,user,std::bind(&PreLoggingUser::_HandlAsyncCreateUser,obj.get(),std::placeholders::_1,createUserTask,user,doResult));
Async_InsertPlatformStore(obj,user,std::bind(&PreLoggingUser::_HandlAsyncCreateUser,obj.get(),std::placeholders::_1,createUserTask,user,doResult));
Async_InsertTechUpgrade(obj,user,std::bind(&PreLoggingUser::_HandlAsyncCreateUser,obj.,std::placeholders::_1,createUserTask,user,doResult));
Async_InsertTowerUpgrade(obj,user,std::bind(&PreLoggingUser::_HandlAsyncCreateUser,obj,std::placeholders::_1,createUserTask,user,doResult));*/
}


//std::string &strUUid,AsyncObjectPtr obj,std::function<void(bool ret)>doResult
void DC_DataBase::_HandlAsyncCreateUser(bool ret,AsyncTaskPtr createUserTask, LoggedUserPtr user,std::function<void (bool ret,  LoggedUserPtr user)>doResult)
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
	return doResult(result,user);
}









bool DC_DataBase::_Insertbaseinfo(std::string &strUUid)
{
	 	 DBTable_baseinfo info;
	    info.Init();
	    PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(INSERT_BASEINFO);
	    stmt->setString(0,strUUid);
	    stmt->setInt32(1,info.chip[0]);
	    stmt->setInt32(2,info.chip[1]);
	    stmt->setInt32(3,info.chip[2]);
	    stmt->setInt32(4,info.money);
	    stmt->setInt32(5,info.diamonds) ;
	    stmt->setUInt32(6,info.purchasedDiamonds);
	    stmt->setUInt32(7,info.stamp);
	    stmt->setInt32(	8,info.coupon_id);
	    stmt->setFloat(9,info.coupon_timer);
	    stmt->setInt32(10,info.shovel0);
	    stmt->setInt32(11,info.shovel1);
	    stmt->setInt32(12,info.skull0);
	    stmt->setInt32(13,	info.skull1);
	    stmt->setInt32(14,info.watch0);
	    stmt->setInt32(15,info.watch1);
	    stmt->setBool(16,info.running);
	    stmt->setFloat(17,info.timer);
	    stmt->setInt32(18,info.count);
	    stmt->setString(19,info.first_date)	;
	    stmt->setInt32(20,info.group);
	    stmt->setString(21,info.last_date);
	    stmt->setInt32(22,info.lottery_draw);
	    stmt->setInt32(23,info.lottery_state);
	    stmt->setFloat(24,info.lottery_timer);
	    if(!PassPortDatabase.SynExecute(stmt))
	    {
	    	VN_LOG_ERROR("database insert "<<strUUid<<"failed");
	    	return false;
	    }
	    return true;
}


void  DC_DataBase::_AsyncInsertbaseinfo(std::string &strUUid,AsyncObjectPtr obj,std::function<void(bool ret)>doResult)
{
		DBTable_baseinfo info;
		info.Init();
		u64 curTime = time(NULL);
		PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(INSERT_BASEINFO);
		stmt->setString(0,strUUid);
		stmt->setInt32(1,info.chip[0]);
		stmt->setInt32(2,info.chip[1]);
		stmt->setInt32(3,info.chip[2]);
		stmt->setInt32(4,info.money);
		stmt->setInt32(5,info.diamonds) ;
		stmt->setUInt32(6,info.purchasedDiamonds);
		stmt->setUInt32(7,info.stamp);
		stmt->setInt32(	8,info.coupon_id);
		stmt->setFloat(9,info.coupon_timer);
		stmt->setUInt64(10,curTime);
		stmt->setInt32(11,info.shovel0);
		stmt->setInt32(12,info.shovel1);
		stmt->setInt32(13,info.skull0);
		stmt->setInt32(14,	info.skull1);
		stmt->setInt32(15,info.watch0);
		stmt->setInt32(16,info.watch1);
		stmt->setBool(17,info.running);
		stmt->setFloat(18,info.timer);
		stmt->setUInt64(19,curTime);
		stmt->setInt32(20,info.count);
		stmt->setString(21,info.first_date)	;
		stmt->setInt32(22,info.group);
		stmt->setString(23,info.last_date);
		stmt->setInt32(24,info.lottery_draw);
		stmt->setInt32(25,info.lottery_state);
		stmt->setFloat(26,info.lottery_timer);
		stmt->setString(27,"");
		stmt->setUInt64(28,curTime);
		PassPortDatabase.AsyncExecute(stmt,obj,doResult);
}



LoggedUserPtr DC_DataBase::createUser()
{
	 UUID uuid;
	    uuid.generate();
	    LoggedUserPtr user = vnnew LoggedUser(uuid);
	    //获取插入数据库的初始数据
	    user->reset_data();
	    std::string strUUid=uuid.to_string();
	    //todo开启事务

	    if(!_Insertbaseinfo(strUUid) || !_InsertForceGuide(user)||!_InsertHero(user)||!_InsertItemStore(user)||!_InsertLevels(user)||!_InsertMines(user)||!_InsertMissions(user)||!_InsertMonsterMap(user)||!_InsertPlatformStore(user)||!_InsertTechUpgrade(user)||!_InsertTowerUpgrade(user) )
	    {
	    	return LoggedUserPtr();
	    }
	    user->_markDirty();
	    _addUser(user);
	    return std::move(user);
}




LoggedUserPtr DC_DataBase::loadUser(const UUID &uuid)
{
	std::string stdUUid=uuid.to_string();
	//同步查询用户基本信息
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(SEL_BASEINFO);
	 stmt->setString(0,stdUUid);
	//同步查询
	PreparedQueryResult baseinfo = PassPortDatabase.SynQuery(stmt);
	//用户不存在
	if (!(baseinfo && baseinfo->GetRowCount()))
	{
		return  LoggedUserPtr();
	}
	//存在多个用户
	if(baseinfo->GetRowCount()>1)
	{
		VN_LOG_ERROR("Exist ONE MORE USER");
		return  LoggedUserPtr();
	}
	LoggedUserPtr user = vnnew LoggedUser(uuid);
	user->load(baseinfo);
	//继续查询用户其他信息
	if(!_LoadHero(stdUUid,user->bsHero()) || !_LoadForceGuide(user)||!_LoadItemStore(stdUUid,user->bsItemStore())||!_LoadLevels(user) || !_LoadMines(stdUUid,user->bsGoldMine())||!_LoadMissions(stdUUid,user->bsMission())||!_LoadMonsterMap(stdUUid,user->bsMonsterMap())||!_LoadPlatformStore(stdUUid,user->bsPlatformStore())||!_LoadTechUpgrade(stdUUid,user->bsTechUpgrade())||!_LoadTowerUpgrade(stdUUid,user->bsTowerUpgrade()))
	{
		return  LoggedUserPtr();
	}
	return std::move(user);
}




void DC_DataBase::AsyncLoadBaseInfo( std::string &stdUUid,vn::RefCountedPtr<AsyncObject> asyncObject,std::function<void (PreparedQueryResult result)> doResult)
{
	//异步查询用户基本信息 todo
	PreparedStatement* stmt = PassPortDatabase.GetPreparedStatement(SEL_BASEINFO);
	 stmt->setString(0,stdUUid);
	PassPortDatabase.AsyncQuery(stmt, asyncObject, doResult);
}











void DC_DataBase::saveUser(LoggedUserPtr user) {
    str8 fileName = "users/"+user->GetUUId() + ".bin";
    //以读属性打开文件（wb 覆盖原文件）
    FileStream *fs = m_fs->create(fileName);
    if (!fs) {
        VN_LOG_ERROR("DC_File: create file \"" << fileName << "\" failed!");
        return ;
    }
    //user->save(*fs);
    Variable_object obj;
    user->save(obj);
    auto txt = VariableScript::generate(&obj);
    fs->write(txt.c_str(), txt.length());
    fs->close();
}

vn::str8 DC_DataBase::generateOrder(const UUID &uuid, vn::u32 productId, vn::s32 price) {
    UUID order;
    m_ordersLock.lock_write();
    std::lock_guard<ReadWriteLock> guard(m_ordersLock, std::adopt_lock);
    order.generate();
    m_dirty = true;
    return m_orders.insert({order.to_string(), {uuid, productId, price, kOrderStatus_Paying}}).first->first;
}

void DC_DataBase::log_user_beginBattle(const UUID &uuid, u32 levelId, bool intrusion) {
    auto p = vnnew DC_File_EventLog_User_BeginBattle();
    p->levelId = levelId;
    p->intrusion = intrusion;
    _log(p, uuid);
}

void DC_DataBase::log_user_endBattle(const UUID &uuid, u32 levelId, bool intrusion, u32 stars, bool watch, bool shovel) {
    auto p = vnnew DC_File_EventLog_User_EndBattle();
    p->levelId = levelId;
    p->intrusion = intrusion;
    p->stars = (u8)stars;
    p->watch = watch;
    p->shovel = shovel;
    _log(p, uuid);
}




DataCenter::OrderStatus DC_DataBase::queryOrder(const vn::str8 &orderId, UUID &userId, vn::u32 &productId) {
    OrderInfo *info = 0;
    m_ordersLock.lock();
    auto it = m_orders.find(orderId);
    if (it != m_orders.end()) {
        info = &it->second;
    }
    m_ordersLock.unlock();

    if (!info) {
        return kOrderStatus_Invalid;
    }

    userId = info->userId;
    productId = info->productId;
    return info->status;
}

bool DC_DataBase::finishOrder(const vn::str8 &orderId, const UUID &userId, bool confirmed) {
    OrderInfo *info = 0;
    m_ordersLock.lock();
    auto it = m_orders.find(orderId);
    if (it != m_orders.end()) {
        info = &it->second;
    }
    m_ordersLock.unlock();

    if (!info || info->userId != userId) {
        return false;
    }
    if (info->status != kOrderStatus_Paying) {
        return false;
    }
    info->status = (confirmed ? kOrderStatus_Confirmed : kOrderStatus_Invalid);
    m_dirty = true;
    return true;
}


void DC_DataBase::fini()
{
	_flush();
	_flush_log();
	_flush_redeem_packets();
}

//accounts orders m_serverdata写入users文件
void DC_DataBase::_flush() {
    if (!m_fs) {
        return ;
    }
    m_accountsLock.lock();
    auto fs = m_fs->create("accounts");
    if (fs) {
        try {
            *fs << DataStream::ccu(m_accounts.size());
            for (auto &i : m_accounts) {
                *fs << i.first << i.second.low() << i.second.high();
            }
        } catch (DataStream::Exception &) {

        }
        fs->close();
    }
    m_accountsLock.unlock();

    m_ordersLock.lock();
    fs = m_fs->create("orders");
    if (fs) {
        try {
            *fs << DataStream::ccu(m_orders.size());
            for (auto &i : m_orders) {
                *fs << i.first << i.second.userId.low() << i.second.userId.high() << i.second.productId << i.second.price << (int)i.second.status;
            }
        } catch (DataStream::Exception &) {

        }
        fs->close();
    }
    m_ordersLock.unlock();

//    fs = m_fs->create("server");
//    if (fs) {
//        try {
//            m_serverData.save(*fs);
//        } catch(DataStream::Exception &) {
//
//        }
//        fs->close();
//    }
    fs = m_fs->create("server.var");
    if (fs) {
        Variable_object obj;
        m_serverData.save(obj);
        auto txt = VariableScript::generate(&obj);
        fs->write(txt.c_str(), txt.length());
        fs->close();
    }
}
void DC_DataBase::update(f32 deltaTime)
{
	//写入accounts ,orders,server.var 文件中
	/* m_timer += deltaTime;
	    if (m_timer >= 120.f) {
	        m_timer = 0;
	        if (m_dirty) {
	            m_dirty = false;
	            _flush();
	        }
	    }

	    static f32 s_log_timer = 0;
	    s_log_timer += deltaTime;
	    if (s_log_timer >= 2.f) {
	        s_log_timer = 0;
	        _flush_log();
	    }*/
}

void DC_DataBase::saveServerData(const ServerData &data)
{
	 m_serverData = data;
	  m_dirty = true;
}



bool DC_DataBase::loadServerData(ServerData &data)
{
	 data = m_serverData;
	  return true;
}


///////////////////////////////////////////////////////////日志
void DC_DataBase::AsyncLog(AsyncObjectPtr obj,UserEventLogPtr log,std::function<void(bool ret)> doResult)
{
	log->Asyncsave(obj,doResult);
}

void DC_DataBase::_log(DC_File_EventLog *el) {
    el->date = DateTime::now();
    std::lock_guard<decltype(m_logs_lock)> guard(m_logs_lock);
    m_logs.push_back(std::unique_ptr<DC_File_EventLog>(el));
}

void DC_DataBase::_log(DC_File_EventLog_User *el, const UUID &uuid) {
    el->userId.first = uuid.low();
    el->userId.second = uuid.high();
    _log(el);
}

void DC_DataBase::_flush_log() {
    decltype(m_logs) logs;
    m_logs_lock.lock();
    m_logs.swap(logs);
    m_logs_lock.unlock();


    if (logs.empty()) {
        return ;
    }

    c8 buf[64];
    Date date = Date::today();
    size_t len = sprintf(buf, "%04u_%02u_%02u.log", date.year, date.month + 1, date.day);

    str8 name {buf, len};
    auto fs = m_fs_log->open(name, false);
    if (!fs) {
        fs = m_fs_log->create(name);
        if (!fs) {
            return ;
        }
    } else {
        fs->seek(0, DataStream::kEnd);
    }

    try {
        for (auto &i : logs) {
            auto p = i.get();
            *fs << DataStream::ccu(p->type());
            p->save(*fs);
        }
    } catch (DataStream::Exception &) {

    }
    fs->close();
}
////////////////////////////////////////////////////////////////用户登录登出日志
void DC_DataBase::log_user_signin(const UUID &uuid) {
    //_log(vnnew DC_File_EventLog_User_Signin(), uuid);
}

void DC_DataBase::log_user_login(const UUID &uuid) {
   // _log(vnnew DC_File_EventLog_User_Login(), uuid);
}

void DC_DataBase::log_user_logout(const UUID &uuid) {
    //_log(vnnew DC_File_EventLog_User_Logout(), uuid);
}


///////////////////////////////////////////////////////////////RedeemPacket
bool DC_DataBase::addRedeemPacket(RedeemPacketPtr redeemPacket) {
    m_redeemPacketPacketsLock.lock_write();
    auto codeId = redeemPacket->codeId;
    auto ret = m_redeemPackets.insert({codeId, std::move(redeemPacket)});
    m_redeemPacketPacketsLock.unlock();
    return ret.second;
}

RedeemPacketPtr DC_DataBase::queryRedeemPacket(u32 codeId) {
    std::lock_guard<ReadWriteLock> guard(m_redeemPacketPacketsLock);
    (void)guard;
    auto it = m_redeemPackets.find(codeId);
    if (it == m_redeemPackets.end()) {
        return nullptr;
    }
    return it->second;
}

void DC_DataBase::removeRedeemPacket(u32 codeId, const UUID &uuid) {
    m_redeemPacketPacketsLock.lock_write();
    auto it = m_redeemPackets.find(codeId);
    if (it != m_redeemPackets.end()) {
        m_redeemPackets.erase(it);
    }
    m_redeemPacketPacketsLock.unlock();
}

str8 DC_DataBase::queryRedeemPacketCategoryName(u32 category) {
    std::lock_guard<ReadWriteLock> guard(m_redeemPacketCategoriesLock);
    (void)guard;
    auto it = m_redeemPacketCategories.find(category);
    if (it == m_redeemPacketCategories.end()) {
        return {};
    }
    return it->second;
}

void DC_DataBase::setRedeemPacketCategory(u32 category, const str8 &name) {
    m_redeemPacketCategoriesLock.lock_write();
    m_redeemPacketCategories[category] = name;
    m_redeemPacketCategoriesLock.unlock();
}

