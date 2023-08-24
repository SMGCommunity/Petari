#include "JSystem/JGadget/linklist.h"

namespace JGadget {
    inline const JGadget::TLinkListNode *getNode(const JGadget::TNodeLinkList::iterator &self) {
        return self.curr;
    }
    TNodeLinkList::~TNodeLinkList() {}
    void TNodeLinkList::splice(iterator a, TNodeLinkList &list, iterator b) {
        TLinkListNode *&curr = b.curr;
        TLinkListNode *&aCurr = a.curr;
        TLinkListNode *next = curr->mNext;
        bool flag = true;
        if(aCurr != curr && aCurr != next) flag = false;
        if(!flag) {
            TLinkListNode *nodeB = b.curr;
            list.Erase(nodeB);
            Insert(iterator(iterator::iteratorData(a.curr)), nodeB);
        }
    }
    TNodeLinkList::iterator::iteratorData TNodeLinkList::Insert
        (iterator iter, TLinkListNode *node) {
        TLinkListNode *next = iter.curr;
        TLinkListNode *prev = next->mPrev;
        node->mNext = next;
        node->mPrev = prev;
        next->mPrev = node;
        prev->mNext = node;
        mLen++;
        return iterator::iteratorData(node);
    }
    TLinkListNode* TNodeLinkList::Erase(TLinkListNode *node) {
        TLinkListNode *next = node->mNext, *prev = node->mPrev;
        next->mPrev = prev;
        prev->mNext = next;
        mLen--;
        return next;
    }
    void TNodeLinkList::Remove(TLinkListNode *node) {
        TLinkListNode *curr, *end, *tmpCurr, *&last = node;
        TNodeLinkList CALL_INLINE_FUNC_NO_ARG(tmp);
        tmpCurr = &tmp.mEnd;
        curr = mEnd.mNext;
        end = &mEnd;
        for( ;curr != end; ) {
            TLinkListNode *prev = curr;
            if(curr == last) {
                curr = curr->mNext;
                tmp.splice(iterator(tmpCurr, false), *this, iterator(prev, false));
            }
            else curr = curr->mNext;
        }
    }
    void TNodeLinkList::Initialize_() {
        mLen = 0;
        mEnd.mNext = &mEnd;
        mEnd.mPrev = &mEnd;
    }
}
