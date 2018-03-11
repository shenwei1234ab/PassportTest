//
//  BSAwardManager.h
//  optd_srv
//
//  Created by Wayne on 12/9/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__BSAwardManager__
#define __optd_srv__BSAwardManager__

#include "vnSingleton.h"

#include "../table/Table_award.h"

#include <map>
#include <memory>

using namespace vn;

struct BSAward : AllocatedObject {
    u32 awardId = 0;
    u32 groupId = 0;
    u32 poolWeight = 0;
    u32 rank = 0;
    u32 weight = 0;
    u32 extraWeight = 0;
    s32 money = 0;
    s32 diamonds = 0;
    std::vector<std::pair<u32, u32>> items; // <id, count>
    str8 desc;
    std::vector<std::pair<u32, u32>> afterWin; // <id, weight>
    u32 imageId = 0;
};

class BSAwardManager : public Singleton<BSAwardManager> {
public:
    void initData(const tbl::Table_award &table);
    
    std::vector<std::pair<const BSAward *, u32>> generate();

    u32 draw(std::vector<std::pair<const BSAward *, u32>> &pool, const BSAward *&generated);
    
    const BSAward * find(u32 awardId);
    
private:
    std::map<u32, std::unique_ptr<BSAward>> m_awards;
};

#endif /* defined(__optd_srv__BSAwardManager__) */
