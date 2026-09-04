#pragma once

#include "Game/Boss/TripodBoss.hpp"

class TripodBossAccesser : public NameObj {
public:
    /// @brief Creates a new `TripodBossAccesser`.
    /// @param pName A pointer to the null-terminated name of the object.
    TripodBossAccesser(const char* pName);

    TripodBoss* getTriPodBoss() const;
    void setTriPodBoss(TripodBoss*);
    void addTripodBossParts(LiveActor*);
    void showTripodBossParts();
    void hideTripodBossParts();
    void activeTripodBossParts();
    bool isTripoddBossParts(const NameObj*) const;

    static TripodBossAccesser* createSceneObj();

private:
    /* 0x00C */ TripodBoss* mBoss;
    /* 0x010 */ LiveActor* mParts[256];
    /* 0x410 */ s32 mPartsNum;
};

namespace MR {
    TripodBossAccesser* getTripodBossAccesser();
    void addTripodBossParts(LiveActor*);
    void addTripodBossPartsMovement(NameObj*);
    bool isTripoddBossParts(const NameObj*);
    void showTripodBossParts();
    void hideTripodBossParts();
    void activeTripodBossParts();
    void requestMovementTripodBossParts();
    bool isCreatedTripodBoss();
    bool isStartedTripodBoss();
    bool isBrokenTripodBoss();
    bool isDemoTripodBoss();
    bool isStartDemoTripodBoss();
    bool isDamageDemoTripodBoss();
    bool isEndBreakDownDemoTripodBoss();
    bool isLeaveMarioNowTripodBoss();
    bool isRideMarioTripodBoss();
    bool tryDamageDemoTripodBoss();
    void requestEndDamageDemoTripodBoss();
    u32 getTripodBossGravityHostID();
    void setTripodBossJointAttachBaseMatrix(const TPos3f&, s32);
    void requestStartTripodBossStepSequence(s32);
    void addTripodBossStepPoint(TripodBossStepPoint*);
    void getTripodBossJointMatrix(TPos3f*, s32);
    void getTripodBossAttachJointMatrix(TPos3f*, s32) NO_INLINE;
    void concatTripodBossAttachJointMatrix(TPos3f*, s32);
    bool isSteppingTripodBossLegID(s32) NO_INLINE;
    bool isSteppingTripodBossJointID(s32);
};  // namespace MR
