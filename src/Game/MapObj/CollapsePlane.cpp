#include "Game/MapObj/CollapsePlane.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util.hpp"

void CollapsePlane_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

namespace {
    static const s32 sDefaultCollapseTime = 140;
    static const f32 sScaleMin = 0.7f;
};  // namespace

namespace NrvCollapsePlane {
    NEW_NERVE(CollapsePlaneNrvWait, CollapsePlane, Wait);
    NEW_NERVE(CollapsePlaneNrvCollapse, CollapsePlane, Collapse);
    NEW_NERVE(CollapsePlaneNrvDPDStop, CollapsePlane, DPDStop);
    NEW_NERVE(CollapsePlaneNrvEnd, CollapsePlane, End);
};  // namespace NrvCollapsePlane

CollapsePlane::CollapsePlane(const char* pName)
    : MapObjActor(pName), mScaleController(), mStateBindStartPointer(), mJointController(), mCollapseStep(-1), mCollapseTime(::sDefaultCollapseTime) {
}

void CollapsePlane::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    info.setupHioNode("地形オブジェ");
    info.setupDefaultPos();
    info.setupConnectToScene();
    info.setupHitSensor();
    info.setupSound(4);
    info.setupProjmapMtx(false);
    info.setupNerve(&NrvCollapsePlane::CollapsePlaneNrvWait::sInstance);
    initialize(rIter, info);
    initEffectKeeper(1, nullptr, false);
    MR::initStarPointerTarget(this, mScale.x * 200.0f, TVec3f(0.0f, 0.0f, 0.0f));

    mScaleController = new AnimScaleController(nullptr);
    mScaleController->setParamTight();

    mStateBindStartPointer = new WalkerStateBindStarPointer(this, mScaleController);

    mJointController = MR::createJointDelegatorWithNullChildFunc(this, &CollapsePlane::calcJointPlane, "Plane");

    MR::initCollisionPartsAutoEqualScale(this, "Move", getSensor(nullptr), MR::getJointMtx(this, "Plane"));
    MR::validateCollisionParts(this);
    MR::getJMapInfoArg0NoInit(rIter, &mCollapseTime);
}

void CollapsePlane::exeWait() {
    if (MR::isOnPlayer(this)) {
        mCollapseStep = 0;
        setNerve(&NrvCollapsePlane::CollapsePlaneNrvCollapse::sInstance);
    }
}

void CollapsePlane::exeCollapse() {
    if (mCollapseStep == 1) {
        MR::startSound(this, "SE_OJ_COLLAPSE_PLANE_SHRINK");
    }

    if (mCollapseStep >= mCollapseTime) {
        MR::hideMaterial(this, "PlaneMat_v");
        MR::invalidateCollisionParts(this);
        MR::emitEffect(this, "Vanish");
        MR::startSound(this, "SE_OJ_COLLAPSE_PLANE_VANISH");
        mCollapseStep = -1;
        setNerve(&NrvCollapsePlane::CollapsePlaneNrvEnd::sInstance);
    } else {
        mCollapseStep++;
    }
}

void CollapsePlane::exeDPDStop() {
    if (mCollapseStep == -1 && MR::isOnPlayer(this)) {
        mCollapseStep = 0;
    }

    if (MR::updateActorState(this, mStateBindStartPointer)) {
        if (mCollapseStep != -1) {
            setNerve(&NrvCollapsePlane::CollapsePlaneNrvCollapse::sInstance);
        } else {
            setNerve(&NrvCollapsePlane::CollapsePlaneNrvWait::sInstance);
        }
    }
}

void CollapsePlane::exeEnd() {
}

void CollapsePlane::calcAndSetBaseMtx() {
    MapObjActor::calcAndSetBaseMtx();

    if (mCollapseStep != -1) {
        mJointController->registerCallBack();
    }

    if (MR::isInitializeStateEnd()) {
        MR::setBaseScale(this, mScaleController->_C * mScale);
    }
}

void CollapsePlane::control() {
    mScaleController->updateNerve();
    tryDPDStop();
}

bool CollapsePlane::calcJointPlane(TPos3f* pMtx, const JointControllerInfo&) {
    f32 xzScale = 1.0f - (::sScaleMin * mCollapseStep) / mCollapseTime;

    TPos3f mtx;
    mtx.identity();
    MR::preScaleMtx(mtx, xzScale, 1.0f, xzScale);
    pMtx->concat(mtx);
    pMtx->setTrans(mPosition);

    return true;
}

bool CollapsePlane::tryDPDStop() {
    if (isNerve(&NrvCollapsePlane::CollapsePlaneNrvDPDStop::sInstance)) {
        return false;
    }

    if (isNerve(&NrvCollapsePlane::CollapsePlaneNrvEnd::sInstance)) {
        return false;
    }

    if (!mStateBindStartPointer->tryStartPointBind()) {
        return false;
    }

    setNerve(&NrvCollapsePlane::CollapsePlaneNrvDPDStop::sInstance);

    return true;
}
