#pragma once

#include "Game/Util/IKJoint.hpp"
#include "Game/Util/JointController.hpp"
#include "Inline.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

class LiveActor;

struct IKJointCtrlParam {
    const char* mName;     // 0x0
    f32 mNearLimitRate;    // 0x4
    f32 mFarLimitRate;     // 0x8
    f32 mEndDirMaxDegree;  // 0xC
    f32 mEndLocalDirX;     // 0x10
    f32 mEndLocalDirY;     // 0x14
    f32 mEndLocalDirZ;     // 0x18
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

    void disableCallBack();
    void enableCallBack();

    const char* mName;  // 0x0
    TMtx34f _4;
    TMtx34f _34;
    LiveActor* mActor;                                  // 0x64
    IKJoint* mJoint;                                    // 0x68
    JointControlDelegator< IKJointCtrl >* mRootCtrl;    // 0x6C
    JointControlDelegator< IKJointCtrl >* mMiddleCtrl;  // 0x70
    JointControlDelegator< IKJointCtrl >* mEndCtrl;     // 0x74
    TVec3f _78;
    TVec3f mEndLocalDir;  // 0x84
    f32 _90;
    f32 _94;
    f32 _98;
    f32 mNearLimitRate;    // 0x9C
    f32 mFarLimitRate;     // 0xA0
    f32 mEndDirMaxDegree;  // 0xA4
    f32 _A8;
    f32 _AC;
    u16 _B0;
    u16 _B2;
    u16 _B4;
    u8 _B6;
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

    IKJointCtrl** mControls;  // 0x0
    s32 mNumControls;         // 0x4
    s32 _8;
    LiveActor* mActor;  // 0xC
    u8 _10;
};

class ActorJoint {
public:
    static IKJointCtrlHolder* createIKJointCtrlHolder(LiveActor*);
};