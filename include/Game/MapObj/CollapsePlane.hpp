#pragma once

#include "Game/MapObj/MapObjActor.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/Util/JointController.hpp"

class CollapsePlane : public MapObjActor {
public:
    CollapsePlane(const char *);

    virtual ~CollapsePlane();
    virtual void init(const JMapInfoIter &);
    virtual void control();
    virtual void calcAndSetBaseMtx();

    void exeWait();
    void exeCollapse();
    void exeDPDStop();
    bool calcJointPlane(TPos3f *, const JointControllerInfo &);
    bool tryDPDStop();

    AnimScaleController* mScaleController;          // 0xC4
    WalkerStateBindStarPointer* mStarPointerBind;   // 0xC8
    JointController* mJointController;              // 0xCC
    s32 _D0;
    s32 mTimer;                                     // 0xD4
};

namespace NrvCollapsePlane {
    NERVE_DECL(CollapsePlaneNrvWait, CollapsePlane, CollapsePlane::exeWait);
    NERVE_DECL(CollapsePlaneNrvCollapse, CollapsePlane, CollapsePlane::exeCollapse);
    NERVE_DECL(CollapsePlaneNrvDPDStop, CollapsePlane, CollapsePlane::exeDPDStop);
    NERVE_DECL_NULL(CollapsePlaneNrvEnd);
};