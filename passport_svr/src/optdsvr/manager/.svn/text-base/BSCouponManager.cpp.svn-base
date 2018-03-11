//
//  BSCouponManager.cpp
//  optd_srv
//
//  Created by Wayne on 12/10/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "BSCouponManager.h"
#include "../worker/LoggedUser.h"


const BSCoupon * BSCouponManager::find(u32 couponId) {
    auto it = m_coupons.find(couponId);
    if (it != m_coupons.end()) {
        return it->second.get();
    }
    return nullptr;
}




void BSCouponManager::initData(const tbl::Table_coupon &table) {
    for (size_t i = 0, c = table.size(); i < c; ++i) {
        auto p = table.at(i);
        std::unique_ptr<BSCoupon> coupon;
        switch (p->f_Type.value) {
            case BSCoupon::kCT_Hero: {
                u32 hero = 0;
                if (sscanf(p->f_Content.value.c_str(), "%u", &hero) && hero) {
                    auto pc = vnnew BSCoupon_Hero();
                    pc->heroClassId = hero;
                    coupon.reset(pc);
                }
                break;
            }
                
            case BSCoupon::kCT_Diamonds: {
                u32 productId = 0;
                if (sscanf(p->f_Content.value.c_str(), "%u", &productId) && productId) {
                    auto pc = vnnew BSCoupon_Diamonds();
                    pc->productId = productId;
                    coupon.reset(pc);
                }
                break;
            }
                
            case BSCoupon::kCT_Items: {
                std::vector<std::pair<u32, u32>> items;
                auto ptr = p->f_Content.value.c_str();
                std::pair<u32, u32> t;
                int offset = 0;
                while (sscanf(ptr, "%u~%u%n", &t.first, &t.second, &offset) && offset) {
                    ptr += offset;
                    offset = 0;
                    if (*ptr == ';') {
                        ++ptr;
                    }
                    if (t.first && t.second) {
                        items.push_back(t);
                    }
                }
                if (!items.empty()) {
                    auto pc = vnnew BSCoupon_Items();
                    pc->items = std::move(items);
                    coupon.reset(pc);
                }
                break;
            }
        }
        if (coupon) {
            coupon->couponId = p->f_CouponId.value;
            coupon->minLevelGroup = (p->f_MinLevelType.value ? true : false);
            coupon->minLevelId = p->f_MinLevelId.value;
            coupon->maxLevelGroup = (p->f_MaxLevelType.value ? true : false);
            coupon->maxLevelId = p->f_MaxLevelId.value;
            coupon->weight = p->f_Weight.value;
            coupon->validTime = p->f_Time.value;
            coupon->imageId = p->f_ImageId.value;
            coupon->subImageId = p->f_SubImageId.value;
            coupon->title = p->f_Title.value;
            coupon->desc = p->f_Desc.value;
            coupon->cost = p->f_Cost.value;
            coupon->originalCost = p->f_OriginalCost.value;
            m_coupons[p->f_CouponId.value] = std::move(coupon);
        }
    }
}


const BSCoupon * BSCouponManager::find(LoggedUser *user, u32 couponId) {
    auto it = m_coupons.find(couponId);
    if (it == m_coupons.end()) {
        return nullptr;
    }
    auto p = it->second.get();
    if (p->type() == BSCoupon::kCT_Hero) {
        auto ph = static_cast<BSCoupon_Hero *>(p);
        if (user->bsHero().unlocked(ph->heroClassId)) {
            return nullptr;
        }
    }
    return p;
}

const BSCoupon * BSCouponManager::generate(LoggedUser *user) {
    std::vector<BSCoupon *> coupons;
    coupons.reserve(m_coupons.size());
    u32 totalWeight = 0;
    for (auto &i : m_coupons) {
        auto p = i.second.get();
        if (!p->weight) {
            continue;
        }
        if (p->minLevelId) {
            if (p->minLevelGroup) {
                if (!user->bsLevelGroupMgr().passed(p->minLevelId)) {
                    continue;
                }
            } else {
                if (!user->bsLevel().passed(p->minLevelId)) {
                    continue;
                }
            }
        }
        if (p->maxLevelId) {
            if (p->maxLevelGroup) {
                if (user->bsLevelGroupMgr().passed(p->maxLevelId)) {
                    continue;
                }
            } else {
                if (user->bsLevel().passed(p->maxLevelId)) {
                    continue;
                }
            }
        }
        if (p->type() == BSCoupon::kCT_Hero) {
            auto ph = static_cast<BSCoupon_Hero *>(p);
            if (user->bsHero().unlocked(ph->heroClassId)) {
                continue;
            }
        }
        totalWeight += p->weight;
        coupons.push_back(p);
    }
    if (coupons.empty()) {
        return nullptr;
    }
    u32 v = (u32)rand() % totalWeight;
    for (auto p : coupons) {
        if (v < p->weight) {
            return p;
        }
        v -= p->weight;
    }
    return nullptr;
}

