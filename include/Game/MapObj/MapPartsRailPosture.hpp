#pragma once

#include "Game/MapObj/MapPartsFunction.hpp"
#include <JSystem/JGeometry.hpp>

class MapPartsRailPosture : public MapPartsFunction {
public:
    MapPartsRailPosture(LiveActor*);

    /* 0x08 */ virtual ~MapPartsRailPosture();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual bool isWorking() const;
    /* 0x28 */ virtual void start();
    /* 0x2C */ virtual void end();

    bool isPostureTypeRailDirRailUseShadowGravity() const;

    void exeDoNothing();
    void exeMove();

    TPos3f _18;
    s32 mMovePosture;  // 0x48
};
