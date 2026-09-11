#pragma once

#include "Game/MapObj/ClipArea.hpp"

class MapPartsRailMover;
class MapPartsRotator;

class ClipAreaMovable : public ClipArea {
public:
    /// @brief Creates a new `ClipAreaMovable`.
    /// @param pName A pointer to the null-terminated name of the object.
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

    /* 0xC0 */ MapPartsRailMover* mMapPartsRailMover;
    /* 0xC0 */ MapPartsRotator* mMapPartsRotator;
};

namespace MR {
    NameObj* createClipAreaSphere(const char* pModel);
    NameObj* createClipAreaCenterBox(const char* pModel);
    NameObj* createClipAreaBottomBox(const char* pModel);
    NameObj* createClipAreaTopCone(const char* pModel);
    NameObj* createClipAreaBottomCone(const char* pModel);
};  // namespace MR
