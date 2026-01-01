#include "Game/Map/SpaceInner.hpp"
#include "Game/Util.hpp"

namespace NrvSpaceInner {
    NEW_NERVE(HostTypeAppear, SpaceInner, Appear);
    NEW_NERVE(HostTypeDisappear, SpaceInner, Disappear);
};  // namespace NrvSpaceInner

SpaceInner::SpaceInner(const char* pName) : LiveActor(pName) {}

void SpaceInner::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("SpaceInner", 0, false);
    MR::connectToSceneSky(this);
    initNerve(&NrvSpaceInner::HostTypeAppear::sInstance);
    MR::invalidateClipping(this);
    MR::registerDemoSimpleCastAll(this);
    makeActorDead();
}

void SpaceInner::calcAnim() {
    TVec3f pos = MR::getCamPos();
    mPosition.x = pos.x;
    mPosition.y = pos.y;
    mPosition.z = pos.z;
    LiveActor::calcAnim();
}

void SpaceInner::exeAppear() {}

void SpaceInner::exeDisappear() {
    if (MR::isBrkStopped(this)) {
        kill();
    }
}

void SpaceInner::appear() {
    LiveActor::appear();
    MR::startBrk(this, "Appear");
    setNerve(&NrvSpaceInner::HostTypeAppear::sInstance);
}

void SpaceInner::disappear() {
    MR::startBrk(this, "Disappear");
    setNerve(&NrvSpaceInner::HostTypeDisappear::sInstance);
}

bool SpaceInner::isAppeared() const {
    if (isNerve(&NrvSpaceInner::HostTypeAppear::sInstance)) {
        return MR::isBrkStopped(this);
    }

    return false;
}

SpaceInner::~SpaceInner() {}
