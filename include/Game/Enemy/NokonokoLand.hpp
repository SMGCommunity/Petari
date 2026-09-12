#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AnimScaleController;
class JetTurtle;
class MapPartsRailPointPassChecker;
class WalkerStateBindStarPointer;

class NokonokoLand : public LiveActor {
public:
    NokonokoLand(const char*);

    virtual ~NokonokoLand();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initJetTurtle();
    bool isEnableAttack() const;
    bool isStateWalk() const;
    bool isStateShell() const;
    bool isStateShellStart() const;
    bool isStateShellWait() const;
    bool isStateRattle() const;
    bool isStateBindStarPointer() const;
    bool isStateAttack() const;
    bool isLookDirRailDirection() const;
    bool tryAttack(HitSensor*, HitSensor*);
    bool tryJetTurtleTouch();
    bool tryRattle();
    bool tryBindStarPointer();
    bool receiveTrample(HitSensor*, HitSensor*);
    bool receivePunch(HitSensor*, HitSensor*);
    bool receiveHipDrop(HitSensor*, HitSensor*);
    bool receiveTurtleAttack(HitSensor*, HitSensor*);
    bool receiveStorm(HitSensor*, HitSensor*);
    bool receiveStarPieceAttack(HitSensor*, HitSensor*);
    void exeAppearReady();
    void exeAppearStart();
    void endAppearStart();
    void tearAppearStart();
    void exeAppear();
    void exeFlyAway();
    void exeDead();
    void exeWalk();
    void exeLookAround();
    void exeTurnStart();
    void exeTurnLoop();
    void exeTurnEnd();
    void exeAttack();
    void exeRattle();
    void exeBindStarPointer();
    void endBindStarPointer();
    void exeShellStartTrample();
    void exeShellStartPunch();
    void exeShellWait();
    void exeShellEnd();
    void exeBecomeJetTurtle();

    /* 0x8C */ JetTurtle* mJetTurtle;
    /* 0x90 */ MapPartsRailPointPassChecker* mRailPointPassChecker;
    /* 0x94 */ AnimScaleController* mAnimScaleController;
    /* 0x98 */ WalkerStateBindStarPointer* mBindStarPointerState;
    /* 0x9C */ s32 mTurtleType;
    /* 0xA0 */ TVec3f mAppearPos;
    /* 0xAC */ TQuat4f mTargetQuat;
    /* 0xBC */ TVec3f mTurnAxis;
};
