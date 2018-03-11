//
//  vnLexLR_Parser.h
//  vnlib
//
//  Created by Wayne on 15/5/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnLexLR_Parser_h
#define vnlib_vnLexLR_Parser_h

#include "vnLexLR_Node.h"
#include <vector>

_vn_begin

namespace lex {

    class LR_Parser : public RefCounted {
    public:
        enum Result {
            kFailed = 0,
            kNext,
            kAccept
        };
        LR_Parser(const LR_State *state0);
        virtual ~LR_Parser();
        Result input(const TokenPtr &token);
        bool accepted() const;
        const LR_NodePtr & result() const;
        void reset();
        
    private:
        typedef std::vector<LR_Node *> NodeStack;
        typedef std::vector<const LR_State *> StateStack;
        NodeStack m_nodes;
        StateStack m_states;
        LR_NodePtr m_result;
        const LR_State *m_state0;
        
        void _clear();
        void _jump(u32 target, LR_Node *node);
        bool _push(const TokenPtr &token);
    };

}

_vn_end



#endif
