#pragma once

#include "Game/Util.h"
#include "Game/System/DrawBufferGroup.h"

class LiveActor;

struct DrawBufferInitialTable {
    s32 mDrawBufferType;
    s32 mCapacity;
    u32 _8;
    s32 mDrawCameraType;
};

class DrawBufferHolder {
public:
    DrawBufferHolder(s32);
    DrawBufferHolder();

    void initTable(const DrawBufferInitialTable *, s32);

    void allocateActorListBuffer();
    void active(LiveActor *, s32, s32);
    s16 registerDrawBuffer(LiveActor *, s32);
    void deactive(LiveActor *, s32, s32);
    void findLightInfo(LiveActor *, s32, s32);
    void entry(s32);

    void drawOpa(s32) const;
    void drawXlu(s32) const;

    DrawBufferGroup* mBufferGroup;  // _0
    s32 mBufferGroupCount;          // _4
    MR::Vector<MR::AssignableArray<DrawBufferGroup *> >* _8;
    u8 _C[0x30-0xC];
};