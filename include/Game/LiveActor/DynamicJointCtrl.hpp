#pragma once

#include "JSystem/JGeometry.hpp"
#include <revolution.h>

class LiveActor;
class JointController;
struct JointControllerInfo;

class JointCtrlRate {
public:
    JointCtrlRate();

    void update();
    void startCtrl(s32);
    void endCtrl(s32);

    /* 0x00 */ f32 _0;
    /* 0x04 */ s32 _4;
    /* 0x08 */ s32 _8;
    /* 0x0C */ s32 _C;
};

struct DynamicJointCtrlParam {
    /* 0x00 */ f32 mGravity;
    /* 0x04 */ f32 mFriction;
    /* 0x08 */ f32 mAccelRatetoBckPos;
    /* 0x0C */ f32 mBendMaxDegree;
    /* 0x10 */ s32 mCtrlJointNumMax;
};

class DynamicJointCtrlNode;

class DynamicJointCtrl {
public:
    DynamicJointCtrl(LiveActor*, const char*, const DynamicJointCtrlParam*);

    void init();
    void update();
    void reset();
    void setCallBackFunction();

    /* 0x00 */ LiveActor* mActor;
    /* 0x04 */ const char* mName;
    /* 0x08 */ s32 _8;
    /* 0x0C */ DynamicJointCtrlNode** mCtrlNodes;
    /* 0x10 */ const DynamicJointCtrlParam* mParams;
    /* 0x14 */ JointCtrlRate* mControlRate;
};

class DynamicJointCtrlNode {
public:
    DynamicJointCtrlNode(const DynamicJointCtrl*, MtxPtr, f32);

    void update(LiveActor*, const DynamicJointCtrlNode*);
    void reset();
    bool updateJointMtxCallBack(TPos3f*, const JointControllerInfo&);

    /* 0x00 */ f32 _0;
    /* 0x04 */ TVec3f _4;
    /* 0x10 */ TVec3f _10;
    /* 0x1C */ TVec3f _1C;
    /* 0x28 */ MtxPtr _28;
    /* 0x2C */ JointController* mJointController;
    /* 0x30 */ DynamicJointCtrlNode* mChild;
    /* 0x34 */ const DynamicJointCtrl* mParentControl;
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

    /* 0x00 */ LiveActor* mActor;
    /* 0x04 */ s32 _4;
    /* 0x08 */ DynamicJointCtrl** mControls;
};