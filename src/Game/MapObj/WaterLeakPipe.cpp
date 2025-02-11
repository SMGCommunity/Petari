#include "Game/MapObj/WaterLeakPipe.hpp"
 
IceStepNoSlip::~IceStepNoSlip() {

}

WaterLeakPipe::~WaterLeakPipe() {

}


IceStepNoSlip::IceStepNoSlip(MtxPtr mtx) : ModelObj("アイス床", "IceStepNoSlip", mtx, 0x1A, -2, -2, false) {

}

void IceStepNoSlip::init(const JMapInfoIter &rIter) {
    ModelObj::init(rIter);
    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    MR::initCollisionParts(this, "IceStepNoSlip", getSensor(nullptr), nullptr);
    initNerve(&NrvIceStepNoSlip::IceStepNoSlipNrvAppear::sInstance);
}

void IceStepNoSlip::appear() {
    LiveActor::appear();
    setNerve(&NrvIceStepNoSlip::IceStepNoSlipNrvAppear::sInstance);
}

void IceStepNoSlip::exeBreak() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "End", nullptr);
    }

    if (MR::isBckStopped(this)) {
        kill();
    }
}

namespace NrvIceStepNoSlip {
    INIT_NERVE(IceStepNoSlipNrvAppear);
    INIT_NERVE(IceStepNoSlipNrvBreak);
};

WaterLeakPipe::WaterLeakPipe(const char *pName) : LiveActor(pName) {
    mIceStep = nullptr;
    mPipeHeight = 500.0f;
    mTopMtx = nullptr;
    mBottomMtx = nullptr;
    _9C.x = 0.0f;
    _9C.y = 0.0f;
    _9C.z = 0.0f;
}

void WaterLeakPipe::init(const JMapInfoIter &rIter) {
    TVec3f upVec, offs;
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("WaterLeakPipe", nullptr, false);
    mTopMtx = MR::getJointMtx(this, "Top");
    mBottomMtx = MR::getJointMtx(this, "Bottom");
    MR::getJMapInfoArg0NoInit(rIter, &mPipeHeight);
    initPipeHeight();
    MR::connectToSceneMapObjNoCalcAnim(this);
    initHitSensor(1);
    offs.x = 0.0f;
    offs.y = 0.0f;
    offs.z = 0.0f;
    MR::addHitSensorAtJointMapObj(this, "ice", "Top", 8, 120.0f, offs);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    MR::calcUpVec(&upVec, this);
    JMAVECScaleAdd(&upVec, &mPosition, &_9C, (0.5f * mPipeHeight));
    MR::setClippingTypeSphere(this, (0.5f * mPipeHeight), &_9C);
    mIceStep = new IceStepNoSlip(mTopMtx);
    mIceStep->initWithoutIter();
    mIceStep->makeActorDead();
    initNerve(&NrvWaterLeakPipe::WaterLeakPipeNrvWait::sInstance);
    makeActorAppeared();
}

void WaterLeakPipe::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "Splash");
    }
    
    MR::startLevelSound(this, "SE_OJ_LV_WATER_LEAK", -1, -1, -1);
}

void WaterLeakPipe::exeFreeze() {
    if (MR::isFirstStep(this) && MR::isDead(mIceStep)) {
        MR::forceDeleteEffect(this, "Splash");
        mIceStep->appear();
        MR::startSound(this, "SE_OJ_ICE_FLOOR_FREEZE", -1, -1);
        MR::invalidateClipping(this);
    }
    else if (MR::isOnPlayer(mIceStep) && MR::isPlayerElementModeIce()) {
        setNerve(&NrvWaterLeakPipe::WaterLeakPipeNrvFreeze::sInstance);
    }
    else if (MR::isStep(this, 15)) {
        mIceStep->setNerve(&NrvIceStepNoSlip::IceStepNoSlipNrvBreak::sInstance);
        MR::startSound(this, "SE_OJ_ICE_FLOOR_MELT", -1, -1);
        MR::validateClipping(this);
        setNerve(&NrvWaterLeakPipe::WaterLeakPipeNrvWait::sInstance);
    }
}

/*
bool WaterLeakPipe::receiveMsgPlayerAttack(u32 msg, HitSensor *a1, HitSensor *a2) {
    if (!MR::isPlayerElementModeIce()) {
        return false;
    }

    if (isNerve(&NrvWaterLeakPipe::WaterLeakPipeNrvFreeze::sInstance)) {
        return false;
    }

    if (MR::isMsgPlayerTrample(msg) || MR::isMsgPlayerHipDrop(msg)) {
        TVec3f upVec;
        MR::calcUpVec(&upVec, this);
        TVec3f v8;
        v8.subInline2(*MR::getPlayerPos(), mPosition);

        if (v8.dot(upVec) < 0.0f) {
            return false;
        }

        if (MR::getPlayerVelocity()->dot(upVec) >= 0.0f) {
            return false;
        }

        setNerve(&NrvWaterLeakPipe::WaterLeakPipeNrvFreeze::sInstance);
        return false;
    }

    return false;
}
*/

void WaterLeakPipe::initPipeHeight() {
    TMtx34f mtx;
    mtx.setInline(mTopMtx);
    TVec3f upVec;
    MR::calcUpVec(&upVec, this);
    TVec3f v10;
    JMAVECScaleAdd(&upVec, &mPosition, &v10, mPipeHeight);
    mtx.mMtx[0][3] = v10.x;
    mtx.mMtx[1][3] = v10.y;
    mtx.mMtx[2][3] = v10.z;
    PSMTXCopy(mtx.toMtxPtr(), mTopMtx);
    calcAndSetBaseMtx();
}

namespace NrvWaterLeakPipe {
    INIT_NERVE(WaterLeakPipeNrvWait);
    INIT_NERVE(WaterLeakPipeNrvFreeze);
};

void WaterLeakPipe::calcAnim() {

}
