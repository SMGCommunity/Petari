#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class ActorCameraInfo;
class ModelObj;
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

    inline bool isGrandStar() {
        return mIsGrandStar;
    }

    /* 0x08C */ s32 mPowerStarId;
    /* 0x090 */ bool mIsInDemo;
    /* 0x094 */ TVec3f mAppearPosition;
    /* 0x0A0 */ TVec3f mInitRotation;
    /* 0x0AC */ TVec3f mInitPosition;
    /* 0x0B8 */ TPos3f _B8;
    /* 0x0E8 */ TPos3f mBaseMtx;
    /* 0x118 */ ModelObj* mPowerStarModelObj;
    /* 0x11C */ u8 _11C;
    /* 0x11D */ u8 _11D;
    /* 0x11E */ u8 _11E;
    /* 0x11F */ u8 _11F;

    // 0x120
    // this isn't what they wrote but oh well
    union {
        vs32 mColorFrame_v;
        s32 mColorFrame;
    };

    /* 0x124 */ volatile bool mIsGrandStar;
    /* 0x125 */ bool _125;
    /* 0x126 */ bool _126;
    /* 0x127 */ bool _127;
    /* 0x128 */ ActorCameraInfo* mCameraInfo;
    /* 0x12C */ PowerStarAppearPoint* mCameraActor;
    /* 0x130 */ ModelObj* mLuigiNPC;
    /* 0x134 */ TMtx34f _134;
    /* 0x164 */ bool _164;
};
