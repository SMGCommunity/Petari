#include "Game/MapObj/LavaProminenceTriple.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Util.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"

void LavaProminenceTriple_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)2.0f;
}

void LavaProminenceTriple_DUMMY() {
    TVec3f a, b;
    a.add(b);
}

namespace {
    static const f32 sNoSensorTable[] = {
        0.0f, 59.765598f, 134.2f, 176.045f, 222.6f, 278.26199f, 360.0f,
    };

    static const f32 sInitRotateVel = 100.0f;
    static const f32 sToSensorRadius = 1900.0f;
    static const f32 sEnvironmentRadius = 2100.0f;
};  // namespace

LavaProminenceTriple::LavaProminenceTriple(const char* pName)
    : LiveActor(pName), mBloomModel(), mBaseRot(0, 0, 0, 1), mCurrRot(0, 0, 0, 1), mSide(0, 0, 1), mFront(1, 0, 0), mIsAppearDrop1(),
      mRotateVel(::sInitRotateVel), mIsAppearDrop2(), mIsAppearDrop3(), mIsEnvironment(), mDefaultAppearTime() {
    mBloomMtx.identity();
}

void LavaProminenceTriple::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    TQuat4f rot;
    rot.setEulerDegree(mRotation.x, mRotation.y, mRotation.z);
    rot.getZDir(mFront);
    rot.getXDir(mSide);
    mBaseRot = rot;

    MR::getJMapInfoArg0NoInit(rIter, &mRotateVel);
    MR::getJMapInfoArg1NoInit(rIter, &mDefaultAppearTime);
    mAppearTime = mDefaultAppearTime;

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
    }

    const char* objName = nullptr;
    MR::getObjectName(&objName, rIter);

    if (MR::isEqualString(objName, "LavaProminenceEnvironment")) {
        initModelManagerWithAnm("LavaProminenceEnvironment", nullptr, false);
        MR::startBtk(this, "LavaProminenceEnvironment");
        mIsEnvironment = true;
        mBloomModel = MR::createBloomModel(this, mBloomMtx);
        MR::startBtk(mBloomModel, "LavaProminenceEnvironmentBloom");
    } else {
        initModelManagerWithAnm("LavaProminenceTriple", nullptr, false);
        MR::startBtk(this, "LavaProminenceTriple");
        mIsEnvironment = false;
        mBloomModel = MR::createBloomModel(this, mBloomMtx);
    }

    initSound(4, false);
    MR::connectToSceneMapObj(this);

    if (!mIsEnvironment) {
        initHitSensor(1);
        MR::addHitSensorCallbackMapObj(this, "attack", 1, 80.0f * mScale.y);
    }

    initEffectKeeper(0, nullptr, false);
    MR::setGroupClipping(this, rIter, 16);

    if (mIsEnvironment) {
        MR::setClippingTypeSphere(this, ::sEnvironmentRadius * mScale.y);
    } else {
        MR::setClippingTypeSphere(this, ::sToSensorRadius * mScale.y);
    }

    if (MR::isValidSwitchAppear(this)) {
        MR::invalidateClipping(this);
    }

    makeActorAppeared();
}

void LavaProminenceTriple::appear() {
    LiveActor::appear();
    mIsAppearDrop1 = true;
    mAppearTime = mDefaultAppearTime;
    MR::emitEffect(this, "Drop1");

    if (!mIsEnvironment) {
        mIsAppearDrop2 = true;
        mIsAppearDrop3 = true;
        MR::emitEffect(this, "Drop2");
        MR::emitEffect(this, "Drop3");
    }
}

void LavaProminenceTriple::control() {
    if (mAppearTime > 0) {
        mAppearTime--;
        return;
    }

    mRotation.y += mRotateVel / 100.0f;
    MR::repeatDegree(&mRotation.y);
    mCurrRot.setRotate(TVec3f(0, 1, 0), MR::toRadian(mRotation.y));
    mBloomModel->mPosition.set(mPosition);
    MR::startLevelSound(this, "SE_OJ_LV_PROMINENCE_T_MOVE");
    updateEffectClipping();

    if (MR::isValidSwitchAppear(this)) {
        if (!MR::isOnSwitchAppear(this)) {
            kill();
        }
    }
}

void LavaProminenceTriple::calcAndSetBaseMtx() {
    TQuat4f rot = mBaseRot;
    rot.mult(rot, mCurrRot);
    TPos3f mtx;
    mtx.zeroTrans();
    mtx.setQT(rot, mPosition);
    MR::setBaseTRMtx(this, mtx);
    mBloomMtx.set(mtx);
}

void LavaProminenceTriple::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    MR::sendMsgEnemyAttackFireStrong(pReceiver, pSender);
}

void LavaProminenceTriple::updateHitSensor(HitSensor* pSensor) {
    // issue with loading from secondary offset in table entries
    // load order swap
    // https://decomp.me/scratch/4nBud

    TVec3f diff = *MR::getPlayerCenterPos();
    diff -= mPosition;

    if (MR::isNearZero(diff)) {
        return;
    }

    f32 y = mSide.dot(diff);
    f32 x = mFront.dot(diff);
    f32 angle = MR::toDegree(MR::atan2(y, x));
    angle -= mRotation.y;
    MR::repeatDegree(&angle);

    const f32* sensor0 = &::sNoSensorTable[0];
    const f32* sensor1 = &::sNoSensorTable[1];

    for (int i = 0; i < 3; i++) {
        s32 index = i * 2;
        s32 nextIndex = index + 1;
        f32 half = (sensor0[index] + sensor0[nextIndex]) / 2.0f;

        if (angle < half) {
            int j;

            if (i == 0) {
                angle += 360.0f;
                j = 2;
            } else {
                j = i - 1;
            }

            angle = MR::clamp(angle, sensor1[j * 2 + 0], sensor1[j * 2 + 1]);
            break;
        }

        if (i == 2) {
            angle = MR::clamp(angle, sensor1[index], sensor1[nextIndex]);
            break;
        }
    }

    angle += mRotation.y;
    MR::repeatDegree(&angle);

    TVec3f offset = mFront * MR::cosDegree(angle);
    offset += mSide * MR::sinDegree(angle);
    offset *= mScale.y * ::sToSensorRadius;
    pSensor->mPosition.set(mPosition);
    pSensor->mPosition.add(offset);
}

void LavaProminenceTriple::updateEffectClipping() {
    updateEffectClippingIndividual(&mIsAppearDrop1, "Drop1", "Head1");

    if (!mIsEnvironment) {
        updateEffectClippingIndividual(&mIsAppearDrop2, "Drop2", "Head2");
        updateEffectClippingIndividual(&mIsAppearDrop3, "Drop3", "Head3");
    }
}

void LavaProminenceTriple::updateEffectClippingIndividual(bool* pIsAppear, const char* pEffectName, const char* pJointName) {
    TVec3f jointPos;
    MR::copyJointPos(this, pJointName, &jointPos);

    if (*pIsAppear == true) {
        if (MR::isJudgedToClipFrustum(jointPos, 1.0f)) {
            MR::deleteEffect(this, pEffectName);
            *pIsAppear = false;
        }
    } else {
        if (!MR::isJudgedToClipFrustum(jointPos, 1.0f)) {
            MR::emitEffect(this, pEffectName);
            *pIsAppear = true;
        }
    }
}
