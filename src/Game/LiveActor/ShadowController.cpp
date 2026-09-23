#include "Game/LiveActor/ShadowController.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ShadowDrawer.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/StringUtil.hpp"

namespace {
    const s32 sCollisionCountLimit = 1;
    const s32 sGravityCountLimit = 1;
}  // namespace

void ShadowController_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)-1.0f;
}

ShadowControllerHolder::ShadowControllerHolder() : NameObj("影管理"), _C(), _18(), _24() {
    mFarClip = 4000.0f;
    _C.init(0x500);
    _18.init(0x400);
    MR::connectToScene(this, MR::MovementType_ShadowControllerHolder, MR::CalcAnimType_None, MR::DrawBufferType_None, MR::DrawType_None);

    if (MR::isEqualStageName("AstroGalaxy") || MR::isEqualStageName("PeachCastleGardenGalaxy") || MR::isEqualStageName("PeachCastleFinalGalaxy")) {
        _24 = true;
    }
}

ShadowController::ShadowController(LiveActor* pActor, const char* pName)
    : mActor(pActor), mName(pName), mGroupName(""), mDrawer(), mProjectedSensor(), mCollisionPartsFilter(), _18(), _1C(), mDropPos(), mDropDir(),
      mProjPos(), mProjNorm(), _30(0.0f, 0.0f, 0.0f), _3C(0.0f, -1.0f, 0.0f), _48(0.0f, 0.0f, 0.0f), _54(0.0f, 1.0f, 0.0f), _60(1), _61(), _62(),
      _63(), _64(), _65(), _66(), _67(), mStartOffset(50.0f), mDropLength(), _70(), _71(1), _72(1) {
    MR::createSceneObj(SceneObj_ShadowControllerHolder);
    MR::addShadowController(this);
}

template MR::Vector< MR::AssignableArray< ShadowController* > >::Vector();
template MR::Vector< MR::AssignableArray< ShadowController* > >::~Vector();
template void MR::Vector< MR::AssignableArray< ShadowController* > >::push_back(ShadowController* const&);

void ShadowControllerHolder::movement() {
    updateController();
}

void ShadowControllerHolder::initAfterPlacement() {
    ShadowController* pController;
    int size = _C.size();

    for (u32 i = 0; i < size; i++) {
        pController = _C[i];

        pController->updateDirection();
        pController->updateProjection();
    }
}

void ShadowControllerHolder::updateController() {
    int size = _18.size();

    for (u32 i = 0; i < size; i++) {
        if (_24) {
            _18[i]->updateFarClipping(mFarClip);
        }

        _18[i]->update();
    }

    _18.clear();
}

ShadowControllerList::ShadowControllerList(LiveActor* pActor, u32 listCount) : mShadowList(), mHost(pActor) {
    mShadowList.init(listCount);
}

void ShadowControllerList::addController(ShadowController* pController) {
    mShadowList.push_back(pController);
}

u32 ShadowControllerList::getControllerCount() const {
    return mShadowList.size();
}

ShadowController* ShadowControllerList::getController(u32 idx) const {
    return mShadowList[idx];
}

ShadowController* ShadowControllerList::getController(const char* pName) const {
    if (mShadowList.size() == 1) {
        return mShadowList[0];
    }

    for (u32 i = 0; i < mShadowList.size(); i++) {
        if (MR::isEqualString(pName, mShadowList[i]->mName)) {
            return mShadowList[i];
        }
    }

    return nullptr;
}

void ShadowControllerList::resetCalcCount() {
    for (u32 i = 0; i < mShadowList.size(); i++) {
        ShadowController* controller = mShadowList[i];
        controller->_65 = 0;
        controller->_66 = 0;
    }
}

void ShadowControllerList::requestCalc() {
    for (u32 i = 0; i < mShadowList.size(); i++) {
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

void ShadowController::updateDirection() {
    if (isCalcGravity()) {
        TVec3f position;
        getDropPos(&position);
        TVec3f previousDirection(_3C);
        if (!isCalcShadowGravity() || !MR::calcDropShadowVectorOrZero(mActor, position, &_3C, nullptr, 0)) {
            if (!MR::calcGravityVectorOrZero(mActor, position, &_3C, nullptr, 0)) {
                _3C = previousDirection;
            }
        }

        if (_61 == 2 || _61 == 5) {
            _66++;
        }
    }
}

void ShadowController::updateProjection() {
    if (isCalcCollision()) {
        Triangle triangle;
        TVec3f position;
        getDropPos(&position);
        TVec3f direction;
        getDropDir(&direction);
        position -= direction * mStartOffset;

        switch (_62) {
        case 0:
            _63 = MR::getFirstPolyOnLineToMap(&_48, &triangle, position, direction * (mDropLength + mStartOffset), mCollisionPartsFilter, nullptr);
            break;
        case 1:
            _63 = MR::getFirstPolyOnLineToWaterSurface(&_48, &triangle, position, direction * (mDropLength + mStartOffset), mCollisionPartsFilter,
                                                       nullptr);
            break;
        }

        if (_63) {
            mProjectedSensor = triangle.getSensor();
            _54.set(*triangle.getNormal(0));
        } else {
            mProjectedSensor = nullptr;
        }

        if (_60 == 2) {
            _65++;
        }
    }
}

ShadowDrawer* ShadowController::getShadowDrawer() {
    return mDrawer;
}

LiveActor* ShadowController::getHost() const {
    return mActor;
}

void ShadowController::setShadowDrawer(ShadowDrawer* pDrawer) {
    mDrawer = pDrawer;
    pDrawer->setShadowController(this);
}

void ShadowController::getDropPos(TVec3f* pOut) const {
    if (mDropPos) {
        pOut->set(*mDropPos);
    } else {
        if (_1C) {
            PSMTXMultVec(_1C, (const Vec*)&_30, (Vec*)pOut);
        } else {
            pOut->set(_30);
        }
    }
}

void ShadowController::getDropDir(TVec3f* pOut) const {
    if (mDropDir) {
        pOut->set(*mDropDir);
    } else {
        pOut->set(_3C);
    }
}

f32 ShadowController::getDropLength() const {
    return mDropLength;
}

void ShadowController::getProjectionPos(TVec3f* pOut) const {
    if (mProjPos) {
        pOut->set(*mProjPos);
    } else {
        pOut->set(_48);
    }
}

void ShadowController::getProjectionNormal(TVec3f* pOut) const {
    if (mProjNorm) {
        pOut->set(*mProjNorm);
    } else {
        pOut->set(_54);
    }
}

f32 ShadowController::getProjectionLength() const {
    if (!_63) {
        return -1.0f;
    }

    TVec3f position;
    getDropPos(&position);
    TVec3f direction;
    getDropDir(&direction);
    TVec3f difference(_48);
    difference -= position;
    f32 length = direction.dot(difference);
    if (length < 0.0f) {
        return 0.0f;
    }

    return position.distance(_48);
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

bool ShadowController::isCalcCollision() const {
    if (!_60) {
        return false;
    }

    if (_60 == 2) {
        return _65 < ::sCollisionCountLimit;
    }

    return true;
}

bool ShadowController::isCalcGravity() const {
    if (_61 == 0 || _61 == 3) {
        return false;
    }

    if (_61 == 0 || _61 == 3) {
        return _66 < ::sGravityCountLimit;
    }

    return true;
}

bool ShadowController::isCalcShadowGravity() const {
    const u8 offset = static_cast< u8 >(-4);
    return static_cast< u8 >(_61 + offset) <= 1;
}

void ShadowController::setGroupName(const char* pName) {
    mGroupName = pName;
}

void ShadowController::setDropPosPtr(const TVec3f* pDropPos) {
    mDropPos = pDropPos;
    _1C = 0;
}

void ShadowController::setDropPosMtxPtr(MtxPtr pMtx, const TVec3f& rOffset) {
    _18 = pMtx;
    mDropPos = nullptr;
    _1C = pMtx;
    _30.set(rOffset);
}

void ShadowController::setDropPosFix(const TVec3f& rPos) {
    _30.set(rPos);
    mDropPos = 0;
    _1C = 0;
}

void ShadowController::setDropDirPtr(const TVec3f* pDropDir) {
    mDropDir = pDropDir;
    _61 = 0;
}

void ShadowController::setProjectionPtr(const TVec3f* pPosition, const TVec3f* pNormal) {
    mProjPos = pPosition;
    mProjNorm = pNormal;
    _60 = 0;
    _63 = 1;
    mProjectedSensor = nullptr;
}

void ShadowController::setDropDirFix(const TVec3f& rDirection) {
    _3C.set(rDirection);
    mDropDir = 0;
    _61 = 0;
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

void ShadowController::setProjectionFix(const TVec3f& rPosition, const TVec3f& rNormal, bool projected) {
    _48.set(rPosition);
    _54.set(rNormal);
    _63 = projected;
    mProjectedSensor = 0;
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

void ShadowController::setCollisionPartsFilter(CollisionPartsFilterBase* pBase) {
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

namespace MR {
    void addShadowController(ShadowController* pController) {
        getSceneObj< ShadowControllerHolder >(SceneObj_ShadowControllerHolder)->_C.push_back(pController);
    }

    void requestCalcActorShadowAppear(LiveActor* pActor) {
        if (!isInitializeStatePlacementSomething()) {
            if (pActor->mShadowControllerList) {
                pActor->mShadowControllerList->resetCalcCount();
            }

            requestCalcActorShadow(pActor);
        }
    }

    void requestCalcActorShadow(LiveActor* pActor) {
        if (pActor->mShadowControllerList) {
            pActor->mShadowControllerList->requestCalc();
        }
    }
}  // namespace MR

template MR::AssignableArray< ShadowController* >::~AssignableArray();

ShadowControllerHolder::~ShadowControllerHolder() {
}
