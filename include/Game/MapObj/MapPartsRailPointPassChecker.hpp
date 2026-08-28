#pragma once

#include "Game/MapObj/MapPartsFunction.hpp"

class MapPartsRailPointPassChecker : public MapPartsFunction {
public:
    MapPartsRailPointPassChecker(LiveActor*);

    /* 0x08 */ virtual ~MapPartsRailPointPassChecker();
    /* 0x14 */ virtual void movement();
    /* 0x24 */ virtual bool isWorking() const;
    /* 0x28 */ virtual void start();
    /* 0x2C */ virtual void end() {
    }

    bool isPassed() const;
    bool isReachedEnd() const;
    bool isPassedStartPoint() const;
    bool isPassedEndPoint() const;

    s32 mRailPointNo;  // 0x18
};