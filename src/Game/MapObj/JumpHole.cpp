#include "Game/MapObj/JumpHole.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

namespace {
    static TVec3f sTestBindSpherePos(0, 0, 0);
    static TVec3f sTestBindSphereVelocity(0, 0, 0);
};  // namespace

namespace NrvJumpHole {
    NEW_NERVE(JumpHoleNrvWait, JumpHole, Wait);
    NEW_NERVE(JumpHoleNrvSetCenter, JumpHole, SetCenter);
    NEW_NERVE(JumpHoleNrvSetUp, JumpHole, SetUp);
    NEW_NERVE(JumpHoleNrvRailMove, JumpHole, RailMove);
};  // namespace NrvJumpHole

JumpHole::JumpHole(const char* pName) : LiveActor(pName), _90(nullptr), _98(0, 0, 0), _A4(0, 0, 0), _B0(0, 0, 0), _E4(0) {
    mCone = new BindCone(mPosition, TVec3f(0.0f, 1.0f, 0.0f), 400.0f, 200.0f);
}

void JumpHole::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("JumpHole", nullptr, false);
    MR::connectToSceneMapObj(this);
    initRailRider(rIter);
    initHitSensor(1);
    MR::addHitSensor(this, "body", 0x79, 8, 200.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initSound(4, false);
    MR::initCollisionParts(this, "JumpHole", getSensor(nullptr), 0);
    mCone->setPosition(mPosition);
    initNerve(&NrvJumpHole::JumpHoleNrvWait::sInstance);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

void JumpHole::initAfterPlacement() {
    MR::calcGravityVector(this, &_B0, nullptr, 0);
    mCone->setDirection(_B0);
}

void JumpHole::control() {
    return;
}

bool JumpHole::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_SPHERE_PLAYER_BINDED) {
        f32 v6 = pReceiver->mRadius * pReceiver->mRadius;
        TVec3f v10(pReceiver->mPosition);
        v10 -= pSender->mPosition;
        f32 v7 = _B0.dot(v10);
        TVec3f v9;
        JMAVECScaleAdd(&_B0, &v10, &v9, -v7);

        if (v9.squared() > v6 || v10.squared(v9) > 25600.0f) {
            return false;
        }

        _90 = pSender;
        _A4 = pSender->mHost->mVelocity;
        MR::zeroVelocity(_90->mHost);
        return true;
    }

    return false;
}

void JumpHole::exeWait() {
    if (_90 != nullptr) {
        bindHole();
        setNerve(&NrvJumpHole::JumpHoleNrvSetCenter::sInstance);
        MR::invalidateClipping(this);
    }
}

void JumpHole::exeSetCenter() {
    if (MR::isFirstStep(this)) {
        _E4 = 0;
    }

    bindHole();

    if (_A4.squared() < 0.009f) {
        _E4++;
    } else {
        _E4 = 0;
    }

    MR::startLevelSound(this, "SE_OJ_LV_JUMP_HOLE_SETTING");

    if (_E4 > 10 || MR::isGreaterStep(this, 180)) {
        setNerve(&NrvJumpHole::JumpHoleNrvSetUp::sInstance);
        _A4.zero();
    }
}

void JumpHole::exeSetUp() {
    if (MR::isFirstStep(this)) {
        _90->receiveMessage(ACTMES_SET_UP_JUMP_HOLE, getSensor("body"));
    }

    if (MR::isGreaterStep(this, 45)) {
        setNerve(&NrvJumpHole::JumpHoleNrvRailMove::sInstance);
    }
}

JumpHole::~JumpHole() {
}
