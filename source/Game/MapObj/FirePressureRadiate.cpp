#include "Game/MapObj/FirePressureRadiate.hpp"

FirePressureRadiate::FirePressureRadiate(const char *pName) : LiveActor(pName) {
    mJointController = nullptr;
    mCannonRotation = 0.0f;
    mWaitTime = 300;
    mShootTime = 300;
    _CC = -1;
    _D0 = 0.0f;
    _D4 = 500.0f;
    mGroup = nullptr;
    _DC = 0;
    mRadiateMtx.identity();
}

// FirePressureRadiate::init

void FirePressureRadiate::initAfterPlacement() {
    if (mGroup != nullptr) {
        FirePressureRadiate* first = reinterpret_cast<FirePressureRadiate*>(MR::getGroupFromArray(this)->getActor(0));

        for (u16 i = 1; i < MR::getGroupFromArray(this)->getObjectCount(); i++) {
            FirePressureRadiate* cur = reinterpret_cast<FirePressureRadiate*>(MR::getGroupFromArray(this)->getActor(i));

            if (first->mWaitTime <= cur->mWaitTime) {
                first = cur;
            }
        }

        _DC = first == this;
    } 
}

void FirePressureRadiate::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
    mJointController->registerCallBack();
}

void FirePressureRadiate::exeRelax() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "FireShotStart", nullptr);
        MR::setBckFrame(this, 1.0f);
        MR::forceDeleteEffectAll(this);
        MR::invalidateHitSensor(this, "radiate");
    }
}

void FirePressureRadiate::exeSyncWait() {
    if (_DC) {
        if (MR::isStep(this, 60)) {
            mGroup->sendMsgToGroupMember(104, getSensor("body"), "body");
        }
    }
}

void FirePressureRadiate::exePrepareToRadiate() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "FireShotStart", nullptr);
    }

    if (MR::isStep(this, 34)) {
        setNerve(&NrvFirePressureRadiate::FirePressureRadiateNrvRadiate::sInstance);
    }
}

void FirePressureRadiate::exeRadiate() {
    MR::startLevelSound(this, "SE_OJ_LV_F_PRESSURE_RADIATE", -1, -1, -1);

    if (MR::isBckOneTimeAndStopped(this)) {
        calcRadiateEffectMtx();
        MR::startBck(this, "FireShot", nullptr);
    }

    if (MR::isStep(this, 25)) {
        MR::validateHitSensor(this, "radiate");
        _CC = 0;
    }

    if (MR::isGreaterEqualStep(this, mShootTime)) {
        setNerve(&NrvFirePressureRadiate::FirePressureRadiateNrvRadiateMargin::sInstance);
    }
}

void FirePressureRadiate::exeRadiateMargin() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "FireShotEnd", nullptr);
    }

    if (MR::isStep(this, 50)) {
        MR::invalidateHitSensor(this, "radiate");

        if (mGroup != nullptr) {
            setNerve(&NrvFirePressureRadiate::FirePressureRadiateNrvSyncWait::sInstance);
        }
        else {
            setNerve(&NrvFirePressureRadiate::FirePressureRadiateNrvWait::sInstance);
        }

        if (_DC) {
            mGroup->sendMsgToGroupMember(105, getSensor("body"), "body");
        }
    }
}

void FirePressureRadiate::exeWait() {
    if (MR::isStep(this, mWaitTime)) {
        setNerve(&NrvFirePressureRadiate::FirePressureRadiateNrvPrepareToRadiate::sInstance);
    }
}

void FirePressureRadiate::control() {
    if (_CC != -1) {
        if (_CC < 70) {
            f32 liner = MR::getLinerValue(_CC / 70.0f, 51.0f, _D4, 1.0f);
            _D0 = liner;
            _CC = _CC + 1;
        }
        else {
            _CC = -1;
            _D0 = _D4;
        }
    }
}

// FirePressureRadiate::attackSensor
// FirePressureRadiate::receiveOtherMsg
// FirePressureRadiate::startWait
// FirePressureRadiate::startRelax
// FirePressureRadiate::updateHitSensor

void FirePressureRadiate::calcRadiateEffectMtx() {
    MtxPtr jointMtx = MR::getJointMtx(this, "Cannon3");
    TPos3f effectMtx;
    JMath::gekko_ps_copy12(&effectMtx, jointMtx);
    TVec3f trans;
    effectMtx.getTrans(trans);
    JMath::gekko_ps_copy12(&mRadiateMtx, &effectMtx);
    mRadiateMtx.mMtx[0][3] = trans.x;
    mRadiateMtx.mMtx[1][3] = trans.y;
    mRadiateMtx.mMtx[2][3] = trans.z;
}

#ifdef NON_MATCHING
bool FirePressureRadiate::calcJointCannon(TPos3f *pOutPos, const JointControllerInfo &) {
    TVec3f v9(0.0f, 0.0f, 1.0f);
    f32 rotDegree = (0.017453292f * mCannonRotation);
    TPos3f v10;
    v10.mMtx[0][3] = 0.0f;
    v10.mMtx[1][3] = 0.0f;
    v10.mMtx[2][3] = 0.0f;
    TVec3f v8;
    v8.setInline(v9);
    PSVECMag(v8.toCVec()); // oops ?
    PSVECNormalize(v8.toCVec(), v8.toVec());
    f32 v5 = sin(rotDegree);
    f32 v6 = cos(rotDegree);
    v10.mMtx[0][0] = v6 + ((1.0f - v6) * (v8.x * v8.x));
    v10.mMtx[1][1] = v6 + ((1.0f - v6) * (v8.y * v8.y));
    v10.mMtx[2][2] = v6 + ((1.0f - v6) * (v8.z * v8.z));
    v10.mMtx[0][1] = (v8.y * ((1.0f - v6) * v8.x)) - (v5 * v8.z);
    v10.mMtx[0][2] = (v8.z * ((1.0f - v6) * v8.x)) + (v5 * v8.y);
    v10.mMtx[1][0] = (v8.y * ((1.0f - v6) * v8.x)) + (v5 * v8.z);
    v10.mMtx[2][0] = (v8.z * ((1.0f - v6) * v8.x)) - (v5 * v8.y);
    v10.mMtx[1][2] = (v8.z * ((1.0f - v6) * v8.y)) - (v5 * v8.x);
    v10.mMtx[2][1] = (v8.z * ((1.0f - v6) * v8.y)) + (v5 * v8.x);
    pOutPos->concat(*pOutPos, v10);
    return true;
}
#endif

namespace NrvFirePressureRadiate {
    INIT_NERVE(FirePressureRadiateNrvRelax);
    INIT_NERVE(FirePressureRadiateNrvSyncWait);
    INIT_NERVE(FirePressureRadiateNrvWait);
    INIT_NERVE(FirePressureRadiateNrvPrepareToRadiate);
    INIT_NERVE(FirePressureRadiateNrvRadiate);
    INIT_NERVE(FirePressureRadiateNrvRadiateMargin);
};

FirePressureRadiate::~ FirePressureRadiate() {

}
