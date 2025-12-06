#pragma once

#include "JSystem/JGeometry/TVec.hpp"
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

    LiveActor* mActor;                         // 0x0
    DynamicJointCtrlKeeper* mJointCtrlKeeper;  // 0x4
    FaceJointCtrl* mFaceJointCtrl;             // 0x8
    IKJointCtrlHolder* mJointCtrlHolder;       // 0xC
    u8 _10;
    u8 _11;
};