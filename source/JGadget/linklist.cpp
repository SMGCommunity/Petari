#include "JGadget/linklist.h"

namespace JGadget
{
    TLinkListNode::TLinkListNode()
    {
        mNext = 0;
        _4 = 0;
    }

    TLinkListNode* TLinkListNode::getNext() const
    {
        return mNext;
    }
};