#pragma once

#include "Game/Effect/MultiEmitterCallBack.hpp"
#include "Game/Player/MarioModule.hpp"
#include "Game/Util/Color.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class MarioActor;
class JPABaseEmitter;

struct MovingFollowMtx {
    MovingFollowMtx() {
        _4.identity();
        _34.identity();
        _64 = nullptr;
        _68 = nullptr;
        _0 = 0;
    }

    /* 0x00 */ u32 _0;
    /* 0x04 */ TMtx34f _4;
    /* 0x34 */ TMtx34f _34;
    /* 0x64 */ MtxPtr _64;
    /* 0x68 */ JPABaseEmitter* _68;
};

class MarioEffect : public MarioModule, public MultiEmitterCallBackBase {
public:
    MarioEffect(MarioActor*);

    virtual void execute(JPABaseEmitter*);

    virtual ~MarioEffect();

    JPABaseEmitter* addRequest(const char*, MtxPtr);
    void updateFollowMtx(MovingFollowMtx*);
    void playSwingEffect();
    void doCubeEffect();

    /* 0x0C */ s32 _C;
    /* 0x10 */ s32 _10;
    /* 0x14 */ s32 _14;
    /* 0x18 */ u8 _18;
    /* 0x1C */ Color8 _1C;
    /* 0x20 */ Color8 _20;
    /* 0x24 */ TMtx34f _24;
    /* 0x54 */ MovingFollowMtx _54[0x100];
};
