#include "Game/MapObj/MarioLauncherAttractor.hpp"
#include "Game/Gravity/GravityCreator.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

namespace NrvMarioLauncherAttractor {
    NEW_NERVE(MarioLauncherAttractorNrvWait, MarioLauncherAttractor, Wait);
};  // namespace NrvMarioLauncherAttractor

MarioLauncherAttractor::MarioLauncherAttractor(const char* pName) : LiveActor(pName), mGravityCreator() {
}

void MarioLauncherAttractor::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("MarioLauncherAttractor", nullptr, nullptr);

    // Create and initialize gravity
    mGravityCreator = new PointGravityCreator();
    mGravityCreator->createFromJMap(rIter);
    PlanetGravity* pGravity = mGravityCreator->getGravity();
    pGravity->mGravityType = GRAVITY_TYPE_MARIO_LAUNCHER;

    // Set clipping & scale
    f32 scale = pGravity->mRange / 1000.0f;
    MR::setClippingTypeSphere(this, pGravity->mRange);
    MR::setClippingFar200m(this);

    mScale.set(TVec3f(scale));

    // Connect to executor, init nerve and start animation
    MR::connectToSceneMapObj(this);
    initNerve(&NrvMarioLauncherAttractor::MarioLauncherAttractorNrvWait::sInstance);
    MR::startBtk(this, "MarioLauncherAttractor");

    makeActorAppeared();
}

void MarioLauncherAttractor::exeWait() {
}
