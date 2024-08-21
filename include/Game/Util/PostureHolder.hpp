#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class PostureHolder {
public:
    PostureHolder(const LiveActor *);

    void store(const LiveActor *);
    void restore(LiveActor *);

    TVec3f mScale;      // 0x0
    TVec3f mRotation;   // 0xC
    TVec3f mPosition;   // 0x18
};