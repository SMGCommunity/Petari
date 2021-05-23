#include "nw4r/ut/ut_LinkList.h"

namespace nw4r
{
    namespace ut
    {
        namespace detail
        {
            LinkListImpl::Iterator LinkListImpl::Erase(LinkListImpl::Iterator iter)
            {
                LinkListImpl::Iterator nextIter = iter;
                ++nextIter;

                LinkListNode* node = iter.mNode;
                LinkListNode* lastNode = nextIter.mNode;
                LinkListNode* nextNode;

                // this code is very close
                for ( ; node != lastNode; node = nextNode)
                {
                    nextNode = node->mNext;
                    
                    LinkListNode* next = node->mNext;
                    LinkListNode* prev = node->mPrev;

                    next->mPrev = prev;
                    prev->mNext = next;

                    mCount--;

                    nextNode->mNext = 0;
                    nextNode->mPrev = 0;
                }

                return nextIter;
            }

            LinkListImpl::Iterator LinkListImpl::Insert(LinkListImpl::Iterator iter, LinkListNode *pNode)
            {
                LinkListNode* node = iter.mNode;
                LinkListNode* prevNode = node->mPrev;

                pNode->mNext = node;
                pNode->mPrev = prevNode;

                node->mPrev = pNode;
                prevNode->mNext = pNode;

                mCount++;
                return Iterator(pNode);
            }

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
