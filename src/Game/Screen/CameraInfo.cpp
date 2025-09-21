#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/CameraInfo.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace {
    static const s32 sLifeTime = 120;
};

namespace NrvCameraInfo {
    NEW_NERVE(CameraInfoNrvHide, CameraInfo, Hide);
    NEW_NERVE(CameraInfoNrvAppear, CameraInfo, Appear);
    NEW_NERVE(CameraInfoNrvNormal, CameraInfo, Normal);
    NEW_NERVE(CameraInfoNrvFirstPerson, CameraInfo, FirstPerson);
    NEW_NERVE(CameraInfoNrvEnd, CameraInfo, End);
};

CameraInfo::CameraInfo(const char* pName) :
    LayoutActor(pName, true),
    mStep(-1),
    mIsActive(true)
{}

void CameraInfo::init(const JMapInfoIter& rIter) {
    initLayoutManager("CameraInfo", 2);
    MR::createAndAddPaneCtrl(this, "Forbid", 1);
    MR::hidePane(this, "Forbid");
    MR::connectToSceneLayout(this);
    initNerve(&NrvCameraInfo::CameraInfoNrvHide::sInstance);
    appear();
}

void CameraInfo::control() {
    if (tryShow()) {
        return;
    }

    if (tryHide()) {
        return;
    }
}

void CameraInfo::movement() {
    if (!mIsActive) {
        return;
    }

    LayoutActor::movement();
}

void CameraInfo::calcAnim() {
    if (!mIsActive) {
        return;
    }

    LayoutActor::calcAnim();
}

void CameraInfo::draw() const {
    if (!mIsActive) {
        return;
    }

    LayoutActor::draw();
}

void CameraInfo::activate() {
    mIsActive = true;
}

void CameraInfo::deactivate() {
    mIsActive = false;
}

void CameraInfo::exeHide() {
    if (MR::isFirstStep(this)) {
        MR::hideLayout(this);
    }
}

void CameraInfo::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::showLayout(this);
        MR::startAnim(this, "Appear", 0);
        hideCrossLineAll();

        _25 = true;
        mStep = 0;
    }

    tryChangeCameraStateNerve();
}

void CameraInfo::exeNormal() {
    if (tryChangeCameraStateNerve()) {
        return;
    }

    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Camera", 1);
        tryAnimShortCut();
        MR::hidePane(this, "LineD");
    }

    if (MR::isPossibleToShiftToFirstPersonCamera()) {
        MR::showPane(this, "LineU");
    }
    else {
        MR::hidePane(this, "LineU");
    }

    if (MR::isCameraPossibleToRoundLeft()) {
        MR::showPane(this, "LineL");
    }
    else {
        MR::hidePane(this, "LineL");
    }

    if (MR::isCameraPossibleToRoundRight()) {
        MR::showPane(this, "LineR");
    }
    else {
        MR::hidePane(this, "LineR");
    }

    updateCameraNG();
}

void CameraInfo::exeFirstPerson() {
    if (tryChangeCameraStateNerve()) {
        return;
    }

    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "FirstPerson", 1);
        tryAnimShortCut();
        hideCrossLineAll();
        MR::showPane(this, "LineD");
    }

    updateCameraNG();
}

void CameraInfo::exeEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "End", 0);
    }

    if (MR::isAnimStopped(this, 0)) {
        setNerve(&NrvCameraInfo::CameraInfoNrvHide::sInstance);
    }
}

void CameraInfo::updateCameraNG() {
    if (MR::isHiddenPane(this, "Forbid")) {
        if (MR::isCameraControlNG()) {
            MR::showPane(this, "Forbid");
            MR::startPaneAnim(this, "Forbid", "Forbid", 0);
        }
    }
    else if (MR::isPaneAnimStopped(this, "Forbid", 0)) {
        MR::hidePane(this, "Forbid");
    }
}

void CameraInfo::hideCrossLineAll() {
    MR::hidePane(this, "LineU");
    MR::hidePane(this, "LineL");
    MR::hidePane(this, "LineR");
    MR::hidePane(this, "LineD");
}

bool CameraInfo::tryAnimShortCut() {
    if (!_25) {
        return false;
    }

    MR::setAnimFrameAndStop(this, MR::getAnimFrameMax(this, 1), 1);

    _25 = false;

    return true;
}

bool CameraInfo::tryShow() {
    if (isNerve(&NrvCameraInfo::CameraInfoNrvAppear::sInstance)) {
        return false;
    }

    if (isNerve(&NrvCameraInfo::CameraInfoNrvNormal::sInstance)) {
        return false;
    }

    if (isNerve(&NrvCameraInfo::CameraInfoNrvFirstPerson::sInstance)) {
        return false;
    }

    bool isTriggerAnyDir = MR::testCorePadTriggerUp(0)
        || MR::testCorePadTriggerDown(0)
        || MR::testCorePadTriggerLeft(0)
        || MR::testCorePadTriggerRight(0);

    if (!isTriggerAnyDir) {
        return false;
    }

    setNerve(&NrvCameraInfo::CameraInfoNrvAppear::sInstance);

    return true;
}

bool CameraInfo::tryHide() {
    if (isNerve(&NrvCameraInfo::CameraInfoNrvEnd::sInstance)) {
        return false;
    }

    if (isNerve(&NrvCameraInfo::CameraInfoNrvHide::sInstance)) {
        return false;
    }

    if (isNerve(&NrvCameraInfo::CameraInfoNrvFirstPerson::sInstance)) {
        return false;
    }

    bool isTriggerAnyDir = MR::testCorePadTriggerUp(0)
        || MR::testCorePadTriggerDown(0)
        || MR::testCorePadTriggerLeft(0)
        || MR::testCorePadTriggerRight(0);

    if (isTriggerAnyDir || getNerveStep() == 0) {
        mStep = 0;
    }

    if (mStep >= sLifeTime) {
        setNerve(&NrvCameraInfo::CameraInfoNrvEnd::sInstance);

        return true;
    }

    mStep++;

    return false;
}

bool CameraInfo::tryChangeCameraStateNerve() {
    if (MR::isFirstPersonCamera()) {
        if (!isNerve(&NrvCameraInfo::CameraInfoNrvFirstPerson::sInstance)) {
            setNerve(&NrvCameraInfo::CameraInfoNrvFirstPerson::sInstance);

            return true;
        }

        return false;
    }
    else if (!isNerve(&NrvCameraInfo::CameraInfoNrvNormal::sInstance)) {
        setNerve(&NrvCameraInfo::CameraInfoNrvNormal::sInstance);

        return true;
    }

    return false;
}
