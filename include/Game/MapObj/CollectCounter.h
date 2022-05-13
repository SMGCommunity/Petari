#pragma once

#include "Game/Screen/LayoutActor.h"
#include "JSystem/JGeometry.h"

class CollectCounter : public LayoutActor {
public:
    CollectCounter(const char *);

    virtual ~CollectCounter();
    virtual void init(const JMapInfoIter &);
    virtual void control();

    void setCount(s32);
    bool tryEndShow();
    void setPosition();

    TVec2f mFollowPosition; // _20
};

namespace NrvCollectCounter {
    NERVE(CollectCounterNrvHide);
    NERVE(CollectCounterNrvShow);   
};
