#pragma once

#include "JSystem/JGeometry.hpp"
#include <revolution.h>

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
    f32 mGravity;            // 0x0
    f32 mFriction;           // 0x4
    f32 mAccelRatetoBckPos;  // 0x8
    f32 mBendMaxDegree;      // 0xC
    const char* mJointName;  // 0x10
};

class DynamicJointCtrlNode;

class DynamicJointCtrl {
public:
    DynamicJointCtrl(LiveActor*, const char*, const DynamicJointCtrlParam*);

    void init();
    void update();
    void reset();
    void setCallBackFunction();

    LiveActor* mActor;  // 0x0
    const char* mName;  // 0x4
    u32 _8;
    DynamicJointCtrlNode** mCtrlNodes;  // 0xC
    DynamicJointCtrlParam* mParams;     // 0x10
    JointCtrlRate* mControlRate;        // 0x14
};

class DynamicJointCtrlNode {
public:
    DynamicJointCtrlNode(const DynamicJointCtrl*, MtxPtr, f32);

    void update(LiveActor*, const DynamicJointCtrlNode*);
    void reset();

    f32 _0;
    TVec3f _4;
    TVec3f _10;
    TVec3f _1C;
    MtxPtr _28;
    u32 _2C;
    u32 _30;
    DynamicJointCtrl* mParentControl;  // 0x34
};

class DynamicJointCtrlKeeper {
public:
    DynamicJointCtrlKeeper(LiveActor*);

    void update();
    void setCallBackFunction();
    void startCtrl(const char*, s32);
    void endCtrl(const char*, s32);
    void reset();
    DynamicJointCtrl* findJointCtrl(const char*);

    LiveActor* mActor;  // 0x0
    s32 _4;
    DynamicJointCtrl** mControls;  // 0x8
};