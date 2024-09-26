#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class MechaKoopaPartsArm : public MapObjActor {
public:
    MechaKoopaPartsArm(const char *);

    virtual ~MechaKoopaPartsArm();
    virtual void init(const JMapInfoIter &);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo &);
    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo &);

    void exeWait();
    void exeMoveStart();
    void exeMove();
    void exeBreakStart();
    void exeBreak();
    void startBreak();

    bool mIsRightArm;   // 0xC4
};

namespace NrvMechaKoopaPartsArm {
    NERVE(MechaKoopaPartsArmNrvWaitFront);
    NERVE(MechaKoopaPartsArmNrvWaitRear);
    NERVE_EXECEND(MechaKoopaPartsArmNrvMoveStartFront);
    NERVE_EXECEND(MechaKoopaPartsArmNrvMoveStartRear);
    NERVE(MechaKoopaPartsArmNrvMoveFront);
    NERVE(MechaKoopaPartsArmNrvMoveRear);
    NERVE(MechaKoopaPartsArmNrvBreakStart);
    NERVE(MechaKoopaPartsArmNrvBreak);
};