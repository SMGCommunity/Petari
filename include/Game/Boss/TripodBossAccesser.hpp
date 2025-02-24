#pragma once

#include "Game/Boss/TripodBoss.hpp"

class TripodBossAccesser : public NameObj {
public:
    TripodBossAccesser(const char *);

    virtual ~TripodBossAccesser();

    void setTriPodBoss(TripodBoss *);
    void addTripodBossParts(LiveActor *);
    void showTripodBossParts();
    void hideTripodBossParts();
    void activeTripodBossParts();
    bool isTripoddBossParts(const NameObj *) const;
    static void createSceneObj();

    TripodBoss* mBoss;      // 0xC
};

namespace MR {
    void addTripodBossPartsMovement(NameObj *);
    bool isTripoddBossParts(const NameObj *);
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
    void setTripodBossJointAttachBaseMatrix(const TPos3f &, s32);
    void requestStartTripodBossStepSequence(s32);
    void addTripodBossStepPoint(TripodBossStepPoint *);
    void getTripodBossJointMatrix(TPos3f *, s32);
    void getTripodBossAttachJointMatrix(TPos3f *, s32);
    void concatTripodBossAttachJointMatrix(TPos3f *, s32);
    bool isSteppingTripodBossLegID(s32);
    bool isSteppingTripodBossJointID(s32);
};
