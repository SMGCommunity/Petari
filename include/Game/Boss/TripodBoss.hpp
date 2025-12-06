#pragma once

#include "Game/Boss/TripodBossStepPoint.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

class TripodBossLeg;
class TripodBossMovableArea;
class TripodBossStepSequence;
class DummyDisplayModel;

class TripodBossBone {
public:
    inline TripodBossBone() {
        _30 = nullptr;
        _0.identity();
    }

    void setAttachBaseMatrix(const TPos3f&);

    TMtx34f _0;
    const TPos3f* _30;
};

class TripodBoss : public LiveActor {
public:
    enum PART_ID {
        LeftLeg = 0,
        MiddleLeg = 1,
        RightLeg = 2,
    };

    enum SUB_PART_ID {
        Part_RootLocalY = 0,
        Part_RootLocalYZ = 1,
        Part_RootJoint = 2,
        Part_MiddleJoint = 3,
        Part_AnkleLocalX = 4,
        Part_AnkleLocalXZ = 5,
        Part_EndJoint = 6
    };

    TripodBoss(const char*);

    virtual ~TripodBoss();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();

    void initEventCamera(const JMapInfoIter&);
    void initLeg(const JMapInfoIter&);
    void initLegIKPlacement();
    void initMovableArea(const TPos3f&);
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
    inline void exeNonActive();
    inline void exeTryStartDemo();
    bool isStopLeg(s32) const;
    bool isStopAllLeg() const;
    bool isStarted() const;
    bool isDemo() const;
    bool isStartDemo() const NO_INLINE;
    bool isDamageDemo() const NO_INLINE;
    bool isEndDemo() const;
    bool isEndPainDemo() const NO_INLINE;
    bool isEndBreakDownDemo() const NO_INLINE;
    bool isEndExplosionDemo() const NO_INLINE;
    bool isBroken() const;
    bool isRideMario() const;
    bool isLeaveMarioNow() const;
    void setJointAttachBaseMatrix(const TPos3f&, s32);
    void addStepPoint(TripodBossStepPoint*);
    void getBodyMatrix(TPos3f*) const;
    void getJointMatrix(TPos3f*, s32) const;
    void getJointAttachMatrix(TPos3f*, s32) const;
    void requestStartStepSequence(s32);
    TripodBossStepSequence* getCurrentStepSequence();
    TripodBossStepSequence* getNextStepSequence();
    void calcLegUpVector(TVec3f*, const TVec3f&);
    void calcDemoMovement();
    void calcBodyMovement();
    void calcLegMovement();
    void addAccelToWeightPosition();
    void calcClippingSphere();
    void clippingModel();
    void startDemo();
    void endDemo(const char*);
    void checkRideMario();
    const TPos3f* getLegMatrixPtr(PART_ID, SUB_PART_ID) const;
    void changeBgmState();
    static s32 getPartIDFromBoneID(s32);
    void setAttachBaseMatrix(const TPos3f&);

    inline TripodBossLeg* getLeg(s32 idx) const { return mLegs[idx]; }

    inline TripodBossStepPoint* getStepPoint(int idx) { return mStepPoints[idx]; }

    inline bool isStateSomething() { return (_634 == 0) || (_634 == 1); }

    TPos3f mBodyMtx;  // 0x8C
    TPos3f _BC;
    TPos3f _EC;
    ModelObj* mLowModel;                    // 0x11C
    TripodBossBone mBossBones[0x16];        // 0x120
    TripodBossLeg* mLegs[3];                // 0x598
    TripodBossStepPoint* mStepPoints[3];    // 0x5A4
    TripodBossMovableArea* mMovableArea;    // 0x5B0
    TripodBossStepSequence* mStepSequence;  // 0x5B4
    DummyDisplayModel* mDummyModel;         // 0x5B8
    TVec3f _5BC;
    TVec3f _5C8;
    TVec3f _5D4;
    TVec3f _5E0;
    TVec3f _5EC;
    f32 _5F8;
    f32 _5FC;
    f32 _600;
    f32 _604;
    f32 _608;
    f32 _60C;
    f32 _610;
    f32 _614;
    f32 _618;
    f32 _61C;
    f32 _620;
    s32 mCurrentStepSeq;  // 0x624
    s32 mNextStepSeq;     // 0x628
    s32 _62C;
    s32 _630;
    s32 _634;
    u8 _638;
    s32 _63C;
    u8 _640;
    ActorCameraInfo* mEventCamera;  // 0x644
};

namespace MR {
    NameObj* createTripodBoss(const char*);
    NameObj* createTripod2Boss(const char*);
};  // namespace MR
