#pragma once

#include "JSystem/JGeometry.hpp"
#include <revolution/types.h>

class JointController;
struct JointControllerInfo;
class JointCtrlRate;
class LiveActor;

class FaceJointCtrl {
public:
    FaceJointCtrl(LiveActor*);

    void init();
    void startCtrl(s32);
    void endCtrl(s32);
    void update() NO_INLINE;
    void setCallBackFunction();

    bool updateJointMtxCallBack(TPos3f* pMtx, const JointControllerInfo& rInfo);

    /* 0x00 */ LiveActor* mHostActor;
    /* 0x04 */ f32 mDegreeMax;
    /* 0x08 */ TVec3f _8;
    /* 0x14 */ TVec3f _14;
    /* 0x20 */ JointController* mJointController;
    /* 0x24 */ JointCtrlRate* mJointCtrlRate;
};
