#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class SpaceShipStep : public MapObjActor {
public:
    SpaceShipStep(const char*);

    virtual ~SpaceShipStep();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
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
    void exeMoveSign();
    void exeMove();
    void exeBreak();
    void updateResetToInitPosCheck();

    TVec3f _C4;
    s32 _D0;
    bool _D4;
};
