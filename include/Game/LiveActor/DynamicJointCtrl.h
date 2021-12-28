#pragma once

#include <revolution.h>
#include "JSystem/JGeometry.h"

class LiveActor;

class JointCtrlRate {
public:
    JointCtrlRate();

    void update();
    void startCtrl(s32);
    void endCtrl(s32);

    f32 _0;
    u32 _4;
    s32 _8;
    s32 _C;
};

struct DynamicJointCtrlParam {
    f32 mGravity;               // _0
    f32 mFriction;              // _4
    f32 mAccelRatetoBckPos;     // _8
    f32 mBendMaxDegree;         // _C
    const char* mJointName;     // _10
};

class DynamicJointCtrlNode;

class DynamicJointCtrl {
public:
    DynamicJointCtrl(LiveActor *, const char *, const DynamicJointCtrlParam *);

    void init();
    void update();
    void reset();
    void setCallBackFunction();

    LiveActor* mActor;                  // _0
    const char* mName;                  // _4
    u32 _8;
    DynamicJointCtrlNode** mCtrlNodes;  // _C
    DynamicJointCtrlParam* mParams;     // _10
    JointCtrlRate* mControlRate;        // _14
};

class DynamicJointCtrlNode {
public:
    DynamicJointCtrlNode(const DynamicJointCtrl *, MtxPtr, f32);

    void update(LiveActor *, const DynamicJointCtrlNode *);
    void reset();

    f32 _0;
    TVec3f _4;
    TVec3f _10;
    TVec3f _1C;
    MtxPtr _28;
    u32 _2C;
    u32 _30;
    DynamicJointCtrl* mParentControl;   // _34
};

class DynamicJointCtrlKeeper {
public:
    DynamicJointCtrlKeeper(LiveActor *);

    void update();
    void setCallBackFunction();
    void startCtrl(const char *, s32);
    void endCtrl(const char *, s32);
    void reset();
    DynamicJointCtrl* findJointCtrl(const char *);

    LiveActor* mActor;              // _0
    s32 _4;
    DynamicJointCtrl** mControls;   // _8
};