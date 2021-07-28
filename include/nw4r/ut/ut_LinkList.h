#pragma once

#include "types.h"

namespace nw4r
{
    namespace ut
    {
        class LinkListNode
        {
        public:
            LinkListNode()
                : mNext(nullptr), mPrev(nullptr)
            { }

            LinkListNode* GetNextNode() const
            {
                return mNext;
            }

            LinkListNode* GetPreviousNode() const
            {
                return mPrev;
            }

            LinkListNode* mNext; // _0
            LinkListNode* mPrev; // _4
        };

        namespace detail
        {
            class LinkListImpl
            {
            public:

                ~LinkListImpl();

                class Iterator
                {
                public:
                    Iterator(LinkListNode *pNode) : mNode(pNode) { }

                    Iterator &operator++()
                    {
                        mNode = mNode->GetNextNode();
                        return *this;
                    }

                    Iterator operator++(int)
                    {
                        Iterator iter(*this);
                        ++*this;
                        return iter;
                    }

                    LinkListNode* mNode; // _0
                };

                Iterator Erase(Iterator);
                Iterator Insert(Iterator, LinkListNode *);
                Iterator Erase(LinkListNode *);

                u32 mCount; // _0
                LinkListNode mNodes; // _4
            };
        };
    };
};