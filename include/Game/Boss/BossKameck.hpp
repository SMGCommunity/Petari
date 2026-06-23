#pragma once

#include "Game/Enemy/KameckBeam.hpp"

class ActiveActorList;
class ActorJointCtrl;
class BossKameck;
class BossKameckBeamEventListener;
class BossKameckMoveRail;
class BossKameckSequencer;
class BossKameckVs2;
class KameckHolder;

class BossKameck : public LiveActor {
public:
    BossKameck(const char*, const char*);

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

    /* 0x8C */ const char* _8C;
    /* 0x90 */ TQuat4f _90;
    /* 0xA0 */ TVec3f _A0;
    /* 0xAC */ BossKameckSequencer* mSequencer;
    /* 0xB0 */ KameckHolder* mKameckHolder;
    /* 0xB4 */ ActorJointCtrl* mJointCtrl;
    /* 0xB8 */ ActiveActorList* mActorList;
    /* 0xBC */ BossKameckBeamEventListener* mBeamListener;
    /* 0xC0 */ BossKameckMoveRail** mMoveRail;
    /* 0xC4 */ s32 _C4;
    /* 0xC8 */ TVec3f _C8;
    /* 0xD4 */ s32 _D4;
};

class BossKameckBeamEventListener : public KameckBeamEventListener {
public:
    BossKameckBeamEventListener(BossKameck* pBoss) : KameckBeamEventListener(), mBossKameck(pBoss) {
    }

    virtual void hitBeam(s32 type) {
        mBossKameck->hitBeam(type);
    }

    /* 0x04 */ BossKameck* mBossKameck;
};

namespace MR {
    NameObj* createBossKameck1(const char*);
    NameObj* createBossKameck2(const char*);
};  // namespace MR
