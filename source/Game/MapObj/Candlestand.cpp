#include "Game/MapObj/Candlestand.hpp"

struct Param {
    const char* mObjName;           // _0
    f32 mClippingRadius;            // _4
    f32 mSensorRange;               // _8
    f32 _C;
    f32 _10;
    f32 _14;
    bool mCanUseSwitch;             // _18
};

namespace {
    static Param sParams[3] = {
        { 
            "PhantomCandlestand", 
            500.0f,                     // mClippingRadius
            50.0f,                      // mSensorRange
            220.0f,                     // _C
            0.0f,                       // _14
            false                       // mCanUseSwitch
        },
        {
            "TeresaMansionCandlestand", 
            800.0f,                     // mClippingRadius
            70.0f,                      // mSensorRange
            300.0f,                     // _C
            220.0f,                     // _10
            -5.0f,                      // _14
            false                       // mCanUseSwitch
        },
        {
            "CandlestandIceVolcano", 
            500.0f,                     // mClippingRadius
            150.0f,                     // mSensorRange
            220.0f,                     // _C
            0.0f,                       // _10
            0.0f,                       // _14
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

// startClipped
// endClipped
// attackSensor
// receiveMsgPlayerAttack
// receiveMsgEnemyAttack
// receiveOtherMsg

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
// exeFire
// exeExtinguish
// exeFlicker
// exeAttack
// exeBurn

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