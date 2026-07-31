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

    bool isPostureTypeRailDirRailUseShadowGravity() const;

    void exeDoNothing();
    void exeMove();

    TPos3f _18;
    s32 mMovePosture;  // 0x48
};
