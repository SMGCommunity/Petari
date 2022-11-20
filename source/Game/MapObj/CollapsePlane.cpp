#include "Game/MapObj/CollapsePlane.h"

CollapsePlane::CollapsePlane(const char *pName) : MapObjActor(pName) {
    mScaleController = NULL;
    mStarPointerBind = NULL;
    mJointController = NULL;
    _D0 = -1;
    mTimer = 140;
}

void CollapsePlane::init(const JMapInfoIter &rIter) {
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
    initEffectKeeper(1, NULL, false);
    TVec3f offs;
    offs.x = 0.0f;
    offs.y = 0.0f;
    offs.z = 0.0f;
    MR::initStarPointerTarget(this, (200.0f * mScale.x), offs);
    mScaleController = new AnimScaleController(NULL);
    mScaleController->setParamTight();
    mStarPointerBind = new WalkerStateBindStarPointer(this, mScaleController);
    mJointController = MR::createJointDelegatorWithNullChildFunc(this, &CollapsePlane::calcJointPlane, "Plane");
    MR::initCollisionPartsAutoEqualScale(this, "Move", getSensor(NULL), MR::getJointMtx(this, "Plane"));
    MR::validateCollisionParts(this);
    MR::getJMapInfoArg0NoInit(rIter, &mTimer);
}

void CollapsePlane::exeWait() {
    if (MR::isOnPlayer(this)) {
        _D0 = 0;
        setNerve(&NrvCollapsePlane::CollapsePlaneNrvCollapse::sInstance);
    }
}

void CollapsePlane::exeCollapse() {
    if (_D0 == 1) {
        MR::startSound(this, "SE_OJ_COLLAPSE_PLANE_SHRINK", -1, -1);
    }

    if (_D0 >= mTimer) {
        MR::hideMaterial(this, "PlaneMat_v");
        MR::invalidateCollisionParts(this);
        MR::emitEffect(this, "Vanish");
        MR::startSound(this, "SE_OJ_COLLAPSE_PLANE_VANISH", -1, -1);
        _D0 = -1;
        setNerve(&NrvCollapsePlane::CollapsePlaneNrvEnd::sInstance);
    }
    else {
        _D0 += 1;
    }
}

void CollapsePlane::exeDPDStop() {
    if (_D0 == -1 && MR::isOnPlayer(this)) {
        _D0 = 0;
    }

    if (MR::updateActorState(this, mStarPointerBind)) {
        if (_D0 == -1) {
            setNerve(&NrvCollapsePlane::CollapsePlaneNrvWait::sInstance);
        }
        else {
            setNerve(&NrvCollapsePlane::CollapsePlaneNrvCollapse::sInstance);
        }
    }
}

void CollapsePlane::calcAndSetBaseMtx() {
    MapObjActor::calcAndSetBaseMtx();

    if (_D0 != -1) {
        mJointController->registerCallBack();
    }

    if (MR::isInitializeStateEnd()) {
        TVec3f new_scale;
        new_scale.multPS(mScaleController->_C, mScale);
        MR::setBaseScale(this, new_scale);
    }
}

void CollapsePlane::control() {
    mScaleController->updateNerve();
    tryDPDStop();
}

bool CollapsePlane::calcJointPlane(TPos3f *pPos, const JointControllerInfo &) {
    f32 new_scale = (1.0f - ((0.69999999f * _D0)) / mTimer);
    TMtx34f mtx;
    mtx.identity();
    MR::preScaleMtx(mtx.toMtxPtr(), new_scale, 1.0f, new_scale);
    pPos->concat(mtx);
    pPos->mMtx[0][3] = mPosition.x;
    pPos->mMtx[1][3] = mPosition.y;
    pPos->mMtx[2][3] = mPosition.z;
    return true;
}

bool CollapsePlane::tryDPDStop() {
    if (isNerve(&NrvCollapsePlane::CollapsePlaneNrvDPDStop::sInstance)) {
        return false;
    }

    if (isNerve(&NrvCollapsePlane::CollapsePlaneNrvEnd::sInstance)) {
        return false;
    }

    if (!mStarPointerBind->tryStartPointBind()) {
        return false;
    }

    setNerve(&NrvCollapsePlane::CollapsePlaneNrvDPDStop::sInstance);
    return true;
}

namespace NrvCollapsePlane {
    INIT_NERVE(CollapsePlaneNrvWait);
    INIT_NERVE(CollapsePlaneNrvCollapse);
    INIT_NERVE(CollapsePlaneNrvDPDStop);
    INIT_NERVE(CollapsePlaneNrvEnd);
};

CollapsePlane::~CollapsePlane() {

}
