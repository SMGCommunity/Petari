#pragma once

#include "JSystem/JGeometry/TVec.hpp"

class LiveActor;

class PostureHolder {
public:
    PostureHolder(const LiveActor* pActor);

    void store(const LiveActor* pActor);
    void restore(LiveActor* pActor);

public:
    /* 0x00 */ TVec3f mScale;
    /* 0x0C */ TVec3f mRotation;
    /* 0x18 */ TVec3f mPosition;
};
