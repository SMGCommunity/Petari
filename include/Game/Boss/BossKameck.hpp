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

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x30 */ virtual void makeActorDead();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

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

    /* 0x8C */ const char* mModelName;
    /* 0x90 */ TQuat4f _90;
    /* 0xA0 */ TVec3f _A0;
    /* 0xAC */ BossKameckSequencer* mSequencer;
    /* 0xB0 */ KameckHolder* mKameckHolder;
    /* 0xB4 */ ActorJointCtrl* mJointCtrl;
    /* 0xB8 */ ActiveActorList* mActorList;
    /* 0xBC */ BossKameckBeamEventListener* mBeamEventListener;
    /* 0xC0 */ BossKameckMoveRail** mMoveRail;
    /* 0xC4 */ s32 mMoveRailNum;
    /* 0xC8 */ TVec3f _C8;
    /* 0xD4 */ s32 mObjArg7;
};

class BossKameckBeamEventListener : public KameckBeamEventListener {
public:
    BossKameckBeamEventListener(BossKameck* pBoss) : KameckBeamEventListener(), mBossKameck(pBoss) {
    }

    /* 0x08 */ virtual void hitBeam(s32 type) {
        mBossKameck->hitBeam(type);
    }

    /* 0x04 */ BossKameck* mBossKameck;
};

namespace MR {
    NameObj* createBossKameck1(const char*);
    NameObj* createBossKameck2(const char*);
};  // namespace MR
