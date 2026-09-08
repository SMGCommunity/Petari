#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class MechaKoopaPartsArm : public MapObjActor {
public:
    /// @brief Creates a new `MechaKoopaPartsArm`.
    /// @param pName A pointer to the null-terminated name of the object.
    MechaKoopaPartsArm(const char* pName);

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

    /* 0xC4 */ bool mIsRightArm;
};
