#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AnimScaleController;
class ItemGenerator;
class WalkerStateWander;
class WalkerStateFindPlayer;
class WalkerStateChase;
class WalkerStateStagger;
class WalkerStateBindStarPointer;

class KuriboMini : public LiveActor {
public:
    KuriboMini(const char*);

    /* 0x08 */ virtual ~KuriboMini();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x28 */ virtual void makeActorAppeared();
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x58 */ virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initSensor();
    void initState();
    bool requestHipDropDown(HitSensor*, HitSensor*);
    bool requestFlatDown(HitSensor*, HitSensor*);
    bool requestPressDown();
    bool requestBlowDown(HitSensor*, HitSensor*);
    bool requestStagger(HitSensor*, HitSensor*);
    bool requestAttackSuccess();
    bool tryFind();
    bool tryPointBind();
    bool tryDeadMap();
    void exeWander();
    void exeFindPlayer();
    void exeChase();
    void exeStagger();
    void exeBindStarPointer();
    void endBindStarPointer();
    void exeAttackSuccess();
    void exeHipDropDown();
    void exeFlatDown();
    void exePressDown();
    void exeBlowDown();
    void calcPassiveMovement();
    bool isEnableAttack() const;
    bool isEnableKick() const NO_INLINE;
    bool isDown() const;

    /* 0x8C */ AnimScaleController* mScaleController;
    /* 0x90 */ ItemGenerator* mItemGenerator;
    /* 0x94 */ WalkerStateWander* mStateWander;
    /* 0x98 */ WalkerStateFindPlayer* mStateFindPlayer;
    /* 0x9C */ WalkerStateChase* mStateChase;
    /* 0xA0 */ WalkerStateStagger* mStateStagger;
    /* 0xA4 */ WalkerStateBindStarPointer* mStateBindStarPointer;
    /* 0xA8 */ TQuat4f _A8;
    /* 0xB8 */ TVec3f _B8;
};
