//
//  vnLexLR_Node.cpp
//  vnlib
//
//  Created by Wayne on 15/5/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnLexLR_Node.h"

#include <cstring>

_vn_begin

namespace lex {
    
    LR_NodeNode::LR_NodeNode(const LR_Reduction &reduction, LR_Node **children)
    : m_productionId(reduction.productionId)
    , m_numChildren(reduction.numItems) {
        m_children = vnmalloc(LR_Node *, m_numChildren);
        memcpy(m_children, children, m_numChildren * sizeof(LR_Node *));
    }
    
    LR_NodeNode::~LR_NodeNode() {
        for (u32 i = 0; i < m_numChildren; ++i) {
            m_children[i]->drop();
        }
        vnfree(m_children);
    }
    
}

_vn_end
