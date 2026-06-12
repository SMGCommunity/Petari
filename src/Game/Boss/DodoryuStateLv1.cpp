#include "Game/Boss/DodoryuStateLv1.hpp"
#include "Game/Boss/Dodoryu.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static const s32 sStopSceneFrame = 10;
    static const f32 sKnockSpeed = 10.0f;
    static const f32 sSpeed = 8.0f;
    static const f32 sAppearSpeed = 4.0f;
    static const f32 sRailAccel = 0.1f;
    static const s32 sHideTermMin = 150;
    static const s32 sHideTermMax = 200;
    static const s32 sAppearStartAttackFrame = 16;
    static const s32 sAppearTermMin = 15;
    static const s32 sAppearTermMax = 30;
    static const s32 sSwoonTerm = 180;
    static const s32 sDiveShadowOffFrame = 50;
    static const s32 sSpinOutEndFrame = 80;
    static const s32 sAppearSignFrame = 30;
    static const s32 sDiveAttackInvalidFrame = 20;
    // static const f32 sPopSpeed = _;
};  // namespace

namespace {
    NEW_NERVE_ONEND(DodoryuStateLv1NrvHide, DodoryuStateLv1, Hide, Hide);
    NEW_NERVE(DodoryuStateLv1NrvAppearSign, DodoryuStateLv1, AppearSign);
    NEW_NERVE_ONEND(DodoryuStateLv1NrvAppearStart, DodoryuStateLv1, AppearStart, AppearStart);
    NEW_NERVE_ONEND(DodoryuStateLv1NrvAppear, DodoryuStateLv1, Appear, Appear);
    NEW_NERVE(DodoryuStateLv1NrvAppearEnd, DodoryuStateLv1, AppearEnd);
    NEW_NERVE(DodoryuStateLv1NrvJumpOut, DodoryuStateLv1, JumpOut);
    NEW_NERVE(DodoryuStateLv1NrvDamage, DodoryuStateLv1, Damage);
    NEW_NERVE(DodoryuStateLv1NrvDive, DodoryuStateLv1, Dive);
    NEW_NERVE(DodoryuStateLv1NrvSpinOut, DodoryuStateLv1, SpinOut);
};  // namespace

DodoryuStateLv1::DodoryuStateLv1(Dodoryu* pHost, const char* pName)
    : DodoryuStateBase(pHost, pName), mHideStep(), mAppearStep(), _A4(), mIsValidPop(), mPopAccel(), mPopSpeed(), mRailSpeed(::sSpeed),
      mRailSpeedMax(::sSpeed) {
    initWithoutIter();
}

void DodoryuStateLv1::init(const JMapInfoIter& rIter) {
    MR::invalidateClipping(this);
    initNerve(&::DodoryuStateLv1NrvHide::sInstance);
    makeActorAppeared();
}

void DodoryuStateLv1::control() {
    if (_A4 && !MR::isPlayerStaggering()) {
        _A4 = false;
    }

    if (mRailSpeedMax > mRailSpeed) {
        mRailSpeed += ::sRailAccel;

        if (mRailSpeed > mRailSpeedMax) {
            mRailSpeed = mRailSpeedMax;
        }
    }

    mHost->setMoveRailSpeed(mRailSpeed);
}

void DodoryuStateLv1::exeHide() {
    if (MR::isFirstStep(this)) {
        MR::invalidateShadowAll(mHost);
        mHost->startHill();
        setRailSpeed(::sSpeed);
        mHost->shiftMoveStateRail(mRailSpeed);

        mHideStep = MR::getRandom(::sHideTermMin, ::sHideTermMax);

        MR::shakeCameraInfinity(this, 0.2f, 2.0f);
        MR::startBva(mHost, "Normal");
    }

    if (MR::isBckOneTimeAndStopped(mHost)) {
        startAnimEffectWalk();
        MR::startBva(mHost, "Normal");
    }

    mHost->tryRumblePad();
    MR::startLevelSound(mHost, "SE_BM_LV_DODORYU_MOVE_U_SHALLOW");

    if (MR::isGreaterEqualStep(this, mHideStep)) {
        setNerve(&::DodoryuStateLv1NrvAppearSign::sInstance);
    }
}

void DodoryuStateLv1::endHide() {
    MR::stopShakingCamera(this);
}

void DodoryuStateLv1::exeAppearSign() {
    if (MR::isFirstStep(this)) {
        startAnimEffectWalkHide();
        mHost->stopHill();
    }

    if (MR::isGreaterEqualStep(this, ::sAppearSignFrame)) {
        setNerve(&::DodoryuStateLv1NrvAppearStart::sInstance);
    }
}

void DodoryuStateLv1::exeAppearStart() {
    if (MR::isFirstStep(this)) {
        startAnimAppear();
        MR::shakeCameraNormal();
        setRailSpeed(::sAppearSpeed);
        mHost->shiftMoveStateRail(mRailSpeed);
        mHost->startHill();
        mHost->validateStarPieceSensor();
    }

    MR::startLevelSound(mHost, "SE_BM_LV_DODORYU_MOVE_U_SHALLOW");

    if (MR::isBckStopped(mHost)) {
        setNerve(&::DodoryuStateLv1NrvAppear::sInstance);
    }
}

void DodoryuStateLv1::endAppearStart() {
    mHost->invalidateStarPieceSensor();
}

void DodoryuStateLv1::exeAppear() {
    if (MR::isFirstStep(this)) {
        startAnimWalk();

        mAppearStep = MR::getRandom(::sAppearTermMin, ::sAppearTermMax);

        mHost->validateStarPieceSensor();
    }

    MR::startLevelSound(mHost, "SE_BM_LV_DODORYU_MOVE_GROUND");

    if (MR::isGreaterEqualStep(this, mAppearStep)) {
        setNerve(&::DodoryuStateLv1NrvAppearEnd::sInstance);
    }
}

void DodoryuStateLv1::endAppear() {
    mHost->invalidateStarPieceSensor();
}

void DodoryuStateLv1::exeAppearEnd() {
    if (MR::isFirstStep(this)) {
        startAnimHide();
        MR::startBva(mHost, "Normal");
    }

    if (MR::isBckStopped(mHost)) {
        startAnimEffectWalkAppear();
        setNerve(&::DodoryuStateLv1NrvHide::sInstance);
    }
}

void DodoryuStateLv1::exeJumpOut() {
    if (MR::isFirstStep(this)) {
        MR::startBck(mHost, "SwoonStart", nullptr);
        MR::startSound(mHost, "SE_BM_DODORYU_JUMP_OUT");
        MR::startSound(mHost, "SE_BV_DODORYU_HIPDRP_DAMAGE");
        MR::validateShadowAll(mHost);
        mHost->stopHillForce();
        mHost->killLeadHill();
        mHost->shiftMoveStateNull();
        MR::startBva(mHost, "Cry");
    }

    if (MR::isBckStopped(mHost)) {
        mPopAccel = 0.0f;
        mPopSpeed = 0.0f;

        setNerve(&::DodoryuStateLv1NrvDamage::sInstance);
    }
}

void DodoryuStateLv1::exeDamage() {
    if (MR::isFirstStep(this)) {
        MR::startBck(mHost, "Swoon", nullptr);
        mHost->stopHillForce();
    }

    updatePop();
    MR::startLevelSound(mHost, "SE_BM_LV_DODORYU_SWOON");
    MR::startLevelSound(mHost, "SE_BV_LV_DODORYU_SWOON");

    if (MR::isGreaterEqualStep(this, ::sSwoonTerm)) {
        setNerve(&::DodoryuStateLv1NrvDive::sInstance);
    }
}

void DodoryuStateLv1::exeDive() {
    if (MR::isFirstStep(this)) {
        MR::startBck(mHost, "SwoonEnd", nullptr);
        MR::startSound(mHost, "SE_BV_DODORYU_WAKEUP");
        MR::startBva(mHost, "Normal");
    }

    if (MR::isStep(this, ::sDiveShadowOffFrame)) {
        MR::invalidateShadowAll(mHost);
    }

    updatePop();

    if (MR::isBckStopped(mHost)) {
        startAnimEffectWalkAppear();
        setNerve(&::DodoryuStateLv1NrvHide::sInstance);
    }
}

void DodoryuStateLv1::exeSpinOut() {
    if (MR::isFirstStep(this)) {
        MR::startBck(mHost, "PunchDamage", nullptr);
        MR::startBlowHitSound(mHost);
        mHost->stopHillForce();
        MR::startBva(mHost, "Cry");
        mHost->startSpinOutCamera();
        MR::startSound(mHost, "SE_BV_DODORYU_SPIN_DMG_1");
        mHost->invalidateEyeSensor();
    }

    TPos3f mtx = mHost->mBaseMtx;
    TVec3f trans;
    mtx.getTrans(trans);
    trans.add(_98 * ::sKnockSpeed);
    mtx.setTrans(trans);

    mHost->setMtx(mtx);
    mHost->keepOffFromClosedArea(nullptr);
    mHost->snapToGround();
    calcVerticalizedDir(&_98, _98);

    if (MR::isStep(this, 60)) {
        mHost->validateEyeSensor();
    }

    if (MR::isGreaterEqualStep(this, ::sSpinOutEndFrame)) {
        mHost->endSpinOutCamera();
        mHost->nextState();
        mHost->validateEyeSensor();
    }
}

void DodoryuStateLv1::start() {
    mHost->shiftMoveStateRail(10.0f);
    mHost->endPlayerCenterCamera();
    setNerve(&::DodoryuStateLv1NrvHide::sInstance);
    mHost->invalidateStarPieceSensor();
}

void DodoryuStateLv1::catchAttackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorPlayerOrRide(pReceiver)) {
        return;
    }

    if (mHost->isHeadNeedle(pSender, pReceiver)) {
        if (!MR::sendMsgEnemyAttack(pReceiver, pSender)) {
            MR::sendMsgPush(pReceiver, pSender);
        }

        return;
    }

    if (!isAttackable() || _A4) {
        MR::sendMsgPush(pReceiver, pSender);
        return;
    }

    TVec3f dir;
    calcAttackDir(&dir, pSender->mPosition, pReceiver->mPosition);

    if (MR::sendMsgEnemyAttackStrongToDir(pReceiver, pSender, dir)) {
        MR::shakeCameraNormal();
        return;
    }

    MR::sendMsgPush(pReceiver, pSender);
}

void DodoryuStateLv1::catchHipDrop() {
    setRailSpeed(0.0f);

    bool isNerves = isNerve(&::DodoryuStateLv1NrvHide::sInstance) || isNerve(&::DodoryuStateLv1NrvAppearSign::sInstance);

    if (isNerves) {
        executeHideJump();
        setNerve(&::DodoryuStateLv1NrvHide::sInstance);
        return;
    }

    if (isHipDropReactionJumpOut()) {
        mHost->reactJumpOutCommon();
        setNerve(&::DodoryuStateLv1NrvJumpOut::sInstance);
        return;
    }

    if (isHipDropReactionPop()) {
        startPop();
        return;
    }

    if (isHipDropReactionNothing()) {
        return;
    }
}

bool DodoryuStateLv1::catchPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    bool isAppear = isNerve(&::DodoryuStateLv1NrvAppearStart::sInstance) || isNerve(&::DodoryuStateLv1NrvAppear::sInstance) ||
                    isNerve(&::DodoryuStateLv1NrvAppearEnd::sInstance);

    if (isAppear && MR::isMsgPlayerSpinAttack(msg)) {
        if (MR::sendMsgEnemyAttackCounterSpin(pSender, pReceiver)) {
            _A4 = true;

            return true;
        }

        return false;
    }

    bool isNerves = isNerve(&::DodoryuStateLv1NrvJumpOut::sInstance) || isNerve(&::DodoryuStateLv1NrvDamage::sInstance);

    if (!isNerves) {
        return false;
    }

    if (mHost->isHeadNeedle(pReceiver, pSender)) {
        return false;
    }

    if (MR::isMsgPlayerTrample(msg) || MR::isMsgPlayerHipDrop(msg)) {
        return true;
    }

    if (MR::isMsgPlayerHitAll(msg)) {
        _98.set(pReceiver->mPosition - pSender->mPosition);

        if (!calcVerticalizedDir(&_98, _98)) {
            mHost->mBaseMtx.getZDirInline(_98);
            _98.negateInternal();
        }

        MR::tryRumblePadMiddle(this, 0);
        MR::stopScene(::sStopSceneFrame);
        setNerve(&::DodoryuStateLv1NrvSpinOut::sInstance);

        return true;
    }

    return false;
}

void DodoryuStateLv1::catchStartClipped() {
}

bool DodoryuStateLv1::isAttackable() const {
    return isNerve(&::DodoryuStateLv1NrvHide::sInstance) ||
           (isNerve(&::DodoryuStateLv1NrvAppearStart::sInstance) && MR::isGreaterStep(this, ::sAppearStartAttackFrame)) ||
           isNerve(&::DodoryuStateLv1NrvAppear::sInstance) || isNerve(&::DodoryuStateLv1NrvAppearEnd::sInstance) ||
           (isNerve(&::DodoryuStateLv1NrvDive::sInstance) && MR::isGreaterStep(this, ::sDiveAttackInvalidFrame));
}

void DodoryuStateLv1::executeHideJump() {
    mHost->appearBank();
    MR::startBck(mHost, "Jump", nullptr);
    mHost->startLeadHillBck("Jump");
    MR::startBva(mHost, "Cry");
    MR::startSound(mHost, "SE_BM_DODORYU_HIPDRP_REACTION");
}

void DodoryuStateLv1::startPop() {
    mIsValidPop = true;
    mPopAccel = 30.0f;
    mPopSpeed = 0.0f;
}

void DodoryuStateLv1::updatePop() {
    if (!mIsValidPop) {
        return;
    }

    TPos3f mtx = mHost->mBaseMtx;
    TVec3f trans;
    mtx.getTrans(trans);
    trans.add(mHost->mGravity.negateOperatorInternal() * mPopAccel);

    mPopSpeed += mPopAccel;
    mPopAccel -= 2.5f;

    if (mPopSpeed < 0.0f) {
        trans.add(mHost->mGravity * mPopSpeed);

        MR::shakeCameraNormal();

        mIsValidPop = false;

        MR::emitEffect(mHost, "SmallLandSmoke");
    }

    mtx.setTrans(trans);
    mHost->setMtx(mtx);
}

bool DodoryuStateLv1::isHipDropReactionJumpOut() const {
    f32 dotP = MR::getPlayerGravity()->dot(mHost->mGravity);

    if (dotP < 0.0f) {
        return false;
    }

    return isNerve(&::DodoryuStateLv1NrvAppearStart::sInstance) || isNerve(&::DodoryuStateLv1NrvAppear::sInstance) ||
           isNerve(&::DodoryuStateLv1NrvAppearEnd::sInstance);
}

bool DodoryuStateLv1::isHipDropReactionPop() const {
    f32 dotP = MR::getPlayerGravity()->dot(mHost->mGravity);

    if (dotP < 0.0f) {
        return false;
    }

    return isNerve(&::DodoryuStateLv1NrvDamage::sInstance);
}

bool DodoryuStateLv1::isHipDropReactionNothing() const {
    f32 dotP = MR::getPlayerGravity()->dot(mHost->mGravity);

    if (dotP < 0.0f) {
        return true;
    }

    return isNerve(&::DodoryuStateLv1NrvJumpOut::sInstance) || isNerve(&::DodoryuStateLv1NrvDamage::sInstance) ||
           isNerve(&::DodoryuStateLv1NrvDive::sInstance) || isNerve(&::DodoryuStateLv1NrvSpinOut::sInstance);
}

void DodoryuStateLv1::setRailSpeed(f32 railSpeed) {
    mRailSpeedMax = railSpeed;

    if (mRailSpeed > railSpeed) {
        mRailSpeed = railSpeed;
    }
}

void DodoryuStateLv1::startAnimAppear() {
    MR::startBck(mHost, "OpeningAppear", nullptr);
    MR::startBtk(mHost, "OpeningAppear");
    mHost->startLeadHillBck("Appear");
    MR::startSound(mHost, "SE_BM_DODORYU_APPEAR_HEAD");
    MR::startSound(mHost, "SE_BV_DODORYU_APPEAR_1");
}

void DodoryuStateLv1::startAnimWalk() {
    MR::startBck(mHost, "OpeningWalk", nullptr);
    MR::startBtk(mHost, "OpeningWalk");
    mHost->startLeadHillBck("Walk");
}

void DodoryuStateLv1::startAnimHide() {
    MR::startBck(mHost, "Hide", nullptr);
    mHost->startLeadHillBck("Hide");
}

void DodoryuStateLv1::startAnimEffectWalkAppear() {
    MR::startBck(mHost, "EffectWalkAppear", nullptr);
    mHost->startLeadHillBck("EffectWalkAppear");
}

void DodoryuStateLv1::startAnimEffectWalk() {
    MR::startBck(mHost, "EffectWalk", nullptr);
    mHost->startLeadHillBck("EffectWalk");
}

void DodoryuStateLv1::startAnimEffectWalkHide() {
    MR::startBck(mHost, "EffectWalkHide", nullptr);
    mHost->startLeadHillBck("EffectWalkHide");
}
