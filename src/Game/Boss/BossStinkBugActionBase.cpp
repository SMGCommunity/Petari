#include "Game/Boss/BossStinkBugActionBase.hpp"
#include "Game/Boss/BossStinkBug.hpp"
#include "Game/Boss/BossStinkBugFunction.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/SpringValue.hpp"
#include "Game/Util/StringUtil.hpp"
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/mtx.h>
#include <revolution/types.h>

void BossStinkBugActionBase_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)3.0f;
    (void)15.0f;
    (void)0.02f;
    (void)0.95f;
    (void)0.75f;
    (void)-15.0f;
    (void)35.0f;
    (void)4e+01f;
    (void)0.3f;
    (void)1.1e+03f;
    (void)1.2f;
    (void)0.99f;
    (void)1.8e+02f;
    (void)0.9f;
    (void)0.6666667f;
    (void)1.5e+03f;
    (void)12.0f;
    (void)2.5e+03f;
    (void)1e+03f;
    (void)5.5e+02f;
    (void)2e+03f;
    (void)4.5e+03f;
    (void)126.0f;
    (void)2e+01f;
    (void)0.0f;
}

void BossStinkBugActionBase_FORCE_MATCH_STRINGS() {
    MR::isEqualString("GroundWalk", "GroundWalk");
    MR::isEqualString("SE_BV_BOSS_BUG_ATTACK_G", "SE_BV_BOSS_BUG_ATTACK_G");
    MR::isEqualString("Find", "Find");
    MR::isEqualString("GroundRide", "GroundRide");
    MR::isEqualString("GroundRun", "GroundRun");
    MR::isEqualString("SE_BM_LV_BOSS_BUG_RUN_ADD_WATER", "SE_BM_LV_BOSS_BUG_RUN_ADD_WATER");
    MR::isEqualString("AttackSuccess", "AttackSuccess");
    MR::isEqualString("SE_BV_BOSS_BUG_GLAD", "SE_BV_BOSS_BUG_GLAD");
    MR::isEqualString("CoolDown", "CoolDown");
    MR::isEqualString("SE_BM_LV_BOSS_BUG_BRAKE", "SE_BM_LV_BOSS_BUG_BRAKE");
    MR::isEqualString("GroundTurnLeft", "GroundTurnLeft");
    MR::isEqualString("GroundTurnRight", "GroundTurnRight");
    MR::isEqualString("SE_BM_BOSS_BUG_PRESSED", "SE_BM_BOSS_BUG_PRESSED");
    MR::isEqualString("GroundDamage", "GroundDamage");
    MR::isEqualString("Fall", "Fall");
    MR::isEqualString("SE_BV_BOSS_BUG_DAMAGE", "SE_BV_BOSS_BUG_DAMAGE");
    MR::isEqualString("FlyWait", "FlyWait");
    MR::isEqualString("FlyRide", "FlyRide");
    MR::isEqualString("FlyRollRight", "FlyRollRight");
    MR::isEqualString("FlyRollWaitRight", "FlyRollWaitRight");
    MR::isEqualString("FlyRollEndRight", "FlyRollEndRight");
}

BossStinkBugActionBase::BossStinkBugActionBase(const char* pName, BossStinkBug* pStinkBug)
    : ActorStateBase< BossStinkBug >(pName, pStinkBug), mSpringValue(), _14(0.0f, 0.0f, 0.0f), _20(0.0f, 0.0f, 1.0f), _38(15.0f), _3C(), _40(), _44(),
      mThrowBombTimer(), _50(true), _54(), _58(), _5C(), _60(), _61() {
    mSpringValue = new SpringValue(_3C, _3C, 0.02f, 0.95f, 0.0f);
}

bool BossStinkBugActionBase::updateGroundWalk(f32 f1) {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(getHost(), "GroundWalk");
    }

    BossStinkBugFunction::moveRail(getHost(), f1, 0.0f, 0.0f);
    _48 = f1;

    MR::calcDistanceToPlayer(getHost());
    return false;
}

bool BossStinkBugActionBase::updateFlyRoll() {
    if (MR::isFirstStep(this)) {
        MR::startBck(getHost(), "FlyRollRight");
    }

    startSoundFly(0);
    _48 = _38;

    BossStinkBugFunction::moveRail(getHost(), _48, _3C, _44);

    if (MR::isBckStopped(getHost())) {
        return true;
    } else {
        return false;
    }
}

bool BossStinkBugActionBase::updateFlyRollWait(s32 waitSteps) {
    if (MR::isFirstStep(this)) {
        MR::startBck(getHost(), "FlyRollWaitRight");
    }

    startSoundFly(0);
    _48 = _38;

    BossStinkBugFunction::moveRail(getHost(), _48, _3C, _44);

    if (MR::isGreaterStep(this, waitSteps)) {
        return true;
    } else {
        return false;
    }
}

bool BossStinkBugActionBase::updateFlyRollEnd() {
    if (MR::isFirstStep(this)) {
        MR::startBck(getHost(), "FlyRollEndRight");
    }

    startSoundFly(0);
    _48 = _38;

    BossStinkBugFunction::moveRail(getHost(), _48, _3C, _44);

    if (MR::isBckStopped(getHost())) {
        return true;
    } else {
        return false;
    }
}

bool BossStinkBugActionBase::updateFlyDash(f32 f1) {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(getHost(), "FlyWait");
        BossStinkBugFunction::calcDiffCurrentRailPosition(&_14, getHost(), _3C, _44);
    }

    startSoundFly(0);

    if (MR::isLessStep(this, 30)) {
        _48 = MR::calcNerveEaseInOutValue(this, 30, f1, 40.0f);
    } else {
        _48 = MR::calcNerveEaseInOutValue(this, 30, 100, 40.0f, _38);
    }

    BossStinkBugFunction::regainToRail(getHost(), _14, MR::calcNerveRate(this, 100), _48, _3C, _44);

    if (MR::isGreaterStep(this, 100)) {
        return true;
    } else {
        return false;
    }
}

void BossStinkBugActionBase::throwBomb(s32 newWaitTime) {
    if (mThrowBombTimer == 0) {
        BossStinkBugFunction::throwBomb(getHost());
        mThrowBombTimer = newWaitTime;
    } else {
        mThrowBombTimer--;
    }
}

void BossStinkBugActionBase::resetThrowBombTiming() {
    mThrowBombTimer = 0;
}

f32 BossStinkBugActionBase::calcShakeOffSignSpeed(s32 a1) const {
    if (MR::isLessStep(this, 10)) {
        return MR::calcNerveEaseInOutValue(this, a1, _38, 3.0f);
    } else {
        return 3.0f;
    }
}

f32 BossStinkBugActionBase::calcShakeOffSpeed() const {
    return MR::calcNerveEaseInOutValue(this, MR::getBckFrameMax(getHost(), "FlyRollRight"), 3.0f, _38);
}

f32 BossStinkBugActionBase::calcShakeOffEndSpeed() const {
    return MR::calcNerveEaseInOutValue(this, MR::getBckFrameMax(getHost(), "FlyRollEndRight"), 35.0f, _38);
}

s32 BossStinkBugActionBase::calcKeepRollTime(s32 a1) const {
    f32 flt1 = (MR::getRailTotalLength(getHost()) * (2.0f / 3.0f)) / _38;

    f32 flt2 = 0.0f;
    flt2 += MR::getBckFrameMax(getHost(), "FlyRollRight");
    flt2 += MR::getBckFrameMax(getHost(), "FlyRollEndRight");
    flt2 += a1;

    s32 keepRollTime = (flt1 - flt2) / 2;

    if (keepRollTime < 0) {
        keepRollTime = 0;
    }

    return keepRollTime;
}

s32 BossStinkBugActionBase::calcFlySoundVol() const {
    s32 vol = 550;
    if (_5C > 0) {
        if (_54 < 1500.0f) {
            vol = 1000;
            if (_5C < 12) {
                vol = MR::getLinerValue(_5C, 0.0f, 1.0f, 12.0f);
                vol = 450 * vol + 550;
            }
        } else if (_54 < 2500) {
            vol = MR::getLinerValueFromMinMax(_54, 1500.0f, 2500.0f, 1000.0f, 550.0f);
            if (_5C < 12) {
                vol = MR::getLinerValue(_5C, 0.0f, 1.0f, 12.0f);
                vol = 450 * vol + 550;
            }
        } else {
            vol = 550;
        }
    }

    return vol;
}

s32 BossStinkBugActionBase::calcIIRCutOff() const {
    if (_54 < 2000.0f) {
        return 126;
    } else if (_54 < 4500.0f) {
        return MR::getLinerValueFromMinMax(_54, 2000.0f, 4500.0f, 126.0f, 20.0f);
    }

    return 20;
}

void BossStinkBugActionBase::updateSound() {
    if (getHost()->isLegInWater()) {
        if (isAngry()) {
            MR::setSeVersion(getHost(), BossStinkBug::SeVersion_ANGRY_WATER);
        } else {
            MR::setSeVersion(getHost(), BossStinkBug::SeVersion_WATER);
        }
    } else {
        if (isAngry()) {
            MR::setSeVersion(getHost(), BossStinkBug::SeVersion_ANGRY);
        } else {
            MR::setSeVersion(getHost(), BossStinkBug::SeVersion_NORMAL);
        }
    }
}

void BossStinkBugActionBase::updateSoundFly() {
    _61 = _60;

    if (MR::isNearPlayer(getHost(), 2000.0f)) {
        _60 = true;
    } else {
        _60 = false;
    }

    if (_5C > 0) {
        _5C--;
    }

    _58 = _54;
    _54 = MR::getCamPos().distance(getHost()->mPosition);

    if (_58 - _54 > 0.0f) {
        _5C = 12;
    }
}

BossStinkBugActionBase::~BossStinkBugActionBase() {
}

bool BossStinkBugActionBase::updateGroundRegainRail(f32 f1) {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(getHost(), "GroundWalk");
        BossStinkBugFunction::calcDiffCurrentRailPosition(&_14, getHost(), _3C, _44);
    }

    f32 nerveRate = MR::calcNerveRate(this, 90);
    BossStinkBugFunction::regainToRail(getHost(), _14, nerveRate, f1, 0.0f, 0.0f);
    _48 = f1;

    if (MR::isGreaterStep(this, 90)) {
        return true;
    } else {
        return false;
    }
}

bool BossStinkBugActionBase::updateGroundFind() {
    if (MR::isFirstStep(this)) {
        MR::startSound(getHost(), "SE_BV_BOSS_BUG_ATTACK_G");
        MR::startBck(getHost(), "Find");
        _48 = 0.0f;
    }

    if (MR::isBckStopped(getHost())) {
        return true;
    } else {
        return false;
    }
}

bool BossStinkBugActionBase::updateGroundShakeOff() {
    if (MR::isFirstStep(this)) {
        MR::startBck(getHost(), "GroundRide");
    }

    _48 *= 0.95f;
    BossStinkBugFunction::moveRail(getHost(), _48, 0.0f, 0.0f);

    if (MR::isGreaterStep(this, 90)) {
        return true;
    } else {
        return false;
    }
}

bool BossStinkBugActionBase::updateGroundRun(f32 f1) {
    if (MR::isFirstStep(this)) {
        MR::startBck(getHost(), "GroundRun");
        BossStinkBugFunction::validateAttack(getHost());
    }

    if (getHost()->isLegInWater()) {
        MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_RUN_ADD_WATER");
    }

    _48 = f1 * MR::calcNerveRate(this, 60);
    BossStinkBugFunction::moveRail(getHost(), _48, 0.0f, 0.0f);

    if (MR::isGreaterStep(this, 180)) {
        return true;
    } else {
        return false;
    }
}

bool BossStinkBugActionBase::updateGroundAttackSuccess() {
    if (MR::isFirstStep(this)) {
        MR::startBck(getHost(), "AttackSuccess");
    }

    if (MR::isStep(this, 12)) {
        MR::startSound(getHost(), "SE_BV_BOSS_BUG_GLAD");
    }

    _48 *= 0.75f;
    BossStinkBugFunction::moveRail(getHost(), _48, 0.0f, 0.0f);

    if (MR::isGreaterStep(this, 180)) {
        return true;
    } else {
        return false;
    }
}

bool BossStinkBugActionBase::updateGroundCoolDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(getHost(), "CoolDown");
    }

    MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_BRAKE");

    if (getHost()->isLegInWater()) {
        MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_RUN_ADD_WATER");
    }

    _48 *= 0.95f;
    BossStinkBugFunction::moveRail(getHost(), _48, 0.0f, 0.0f);

    if (MR::isBckStopped(getHost())) {
        return true;
    } else {
        return false;
    }
}

bool BossStinkBugActionBase::updateGroundTurn() {
    if (MR::isFirstStep(this)) {
        _20 = getHost()->_EC;

        _50 = MR::isRailGoingToEnd(getHost());

        if (_50) {
            MR::startBck(getHost(), "GroundTurnLeft");
        } else {
            MR::startBck(getHost(), "GroundTurnRight");
        }

        _48 = 0.0f;
    }

    BossStinkBugFunction::turnRailGround(getHost(), _20, MR::calcNerveRate(this, 120), _50);

    if (MR::isGreaterStep(this, 160)) {
        return true;
    } else {
        return false;
    }
}

bool BossStinkBugActionBase::updateGroundDamage() {
    if (MR::isFirstStep(this)) {
        MR::zeroVelocity(getHost());
        MR::startSound(getHost(), "SE_BM_BOSS_BUG_PRESSED");
        MR::startBck(getHost(), "GroundDamage");

        MR::tryRumblePadStrong(getHost(), WPAD_CHAN0);
        MR::shakeCameraStrong();

        getHost()->startEventCamera("Fall");
        getHost()->disposeBomb();

        _48 = 0.0f;
        MR::offPlayerControl();
    }

    if (MR::isStep(this, 9)) {
        MR::startSound(getHost(), "SE_BV_BOSS_BUG_DAMAGE");
    }

    if (MR::isBckStopped(getHost())) {
        MR::onPlayerControl(true);
        getHost()->endEventCamera("Fall");
        return true;
    } else {
        return false;
    }
}

bool BossStinkBugActionBase::updateFly() {
    if (MR::isFirstStep(this)) {
        MR::startBck(getHost(), "FlyWait");
    }

    startSoundFly(0);

    _48 = _38;
    BossStinkBugFunction::moveRail(getHost(), _48, _3C, _44);
    return false;
}

bool BossStinkBugActionBase::updateFlyShakeOffSign(s32 numSteps) {
    if (MR::isFirstStep(this)) {
        MR::startBck(getHost(), "FlyRide");
        mSpringValue->mSpringValue = _3C;
        mSpringValue->mRestValue = _3C;
        mSpringValue->mVelocity = -15.0f;
    }

    startSoundFly(0);

    _48 = calcShakeOffSignSpeed(numSteps);

    BossStinkBugFunction::moveRail(getHost(), _48, mSpringValue->update(), _44);

    if (MR::isGreaterStep(this, numSteps)) {
        return true;
    } else {
        return false;
    }
}

bool BossStinkBugActionBase::updateFlyShakeOff() {
    if (MR::isFirstStep(this)) {
        MR::startBck(getHost(), "FlyRollRight");
    }

    startSoundFly(3);

    _48 = calcShakeOffSpeed();

    BossStinkBugFunction::moveRail(getHost(), _48, mSpringValue->update(), _44);

    if (MR::isBckStopped(getHost())) {
        return true;
    } else {
        return false;
    }
}

bool BossStinkBugActionBase::updateFlyShakeOffWait(s32 numSteps) {
    if (MR::isFirstStep(this)) {
        MR::startBck(getHost(), "FlyRollWaitRight");
    }

    startSoundFly(0);

    _48 = 35.0f;
    BossStinkBugFunction::moveRail(getHost(), _48, _3C, _44);

    if (MR::isGreaterStep(this, numSteps)) {
        return true;
    } else {
        return false;
    }
}

bool BossStinkBugActionBase::updateFlyShakeOffEnd() {
    if (MR::isFirstStep(this)) {
        MR::startBck(getHost(), "FlyRollEndRight");
    }

    startSoundFly(0);

    _48 = calcShakeOffEndSpeed();
    BossStinkBugFunction::moveRail(getHost(), _48, _3C, _44);

    if (MR::isBckStopped(getHost())) {
        return true;
    } else {
        return false;
    }
}

bool BossStinkBugActionBase::updateFlyRollSign(s32 numSteps) {
    if (MR::isFirstStep(this)) {
        MR::startBck(getHost(), "FlyRollSignRight");
    }

    if (MR::isStep(this, 10)) {
        MR::startSound(getHost(), "SE_BV_BOSS_BUG_ROLL");
    }

    startSoundFly(0);

    _48 = _38;
    BossStinkBugFunction::moveRail(getHost(), _48, _3C, _44);

    if (MR::isGreaterStep(this, numSteps)) {
        return true;
    } else {
        return false;
    }
}

bool BossStinkBugActionBase::updateFlyTurnSign() {
    if (MR::isFirstStep(this)) {
        MR::startSound(getHost(), "SE_BV_BOSS_BUG_TURN");
        MR::startBck(getHost(), "FlyTurnSign");
    }

    startSoundFly(0);

    _48 = _38;
    BossStinkBugFunction::moveRail(getHost(), _48, _3C, _44);

    if (MR::isBckStopped(getHost())) {
        return true;
    } else {
        return false;
    }
}

bool BossStinkBugActionBase::updateFlyTurn() {
    if (MR::isFirstStep(this)) {
        MR::startBck(getHost(), "FlyTurn");
        _20 = getHost()->_EC;
        _50 = MR::isRailGoingToEnd(getHost());
    }

    startSoundFly(1);

    _48 = 0.0f;
    BossStinkBugFunction::turnRail(getHost(), _20, 0.0f, MR::calcNerveRate(this, 120), _38, _3C, _44, _50);

    if (MR::isGreaterStep(this, 120)) {
        return true;
    } else {
        return false;
    }
}

bool BossStinkBugActionBase::updateFall() {
    if (MR::isFirstStep(this)) {
        getHost()->startEventCamera("Fall");
        MR::startBck(getHost(), "Fall");
        MR::onBind(getHost());

        _14 = getHost()->mPosition;
        _48 = 0.0f;
        getHost()->disposeBomb();
    }

    MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_FLY_ADD", 600, 126);

    BossStinkBugFunction::setFallVelocity(getHost(), _14, MR::calcNerveRate(this, 80), 0.3f, 1100.0f);

    if (MR::isBinded(getHost())) {
        MR::offBind(getHost());
        MR::zeroVelocity(getHost());
        return true;
    } else {
        return false;
    }
}

bool BossStinkBugActionBase::updateFallDamage() {
    if (MR::isFirstStep(this)) {
        MR::startSound(getHost(), "SE_BM_BOSS_BUG_PRESSED");
        MR::startBck(getHost(), "FallDamage");
        MR::startAction(getHost()->getWingModel(), "FallDamage");
        MR::tryRumblePadStrong(getHost(), WPAD_CHAN0);
        MR::shakeCameraStrong();
        MR::offBind(getHost());

        _48 = 0.0f;
        MR::offPlayerControl();
        getHost()->disposeBomb();
    }

    if (MR::isStep(this, 9)) {
        MR::startSound(getHost(), "SE_BV_BOSS_BUG_DAMAGE");
    }

    if (MR::isBckStopped(getHost())) {
        MR::onPlayerControl(true);
        getHost()->endEventCamera("Fall");
        return true;
    } else {
        return false;
    }
}

bool BossStinkBugActionBase::updateToGround() {
    if (MR::isFirstStep(this)) {
        MR::startSound(getHost(), "SE_BV_BOSS_BUG_ANGRY_LAND");
        MR::startAction(getHost()->getWingModel(), "Ground");
        MR::tryStartBck(getHost(), "TurnDown");
        getHost()->validateCollisionGround();
        getHost()->onBindLeg();
        getHost()->_10C = 0.0f;
        _20 = getHost()->_EC;

        _2C = _3C + _44;
        _34 = 0.0f;
    }

    startSoundFly(0);
    _2C += _34;

    if (MR::isLessEqualStep(this, 40)) {
        _34 += 1.2f;
    }

    _34 -= 1.0f;
    _34 *= 0.99f;

    if (_2C < 0.0f) {
        _2C = 0.0f;
    }

    f32 nerveRate = MR::calcNerveRate(this, 60);
    getHost()->_10C = nerveRate;
    MR::rotateVecDegree(&getHost()->_EC, MR::getRailDirection(getHost()), getHost()->mGravity, MR::calcNerveValue(this, 60, 0.0f, 180.0f));

    _48 *= 0.9f;
    BossStinkBugFunction::moveRail(getHost(), _48, _2C, 0.0f);

    if (_2C <= 0.0f) {
        MR::reverseRailDirection(getHost());
        return true;
    } else {
        return false;
    }
}

bool BossStinkBugActionBase::updateLand() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(getHost(), "Land");
        getHost()->_10C = 1.0f;
    }

    _48 *= 0.99f;
    BossStinkBugFunction::moveRail(getHost(), _48, 0.0f, 0.0f);

    if (MR::isGreaterStep(this, 20)) {
        return true;
    } else {
        return false;
    }
}

bool BossStinkBugActionBase::updateToFly() {
    if (MR::isFirstStep(this)) {
        MR::startSound(getHost(), "SE_BV_BOSS_BUG_ANGRY_TAKEOFF");
        MR::startAction(getHost()->getWingModel(), "Fly");
        MR::tryStartBck(getHost(), "TakeOff");
        _30 = _48;
        getHost()->validateCollisionHitOnly();
        getHost()->offBindLeg();
    }

    if (MR::isLessStep(this, 0)) {
        startSoundFly(0);
    } else {
        startSoundFly(2);
    }

    if (MR::isLessStep(this, 60)) {
        _48 = MR::calcNerveEaseInOutValue(this, 60, _30, 40.0f);
    } else {
        _48 = MR::calcNerveEaseInOutValue(this, 60, 120, 40.0f, _38);
    }

    f32 nerveEaseInOutValue = MR::calcNerveEaseInOutValue(this, 120, 0.0f, _3C + _44);
    BossStinkBugFunction::moveRail(getHost(), _48, nerveEaseInOutValue, 0.0f);

    if (MR::isGreaterStep(this, 120)) {
        getHost()->validateCollisionFly();
        return true;
    } else {
        return false;
    }
}

void BossStinkBugActionBase::startSoundFly(s32 sound) {
    s32 flySoundVol = calcFlySoundVol();
    s32 iIRCCutoff = calcIIRCutOff();

    switch (sound) {
    case 0:
        if (isAngry()) {
            MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_FLY_BASE2", flySoundVol, iIRCCutoff);
        } else {
            MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_FLY_BASE", flySoundVol, iIRCCutoff);
        }

        MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_FLY_ADD", flySoundVol, iIRCCutoff);
        MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_FLY_ALARM", flySoundVol, iIRCCutoff);
        break;
    case 1:
        MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_FLY_ROTATE", flySoundVol, iIRCCutoff);
        MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_FLY_ROTATE2");
        MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_FLY_ADD", flySoundVol, iIRCCutoff);
        MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_FLY_ALARM", flySoundVol, iIRCCutoff);
        break;
    case 2:
        MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_FLY_ROLL", flySoundVol, iIRCCutoff);
        MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_FLY_ADD", flySoundVol, iIRCCutoff);
        MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_FLY_ALARM", flySoundVol, iIRCCutoff);
        break;
    case 3:
        if (isAngry()) {
            MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_FLY_SHAKE2", flySoundVol, iIRCCutoff);
        } else {
            MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_FLY_SHAKE", flySoundVol, iIRCCutoff);
        }

        MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_FLY_ADD", flySoundVol, iIRCCutoff);
        MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_FLY_ALARM", flySoundVol, iIRCCutoff);
    }
}
