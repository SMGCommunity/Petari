#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ClipAreaShape;

class ClipArea : public LiveActor {
public:
    ClipArea(const char *);

    TPos3f mBaseMatrix;             // _8C
    ClipAreaShape* mShape;          // _BC
};