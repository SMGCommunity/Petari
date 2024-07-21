#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ActiveActorList.hpp"

class BossKameckMoveRail;
class KameckHolder;
class ActorJointCtrl;
class KameckBeamEventListener;

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

    const char* _8C;
    TQuat4f _90;
    TVec3f _A0;
    u32 _AC;
    KameckHolder* mKameckHolder;                // _B0
    ActorJointCtrl* mJointCtrl;                 // _B4
    ActiveActorList* mActorList;                // _B8
    KameckBeamEventListener* mBeamListener;     // _BC
    BossKameckMoveRail* mMoveRail;              // _C0
    u32 _C4;
    TVec3f _C8;
    s32 _D4;
};