#pragma once

#include "Game/Player/MarioModule.hpp"
#include "Game/Util/Color.hpp"
#include "JSystem/JGeometry.hpp"
#include "JSystem/JParticle/MultiEmitterCallBack.hpp"
#include <revolution/mtx.h>

class MarioActor;
class JPABaseEmitter;

struct MovingFollowMtx {
    /* 0x00 */ u32 _0;
    /* 0x04 */ TMtx34f _4;
    /* 0x34 */ TMtx34f _34;
    /* 0x64 */ MtxPtr _64;
    /* 0x68 */ JPABaseEmitter* mEmitter;
};

class MarioEffect : public MarioModule, public MultiEmitterCallBackBase {
public:
    MarioEffect(MarioActor*);

    virtual void execute(JPABaseEmitter*);

    // We can infer from the multiple inheritance that this destructor was declared after execute
    // and that the virtual destructor in JPAEmitterCallBack comes first
    virtual ~MarioEffect();

    JPABaseEmitter* addRequest(const char*, MtxPtr);
    void updateFollowMtx(MovingFollowMtx*);
    void playSwingEffect();
    void doCubeEffect();

    /* 0x0C */ s32 _C;
    /* 0x10 */ s32 _10;
    /* 0x14 */ s32 _14;
    /* 0x18 */ u8 _18;
    /* 0x19 */ u8 _19[3];
    /* 0x1C */ Color8 _1C;
    /* 0x20 */ Color8 _20;
    /* 0x24 */ TMtx34f _24;
    /* 0x54 */ MovingFollowMtx _54[0x100];
};
