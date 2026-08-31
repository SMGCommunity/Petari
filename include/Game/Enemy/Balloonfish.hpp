#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AnimScaleController;

/// @brief Spiny Cheep Cheep
class Balloonfish : public LiveActor {
public:
    Balloonfish(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeDash();
    void exeDashEnd();
    void exeStarPointerBind();
    void endStarPointerBind();

    /* 0x8C */ AnimScaleController* mAnimeScaleController;
    /* 0x90 */ const Nerve* mNerveBeforeBind;
    /* 0x94 */ s32 mNotBoundStep;
    /* 0x98 */ TQuat4f mQuat;
    /* 0xA8 */ f32 _A8;
};
