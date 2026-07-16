#include "Game/MapObj/WaterLeakPipe.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util.hpp"

void WaterLeakPipe_FORCE_MATCH_SDATA2() {
    (void)0.0f;
    (void)0.5f;
}

namespace {
    static const f32 sDefaultHeight = 500.0f;
    static const s32 sLifeTime = 15;
};  // namespace

namespace NrvIceStepNoSlip {
    NEW_NERVE(IceStepNoSlipNrvAppear, IceStepNoSlip, Appear);
    NEW_NERVE(IceStepNoSlipNrvBreak, IceStepNoSlip, Break);
};  // namespace NrvIceStepNoSlip

namespace NrvWaterLeakPipe {
    NEW_NERVE(WaterLeakPipeNrvWait, WaterLeakPipe, Wait);
    NEW_NERVE(WaterLeakPipeNrvFreeze, WaterLeakPipe, Freeze);
};  // namespace NrvWaterLeakPipe

IceStepNoSlip::~IceStepNoSlip() {
}

WaterLeakPipe::~WaterLeakPipe() {
}

IceStepNoSlip::IceStepNoSlip(MtxPtr mtx) : ModelObj("アイス床", "IceStepNoSlip", mtx, MR::DrawBufferType_IndirectMapObjStrongLight, -2, -2, false) {
}

void IceStepNoSlip::init(const JMapInfoIter& rIter) {
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

void IceStepNoSlip::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Start", nullptr);
    }
}

void IceStepNoSlip::exeBreak() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "End", nullptr);
    }

    if (MR::isBckStopped(this)) {
        kill();
    }
}

WaterLeakPipe::WaterLeakPipe(const char* pName)
    : LiveActor(pName), mIceStep(), mPipeHeight(::sDefaultHeight), mTopMtx(), mBottomMtx(), _9C(0.0f, 0.0f, 0.0f) {
}

void WaterLeakPipe::init(const JMapInfoIter& rIter) {
    TVec3f upVec;
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("WaterLeakPipe", nullptr, false);
    mTopMtx = MR::getJointMtx(this, "Top");
    mBottomMtx = MR::getJointMtx(this, "Bottom");
    MR::getJMapInfoArg0NoInit(rIter, &mPipeHeight);
    initPipeHeight();
    MR::connectToSceneMapObjNoCalcAnim(this);
    initHitSensor(1);
    MR::addHitSensorAtJointMapObj(this, "ice", "Top", 8, 120.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    MR::calcUpVec(&upVec, this);
    _9C.scaleAdd(0.5f * mPipeHeight, upVec, mPosition);
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

    MR::startLevelSound(this, "SE_OJ_LV_WATER_LEAK");
}

void WaterLeakPipe::exeFreeze() {
    if (MR::isFirstStep(this) && MR::isDead(mIceStep)) {
        MR::forceDeleteEffect(this, "Splash");
        mIceStep->appear();
        MR::startSound(this, "SE_OJ_ICE_FLOOR_FREEZE");
        MR::invalidateClipping(this);
    } else if (MR::isOnPlayer(mIceStep) && MR::isPlayerElementModeIce()) {
        setNerve(&NrvWaterLeakPipe::WaterLeakPipeNrvFreeze::sInstance);
    } else if (MR::isStep(this, ::sLifeTime)) {
        mIceStep->setNerve(&NrvIceStepNoSlip::IceStepNoSlipNrvBreak::sInstance);
        MR::startSound(this, "SE_OJ_ICE_FLOOR_MELT");
        MR::validateClipping(this);
        setNerve(&NrvWaterLeakPipe::WaterLeakPipeNrvWait::sInstance);
    }
}

bool WaterLeakPipe::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
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
        v8.sub(*MR::getPlayerPos(), mPosition);

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

void WaterLeakPipe::initPipeHeight() {
    TPos3f mtx;
    mtx.set(mTopMtx);
    TVec3f upVec;
    MR::calcUpVec(&upVec, this);
    TVec3f v10;
    v10.scaleAdd(mPipeHeight, upVec, mPosition);
    mtx.setTrans(v10);
    PSMTXCopy(mtx, mTopMtx);
    calcAndSetBaseMtx();
}
