#pragma once

#include "Game/NameObj/NameObjArchiveListCollector.h"
#include "Game/LiveActor/LiveActor.h"
#include "JSystem/JGeometry.h"

namespace {
    const char* cAppearDemoName = "パワースター出現";
    const char* cStageClearAnimNameKoopaVs3 = "GrandStarGetKoopaVs3";

    /*void setupColorGrandStar(LiveActor *, bool) NO_INLINE {

    }

    void setupColor(LiveActor *, bool, int) NO_INLINE {

    }*/
};

class PowerStar : public LiveActor {
public:
    PowerStar(const char *);

    virtual ~PowerStar();
    virtual void init(const JMapInfoIter &);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void makeActorAppeared();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual u32 receiveMsgPLayerAttack(u32, HitSensor *, HitSensor *);
    virtual u32 receiveOtherMsg(u32, HitSensor *, HitSensor *);

    void requestAppear();
    void setDemoAppearPos(const TVec3f &);
    bool isEndAppearDemo() const;
    void offAppearDemo();
    void setupColor(LiveActor *, const NameObj *, s32);
    void setupColorAtResultSequence(LiveActor *, bool);
    void requestPointLight(const LiveActor *, const NameObj *, s32);
    void requestPointLightAtResultSequence(const LiveActor *);
    f32 getPowerStarWaitRotateSpeed();
    u32 getBtpFrameCurrentStage(s32);
    bool isCurrentStageKoopaVs3();
    void initShadowPowerStar(LiveActor *, bool);
    void makeArchiveList(NameObjArchiveListCollector *, const JMapInfoIter &);
    void initMapToolInfo(const JMapInfoIter &);
    void initModel();
    void initSensorGrandStar();
    void initShadow(const JMapInfoIter &);
    void initPosture();
    void endAppearDemo();
    TVec3f* getNearestAppearPoint(const TVec3f &);
    LiveActor* getAppearCameraActor();
    const ActorCameraInfo* getAppearCameraInfo() const;
    void requestAppearOrWait();
    void calcAppearDemoRiseTrans(TVec3f *, f32) const;
    void processWait(f32);
    void exeAppearDemoRise();
    void exeAppearDemoMove();
    void exeAppearDemoKoopa();
    void exeWait();
    void exeWeak();
    void exeWeakNoRotate();
    void exeWeakToWait();
    void exeStageClearDemo();

    s32 mPowerStarId;               // _8C
    bool mIsInDemo;                 // _90
    TVec3f mAppearPosition;         // _94
    TVec3f _A0;
    TVec3f _AC;
    TMtx34f _B8;
    TMtx34f _E8;
    ModelObj* mPowerStarModelObj;   // _118
    u8 _11C;
    u8 _11D;
    u8 _11E;
    u8 _11F;
    u32 mColorFrame;                // _120
    bool mIsGrandStar;              // _124
    u8 _125;                        // Obj_arg0
    u8 _126;                        // Obj_arg1
    u8 _127;                        // Obj_arg4
    ActorCameraInfo* mCameraInfo;   // _128
    LiveActor* mCameraActor;        // _12C
    ModelObj* mLuigiNPC;            // _130
    TMtx34f _134;
    u8 _164;
};

namespace NrvPowerStar {
    NERVE(PowerStarNrvWaitStartAppear);
    NERVE(PowerStarNrvAppearDemoRise);
    NERVE(PowerStarNrvAppearDemoMove);
    NERVE(PowerStarNrvAppearDemoKoopa);
    NERVE(PowerStarNrvWait);
    NERVE(PowerStarNrvWeak);
    NERVE(PowerStarNrvWeakNoRotate);
    NERVE(PowerStarNrvWeakToWait);
    NERVE(PowerStarNrvStageClearDemo);
};
