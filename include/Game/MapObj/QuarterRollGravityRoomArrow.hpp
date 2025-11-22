#pragma once

#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util/JMapInfo.hpp"

class QuarterRollGravityRoomArrow : public MapObjActor {
public:
    QuarterRollGravityRoomArrow(const char* pName);
    virtual ~QuarterRollGravityRoomArrow();
    virtual void init(const JMapInfoIter& rIter);
    virtual void initCaseUseSwitchA(const MapObjActorInitInfo& rInfo);
    virtual void initCaseNoUseSwitchA(const MapObjActorInitInfo& rInfo);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo& rInfo);
    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo& rInfo);
    void         exeRollOnA();
    void         exeRollOffA();
    void         exeRollOnB();
    void         exeRollOffB();
    void         onRollA();
    void         offRollA();
    void         onRollB();
    void         offRollB();
};

namespace NrvQuarterRollGravityRoomArrow {

    NERVE_DECL_NULL(QuarterRollGravityRoomArrowNrvWait);
    NERVE_DECL(QuarterRollGravityRoomArrowNrvRollOnA, QuarterRollGravityRoomArrow, exeRollOnA)
    NERVE_DECL(QuarterRollGravityRoomArrowNrvRollOffA, QuarterRollGravityRoomArrow, exeRollOffA)
    NERVE_DECL(QuarterRollGravityRoomArrowNrvRollOnB, QuarterRollGravityRoomArrow, exeRollOnB)
    NERVE_DECL(QuarterRollGravityRoomArrowNrvRollOffB, QuarterRollGravityRoomArrow, exeRollOffB)
} // namespace NrvQuarterRollGravityRoomArrow
