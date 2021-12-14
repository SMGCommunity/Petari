#pragma once

#include "Game/Util.h"
#include "JSystem/JGeometry/TVec.h"

class LiveActor;

class ClippingActorInfo {
public:
    ClippingActorInfo(LiveActor *);

    void judgeClipping();
    bool isJudgedToClip() const;
    bool isGroupClipping() const;
    void setTypeToSphere(f32, const TVec3f *);
    void setGroupClippingNo(const JMapInfoIter &);

    LiveActor* mActor;  // _0
    s32 _4;
    f32 _8;
    u32 _C;
    u16 _10;
    s16 _12;
    u32 _14;
};

class ClippingActorInfoList {
public:
    ClippingActorInfoList(int);

    void add(ClippingActorInfo *);
    ClippingActorInfo* find(const LiveActor *, s32 *) const;
    ClippingActorInfo* findOrNone(const LiveActor *) const;
    bool isInList(const LiveActor *) const;

    int _0;
    s32 _4;
    ClippingActorInfo** mClippingActorList;  // _8
};