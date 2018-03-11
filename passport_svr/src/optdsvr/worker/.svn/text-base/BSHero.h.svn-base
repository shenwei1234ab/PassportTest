//
//  BSHero.h
//  optd_srv
//
//  Created by Cloverz on 9/2/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__BSHero__
#define __optd_srv__BSHero__

#include <vector>

#include "vnAllocatedObject.h"
#include "vnDataStream.h"
#include "vnVariable.h"
#include "../database/PassPortDatabaseConnection.h"
using namespace vn;
class LoggedUser;
class BSHero
{
public:
    enum Status
    {
        kNotOpen = 0,
        kLock,
        kUnLock,
        kMaxLv
    };

    struct Rune : public AllocatedObject
    {
        Rune() : id(0), status(kNotOpen) {}

        u32 id;
        Status status;
    };

    struct HeroInfo : public AllocatedObject
    {
        HeroInfo() : id(0), towerClass(0), unlockRes(0), status(kNotOpen) {}
        HeroInfo(const HeroInfo &) = default;
        HeroInfo(HeroInfo &&other)
        : id(other.id)
        , towerClass(other.towerClass)
        , unlockRes(other.unlockRes)
        , status(other.status)
        , runeList(std::move(other.runeList)) {

        }

        HeroInfo & operator =(const HeroInfo &) = default;

        u32 id;
        u32 towerClass;
        u32 unlockRes;
        Status status;

        std::vector<Rune> runeList;
    };

    BSHero(LoggedUser *);
    ~BSHero();
    void init();
    void load(u32 version, DataStream &stream);
    void save(DataStream &stream) const;
    
    void save(Variable_object &vobj) const;
    void load(Variable_object &vobj);

    void load(PreparedQueryResult baseinfo);
    void load(HeroInfo &heroinfo);
    void Asyncsave(u32 idx,std::function<void(bool ret)>doResult);
    void AsyncsaveRune(u32 idx, u32 runeIdx,std::function<void(bool ret)>doResult);
    void open_hero(); //Calc by self, no msg.
    bool upgrade_hero(u32 idx, bool useCoupon,s32 *payDiamond);
    void Asyncupgrade_hero(u32 idx, bool useCoupon,std::function<void(int ret)>doResult);
    bool unlock_rune(u32 idx, u32 runeIdx);
    void  Asyncunlock_rune(u32 idx, u32 runeIdx,std::function<void(int ret)>doResult);

    const u32 * skull();
    const u32 * shovel();
    const u32 * watch();
    const std::vector<HeroInfo> & heroList();

    bool unlocked(u32 heroClassId);
    
    static bool loadInitalHeroList(u32 fsId, const str8 &fileName);


    bool  SyncOpenHero();
    bool SyncOpenRune(std::vector<Rune> &runeList, u32 towerClass, u32 lv);
private:
    void _open_rune(std::vector<Rune> &runeList, u32 towerClass, u32 lv);
    void _update_rune_res();                 //Calc by self, no msg.
    
    u32 m_skull[2]; // 0=used 1=total
    u32 m_shovel[2];
    u32 m_watch[2];
    LoggedUser *m_user;
    std::vector<HeroInfo> m_heroList;
public:
    static decltype(m_heroList) s_initialHeroList;
};

#endif /* defined(__optd_srv__BSHero__) */
