#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class HitSensor;
class Koopa;

namespace MR {
    struct ActorMoveParam;
};  // namespace MR

class KoopaStateDamageEscape : public ActorStateBase< Koopa > {
public:
    KoopaStateDamageEscape(Koopa* pKoopa);

    virtual ~KoopaStateDamageEscape();
    virtual void init();
    virtual void appear();
    virtual void kill();

    void startDamageReverse();
    bool isDownEnd() const;
    bool tryDamage(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    bool attackSensor(HitSensor* pSender, HitSensor* pReceiver);

    void exeEscapeStart();
    void exeEscapeStartLoop();
    void exeEscapeStartLand();
    void exeEscapeRun();
    void exeEscapeFind();
    void exeEscapeEnd();
    void exeDamageTailRunStart();
    void exeDamageTailRun();
    void exeDamageTailRunEnd();
    void exeDamageTailRunStartFinal();
    void exeDamageTailRunFinal();
    void exeDamageTailRunEndFinal();
    void exeDown();
    void exeDownLand();
    void exeDownEnd();

private:
    /* 0x10 */ s32 mEscapeTime;
    /* 0x14 */ s32 mMaxEscapeTime;
    /* 0x18 */ s32 _18;
    /* 0x1C */ bool mIsTurnClockwise;
    /* 0x1D */ bool mIsLastHit;
    /* 0x20 */ MR::ActorMoveParam* mEscapeRunParam;
    /* 0x24 */ MR::ActorMoveParam* mDamageTailRunParam;
    /* 0x28 */ u32 mMaxRunFrames;
    /* 0x2C */ f32 mRotateVelocity;
    /* 0x30 */ f32 mJumpAwayVelocity;
};
