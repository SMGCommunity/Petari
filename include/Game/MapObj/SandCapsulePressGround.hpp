#pragma once

#include "Game/MapObj/MapObjActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class SandCapsulePressGround : public MapObjActor {
public:
    SandCapsulePressGround(const char*);

    virtual void init(const JMapInfoIter&);

    virtual void initCaseUseSwitchA(const MapObjActorInitInfo&) {
    }

    virtual void initCaseUseSwitchB(const MapObjActorInitInfo&) {
    }

    virtual void initCaseNoUseSwitchA(const MapObjActorInitInfo&) {
    }

    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&) {
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
