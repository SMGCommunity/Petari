#include "Game/MapObj/AstroDomeGalaxySelector.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/SphereSelector.hpp"
#include "Game/MapObj/MiniatureGalaxy.hpp"
#include "Game/Screen/GalaxyConfirmLayout.hpp"
#include "Game/Screen/GalaxySelectBackButton.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"

namespace NrvAstroDomeGalaxySelector {
    NEW_NERVE(AstroDomeGalaxySelectorNrvGalaxySelectStart, AstroDomeGalaxySelector, GalaxySelectStart);
    NEW_NERVE(AstroDomeGalaxySelectorNrvGalaxySelect, AstroDomeGalaxySelector, GalaxySelect);
    NEW_NERVE(AstroDomeGalaxySelectorNrvGalaxyConfirmStart, AstroDomeGalaxySelector, GalaxyConfirmStart);
    NEW_NERVE(AstroDomeGalaxySelectorNrvGalaxySelectCancel, AstroDomeGalaxySelector, GalaxySelectCancel);

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
    if (isNerve(pNerve)) {
        if (MR::isOnGameEventFlagEndButlerDomeLecture()) {
            
        }
    }
}
