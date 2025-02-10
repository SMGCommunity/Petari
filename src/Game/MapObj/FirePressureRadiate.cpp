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

void FirePressureRadiate::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("FirePressure", nullptr, false);
    MR::connectToSceneNoShadowedMapObjStrongLight(this);
    initHitSensor(3);
    MR::addHitSensorMapObj(this, "body", 8, 70.0f, TVec3f(0.0f, 30.0f, 0.0f));
    MR::addHitSensorAtJointMapObj(this, "cannon", "Cannon1", 8, 70.0f, TVec3f(40.0f, 0.0f, 0.0f));
    MR::addHitSensorCallbackEnemyAttack(this, "radiaet", 8, 50.0f);
    MR::invalidateHitSensor(this, "radiate");
    initEffectKeeper(0, nullptr, false);
    MR::setEffectHostMtx(this, "Fire", mRadiateMtx.mMtx);
    MR::setEffectHostMtx(this, "FireInd", mRadiateMtx.mMtx);
    initSound(4, false);
    MR::getJMapInfoArg0NoInit(rIter, &mCannonRotation);
    MR::getJMapInfoArg1NoInit(rIter, &mWaitTime);
    MR::getJMapInfoArg2NoInit(rIter, &mShootTime);
    mJointController = MR::createJointDelegatorWithNullChildFunc(this, &FirePressureRadiate::calcJointCannon, "Cannon1");
    MR::initJointTransform(this);
    MR::calcGravity(this);
    MR::setGroupClipping(this, rIter, 16);
    mGroup = MR::joinToGroupArray(this, rIter, "ファイアプレッシャー（放射）軍団", 16);

    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::tryRegisterDemoActionFunctor(this, MR::Functor(this, &FirePressureRadiate::startRelax), nullptr);
    }

    if (MR::useStageSwitchWriteA(this, rIter)) {
        MR::listenStageSwitchOnOffA(this, MR::Functor(this, &FirePressureRadiate::startWait), MR::Functor(this, &FirePressureRadiate::startRelax));
        initNerve(&NrvFirePressureRadiate::FirePressureRadiateNrvRelax::sInstance);
    }
    else {
        initNerve(&NrvFirePressureRadiate::FirePressureRadiateNrvWait::sInstance);
    }

    MR::useStageSwitchSleep(this, rIter);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        makeActorDead();
    }
    else {
        makeActorAppeared();
    }
}

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

void FirePressureRadiate::attackSensor(HitSensor *a1, HitSensor *a2) {
    if (MR::isSensorEnemyAttack(a1) && MR::isSensorPlayerOrRide(a2)) {
        MR::sendMsgEnemyAttackFire(a2, a1);
    }
    else if (MR::isSensorMapObj(a1) && (MR::isSensorPlayer(a2) || MR::isSensorEnemy(a2))) {
        MR::sendMsgPush(a2, a1);
    }
}

bool FirePressureRadiate::receiveOtherMsg(u32 msg, HitSensor *a1, HitSensor *) {
    if (msg == 105) {
        setNerve(&NrvFirePressureRadiate::FirePressureRadiateNrvSyncWait::sInstance);
        return true;
    }
    else if (msg == 104) {
        setNerve(&NrvFirePressureRadiate::FirePressureRadiateNrvWait::sInstance);
        return true;
    }

    return false;
}

void FirePressureRadiate::startWait() {
    if (isNerve(&NrvFirePressureRadiate::FirePressureRadiateNrvWait::sInstance)) {
        setNerve(&NrvFirePressureRadiate::FirePressureRadiateNrvWait::sInstance);
    }
}

void FirePressureRadiate::startRelax() {
    if (!isNerve(&NrvFirePressureRadiate::FirePressureRadiateNrvRelax::sInstance)) {
        setNerve(&NrvFirePressureRadiate::FirePressureRadiateNrvRelax::sInstance);
    }
}

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
    PSVECMag(v8); // oops ?
    PSVECNormalize(v8, v8);
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
