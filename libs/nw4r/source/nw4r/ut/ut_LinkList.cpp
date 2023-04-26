#include "nw4r/ut/LinkList.h"

namespace nw4r {
    namespace ut {
        namespace detail {

            LinkListImpl::~LinkListImpl() {
                Clear();
            }

            LinkListImpl::Iterator LinkListImpl::Erase(Iterator it) {
                Iterator itNext = it;
                ++itNext;
                return Erase(it, itNext);
            }

            LinkListImpl::Iterator LinkListImpl::Erase(Iterator itFirst, Iterator itLast) {
                Node* pIt = itFirst.mPointer;
                Node* pItLast = itLast.mPointer;
                Node* pNext;

                for (; pIt != pItLast; pIt = pNext) {
                    pNext = pIt->mNext;
                    Erase(pIt);
                }

                return itLast;
            }

            void LinkListImpl::Clear() {
                Erase(GetBeginIter(), GetEndIter());
            }

            LinkListImpl::Iterator LinkListImpl::Insert(Iterator it, pointer p) {
                Node* pIt = it.mPointer;
                Node* pItPrev = pIt->mPrev;

                p->mNext = pIt;
                p->mPrev = pItPrev;

                pIt->mPrev = p;
                pItPrev->mNext = p;
                mSize++;

                return Iterator(p);
            }

            LinkListImpl::Iterator LinkListImpl::Erase(pointer p) {
                Node* pNext = p->mNext;
                Node* pPrev = p->mPrev;

                pNext->mPrev = pPrev;
                pPrev->mNext = pNext;
                mSize--;

                p->mNext = nullptr;
                p->mPrev = nullptr;

                return Iterator(pNext);
            }
        };
    };
};