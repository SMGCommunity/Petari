#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class ActorCameraInfo;
class DummyDisplayModel;
class ModelObj;
class TripodBossLeg;
class TripodBossMovableArea;
class TripodBossStepPoint;
class TripodBossStepSequence;

class TripodBossBone {
public:
    TripodBossBone() : _30() {
        _0.identity();
    }

    void setAttachBaseMatrix(const TPos3f&);

    /* 0x00 */ TMtx34f _0;
    /* 0x30 */ const TPos3f* _30;
};

/// @brief Megaleg
class TripodBoss : public LiveActor {
public:
    enum PART_ID {
        /* 0x00 */ PART_ID_LEFT_LEG,
        /* 0x01 */ PART_ID_BACK_LEG,
        /* 0x02 */ PART_ID_RIGHT_LEG,
        /* 0x03 */ PART_ID_MAX,
    };

    enum SUB_PART_ID {
        /* 0x00 */ SUB_PART_ID_ROOT_LOCAL_Y,
        /* 0x01 */ SUB_PART_ID_ROOT_LOCAL_YZ,
        /* 0x02 */ SUB_PART_ID_ROOT_JOINT,
        /* 0x03 */ SUB_PART_ID_MIDDLE_JOINT,
        /* 0x04 */ SUB_PART_ID_ANKLE_LOCAL_X,
        /* 0x05 */ SUB_PART_ID_ANKLE_LOCAL_XZ,
        /* 0x06 */ SUB_PART_ID_END_JOINT,
    };

    /// @brief Creates a new `TripodBoss`.
    /// @param pName A pointer to the null-terminated name of the object.
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

    void exeTryStartDemo();
    void exeNonActive();
    void exeWait();
    void exeStep();
    void exeWaitStep();
    void exeDamage();
    void exeChangeSequence();
    void exeLeaveLegOutOfPlayer();
    void exeStartDemo();
    void exeDamageDemo();
    void exePainDemo();
    void exeBreakDownDemo();
    void exeExplosionDemo();

    inline TripodBossLeg* getLeg(s32 idx) const {
        return mLegs[idx];
    }

    inline TripodBossStepPoint* getStepPoint(int idx) {
        return mStepPoints[idx];
    }

    inline bool isStateSomething() {
        return (_634 == 0) || (_634 == 1);
    }

    /* 0x08C */ TPos3f mBodyMtx;
    /* 0x0BC */ TPos3f _BC;
    /* 0x0EC */ TPos3f _EC;
    /* 0x11C */ ModelObj* mLowModel;
    /* 0x120 */ TripodBossBone mBossBones[22];
    /* 0x598 */ TripodBossLeg* mLegs[3];
    /* 0x5A4 */ TripodBossStepPoint* mStepPoints[3];
    /* 0x5B0 */ TripodBossMovableArea* mMovableArea;
    /* 0x5B4 */ TripodBossStepSequence* mStepSequence;
    /* 0x5B8 */ DummyDisplayModel* mDummyModel;
    /* 0x5BC */ TVec3f _5BC;
    /* 0x5C8 */ TVec3f _5C8;
    /* 0x5D4 */ TVec3f _5D4;
    /* 0x5E0 */ TVec3f _5E0;
    /* 0x5EC */ TVec3f _5EC;
    /* 0x5F8 */ f32 _5F8;
    /* 0x5FC */ f32 _5FC;
    /* 0x600 */ f32 _600;
    /* 0x604 */ f32 _604;
    /* 0x608 */ f32 _608;
    /* 0x60C */ f32 _60C;
    /* 0x610 */ f32 _610;
    /* 0x614 */ f32 _614;
    /* 0x618 */ f32 _618;
    /* 0x61C */ f32 _61C;
    /* 0x620 */ f32 _620;
    /* 0x624 */ s32 mCurrentStepSeq;
    /* 0x628 */ s32 mNextStepSeq;
    /* 0x62C */ s32 _62C;
    /* 0x630 */ s32 _630;
    /* 0x634 */ s32 _634;
    /* 0x638 */ u8 _638;
    /* 0x63C */ s32 _63C;
    /* 0x640 */ u8 _640;
    /* 0x644 */ ActorCameraInfo* mEventCamera;
};

namespace MR {
    NameObj* createTripodBoss(const char*);
    NameObj* createTripod2Boss(const char*);
};  // namespace MR
