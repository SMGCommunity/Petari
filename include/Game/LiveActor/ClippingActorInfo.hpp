#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/types.h>

class JMapIdInfo;
class JMapInfoIter;
class LiveActor;
class ViewGroupCtrlDataEntry;

class ClippingActorInfo {
public:
    ClippingActorInfo(LiveActor*);

    void judgeClipping();
    bool isJudgedToClip() const;
    bool isGroupClipping() const;
    void setTypeToSphere(f32, const TVec3f*);
    void setGroupClippingNo(const JMapInfoIter&);

    /* 0x00 */ LiveActor* mActor;
    /* 0x04 */ const TVec3f* _4;
    /* 0x08 */ f32 _8;
    /* 0x0C */ JMapIdInfo* mInfo;
    /* 0x10 */ s16 mFarClipLevel;
    /* 0x12 */ s16 _12;
    /* 0x14 */ ViewGroupCtrlDataEntry* _14;
};

class ClippingActorInfoList {
public:
    ClippingActorInfoList(int);

    void add(ClippingActorInfo*);
    ClippingActorInfo* find(const LiveActor*, s32*) const;
    ClippingActorInfo* findOrNone(const LiveActor*) const;
    bool isInList(const LiveActor*) const;

    ClippingActorInfo* remove(LiveActor*);

    /* 0x00 */ int _0;
    /* 0x04 */ s32 _4;
    /* 0x08 */ ClippingActorInfo** mClippingActorList;
};
