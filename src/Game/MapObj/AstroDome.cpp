#include "Game/MapObj/AstroDome.hpp"
#include "Game/MapObj/AstroMapObjFunction.hpp"
#include "Game/Map/SphereSelector.hpp"

AstroDome::AstroDome(const char *pName) : MapObjActor(pName) {

}

void AstroDome::init(const JMapInfoIter &rIter) {
    MapObjActorInitInfo info;
    s32 domeId = AstroMapObjFunction::getDomeIdFromArg0(rIter);
    info.setupModelName(AstroMapObjFunction::getModelName("AstroDome", domeId));
    info.setupNerve(&NrvAstroDome::AstroDomeNrvWait::sInstance);
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    info.setupNoAppearRiddleSE();
    initialize(rIter, info);
    MR::invalidateClipping(this);
    SphereSelectorFunction::registerTarget(this);
    MR::registerDemoSimpleCastAll(this);
    makeActorAppeared();
}

void AstroDome::appear() {
    MapObjActor::appear();
    setNerve(&NrvAstroDome::AstroDomeNrvAppear::sInstance);
}

void AstroDome::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "Appear");
        MR::setAllAnimFrameAtEnd(this, "Appear");
    }
}

void AstroDome::exeDisappear() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "Disappear");
        MR::invalidateCollisionParts(this);
    }

    if (MR::isBrkStopped(this)) {
        kill();
    }
}

void AstroDome::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::showModel(this);
        MR::startBrk(this, "Appear");
        MR::validateCollisionParts(this);
    }

    if (MR::isBrkStopped(this)) {
        setNerve(&NrvAstroDome::AstroDomeNrvWait::sInstance);
    }
}

void AstroDome::control() {
    if (MR::isEqualString(mObjectName, "AstroDomeObservatory")) {
        if (MR::isTimeKeepDemoActive()) {
            MR::hideMaterial(this, "Z_SpotLight_v");
        }
        else {
            MR::showMaterial(this, "Z_SpotLight_v");
        }
    }
}

bool AstroDome::receiveOtherMsg(u32 msg, HitSensor *, HitSensor *) {
    if (SphereSelectorFunction::isMsgSelectStart(msg)) {
        setNerve(&NrvAstroDome::AstroDomeNrvDisappear::sInstance);
        return true;
    }
    else if (SphereSelectorFunction::isMsgSelectEnd(msg)) {
        appear();
        return true;
    }

    return false;
}

AstroDome::~AstroDome() {

}

namespace NrvAstroDome {
    INIT_NERVE(AstroDomeNrvWait);
    INIT_NERVE(AstroDomeNrvDisappear);
    INIT_NERVE(AstroDomeNrvAppear);
};