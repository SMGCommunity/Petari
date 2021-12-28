#pragma once

#include "Game/Util/IKJoint.h"
#include "Game/Util/JointController.h"
#include "JSystem/JGeometry/TMatrix.h"
#include "Inline.h"

class LiveActor;

struct IKJointCtrlParam {
    const char* mName;      // _0
    f32 mNearLimitRate;     // _4
    f32 mFarLimitRate;      // _8
    f32 mEndDirMaxDegree;   // _C
    f32 mEndLocalDirX;      // _10
    f32 mEndLocalDirY;      // _14
    f32 mEndLocalDirZ;      // _18
};

class IKJointCtrl {
public:
    IKJointCtrl(LiveActor *);

    void init(const IKJointCtrlParam &);
    void setEndPosition(const TVec3f &, f32);
    void setEndDirection(const TVec3f &, f32);
    void endCtrl();
    void setCallBackFunction();
    bool updateRootJointCallBack(TPos3f *, const JointControllerInfo &);
    bool updateMiddleJointCallBack(TPos3f *, const JointControllerInfo &);

    void disableCallBack();
    void enableCallBack();

    const char* mName;                                      // _0
    TMtx34f _4;
    TMtx34f _34;
    LiveActor* mActor;                                      // _64
    IKJoint* mJoint;                                        // _68
    JointControllerDelegator<IKJointCtrl>* mRootCtrl;       // _6C
    JointControllerDelegator<IKJointCtrl>* mMiddleCtrl;     // _70
    JointControllerDelegator<IKJointCtrl>* mEndCtrl;        // _74
    TVec3f _78;
    TVec3f mEndLocalDir;                                    // _84
    f32 _90;
    f32 _94;
    f32 _98;
    f32 mNearLimitRate;                                     // _9C
    f32 mFarLimitRate;                                      // _A0
    f32 mEndDirMaxDegree;                                   // _A4
    f32 _A8;
    f32 _AC;
    u16 _B0;
    u16 _B2;
    u16 _B4;
    u8 _B6;
};

class IKJointCtrlHolder { 
public:
    IKJointCtrlHolder(LiveActor *, u32);

    void addCtrl(const IKJointCtrlParam &);

    void setEndPosition(const char *, const TVec3f &, f32);
    void setEndDirection(const char *, const TVec3f &, f32);

    void startUpdate();

    void endUpdate();
    void endCtrlAll();
    void setCallBackFunction();
    IKJointCtrl* findIKJointCtrl(const char *);

    IKJointCtrl** mControls;    // _0
    s32 mNumControls;           // _4
    s32 _8;
    LiveActor* mActor;          // _C
    u8 _10;
};

class ActorJoint {
public:
    static IKJointCtrlHolder* createIKJointCtrlHolder(LiveActor *);
};