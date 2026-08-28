#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class MechaKoopaPartsArm : public MapObjActor {
public:
    MechaKoopaPartsArm(const char*);

    /* 0x08 */ virtual ~MechaKoopaPartsArm();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x84 */ virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);
    /* 0x88 */ virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&);

    void exeWait();
    void exeMoveStart();
    void endMoveStart();
    void exeMove();
    void exeBreakStart();
    void exeBreak();
    void startBreak();

    bool mIsRightArm;  // 0xC4
};
