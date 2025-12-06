#pragma once

#include "Game/Player/MarioModule.hpp"
#include "JSystem/JGeometry.hpp"
#include "JSystem/JParticle/MultiEmitterCallBack.hpp"

class MarioActor;
class JPABaseEmitter;

struct SomeData {
    u32 _0;
    TMtx34f _4;
    TMtx34f _34;
    u32 _64;
    u32 _68;
};

class MarioEffect : public MarioModule, public MultiEmitterCallBackBase {
public:
    MarioEffect(MarioActor*);

    virtual void execute(JPABaseEmitter*);

    // We can infer from the multiple inheritance that this destructor was declared after execute
    // and that the virtual destructor in JPAEmitterCallBack comes first
    virtual ~MarioEffect();

    s32 _C;
    s32 _10;
    s32 _14;
    u8 _18;
    Color8 _1C;
    Color8 _20;
    TMtx34f _24;
    SomeData _54[0x100];
};
