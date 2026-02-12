#include "Game/Boss/TombSpiderActionBase.hpp"
#include "Game/Boss/TombSpider.hpp"
#include "Game/Boss/TombSpiderFunction.hpp"
#include "Game/Boss/TombSpiderGland.hpp"
#include "Game/Boss/TombSpiderVitalSpot.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/MapObj/SpiderThread.hpp"

// NOTE: this function order is how it appears in the debug symbol map, because that order lets the .data section match.
// Writing directly as the release symbol map produces incorrect .data order.

// FIXME: this should be the definition for MR::clamp, however MR::clamp does not behave well with this file
inline f32 clamp(f32 x, f32 min, f32 max) {
    if (x < min) {
        return min;
    }

    if (x > max) {
        return max;
    }

    return x;
}

namespace {
    static const s32 sEnergyMax = 3;
    static const s32 sStepAttackLoopSensorValidate = 20;
    static const s32 sStepAttackLoopMin = 30;
    static const s32 sStepChanceStartEndSign1 = -480;
    static const s32 sStepChanceStartEndSign2 = -240;
    static const f32 sChanceStartEndSignAnimRate1 = 1.0f;
    static const f32 sChanceStartEndSignAnimRate2 = 2.0f;
    static const s32 sStepChanceEndDemo = 120;
    static const s32 sStepChanceEndAnim = 30;
}  // namespace

TombSpiderActionBase::TombSpiderActionBase(TombSpider* pParent, const char* pName)
    : ActorStateBase< TombSpider >(pName, pParent), mParent(pParent), mRotateAccel(0.0f), mRotateSpeed(0.0f), mHitStep(-1), mWaitTime(0),
      mAttackTime(0), mEnergy(sEnergyMax), mPosZ(0.0f), mInDemo(true) {
}

void TombSpiderActionBase::init() {
    kill();
}

void TombSpiderActionBase::appear() {
    mPosZ = mParent->mPosition.z;
    mEnergy = sEnergyMax;
    mIsDead = false;
}

bool TombSpiderActionBase::updateWait(s32 minWaitTime, s32 maxWaitTime) {
    if (MR::isFirstStep(this)) {
        MR::startAction(mParent, "Wait");

        if (!TombSpiderFunction::getGlandFrontL(mParent)->isDamage()) {
            MR::startAction(TombSpiderFunction::getGlandFrontL(mParent), "Wait");
        }

        if (!TombSpiderFunction::getGlandFrontR(mParent)->isDamage()) {
            MR::startAction(TombSpiderFunction::getGlandFrontR(mParent), "Wait");
        }

        if (!TombSpiderFunction::getGlandRearL(mParent)->isDamage()) {
            MR::startAction(TombSpiderFunction::getGlandRearL(mParent), "Wait");
        }

        if (!TombSpiderFunction::getGlandRearR(mParent)->isDamage()) {
            MR::startAction(TombSpiderFunction::getGlandRearR(mParent), "Wait");
        }

        mWaitTime = MR::getRandom(minWaitTime, maxWaitTime);
    }

    if (MR::isStep(this, mWaitTime)) {
        return true;
    }

    return false;
}

bool TombSpiderActionBase::updateDamageEye() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mParent, "EyeDamage");
        TombSpiderFunction::endAcid(mParent);
        MR::startSound(mParent, "SE_BM_TSPIDER_DAMAGE_EYE", -1, -1);
        MR::shakeCameraNormal();
    }

    if (MR::isBckStopped(mParent)) {
        return true;
    }

    return false;
}

bool TombSpiderActionBase::updateDamageHip() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mParent, "HipDamage");
        TombSpiderFunction::endAcid(mParent);
        MR::startSound(mParent, "SE_BM_TSPIDER_DAMAGE_HIP", -1, -1);
        MR::shakeCameraNormal();
    }

    if (MR::isBckStopped(mParent)) {
        return true;
    }

    return false;
}

bool TombSpiderActionBase::updateDamageGland() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mParent, "GlandDamage");
        MR::shakeCameraNormal();
        TombSpiderFunction::endAcid(mParent);
        TombSpiderFunction::invalidateAttackSensor(mParent);
        MR::startSound(mParent, "SE_BM_TSPIDER_DAMAGE_GLAND", -1, -1);
    }

    if (MR::isBckStopped(mParent)) {
        return true;
    }

    return false;
}

bool TombSpiderActionBase::updateAttackStart() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mParent, "AttackStart");

        if (TombSpiderFunction::isAttackEnable(TombSpiderFunction::getGlandFrontL(mParent))) {
            MR::startAction(TombSpiderFunction::getGlandFrontL(mParent), "AttackStart");
        }

        if (TombSpiderFunction::isAttackEnable(TombSpiderFunction::getGlandFrontR(mParent))) {
            MR::startAction(TombSpiderFunction::getGlandFrontR(mParent), "AttackStart");
        }

        if (TombSpiderFunction::isAttackEnable(TombSpiderFunction::getGlandRearL(mParent))) {
            MR::startAction(TombSpiderFunction::getGlandRearL(mParent), "AttackStart");
        }

        if (TombSpiderFunction::isAttackEnable(TombSpiderFunction::getGlandRearR(mParent))) {
            MR::startAction(TombSpiderFunction::getGlandRearR(mParent), "AttackStart");
        }

        MR::startSound(mParent, "SE_BM_TSPIDER_PRE_ACID", -1, -1);
    }

    if (MR::isBckStopped(mParent)) {
        return true;
    }

    MR::startLevelSound(mParent, "SE_BM_LV_TSPIDER_PRE_ACID", -1, -1, -1);
    return false;
}

bool TombSpiderActionBase::updateAttackLoop(s32 minAttackTime, s32 maxAttackTime, f32 rotateAccel, f32 maxRotateSpeed) {
    if (MR::isFirstStep(this)) {
        mRotateSpeed = 0.0f;

        if (TombSpiderFunction::isRotateLeftToPlayer(mParent)) {
            MR::startAction(mParent, "AttackLoopL");
            mRotateAccel = rotateAccel;
        } else {
            MR::startAction(mParent, "AttackLoopR");
            mRotateAccel = -rotateAccel;
        }

        if (TombSpiderFunction::isAttackEnable(TombSpiderFunction::getGlandFrontL(mParent))) {
            MR::startAction(TombSpiderFunction::getGlandFrontL(mParent), "AttackLoop");
        }

        if (TombSpiderFunction::isAttackEnable(TombSpiderFunction::getGlandFrontR(mParent))) {
            MR::startAction(TombSpiderFunction::getGlandFrontR(mParent), "AttackLoop");
        }

        if (TombSpiderFunction::isAttackEnable(TombSpiderFunction::getGlandRearL(mParent))) {
            MR::startAction(TombSpiderFunction::getGlandRearL(mParent), "AttackLoop");
        }

        if (TombSpiderFunction::isAttackEnable(TombSpiderFunction::getGlandRearR(mParent))) {
            MR::startAction(TombSpiderFunction::getGlandRearR(mParent), "AttackLoop");
        }

        TombSpiderFunction::startAcid(mParent);
        mHitStep = -1;
        mAttackTime = MR::getRandom(minAttackTime, maxAttackTime);
    }

    if (MR::isStep(this, sStepAttackLoopSensorValidate)) {
        TombSpiderFunction::validateAttackSensor(mParent);
    }

    mRotateSpeed += mRotateAccel;
    // MR::clamp needs to be properly defined
    // mRotateSpeed = MR::clamp(mRotateSpeed, -maxRotateSpeed, maxRotateSpeed);
    mRotateSpeed = clamp(mRotateSpeed, -maxRotateSpeed, maxRotateSpeed);
    mParent->mRotation.z += mRotateSpeed;
    mParent->mRotation.z = MR::repeat(mParent->mRotation.z, 0.0f, 360.0f);

    MR::startLevelSound(mParent, "SE_BM_LV_TSPIDER_ACID", -1, -1, -1);

    if (mHitStep >= 0 && MR::isStep(this, mHitStep + sStepAttackLoopMin)) {
        TombSpiderFunction::endAcid(mParent);
        return true;
    }

    if (MR::isGreaterStep(this, mAttackTime)) {
        TombSpiderFunction::endAcid(mParent);
        return true;
    }

    return false;
}

bool TombSpiderActionBase::updateAttackEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mParent, "AttackEnd");
        MR::startSound(mParent, "SE_BM_TSPIDER_ACID_END", -1, -1);
    }

    if (MR::isBckStopped(mParent)) {
        return true;
    }

    return false;
}

bool TombSpiderActionBase::updateChanceStart() {
    if (mInDemo) {
        return updateChanceStartDemo();
    } else {
        return updateChanceStartNoDemo();
    }
}

bool TombSpiderActionBase::updateChanceStartNoDemo() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mParent, "ChanceStart");

        TombSpiderFunction::getVitalSpotC(mParent)->startChance();
        TombSpiderFunction::getVitalSpotL(mParent)->startChance();
        TombSpiderFunction::getVitalSpotR(mParent)->startChance();

        TombSpiderFunction::invalidateMainPartsSensor(mParent);
        TombSpiderFunction::invalidateGlandSensor(mParent);

        mParent->getSensor("body")->validate();
    }

    if (MR::isBckStopped(mParent)) {
        mParent->getSensor("body")->invalidate();
        TombSpiderFunction::validateVitalSpotSensor(mParent);
        MR::onSwitchA(mParent);
        return true;
    }

    return false;
}

bool TombSpiderActionBase::updateChanceStartDemo() {
    if (MR::isFirstStep(this)) {
        TombSpiderFunction::invalidateMainPartsSensor(mParent);
        TombSpiderFunction::invalidateGlandSensor(mParent);
        TombSpiderFunction::validateVitalSpotSensor(mParent);
        TombSpiderFunction::startTombSpiderDemo(mParent, "チャンス開始", "チャンス開始[トゥームスパイダー]");

        MR::sendMsgToAllLiveActor(ACTMES_TOMB_SPIDER_DEMO_START, nullptr);
        MR::startAction(mParent, "ChanceStart");

        TombSpiderFunction::getVitalSpotC(mParent)->startChance();
        TombSpiderFunction::getVitalSpotL(mParent)->startChance();
        TombSpiderFunction::getVitalSpotR(mParent)->startChance();
    }

    if (MR::isBckStopped(mParent)) {
        TombSpiderFunction::endTombSpiderDemo(mParent, "チャンス開始", "チャンス開始[トゥームスパイダー]");
        MR::onSwitchA(mParent);
        return true;
    }

    return false;
}

bool TombSpiderActionBase::updateChanceWait(s32 waitEndTime) {
    if (MR::isFirstStep(this)) {
        MR::startAction(mParent, "ChanceWait");
        TombSpiderFunction::killThreadAttacherAll(mParent);
        MR::startSpiderThreadChance();
    }

    if (MR::isStep(this, waitEndTime + sStepChanceStartEndSign1)) {
        MR::startAction(mParent, "ChanceWaitEndSign");
        MR::setBckRate(mParent, sChanceStartEndSignAnimRate1);
    }

    if (MR::isBckOneTimeAndStopped(mParent)) {
        MR::startAction(mParent, "ChanceWaitEndSign");
        if (MR::isGreaterStep(this, waitEndTime + sStepChanceStartEndSign2)) {
            MR::setBckRate(mParent, sChanceStartEndSignAnimRate2);
        } else if (MR::isGreaterStep(this, waitEndTime + sStepChanceStartEndSign1)) {
            MR::setBckRate(mParent, sChanceStartEndSignAnimRate1);
        }
    }

    if (MR::isStep(this, waitEndTime)) {
        return true;
    }

    return false;
}

bool TombSpiderActionBase::updateChanceDamage() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mParent, "ChanceDamage");
    }

    if (MR::isBckStopped(mParent)) {
        TombSpiderFunction::killThreadAttacherAll(mParent);
        mParent->mPosition.z = mPosZ;
        return true;
    }

    return false;
}

bool TombSpiderActionBase::updateChanceEnd() {
    if (mInDemo) {
        return updateChanceEndDemo();
    } else {
        return updateChanceEndNoDemo();
    }
}

bool TombSpiderActionBase::updateChanceEndNoDemo() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mParent, "ChanceEnd");

        TombSpiderFunction::getVitalSpotC(mParent)->endChance();
        TombSpiderFunction::getVitalSpotL(mParent)->endChance();
        TombSpiderFunction::getVitalSpotR(mParent)->endChance();

        TombSpiderFunction::invalidateVitalSpotSensor(mParent);
        MR::offSwitchA(mParent);
        TombSpiderFunction::appearThreadAttacherAll(mParent);
        MR::startSpiderThreadBattle();
    }

    if (MR::isBckStopped(mParent)) {
        TombSpiderFunction::validateMainPartsSensor(mParent);
        TombSpiderFunction::validateGlandSensor(mParent);
        return true;
    }

    return false;
}

bool TombSpiderActionBase::updateChanceEndDemo() {
    if (MR::isFirstStep(this)) {
        TombSpiderFunction::validateMainPartsSensor(mParent);
        TombSpiderFunction::validateGlandSensor(mParent);
        TombSpiderFunction::invalidateVitalSpotSensor(mParent);
        TombSpiderFunction::startTombSpiderDemo(mParent, "チャンス終了", "チャンス開始[トゥームスパイダー]");
        MR::offSwitchA(mParent);
        TombSpiderFunction::appearThreadAttacherAll(mParent);
        MR::startSpiderThreadBattle();
        MR::sendMsgToAllLiveActor(ACTMES_TOMB_SPIDER_DEMO_START, nullptr);
    }

    if (MR::isStep(this, sStepChanceEndAnim)) {
        MR::startAction(mParent, "ChanceEnd");
        TombSpiderFunction::getVitalSpotC(mParent)->endChance();
        TombSpiderFunction::getVitalSpotL(mParent)->endChance();
        TombSpiderFunction::getVitalSpotR(mParent)->endChance();
    }

    if (MR::isStep(this, sStepChanceEndDemo)) {
        mInDemo = false;
        TombSpiderFunction::endTombSpiderDemo(mParent, "チャンス終了", "チャンス開始[トゥームスパイダー]");
        return true;
    }

    return false;
}

bool TombSpiderActionBase::tryWaitChanceStartDemo(const Nerve* pNerve) {
    if (!mInDemo) {
        return false;
    }
    // inline? this string appears later than this
    MR::requestStartDemoMarioPuppetable(this, mParent, "チャンス開始", pNerve, nullptr);
    return true;
}

bool TombSpiderActionBase::tryWaitChanceEndDemo(const Nerve* pNerve) {
    if (!mInDemo) {
        return false;
    }
    // inline? this string appears later than this
    MR::requestStartDemoMarioPuppetable(this, mParent, "チャンス終了", pNerve, nullptr);
    return true;
}

void TombSpiderActionBase::startChanceDamage() {
    mEnergy--;
    MR::shakeCameraNormal();
    TombSpiderFunction::appearThreadAttacherAll(mParent);
    if (mEnergy <= 0) {
        MR::sendMsgToAllLiveActor(ACTMES_TOMB_SPIDER_DEMO_START, nullptr);
        MR::offSwitchA(mParent);
        TombSpiderFunction::appearThreadAttacherAll(mParent);
        MR::startSpiderThreadBattle();
    }
}
