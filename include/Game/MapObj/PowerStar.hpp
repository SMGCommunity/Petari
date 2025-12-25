#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class NameObjArchiveListCollector;
class PowerStarAppearPoint;

class PowerStar : public LiveActor {
public:
    PowerStar(const char*);

    virtual ~PowerStar();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void makeActorAppeared();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void requestAppear();
    void setDemoAppearPos(const TVec3f&);
    bool isEndAppearDemo() const;
    void offAppearDemo();
    static void setupColor(LiveActor*, const NameObj*, s32);
    static void setupColorAtResultSequence(LiveActor*, bool);
    static void requestPointLight(const LiveActor*, const NameObj*, s32);
    static void requestPointLightAtResultSequence(const LiveActor*);
    static f32 getPowerStarWaitRotateSpeed();
    static s32 getBtpFrameCurrentStage(s32);
    static bool isCurrentStageKoopaVs3();
    static void initShadowPowerStar(LiveActor*, bool);
    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);
    void initMapToolInfo(const JMapInfoIter&);
    void initModel();
    void initSensorGrandStar();
    void initShadow(const JMapInfoIter&);
    void initPosture();
    void endAppearDemo();
    PowerStarAppearPoint* getNearestAppearPoint(const TVec3f&) const;
    LiveActor* getAppearCameraActor();
    ActorCameraInfo* getAppearCameraInfo() const;
    void requestAppearOrWait();
    void calcAppearDemoRiseTrans(TVec3f*, f32) const;
    void processWait(f32);
    void exeWaitStartAppear();
    void exeAppearDemoRise();
    void exeAppearDemoMove();
    void exeAppearDemoKoopa();
    void exeWait();
    void exeWeak();
    void exeWeakNoRotate();
    void exeWeakToWait();
    void exeStageClearDemo();

    inline bool isGrandStar() { return mIsGrandStar; }

    s32 mPowerStarId;        // 0x8C
    bool mIsInDemo;          // 0x90
    TVec3f mAppearPosition;  // 0x94
    TVec3f _A0;
    TVec3f _AC;
    TPos3f _B8;
    TMtx34f _E8;
    ModelObj* mPowerStarModelObj;  // 0x118
    u8 _11C;
    u8 _11D;
    u8 _11E;
    u8 _11F;

    // 0x120
    // this isn't what they wrote but oh well
    union {
        volatile s32 mColorFrame_v;
        s32 mColorFrame;
    };

    volatile bool mIsGrandStar;          // 0x124
    bool _125;                           // Obj_arg0
    bool _126;                           // Obj_arg1
    bool _127;                           // Obj_arg4
    ActorCameraInfo* mCameraInfo;        // 0x128
    PowerStarAppearPoint* mCameraActor;  // 0x12C
    ModelObj* mLuigiNPC;                 // 0x130
    TMtx34f _134;
    bool _164;
};
