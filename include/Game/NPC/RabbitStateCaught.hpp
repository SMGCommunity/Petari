#pragma once 

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NPC/TalkMessageCtrl.hpp"
#include "Game/System/NerveExecutor.hpp"
#include <cstddef>

class RabbitStateCaught : public NerveExecutor {
public:
    RabbitStateCaught(LiveActor* pActor, TalkMessageCtrl* pCtrl);
    virtual ~RabbitStateCaught();
    virtual void init();
    virtual void appear();
    virtual void control();
    void unusePowerStarModel();
    void exeCaught();
    void exeCaughtLand();
    void exeCaughtEvent();
    void setCaughtStartMarioPose();
    void blendBaseMatrixToMario(f32 f) const;

    s32 _C;
    TQuat4f _10;
    TVec3f _20;
    s32 _2C;
    s32 _30;
    TMtx34f _34;
    s32 _60;
    bool _64;
};

const size_t test = sizeof(RabbitStateCaught);

namespace NrvRabbitStateCaught {
    NERVE_DECL_NULL(RabbitStateCaughtNrvTryStartDemo);
    NERVE_DECL_EXE(RabbitStateCaughtNrvCaught, RabbitStateCaught, Caught);
    NERVE_DECL_EXE(RabbitStateCaughtNrvCaughtLand, RabbitStateCaught, CaughtLand);
    NERVE_DECL_EXE(RabbitStateCaughtNrvCaughtEvent, RabbitStateCaught, CaughtEvent);
}
