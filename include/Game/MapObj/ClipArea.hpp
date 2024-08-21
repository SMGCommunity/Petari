#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ClipAreaShape;

class ClipArea : public LiveActor {
public:
    ClipArea(const char *);

    TPos3f mBaseMatrix;             // 0x8C
    ClipAreaShape* mShape;          // 0xBC
};