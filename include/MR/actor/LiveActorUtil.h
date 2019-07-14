#pragma once

#include "actor/LiveActor/LiveActor.h"

namespace MR
{
    bool isDead(const LiveActor *);
    bool isHiddenModel(const LiveActor *);
    void showModel(LiveActor *);
    void hideModel(LiveActor *);
    void hideModelAndOnCalcAnim(LiveActor *);
};