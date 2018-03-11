//
//  vnLexLR_Node.h
//  vnlib
//
//  Created by Wayne on 15/5/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnLexLR_Node_h
#define vnlib_vnLexLR_Node_h

#include "vnLexToken.h"
#include "vnLexLR_State.h"

_vn_begin

namespace lex {
    
    class LR_Node : public RefCounted {
    public:
        virtual bool isToken() const = 0;
        virtual TokenPtr token() const = 0;
        virtual u32 productionId() const = 0;
        virtual u32 numChildren() const = 0;
        virtual LR_Node * child(u32 index) const = 0;
    };
    typedef RefCountedPtr<LR_Node> LR_NodePtr;
    
    class LR_NodeToken : public LR_Node {
    public:
        LR_NodeToken(const TokenPtr &token) : m_token(token) {}
        LR_NodeToken(TokenPtr &&token) : m_token(token) {}
        virtual bool isToken() const { return true; }
        virtual TokenPtr token() const { return m_token; }
        virtual u32 productionId() const { return -1; }
        virtual u32 numChildren() const { return 0; }
        virtual LR_Node * child(u32 index) const { return 0; }
    private:
        TokenPtr m_token;
    };
    
    class LR_NodeNode : public LR_Node {
    public:
        LR_NodeNode(const LR_Reduction &reduction, LR_Node **children);
        virtual ~LR_NodeNode();
        virtual bool isToken() const { return false; }
        virtual TokenPtr token() const { return nullptr; }
        virtual u32 productionId() const { return m_productionId; }
        virtual u32 numChildren() const { return m_numChildren; }
        virtual LR_Node * child(u32 index) const { return m_children[index]; }
    private:
        u32 m_productionId;
        LR_Node **m_children;
        u32 m_numChildren;
    };

}

_vn_end

#endif
