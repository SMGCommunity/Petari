#ifndef LINKLIST_H
#define LINKLIST_H

#include "types.h"

namespace nw4r
{
    namespace ut
    {
        class LinkListNode
        {
        public:
            LinkListNode* mNext; // _0
            LinkListNode* mPrev; // _4
        };

        namespace detail
        {
            template <typename T, typename Diff = PtrDiff, typename Pointer = T*, typename Reference = T&>
            class Iterator
            {
            public:
                typedef T val;
                typedef Diff diff;
                typedef Pointer ptr;
                typedef Reference ref;
            };

            class LinkListImpl
            {
                typedef LinkListImpl self;
                typedef u32 sizeType;
                typedef PtrDiff diff;
                typedef LinkListNode node;
                typedef node valType;
                typedef valType* ptr;
                typedef const valType* constPtr;
                typedef valType& ref;
                typedef const valType& constRef;

                class Iterator : public detail::Iterator<valType>
                {
                public:
                    ptr mPointer; // _0
                };

            public:
                ~LinkListImpl();
                Iterator Erase(Iterator);
                Iterator Insert(Iterator, ptr);
                Iterator Erase(ptr);
            };
        };
       
        template<typename T, PtrDiff offs>
        class LinkList { };
    };
};

#endif // LINKLIST_H