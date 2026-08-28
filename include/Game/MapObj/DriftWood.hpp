#pragma once

#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActor.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

class DriftWood : public MapObjActor {
public:
    DriftWood(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x78 */ virtual void connectToScene(const MapObjActorInitInfo&);
    /* 0x4C */ virtual void calcAndSetBaseMtx();

    void attackSensor(HitSensor*, HitSensor*);
    bool tryVibrate();
    void exeWait();
    void exeVibrate();

    /* 0xC4 */ TVec3f mRailDirection;
    /* 0xD0 */ TVec3f mRailClipping;
    /* 0xDC */ TVec3f mVibrateOffset;
    /* 0xE8 */ s32 mWaveSoundTimer;
};
