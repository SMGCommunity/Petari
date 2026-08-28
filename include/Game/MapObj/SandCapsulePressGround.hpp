#pragma once

#include "Game/MapObj/MapObjActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class SandCapsulePressGround : public MapObjActor {
public:
    SandCapsulePressGround(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);

    /* 0x7C */ virtual void initCaseUseSwitchA(const MapObjActorInitInfo&) {
    }

    /* 0x84 */ virtual void initCaseUseSwitchB(const MapObjActorInitInfo&) {
    }

    /* 0x80 */ virtual void initCaseNoUseSwitchA(const MapObjActorInitInfo&) {
    }

    /* 0x88 */ virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&) {
    }

    void exeWait();
    void exeMoveStart();
    void exeMove();
    void exeMoveEnd();
    void exeDisappearDemo();
    void exeStop();
    void calcMoveVelocity() NO_INLINE;
    bool tryStop();

    /* 0xC4 */ TVec3f _C4;
    /* 0xD0 */ TVec3f _D0;
    /* 0xDC */ TPos3f _DC;
};
