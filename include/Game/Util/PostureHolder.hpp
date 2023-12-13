#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class PostureHolder {
public:
    PostureHolder(const LiveActor *);

    void store(const LiveActor *);
    void restore(LiveActor *);

    TVec3f mScale;      // _0
    TVec3f mRotation;   // _C
    TVec3f mPosition;   // _18
};