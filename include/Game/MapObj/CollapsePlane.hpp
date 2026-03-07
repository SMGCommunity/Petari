#pragma once

#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/MapObj/MapObjActor.hpp"
#include "Game/Util/JointController.hpp"

class CollapsePlane : public MapObjActor {
public:
    CollapsePlane(const char*);

    virtual ~CollapsePlane();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void calcAndSetBaseMtx();

    void exeWait();
    void exeCollapse();
    void exeDPDStop();
    void exeEnd();
    bool calcJointPlane(TPos3f*, const JointControllerInfo&);
    bool tryDPDStop();

    AnimScaleController* mScaleController;         // 0xC4
    WalkerStateBindStarPointer* mStarPointerBind;  // 0xC8
    JointController* mJointController;             // 0xCC
    s32 _D0;
    s32 mTimer;  // 0xD4
};
