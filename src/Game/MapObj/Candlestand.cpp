#include "Game/MapObj/Candlestand.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util.hpp"

namespace NrvCandlestand {
    NEW_NERVE(HostTypeWaitFire, Candlestand, WaitFire);
    NEW_NERVE(HostTypeFire, Candlestand, Fire);
    NEW_NERVE(HostTypeBurn, Candlestand, Burn);
    NEW_NERVE(HostTypeAttack, Candlestand, Attack);
    NEW_NERVE(HostTypeExtinguish, Candlestand, Extinguish);
    NEW_NERVE(HostTypeFlicker, Candlestand, Flicker);
};  // namespace NrvCandlestand

struct Param {
    const char* mObjName;  // 0x0
    f32 mClippingRadius;   // 0x4
    f32 mSensorRange;      // 0x8
    f32 _C;
    f32 _10;
    f32 _14;
    bool mCanUseSwitch;  // 0x18
};

namespace {
    static const Param sParams[] = {{
                                        "PhantomCandlestand",
                                        500.0f,  // mClippingRadius
                                        50.0f,   // mSensorRange
                                        220.0f,  // 0xC
                                        0.0f,    // 0x14
                                        false    // mCanUseSwitch
                                    },
                                    {
                                        "TeresaMansionCandlestand",
                                        800.0f,  // mClippingRadius
                                        70.0f,   // mSensorRange
                                        300.0f,  // 0xC
                                        220.0f,  // 0x10
                                        -5.0f,   // 0x14
                                        false    // mCanUseSwitch
                                    },
                                    {
                                        "CandlestandIceVolcano",
                                        500.0f,  // mClippingRadius
                                        150.0f,  // mSensorRange
                                        220.0f,  // 0xC
                                        0.0f,    // 0x10
                                        0.0f,    // 0x14
                                        true     // mCanUseSwitch
                                    }};

    const Param* getParam(const char* pObjName) NO_INLINE {
        for (u32 i = 0; i < ARRAY_SIZE(sParams); i++) {
            if (MR::isEqualString(pObjName, sParams[i].mObjName)) {
                return &sParams[i];
            }
        }

        return nullptr;
    }
};  // namespace

Candlestand::Candlestand(const char* pName) : MapObjActor(pName) {
    mItem = -1;
    mHasItemAppear = false;
    mSpinPtclCb = nullptr;
}

void Candlestand::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    info.setupHitSensor();
    TVec3f offs;
    offs.y = ::getParam(mObjectName)->_C;
    offs.x = 0.0f;
    offs.z = 0.0f;
    f32 sensorRange = ::getParam(mObjectName)->mSensorRange;
    info.setupHitSensorParam(8, sensorRange, offs);
    f32 clippingRadius = ::getParam(mObjectName)->mClippingRadius;
    info.setupClippingRadius(clippingRadius);
    info.setupNerve(GET_NERVE(Candlestand, HostTypeBurn));
    info.setupAffectedScale();
    initialize(rIter, info);

    const char* objName = mObjectName;
    ::getParam(objName);
    ::getParam(objName);
    MR::getJMapInfoArg0NoInit(rIter, &mItem);
    if (mItem == -1) {
        MR::declareCoin(this, 1);
    }

    if (mItem == 0) {
        MR::declareStarPiece(this, 1);
    }

    if (::getParam(mObjectName)->mCanUseSwitch) {
        if (MR::isValidSwitchA(this)) {
            setNerve(GET_NERVE(Candlestand, HostTypeWaitFire));
        } else {
            setNerve(GET_NERVE(Candlestand, HostTypeBurn));
        }
    }

    mSpinPtclCb = new SpinPullParticleCallBack();
}

void Candlestand::makeActorAppeared() {
    LiveActor::makeActorAppeared();

    if (!::getParam(mObjectName)->mCanUseSwitch && !MR::isClipped(this)) {
        emitEffectFire();
    }
}

void Candlestand::startClipped() {
    MapObjActor::startClipped();

    if (isNerve(GET_NERVE(Candlestand, HostTypeBurn)) || isNerve(GET_NERVE(Candlestand, HostTypeAttack))) {
        deleteEffectFire();
    }
}

void Candlestand::endClipped() {
    if (isNerve(GET_NERVE(Candlestand, HostTypeBurn)) || isNerve(GET_NERVE(Candlestand, HostTypeAttack))) {
        emitEffectFire();
    }

    MapObjActor::endClipped();
}

void Candlestand::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!isNerve(GET_NERVE(Candlestand, HostTypeBurn))) {
        return;
    }

    f32 range = mScale.x * 50.0f;
    f32 radius = pReceiver->mRadius;

    if (!MR::isNear(pSender, pReceiver, range + radius)) {
        return;
    }

    if (!MR::sendMsgEnemyAttackFire(pReceiver, pSender)) {
        return;
    }

    setNerve(GET_NERVE(Candlestand, HostTypeAttack));
}

bool Candlestand::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!isNerve(GET_NERVE(Candlestand, HostTypeWaitFire))) {
        return false;
    }

    if (!MR::isMsgFireBallAttack(msg)) {
        return false;
    }

    f32 range = mScale.x * 150.0f;
    f32 radius = pSender->mRadius;

    if (!MR::isNear(pSender, pReceiver, range + radius)) {
        return false;
    }

    setNerve(GET_NERVE(Candlestand, HostTypeFire));
    return true;
}

bool Candlestand::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!isNerve(GET_NERVE(Candlestand, HostTypeWaitFire))) {
        return false;
    }

    if (!MR::isMsgEnemyAttackFire(msg)) {
        return false;
    }

    f32 range = mScale.x * 150.0f;
    f32 radius = pSender->mRadius;

    if (!MR::isNear(pSender, pReceiver, range + radius)) {
        return false;
    }

    setNerve(GET_NERVE(Candlestand, HostTypeFire));
    return true;
}

bool Candlestand::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isInSpinStormRange(msg, pSender, pReceiver, (350.0f * mScale.x)) && isNerve(GET_NERVE(Candlestand, HostTypeBurn))) {
        if (::getParam(mObjectName)->mCanUseSwitch) {
            setNerve(GET_NERVE(Candlestand, HostTypeFlicker));
            return true;
        } else {
            setNerve(GET_NERVE(Candlestand, HostTypeExtinguish));
            return true;
        }
    }

    return false;
}

void Candlestand::emitEffectFire() {
    if (isObjectName("TeresaMansionCandlestand")) {
        MR::emitEffect(this, "Fire01");
        MR::emitEffect(this, "Fire02");
    } else {
        MR::emitEffect(this, "Fire");
    }
}

void Candlestand::deleteEffectFire() {
    if (isObjectName("TeresaMansionCandlestand")) {
        MR::deleteEffect(this, "Fire01");
        MR::deleteEffect(this, "Fire02");
    } else {
        MR::deleteEffect(this, "Fire");
    }
}

void Candlestand::emitEffectExtinguishFire() {
    if (isObjectName("TeresaMansionCandlestand")) {
        MR::emitEffectWithParticleCallBack(this, "Extinguish01", mSpinPtclCb);
        MR::emitEffectWithParticleCallBack(this, "Extinguish02", mSpinPtclCb);
    } else {
        MR::emitEffectWithParticleCallBack(this, "Extinguish", mSpinPtclCb);
    }
}

// appearItem

void Candlestand::exeWaitFire() {
}

void Candlestand::exeFire() {
    if (MR::isFirstStep(this)) {
        emitEffectFire();
        MR::startSound(this, "SE_OJ_FIRE_STAND_IGNIT");
    }

    MR::startLevelSound(this, "SE_OJ_LV_PHANTOM_TOACH_BURN");

    if (MR::isStep(this, 60)) {
        if (MR::isValidSwitchA(this)) {
            MR::onSwitchA(this);
        }

        setNerve(GET_NERVE(Candlestand, HostTypeBurn));
    }
}

void Candlestand::exeExtinguish() {
    if (MR::isFirstStep(this)) {
        deleteEffectFire();
        emitEffectExtinguishFire();
        MR::startSound(this, "SE_OJ_PHANTOM_TOACH_OFF");
    }

    if (MR::isStep(this, 15)) {
        if (MR::isValidSwitchA(this)) {
            MR::offSwitchA(this);
        }

        appearItem();

        if (::getParam(mObjectName)->mCanUseSwitch) {
            setNerve(GET_NERVE(Candlestand, HostTypeWaitFire));
        }
    }
}

void Candlestand::exeFlicker() {
    if (MR::isFirstStep(this)) {
        deleteEffectFire();

        if (isObjectName("TeresaMansionCandlestand")) {
            MR::emitEffect(this, "Extinguish01");
            MR::emitEffect(this, "Extinguish02");
        } else {
            MR::emitEffect(this, "Extinguish");
        }

        MR::startSound(this, "SE_OJ_PHANTOM_TOACH_OFF");
    }

    if (MR::isStep(this, 30)) {
        emitEffectFire();
        setNerve(GET_NERVE(Candlestand, HostTypeBurn));
    }
}

void Candlestand::exeAttack() {
    if (MR::isStep(this, 30)) {
        setNerve(GET_NERVE(Candlestand, HostTypeBurn));
    } else {
        MR::startLevelSound(this, "SE_OJ_LV_PHANTOM_TOACH_BURN");
    }
}

void Candlestand::exeBurn() {
    MR::startLevelSound(this, "SE_OJ_LV_PHANTOM_TOACH_BURN");
}

Candlestand::~Candlestand() {
}
