#pragma once

#include "Game/MapObj/MapPartsFunction.hpp"
#include <JSystem/JGeometry.hpp>

class MapPartsRailPosture : public MapPartsFunction {
public:
    MapPartsRailPosture(LiveActor*);

    virtual ~MapPartsRailPosture();
    virtual void init(const JMapInfoIter&);
    virtual bool isWorking() const;
    virtual void start();
    virtual void end();

    void exeMove();

    TMtx34f _18;
    u32 _48;
};