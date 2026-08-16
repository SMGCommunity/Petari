#include "Game/MapObj/ShootingStar.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

void ShootingStar_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)-1.0f;
}

namespace {
    static const f32 sFallHeight = 2000.0f;
    // static const f32 sAppearWidth =
    // static const f32 sShootingStarSize =
    // static const f32 sShootingStarSizeScale =
    // static const f32 sShootingStarTailSize =
    // static const f32 sGotRotateSlow =
    // static const f32 sGotRotateFast =
    static const f32 sFallRotateYAngle = 10.0f;
    static const f32 sFallVelScale = 25.0f;
    static const s32 sShootingLimitTime = 360;
    static const s32 sPreShootingTime = 20;
    static const s32 sWaitForNextShootTime = 240;
    // static const s32 sClearTime =
    // static const s32 sRandomPieceValue =
    static const f32 sLaunchYVel = 40.0f;
    // static const s32 sGetKind =
};  // namespace

namespace NrvShootingStar {
    NEW_NERVE(HostTypeNrvPreShooting, ShootingStar, PreShooting);
    NEW_NERVE(HostTypeNrvShooting, ShootingStar, Shooting);
    NEW_NERVE(HostTypeNrvWaitForNextShoot, ShootingStar, WaitForNextShoot);
};  // namespace NrvShootingStar

ShootingStar::ShootingStar(const char* pName)
    : LiveActor(pName), mBasePos(0, 0, 0), mFallAxis(0, 0, 1), mGravityPreShoot(0, -1, 0), mNumBurstStarPiece(5),
      mWaitForNextShootTime(::sWaitForNextShootTime), mFallHeight(::sFallHeight) {
}

void ShootingStar::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm("ShootingStar", nullptr, false);
    MR::connectToSceneMapObj(this);
    MR::initDefaultPos(this, rIter);
    mBasePos = mPosition;
    s32 arg0 = 5;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);
    mNumBurstStarPiece = arg0;
    arg0 = ::sWaitForNextShootTime;
    MR::getJMapInfoArg1NoInit(rIter, &arg0);
    mWaitForNextShootTime = arg0;
    f32 arg2 = -1.0f;
    MR::getJMapInfoArg2NoInit(rIter, &arg2);

    if (arg2 != -1.0f) {
        mFallHeight = arg2;
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
    TPos3f matrix = getBaseMtx();
    matrix.getYDir(mFallAxis);
    MR::startBpk(this, "ShootingStar");
}

void ShootingStar::appearPreShooting() {
    appear();
    MR::invalidateClipping(this);
    setNerve(&NrvShootingStar::HostTypeNrvPreShooting::sInstance);
}

void ShootingStar::control() {
    mRotation.y = MR::repeatDegree(mRotation.y + ::sFallRotateYAngle);
}

void ShootingStar::exePreShooting() {
    if (MR::isFirstStep(this)) {
        MR::calcGravityVector(this, &mGravityPreShoot, nullptr, 0);
        MR::showModel(this);
        MR::emitEffect(this, "ShootingStarAppear");
        mPosition.set(mBasePos);
        mPosition.add(mFallAxis * mFallHeight);
        mVelocity.zero();
        if (!MR::isValidSwitchAppear(this)) {
            MR::invalidateClipping(this);
        }
    }

    mScale.set(static_cast< f32 >(getNerveStep()) / ::sPreShootingTime);

    if (MR::isGreaterStep(this, ::sPreShootingTime)) {
        setNerve(&NrvShootingStar::HostTypeNrvShooting::sInstance);
    }
}

void ShootingStar::exeShooting() {
    if (MR::isFirstStep(this)) {
        mVelocity = -mFallAxis * ::sFallVelScale;
        MR::emitEffect(this, "ShootingStarBlur");
        mScale.set(1.0f, 1.0f, 1.0f);
    }

    MR::startLevelSound(this, "SE_OJ_LV_STAR_PIECE_FALL");
    if (MR::isGreaterStep(this, ::sShootingLimitTime)) {
        setNerve(&NrvShootingStar::HostTypeNrvPreShooting::sInstance);
        MR::deleteEffect(this, "ShootingStarBlur");
        return;
    }

    if (MR::isBinded(this)) {
        setNerve(&NrvShootingStar::HostTypeNrvWaitForNextShoot::sInstance);

        if (MR::appearStarPiece(this, mPosition, mNumBurstStarPiece, 15.0f, ::sLaunchYVel, false)) {
            MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
        } else {
            MR::startSound(this, "SE_OJ_STAR_PIECE_NOT_BURST");
        }

        MR::deleteEffect(this, "ShootingStarBlur");
    }
}

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

    if (MR::isGreaterStep(this, mWaitForNextShootTime)) {
        MR::onCalcAnim(this);
        MR::validateShadow(this, nullptr);
        setNerve(&NrvShootingStar::HostTypeNrvPreShooting::sInstance);
    }
}
