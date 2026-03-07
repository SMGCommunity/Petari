#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class MechaKoopaPartsArm : public MapObjActor {
public:
    MechaKoopaPartsArm(const char*);

    virtual ~MechaKoopaPartsArm();
    virtual void init(const JMapInfoIter&);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);
    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&);

    void exeWait();
    void exeMoveStart();
    void endMoveStart();
    void exeMove();
    void exeBreakStart();
    void exeBreak();
    void startBreak();

    bool mIsRightArm;  // 0xC4
};
