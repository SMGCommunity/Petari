#pragma once

#include "Game/Util.h"
#include "JSystem/JGeometry/TVec.h"

class ViewGroupCtrlDataEntry;
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
    const TVec3f* _4;
    f32 _8;
    JMapIdInfo* mInfo;  // _C
    s16 mFarClipLevel;  // _10
    s16 _12;
    ViewGroupCtrlDataEntry* _14;
};

class ClippingActorInfoList {
public:
    ClippingActorInfoList(int);

    void add(ClippingActorInfo *);
    ClippingActorInfo* find(const LiveActor *, s32 *) const;
    ClippingActorInfo* findOrNone(const LiveActor *) const;
    bool isInList(const LiveActor *) const;

    ClippingActorInfo* remove(LiveActor *);

    int _0;
    s32 _4;
    ClippingActorInfo** mClippingActorList;  // _8
};