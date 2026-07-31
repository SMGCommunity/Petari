#pragma once

#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include <JSystem/JGeometry.hpp>

class SandCapsulePressGround : public MapObjActor {
public:
    SandCapsulePressGround(const char*);

    virtual ~SandCapsulePressGround();
    virtual void init(const JMapInfoIter&);
    virtual void initCaseUseSwitchA(const MapObjActorInitInfo&) {
        return;
    }
    virtual void initCaseNoUseSwitchA(const MapObjActorInitInfo&) {
        return;
    }
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo&) {
        return;
    }
    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&) {
        return;
    }

    void exeWait();
    void exeMoveStart();
    void exeMove();
    void exeMoveEnd();
    void exeDisappearDemo();
    void exeStop();
    void calcMoveVelocity() NO_INLINE;
    bool tryStop();

    TVec3f _C4;
    TVec3f _D0;
    TPos3f _DC;
};
