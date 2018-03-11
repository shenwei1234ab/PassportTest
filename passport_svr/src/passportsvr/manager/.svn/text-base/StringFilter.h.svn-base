//
//  StringFilter.h
//  vntest
//
//  Created by Wayne on 9/26/15.
//  Copyright © 2015 viichi.com. All rights reserved.
//

#ifndef StringFilter_h
#define StringFilter_h

#include "vnAllocatedObject.h"

using namespace _vn_ns;

#include <map>

class StringFilter {
public:
    
    void add(const c32 *p, size_t size);
    void add(const c8 *p, size_t size);
    void clear();
    
    bool check(const c32 *p, size_t size) const;
    bool check(const c8 *p, size_t size) const;
    
    //str32 filter(const c32 *p, size_t size, c32 replace) const;
    //str8 filter(const c8 *p, size_t size, c32 replace) const;
    
private:
    
    struct Node : AllocatedObject {
        c32 chr = 0;
        std::map<c32, Node> nodes;
        bool null = false;
    };
    
    Node m_root;
    
};

#endif /* StringFilter_hpp */
