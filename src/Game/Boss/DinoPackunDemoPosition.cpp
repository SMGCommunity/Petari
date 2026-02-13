#include "Game/Boss/DinoPackunDemoPosition.hpp"
#include "Game/Boss/DinoPackun.hpp"

namespace NrvDinoPackunDemo {
    NEW_NERVE(DinoPackunDemoPositionNrvOpeningDemo, DinoPackunDemoPosition, OpeningDemo);
};

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
        MR::startBckPlayer("CocoonFly", static_cast< const char* >(0));
    }

    if (MR::isStep(this, 28)) {
        MR::startBckPlayer("LandStiffen", static_cast< const char* >(0));
    }

    if (MR::isStep(this, 56)) {
        MR::startBckPlayer("JumpBack", static_cast< const char* >(0));
    }

    calcAnim();
    MR::setPlayerBaseMtx(MR::getJointMtx(this, "MarioPosition"));
}

DinoPackunDemoPosition::~DinoPackunDemoPosition() {
    return;
}
