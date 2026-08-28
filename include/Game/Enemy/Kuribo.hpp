#pragma once

#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "JSystem/JGeometry/TQuat.hpp"

class ItemGenerator;
class WalkerStateStagger;
class WalkerStateChase;
class WalkerStateWander;
class WalkerStateFindPlayer;

class Kuribo : public LiveActor {
public:
    Kuribo(const char*);

    /* 0x08 */ virtual ~Kuribo();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x28 */ virtual void makeActorAppeared();
    /* 0x2C */ virtual void kill();
    /* 0x30 */ virtual void makeActorDead();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x58 */ virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initState();
    void initSensor();
    void initAppearState(const JMapInfoIter&);
    bool requestDead();
    bool requestFlatDown(HitSensor*, HitSensor*);
    bool requestHipDropDown(HitSensor*, HitSensor*);
    bool requestPressDown();
    bool requestStagger(HitSensor*, HitSensor*);
    bool requestBlowDown(HitSensor*, HitSensor*);
    bool requestAttackSuccess();
    void onNoGravitySupport();
    void setUp(const TVec3f&, const TQuat4f&, const TVec3f&);
    void appearBlowed(const TVec3f&, const TQuat4f&, const TVec3f&);
    void appearHipDropped(const TVec3f&, const TQuat4f&);
    bool tryNonActive();
    bool tryActive();
    bool tryEndBlow();
    bool tryEndBlowLand();
    bool tryFind();
    bool tryPointBind();
    bool tryDead();
    void exeWander();
    void exeFindPlayer();
    void exeChase();
    void exeStagger();
    void exeNonActive();
    void endNonActive();
    void exeAppearFromBox();
    void exeBlow();
    void exeAttackSuccess();
    void exeHipDropDown();
    void exeFlatDown();
    void exePressDown();
    void exeBlowDown();
    void exeBindStarPointer();
    void endBindStarPointer();
    void exeBlowLand();
    void calcPassiveMovement();
    bool isEnableKick() const;
    bool isUpsideDown() const;
    bool isEnableDead() const;
    bool isEnablePointBind() const;
    bool isEnableAttack() const;
    bool isEnableStamp() const;
    bool isEnablePanch() const;
    bool isEnablePushMove() const;

    AnimScaleController* mScaleController;         // 0x8C
    ItemGenerator* mItemGenerator;                 // 0x90
    WalkerStateWander* mStateWander;               // 0x94
    WalkerStateFindPlayer* mStateFindPlayer;       // 0x98
    WalkerStateBindStarPointer* mBindStarPointer;  // 0x9C
    WalkerStateStagger* mStateStagger;             // 0xA0
    WalkerStateChase* mStateChase;                 // 0xA4
    TQuat4f _A8;
    TVec3f _B8;
    bool _C4;
    u8 _C5;
};

namespace MR {
    LiveActor* createNoItemKuriboActor(const char*);
};  // namespace MR
