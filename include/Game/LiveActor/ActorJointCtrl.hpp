#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include <revolution.h>

class DynamicJointCtrlKeeper;
class FaceJointCtrl;
class IKJointCtrlHolder;
class LiveActor;

class ActorJointCtrl {
public:
    ActorJointCtrl(LiveActor*);

    void startDynamicCtrl(const char*, s32);
    void endDynamicCtrl(const char*, s32);
    void resetDynamicCtrl();
    void startFaceCtrl(s32);
    void endFaceCtrl(s32);
    void setIKEndPosition(const char*, const TVec3f&, f32);
    void setIKEndDirection(const char*, const TVec3f&, f32);
    void endIKCtrlAll();
    void update();
    void startUpdate();
    void endUpdate();
    void setCallBackFunction();

    /* 0x00 */ LiveActor* mActor;
    /* 0x04 */ DynamicJointCtrlKeeper* mJointCtrlKeeper;
    /* 0x08 */ FaceJointCtrl* mFaceJointCtrl;
    /* 0x0C */ IKJointCtrlHolder* mJointCtrlHolder;
    /* 0x10 */ u8 _10;
    /* 0x11 */ u8 _11;
};
