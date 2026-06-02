#include "JSystem/JGadget/linklist.hpp"
#include "JSystem/JGadget/predicate.hpp"

namespace JGadget {

    inline const JGadget::TLinkListNode* getNode(const JGadget::TNodeLinkList::iterator& self) {
        return self.curr;
    }

    TNodeLinkList::~TNodeLinkList() {
    }

    void TNodeLinkList::splice(iterator a, TNodeLinkList& list, iterator b) {
        TLinkListNode*& curr = b.curr;
        TLinkListNode*& aCurr = a.curr;
        TLinkListNode* next = curr->mNext;
        bool flag = true;
        if (aCurr != curr && aCurr != next)
            flag = false;
        if (!flag) {
            TLinkListNode* nodeB = b.curr;
            list.Erase(nodeB);
            Insert(iterator(a.curr), nodeB);
        }
    }

    TNodeLinkList::iterator TNodeLinkList::Insert(iterator iter, TLinkListNode* node) {
        TLinkListNode* next = iter.curr;
        TLinkListNode* prev = next->mPrev;
        node->mNext = next;
        node->mPrev = prev;
        next->mPrev = node;
        prev->mNext = node;
        mLen++;
        return iterator(node);
    }

    TNodeLinkList::iterator TNodeLinkList::Erase(TLinkListNode* node) {
        TLinkListNode *next = node->mNext, *prev = node->mPrev;
        next->mPrev = prev;
        prev->mNext = next;
        mLen--;
        return iterator(next);
    }

    void TNodeLinkList::Remove(TLinkListNode* node) {
        // FIXME: inline moment
        remove_if(TPRIsEqual_pointer_< TLinkListNode >(node));
    }
}  // namespace JGadget
