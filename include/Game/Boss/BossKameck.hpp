#pragma once

#include "Game/Enemy/KameckBeam.hpp"
#include "Game/LiveActor/ActiveActorList.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class BossKameckVs2;
class BossKameckMoveRail;
class KameckHolder;
class ActorJointCtrl;
class BossKameck;
class BossKameckSequencer;
class BossKameckBeamEventListener;

class BossKameck : public LiveActor {
public:
    BossKameck(const char*, const char*);

    virtual ~BossKameck();
    virtual void init(const JMapInfoIter&);
    virtual void makeActorDead();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

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
    void initKameckHolder(const JMapInfoIter&);
    void initMoveRail(const JMapInfoIter&);
    void appearStarPieceToUp(s32);
    void startDemo();
    void hitBeam(s32);

    const char* _8C;
    TQuat4f _90;
    TVec3f _A0;
    BossKameckSequencer* mSequencer;             // 0xAC
    KameckHolder* mKameckHolder;                 // 0xB0
    ActorJointCtrl* mJointCtrl;                  // 0xB4
    ActiveActorList* mActorList;                 // 0xB8
    BossKameckBeamEventListener* mBeamListener;  // 0xBC
    BossKameckMoveRail** mMoveRail;              // 0xC0
    s32 _C4;
    TVec3f _C8;
    s32 _D4;
};

class BossKameckBeamEventListener : public KameckBeamEventListener {
public:
    inline BossKameckBeamEventListener(BossKameck* pBoss) : KameckBeamEventListener() { mBossKameck = pBoss; }

    virtual void hitBeam(s32 type) { mBossKameck->hitBeam(type); }

    BossKameck* mBossKameck;  // 0x4
};

namespace MR {
    NameObj* createBossKameck1(const char*);
    NameObj* createBossKameck2(const char*);
};  // namespace MR
