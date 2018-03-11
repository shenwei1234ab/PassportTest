//
//  BSAwardManager.cpp
//  optd_srv
//
//  Created by Wayne on 12/9/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#include "BSAwardManager.h"

#include <set>

void BSAwardManager::initData(const tbl::Table_award &table) {
    struct _ {
        static std::vector<std::pair<u32, u32>> _convert(const str8 &s) {
            std::vector<std::pair<u32, u32>> ret;
            int off = 0;
            auto p = s.c_str();
            std::pair<u32, u32> v;
            while (sscanf(p, "%u~%u%n", &v.first, &v.second, &off) >= 2 && off) {
                ret.push_back(v);
                p += off;
                if (*p == ';') {
                    ++p;
                }
                off = 0;
            }
            return std::move(ret);
        }
    };
    
    for (size_t i = 0, c = table.size(); i < c; ++i) {
        auto p = table.at(i);
        std::unique_ptr<BSAward> award(vnnew BSAward());
        award->awardId = p->f_AwardId.value;
        award->groupId = p->f_GroupId.value;
        award->poolWeight = p->f_PoolWeight.value;
        award->rank = p->f_Rank.value;
        award->weight = p->f_Weight.value;
        award->extraWeight = p->f_ExtraWeight.value;
        award->money = p->f_Money.value;
        award->diamonds = p->f_Diamonds.value;
        award->items = _::_convert(p->f_Items.value);
        award->desc = p->f_Desc.value;
        award->afterWin = _::_convert(p->f_AfterWin.value);
        award->imageId = p->f_ImageId.value;
        
        m_awards[p->f_AwardId.value] = std::move(award);
    }
}

std::vector<std::pair<const BSAward *, u32>> BSAwardManager::generate() {
    std::vector<std::pair<const BSAward *, u32>> ret;
    u32 poolWeight = 0;
    for (auto &i : m_awards) {
        auto p = i.second.get();
        if (!p->poolWeight) {
            continue;
        }
        poolWeight += p->poolWeight;
        ret.push_back({p, p->weight});
    }
    if (ret.size() > 8) {
        auto tmp = std::move(ret);
        do {
            u32 w = (u32)rand() % poolWeight;
            for (auto it = tmp.begin(); it != tmp.end(); ++it) {
                auto &p = *it;
                if (w < p.first->poolWeight) {
                    ret.push_back(p);
                    if (p.first->groupId) {
                        auto _t = std::move(tmp);
                        tmp.reserve(_t.size());
                        poolWeight = 0;
                        for (auto &i : _t) {
                            if (i.first->groupId != p.first->groupId) {
                                poolWeight += i.first->poolWeight;
                                tmp.push_back(i);
                            }
                        }
                    } else {
                        poolWeight -= p.first->poolWeight;
                        tmp.erase(it);
                    }
                    break;
                }
                w -= p.first->poolWeight;
            }
        } while (ret.size() < 8 && !tmp.empty());
    }
    return std::move(ret);
}

u32 BSAwardManager::draw(std::vector<std::pair<const BSAward *, u32>> &pool, const BSAward *&generated) {
    u32 total = 0;
    for (auto &p : pool) {
        total += p.second;
    }
    u32 w = (u32)rand() % total;
    u32 ret = 0;
    for (; ret < pool.size(); ++ret) {
        auto &p = pool[ret];
        if (w < p.second) {
            break;
        }
        w -= p.second;
    }
    auto award = pool[ret].first;
    total = 0;
    std::set<u32> gids;
    for (size_t idx = 0; idx < pool.size(); ++idx) {
        if (idx == ret) {
            continue;
        }
        auto &p = pool[idx];
        p.second += p.first->extraWeight;
        if (p.first->groupId) {
            gids.insert(p.first->groupId);
        }
    }
    std::vector<std::pair<const BSAward *, u32>> tmp;
    for (auto &i : award->afterWin) {
        auto it = m_awards.find(i.first);
        if (it == m_awards.end()) {
            continue;
        }
        auto p = it->second.get();
        if (p->groupId && gids.find(p->groupId) != gids.end()) {
            continue;
        }
        total += i.second;
        tmp.push_back({p, i.second});
    }
    if (tmp.empty()) {
        generated = nullptr;
    } else {
        w = (u32)rand() % total;
        for (auto &i : tmp) {
            if (w < i.second) {
                generated = i.first;
                break;
            }
            w -= i.second;
        }
    }
    return ret;
}

const BSAward * BSAwardManager::find(u32 awardId) {
    auto it = m_awards.find(awardId);
    if (it != m_awards.end()) {
        return it->second.get();
    }
    return nullptr;
}

