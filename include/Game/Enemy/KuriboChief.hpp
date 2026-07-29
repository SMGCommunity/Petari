#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"

class AnimScaleController;
class WalkerStateWander;
class WalkerStateFindPlayer;
class WalkerStateChase;
class WalkerStateStagger;
class WalkerStateBindStarPointer;
class ItemGenerator;
class KeySwitch;

class KuriboChief : public LiveActor {
public:
    KuriboChief(const char*);

    virtual ~KuriboChief();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initSensor();
    void initState();
    void initKeySwitch(const JMapInfoIter&);
    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);
    void requestStagger(HitSensor*, HitSensor*);
    void requestBlowDown(HitSensor*, HitSensor*);
    bool tryFind();
    bool tryPointBind();
    void exeWander();
    void exeFindPlayer();
    void exeChase();
    void exeStagger();
    void exeTrample();
    void exeAttackSuccess();
    void exeBlowDown();
    void exeBlowDownLand();
    bool isEnableAttack();
    bool isEnableKick();
    bool isDown();

    /* 0x8C */ AnimScaleController* mScaleController;
    /* 0x90 */ WalkerStateWander* mStateWander;
    /* 0x94 */ WalkerStateFindPlayer* mStateFindPlayer;
    /* 0x98 */ WalkerStateChase* mStateChase;
    /* 0x9C */ WalkerStateStagger* mStateStagger;
    /* 0xA0 */ WalkerStateBindStarPointer* mStateBindStarPointer;
    /* 0xA4 */ ItemGenerator* mItemGenerator;    
    /* 0xA8 */ KeySwitch* mKeySwitch;
    /* 0xAC */ TQuat4f _AC;
    /* 0xBC */ TVec3f _BC;
};
