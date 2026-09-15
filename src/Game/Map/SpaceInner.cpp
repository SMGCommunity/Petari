#include "Game/Map/SpaceInner.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace NrvSpaceInner {
    NEW_NERVE(HostTypeAppear, SpaceInner, Appear);
    NEW_NERVE(HostTypeDisappear, SpaceInner, Disappear);
};  // namespace NrvSpaceInner

SpaceInner::SpaceInner(const char* pName) : LiveActor(pName) {
}

void SpaceInner::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("SpaceInner", 0, false);
    MR::connectToSceneSky(this);
    initNerve(GET_NERVE(SpaceInner, HostTypeAppear));
    MR::invalidateClipping(this);
    MR::registerDemoSimpleCastAll(this);
    makeActorDead();
}

void SpaceInner::calcAnim() {
    mPosition.set(MR::getCamPos());
    LiveActor::calcAnim();
}

void SpaceInner::exeAppear() {
}

void SpaceInner::exeDisappear() {
    if (MR::isBrkStopped(this)) {
        kill();
    }
}

void SpaceInner::appear() {
    LiveActor::appear();
    MR::startBrk(this, "Appear");
    setNerve(GET_NERVE(SpaceInner, HostTypeAppear));
}

void SpaceInner::disappear() {
    MR::startBrk(this, "Disappear");
    setNerve(GET_NERVE(SpaceInner, HostTypeDisappear));
}

bool SpaceInner::isAppeared() const {
    if (isNerve(GET_NERVE(SpaceInner, HostTypeAppear))) {
        return MR::isBrkStopped(this);
    }

    return false;
}
