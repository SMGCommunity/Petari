#include "Game/MapObj/LavaProminence.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StringUtil.hpp"

void LavaProminence_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)2.0f;
}

namespace {
    static const s32 sDefaultTimeWait = 180;
    static const s32 sTimeSign = 90;
    static const f32 sDefaultMoveSpeed = 20.0f;
    // sSubScale
    // sScaleMin
    // sSubNum
    // sExtraMoveLength
    // sDefaultCoordBeforeLanding
};  // namespace

namespace NrvLavaProminence {
    NEW_NERVE(HostTypeNrvWaitSwitch, LavaProminence, WaitSwitch);
    NEW_NERVE(HostTypeNrvWait, LavaProminence, Wait);
    NEW_NERVE(HostTypeNrvSign, LavaProminence, Sign);
    NEW_NERVE(HostTypeNrvMoveStartExtra, LavaProminence, MoveStartExtra);
    NEW_NERVE(HostTypeNrvMoveStart, LavaProminence, MoveStart);
    NEW_NERVE(HostTypeNrvMoveLoop, LavaProminence, MoveLoop);
    NEW_NERVE(HostTypeNrvMoveEnd, LavaProminence, MoveEnd);
    NEW_NERVE(HostTypeNrvMoveEndExtra, LavaProminence, MoveEndExtra);
};  // namespace NrvLavaProminence

LavaProminence::LavaProminence(const char* pName)
    : LiveActor(pName), mWaitTime(::sDefaultTimeWait), mMoveSpeed(::sDefaultMoveSpeed), mMoveRailCoord(), mMoveRailEndOffset(),
      mRailStartDir(0, 0, 0), mRailEndDir(0, 0, 0), mRailStartPos(0.0f), mRailEndPos(0.0f), _12C(0, 0, 0), mBloomModel(),
      _16C(0.0f, 0.0f, 0.0f, 1.0f), mRailDir(0, 0, 1), mEmitDropEffect(true) {
    mEffectStartMtx.identity();
    mEffectEndMtx.identity();
    mBloomModelMtx.identity();
    _18C.identity();
}

void LavaProminence::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    _16C.setEuler(mRotation);
    MR::getJMapInfoArg0NoInit(rIter, &mWaitTime);
    MR::getJMapInfoArg1NoInit(rIter, &mMoveSpeed);
    MR::getJMapInfoArg3NoInit(rIter, &mMoveRailEndOffset);
    MR::useStageSwitchReadA(this, rIter);
    const char* name = nullptr;
    MR::getObjectName(&name, rIter);
    bool isWithoutShadow = false;
    initModelManagerWithAnm("LavaProminence", nullptr, false);
    MR::startBtk(this, "LavaProminence");

    if (MR::isEqualString(name, "LavaProminenceWithoutShadow")) {
        isWithoutShadow = true;
    }

    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    TVec3f offset;
    offset.set(0.0f);
    MR::addHitSensorMapObjSimple(this, "body", 8, 120.0f, offset);
    initRailRider(rIter);
    MR::calcRailPosAtCoord(&mRailStartPos, this, 0.0f);
    MR::calcRailPosAtCoord(&mRailEndPos, this, MR::getRailTotalLength(this));
    MR::calcRailDirectionAtCoord(&mRailStartDir, this, 0.0f);
    MR::calcRailDirectionAtCoord(&mRailEndDir, this, MR::getRailTotalLength(this));
    initSound(6, false);
    MR::setGroupClipping(this, rIter, 16);
    MR::initAndSetRailClipping(&_12C, this, 300.0f, 300.0f);

    if (MR::isValidSwitchA(this)) {
        initNerve(&NrvLavaProminence::HostTypeNrvWaitSwitch::sInstance);
    } else {
        initNerve(&NrvLavaProminence::HostTypeNrvWait::sInstance);
    }

    initEffectKeeper(0, nullptr, false);
    MR::setEffectHostMtx(this, "Sign", mEffectStartMtx);
    MR::setEffectHostMtx(this, "Start", mEffectStartMtx);
    MR::setEffectHostMtx(this, "End", mEffectEndMtx);
    MR::setEffectName(this, "Drop", "DropEffect");

    if (!isWithoutShadow) {
        MR::initShadowVolumeSphere(this, 100.0f);
    }

    mBloomModel = MR::createBloomModel(this, mBloomModelMtx);
    MR::startBtk(mBloomModel, "LavaProminenceBloom");
    makeActorAppeared();
}

void LavaProminence::appear() {
    LiveActor::appear();
}

void LavaProminence::kill() {
    LiveActor::kill();
}

void LavaProminence::initAfterPlacement() {
    TVec3f initPosVec(mPosition);
    TVec3f upVec;

    mPosition.set(mRailStartPos);
    MR::calcGravityVectorOrZero(this, &upVec, nullptr, 0);

    if (MR::isNearZero(upVec)) {
        MR::calcUpVec(&upVec, this);
        upVec = -upVec;
    }

    MR::makeMtxUpNoSupportPos(&mEffectStartMtx, -upVec, mRailStartPos);
    mPosition.set(mRailEndPos);
    MR::calcGravityVectorOrZero(this, &upVec, nullptr, 0);

    if (MR::isNearZero(upVec)) {
        MR::calcUpVec(&upVec, this);
        upVec = -upVec;
    }

    MR::makeMtxUpNoSupportPos(&mEffectEndMtx, -upVec, mRailEndPos);
    mPosition.set(initPosVec);
}

void LavaProminence::startClipped() {
    LiveActor::startClipped();
    MR::deleteEffect(this, "DropEffect");
    mEmitDropEffect = true;
    MR::forceDeleteEffectAll(this);

    if (!isNerve(&NrvLavaProminence::HostTypeNrvWaitSwitch::sInstance)) {
        setNerve(&NrvLavaProminence::HostTypeNrvWait::sInstance);
    }
}

void LavaProminence::moveOnRail() {
    mMoveRailCoord += mMoveSpeed;
    MR::setRailCoord(this, MR::clamp(mMoveRailCoord, 0.0f, MR::getRailTotalLength(this)));
    mRailDir.set(MR::getRailDirection(this));
    MR::moveTransToCurrentRailPos(this);
    updateEffectClipping(isNrvMove());
    MR::calcGravity(this);
    setGravityAndMakeMtx();
}

void LavaProminence::setGravityAndMakeMtx() {
    MR::makeMtxUpNoSupportPos(&_18C, -mGravity, mPosition);

    // TODO: TRot3f::setScale inline?
    f32 z = mScale.z;
    f32 y = mScale.y;
    f32 x = mScale.x;
    _18C[0][1] = 0.0f;
    _18C[0][0] = x;
    _18C[0][2] = 0.0f;
    _18C[1][0] = 0.0f;
    _18C[1][1] = y;
    _18C[1][2] = 0.0f;
    _18C[2][0] = 0.0f;
    _18C[2][1] = 0.0f;
    _18C[2][2] = z;
}

void LavaProminence::updateEffectClipping(bool a1) {
    if (!mEmitDropEffect) {
        if (!a1 || MR::isJudgedToClipFrustum(mPosition, getSensor("body")->getRadius())) {
            MR::deleteEffect(this, "DropEffect");
            mEmitDropEffect = true;
        }
    } else if (a1 && !MR::isJudgedToClipFrustum(mPosition, getSensor("body")->getRadius())) {
        MR::emitEffect(this, "DropEffect");
        mEmitDropEffect = false;
    }
}

void LavaProminence::exeWaitSwitch() {
    if (MR::isFirstStep(this)) {
        MR::hideModel(this);
        MR::deleteEffect(this, "DropEffect");
        mEmitDropEffect = true;
    }

    if (MR::isOnSwitchA(this)) {
        setNerve(&NrvLavaProminence::HostTypeNrvWait::sInstance);
    }
}

void LavaProminence::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::hideModel(this);
        MR::deleteEffect(this, "DropEffect");
        mEmitDropEffect = true;
        mMoveRailCoord = 0.0f;
        MR::setRailCoord(this, 0.0f);
        MR::moveTransToCurrentRailPos(this);
    }

    if (MR::isValidSwitchA(this) && !MR::isOnSwitchA(this)) {
        setNerve(&NrvLavaProminence::HostTypeNrvWaitSwitch::sInstance);
    } else if (MR::isStep(this, mWaitTime)) {
        setNerve(&NrvLavaProminence::HostTypeNrvSign::sInstance);
    }
}

void LavaProminence::exeSign() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "Sign");
        MR::hideModel(this);
        MR::deleteEffect(this, "DropEffect");
        mEmitDropEffect = true;
    }

    MR::startLevelSound(this, "SE_OJ_LV_PROMINENCE_SIGN");

    if (MR::isStep(this, ::sTimeSign)) {
        MR::deleteEffect(this, "Sign");
        setNerve(&NrvLavaProminence::HostTypeNrvMoveStartExtra::sInstance);
    }
}

void LavaProminence::exeMoveStartExtra() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "Start");
        MR::startSound(this, "SE_OJ_PROMINENCE_START");
        mMoveRailCoord = 0.0f;
        MR::setRailCoord(this, 0.0f);
        mPosition.set(mRailStartPos - mRailStartDir * 300.0f);
        mRailDir.set(mRailStartDir);
        MR::showModel(this);
        MR::emitEffect(this, "DropEffect");
        mEmitDropEffect = false;
    }

    MR::startLevelSound(this, "SE_OJ_LV_PROMINENCE_SIGN");
    MR::startLevelSound(this, "SE_OJ_LV_PROMINENCE_MOVE");
    mPosition.add(mRailStartDir * mMoveSpeed);
    MR::calcGravity(this);
    setGravityAndMakeMtx();
    s32 endStartStep = 300.0f / mMoveSpeed;

    if (MR::isGreaterStep(this, endStartStep - 1)) {
        MR::deleteEffect(this, "Start");
        setNerve(&NrvLavaProminence::HostTypeNrvMoveLoop::sInstance);
    }
}

void LavaProminence::exeMoveStart() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "Start");
        MR::startSound(this, "SE_OJ_PROMINENCE_START");
        mMoveRailCoord = 0.0f;
        MR::setRailCoord(this, 0.0f);
        MR::moveTransToCurrentRailPos(this);
        MR::showModel(this);
        MR::emitEffect(this, "DropEffect");
        mEmitDropEffect = false;
    }

    MR::startLevelSound(this, "SE_OJ_LV_PROMINENCE_SIGN");
    MR::startLevelSound(this, "SE_OJ_LV_PROMINENCE_MOVE");
    moveOnRail();

    if (mMoveRailCoord >= 300.0f) {
        MR::deleteEffect(this, "Start");
        setNerve(&NrvLavaProminence::HostTypeNrvMoveLoop::sInstance);
    }
}

void LavaProminence::exeMoveLoop() {
    MR::startLevelSound(this, "SE_OJ_LV_PROMINENCE_MOVE");
    moveOnRail();

    if (mMoveRailCoord >= MR::getRailTotalLength(this) - mMoveRailEndOffset) {
        setNerve(&NrvLavaProminence::HostTypeNrvMoveEndExtra::sInstance);
    }
}

void LavaProminence::exeMoveEnd() {
    if (MR::isFirstStep(this)) {
        MR::setRailCoord(this, mMoveRailCoord);
        MR::emitEffect(this, "End");
    }

    MR::startLevelSound(this, "SE_OJ_LV_PROMINENCE_MOVE");
    MR::startLevelSound(this, "SE_OJ_LV_PROMINENCE_END");
    moveOnRail();

    if (mMoveRailCoord >= MR::getRailTotalLength(this)) {
        setNerve(&NrvLavaProminence::HostTypeNrvMoveEndExtra::sInstance);
    }
}

void LavaProminence::exeMoveEndExtra() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "End");
    }

    MR::startLevelSound(this, "SE_OJ_LV_PROMINENCE_MOVE");
    MR::startLevelSound(this, "SE_OJ_LV_PROMINENCE_END");
    mPosition.add(mRailEndDir * mMoveSpeed);

    if (90000.0f < mRailEndPos.squared(mPosition)) {
        MR::hideModel(this);
        MR::deleteEffect(this, "DropEffect");
        mEmitDropEffect = true;
        MR::deleteEffect(this, "End");
        setNerve(&NrvLavaProminence::HostTypeNrvWait::sInstance);
    }
}

void LavaProminence::calcAndSetBaseMtx() {
    TPos3f mtx;
    TVec3f vec(mRailDir);
    TVec3f front;
    TQuat4f quat;

    MR::normalizeOrZero(&vec);

    if (!MR::isNearZero(vec)) {
        _16C.getZDir(front);
        quat.setRotate(front, vec);
        _16C.mult(quat);
    }

    mtx.makeQuat(_16C);
    mtx.setTrans(mPosition);
    MR::setBaseTRMtx(this, mtx);
    mBloomModelMtx.set(mtx);
}

void LavaProminence::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayerOrRide(pReceiver)) {
        MR::sendMsgEnemyAttackFireStrong(pReceiver, pSender);
    }
}

bool LavaProminence::isNrvMove() {
    return isNerve(&NrvLavaProminence::HostTypeNrvMoveStartExtra::sInstance) || isNerve(&NrvLavaProminence::HostTypeNrvMoveLoop::sInstance) ||
           isNerve(&NrvLavaProminence::HostTypeNrvMoveEndExtra::sInstance);
}
