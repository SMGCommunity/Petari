#pragma once

#include <revolution.h>

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