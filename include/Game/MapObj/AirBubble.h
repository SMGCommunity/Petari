#pragma once

#include "Game/LiveActor/LiveActor.h"

class AirBubble : public LiveActor {
public:
    AirBubble(const char *);

    virtual void init(const JMapInfoIter &);
    void appearMove(const TVec3f &, s32);

    TVec3f _8C;

    TVec3f _A4;

    f32 _B0;

    s32 _C4;
};