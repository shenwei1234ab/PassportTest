//
//  BSCouponManager.h
//  optd_srv
//
//  Created by Wayne on 12/10/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__BSCouponManager__
#define __optd_srv__BSCouponManager__

#include "vnSingleton.h"
#include "vnRefCounted.h"

#include "../table/Table_coupon.h"

#include <vector>
#include <map>

using namespace vn;

class LoggedUser;

struct BSCoupon : AllocatedObject {
    enum Type {
        kCT_Hero = 1,
        kCT_Diamonds = 2,
        kCT_Items = 3
    };
    
    vn::u32 couponId = 0;
    bool minLevelGroup = false;
    vn::u32 minLevelId = 0;
    bool maxLevelGroup = false;
    vn::u32 maxLevelId = 0;
    vn::u32 weight = 0;	//权重
    vn::f32 validTime = 0;
    vn::u32 imageId = 0;
    vn::u32 subImageId = 0;
    vn::str8 title;
    vn::str8 desc;
    vn::u32 cost = 0;
    vn::u32 originalCost = 0;
    
    virtual ~BSCoupon() = default;
    virtual Type type() const = 0;
};

struct BSCoupon_Hero : BSCoupon {
	vn::u32 heroClassId = 0;
    Type type() const { return kCT_Hero; }
};

struct BSCoupon_Diamonds : BSCoupon {
	vn::u32 productId = 0;
    Type type() const { return kCT_Diamonds; }
};

struct BSCoupon_Items : BSCoupon {
    std::vector<std::pair<u32, u32>> items; // <id, count>
    Type type() const { return kCT_Items; }
};

class BSCouponManager : public Singleton<BSCouponManager> {
public:
    const BSCoupon * find(u32 couponId);
    const BSCoupon * find(LoggedUser *user, u32 couponId);
    
    const BSCoupon * generate(LoggedUser *user);
    
    void initData(const tbl::Table_coupon &table);
    
private:
    std::map<u32, std::unique_ptr<BSCoupon>> m_coupons;
};

#endif /* defined(__optd_srv__BSCouponManager__) */
