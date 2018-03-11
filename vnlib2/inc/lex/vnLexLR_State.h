//
//  vnLexLR_State.h
//  vnlib
//
//  Created by Wayne on 15/5/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnLexLR_State_h
#define vnlib_vnLexLR_State_h

#include "../vnDefine.h"

_vn_begin

namespace lex {
    
    struct LR_Terminal {
        const c8 *name;
        u32 identity;
    };
    
    struct LR_Reduction {
        u32 numItems;
        u32 productionId;
        u32 nodeId;
    };
    
    struct LR_Jump {
        enum ConditionType {
            kNull = 0,
            kToken,
            kTokenWithValue,
            kNode,
        } cType;
        union {
            u32 nodeId;
            struct {
                u16 tokenType;
                u16 tokenValue;
            };
        };
        u32 targetId;
        
        LR_Jump() : cType(kNull), targetId(0) { nodeId = 0; }
        
        LR_Jump(unsigned int d, unsigned int i)
        : cType(kNode), targetId(i) {
            nodeId = d;
        }
        
        LR_Jump(ConditionType t, unsigned short tt, unsigned short tv, unsigned int i)
        : cType(t), targetId(i) {
            tokenType = tt;
            tokenValue = tv;
        }
    };
    
    struct LR_State {
        u32 sId;
        const LR_Jump *jumps;
        const LR_Reduction * reduction;
    };
}


_vn_end

#endif
