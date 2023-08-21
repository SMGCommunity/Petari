#include "JSystem/JGadget/linklist.h"

namespace JGadget {
    void TNodeLinkList::Initialize_() {
        _0 = 0;
        mEnd.mNext = &mEnd;
        mEnd.mPrev = &mEnd;
    }
}
