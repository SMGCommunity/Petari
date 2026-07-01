#include "Game/Map/SphereSelector.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/SphereSelectorHandle.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util.hpp"

namespace {
    SphereSelector* getSphereSelector() {
        return MR::getSceneObj< SphereSelector >(SceneObj_SphereSelector);
    }

    void getHandleMtx(TPos3f* pPos) {
        pPos->identity();
        if (MR::isExistSceneObj(SceneObj_SphereSelector)) {
            SphereSelectorHandle* pSelectorHandle = ::getSphereSelector()->mHandle;
            if (pSelectorHandle && !MR::isDead(pSelectorHandle)) {
                pPos->set(pSelectorHandle->getBaseMtx());
            }
        }
    }
};  // namespace

namespace NrvSphereSelector {
    NEW_NERVE(SphereSelectorNrvSelectStart, SphereSelector, SelectStart);
    NEW_NERVE(SphereSelectorNrvSelectWait, SphereSelector, SelectWait);
    NEW_NERVE(SphereSelectorNrvSelectCancel, SphereSelector, SelectCancel);
    NEW_NERVE(SphereSelectorNrvConfirmStart, SphereSelector, ConfirmStart);
    NEW_NERVE(SphereSelectorNrvConfirmWait, SphereSelector, ConfirmWait);
    NEW_NERVE(SphereSelectorNrvConfirmCancel, SphereSelector, ConfirmCancel);
    NEW_NERVE(SphereSelectorNrvConfirmed, SphereSelector, Confirm);
};  // namespace NrvSphereSelector

SphereSelector::SphereSelector()
    : LiveActor("スフィアセレクター"), mSphereGroup(), mHandle(), mSelectedTarget(), _98(), mPointingTarget(), _A4(),
      _A8(), _AC(), mIsPointingInvalid(), _B1() {
}

void SphereSelector::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjDecorationMovement(this);
    MR::invalidateClipping(this);
    mSphereGroup = new LiveActorGroup("スフィアセレクターアクターグループ", 32);
    initNerve(&NrvSphereSelector::SphereSelectorNrvSelectStart::sInstance);
    makeActorDead();
}

void SphereSelector::appear() {
    LiveActor::appear();
    mSelectedTarget = nullptr;
    _98 = 0;
    mPointingTarget = nullptr;
    mIsPointingInvalid = false;
    MR::deactivateDefaultGameLayout();
    MR::startStarPointerModeSphereSelectorFinger(this);
    setNerve(&NrvSphereSelector::SphereSelectorNrvSelectStart::sInstance);
}

void SphereSelector::kill() {
    sendMsgToAllActor(ACTMES_SPHERE_SELECTOR_SELECT_END);
    LiveActor::kill();
    MR::activateDefaultGameLayout();
    MR::endStarPointerMode(this);
    MR::endStarPointerMode(this);
}

// void SphereSelector::registerPointingTarget(LiveActor* pActor, HandlePointingPriority property) {}

void SphereSelector::validatePointing() {
    mIsPointingInvalid = false;
    mHandle->validateRotate();
    MR::startStarPointerModeSphereSelectorOnReaction(this);
}

void SphereSelector::invalidatePointing() {
    mIsPointingInvalid = true;
    mHandle->invalidateRotate();
    MR::endStarPointerMode(this);
}

void SphereSelector::sendMsgToAllActor(u32 msg) {
    for (int i = 0; i < mSphereGroup->mObjectCount; i++) {
        MR::sendSimpleMsgToActor(msg, mSphereGroup->getActor(i));
    }
}

void SphereSelector::playSelectedME() {
    switch (MR::getRandom(0L, 4L)) {
    case 0:
        MR::startSystemME("ME_ASTRO_DOME_SELECT1");
        break;
    case 1:
        MR::startSystemME("ME_ASTRO_DOME_SELECT2");
        break;
    case 2:
        MR::startSystemME("ME_ASTRO_DOME_SELECT3");
        break;
    case 3:
        MR::startSystemME("ME_ASTRO_DOME_SELECT4");
        break;
    }
}

void SphereSelector::playCanceledME() {
    switch (MR::getRandom(0L, 4L)) {
    case 0:
        MR::startSystemME("ME_ASTRO_DOME_CALCEL1");
        break;
    case 1:
        MR::startSystemME("ME_ASTRO_DOME_CALCEL2");
        break;
    case 2:
        MR::startSystemME("ME_ASTRO_DOME_CALCEL3");
        break;
    case 3:
        MR::startSystemME("ME_ASTRO_DOME_CALCEL4");
        break;
    case 4:
        MR::startSystemME("ME_ASTRO_DOME_CALCEL5");
        break;
    }
}

void SphereSelector::exeSelectCancel() {
    if (MR::isFirstStep(this)) {
        if (!_B1) {
            MR::startSystemSE("SE_SY_GALAXY_DECIDE_CANCEL");
            playCanceledME();
        }
        _B1 = false;
    }
}

void SphereSelector::exeConfirmStart() {
    if (MR::isFirstStep(this)) {
        sendMsgToAllActor(ACTMES_SPHERE_SELECTOR_CONFIRM_START);
        if (!_B1) {
            MR::startSystemSE("SE_SY_GALAXY_SELECTED");
            playSelectedME();
        }
        _B1 = false;
        MR::endStarPointerMode(this);
    }
    MR::setNerveAtStep(this, &NrvSphereSelector::SphereSelectorNrvConfirmWait::sInstance, SphereSelectorFunction::getConfirmStartCancelFrame());
}

void SphereSelector::exeConfirmCancel() {
    if (MR::isFirstStep(this)) {
        sendMsgToAllActor(ACTMES_SPHERE_SELECTOR_CONFIRM_CANCEL);
        mSelectedTarget = nullptr;
        mPointingTarget = nullptr;
        if (!_B1) {
            playCanceledME();
        }
        _B1 = false;
    }
    MR::setNerveAtStep(this, &NrvSphereSelector::SphereSelectorNrvSelectWait::sInstance, SphereSelectorFunction::getConfirmStartCancelFrame());
}

void SphereSelector::exeSelectStart() {
    if (MR::isFirstStep(this)) {
        sendMsgToAllActor(ACTMES_SPHERE_SELECTOR_SELECT_START);
    }

    MR::setNerveAtStep(this, &NrvSphereSelector::SphereSelectorNrvSelectWait::sInstance, 60);
}

void SphereSelector::exeConfirmWait() {
}

void SphereSelector::exeConfirm() {
    if (MR::isFirstStep(this)) {
        sendMsgToAllActor(ACTMES_SPHERE_SELECTOR_CONFIRMED);
        MR::stopStageBGM(90);
    }
}

void SphereSelectorFunction::registerTarget(LiveActor* actor) {
    MR::createSceneObj(SceneObj_SphereSelector);
    ::getSphereSelector()->mSphereGroup->registerActor(actor);
}

bool SphereSelectorFunction::isPadButton() {
    return MR::testCorePadButtonA(WPAD_CHAN0) && !MR::isDemoActive();
}

s32 SphereSelectorFunction::getSelectStartFrame() {
    return 60;
}

s32 SphereSelectorFunction::getConfirmStartCancelFrame() {
    return 45;
}

void SphereSelectorFunction::selectStart() {
    ::getSphereSelector()->appear();
}

void SphereSelectorFunction::selectCancel(bool b) {
    if (b) {
        ::getSphereSelector()->_B1 = true;
    }
    ::getSphereSelector()->setNerve(&NrvSphereSelector::SphereSelectorNrvSelectCancel::sInstance);
}

void SphereSelectorFunction::selectEnd() {
    ::getSphereSelector()->kill();
}

void SphereSelectorFunction::confirmStart() {
    ::getSphereSelector()->setNerve(&NrvSphereSelector::SphereSelectorNrvConfirmStart::sInstance);
}

void SphereSelectorFunction::confirmCancel(bool b) {
    if (b) {
        ::getSphereSelector()->_B1 = true;
    }
    ::getSphereSelector()->setNerve(&NrvSphereSelector::SphereSelectorNrvConfirmCancel::sInstance);
}

void SphereSelectorFunction::confirmed() {
    ::getSphereSelector()->setNerve(&NrvSphereSelector::SphereSelectorNrvConfirmed::sInstance);
}

bool SphereSelectorFunction::isSelectWait() {
    return ::getSphereSelector()->isNerve(&NrvSphereSelector::SphereSelectorNrvSelectWait::sInstance);
}

bool SphereSelectorFunction::isConfirmWait() {
    return ::getSphereSelector()->isNerve(&NrvSphereSelector::SphereSelectorNrvConfirmWait::sInstance);
}

bool SphereSelectorFunction::isMsgSelectStart(u32 msg) {
    return ACTMES_SPHERE_SELECTOR_SELECT_START == msg;
}

bool SphereSelectorFunction::isMsgSelectEnd(u32 msg) {
    return ACTMES_SPHERE_SELECTOR_SELECT_END == msg;
}

bool SphereSelectorFunction::isMsgConfirmStart(u32 msg) {
    return ACTMES_SPHERE_SELECTOR_CONFIRM_START == msg;
}

bool SphereSelectorFunction::isMsgConfirmCancel(u32 msg) {
    return ACTMES_SPHERE_SELECTOR_CONFIRM_CANCEL == msg;
}

bool SphereSelectorFunction::isMsgConfirmed(u32 msg) {
    return ACTMES_SPHERE_SELECTOR_CONFIRMED == msg;
}

bool SphereSelectorFunction::isMsgTargetSelected(u32 msg) {
    return ACTMES_SPHERE_SELECTOR_TARGET_SELECTED == msg;
}

bool SphereSelectorFunction::trySyncAppearMsgSelectStart(LiveActor* pActor, u32 msg) {
    if (isMsgSelectStart(msg)) {
        pActor->appear();
        return true;
    }
    if (isMsgSelectEnd(msg)) {
        pActor->kill();
        return true;
    }
    return false;
}

bool SphereSelectorFunction::trySyncKillMsgSelectStart(LiveActor* pActor, u32 msg) {
    if (isMsgSelectStart(msg)) {
        pActor->kill();
        return true;
    }
    if (isMsgSelectEnd(msg)) {
        pActor->appear();
        return true;
    }
    return false;
}

void SphereSelectorFunction::validatePointing() {
    ::getSphereSelector()->validatePointing();
}

void SphereSelectorFunction::invalidatePointing() {
    ::getSphereSelector()->invalidatePointing();
}

bool SphereSelectorFunction::isValidPointing() {
    return !::getSphereSelector()->mIsPointingInvalid;
}

void SphereSelectorFunction::setHandle(SphereSelectorHandle* pHandle) {
    ::getSphereSelector()->mHandle = pHandle;
}

void SphereSelectorFunction::calcHandledTrans(const TVec3f& rSrc, TVec3f* pDst) {
    TPos3f mtx;
    ::getHandleMtx(&mtx);
    mtx.mult(rSrc, *pDst);
}

void SphereSelectorFunction::calcHandledRotateMtx(const TVec3f& rSrc, TPos3f* pDst) {
    TPos3f mtx;
    ::getHandleMtx(&mtx);
    TMtx34f mtx2;
    MR::makeMtxRotate(mtx2, rSrc);
    pDst->concat(mtx, mtx2);
}

TVec3f& SphereSelectorFunction::getHandleTrans() {
    return ::getSphereSelector()->mHandle->mPosition;
}

f32 SphereSelectorFunction::getHandleRotateSpeed() {
    return ::getSphereSelector()->mHandle->mRotateSpeed;
}

bool SphereSelectorFunction::isHandleHolding() {
    return ::getSphereSelector()->mHandle->isHolding();
}

void SphereSelectorFunction::registerPointingTarget(LiveActor* pActor, HandlePointingPriority priority) {
    ::getSphereSelector()->registerPointingTarget(pActor, priority);
}

bool SphereSelectorFunction::tryRegisterPointingTarget(LiveActor* pActor, HandlePointingPriority priority) {
    if (isHandleHolding() && MR::isStarPointerPointing1PWithoutCheckZ(pActor, nullptr, true, false)) {
        ::getSphereSelector()->registerPointingTarget(pActor, priority);
        return true;
    }
    return false;
}

bool SphereSelectorFunction::isPointingTarget() {
    return ::getSphereSelector()->mPointingTarget != nullptr;
}

bool SphereSelectorFunction::isPointingTarget(const LiveActor* pActor) {
    return ::getSphereSelector()->mPointingTarget == pActor;
}

LiveActor* SphereSelectorFunction::getPointingTarget() {
    return ::getSphereSelector()->mPointingTarget;
}

LiveActor* SphereSelectorFunction::getSelectedTarget() {
    return ::getSphereSelector()->mSelectedTarget;
}

TVec3f& SphereSelectorFunction::getSelectedActorTrans() {
    return getSelectedTarget()->mPosition;
}

// void SphereSelectorFunction::calcOffsetPos(TVec3f * dst, const TVec3f & vec2, const TVec3f & vec3, const TVec3f & vec4, const TVec3f & vec5) {}

SphereSelector::~SphereSelector() {}
