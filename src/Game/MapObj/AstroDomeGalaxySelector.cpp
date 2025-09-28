#include "Game/MapObj/AstroDomeGalaxySelector.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/SphereSelector.hpp"
#include "Game/MapObj/MiniatureGalaxy.hpp"
#include "Game/MapObj/MiniatureGalaxyHolder.hpp"
#include "Game/Screen/GalaxyConfirmLayout.hpp"
#include "Game/Screen/GalaxySelectBackButton.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

namespace NrvAstroDomeGalaxySelector {
    NEW_NERVE(AstroDomeGalaxySelectorNrvGalaxySelectStart, AstroDomeGalaxySelector, GalaxySelectStart);
    NEW_NERVE(AstroDomeGalaxySelectorNrvGalaxySelect, AstroDomeGalaxySelector, GalaxySelect);
    NEW_NERVE(AstroDomeGalaxySelectorNrvGalaxyConfirmStart, AstroDomeGalaxySelector, GalaxyConfirmStart);
    NEW_NERVE(AstroDomeGalaxySelectorNrvGalaxySelectCancel, AstroDomeGalaxySelector, GalaxySelectCancel);
    NEW_NERVE(AstroDomeGalaxySelectorNrvWaitStartDemo, AstroDomeGalaxySelector, WaitStartDemo);
    NEW_NERVE(AstroDomeGalaxySelectorNrvDemoDomeLecture, AstroDomeGalaxySelector, DemoDomeLecture);
}

namespace {
    const char* cDemoNameDomeLecture = "ドームレクチャー２";
}

AstroDomeGalaxySelector::AstroDomeGalaxySelector(const char* pName) : LiveActor(pName) {
    pDomeCamCtrl = 0;
    pGSBackButton = 0;
    pGSInfo = 0;
    pGConfirmLayout = 0;
    _9C = 0;
}

void AstroDomeGalaxySelector::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjMovement(this);
    MR::invalidateClipping(this);
    pDomeCamCtrl = new AstroDomeCameraController("天文ドームカメラ制御");
    pDomeCamCtrl->initWithoutIter();
    pGSBackButton = new GalaxySelectBackButton();
    pGSBackButton->initWithoutIter();
    pGSInfo = new GalaxySelectInfo(false);
    pGSInfo->initWithoutIter();
    pGConfirmLayout = new GalaxyConfirmLayout();
    pGConfirmLayout->initWithoutIter();
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
    pGSInfo->makeActorAppeared();
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
        pGSBackButton->decide();
        SphereSelectorFunction::selectCancel(false);
        setNerve(&NrvAstroDomeGalaxySelector::AstroDomeGalaxySelectorNrvGalaxySelectCancel::sInstance);
        return true;
    }
    return false;
}

/* void AstroDomeGalaxySelector::showGalaxyInfo(const MiniatureGalaxy* pMiniGalaxy) {
    switch ( pMiniGalaxy->??? )
    {
        case 2:

        break;
        case 1:

        break;
        case 0:

        break;
    }
} */

bool AstroDomeGalaxySelector::tryStartLectureDemo(const Nerve* pNerve) {
    if (isNerve(&NrvAstroDomeGalaxySelector::AstroDomeGalaxySelectorNrvGalaxySelectStart::sInstance)) {
        if (!MR::isOnGameEventFlagEndButlerDomeLecture()) {
            MR::requestStartTimeKeepDemoWithoutCinemaFrame(this, cDemoNameDomeLecture, pNerve, &NrvAstroDomeGalaxySelector::AstroDomeGalaxySelectorNrvWaitStartDemo::sInstance, 0);
            return true;
        }
    }
    else if (isNerve(&NrvAstroDomeGalaxySelector::AstroDomeGalaxySelectorNrvGalaxyConfirmStart::sInstance)) {
        if (!MR::isOnGameEventFlagEndButlerGalaxyMoveLecture()) {
            MR::requestStartTimeKeepDemoWithoutCinemaFrame(this, cDemoNameDomeLecture, pNerve, &NrvAstroDomeGalaxySelector::AstroDomeGalaxySelectorNrvWaitStartDemo::sInstance, 0);
            return true;
        }
    }
    return false;
}

void AstroDomeGalaxySelector::exeGalaxySelectStart() {
    if (SphereSelectorFunction::isSelectWait()) {
        if (!tryStartLectureDemo(&NrvAstroDomeGalaxySelector::AstroDomeGalaxySelectorNrvDemoDomeLecture::sInstance)) {
            MR::appearStarCounter();
            setNerve(&NrvAstroDomeGalaxySelector::AstroDomeGalaxySelectorNrvGalaxySelect::sInstance);
        }
    }
}

void AstroDomeGalaxySelector::exeGalaxySelect() {
    if (MR::isFirstStep(this)) {
        pGSBackButton->appear();
    }
    if (SphereSelectorFunction::isValidPointing()) {
        if (MR::testCorePadTriggerB(0)) {
            MR::startSystemSE("SE_SY_GALAXY_DECIDE_CANCEL", -1, -1);
            if (pGSBackButton->isAppearing()) {
                pGSBackButton->kill();
            }
            pGSBackButton->disappear();
        }
        SphereSelectorFunction::selectCancel(false);
        setNerve(&NrvAstroDomeGalaxySelector::AstroDomeGalaxySelectorNrvGalaxySelectCancel::sInstance);
    }
    else {
        if (!MR::isOnGameEventFlagOffAstroDomeGuidance()) {
            MR::requestPointerGuidanceNoInformation();
        }
        if (SphereSelectorFunction::isHandleHolding()) {
            if (pGSBackButton->isPointing()) {
                SphereSelectorFunction::registerPointingTarget(this, HandlePointingPriority(2));
                if (!_9C) {
                    MR::startSystemSE("SE_SY_BUTTON_CURSOR_ON", -1, -1);
                }
                _9C = true;
            }
            else {
                _9C = false;
            }
            if (SphereSelectorFunction::isPointingTarget(this)) {
                pGSBackButton->tryPointing();
            }
            MiniatureGalaxy* miniGalaxy = MiniatureGalaxyFunction::getPointingMiniatureGalaxy();
            if (miniGalaxy) {
                showGalaxyInfo(miniGalaxy);
            }
        }
    }
}
