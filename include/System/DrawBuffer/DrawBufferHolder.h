#pragma once

#include <revolution.h>
#include "System/DrawBuffer/DrawBufferGroup.h"

class LiveActor;

class DrawBufferHolder
{
public:
    DrawBufferHolder();

    void registerDrawBuffer(LiveActor *, s32);

    void allocateActorListBuffer();

    DrawBufferGroup* mGroups; // _0
};