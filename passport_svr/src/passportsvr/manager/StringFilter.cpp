//
//  StringFilter.cpp
//  vntest
//
//  Created by Wayne on 9/26/15.
//  Copyright © 2015 viichi.com. All rights reserved.
//

#include "../../passportsvr/manager/StringFilter.h"

#include "vnUnicode.h"

void StringFilter::add(const c8 *p, size_t size) {
    str32 ret;
    Unicode::convert(p, size, ret);
    if (!ret.empty()) {
        add(ret.c_str(), ret.length());
    }
}

void StringFilter::add(const c32 *p, size_t size) {
    Node *node = &m_root;
    for (; size && *p; --size, ++p) {
        c32 c = *p;
        if (c >= 'A' && c <= 'Z') {
            c = c - 'A' + 'a';
        }
        node = &node->nodes[c];
        node->chr = c;
    }
    node->null = true;
}



void StringFilter::clear() {
    m_root.nodes.clear();
}

bool StringFilter::check(const c32 *p, size_t size) const {
    for (;size && *p; --size, ++p) {
        auto q = p;
        auto m = size;
        auto node = &m_root;
        do {
            c32 c = *q;
            if (c >= 'A' && c <= 'Z') {
                c = c - 'A' + 'a';
            }
            auto it = node->nodes.find(c);
            if (it == node->nodes.end()) {
                break;
            }
            node = &it->second;
            if (node->null) {
                return false;
            }
        } while (--m && *++q);
    }
    return true;
}

bool StringFilter::check(const c8 *p, size_t size) const {
    str32 t;
    Unicode::convert(p, size, t);
    return check(t.c_str(), t.length());
}

//str32 StringFilter::filter(const c32 *p, size_t size, c32 replace) const {
//    
//}

//str8 StringFilter::filter(const c8 *p, size_t size, c32 replace) const {
//    
//}