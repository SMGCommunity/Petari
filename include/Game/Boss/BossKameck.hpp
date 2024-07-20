#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class BossKameckMoveRail;

class BossKameck : public LiveActor {
public:
    BossKameck(const char *, const char *);

    virtual ~BossKameck();
    virtual void init(const JMapInfoIter &);
    virtual void makeActorDead();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor *, HitSensor *);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);

    BossKameckMoveRail* getMoveRail(s32);
    void startSequence();
    void startDemoAppearKameck();
    void endDemoAppearKameck();
    void appearKameck();
    void deadKameck();
    s32 getLivingKameckNum() const;
    void appearStarPieceToPlayer(s32);
    void endDemo();
    void setPose(MtxPtr);
    void killAllBeam();
    void updatePose();
    void initKameckHolder(const JMapInfoIter &);
    void initMoveRail(const JMapInfoIter &);
    void appearStarPieceToUp(s32);
    void startDemo();
    void hitBeam(s32);
};