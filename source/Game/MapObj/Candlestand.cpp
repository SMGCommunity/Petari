#include "Game/MapObj/Candlestand.hpp"

struct Param {
    const char* mObjName;           // 0x0
    f32 mClippingRadius;            // 0x4
    f32 mSensorRange;               // 0x8
    f32 _C;
    f32 _10;
    f32 _14;
    bool mCanUseSwitch;             // 0x18
};

namespace {
    static Param sParams[3] = {
        { 
            "PhantomCandlestand", 
            500.0f,                     // mClippingRadius
            50.0f,                      // mSensorRange
            220.0f,                     // 0xC
            0.0f,                       // 0x14
            false                       // mCanUseSwitch
        },
        {
            "TeresaMansionCandlestand", 
            800.0f,                     // mClippingRadius
            70.0f,                      // mSensorRange
            300.0f,                     // 0xC
            220.0f,                     // 0x10
            -5.0f,                      // 0x14
            false                       // mCanUseSwitch
        },
        {
            "CandlestandIceVolcano", 
            500.0f,                     // mClippingRadius
            150.0f,                     // mSensorRange
            220.0f,                     // 0xC
            0.0f,                       // 0x10
            0.0f,                       // 0x14
            true                       // mCanUseSwitch
        }
    };

    Param* get(s32 idx) {
        return &sParams[idx];
    }

    Param* getParam(const char *pObjName) NO_INLINE {
        for (s32 i = 0; i < 3; i++) {
            if (MR::isEqualString(pObjName, get(i)->mObjName)) {
                return get(i);
            }
        }

        return nullptr;
    }
};

Candlestand::Candlestand(const char *pName) : MapObjActor(pName) {
    mItem = -1;
    mHasItemAppear = false;
    mSpinPtclCb = nullptr;
}

void Candlestand::init(const JMapInfoIter &rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    info.setupHitSensor();
    TVec3f offs;
    offs.y = ::getParam(mObjectName)->_C;
    offs.x = 0.0f;
    offs.z = 0.0f;
    f32 sensorRange = getParam(mObjectName)->mSensorRange;
    info.setupHitSensorParam(8, sensorRange, offs);
    f32 clippingRadius = getParam(mObjectName)->mClippingRadius;
    info.setupClippingRadius(clippingRadius);
    info.setupNerve(&NrvCandlestand::HostTypeBurn::sInstance);
    info.setupAffectedScale();
    initialize(rIter, info);

    const char* objName = mObjectName;
    getParam(objName);
    getParam(objName);
    MR::getJMapInfoArg0NoInit(rIter, &mItem);
    if (mItem == -1) {
        MR::declareCoin(this, 1);
    }

    if (mItem == 0) {
        MR::declareStarPiece(this, 1);
    }

    if (getParam(mObjectName)->mCanUseSwitch) {
        if (MR::isValidSwitchA(this)) {
            setNerve(&NrvCandlestand::HostTypeWaitFire::sInstance);
        }
        else {
            setNerve(&NrvCandlestand::HostTypeBurn::sInstance);
        }
    }

    mSpinPtclCb = new SpinPullParticleCallBack();
}

void Candlestand::makeActorAppeared() {
    LiveActor::makeActorAppeared();

    if (!getParam(mObjectName)->mCanUseSwitch && !MR::isClipped(this)) {
        emitEffectFire();
    }
}

void Candlestand::startClipped() {
    MapObjActor::startClipped();

    if (isNerve(&NrvCandlestand::HostTypeBurn::sInstance) || isNerve(&NrvCandlestand::HostTypeAttack::sInstance)) {
        deleteEffectFire();
    }
}

void Candlestand::endClipped() {
    if (isNerve(&NrvCandlestand::HostTypeBurn::sInstance) || isNerve(&NrvCandlestand::HostTypeAttack::sInstance)) {
        emitEffectFire();
    }

    MapObjActor::endClipped();
}

void Candlestand::attackSensor(HitSensor* a2, HitSensor *a3) {
    if (isNerve(&NrvCandlestand::HostTypeBurn::sInstance)) {
        f32 range = mScale.x * 50.0f;
        f32 radius = a3->mRadius;
        if (MR::isNear(a2, a3, range + radius)) {
            if (MR::sendMsgEnemyAttackFire(a3, a2)) {
                setNerve(&NrvCandlestand::HostTypeAttack::sInstance);
            }
        }
    }
}

bool Candlestand::receiveMsgPlayerAttack(u32 msg, HitSensor *a3, HitSensor *a4) {
    if (!isNerve(&NrvCandlestand::HostTypeWaitFire::sInstance)) {
        return false;
    }

    if (!MR::isMsgFireBallAttack(msg)) {
        return false;
    }

    f32 range = mScale.x * 150.0f;
    f32 radius = a3->mRadius;

    if (!MR::isNear(a3, a4, range + radius)) {
        return false;
    }

    setNerve(&NrvCandlestand::HostTypeFire::sInstance);
    return true;
}

bool Candlestand::receiveMsgEnemyAttack(u32 msg, HitSensor *a3, HitSensor *a4) {
    if (!isNerve(&NrvCandlestand::HostTypeWaitFire::sInstance)) {
        return false;
    }

    if (!MR::isMsgEnemyAttackFire(msg)) {
        return false;
    }

    f32 range = mScale.x * 150.0f;
    f32 radius = a3->mRadius;

    if (!MR::isNear(a3, a4, range + radius)) {
        return false;
    }

    setNerve(&NrvCandlestand::HostTypeFire::sInstance);
    return true;
}

bool Candlestand::receiveOtherMsg(u32 msg, HitSensor *a3, HitSensor *a4) {
    if (MR::isInSpinStormRange(msg, a3, a4, (350.0f * mScale.x)) && isNerve(&NrvCandlestand::HostTypeBurn::sInstance)) {
        if (getParam(mObjectName)->mCanUseSwitch) {
            setNerve(&NrvCandlestand::HostTypeFlicker::sInstance);
            return true;
        }
        else {
            setNerve(&NrvCandlestand::HostTypeExtinguish::sInstance);
            return true;
        }
    }

    return false;
}


void Candlestand::emitEffectFire() {
    if (isObjectName("TeresaMansionCandlestand")) {
        MR::emitEffect(this, "Fire01");
        MR::emitEffect(this, "Fire02");
    }
    else {
        MR::emitEffect(this, "Fire");
    }
}

void Candlestand::deleteEffectFire() {
    if (isObjectName("TeresaMansionCandlestand")) {
        MR::deleteEffect(this, "Fire01");
        MR::deleteEffect(this, "Fire02");
    }
    else {
        MR::deleteEffect(this, "Fire");
    }
}

void Candlestand::emitEffectExtinguishFire() {
    if (isObjectName("TeresaMansionCandlestand")) {
        MR::emitEffectWithParticleCallBack(this, "Extinguish01", mSpinPtclCb);
        MR::emitEffectWithParticleCallBack(this, "Extinguish02", mSpinPtclCb);
    }
    else {
        MR::emitEffectWithParticleCallBack(this, "Extinguish", mSpinPtclCb);
    }
}

// appearItem

void Candlestand::exeFire() {
    if (MR::isFirstStep(this)) {
        emitEffectFire();
        MR::startSound(this, "SE_OJ_FIRE_STAND_IGNIT", -1, -1);
    }

    MR::startLevelSound(this, "SE_OJ_LV_PHANTOM_TOACH_BURN", -1, -1, -1);

    if (MR::isStep(this, 60)) {
        if (MR::isValidSwitchA(this)) {
            MR::onSwitchA(this);
        }

        setNerve(&NrvCandlestand::HostTypeBurn::sInstance);
    }
}

void Candlestand::exeExtinguish() {
    if (MR::isFirstStep(this)) {
        deleteEffectFire();
        emitEffectExtinguishFire();
        MR::startSound(this, "SE_OJ_PHANTOM_TOACH_OFF", -1, -1);
    }

    if (MR::isStep(this, 15)) {
        if (MR::isValidSwitchA(this)) {
            MR::offSwitchA(this);
        }

        appearItem();

        if (getParam(mObjectName)->mCanUseSwitch) {
            setNerve(&NrvCandlestand::HostTypeWaitFire::sInstance);
        }
    }
}

void Candlestand::exeFlicker() {
    if (MR::isFirstStep(this)) {
        deleteEffectFire();

        if (isObjectName("TeresaMansionCandlestand")) {
            MR::emitEffect(this, "Extinguish01");
            MR::emitEffect(this, "Extinguish02");
        }
        else {
            MR::emitEffect(this, "Extinguish");
        }

        MR::startSound(this, "SE_OJ_PHANTOM_TOACH_OFF", -1, -1);
    }

    if (MR::isStep(this, 30)) {
        emitEffectFire();
        setNerve(&NrvCandlestand::HostTypeBurn::sInstance);
    }
}

void Candlestand::exeAttack() {
    if (MR::isStep(this, 0x1E)) {
        setNerve(&NrvCandlestand::HostTypeBurn::sInstance);
    }
    else {
        MR::startLevelSound(this, "SE_OJ_LV_PHANTOM_TOACH_BURN", -1, -1, -1);
    }
}

void Candlestand::exeBurn() {
    MR::startLevelSound(this, "SE_OJ_LV_PHANTOM_TOACH_BURN", -1, -1, -1);
}

Candlestand::~Candlestand() {

}

namespace NrvCandlestand {
    INIT_NERVE(HostTypeWaitFire);
    INIT_NERVE(HostTypeFire);
    INIT_NERVE(HostTypeBurn);
    INIT_NERVE(HostTypeAttack);
    INIT_NERVE(HostTypeExtinguish);
    INIT_NERVE(HostTypeFlicker);
};