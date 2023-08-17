#pragma once

#include <cstddef>

#define JGADGET_LINK_LIST(type, node) JGadget::TLinkList<type, -offsetof(type, node)>

namespace JGadget {
    template<typename T, int Offset>
    class TLinkList {
    public:
        struct iterator {
            iterator();
            const iterator& operator=(const iterator &);
            iterator& operator++();
            T* operator->() const;
            T* operator*() const;
            u32 _0;
        };
        iterator begin();
        iterator end();
        void Push_back(T *element) NO_INLINE {
            Insert(end(), element);
        }
        void Insert(iterator, JASTrack *);
        void Remove(T*);
    };
    class TLinkListNode {
    public:
        TLinkListNode();

        TLinkListNode *mPrev;
        TLinkListNode *mNext;
    };
}
