#pragma once

#include "Game/MapObj/MapObjActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class AnimScaleController;
class JointController;
class JointControllerInfo;
class WalkerStateBindStarPointer;

class CollapsePlane : public MapObjActor {
public:
    /// @brief Creates a new `CollapsePlane`.
    /// @param pName A pointer to the null-terminated name of the object.
    CollapsePlane(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void control();
    virtual void calcAndSetBaseMtx();

    void exeWait();
    void exeCollapse();
    void exeDPDStop();
    void exeEnd();
    bool calcJointPlane(TPos3f*, const JointControllerInfo&);
    bool tryDPDStop();

    /* 0xC4 */ AnimScaleController* mScaleController;
    /* 0xC8 */ WalkerStateBindStarPointer* mStateBindStartPointer;
    /* 0xCC */ JointController* mJointController;
    /* 0xD0 */ s32 mCollapseStep;
    /* 0xD4 */ s32 mCollapseTime;
};
