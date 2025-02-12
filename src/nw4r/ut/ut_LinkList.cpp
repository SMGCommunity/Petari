#include "revolution/types.h"
#include "nw4r/ut/LinkList.h"

namespace nw4r {
    namespace ut {
        namespace detail {
            void LinkListImpl::Clear() {
                Erase(GetBeginIter(), GetEndIter());
            }

            LinkListImpl::Iterator LinkListImpl::Insert(Iterator it,pointer p)
            {
                Node *const pIt=it.mPointer;
                Node *const pItPrev=pIt->mPrev;
                p->mNext=pIt;
                p->mPrev=pItPrev;

                pIt->mPrev=p;
                pItPrev->mNext=p;

                ++mSize;
                return Iterator(p);
            }

            LinkListImpl::Iterator LinkListImpl::Erase(pointer p)
            {
                Node *const pNext=p->mNext,*const pPrev=p->mPrev;
                pNext->mPrev=pPrev;
                pPrev->mNext=pNext;
                --mSize;

                p->mNext = nullptr;
                p->mPrev = nullptr;
                
                return Iterator(pNext);
            }

            LinkListImpl::Iterator LinkListImpl::Erase(Iterator itFirst,Iterator itLast) {
                Node *pIt=itFirst.mPointer,* const pItLast = itLast.mPointer,*pNext;
                for (; pIt != pItLast; pIt=pNext) {
                    pNext = pIt->mNext;
                    Erase(pIt);
                }

                return itLast;
            }

            LinkListImpl::~LinkListImpl() {
                Clear();
            }

                    
            LinkListImpl::Iterator LinkListImpl::Erase(Iterator it) {
                Iterator itNext=it;
                (void)++itNext;
                return Erase(it,itNext); 
            }
        };
    };
};
