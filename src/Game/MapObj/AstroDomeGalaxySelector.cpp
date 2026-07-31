#include "Game/MapObj/AstroDomeGalaxySelector.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/SphereSelector.hpp"
#include "Game/MapObj/AstroDomeCameraController.hpp"
#include "Game/MapObj/MiniatureGalaxy.hpp"
#include "Game/MapObj/MiniatureGalaxyHolder.hpp"
#include "Game/Screen/GalaxyConfirmLayout.hpp"
#include "Game/Screen/GalaxySelectBackButton.hpp"
#include "Game/Screen/GalaxySelectInfo.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SequenceUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

namespace {
    const char* const cDemoNameDomeLecture = "ドームレクチャー２";
    const char* const cDemoNameJumpOut = "マリオ飛び出し";
};  // namespace

namespace {
    MiniatureGalaxy* getSelectedMiniatureGalaxy() {
        return static_cast< MiniatureGalaxy* >(SphereSelectorFunction::getSelectedTarget());
    }
};  // namespace

namespace NrvAstroDomeGalaxySelector {
    NEW_NERVE(AstroDomeGalaxySelectorNrvGalaxySelectStart, AstroDomeGalaxySelector, GalaxySelectStart);
    NEW_NERVE(AstroDomeGalaxySelectorNrvGalaxySelect, AstroDomeGalaxySelector, GalaxySelect);
    NEW_NERVE(AstroDomeGalaxySelectorNrvGalaxySelectCancel, AstroDomeGalaxySelector, GalaxySelectCancel);
    NEW_NERVE(AstroDomeGalaxySelectorNrvGalaxyConfirmStart, AstroDomeGalaxySelector, GalaxyConfirmStart);
    NEW_NERVE(AstroDomeGalaxySelectorNrvGalaxyConfirm, AstroDomeGalaxySelector, GalaxyConfirm);
    NEW_NERVE(AstroDomeGalaxySelectorNrvGalaxyConfirmCancel, AstroDomeGalaxySelector, GalaxyConfirmCancel);
    NEW_NERVE(AstroDomeGalaxySelectorNrvDemoJumpOut, AstroDomeGalaxySelector, DemoJumpOut);
    NEW_NERVE(AstroDomeGalaxySelectorNrvDemoDomeLecture, AstroDomeGalaxySelector, DemoDomeLecture);
    NEW_NERVE(AstroDomeGalaxySelectorNrvGalaxyMoveLecture, AstroDomeGalaxySelector, GalaxyMoveLecture);
    NEW_NERVE(AstroDomeGalaxySelectorNrvWaitStartDemo, AstroDomeGalaxySelector, WaitStartDemo);
};  // namespace NrvAstroDomeGalaxySelector

AstroDomeGalaxySelector::AstroDomeGalaxySelector(const char* pName)
    : LiveActor(pName), mCameraController(), mBackButton(), mInfo(), mConfirmLayout(), _9C() {
}

void AstroDomeGalaxySelector::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjMovement(this);
    MR::invalidateClipping(this);

    mCameraController = new AstroDomeCameraController("天文ドームカメラ制御");
    mCameraController->initWithoutIter();

    mBackButton = new GalaxySelectBackButton();
    mBackButton->initWithoutIter();

    mInfo = new GalaxySelectInfo(false);
    mInfo->initWithoutIter();

    mConfirmLayout = new GalaxyConfirmLayout();
    mConfirmLayout->initWithoutIter();

    MR::needStageSwitchWriteA(this, rIter);
    MR::onSwitchA(this);
    initNerve(&NrvAstroDomeGalaxySelector::AstroDomeGalaxySelectorNrvGalaxySelectStart::sInstance);
    MR::tryRegisterDemoCast(this, rIter);
    SphereSelectorFunction::registerTarget(this);
    makeActorDead();
}

void AstroDomeGalaxySelector::appear() {
    LiveActor::appear();
    MR::offSwitchA(this);
    setNerve(&NrvAstroDomeGalaxySelector::AstroDomeGalaxySelectorNrvGalaxySelectStart::sInstance);
}

void AstroDomeGalaxySelector::kill() {
    MR::resetYesNoSelectorSE();
    MR::onSwitchA(this);
    LiveActor::kill();
    mInfo->kill();
    MR::disappearStarCounter();
}

bool AstroDomeGalaxySelector::receiveOtherMsg(u32 v1, HitSensor* pSender, HitSensor* pReceiver) {
    if (SphereSelectorFunction::isMsgSelectStart(v1)) {
        appear();
        return true;
    }

    if (SphereSelectorFunction::isMsgConfirmStart(v1)) {
        setNerve(&NrvAstroDomeGalaxySelector::AstroDomeGalaxySelectorNrvGalaxyConfirmStart::sInstance);
        return true;
    }

    if (SphereSelectorFunction::isMsgTargetSelected(v1)) {
        mBackButton->decide();
        SphereSelectorFunction::selectCancel(false);
        setNerve(&NrvAstroDomeGalaxySelector::AstroDomeGalaxySelectorNrvGalaxySelectCancel::sInstance);
        return true;
    }

    return false;
}

void AstroDomeGalaxySelector::showGalaxyInfo(const MiniatureGalaxy* pMiniGalaxy) {
    s32 state = 3;
    s32 cometID = -1;

    if (pMiniGalaxy->mState == 2) {
        state = 2;
    } else if (pMiniGalaxy->mState == 1) {
        state = 1;
    } else if (pMiniGalaxy->mState == 0) {
        state = 0;
    }

    if (state == 3 && pMiniGalaxy == MiniatureGalaxyFunction::getCometLandMiniatureGalaxy()) {
        cometID = MiniatureGalaxyFunction::getCometNameId();
    }

    mInfo->show(pMiniGalaxy->mName, state, cometID);
}

bool AstroDomeGalaxySelector::tryStartLectureDemo(const Nerve* pNerve) {
    if (isNerve(&NrvAstroDomeGalaxySelector::AstroDomeGalaxySelectorNrvGalaxySelectStart::sInstance)) {
        if (!MR::isOnGameEventFlagEndButlerDomeLecture()) {
            MR::requestStartTimeKeepDemoWithoutCinemaFrame(this, ::cDemoNameDomeLecture, pNerve,
                                                           &NrvAstroDomeGalaxySelector::AstroDomeGalaxySelectorNrvWaitStartDemo::sInstance, 0);
            return true;
        }
    } else if (isNerve(&NrvAstroDomeGalaxySelector::AstroDomeGalaxySelectorNrvGalaxyConfirmStart::sInstance)) {
        if (!MR::isOnGameEventFlagEndButlerGalaxyMoveLecture()) {
            MR::requestStartTimeKeepDemoWithoutCinemaFrame(this, ::cDemoNameDomeLecture, pNerve,
                                                           &NrvAstroDomeGalaxySelector::AstroDomeGalaxySelectorNrvWaitStartDemo::sInstance, 0);
            return true;
        }
    }

    return false;
}

void AstroDomeGalaxySelector::exeGalaxySelectStart() {
    if (!SphereSelectorFunction::isSelectWait()) {
        return;
    }

    if (tryStartLectureDemo(&NrvAstroDomeGalaxySelector::AstroDomeGalaxySelectorNrvDemoDomeLecture::sInstance)) {
        return;
    }

    MR::appearStarCounter();
    setNerve(&NrvAstroDomeGalaxySelector::AstroDomeGalaxySelectorNrvGalaxySelect::sInstance);
}

void AstroDomeGalaxySelector::exeGalaxySelect() {
    if (MR::isFirstStep(this)) {
        mBackButton->appear();
    }

    if (SphereSelectorFunction::isValidPointing() && MR::testCorePadTriggerB(WPAD_CHAN0)) {
        MR::startSystemSE("SE_SY_GALAXY_DECIDE_CANCEL");

        if (mBackButton->isAppearing()) {
            mBackButton->kill();
        } else {
            mBackButton->disappear();
        }

        SphereSelectorFunction::selectCancel(false);
        setNerve(&NrvAstroDomeGalaxySelector::AstroDomeGalaxySelectorNrvGalaxySelectCancel::sInstance);
    } else {
        if (!MR::isOnGameEventFlagOffAstroDomeGuidance()) {
            MR::requestPointerGuidanceNoInformation();
        }

        if (!SphereSelectorFunction::isHandleHolding() && mBackButton->isPointing()) {
            SphereSelectorFunction::registerPointingTarget(this, HandlePointingPriority(2));

            if (!_9C) {
                MR::startSystemSE("SE_SY_BUTTON_CURSOR_ON");
            }

            _9C = true;
        } else {
            _9C = false;
        }

        if (SphereSelectorFunction::isPointingTarget(this)) {
            mBackButton->tryPointing();
        }

        MiniatureGalaxy* pointingMiniatureGalaxy = MiniatureGalaxyFunction::getPointingMiniatureGalaxy();

        if (pointingMiniatureGalaxy != nullptr) {
            showGalaxyInfo(pointingMiniatureGalaxy);
        }
    }
}

void AstroDomeGalaxySelector::exeGalaxySelectCancel() {
    if (mBackButton->isDisappearing() || MR::isDead(mBackButton)) {
        SphereSelectorFunction::selectEnd();
        kill();
    }
}

void AstroDomeGalaxySelector::exeGalaxyConfirmStart() {
    if (MR::isFirstStep(this)) {
        mBackButton->disappear();
    }

    showGalaxyInfo(::getSelectedMiniatureGalaxy());

    if (SphereSelectorFunction::isConfirmWait() &&
        !tryStartLectureDemo(&NrvAstroDomeGalaxySelector::AstroDomeGalaxySelectorNrvGalaxyMoveLecture::sInstance)) {
        setNerve(&NrvAstroDomeGalaxySelector::AstroDomeGalaxySelectorNrvGalaxyConfirm::sInstance);
    }
}

void AstroDomeGalaxySelector::exeGalaxyConfirm() {
    if (MR::isFirstStep(this)) {
        mConfirmLayout->appear();
    }

    showGalaxyInfo(::getSelectedMiniatureGalaxy());

    if (!mConfirmLayout->isSelected()) {
        return;
    }

    if (mConfirmLayout->isSelectedYes()) {
        SphereSelectorFunction::confirmed();
        MR::requestStartTimeKeepDemoMarioPuppetable(this, ::cDemoNameJumpOut,
                                                    &NrvAstroDomeGalaxySelector::AstroDomeGalaxySelectorNrvDemoJumpOut::sInstance,
                                                    &NrvAstroDomeGalaxySelector::AstroDomeGalaxySelectorNrvWaitStartDemo::sInstance, 0);
    } else {
        SphereSelectorFunction::confirmCancel(false);
        setNerve(&NrvAstroDomeGalaxySelector::AstroDomeGalaxySelectorNrvGalaxyConfirmCancel::sInstance);
    }
}

void AstroDomeGalaxySelector::exeGalaxyConfirmCancel() {
    if (SphereSelectorFunction::isSelectWait()) {
        setNerve(&NrvAstroDomeGalaxySelector::AstroDomeGalaxySelectorNrvGalaxySelect::sInstance);
    }
}

void AstroDomeGalaxySelector::exeDemoJumpOut() {
    if (MR::isFirstStep(this)) {
        MR::disappearStarCounter();
        mConfirmLayout->requestMovementOn();
        MR::requestMovementOn(mInfo);
    }

    if (MR::isDemoActive(::cDemoNameJumpOut)) {
        return;
    }

    const char* pGalaxyName = ::getSelectedMiniatureGalaxy()->mName;
    MiniatureGalaxy* pCometGalaxy = MiniatureGalaxyFunction::getCometLandMiniatureGalaxy();

    if (pCometGalaxy != nullptr && pCometGalaxy == SphereSelectorFunction::getSelectedTarget()) {
        MR::requestStartScenarioSelectForComet(pGalaxyName, MR::getEncounterGalaxyCometPowerStarId(pGalaxyName));
    } else {
        MR::requestStartScenarioSelect(pGalaxyName);
    }

    kill();
}

void AstroDomeGalaxySelector::exeDemoDomeLecture() {
    if (MR::isFirstStep(this)) {
        MR::requestMovementOnPlayer();
    }

    if (MR::isTimeKeepDemoActive()) {
        return;
    }

    MR::onGameEventFlagEndButlerDomeLecture();
    MR::appearStarCounter();
    setNerve(&NrvAstroDomeGalaxySelector::AstroDomeGalaxySelectorNrvGalaxySelect::sInstance);
}

void AstroDomeGalaxySelector::exeGalaxyMoveLecture() {
    if (MR::isFirstStep(this)) {
        MR::requestMovementOnPlayer();
    }

    if (MR::isTimeKeepDemoActive()) {
        return;
    }

    MR::onGameEventFlagEndButlerGalaxyMoveLecture();
    setNerve(&NrvAstroDomeGalaxySelector::AstroDomeGalaxySelectorNrvGalaxyConfirm::sInstance);
}

void AstroDomeGalaxySelector::exeWaitStartDemo() {
}
