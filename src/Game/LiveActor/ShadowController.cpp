#include "Game/LiveActor/ShadowController.hpp"
#include "Game/LiveActor/ShadowDrawer.hpp"

ShadowControllerHolder::ShadowControllerHolder() : NameObj("影管理"), _C(), _18(), _24(false) {
    mFarClip = 4000.0f;
    _C.init(0x500);
    _18.init(0x400);
    MR::connectToScene(this, 0x2D, -1, -1, -1);

    if (MR::isEqualStageName("AstroGalaxy") || MR::isEqualStageName("PeachCastleGardenGalaxy") || MR::isEqualStageName("PeachCastleFinalGalaxy")) {
        _24 = true;
    }
}

void ShadowControllerHolder::movement() {
    updateController();
}

void ShadowControllerHolder::initAfterPlacement() {
    u32 count = _C.getCount();
    for (u32 i = 0; i < count; i++) {
        ShadowController* ctrl = _C[i];
        ctrl->updateDirection();
        ctrl->updateProjection();
    }
}

void ShadowControllerHolder::updateController() {
    u32 count = _18.getCount();

    for (u32 i = 0; i < count; i++) {
        if (_24) {
            ShadowController* ctrl = _18[i];
            ctrl->updateFarClipping(mFarClip);
        }

        ShadowController* ctrl = _18[i];
        ctrl->update();
    }

    _18.mCount = 0;
}

ShadowControllerList::ShadowControllerList(LiveActor *pActor, u32 listCount) : mShadowList(), mHost(pActor) {
    mShadowList.init(listCount);
}

void ShadowControllerList::addController(ShadowController *pController) {
    mShadowList.push_back(pController);
}

u32 ShadowControllerList::getControllerCount() const {
    return mShadowList.mCount;
}

ShadowController* ShadowControllerList::getController(u32 idx) const {
    return mShadowList[idx];
}

ShadowController* ShadowControllerList::getController(const char *pName) const {
    if (mShadowList.mCount == 1) {
        return mShadowList[0];
    }

    for (u32 i = 0; i < mShadowList.mCount; i++) {
        if (MR::isEqualString(pName, mShadowList[i]->mName)) {
            return mShadowList[i];
        }
    }

    return nullptr;
}

void ShadowControllerList::resetCalcCount() {
    for (u32 i = 0; i < mShadowList.getCount(); i++) {
        ShadowController* controller = mShadowList[i];
        controller->_65 = 0;
        controller->_66 = 0;
    }
}

void ShadowControllerList::requestCalc() {
    for (u32 i = 0; i < mShadowList.getCount(); i++) {
        mShadowList[i]->requestCalc();
    }
}

void ShadowController::requestCalc() {
    if (!_64) {
        _64 = 1;
        appendToHolder();
    }
}

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
        pOut->set<f32>(*mDropPos);
    }
    else {
        if (_1C) {
            PSMTXMultVec(_1C, (const Vec*)&_30, (Vec*)pOut);
        }
        else {
            pOut->set<f32>(_30);
        }
    }
}

void ShadowController::getDropDir(TVec3f *pOut) const {
    if (mDropDir) {
        pOut->set<f32>(*mDropDir);
    }
    else {
        pOut->set<f32>(_3C);
    }
}

f32 ShadowController::getDropLength() const {
    return mDropLength;
}

void ShadowController::getProjectionPos(TVec3f *pOut) const {
    if (mProjPos) {
        pOut->set<f32>(*mProjPos);
    }
    else {
        pOut->set<f32>(_48);
    }
}

void ShadowController::getProjectionNormal(TVec3f *pOut) const {
    if (mProjNorm) {
        pOut->set<f32>(*mProjNorm);
    }
    else {
        pOut->set<f32>(_54);
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
    _30.set<f32>(rPos);
    mDropPos = 0;
    _1C = 0;
}

void ShadowController::setDropDirPtr(const TVec3f *pDropDir) {
    mDropDir = pDropDir;
    _61 = 0;
}

void ShadowController::setDropDirFix(const TVec3f &a1) {
    _3C.set<f32>(a1);
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
    _48.set<f32>(a1);
    _54.set<f32>(a2);
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
    _3C.set<f32>(vec);
    mDropDir = 0;
    _61 = 1;
}

void ShadowController::onCalcDropGravityOneTime() {
    TVec3f vec(0.0f, 1.0f, 0.0f);
    _3C.set<f32>(vec);
    mDropDir = 0;
    _61 = 2;
    _66 = 0;
}

void ShadowController::offCalcDropGravity() {
    _61 = 0;
}

void ShadowController::onCalcDropPrivateGravity() {
    TVec3f vec(0.0f, 1.0f, 0.0f);
    _3C.set<f32>(vec);
    mDropDir = 0;
    _61 = 4;
}

void ShadowController::onCalcDropPrivateGravityOneTime() {
    TVec3f vec(0.0f, 1.0f, 0.0f);
    _3C.set<f32>(vec);
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

ShadowControllerHolder::~ShadowControllerHolder() {

}
