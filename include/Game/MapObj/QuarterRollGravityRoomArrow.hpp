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
    inline void exeRollOnA();
    void exeRollOffA();
    inline void exeRollOnB();
    inline void exeRollOffB();
    virtual void initCaseUseSwitchA(const MapObjActorInitInfo& rInfo);
    virtual void initCaseNoUseSwitchA(const MapObjActorInitInfo& rInfo);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo& rInfo);
    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo& rInfo);
    void onRollA();
    void offRollA();
    void onRollB();
    void offRollB();
};

namespace NrvQuarterRollGravityRoomArrow {
    NERVE(QuarterRollGravityRoomArrowNrvWait);
    NERVE_EXECEND(QuarterRollGravityRoomArrowNrvRollOnA);
    NERVE_EXECEND(QuarterRollGravityRoomArrowNrvRollOffA);
    NERVE_EXECEND(QuarterRollGravityRoomArrowNrvRollOnB);
    NERVE_EXECEND(QuarterRollGravityRoomArrowNrvRollOffB);
}
