#include "Game/MapObj/LavaProminenceTriple.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Util.hpp"

LavaProminenceTriple::LavaProminenceTriple(const char* pName)
    : LiveActor(pName), mBloomModel(), _C0(0, 0, 0, 1), _D0(0, 0, 0, 1), _E0(0, 0, 1), _EC(1, 0, 0) {
    _FC = 0;
    mArg0 = 100.0f;
    _FD = 0;
    _FE = 0;
    _FF = 0;
    mArg1 = 0;
    _90.identity();
}

void LavaProminenceTriple::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    TQuat4f v7;
    v7.setEulerDegree(mRotation.x, mRotation.y, mRotation.z);
    v7.getZDir(_EC);
    v7.getXDir(_E0);
    _C0 = v7;

    MR::getJMapInfoArg0NoInit(rIter, &mArg0);
    MR::getJMapInfoArg1NoInit(rIter, &mArg1);
    _104 = mArg1;

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
    }

    const char* objName = nullptr;
    MR::getObjectName(&objName, rIter);

    if (MR::isEqualString(objName, "LavaProminenceEnvironment")) {
        initModelManagerWithAnm("LavaProminenceEnvironment", nullptr, false);
        MR::startBtk(this, "LavaProminenceEnvironment");
        _FF = 1;
        mBloomModel = MR::createBloomModel(this, _90);
        MR::startBtk(mBloomModel, "LavaProminenceEnvironmentBloom");
    } else {
        initModelManagerWithAnm("LavaProminenceTriple", nullptr, false);
        MR::startBtk(this, "LavaProminenceTriple");
        _FF = 0;
        mBloomModel = MR::createBloomModel(this, _90);
    }

    initSound(4, false);
    MR::connectToSceneMapObj(this);

    if (!_FF) {
        initHitSensor(1);
        MR::addHitSensorCallbackMapObj(this, "attack", 1, 80.0f * mScale.y);
    }

    initEffectKeeper(0, nullptr, false);
    MR::setGroupClipping(this, rIter, 16);

    if (_FF) {
        MR::setClippingTypeSphere(this, 2100.0f * mScale.y);
    } else {
        MR::setClippingTypeSphere(this, 1900.0f * mScale.y);
    }

    if (MR::isValidSwitchAppear(this)) {
        MR::invalidateClipping(this);
    }

    makeActorAppeared();
}

void LavaProminenceTriple::appear() {
    LiveActor::appear();
    _FC = 1;
    _104 = mArg1;
    MR::emitEffect(this, "Drop1");

    if (!_FF) {
        _FD = 1;
        _FE = 1;
        MR::emitEffect(this, "Drop2");
        MR::emitEffect(this, "Drop3");
    }
}

/*
void LavaProminenceTriple::control() {
    if (_104 <= 0) {
        f32 rot = (mRotation.y + (mArg0 / 100.0f));
        f32* ptr = &mRotation.y;
        *ptr = rot;
        MR::repeatDegree(ptr);
        f32 v5 = RAD_TO_DEG(mRotation.y) / 2.0f;
        _D0.scale(sin(v5), TVec3f(0, 1, 0));
        f32 v7 = cos(v5);
        TVec3f* pPos = &mBloomModel->mPosition;
        _D0.w = v7;
        pPos->set< f32 >(mPosition);
        MR::startLevelSound(this, "SE_OJ_LV_PROMINENCE_T_MOVE");
        updateEffectClipping();

        if (MR::isValidSwitchAppear(this)) {
            if (!MR::isOnSwitchAppear(this)) {
                kill();
            }
        }
    } else {
        _104--;
    }
}
*/

void LavaProminenceTriple::calcAndSetBaseMtx() {
    TQuat4f v10 = _C0;
    PSQUATMultiply(&v10, &_D0, &v10);
    TPos3f mtx;
    mtx.zeroTrans();
    mtx.setQT(v10, mPosition);
    MR::setBaseTRMtx(this, mtx);
    _90.set(mtx);
}

void LavaProminenceTriple::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    MR::sendMsgEnemyAttackFireStrong(pReceiver, pSender);
}

// LavaProminenceTriple::updateHitSensor

void LavaProminenceTriple::updateEffectClipping() {
    updateEffectClippingIndividual(&_FC, "Drop1", "Head1");

    if (!_FF) {
        updateEffectClippingIndividual(&_FD, "Drop2", "Head2");
        updateEffectClippingIndividual(&_FE, "Drop3", "Head3");
    }
}

void LavaProminenceTriple::updateEffectClippingIndividual(bool* a1, const char* a2, const char* a3) {
    TVec3f jointPos;
    MR::copyJointPos(this, a3, &jointPos);
    if (*a1 == 1) {
        if (MR::isJudgedToClipFrustum(jointPos, 1.0f)) {
            MR::deleteEffect(this, a2);
            *a1 = false;
        }
    } else if (!MR::isJudgedToClipFrustum(jointPos, 1.0f)) {
        MR::emitEffect(this, a2);
        *a1 = true;
    }
}

LavaProminenceTriple::~LavaProminenceTriple() {
}
