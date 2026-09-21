#pragma once

#include <JSystem/JGeometry/TMatrix.hpp>

class LiveActor;
class JointController;
class JointCtrlRate;
struct JointControllerInfo;

class TurnJointCtrl {
public:
    enum AXIS { X = 0, Y = 1, Z = 2 };
    struct Ctrl {
        /* 0x00 */ bool mEnabled;
        /* 0x04 */ JointController* mJointController;
        /* 0x08 */ f32 mBlendRate;
        /* 0x0C */ AXIS mFrontAxis;
        /* 0x10 */ AXIS mVerticalTurnAxis;
        /* 0x14 */ AXIS mHorizontalTurnAxis;
    };

    TurnJointCtrl(LiveActor*);

    void init(f32, f32, f32);
    void addFace(const char*, f32, AXIS, AXIS, AXIS);
    void addWaist(const char*, f32, AXIS, AXIS, AXIS);
    void startCtrl(s32);
    void endCtrl(s32);
    void validate();
    void invalidate();
    void setStarePos(const TVec3f&);
    void update();
    void setCallBackFunction();
    bool updateJointMtxCallBackFace(TPos3f*, const JointControllerInfo&);
    bool updateJointMtxCallBackWaist(TPos3f*, const JointControllerInfo&);
    void getMtxDir(TVec3f*, const TPos3f*, AXIS);
    bool updateJointMtxCallBack(TPos3f*, const Ctrl&);

    /* 0x00 */ LiveActor* mActor;
    /* 0x04 */ Ctrl mFace;
    /* 0x1C */ Ctrl mWaist;
    /* 0x34 */ JointCtrlRate* mCtrlRate;
    /* 0x38 */ f32 mHorizontalDegreeMax;
    /* 0x3C */ f32 mPositiveDegreeMax;
    /* 0x40 */ f32 mNegativeDegreeMax;
    /* 0x44 */ TVec3f mStarePos;
    /* 0x50 */ TVec3f _50;
    /* 0x5C */ f32 _5C;
    /* 0x60 */ f32 _60;
    /* 0x64 */ s32 mCtrlFrames;
    /* 0x68 */ bool mEnabled;
};
