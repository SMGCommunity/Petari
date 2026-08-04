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

    virtual ~KuriboMini();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void makeActorAppeared();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

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
