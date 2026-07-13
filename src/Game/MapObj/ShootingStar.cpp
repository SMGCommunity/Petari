#include "Game/MapObj/ShootingStar.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

namespace NrvShootingStar {
    NEW_NERVE(HostTypeNrvPreShooting, ShootingStar, PreShooting);
    NEW_NERVE(HostTypeNrvShooting, ShootingStar, Shooting);
    NEW_NERVE(HostTypeNrvWaitForNextShoot, ShootingStar, WaitForNextShoot);
};  // namespace NrvShootingStar

ShootingStar::ShootingStar(const char* pName) : LiveActor(pName), _8C(0, 0, 0), _98(0, 0, 1), _A4(0, -1, 0) {
    _B0 = 5;
    _B4 = 0xF0;
    _B8 = 2000.0f;
}

void ShootingStar::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm("ShootingStar", nullptr, false);
    MR::connectToSceneMapObj(this);
    MR::initDefaultPos(this, rIter);
    _8C = mPosition;
    s32 arg0 = 5;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);
    _B0 = arg0;
    arg0 = 0xF0;
    MR::getJMapInfoArg1NoInit(rIter, &arg0);
    _B4 = arg0;
    f32 arg2 = -1.0f;
    MR::getJMapInfoArg2NoInit(rIter, &arg2);

    if (arg2 != 1.0f) {
        _B8 = arg2;
    }

    arg0 = 5;
    MR::getJMapInfoArg3NoInit(rIter, &arg0);
    MR::declareStarPiece(this, arg0);
    initBinder(100.0f, 0.0f, 0);
    MR::setBindTriangleFilter(this, MR::createTriangleFilterFunc(MR::isWallCodeNoAction));
    initNerve(&NrvShootingStar::HostTypeNrvPreShooting::sInstance);
    initEffectKeeper(0, "ShootingStar", false);
    initSound(4, false);
    initHitSensor(1);
    MR::addHitSensorMapObj(this, "message", 1, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::initShadowVolumeSphere(this, 30.0f);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::listenStageSwitchOnOffAppear(this, MR::Functor_Inline(this, &ShootingStar::appearPreShooting),
                                         MR::Functor_Inline< LiveActor >(this, &LiveActor::kill));
        makeActorDead();
    } else {
        makeActorAppeared();
    }

    LiveActor::calcAndSetBaseMtx();
    TPos3f matrix;
    matrix.setInline(getBaseMtx());
    matrix.getYDir(_98);
    MR::startBpk(this, "ShootingStar");
}

void ShootingStar::appearPreShooting() {
    appear();
    MR::invalidateClipping(this);
    setNerve(&NrvShootingStar::HostTypeNrvPreShooting::sInstance);
}

/*
void ShootingStar::control() {

    mRotation.y = zero + fmod((360.0f + ((10.0f + mRotation.y) - zero)), 360.0);
}

void ShootingStar::exePreShooting() {
    if (MR::isFirstStep(this)) {
        MR::calcGravityVector(this, &_A4, nullptr, 0);
        MR::showModel(this);
        MR::emitEffect(this, "ShooingStarAppear");
        mPosition.x = _8C.x;
        mPosition.y = _8C.y;
        mPosition.z = _8C.z;
        TVec3f stack_8 = _98 * _B8;
    }
}*/

// ShootingSTar::exeShooting

void ShootingStar::exeWaitForNextShoot() {
    if (MR::isFirstStep(this)) {
        MR::hideModel(this);
        MR::emitEffect(this, "ShootingStarBreak");
        mVelocity.zero();

        if (!MR::isValidSwitchAppear(this)) {
            MR::validateClipping(this);
        }

        MR::offCalcAnim(this);
        MR::invalidateShadow(this, nullptr);
    }

    if (!MR::getDeclareRemnantStarPieceCount(this)) {
        kill();
    }

    if (MR::isGreaterStep(this, _B4)) {
        MR::onCalcAnim(this);
        MR::validateShadow(this, nullptr);
        setNerve(&NrvShootingStar::HostTypeNrvPreShooting::sInstance);
    }
}

ShootingStar::~ShootingStar() {
}
