//
//  BSLevelGroup.h
//  optd_srv
//
//  Created by Wayne on 11/7/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__BSLevelGroup__
#define __optd_srv__BSLevelGroup__

#include "BSLevelGroupInfo.h"
#include "vnVariable.h"
#include "AsyncTask.h"
#include <array>
#include <map>
#include <set>
#include "BSTool.h"
#include "../../db/QueryResult.h"
#include "../../passportsvr/entity/AsyncObject.h"
using namespace vn;

class BSLevelGroup : public AllocatedObject {
public:
    BSLevelGroup(const BSLevelGroupInfo *_info);
    
    const BSLevelGroupInfo *info;
    
    struct LevelInfo {
        LevelInfo() = default;
        LevelInfo(const tbl::Record_level *_l) : level(_l) {}
        
        const tbl::Record_level *level = 0;
        
        u8 passed = 0; // 0, not passed. 1, passed. 2, intrusion.
        u32 stars = 0;
        bool watch = false;
        bool shovel = false;
        bool resolved = true;
        void 	ConvertStrToHeroArray(std::string &strhero)
		{
				heros.clear();
				std::vector<std::string> dest ;
				StrSplit(strhero,",",dest);
				for(uint i=0;i<dest.size();++i)
				{
					heros.push_back(ConvertStrToU32(dest[i]));
				}
		}

        void ConvertStrToPlatforms(std::string &strPlat)
        {
        	platforms.clear();
        	std::vector<std::string> pairVec;
        	StrSplit(strPlat,"|",pairVec);
        	for(uint i=0;i<pairVec.size();++i)
			{
        		std::vector<std::string>	pairs;
        		StrSplit(pairVec[i],",",pairs);
        		platforms.push_back({ConvertStrToU32(pairs[0]),ConvertStrToU32(pairs[1])});
			}
        }

        std::vector<u32> heros;
        std::vector<std::pair<u32, u32>> towers;
        std::vector<std::pair<u32, u32>> platforms;
    };
    
    std::map<u32, LevelInfo> levels;

    bool passed = false;
    bool starsCompleted = false;
    bool watchCompleted = false;
    bool shovelCompleted = false;
    size_t rewardsCount = 0;
    
    u32 intrusion = 0;
    u32 intrusionTimes = 0;
    
    LevelInfo * win(u32 levelId, u32 stars, bool watch, bool shovel, const std::vector<u32> &heros, const std::vector<std::pair<u32, u32>> &towers, const std::vector<std::pair<u32, u32>> &platforms, bool &firstVictory, size_t &firstReward);
    LevelInfo * reset(u32 levelId);
    
    void refresh();
};

class LoggedUser;

class BSLevelGroupManager : public AllocatedObject {
public:
    BSLevelGroupManager(LoggedUser *user) : m_user(user) {}
    
    const std::map<u32, BSLevelGroup> & groups() { return m_groups; }

    void save(DataStream &stream) const;
    void load(u8 version, DataStream &stream);
    
    void save(Variable_object &vobj) const;
    void AsyncsaveNew(u32 levelId,std::function<void(int ret)>doResult);
    void	_AsyncsaveNewHandl(AsyncTaskPtr task,std::function<void(int ret)>doResult);
    void Asyncsave(u32 levelId,std::function<void(int ret)>doResult);
    void load(Variable_object &vobj);
    void	load(PreparedQueryResult groupResult);

    bool win(u32 levelId, u32 stars, bool watch, bool shovel, const std::vector<u32> &heroes, const std::vector<std::pair<u32, u32>> &towers, const std::vector<std::pair<u32, u32>> &platforms, bool *intrusion = 0,bool *ifNew=0,vn::s32 *pAddDiamond=0);
    //void Asyncwin(u32 levelId, u32 stars, bool watch, bool shovel, const std::vector<u32> &heroes, const std::vector<std::pair<u32, u32>> &towers, const std::vector<std::pair<u32, u32>> &platforms, bool *intrusion,std::function<void(int ret)> doResult);
    void	_AsyncInsertLevelToGroupHandl(bool ret,u32 groupId,u32 levelId,AsyncTaskPtr task,std::function<void(int ret)> doResult);

    bool reset(u32 levelId);
    
    void intrude(u32 groupId);
    void Asyncintrude(AsyncObjectPtr asyncObjPtr,u32 groupId,std::function<void(bool ret)> doResult);
    bool Syncintrude(u32 groupId);
    void _AsyncintrudeHandl(bool ret,AsyncTaskPtr task,std::function<void(bool ret)> doResult,u32 groupId,u32 levelId,u32 mineId);
    void clear();
    
    bool passed(u32 groupId);
    bool intrusion(u32 groupId);
    
    u32 checkGroupStatus(u32 groupId);
    
private:
    LoggedUser *m_user;
    std::map<u32, BSLevelGroup> m_groups;
};

#endif /* defined(__optd_srv__BSLevelGroup__) */
