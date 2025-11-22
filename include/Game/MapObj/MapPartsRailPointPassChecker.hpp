#pragma once

#include "Game/MapObj/MapPartsFunction.hpp"

class MapPartsRailPointPassChecker : public MapPartsFunction {
public:
    MapPartsRailPointPassChecker(LiveActor*);

    virtual ~MapPartsRailPointPassChecker();
    virtual void movement();
    virtual bool isWorking() const;
    virtual void start();
    virtual void end() {}

    bool isPassed() const;
    bool isReachedEnd() const;
    bool isPassedStartPoint() const;
    bool isPassedEndPoint() const;

    s32 mRailPointNo;  // 0x18
};