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
    ClipArea* createClipAreaSphere(const char* pModel);
    ClipArea* createClipAreaCenterBox(const char* pModel);
    ClipArea* createClipAreaBottomBox(const char* pModel);
    ClipArea* createClipAreaTopCone(const char* pModel);
    ClipArea* createClipAreaBottomCone(const char* pModel);
}  // namespace MR
