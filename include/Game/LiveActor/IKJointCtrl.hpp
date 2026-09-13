#pragma once

#include <JSystem/JGeometry/TMatrix.hpp>

class IKJoint;
class JointController;
class JointControllerInfo;
class LiveActor;

struct IKJointCtrlParam {
    IKJointCtrlParam() : mName(""), mNearLimitRate(), mFarLimitRate(1.0f), mEndDirMaxDegree(45.0f), mEndLocalDir(0.0f, 1.0f, 0.0f) {
    }

    /* 0x00 */ const char* mName;
    /* 0x04 */ f32 mNearLimitRate;
    /* 0x08 */ f32 mFarLimitRate;
    /* 0x0C */ f32 mEndDirMaxDegree;
    /* 0x10 */ TVec3f mEndLocalDir;
};

class IKJointCtrl {
public:
    IKJointCtrl(LiveActor*);

    void init(const IKJointCtrlParam&);
    void setEndPosition(const TVec3f&, f32);
    void setEndDirection(const TVec3f&, f32);
    void endCtrl();
    void setCallBackFunction();
    bool updateRootJointCallBack(TPos3f*, const JointControllerInfo&);
    bool updateMiddleJointCallBack(TPos3f*, const JointControllerInfo&);
    bool updateEndJointCallBack(TPos3f*, const JointControllerInfo&);

    void disableCallBack() {
        _B6 = false;
    }

    void enableCallBack() {
        _B6 = true;
    }

    /* 0x00 */ const char* mName;
    /* 0x04 */ TMtx34f _4;
    /* 0x34 */ TMtx34f _34;
    /* 0x64 */ LiveActor* mActor;
    /* 0x68 */ IKJoint* mJoint;
    /* 0x6C */ JointController* mRootCtrl;
    /* 0x70 */ JointController* mMiddleCtrl;
    /* 0x74 */ JointController* mEndCtrl;
    /* 0x78 */ TVec3f _78;
    /* 0x84 */ TVec3f mEndLocalDir;
    /* 0x90 */ TVec3f _90;
    /* 0x9C */ f32 mNearLimitRate;
    /* 0xA0 */ f32 mFarLimitRate;
    /* 0xA4 */ f32 mEndDirMaxDegree;
    /* 0xA8 */ f32 _A8;
    /* 0xAC */ f32 _AC;
    /* 0xB0 */ u16 _B0;
    /* 0xB2 */ u16 _B2;
    /* 0xB4 */ u16 _B4;
    /* 0xB6 */ u8 _B6;
};

class IKJointCtrlHolder {
public:
    IKJointCtrlHolder(LiveActor*, u32);

    void addCtrl(const IKJointCtrlParam&);

    void setEndPosition(const char*, const TVec3f&, f32);
    void setEndDirection(const char*, const TVec3f&, f32);

    void startUpdate();
    void endUpdate();
    void endCtrlAll();
    void setCallBackFunction();
    IKJointCtrl* findIKJointCtrl(const char*);

    /* 0x00 */ IKJointCtrl** mControls;
    /* 0x04 */ s32 mNumControls;
    /* 0x08 */ s32 _8;
    /* 0x0C */ LiveActor* mActor;
    /* 0x10 */ u8 _10;
};

class ActorJoint {
public:
    static IKJointCtrlHolder* createIKJointCtrlHolder(LiveActor*);
};