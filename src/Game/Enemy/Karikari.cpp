#include "Game/Enemy/Karikari.hpp"
#include "Game/Enemy/KarikariDirector.hpp"
#include "Game/LiveActor/HitSensor.hpp"

namespace {
    const f32 sSize = 30.0f;

    static const f32 sGravity = 2.4f;
    static const f32 sTurnRatio = 0.25f;
    // static const f32 sTornadoPower = ; // unused
    // static const f32 sTornadoUpPower = ; // unused
    // static const f32 sTornadoTangentPower = ; // unused
    static const f32 sVelocityDampGround = 0.8f;
    static const f32 sVelocityDampAir = 0.98f;
    static const f32 sStopVelocitySquared = 100.0f;
    static const s32 sJumpAgainTime = 11;
    static const s32 sStopSceneTime = 5;
    static const f32 sScratchStrongVel = 18.0f;
    static const f32 sFromCursorVelScale = 5.0f;
    static const f32 sJumpToPrePursueVel = 50.0f;
    static const f32 sJumpToPursueVel = 33.0f;
    static const f32 sPursueVel = 10.0f;
    static const f32 sJumpAnimRate = 1.0f;
    static const f32 sDistToPursue = 1000.0f;
    static const f32 sWatchForGravity = 1.4f;
    static const f32 sWatchForJumpVel = 35.0f;
    static const f32 sWatchForDistance = 500.0f;
    static const s32 sWatchForJumpTime = 7;
    static const s32 sWatchForLoopInterval = 45;
    static const s32 sPreClingTime = 3;
    static const f32 sDistToCling = 80.0f;
    static const s32 sSpinAttackedTime = 8;
    static const f32 sVelocityDampSpinAttacked = 0.9f;
    // static const f32 sSpinAttackedVel = ;
    // static const f32 sSpinAttackedVerticalVel = ;
    static const s32 sBlowOutTime = 60;
    static const f32 sBlowOutGravity = 1.3f;
    static const f32 sBlowOutVel = 10.0f;
    static const f32 sBlowOutVerticalVel = 38.0f;
    static const f32 sDPDAttackedGravity = 2.4f;
    static const f32 sVelocityDampAttacked = 0.9f;
    static const f32 sAttackedVel = 43.0f;
    static const f32 sAttackedVerticalVel = 33.0f;
    static const s32 sFrozenLimitTime = 180;
    // static const f32 sSpinFrozenAttackedVel = ;
    // static const f32 sFrozenAttackedBySpinRadius = ;
    // static const f32 sDeathEffectScale = ;
    // static const f32 sIceShadowSize = ;
    static const f32 sFrozenRecoverGravity = 2.4f;
    static const f32 sJumpToFrozenRecoverVel = 40.0f;
    static const s32 sPreWaitTime = 90;
    static const s32 sBeginWaitMotionFrame = 20;
    static const s32 sPressTime = 40;
    static const f32 sExplosionBlowOutVel = 30.0f;
    static const f32 sEnemyAttackHitVel = 30.0f;
    static const f32 sSpinXAnimRate = 2.0f;

    static const f32 sGravitySpinAttacked = 0.0f;
    static const s32 sIceAppearTime = 0;

    KarikariDirector* getKarikariDirector() {
        return MR::getSceneObj< KarikariDirector >(SceneObj_KarikariDirector);
    }
}  // namespace

namespace NrvKarikari {
    NEW_NERVE(HostTypeNrvFall, Karikari, Fall);
    NEW_NERVE(HostTypeNrvLand, Karikari, Land);
    NEW_NERVE(HostTypeNrvNoCalcWait, Karikari, NoCalcWait);
    NEW_NERVE(HostTypeNrvWait, Karikari, Wait);
    NEW_NERVE(HostTypeNrvPrePursue, Karikari, PrePursue);
    NEW_NERVE(HostTypeNrvPursue, Karikari, Pursue);
    NEW_NERVE(HostTypeNrvWatchFor, Karikari, WatchFor);
    NEW_NERVE(HostTypeNrvPreCling, Karikari, PreCling);
    NEW_NERVE(HostTypeNrvCling, Karikari, Cling);
    NEW_NERVE(HostTypeNrvRelease, Karikari, Release);
    NEW_NERVE(HostTypeNrvSpinAttacked, Karikari, SpinAttacked);
    NEW_NERVE(HostTypeNrvDPDAttacked, Karikari, DPDAttacked);
    NEW_NERVE(HostTypeNrvFrozen, Karikari, Frozen);
    NEW_NERVE(HostTypeNrvFrozenRecover, Karikari, FrozenRecover);
    NEW_NERVE(HostTypeNrvBlowOut, Karikari, BlowOut);
    NEW_NERVE(HostTypeNrvBlowOutStarPiece, Karikari, BlowOut);
    NEW_NERVE(HostTypeNrvInTornado, Karikari, InTornado);
    NEW_NERVE(HostTypeNrvPress, Karikari, Press);
}  // namespace NrvKarikari

Karikari::Karikari(const char* pName)
    : LiveActor(pName), mFront(0.0f, 0.0f, 1.0f), mClingPosition(nullptr), mIsReadyToLandTornado(false), _9D(false), _9E(true), mIsPushable(true),
      mFrozenTime(0), _A4(0) {
}

void Karikari::init(const JMapInfoIter& rIter) {
    MR::createSceneObj(SceneObj_KarikariDirector);
    MR::joinToGroup(this, "カリカリディレクター");
    MR::initDefaultPos(this, rIter);
    MR::useStageSwitchWriteDead(this, rIter);
    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
    }
    initModelManagerWithAnm("Karipon", nullptr, false);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    initSound(4, false);
    f32 size = mScale.y * sSize;
    initBinder(size, size, 0);
    initEffectKeeper(0, nullptr, false);

    initHitSensor(1);
    MR::addHitSensor(this, "body", ATYPE_KARIKARI, 32, 100.0f, TVec3f(0.0f, 30.0f, 0.0f));
    MR::initShadowVolumeSphere(this, sSize);
    MR::onCalcShadowOneTime(this, nullptr);
    initNerve(&NrvKarikari::HostTypeNrvFall::sInstance);
    MR::initStarPointerTarget(this, 40.0f, TVec3f(0.0f, 25.0f, 0.0f));

    if (MR::isValidSwitchAppear(this)) {
        makeActorDead();
    } else {
        appear();
    }

    MR::onCalcGravity(this);
    MR::declareStarPiece(this, 3);
}

void Karikari::applyEnvironmentInfluenceToVelocity() {
    if (MR::isOnGround(this)) {
        MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
        mVelocity.mult(sVelocityDampGround);
        TVec3f norm(*MR::getGroundNormal(this));
        TVec3f velH(mVelocity);
        MR::vecKillElement(velH, norm, &velH);
        if (velH.squared() < sStopVelocitySquared) {
            mVelocity.scale(norm.dot(mVelocity), norm);
        }
        return;
    }

    if (isNerve(&NrvKarikari::HostTypeNrvSpinAttacked::sInstance)) {
        mVelocity.mult(sVelocityDampSpinAttacked);
        return;
    }

    if (isNerve(&NrvKarikari::HostTypeNrvDPDAttacked::sInstance)) {
        mVelocity.mult(sVelocityDampAttacked);
        return;
    }

    mVelocity.mult(sVelocityDampAir);
}

void Karikari::killedInFrozenState() {
    MR::deleteEffect(this, "KarikariCrystalLight");
    MR::emitEffect(this, "KarikariCrystalBreak");
    MR::startSound(this, "SE_EM_KARIKARI_BLOW", -1, -1);
    MR::startSound(this, "SE_EM_KARIKARI_CRUSH_CRYSTAL_L", -1, -1);
    generateItem(3);
    makeActorDead();
    MR::stopScene(sStopSceneTime);
}

void Karikari::control() {
    if (MR::isInDeath(this, TVec3f(0.0f, 0.0f, 0.0f)) || MR::isInWater(mPosition)) {
        kill();
    }
}

void Karikari::exeFall() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
        _9E = false;
        mIsPushable = true;
        MR::onBind(this);
        MR::onCalcShadow(this, nullptr);
        MR::onCalcGravity(this);
        MR::startBtp(this, "Bite");
        MR::setBtpFrameAndStop(this, 0.0f);
    }

    applyEnvironmentInfluenceToVelocity();
    mVelocity += mGravity.scaleInline(sGravity);

    if (MR::isOnGround(this)) {
        MR::emitEffect(this, "SmokeLand");
        setNerve(&NrvKarikari::HostTypeNrvLand::sInstance);
    }
}

void Karikari::exeLand() {
    applyEnvironmentInfluenceToVelocity();
    mVelocity += mGravity.scaleInline(sGravity);

    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Land", nullptr);
        MR::startBtp(this, "Land");
        _9E = false;
        mIsPushable = true;
        MR::onBind(this);
        MR::onCalcShadow(this, nullptr);
        MR::onCalcGravity(this);
        mVelocity.zero();
    }

    if (tryDPDAttacked() == true) {
        return;
    }

    if (MR::isGreaterStep(this, sPreWaitTime)) {
        TVec3f diff(*MR::getPlayerPos());
        diff.sub(mPosition);
        if (diff.squared() < sDistToPursue) {
            setNerve(&NrvKarikari::HostTypeNrvPursue::sInstance);
            return;
        } else {
            setNerve(&NrvKarikari::HostTypeNrvWait::sInstance);
            return;
        }
    }

    if (MR::isStep(this, sBeginWaitMotionFrame)) {
        MR::startBck(this, "Wait", nullptr);
        MR::startBtp(this, "Bite");
        MR::setBtpFrameAndStop(this, 0.0f);
        return;
    }

    if (MR::isGreaterStep(this, sBeginWaitMotionFrame)) {
        TVec3f toPlayerH(MR::getPlayerPos()->subOperatorInLine(mPosition));
        MR::vecKillElement(toPlayerH, mGravity, &toPlayerH);
        MR::normalizeOrZero(&toPlayerH);
        tryTurnToDirection(toPlayerH, sTurnRatio);
    }
}

void Karikari::exeNoCalcWait() {
    if (MR::isFirstStep(this)) {
        MR::offBind(this);
        MR::offCalcShadow(this, nullptr);
        MR::offCalcGravity(this);
        _9E = false;
        mIsPushable = false;
    }

    mVelocity.zero();
    if (MR::calcDistanceToPlayer(mPosition) < 1000.0f) {
        setNerve(&NrvKarikari::HostTypeNrvWait::sInstance);
    }
}

void Karikari::exeWait() {
    applyEnvironmentInfluenceToVelocity();
    mVelocity += mGravity.scaleInline(sGravity);

    if (MR::isFirstStep(this)) {
        MR::onBind(this);
        MR::onCalcShadow(this, nullptr);
        MR::onCalcGravity(this);
        MR::validateHitSensors(this);
        MR::startBck(this, "Wait", nullptr);
        MR::startBtp(this, "Bite");
        MR::setBtpFrameAndStop(this, 0.0f);
        _9E = true;
        mIsPushable = true;
    }

    if (tryDPDAttacked() == true) {
        return;
    }

    TVec3f diff(*MR::getPlayerPos());
    diff.sub(mPosition);
    if (diff.squared() < sDistToPursue * sDistToPursue) {
        setNerve(&NrvKarikari::HostTypeNrvPrePursue::sInstance);
        return;
    }

    if (MR::isOnGround(this)) {
        setNerve(&NrvKarikari::HostTypeNrvNoCalcWait::sInstance);
    }
}

void Karikari::exePrePursue() {
    if (MR::isFirstStep(this)) {
        MR::onBind(this);
        MR::onCalcShadow(this, nullptr);
        MR::onCalcGravity(this);
        MR::validateHitSensors(this);
        MR::startSound(this, "SE_EM_KARIKARI_FIND", -1, -1);
        MR::startBck(this, "Search", nullptr);
        MR::startBtp(this, "Bite");
        MR::setBtpFrameAndStop(this, 0.0f);
        _9E = true;
        mIsPushable = false;
    }

    applyEnvironmentInfluenceToVelocity();
    mVelocity += mGravity.scaleInline(sGravity);

    if (tryDPDAttacked() == true) {
        return;
    }

    if (MR::isLessStep(this, sJumpAgainTime)) {
        TVec3f toPlayer(MR::getPlayerPos()->subOperatorInLine(mPosition));
        tryTurnToDirection(toPlayer, sTurnRatio);
        mIsPushable = true;
        return;
    }

    if (MR::isStep(this, sJumpAgainTime)) {
        mVelocity.sub(mGravity.scaleInline(sJumpToPrePursueVel));
        mIsPushable = false;
        return;
    }

    if (MR::isOnGround(this)) {
        setNerve(&NrvKarikari::HostTypeNrvPursue::sInstance);
        MR::emitEffect(this, "SmokeLand");
    }
}

void Karikari::exePursue() {
    if (MR::isFirstStep(this)) {
        MR::onBind(this);
        MR::onCalcShadow(this, nullptr);
        MR::onCalcGravity(this);
        MR::validateHitSensors(this);
        MR::startBck(this, "Jump", nullptr);
        MR::setBckRate(this, sJumpAnimRate);
        MR::startBtp(this, "Bite");
        MR::setBtpFrameAndStop(this, 0.0f);
        _9E = true;
        mIsPushable = true;
    }

    applyEnvironmentInfluenceToVelocity();
    mVelocity += mGravity.scaleInline(sGravity);

    if (tryDPDAttacked() == true) {
        return;
    }

    TVec3f toPlayerH(MR::getPlayerPos()->subOperatorInLine(mPosition));
    f32 dist = toPlayerH.squared();
    MR::vecKillElement(toPlayerH, mGravity, &toPlayerH);
    MR::normalizeOrZero(&toPlayerH);
    if (MR::isLessStep(this, sJumpAgainTime)) {
        mIsPushable = true;
        tryTurnToDirection(toPlayerH, sTurnRatio);
        return;
    }

    if (MR::isStep(this, sJumpAgainTime)) {
        MR::startSound(this, "SE_EM_KARIKARI_JUMP2", -1, -1);
        mVelocity.sub(mGravity.scaleInline(sJumpToPursueVel));
        toPlayerH.scale(sPursueVel);
        mVelocity += toPlayerH;
        return;
    }

    if (MR::isOnGround(this)) {
        MR::emitEffect(this, "SmokeLand");
        if (getKarikariDirector()->isMaxNumCling()) {
            setNerve(&NrvKarikari::HostTypeNrvWatchFor::sInstance);
            return;
        }

        if (sDistToPursue < dist) {
            setNerve(&NrvKarikari::HostTypeNrvWait::sInstance);
            return;
        }

        tryTurnToDirection(toPlayerH, sTurnRatio);
        setNerve(&NrvKarikari::HostTypeNrvPursue::sInstance);

    } else {
        mIsPushable = false;
    }
}

void Karikari::exeWatchFor() {
    if (MR::isFirstStep(this)) {
        MR::onBind(this);
        MR::onCalcShadow(this, nullptr);
        MR::onCalcGravity(this);
        MR::validateHitSensors(this);
        MR::startBck(this, "Glad", nullptr);
        MR::startBtp(this, "Bite");
        MR::setBtpFrameAndStop(this, 0.0f);
        _9E = false;
        mIsPushable = true;
    }

    applyEnvironmentInfluenceToVelocity();
    mVelocity += mGravity.scaleInline(sWatchForGravity);

    if (tryDPDAttacked() == true) {
        return;
    }

    if (!MR::isOnGround(this)) {
        return;
    }

    if (MR::isLessStep(this, sWatchForJumpTime)) {
        TVec3f toPlayer(MR::getPlayerPos()->subOperatorInLine(mPosition));
        tryTurnToDirection(toPlayer, sTurnRatio);

        if (!getKarikariDirector()->isMaxNumCling() || sWatchForDistance * sWatchForDistance < toPlayer.squared()) {
            setNerve(&NrvKarikari::HostTypeNrvPrePursue::sInstance);
        }
        return;
    }

    if (MR::isStep(this, sWatchForJumpTime)) {
        mVelocity.sub(mGravity.scaleInline(sWatchForJumpVel));
        return;
    }

    if (MR::isGreaterStep(this, sWatchForLoopInterval)) {
        setNerve(&NrvKarikari::HostTypeNrvWatchFor::sInstance);
    }
}

void Karikari::exePreCling() {
    if (MR::isFirstStep(this)) {
        MR::offBind(this);
        MR::onCalcShadow(this, nullptr);
        MR::onCalcGravity(this);
        MR::validateHitSensors(this);
        MR::startBck(this, "Wait", nullptr);
        MR::startBtp(this, "Bite");
        MR::setBtpFrameAndStop(this, 0.0f);
        _9E = false;
        mIsPushable = false;
    }

    if (tryDPDAttacked() == true) {
        return;
    }

    TVec3f toClingPos;
    f32 t = getNerveStep() / static_cast< f32 >(sPreClingTime);
    mClingPosition->copyTrans(&toClingPos);
    toClingPos.sub(mPosition);
    mVelocity = mVelocity.scaleInline(1.0f - t).translate(toClingPos.scaleInline(t));
    MR::startLevelSound(this, "SE_EM_LV_KARIKARI_CLING", -1, -1, -1);

    if (MR::isGreaterStep(this, sPreClingTime)) {
        MR::onBind(this);
        setNerve(&NrvKarikari::HostTypeNrvCling::sInstance);
    }
}

void Karikari::exeCling() {
    if (MR::isFirstStep(this)) {
        MR::offBind(this);
        MR::offCalcShadow(this, nullptr);
        MR::invalidateShadow(this, nullptr);
        MR::offCalcGravity(this);
        MR::validateHitSensors(this);
        _9E = false;
        mIsPushable = false;
        MR::setBckRate(this, 1.0f);
        MR::startBck(this, "Bite", nullptr);
        MR::startBtp(this, "Bite");
        MR::startSound(this, "SE_EM_KARIKARI_TOUCH", -1, -1);
        mVelocity.zero();
    }

    if (tryDPDAttacked() == true) {
        return;
    }

    mClingPosition->copyTrans(&mPosition);
    MR::startLevelSound(this, "SE_EM_LV_KARIKARI_CLING", -1, -1, -1);
}

void Karikari::exeRelease() {
    if (MR::isFirstStep(this)) {
        MR::startBtp(this, "Bite");
        MR::setBtpFrameAndStop(this, 0.0f);
    }

    TVec3f away(mPosition);
    away.sub(*MR::getPlayerPos());
    MR::vecKillElement(away, mGravity, &away);
    away.setLength(sAttackedVel);
    away.sub(mGravity.scaleInline(sAttackedVerticalVel));
    mVelocity.set(away);
    setNerve(&NrvKarikari::HostTypeNrvSpinAttacked::sInstance);
}

void Karikari::exeSpinAttacked() {
    if (MR::isFirstStep(this)) {
        MR::onBind(this);
        MR::onCalcShadow(this, nullptr);
        MR::onCalcGravity(this);
        MR::validateShadow(this, nullptr);
        MR::validateHitSensors(this);
        MR::startBck(this, "SpinX", nullptr);
        MR::setBckRate(this, sSpinXAnimRate);
        MR::startBtp(this, "Bite");
        MR::setBtpFrameAndStop(this, 0.0f);
        MR::startSystemME("ME_MAGIC");
        _9E = false;
        mIsPushable = true;
        _A4 = 0;
    }

    applyEnvironmentInfluenceToVelocity();
    mVelocity += mGravity.scaleInline(sGravitySpinAttacked);

    if (MR::isGreaterStep(this, sSpinAttackedTime)) {
        setNerve(&NrvKarikari::HostTypeNrvFrozen::sInstance);
    }
}

void Karikari::exeDPDAttacked() {
    if (MR::isFirstStep(this)) {
        MR::onBind(this);
        MR::onCalcShadow(this, nullptr);
        MR::onCalcGravity(this);
        MR::validateShadow(this, nullptr);
        MR::validateHitSensors(this);
        MR::startBck(this, "SpinX", nullptr);
        MR::setBckRate(this, sSpinXAnimRate);
        MR::startBtp(this, "Bite");
        MR::setBtpFrameAndStop(this, 0.0f);
        _9E = false;
        mIsPushable = true;
        _A4 = 0;
        MR::startSound(this, "SE_EM_KARIKARI_RELEASE", -1, -1);
        MR::emitEffect(this, "Touch");
    }

    applyEnvironmentInfluenceToVelocity();
    mVelocity += mGravity.scaleInline(sDPDAttackedGravity);

    if (MR::isOnGround(this)) {
        MR::emitEffect(this, "SmokeLand");
        setNerve(&NrvKarikari::HostTypeNrvLand::sInstance);
    }
}

void Karikari::exeFrozen() {
    if (MR::isFirstStep(this)) {
        MR::offBind(this);
        MR::offCalcShadow(this, nullptr);
        MR::offCalcGravity(this);
        MR::validateHitSensors(this);
        _9E = true;
        mIsPushable = false;
        MR::startBtp(this, "Bite");
        MR::setBtpFrameAndStop(this, 0.0f);
        mVelocity.zero();
        MR::startBck(this, "Wait", nullptr);
        MR::setBckFrameAndStop(this, 0.0f);
        MR::emitEffect(this, "KarikariCrystalLight");
        mFrozenTime = sFrozenLimitTime;
    }

    if (MR::isStep(this, sIceAppearTime)) {
        MR::invalidateShadow(this, nullptr);
        MR::startSound(this, "SE_EM_KARIKARI_FREEZE_CRYSTAL", -1, -1);
    }

    if (--mFrozenTime <= 0) {
        MR::onBind(this);
        setNerve(&NrvKarikari::HostTypeNrvFrozenRecover::sInstance);
    }
}

void Karikari::exeFrozenRecover() {
    if (MR::isFirstStep(this)) {
        MR::onBind(this);
        MR::onCalcShadow(this, nullptr);
        MR::onCalcGravity(this);
        MR::validateShadow(this, nullptr);
        MR::validateHitSensors(this);
        _9E = true;
        mIsPushable = true;

        mVelocity = (-mGravity).scaleInline(sJumpToFrozenRecoverVel);

        MR::deleteEffect(this, "KarikariCrystalLight");
        MR::emitEffect(this, "KarikariCrystalBreak");
        MR::startSound(this, "SE_EM_KARIKARI_CRUSH_CRYSTAL_M", -1, -1);
        MR::startBck(this, "SpinX", nullptr);
        MR::setBckRate(this, sSpinXAnimRate);
        MR::startBtp(this, "Bite");
        MR::setBtpFrameAndStop(this, 0.0f);
    }

    applyEnvironmentInfluenceToVelocity();
    mVelocity += mGravity.scaleInline(sFrozenRecoverGravity);

    if (MR::isOnGround(this)) {
        MR::emitEffect(this, "KarikariSmokeLand");
        setNerve(&NrvKarikari::HostTypeNrvLand::sInstance);
    }
}

void Karikari::exeBlowOut() {
    if (MR::isFirstStep(this)) {
        MR::onBind(this);
        MR::onCalcShadow(this, nullptr);
        MR::onCalcGravity(this);
        MR::validateShadow(this, nullptr);
        MR::validateHitSensors(this);
        _9E = false;
        mIsPushable = true;
        MR::startSound(this, "SE_EM_KARIKARI_BLOW", -1, -1);
        MR::startBck(this, "SpinX", nullptr);
        MR::setBckRate(this, sSpinXAnimRate);
        MR::startBtp(this, "Bite");
        MR::setBtpFrameAndStop(this, 0.0f);
        MR::startBlowHitSound(this);
    }

    applyEnvironmentInfluenceToVelocity();
    mVelocity += mGravity.scaleInline(sBlowOutGravity);

    if (MR::isBinded(this) || MR::isGreaterStep(this, sBlowOutTime)) {
        if (isNerve(&NrvKarikari::HostTypeNrvBlowOut::sInstance)) {
            generateItem(1);
        } else {
            generateItem(3);
        }
        kill();
    }
}

void Karikari::exeInTornado() {
    if (MR::isFirstStep(this)) {
        MR::onBind(this);
        MR::onCalcShadow(this, nullptr);
        MR::onCalcGravity(this);
        MR::validateHitSensors(this);
        MR::startBck(this, "Wait", nullptr);
        MR::setBckRate(this, 2.0f);
        MR::startBtp(this, "Bite");
        MR::setBtpFrameAndStop(this, 0.0f);
        _9E = false;
        mIsPushable = true;
    }

    applyEnvironmentInfluenceToVelocity();
    mVelocity += mGravity.scaleInline(sGravity);

    MR::startLevelSound(this, "SE_EM_LV_KARIKARI_TORNADO", -1, -1, -1);

    if (!mIsReadyToLandTornado && MR::isOnGround(this)) {
        setNerve(&NrvKarikari::HostTypeNrvWait::sInstance);
        MR::emitEffect(this, "SmokeLand");
        return;
    }

    mIsReadyToLandTornado = false;
}

void Karikari::exePress() {
    if (MR::isFirstStep(this)) {
        MR::onBind(this);
        MR::onCalcShadow(this, nullptr);
        MR::onCalcGravity(this);
        MR::invalidateHitSensors(this);
        MR::startBck(this, "Press", nullptr);
        MR::startBtp(this, "Bite");
        MR::setBtpFrameAndStop(this, 0.0f);
        MR::startSound(this, "SE_EM_KARIKARI_STOMPED", -1, -1);
    }

    applyEnvironmentInfluenceToVelocity();
    mVelocity += mGravity.scaleInline(sGravity);

    if (MR::isGreaterStep(this, sPressTime)) {
        kill();
    }
}

bool Karikari::inClingNerve() const {
    return isNerve(&NrvKarikari::HostTypeNrvPreCling::sInstance) || isNerve(&NrvKarikari::HostTypeNrvCling::sInstance);
}

bool Karikari::tryBlowOut(const TVec3f& rPos, bool spawnMultipleStarPieces) {
    if (isNerve(&NrvKarikari::HostTypeNrvBlowOut::sInstance) || isNerve(&NrvKarikari::HostTypeNrvBlowOutStarPiece::sInstance) ||
        isNerve(&NrvKarikari::HostTypeNrvRelease::sInstance) || isNerve(&NrvKarikari::HostTypeNrvSpinAttacked::sInstance) ||
        isNerve(&NrvKarikari::HostTypeNrvDPDAttacked::sInstance) || isNerve(&NrvKarikari::HostTypeNrvPress::sInstance)) {
        return false;
    }

    TVec3f toPosition(mPosition);
    toPosition.sub(rPos);
    MR::vecKillElement(toPosition, mGravity, &toPosition);
    toPosition.setLength(sBlowOutVel);
    toPosition.sub(mGravity.scaleInline(sBlowOutVerticalVel));
    mVelocity = toPosition;

    MR::emitEffect(this, "Hit");

    if (inClingNerve()) {
        getKarikariDirector()->unregistCling(this);
    }

    if (spawnMultipleStarPieces) {
        setNerve(&NrvKarikari::HostTypeNrvBlowOutStarPiece::sInstance);
    } else {
        setNerve(&NrvKarikari::HostTypeNrvBlowOut::sInstance);
    }

    return true;
}

bool Karikari::tryElectricKill() {
    if (!inClingNerve()) {
        return false;
    }

    MR::emitEffect(this, "ElectricDeath");
    makeActorDead();
    return true;
}

bool Karikari::tryHipDropRelease() {
    TVec3f away(mPosition);
    away.sub(*MR::getPlayerPos());
    MR::vecKillElement(away, mGravity, &away);
    away.setLength(sAttackedVel);
    away.sub(mGravity.scaleInline(sAttackedVerticalVel));
    mVelocity.set(away);
    setNerve(&NrvKarikari::HostTypeNrvDPDAttacked::sInstance);
    return true;
}

bool Karikari::tryDPDRelease(const TVec2f& rVel) {
    setVelocityFromCursorMove(rVel);
    setNerve(&NrvKarikari::HostTypeNrvDPDAttacked::sInstance);
    return true;
}

bool Karikari::tryDPDAttacked() {
    // FIXME: TVec2 copy operations are done via memregs, not float regs
    // https://decomp.me/scratch/W6YWF

    if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        MR::startDPDHitSound();
        s32 padChannel = *MR::getStarPointerLastPointedPort(this);
        TVec2f pointerVel(*MR::getStarPointerScreenVelocity(padChannel));
        TVec2f pointingPos;
        TVec2f screenPos;
        MR::getCorePadPointingPosBasedOnScreen(&pointingPos, padChannel);
        MR::calcScreenPosition(&screenPos, mPosition);
        TVec2f fromCursorPos = screenPos;
        fromCursorPos.sub(pointingPos);
        TVec2f vel(pointerVel);
        if (MR::isNearZero(pointerVel)) {
            vel = fromCursorPos;
            vel.setLength(sFromCursorVelScale);
        }

        if (inClingNerve()) {
            getKarikariDirector()->requestRelease(this, vel, fromCursorPos.squared());
        } else {
            setVelocityFromCursorMove(vel);
            setNerve(&NrvKarikari::HostTypeNrvDPDAttacked::sInstance);
        }
        return true;
    }

    return false;
}

void Karikari::setVelocityFromCursorMove(const TVec2f& rVel) {
    TVec3f camX = MR::getCamXdir();
    TVec3f camY = -MR::getCamYdir();
    mVelocity = camX.scaleInline(rVel.x).translate(camY.scaleInline(rVel.y));
    if (mVelocity.dot(mGravity) > 0.0f) {
        MR::vecKillElement(mVelocity, mGravity, &mVelocity);
    }
    mVelocity.setLength(sScratchStrongVel);
    mVelocity.sub(mGravity.scaleInline(40.0f));
}

void Karikari::makeActorDead() {
    LiveActor::makeActorDead();
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }

    if (isNerve(&NrvKarikari::HostTypeNrvPreCling::sInstance) || isNerve(&NrvKarikari::HostTypeNrvCling::sInstance)) {
        getKarikariDirector()->unregistCling(this);
    }
}

void Karikari::kill() {
    MR::emitEffect(this, "Death");
    MR::startSound(this, "SE_EM_EXPLODE_S", -1, -1);
    LiveActor::kill();
}

void Karikari::generateItem(s32 numStarPieces) {
    // FIXME: TVec and register order
    // https://decomp.me/scratch/hBIP5

    TVec3f pos = (-mGravity).scaleInline(80.0f);
    pos += mPosition;
    MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
    MR::appearStarPiece(this, pos, numStarPieces, 10.0f, 40.0f, false);
}

void Karikari::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvKarikari::HostTypeNrvCling::sInstance) || isNerve(&NrvKarikari::HostTypeNrvPreCling::sInstance) ||
        isNerve(&NrvKarikari::HostTypeNrvBlowOut::sInstance) || isNerve(&NrvKarikari::HostTypeNrvBlowOutStarPiece::sInstance) ||
        isNerve(&NrvKarikari::HostTypeNrvSpinAttacked::sInstance)) {
        return;
    }

    if (!pSender->isType(ATYPE_KARIKARI)) {
        return;
    }

    if (mIsPushable && MR::isSensorEnemy(pReceiver)) {
        MR::sendMsgPush(pReceiver, pSender);
        return;
    }

    if (_9E && MR::isSensorPlayer(pReceiver)) {
        if (isNerve(&NrvKarikari::HostTypeNrvFrozen::sInstance)) {
            killedInFrozenState();
            return;
        }

        TVec3f diff = pSender->mPosition.subOperatorInLine(pReceiver->mPosition);
        f32 dist = diff.squared();
        f32 clingDist = sDistToCling;
        if (dist < clingDist * clingDist) {
            if (getKarikariDirector()->registCling(this)) {
                setNerve(&NrvKarikari::HostTypeNrvPreCling::sInstance);
                mVelocity.zero();
            }
        }
    }
}

bool Karikari::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgHitmarkEmit(msg)) {
        return true;
    }

    if (MR::isMsgInhaleBlackHole(msg)) {
        kill();
        return true;
    }

    return false;
}

bool Karikari::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        return tryBlowOut(pSender->mPosition, true);
    }

    if (inClingNerve()) {
        return false;
    }

    if (MR::isMsgPlayerHitAll(msg)) {
        return tryBlowOut(pSender->mPosition, false);
    }

    return false;
}

bool Karikari::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (pReceiver->mType != ATYPE_KARIKARI) {
        return false;
    }

    if (isNerve(&NrvKarikari::HostTypeNrvCling::sInstance) || isNerve(&NrvKarikari::HostTypeNrvPreCling::sInstance) ||
        isNerve(&NrvKarikari::HostTypeNrvBlowOut::sInstance) || isNerve(&NrvKarikari::HostTypeNrvBlowOutStarPiece::sInstance) ||
        isNerve(&NrvKarikari::HostTypeNrvFrozen::sInstance)) {
        return false;
    }

    if (MR::isSensorEnemy(pSender) || MR::isSensorMapObj(pSender)) {
        TVec3f diff(mPosition);
        diff.sub(pSender->mHost->mPosition);
        MR::normalizeOrZero(&diff);
        mVelocity += diff;
        return true;
    }

    return false;
}

bool Karikari::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvKarikari::HostTypeNrvRelease::sInstance) || isNerve(&NrvKarikari::HostTypeNrvSpinAttacked::sInstance) ||
        isNerve(&NrvKarikari::HostTypeNrvBlowOut::sInstance) || isNerve(&NrvKarikari::HostTypeNrvBlowOutStarPiece::sInstance) ||
        isNerve(&NrvKarikari::HostTypeNrvPress::sInstance)) {
        return false;
    }

    if (isNerve(&NrvKarikari::HostTypeNrvFrozen::sInstance)) {
        killedInFrozenState();
        return true;
    }

    if (MR::isMsgToEnemyAttackBlow(msg)) {
        if (isNerve(&NrvKarikari::HostTypeNrvPreCling::sInstance) || isNerve(&NrvKarikari::HostTypeNrvCling::sInstance)) {
            getKarikariDirector()->unregistCling(this);
        }
        // blow away from enemy
        TVec3f vel(pReceiver->mPosition);
        vel.sub(pSender->mPosition);
        MR::normalizeOrZero(&vel);
        vel.scale(sEnemyAttackHitVel);
        mVelocity.set(vel);
        setNerve(&NrvKarikari::HostTypeNrvBlowOut::sInstance);
        return true;
    }

    if (MR::isMsgToEnemyAttackTrample(msg)) {
        if (isNerve(&NrvKarikari::HostTypeNrvPreCling::sInstance) || isNerve(&NrvKarikari::HostTypeNrvCling::sInstance)) {
            getKarikariDirector()->unregistCling(this);
        }

        setNerve(&NrvKarikari::HostTypeNrvPress::sInstance);
        return true;
    }

    if (MR::isMsgExplosionAttack(msg)) {
        if (isNerve(&NrvKarikari::HostTypeNrvFrozen::sInstance)) {
            killedInFrozenState();
            return true;
        }

        if (isNerve(&NrvKarikari::HostTypeNrvPreCling::sInstance) || isNerve(&NrvKarikari::HostTypeNrvCling::sInstance)) {
            getKarikariDirector()->unregistCling(this);
        }
        // blow away from explosion
        TVec3f vel(pReceiver->mPosition);
        vel.sub(pSender->mPosition);
        vel.setLength(sExplosionBlowOutVel);
        mVelocity = vel;
        setNerve(&NrvKarikari::HostTypeNrvBlowOut::sInstance);
        return true;
    }

    return false;
}

void Karikari::tryTurnToDirection(const TVec3f& rDir, f32 turnRatio) {
    TQuat4f rot;
    rot.setRotate(mFront, rDir, turnRatio);
    rot.transform(mFront);
}

void Karikari::calcAndSetBaseMtx() {
    if (isNerve(&NrvKarikari::HostTypeNrvCling::sInstance)) {
        MR::setBaseTRMtx(this, mClingPosition->mMtx);
        return;
    }

    TPos3f mtx;
    MR::calcMtxFromGravityAndZAxis(&mtx, this, mGravity, mFront);
    MR::setBaseTRMtx(this, mtx);
}
