#pragma once

#include <revolution.h>

class JointController;
class JointCtrlRate;
class LiveActor;

class FaceJointCtrl {
public:
    FaceJointCtrl(LiveActor *);

    void init();
    void startCtrl(s32);
    void endCtrl(s32);
    void update();
    void setCallBackFunction();

    LiveActor* mHostActor;              // _0
    f32 _4;
    f32 _8;
    f32 _C;
    f32 _10;
    f32 _14;
    f32 _18;
    f32 _1C;
    JointController* mJointController;  // _20
    JointCtrlRate* mJointCtrlRate;      // _24
};