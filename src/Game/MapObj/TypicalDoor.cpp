#include "Game/MapObj/TypicalDoor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/MapObj/StageEffectDataTable.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace NrvTypicalDoor {
    NEW_NERVE(HostTypeClose, TypicalDoor, Close);
    NEW_NERVE(HostTypeCloseForDemo, TypicalDoor, Close);
    NEW_NERVE(HostTypeOpen, TypicalDoor, Open);
};  // namespace NrvTypicalDoor

TypicalDoor::TypicalDoor(const char* pName) : MapObjActor(pName) {
    mCloseCollision = nullptr;
    mOpenCollision = nullptr;
}

void TypicalDoor::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    info.setupHioNode("地形オブジェ");
    info.setupDefaultPos();
    info.setupConnectToScene();
    info.setupSound(4);
    info.setupEffect(nullptr);
    info.setupNerve(&NrvTypicalDoor::HostTypeClose::sInstance);
    MapObjActor::initialize(rIter, info);

    if (info.mHasSensors) {
        initHitSensor(1);
        MR::addBodyMessageSensorMapObj(this);
    }

    if (MR::isExistCollisionResource(this, "Close")) {
        mCloseCollision = MR::createCollisionPartsFromLiveActor(this, "Close", getSensor("body"), (MR::CollisionScaleType)2);
        MR::invalidateCollisionParts(mCloseCollision);
    }

    if (MR::isExistCollisionResource(this, "Open")) {
        mOpenCollision = MR::createCollisionPartsFromLiveActor(this, "Open", getSensor("body"), (MR::CollisionScaleType)2);
        MR::invalidateCollisionParts(mOpenCollision);
    }

    if (MR::isDemoPartExist(this, "扉開く")) {
        MR::registerDemoActionNerve(this, &NrvTypicalDoor::HostTypeOpen::sInstance, "扉開く");
        setNerve(&NrvTypicalDoor::HostTypeCloseForDemo::sInstance);
    }

    makeActorAppeared();
}

void TypicalDoor::initCaseUseSwitchB(const MapObjActorInitInfo& rInfo) {
    MR::listenStageSwitchOnB(this, MR::Functor(this, &TypicalDoor::open));
}

void TypicalDoor::listenForClose() {
    MR::listenStageSwitchOnB(this, MR::Functor(this, &TypicalDoor::close));
}

void TypicalDoor::open() {
    setNerve(&NrvTypicalDoor::HostTypeOpen::sInstance);
}

void TypicalDoor::close() {
    setNerve(&NrvTypicalDoor::HostTypeClose::sInstance);
}

void TypicalDoor::exeClose() {
    if (MR::isFirstStep(this)) {
        MR::tryStartAllAnim(this, "Close");
        if (MR::StageEffect::isExistStageEffectSeData(mObjectName)) {
            const char* stopSe = MR::StageEffect::getStopSe(mObjectName);
            if (stopSe != nullptr) {
                MR::startSound(this, stopSe, -1, -1);
            }
        }
    }

    if (MR::isStep(this, 0)) {
        if (mOpenCollision != nullptr) {
            MR::invalidateCollisionParts(mOpenCollision);
        }

        if (mCloseCollision != nullptr) {
            MR::validateCollisionParts(mCloseCollision);
        }
    }
}

void TypicalDoor::exeOpen() {
    if (MR::isFirstStep(this)) {
        MR::tryStartAllAnim(this, "Open");
        if (MR::StageEffect::isExistStageEffectSeData(mObjectName)) {
            const char* startSe = MR::StageEffect::getStartSe(mObjectName);
            if (startSe != nullptr) {
                MR::startSound(this, startSe, -1, -1);
            }
        }
    }

    if (MR::isStep(this, 30)) {
        if (mCloseCollision != nullptr) {
            MR::invalidateCollisionParts(mCloseCollision);
        }

        if (mOpenCollision != nullptr) {
            MR::validateCollisionParts(mOpenCollision);
        }
    }
}

void TypicalDoorOpen::init(const JMapInfoIter& rIter) {
    TypicalDoor::init(rIter);
    open();
}

void TypicalDoorOpen::initCaseUseSwitchB(const MapObjActorInitInfo& rInfo) {
    listenForClose();
}

DarknessRoomDoor::DarknessRoomDoor(const char* pName) : TypicalDoor(pName) {
    _CC = false;
}

TypicalDoor::~TypicalDoor() {}

void DarknessRoomDoor::init(const JMapInfoIter& rIter) {
    s32 arg0 = -1;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);
    _CC = arg0 != -1;
    TypicalDoor::init(rIter);
    MR::useStageSwitchReadA(this, rIter);
}

void DarknessRoomDoor::control() {
    MapObjActor::control();
    if (!_CC && MR::isValidSwitchA(this)) {
        if (MR::isOnSwitchA(this)) {
            validateCollision();
        } else {
            invalidateCollision();
        }
    }
}

void DarknessRoomDoor::connectToScene(const MapObjActorInitInfo& rIter) {
    if (_CC) {
        MapObjActor::connectToScene(rIter);
    } else {
        MR::connectToClippedMapParts(this);
    }
}

void DarknessRoomDoor::validateCollision() {
    if (!MR::isValidCollisionParts(this)) {
        MR::validateCollisionParts(this);
    }

    if (isNerve(&NrvTypicalDoor::HostTypeClose::sInstance)) {
        if (mCloseCollision != nullptr) {
            MR::validateCollisionParts(mCloseCollision);
        }
    }

    if (isNerve(&NrvTypicalDoor::HostTypeOpen::sInstance)) {
        if (mOpenCollision != nullptr) {
            MR::validateCollisionParts(mOpenCollision);
        }
    }
}

void DarknessRoomDoor::invalidateCollision() {
    if (MR::isValidCollisionParts(this)) {
        MR::invalidateCollisionParts(this);
    }

    if (isNerve(&NrvTypicalDoor::HostTypeClose::sInstance)) {
        if (mCloseCollision != nullptr) {
            MR::invalidateCollisionParts(mCloseCollision);
        }
    }

    if (isNerve(&NrvTypicalDoor::HostTypeOpen::sInstance)) {
        if (mOpenCollision != nullptr) {
            MR::invalidateCollisionParts(mOpenCollision);
        }
    }
}

TypicalDoorOpen::~TypicalDoorOpen() {}

DarknessRoomDoor::~DarknessRoomDoor() {}
