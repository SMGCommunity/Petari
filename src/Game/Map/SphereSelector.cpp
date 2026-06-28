#include "Game/Map/SphereSelector.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/SphereSelectorHandle.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util.hpp"

namespace {
    void getHandleMtx(TPos3f* pPos) {
        pPos->identity();
        if (MR::isExistSceneObj(SceneObj_SphereSelector)) {
            SphereSelectorHandle* pSelectorHandle = MR::getSceneObj<SphereSelector>(SceneObj_SphereSelector)->_90;
            if (pSelectorHandle && !MR::isDead(pSelectorHandle)) {
                pPos->set(pSelectorHandle->getBaseMtx());
            }
        }
    }
}

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
    : LiveActor("スフィアセレクター"), mSphereGroup(nullptr), _90(nullptr), mSelectedTarget(nullptr), _98(0), mPointingTarget(nullptr), _A4(0), _A8(0.0f), _AC(0.0f),
      mIsPointingInvalid(false), _B1(false) {
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
    mIsPointingInvalid = nullptr;
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
    _90->validateRotate();
    MR::startStarPointerModeSphereSelectorOnReaction(this);
}

void SphereSelector::invalidatePointing() {
    mIsPointingInvalid = true;
    _90->invalidateRotate();
    MR::endStarPointerMode(this);
}

void SphereSelector::sendMsgToAllActor(u32 msg) {
    for (int i = 0; i < mSphereGroup->mObjectCount; i++) {
        MR::sendSimpleMsgToActor(msg, mSphereGroup->getActor(i));
    }
}

void SphereSelector::playSelectedME() {
    s32 random = MR::getRandom((s32)0, (s32)4);

    switch(random) {
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
    s32 random = MR::getRandom((s32)0, (s32)4);

    switch(random) {
        case 0: 
            MR::startSystemME("ME_ASTRO_DOME_CALCEL1");
            break;
        case 1: 
            MR::startSystemME("ME_ASTRO_DOME_CALCEL2");
            break;        
        case 2: 
            MR::startSystemME("ME_ASTRO_DOME_CANCEL3");
            break;        
        case 3: 
            MR::startSystemME("ME_ASTRO_DOME_CANCEL4");
            break;
        case 4:
            MR::startSystemME("ME_ASTRO_DOME_CANCEL5");
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
    MR::setNerveAtStep(this, &NrvSphereSelector::SphereSelectorNrvConfirmWait::sInstance, 45);
}

void SphereSelector::exeConfirmCancel() {
    if (MR::isFirstStep(this)) {
        sendMsgToAllActor(ACTMES_SPHERE_SELECTOR_CONFIRM_CANCEL);
        mSelectedTarget = 0;
        mPointingTarget = 0;
        if (!_B1) {
            playCanceledME();
        }
        _B1 = false;
    }
    MR::setNerveAtStep(this, &NrvSphereSelector::SphereSelectorNrvSelectWait::sInstance, 45);
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
    MR::getSceneObj<SphereSelector>(SceneObj_SphereSelector)->mSphereGroup->registerActor(actor);
}

bool SphereSelectorFunction::isPadButton() {
    return MR::testCorePadButtonA(0) && !MR::isDemoActive();
}

s32 SphereSelectorFunction::getSelectStartFrame() {
    return 60;
}

s32 SphereSelectorFunction::getConfirmStartCancelFrame() {
    return 45;
}

void SphereSelectorFunction::selectStart() {
    MR::getSceneObj<SphereSelector>(SceneObj_SphereSelector)->appear();
}

void SphereSelectorFunction::selectCancel(bool b) {
    if (b) {
        MR::getSceneObj<SphereSelector>(SceneObj_SphereSelector)->_B1 = true;
    }
    MR::getSceneObj<SphereSelector>(SceneObj_SphereSelector)->setNerve(&NrvSphereSelector::SphereSelectorNrvSelectCancel::sInstance);
}

void SphereSelectorFunction::selectEnd() {
    MR::getSceneObj<SphereSelector>(SceneObj_SphereSelector)->kill();
}

void SphereSelectorFunction::confirmStart() {
    MR::getSceneObj<SphereSelector>(SceneObj_SphereSelector)->setNerve(&NrvSphereSelector::SphereSelectorNrvConfirmStart::sInstance);
}

void SphereSelectorFunction::confirmCancel(bool b) {
    if (b) {
        MR::getSceneObj<SphereSelector>(SceneObj_SphereSelector)->_B1 = true;
    }
    MR::getSceneObj<SphereSelector>(SceneObj_SphereSelector)->setNerve(&NrvSphereSelector::SphereSelectorNrvConfirmCancel::sInstance);
}

void SphereSelectorFunction::confirmed() {
    MR::getSceneObj<SphereSelector>(SceneObj_SphereSelector)->setNerve(&NrvSphereSelector::SphereSelectorNrvConfirmed::sInstance);
}

bool SphereSelectorFunction::isSelectWait() {
    return MR::getSceneObj<SphereSelector>(SceneObj_SphereSelector)->isNerve(&NrvSphereSelector::SphereSelectorNrvSelectWait::sInstance);
}

bool SphereSelectorFunction::isConfirmWait() {
    return MR::getSceneObj<SphereSelector>(SceneObj_SphereSelector)->isNerve(&NrvSphereSelector::SphereSelectorNrvConfirmWait::sInstance);
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

bool SphereSelectorFunction::trySyncKillMsgSelectStart(LiveActor * pActor, u32 msg) {
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
    MR::getSceneObj<SphereSelector>(SceneObj_SphereSelector)->validatePointing();
}

void SphereSelectorFunction::invalidatePointing() {
    MR::getSceneObj<SphereSelector>(SceneObj_SphereSelector)->invalidatePointing();
}

bool SphereSelectorFunction::isValidPointing() {
    return !MR::getSceneObj<SphereSelector>(SceneObj_SphereSelector)->mIsPointingInvalid;
}

void SphereSelectorFunction::setHandle(SphereSelectorHandle* pSelectorHandler) {
    MR::getSceneObj<SphereSelector>(SceneObj_SphereSelector)->_90 = pSelectorHandler;
}

void SphereSelectorFunction::calcHandledTrans(const TVec3f& src, TVec3f* dst) {
    TPos3f mtx;
    getHandleMtx(&mtx);
    mtx.mult(src, *dst);
}

void SphereSelectorFunction::calcHandledRotateMtx(const TVec3f& src, TPos3f* dst) {
    TPos3f mtx;
    getHandleMtx(&mtx);
    TMtx34f mtx2;
    MR::makeMtxRotate(mtx2, src);
    dst->concat(mtx, mtx2);
}

TVec3f& SphereSelectorFunction::getHandleTrans() {
    return MR::getSceneObj<SphereSelector>(SceneObj_SphereSelector)->_90->mPosition;
}

f32 SphereSelectorFunction::getHandleRotateSpeed() {
    return MR::getSceneObj<SphereSelector>(SceneObj_SphereSelector)->_90->mRotateSpeed;
}

bool SphereSelectorFunction::isHandleHolding() {
    return MR::getSceneObj<SphereSelector>(SceneObj_SphereSelector)->_90->isHolding();
}

void SphereSelectorFunction::registerPointingTarget(LiveActor * pActor, HandlePointingPriority priority) {
    MR::getSceneObj<SphereSelector>(SceneObj_SphereSelector)->registerPointingTarget(pActor, priority);
}

bool SphereSelectorFunction::tryRegisterPointingTarget(LiveActor * pActor, HandlePointingPriority priority) {
    if (isHandleHolding() && MR::isStarPointerPointing1PWithoutCheckZ(pActor, nullptr, true, false)) {
        MR::getSceneObj<SphereSelector>(SceneObj_SphereSelector)->registerPointingTarget(pActor, priority);
        return true;
    }
    return false;
}

bool SphereSelectorFunction::isPointingTarget() {
    return MR::getSceneObj<SphereSelector>(SceneObj_SphereSelector)->mPointingTarget;
}

bool SphereSelectorFunction::isPointingTarget(const LiveActor* pActor) {
    return MR::getSceneObj<SphereSelector>(SceneObj_SphereSelector)->mPointingTarget == pActor;
}

LiveActor* SphereSelectorFunction::getPointingTarget() {
    return MR::getSceneObj<SphereSelector>(SceneObj_SphereSelector)->mPointingTarget;
}

LiveActor* SphereSelectorFunction::getSelectedTarget() {
    return MR::getSceneObj<SphereSelector>(SceneObj_SphereSelector)->mSelectedTarget;
}

TVec3f& SphereSelectorFunction::getSelectedActorTrans() {
    return getSelectedTarget()->mPosition;
}

// void SphereSelectorFunction::calcOffsetPos(TVec3f * dst, const TVec3f & vec2, const TVec3f & vec3, const TVec3f & vec4, const TVec3f & vec5) {}

SphereSelector::~SphereSelector() {}

// I have no idea why is this at this file
MtxPtr SphereSelectorHandle::getBaseMtx() const {
    return (MtxPtr)&_90;
}
