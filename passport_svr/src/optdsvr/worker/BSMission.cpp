//
//  BSMission.cpp
//  optd_srv
//
//  Created by Cloverz on 2/28/15.
//  Copyright (c) 2015 longsgoo.com. All rights reserved.
//

#include "BSMission.h"
#include "vnVariableAccessor.h"
#include "../manager/DC_DataBase.h"
void BSMission::save(DataStream &stream) const {
    stream << DataStream::ccu(m_rewards.size());
    for (auto &i : m_rewards) {
        stream << DataStream::ccu(i.first);
        stream << i.second;
    }
}

void BSMission::load(u8 version, DataStream &stream) {
    
    for (auto &i : m_rewards) {
        i.second = false;
    }
    
    size_t size;
    stream >> DataStream::cu(size);
    for (size_t i = 0; i < size; ++i) {
        u32 missionId;
        stream >> DataStream::cu(missionId);
        auto it = m_rewards.find(missionId);
        if (it == m_rewards.end()) {
            throw DataStream::Exception();
        }
        stream >> it->second;
    }
}

void BSMission::save(Variable_object &vobj) const {
    auto p = vnnew Variable_object();
    vobj.setAttribute("mission", p);
    for (auto &i : m_rewards) {
        p->add(vnnew Variable_int32(i.first));
        p->add(vnnew Variable_bool(i.second));
    }
}

//update
void BSMission::Asyncsave(u32 missionId,std::function<void(bool ret)>doResult)
{
	auto iter = m_rewards.find(missionId);
	if(iter == m_rewards.end())
	{
		return doResult(false);
	}
	bool missionStatus = iter->second;
	std::string strUUId = m_user->GetUUId();
	((DC_DataBase *)&DataCenter::instance())->AsyncUpdateMission({m_user,true},strUUId,missionId,missionStatus,doResult);
}

void BSMission::load(Variable_object &vobj) {
    VariableAccessor<Variable_int32, Variable_bool> va(vobj.queryAttributeObject("mission"));
    while (va.fetch()) {
        auto it = m_rewards.find(va.at<0>()->value());
        if (it != m_rewards.end()) {
            it->second = va.at<1>()->value();
        } else {
            it->second = false;
        }
    }
}

void BSMission::load(PreparedQueryResult result )
{
	do
	{
		  u32 missionId = (*result)[0].GetUInt32();
		  bool missionstatus = (*result)[1].GetBool();
		  auto it = m_rewards.find(missionId);
		 if (it != m_rewards.end())
		 {
			 it->second = missionstatus;
		 }
		 else
		 {
			 it->second = false;
		 }
	}while(result->NextRow());
}

void BSMission::reset() {
    for (auto &it:m_rewards) {
        it.second = false;
    }
}

void BSMission::setReward(u32 id ,bool b) {
    std::map<u32, bool>::iterator it = m_rewards.find(id);
    if (it != m_rewards.end()) {
        (*it).second = b;
    }
}


void BSMission::AsyncsetReward(u32 missionId ,bool missionStatus,std::function<void(bool ret)> doResult)
{
    std::map<u32, bool>::iterator it = m_rewards.find(missionId);
   if(it == m_rewards.end())
   {
	   return doResult(true);
   }
  (*it).second = missionStatus;
  std::string strUUId= m_user->GetUUId();
  ((DC_DataBase *)&DataCenter::instance())->AsyncUpdateMission({m_user,true},strUUId,missionId,missionStatus,[doResult,this,missionId,missionStatus](bool ret)
  {
	  if(!ret)
	  {
		  std::string strUUId = m_user->GetUUId();
		  VN_LOG_ERROR("uuid:"<<strUUId<<"missionid"<<missionId<<"missionstatus"<<missionStatus<<"update failed");
	  }
	  doResult(ret);
  });

}






