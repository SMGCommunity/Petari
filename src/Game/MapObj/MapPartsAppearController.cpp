#include "Game/MapObj/MapPartsAppearController.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/StageSwitch.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapPartsUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PostureHolder.hpp"

namespace NrvMapPartsAppearController {
    NEW_NERVE(HostTypeWait, MapPartsAppearController, Wait);
    NEW_NERVE(HostTypeDisappear, MapPartsAppearController, Disappear);
};  // namespace NrvMapPartsAppearController

MapPartsAppearController::MapPartsAppearController(LiveActor* pActor) : MapPartsFunction(pActor, "出現制御") {
    mSignMotionType = 0;
    mPostureHolder = nullptr;
    _20 = 0;
}

void MapPartsAppearController::init(const JMapInfoIter& rIter) {
    initNerve(GET_NERVE(MapPartsAppearController, HostTypeWait));
    initSwitchMessenger(rIter);
    MR::getMapPartsArgSignMotionType(&mSignMotionType, rIter);
    mPostureHolder = new PostureHolder(mHost);
}

void MapPartsAppearController::start() {
    setNerve(GET_NERVE(MapPartsAppearController, HostTypeWait));
}

void MapPartsAppearController::end() {
    setNerve(GET_NERVE(MapPartsAppearController, HostTypeWait));
}

void MapPartsAppearController::storeCurrentPosture() {
    mPostureHolder->store(mHost);
}

void MapPartsAppearController::initSwitchMessenger(const JMapInfoIter& rIter) {
    if (MR::isExistStageSwitchAppear(rIter)) {
        MR::listenNameObjStageSwitchOnOffAppear(this, MR::createStageSwitchCtrl(this, rIter),
                                                MR::Functor(this, &MapPartsAppearController::startAppear),
                                                MR::Functor(this, &MapPartsAppearController::startKill));
    }
}

bool MapPartsAppearController::receiveMsg(u32 msg) {
    if (msg == ACTMES_MAPPARTS_DISAPPEAR_WITH_BLINK) {
        if (MR::isDead(mHost)) {
            return false;
        }

        if (!isNerve(GET_NERVE(MapPartsAppearController, HostTypeWait))) {
            return false;
        }

        if (MR::hasMapPartsVanishSignMotion(mSignMotionType)) {
            setNerve(GET_NERVE(MapPartsAppearController, HostTypeDisappear));

            return true;
        }

        killHost();

        return true;
    }

    return false;
}

void MapPartsAppearController::startAppear() {
    if (MR::isValidSwitchAppear(mHost) && !_20) {
        if (isNerve(GET_NERVE(MapPartsAppearController, HostTypeDisappear))) {
            setNerve(GET_NERVE(MapPartsAppearController, HostTypeWait));
        }

        appearHost();
        _20 = 1;
    }
}

void MapPartsAppearController::startKill() {
    if (MR::isValidSwitchAppear(mHost)) {
        _20 = 0;
        if (!MR::isDead(mHost)) {
            if (isNerve(GET_NERVE(MapPartsAppearController, HostTypeWait))) {
                if (MR::hasMapPartsVanishSignMotion(mSignMotionType)) {
                    setNerve(GET_NERVE(MapPartsAppearController, HostTypeDisappear));
                } else {
                    killHost();
                }
            }
        }
    }
}

void MapPartsAppearController::appearHost() {
    if (MR::isDead(mHost)) {
        mHost->appear();
        mPostureHolder->store(mHost);
    }

    if (MR::isHiddenModel(mHost)) {
        MR::showModel(mHost);
    }
}

void MapPartsAppearController::killHost() {
    if (!MR::isDead(mHost)) {
        mHost->kill();
        mPostureHolder->restore(mHost);
    }
}

void MapPartsAppearController::exeWait() {
}

void MapPartsAppearController::exeDisappear() {
    s32 div = getStep() < 218 ? 13 : 6;
    if (getStep() % div == 0) {
        LiveActor* host = mHost;
        if (MR::isHiddenModel(host)) {
            MR::showModel(host);
        } else {
            MR::hideModel(host);
        }
    }

    if (isStep(436)) {
        setNerve(GET_NERVE(MapPartsAppearController, HostTypeWait));
        MR::showModel(mHost);
        killHost();
    }
}
