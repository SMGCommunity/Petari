#pragma once

#include "Game/MapObj/ClipArea.hpp"
#include "Game/MapObj/MapPartsRailMover.hpp"
#include "Game/MapObj/MapPartsRotator.hpp"
#include "Game/Util/JMapInfo.hpp"

class ClipAreaMovable : public ClipArea {
    ClipAreaMovable(const char* pName);
    virtual void init(const JMapInfoIter& rIter);
    virtual void makeActorAppeared();
    virtual void makeActorDead();
    virtual void control();
    void initMoveFunction(const JMapInfoIter& rIter);
    void startMoveFunction();
    void endMoveFunction();
    void movementMoveFunction();
    void updateMatrix();

    MapPartsRailMover* mMapPartsRailMover;  // 0xC0
    MapPartsRotator* mMapPartsRotator;      // 0xC4
};

namespace MR {
    NameObj* createClipAreaSphere(const char* pModel);
    NameObj* createClipAreaCenterBox(const char* pModel);
    NameObj* createClipAreaBottomBox(const char* pModel);
    NameObj* createClipAreaTopCone(const char* pModel);
    NameObj* createClipAreaBottomCone(const char* pModel);
}  // namespace MR
