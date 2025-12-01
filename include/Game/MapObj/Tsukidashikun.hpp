#pragma once

#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActor.hpp"

class Tsukidashikun : public MapObjActor {
public:
    Tsukidashikun(const char*);

    virtual void init(const JMapInfoIter&);

    void exeWait();
    void exeSign();
    void exeMove();
    void connectToScene(const MapObjActorInitInfo&);
    void initCaseUseSwitchB(const MapObjActorInitInfo&);
    void initCaseNoUseSwitchB(const MapObjActorInitInfo&);
    void startMove();

    /* 0xC4 */ f32 _C4;
    /* 0xC8 */ s32 mTimer;
};

namespace NrvTsukidashikun {
    NEW_NERVE(TsukidashikunNrvMoveForward, Tsukidashikun, Move);
    NEW_NERVE(TsukidashikunNrvMoveBack, Tsukidashikun, Move);
    NEW_NERVE(TsukidashikunNrvSignForward, Tsukidashikun, Sign);
    NEW_NERVE(TsukidashikunNrvSignBack, Tsukidashikun, Sign);
    NEW_NERVE(TsukidashikunNrvWaitForward, Tsukidashikun, Wait);
    NEW_NERVE(TsukidashikunNrvWaitBack, Tsukidashikun, Wait);
    NERVE(TsukidashikunNrvRelax);
}; // namespace NrvTsukidashikun
