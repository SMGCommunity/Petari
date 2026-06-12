#include "Game/Boss/DinoPackunDemoPosition.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"

namespace {
    static const s32 sLandMotionStartTime = 28;
    static const s32 sJumpBackTime = 56;
};  // namespace

namespace NrvDinoPackunDemo {
    NEW_NERVE(DinoPackunDemoPositionNrvOpeningDemo, DinoPackunDemoPosition, OpeningDemo);
};  // namespace NrvDinoPackunDemo

DinoPackunDemoPosition::DinoPackunDemoPosition(const char* pName) : LiveActor(pName) {
}

void DinoPackunDemoPosition::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initNerve(&NrvDinoPackunDemo::DinoPackunDemoPositionNrvOpeningDemo::sInstance);
    initModelManagerWithAnm("DinoPackunDemoPosition", nullptr, false);
    MR::invalidateClipping(this);
    makeActorDead();
}

void DinoPackunDemoPosition::control() {
    return;
}

void DinoPackunDemoPosition::startOpeningDemo() {
    setNerve(&NrvDinoPackunDemo::DinoPackunDemoPositionNrvOpeningDemo::sInstance);
    makeActorAppeared();
}

void DinoPackunDemoPosition::endDemo() {
    makeActorDead();
}

void DinoPackunDemoPosition::exeOpeningDemo() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "OpeningDemo", nullptr);
    }

    if (MR::isStep(this, 0)) {
        MR::startBckPlayer("CocoonFly", static_cast< const char* >(nullptr));
    }

    if (MR::isStep(this, ::sLandMotionStartTime)) {
        MR::startBckPlayer("LandStiffen", static_cast< const char* >(nullptr));
    }

    if (MR::isStep(this, ::sJumpBackTime)) {
        MR::startBckPlayer("JumpBack", static_cast< const char* >(nullptr));
    }

    calcAnim();
    MR::setPlayerBaseMtx(MR::getJointMtx(this, "MarioPosition"));
}

DinoPackunDemoPosition::~DinoPackunDemoPosition() {
}
