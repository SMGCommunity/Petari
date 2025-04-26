#pragma once

#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

class TripodBossLeg;
class TripodBossStepPoint;
class TripodBossMovableArea;
class TripodBossStepSequence;

struct SomeStruct {
    TMtx34f _0;
    u32 _30;
};

class TripodBoss : public LiveActor {
public:
    enum PART_ID {

    };

    enum SUB_PART_ID {

    };

    TripodBoss(const char *);

    virtual ~TripodBoss();
    virtual void init(const JMapInfoIter &);
    virtual void initAfterPlacement();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();

    void initEventCamera(const JMapInfoIter &);
    void initLeg(const JMapInfoIter &);
    void initLegIKPlacement();
    void initMovableArea(const TPos3f &);
    void initBodyPosition();
    void initBoneInfo();
    void initPose();
    bool tryStartStep();
    bool tryChangeSequence();
    bool tryEndSequence();
    bool tryNextSequence();
    bool tryDamage();
    bool tryWaitStep();
    bool tryNextStep();
    bool tryLeaveLegOutOfPlayer();
    bool tryEndLeaveLegOutOfPlayer();
    bool tryEndDamage();
    bool tryBreak();
    void requestOpeningDemo();
    bool tryDamageDemo();
    void requestEndDamageDemo();
    void exeWait();
    void exeStep();
    void exeWaitStep();
    void exeChangeSequence();
    void exeLeaveLegOutOfPlayer();
    void exeDamage();
    void exeStartDemo();
    void exeDamageDemo();
    void exePainDemo();
    void exeBreakDownDemo();
    void exeExplosionDemo();
    bool isStopLeg(s32) const;
    bool isStopAllLeg() const;
    bool isStarted() const;
    bool isDemo() const;
    bool isStartDemo() const;
    bool isDamageDemo() const;
    bool isEndDemo() const;
    bool isEndPainDemo() const;
    bool isEndBreakDownDemo() const;
    bool isEndExplosionDemo() const;
    bool isBroken() const;
    bool isRideMario() const;
    bool isLeaveMarioNow() const;
    void setJointAttachBaseMatrix(const TPos3f &, s32);
    void addStepPoint(TripodBossStepPoint *);
    void getBodyMatrix(TPos3f *) const;
    void getJointMatrix(TPos3f *, s32) const;
    void getJointAttachMatrix(TPos3f *, s32) const;
    void requestStartStepSequence(s32);
    TripodBossStepSequence* getCurrentStepSequence();
    TripodBossStepSequence* getNextStepSequence();
    void calcLegUpVector(TVec3f *, const TVec3f &);
    void calcDemoMovement();
    void calcBodyMovement();
    void calcLegMovement();
    void addAccelToWeightPosition();
    void calcClippingSphere();
    void clippingModel();
    void startDemo();
    void endDemo(const char *);
    void checkRideMario();
    MtxPtr getLegMatrixPtr(PART_ID, SUB_PART_ID) const;
    void changeBgmState();
    static PART_ID getPartIDFromBoneID(s32);
    void setAttachBaseMatrix(const TPos3f &);

    Mtx _8C;
    Mtx _BC;
    TPos3f _EC;
    s32 _11C;
    SomeStruct _120[0x16];
    TripodBossLeg** mLegs;                      // 0x598
    u32 _59C;
    u32 _5A0;
    TripodBossStepPoint** mStepPoints;          // 0x5A4
    u32 _5A8;
    u32 _5AC;
    TripodBossMovableArea* mMovableArea;        // 0x5B0
    TripodBossStepSequence* mStepSequence;      // 0x5B4
    u32 _5B8;
    u32 _5BC;
    u32 _5C0;
    u32 _5C4;
    TVec3f _5C8;
    TVec3f _5D4;
    TVec3f _5E0;
    TVec3f _5EC;
    u32 _5F8;
    u32 _5FC;
    u32 _600;
    u32 _604;
    u32 _608;
    u32 _60C;
    u32 _610;
    s32 _614;
    s32 _618;
    s32 _61C;
    u32 _620;
    s32 _624;
    s32 _628;
    s32 _62C;
    u32 _630;
    u32 _634;
    u8 _638;
    u32 _63C;
    u8 _640;
    ActorCameraInfo* mEventCamera;              // 0x644
};
