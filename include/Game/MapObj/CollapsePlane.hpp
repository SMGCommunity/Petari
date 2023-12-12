#pragma once

#include "Game/MapObj/MapObjActor.h"
#include "Game/Enemy/WalkerStateBindStarPointer.h"
#include "Game/Util/JointController.h"

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

    AnimScaleController* mScaleController;          // _C4
    WalkerStateBindStarPointer* mStarPointerBind;   // _C8
    JointController* mJointController;              // _CC
    s32 _D0;
    s32 mTimer;                                     // _D4
};

namespace NrvCollapsePlane {
    NERVE_DECL(CollapsePlaneNrvWait, CollapsePlane, CollapsePlane::exeWait);
    NERVE_DECL(CollapsePlaneNrvCollapse, CollapsePlane, CollapsePlane::exeCollapse);
    NERVE_DECL(CollapsePlaneNrvDPDStop, CollapsePlane, CollapsePlane::exeDPDStop);
    NERVE_DECL_NULL(CollapsePlaneNrvEnd);
};