#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class AnimScaleController;
class WalkerStateBindStarPointer;
class TerritoryMover;

struct KaronParam {
    f32 _0;
    f32 _4;
    f32 _8;
    f32 _C;
};

class Karon : public LiveActor {
public:
    Karon(const char*);

    virtual ~Karon();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPush(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeFixWait();
    void exeWait();
    void exeWalk();
    void exeTurn();
    void exeWalkOnRail();
    void exeSearch();
    void exePursue();
    void endPursue();
    void exeBroken();
    void exeStepBroken();
    void exeRecover();
    void exeHitReaction();
    void exeDeath();
    void exeSinkDown();
    void exeBindStarPointer();
    void endBindStarPointer();
    bool isFallNextMove(bool);

    inline void moveAndTurn(const KaronParam* pParam) { MR::moveAndTurnToPlayer(this, pParam->_0, pParam->_4, pParam->_8, pParam->_C); }

    TerritoryMover* mTerritoryMover;  // 0x8C
    u32 _90;
    u8 _94;
    AnimScaleController* mScaleController;          // 0x98
    WalkerStateBindStarPointer* mStateStarPointer;  // 0x9C
};
