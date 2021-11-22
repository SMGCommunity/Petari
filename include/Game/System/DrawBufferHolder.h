#pragma once

#include "Game/Util.h"
#include "Game/System/DrawBufferGroup.h"

class LiveActor;

class DrawBufferHolder {
public:
    DrawBufferHolder(s32);

    void allocateActorListBuffer();

    DrawBufferGroup* mBufferGroup;  // _0
    s32 mBufferGroupCount;          // _4
    MR::Vector<MR::AssignableArray<DrawBufferGroup *> >* _8;
};