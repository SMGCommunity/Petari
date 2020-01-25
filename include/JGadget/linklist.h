#ifndef LINKLIST_H
#define LINKLIST_H

#include "types.h"

namespace JGadget
{
    class TLinkListNode
    {
    public:
        TLinkListNode();

        TLinkListNode* getNext() const;

        TLinkListNode* mNext; // _0
        u32 _4;
    };

    class TNodeLinkList
    {
    public:
    };
};

#endif // LINKLIST_H