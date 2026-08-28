#pragma once

#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util/JMapInfo.hpp"

class QuarterRollGravityRoomArrow : public MapObjActor {
public:
    QuarterRollGravityRoomArrow(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x7C */ virtual void initCaseUseSwitchA(const MapObjActorInitInfo& rInfo);
    /* 0x80 */ virtual void initCaseNoUseSwitchA(const MapObjActorInitInfo& rInfo);
    /* 0x84 */ virtual void initCaseUseSwitchB(const MapObjActorInitInfo& rInfo);
    /* 0x88 */ virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo& rInfo);

    void exeWait();
    void exeRollOnA();
    void exeRollOffA();
    void exeRollOnB();
    void exeRollOffB();
    void onRollA();
    void offRollA();
    void onRollB();
    void offRollB();
};
