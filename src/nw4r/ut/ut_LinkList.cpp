#include "nw4r/ut/ut_LinkList.h"

namespace nw4r
{
    namespace ut
    {
        namespace detail
        {
            LinkListImpl::Iterator LinkListImpl::Erase(LinkListNode *pNode) 
            {
                LinkListNode* next = pNode->mNext;
                LinkListNode* prev = pNode->mPrev;

                next->mPrev = prev;
                prev->mNext = next;

                mCount--;
                pNode->mNext = nullptr;
                pNode->mPrev = nullptr;

                return Iterator(next);
            }
        };
    };
};