#pragma once

#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/MapObj/MapObjActor.hpp"
#include "Game/Util/JointController.hpp"

class CollapsePlane : public MapObjActor {
public:
    CollapsePlane(const char*);

    /* 0x08 */ virtual ~CollapsePlane();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();

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
