#pragma once
#include "Game/Enemy/BegomanBase.hpp"
#include "Game/LiveActor/HitSensor.hpp"

class BegomanBaby : public BegomanBase {
public:
    BegomanBaby(const char*);
    BegomanBaby(NameObj*, const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual void appear();
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x58 */ virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    virtual u32 getKind() const {
        return 3;
    }

    /* 0x78 */ virtual bool onTouchElectric(const TVec3f&, const TVec3f&);
    /* 0x7C */ virtual bool setNerveReturn();
    /* 0x84 */ virtual const Nerve* getNerveWait();

    bool receiveMsgTrample(HitSensor*, HitSensor*);
    void appearFromLaunch(const TVec3f&, const TVec3f&);
    void appearFromGuarder();
    void killWithGenItem();
    void exeNoCalcWait();
    void endNoCalcWait();
    void exeWait();
    void exeSignAttack();
    void exePursue();
    void exeTurn();
    void exeBrake();
    void exeStepBack();
    void exeReturn();
    void exeProvoke();
    void exeTrample();
    void exeHitReaction();
    void exeTired();
    void exeBlow();
    void exeAfterLaunch();
    void exeAfterLaunchOnGround();
    void exeLaunchFromGuarder();
    void exeLaunchTurn();
    void exeInWater();
    void exeKeepDistance();
    void exeBindStarPointer();
    void endBindStarPointer();
    bool calcHeadJoint(TPos3f*, const JointControllerInfo&);

    /* 0x100 */ JointControlDelegator< BegomanBaby >* mBabyDelegator;
    /* 0x104 */ TPos3f mWaterColumnMatrix;
    /* 0x134 */ NameObj* mHost;
    /* 0x138 */ bool mAppearThreeStarPiece;
};
