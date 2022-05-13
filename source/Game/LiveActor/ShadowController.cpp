#include "Game/LiveActor/ShadowController.h"
#include "Game/LiveActor/ShadowDrawer.h"

void ShadowController::update() {
    if (isDraw()) {
        updateDirection();
        updateProjection();
    }

    _64 = 0;
}

void ShadowController::updateFarClipping(f32 clip) {
    TVec3f dropPos;
    getDropPos(&dropPos);
    f32 cameraDistZ = MR::calcCameraDistanceZ(dropPos);
    _67 = cameraDistZ >= clip;
}

ShadowDrawer* ShadowController::getShadowDrawer() {
    return mDrawer;
}

LiveActor* ShadowController::getHost() const {
    return mActor;
}

void ShadowController::setShadowDrawer(ShadowDrawer *pDrawer) {
    mDrawer = pDrawer;
    pDrawer->setShadowController(this);
}

void ShadowController::getDropPos(TVec3f *pOut) const {
    if (mDropPos) {
        pOut->set(*mDropPos);
    }
    else {
        if (_1C) {
            PSMTXMultVec(_1C, (const Vec*)&_30, (Vec*)pOut);
        }
        else {
            pOut->set(_30);
        }
    }
}

void ShadowController::getDropDir(TVec3f *pOut) const {
    if (mDropDir) {
        pOut->set(*mDropDir);
    }
    else {
        pOut->set(_3C);
    }
}

f32 ShadowController::getDropLength() const {
    return mDropLength;
}

void ShadowController::getProjectionPos(TVec3f *pOut) const {
    if (mProjPos) {
        pOut->set(*mProjPos);
    }
    else {
        pOut->set(_48);
    }
}

void ShadowController::getProjectionNormal(TVec3f *pOut) const {
    if (mProjNorm) {
        pOut->set(*mProjNorm);
    }
    else {
        pOut->set(_54);
    }
}

bool ShadowController::isProjected() const {
    return _63 != 0;
}

bool ShadowController::isDraw() const {
    if (_67) {
        return false;
    }
    
    if (!_71) {
        return false;
    }

    if (!_72) {
        return true;
    }

    return MR::isValidDraw(mActor);
}

/*
bool ShadowController::isCalcCollision() const {
    if (!_60) {
        return false;
    }

    if (_60 == 2) {
        return _65 >= 0; 
    }

    return true;
}
*/

/*
bool ShadowController::isCalcShadowGravity() const {
    return (_61 + 0xFC) <= 1;
}
*/

void ShadowController::setGroupName(const char *pName) {
    mGroupName = pName;
}

void ShadowController::setDropPosPtr(const TVec3f *pDropPos) {
    mDropPos = pDropPos;
    _1C = 0;
}

void ShadowController::setDropPosFix(const TVec3f &rPos) {
    _30.set(rPos);
    mDropPos = 0;
    _1C = 0;
}

void ShadowController::setDropDirPtr(const TVec3f *pDropDir) {
    mDropDir = pDropDir;
    _61 = 0;
}

void ShadowController::setDropDirFix(const TVec3f &a1) {
    _3C.set(a1);
    mDropDir = 0;
    _61= 0;
}

void ShadowController::setDropLength(f32 len) {
    mDropLength = len;
}

void ShadowController::setDropStartOffset(f32 offs) {
    mStartOffset = offs;
}

void ShadowController::setDropTypeNormal() {
    _62 = 0;
}

void ShadowController::setDropTypeSurface() {
    _62 = 1;
}

void ShadowController::setProjectionFix(const TVec3f &a1, const TVec3f &a2, bool a3) {
    _48.set(a1);
    _54.set(a2);
    _63 = a3;
    _10 = 0;
}

void ShadowController::onCalcCollision() {
    _60 = 1;
}

void ShadowController::offCalcCollision() {
    _60 = 0;
}

void ShadowController::onCalcCollisionOneTime() {
    _60 = 2;
    _65 = 0;
}

void ShadowController::onCalcDropGravity() {
    TVec3f vec(0.0f, 1.0f, 0.0f);
    _3C.set(vec);
    mDropDir = 0;
    _61 = 1;
}

void ShadowController::onCalcDropGravityOneTime() {
    TVec3f vec(0.0f, 1.0f, 0.0f);
    _3C.set(vec);
    mDropDir = 0;
    _61 = 2;
    _66 = 0;
}

void ShadowController::offCalcDropGravity() {
    _61 = 0;
}

void ShadowController::onCalcDropPrivateGravity() {
    TVec3f vec(0.0f, 1.0f, 0.0f);
    _3C.set(vec);
    mDropDir = 0;
    _61 = 4;
}

void ShadowController::onCalcDropPrivateGravityOneTime() {
    TVec3f vec(0.0f, 1.0f, 0.0f);
    _3C.set(vec);
    mDropDir = 0;
    _61 = 5;
    _66 = 0;
}

void ShadowController::offCalcDropPrivateGravity() {
    _61 = 3;
}

void ShadowController::setCollisionPartsFilter(CollisionPartsFilterBase *pBase) {
    mCollisionPartsFilter = pBase;
}

void ShadowController::onFollowHostScale() {
    _70 = 1;
}

void ShadowController::offFollowHostScale() {
    _70 = 0;
}

bool ShadowController::isFollowHostScale() const {
    return _70;
}

void ShadowController::onVisibleSyncHost() {
    _72 = 1;
}

void ShadowController::offVisibleSyncHost() {
    _72 = 0;
}

void ShadowController::validate() {
    _71 = 1;
}

void ShadowController::invalidate() {
    _71 = 0;
}