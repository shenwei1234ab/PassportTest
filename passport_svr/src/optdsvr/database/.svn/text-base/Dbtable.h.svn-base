#ifndef __optd_srv__DBTABLE__
#define __optd_srv__DBTABLE__

#include "../worker/BSHero.h"
#include <set>
#include "vnDataStream.h"
#include "vnVariable.h"
#include <list>
#include <vector>


class DBTable_baseinfo
{
public:
	void Init()
	{

	}
	DBTable_baseinfo()
	{
		chip[0] = 0;
		chip[1] = 0;
		chip[2] = 0;
	}
	vn::s32 chip[3];
	vn::s32 money=0;
	vn::s32 diamonds=0;
	vn::u32 purchasedDiamonds=0;
	vn::u32 stamp=0;
	vn::s32 coupon_id=0;
	float coupon_timer=0;
	vn::s32 shovel0=0;
	vn::s32 shovel1=0;
	vn::s32 skull0=0;
	vn::s32 skull1=0;
	vn::s32 watch0=0;
	vn::s32 watch1=0;
	bool running=false;
	vn::f32 timer=0;
	vn::s32 count=0;
	std::string first_date="0-1-0";
	vn::s32 group	=0;
	std::string last_date="0-1-0";
	vn::s32 lottery_draw=0;
	vn::s32 lottery_state=0;
	vn::f32 lottery_timer=0;
};


class DBTable_baseinfo_forceguide
{
public:
	void Init()
	{
		guideids.clear();
	}
	std::set<vn::u32> guideids;
};

class DBTable_Hero
{
public:
	void Init()
	{
		heros.clear();
		heros = BSHero::s_initialHeroList;
	}
	std::vector<BSHero::HeroInfo> heros;
};

class DBTable_ItemStore
{
public:
	void Init()
	{

	}


};



#endif /* defined(__optd_srv__BSForceGuide__) */
