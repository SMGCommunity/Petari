#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class LavaGeyser : public LiveActor {
public:
    LavaGeyser(const char*);

    virtual void init(const JMapInfoIter&);

    /* 8C */ s32 _8c;
    /* 90 */ s32 _90;
    /* 94 */ TVec3f _94;
    /* A0 */ TVec3f _A0;
    /* AC */ f32 _AC;
    /* B0 */ f32 _B0;
    /* B4 */ f32 _B4;
    /* B8 */ f32 _B8;

    void exeWait();
    void exeWaitSwitch();
    void exeSign();
    void exeShootUp();
    void exeShootKeep();
};
