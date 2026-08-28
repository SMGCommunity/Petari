#pragma once

#include "Game/MapObj/ClipArea.hpp"
#include "Game/MapObj/MapPartsRailMover.hpp"
#include "Game/MapObj/MapPartsRotator.hpp"
#include "Game/Util/JMapInfo.hpp"

class ClipAreaMovable : public ClipArea {
    ClipAreaMovable(const char* pName);
    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x28 */ virtual void makeActorAppeared();
    /* 0x30 */ virtual void makeActorDead();
    /* 0x48 */ virtual void control();
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
};  // namespace MR
