//
//  vnLinkBase.h
//  vnlib
//
//  Created by Wayne on 4/28/15.
//  Copyright (c) 2015 viichi.com. All rights reserved.
//

#ifndef __vnlib__vnLinkBase__
#define __vnlib__vnLinkBase__

#include "vnLinkPool.h"
#include "vnSpinLock.h"

_vn_begin

namespace internal {


    class LinkBaseHolder;
    typedef RefCountedPtr<LinkBaseHolder> LinkBaseHolderPtr;

    class LinkBase : public RefCounted {
    public:
        LinkBase(RefCountedPtr<LinkPool> pool);
        virtual ~LinkBase();
        
        int sockfd() const { return m_sockfd; }
        void set_nonblocking();
        bool bind(const NetAddress &addr);
        void post(std::function<void()> fn);

        virtual void ack(bool read, bool write, bool error) {}

        LinkBaseHolder * holder() const { return m_holder.get(); }
    protected:
        int m_sockfd = -1;
        RefCountedPtr<LinkPool> m_pool;

        LinkBaseHolderPtr m_holder;
    };

    class LinkBaseHolder : public RefCounted {
    public:
        explicit LinkBaseHolder(LinkBase *link);

        bool ack(bool read, bool write, bool error);
        void detach();
    private:
        LinkBase *m_link;
        SpinLock m_lock;
    };
    
} // namespace internal

_vn_end

#endif /* defined(__vnlib__vnLinkBase__) */
