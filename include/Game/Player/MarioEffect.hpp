#pragma once

#include "Game/Effect/MultiEmitterCallBack.hpp"
#include "Game/Player/MarioModule.hpp"
#include "Game/Util/Color.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class MarioActor;
class JPABaseEmitter;

struct MovingFollowMtx {
    u32 _0;
    TMtx34f _4;
    TMtx34f _34;
    MtxPtr _64;
    JPABaseEmitter* _68;
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

    s32 _C;
    s32 _10;
    s32 _14;
    u8 _18;
    Color8 _1C;
    Color8 _20;
    TMtx34f _24;
    MovingFollowMtx _54[0x100];
};
