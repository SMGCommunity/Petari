#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class SpaceShipStep : public MapObjActor {
public:
    SpaceShipStep(const char*);

    /* 0x08 */ virtual ~SpaceShipStep();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    /* 0x7C */ virtual void initCaseUseSwitchA(const MapObjActorInitInfo&) {
        return;
    }
    /* 0x80 */ virtual void initCaseNoUseSwitchA(const MapObjActorInitInfo&) {
        return;
    }
    /* 0x84 */ virtual void initCaseUseSwitchB(const MapObjActorInitInfo&) {
        return;
    }
    /* 0x88 */ virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&) {
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
