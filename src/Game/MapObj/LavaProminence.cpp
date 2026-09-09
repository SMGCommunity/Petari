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
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

void LavaProminence_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)2.0f;
}

namespace {
    // sDefaultTimeWait
    // sTimeSign
    // sDefaultMoveSpeed
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
    : LiveActor(pName), mStepAppearance(180), mMovementSpeed(20.0f), _94(), mEndPathOffset(), mRailDir(0, 0, 0), mRailDirEnd(0, 0, 0), _B4(0.0f),
      _C0(0.0f), _12C(0, 0, 0), mBloomModel(), _16C(0.0f, 0.0f, 0.0f, 1.0f), _17C(0, 0, 1), mEmitDropEffect(true) {
    _CC.identity();
    _FC.identity();
    _13C.identity();
    _18C.identity();
}

void LavaProminence::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    _16C.setEuler(mRotation);
    MR::getJMapInfoArg0NoInit(rIter, &mStepAppearance);
    MR::getJMapInfoArg1NoInit(rIter, &mMovementSpeed);
    MR::getJMapInfoArg3NoInit(rIter, &mEndPathOffset);
    MR::useStageSwitchReadA(this, rIter);
    const char* name = nullptr;
    MR::getObjectName(&name, rIter);
    bool isEqualString = false;
    initModelManagerWithAnm("LavaProminence", nullptr, false);
    MR::startBtk(this, "LavaProminence");

    if (MR::isEqualString(name, "LavaProminenceWithoutShadow")) {
        isEqualString = true;
    }

    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    TVec3f offset;
    offset.set(0.0f);
    MR::addHitSensorMapObjSimple(this, "body", 8, 120.0f, offset);
    initRailRider(rIter);
    MR::calcRailPosAtCoord(&_B4, this, 0.0f);
    MR::calcRailPosAtCoord(&_C0, this, MR::getRailTotalLength(this));
    MR::calcRailDirectionAtCoord(&mRailDir, this, 0.0f);
    MR::calcRailDirectionAtCoord(&mRailDirEnd, this, MR::getRailTotalLength(this));
    initSound(6, false);
    MR::setGroupClipping(this, rIter, 16);
    MR::initAndSetRailClipping(&_12C, this, 300.0f, 300.0f);

    if (MR::isValidSwitchA(this)) {
        initNerve(&NrvLavaProminence::HostTypeNrvWaitSwitch::sInstance);
    } else {
        initNerve(&NrvLavaProminence::HostTypeNrvWait::sInstance);
    }

    initEffectKeeper(0, nullptr, false);
    MR::setEffectHostMtx(this, "Sign", _CC);
    MR::setEffectHostMtx(this, "Start", _CC);
    MR::setEffectHostMtx(this, "End", _FC);
    MR::setEffectName(this, "Drop", "DropEffect");

    if (!isEqualString) {
        MR::initShadowVolumeSphere(this, 100.0f);
    }

    mBloomModel = MR::createBloomModel(this, _13C);
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

    mPosition.set(_B4);
    MR::calcGravityVectorOrZero(this, &upVec, nullptr, 0);

    if (MR::isNearZero(upVec, 0.001)) {
        MR::calcUpVec(&upVec, this);
        upVec = -upVec;
    }

    MR::makeMtxUpNoSupportPos(&_CC, -upVec, _B4);
    mPosition.set(_C0);
    MR::calcGravityVectorOrZero(this, &upVec, nullptr, 0);

    if (MR::isNearZero(upVec, 0.001f)) {
        MR::calcUpVec(&upVec, this);
        upVec = -upVec;
    }

    MR::makeMtxUpNoSupportPos(&_FC, -upVec, _C0);
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
    _94 += mMovementSpeed;
    MR::setRailCoord(this, MR::clamp(_94, 0.0f, MR::getRailTotalLength(this)));
    _17C.set(MR::getRailDirection(this));
    MR::moveTransToCurrentRailPos(this);
    updateEffectClipping(isNrvExtra());
    MR::calcGravity(this);
    setGravityAndMakeMtx();
}

void LavaProminence::setGravityAndMakeMtx() {
    MR::makeMtxUpNoSupportPos(&_18C, -mGravity, mPosition);
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
        if (!a1 || MR::isJudgedToClipFrustum(mPosition, getRadius("body"))) {
            MR::deleteEffect(this, "DropEffect");
            mEmitDropEffect = true;
        }
    } else if (a1 && !MR::isJudgedToClipFrustum(mPosition, getRadius("body"))) {
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
        _94 = 0.0f;
        MR::setRailCoord(this, 0.0f);
        MR::moveTransToCurrentRailPos(this);
    }

    if (MR::isValidSwitchA(this) && !MR::isOnSwitchA(this)) {
        setNerve(&NrvLavaProminence::HostTypeNrvWaitSwitch::sInstance);
    } else if (MR::isStep(this, mStepAppearance)) {
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

    if (MR::isStep(this, 90)) {
        MR::deleteEffect(this, "Sign");
        setNerve(&NrvLavaProminence::HostTypeNrvMoveStartExtra::sInstance);
    }
}

void LavaProminence::exeMoveStartExtra() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "Start");
        MR::startSound(this, "SE_OJ_PROMINENCE_START");
        _94 = 0.0f;
        MR::setRailCoord(this, 0.0f);
        mPosition.set(_B4 - mRailDir * 300.0f);
        _17C.set(mRailDir);
        MR::showModel(this);
        MR::emitEffect(this, "DropEffect");
        mEmitDropEffect = false;
    }

    MR::startLevelSound(this, "SE_OJ_LV_PROMINENCE_SIGN");
    MR::startLevelSound(this, "SE_OJ_LV_PROMINENCE_MOVE");
    mPosition.add(mRailDir * mMovementSpeed);
    MR::calcGravity(this);
    setGravityAndMakeMtx();
    s32 endStartStep = 300.0f / mMovementSpeed;

    if (MR::isGreaterStep(this, endStartStep - 1)) {
        MR::deleteEffect(this, "Start");
        setNerve(&NrvLavaProminence::HostTypeNrvMoveLoop::sInstance);
    }
}

void LavaProminence::exeMoveStart() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "Start");
        MR::startSound(this, "SE_OJ_PROMINENCE_START");
        _94 = 0.0f;
        MR::setRailCoord(this, 0.0f);
        MR::moveTransToCurrentRailPos(this);
        MR::showModel(this);
        MR::emitEffect(this, "DropEffect");
        mEmitDropEffect = false;
    }

    MR::startLevelSound(this, "SE_OJ_LV_PROMINENCE_SIGN");
    MR::startLevelSound(this, "SE_OJ_LV_PROMINENCE_MOVE");
    moveOnRail();

    if (_94 >= 300.0f) {
        MR::deleteEffect(this, "Start");
        setNerve(&NrvLavaProminence::HostTypeNrvMoveLoop::sInstance);
    }
}

void LavaProminence::exeMoveLoop() {
    MR::startLevelSound(this, "SE_OJ_LV_PROMINENCE_MOVE");
    moveOnRail();

    if (_94 >= MR::getRailTotalLength(this) - mEndPathOffset) {
        setNerve(&NrvLavaProminence::HostTypeNrvMoveEndExtra::sInstance);
    }
}

void LavaProminence::exeMoveEnd() {
    if (MR::isFirstStep(this)) {
        MR::setRailCoord(this, _94);
        MR::emitEffect(this, "End");
    }

    MR::startLevelSound(this, "SE_OJ_LV_PROMINENCE_MOVE");
    MR::startLevelSound(this, "SE_OJ_LV_PROMINENCE_END");
    moveOnRail();

    if (_94 >= MR::getRailTotalLength(this)) {
        setNerve(&NrvLavaProminence::HostTypeNrvMoveEndExtra::sInstance);
    }
}

void LavaProminence::exeMoveEndExtra() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "End");
    }

    MR::startLevelSound(this, "SE_OJ_LV_PROMINENCE_MOVE");
    MR::startLevelSound(this, "SE_OJ_LV_PROMINENCE_END");
    mPosition.add(mRailDirEnd * mMovementSpeed);

    if (90000.0f < _C0.squared(mPosition)) {
        MR::hideModel(this);
        MR::deleteEffect(this, "DropEffect");
        mEmitDropEffect = true;
        MR::deleteEffect(this, "End");
        setNerve(&NrvLavaProminence::HostTypeNrvWait::sInstance);
    }
}

void LavaProminence::calcAndSetBaseMtx() {
    TPos3f pos;
    TVec3f vec(_17C);
    TVec3f v1;
    TQuat4f quad;

    MR::normalizeOrZero(&vec);
    if (!MR::isNearZero(vec, 0.001f)) {
        _16C.getZDir(v1);
        quad.setRotate(v1, vec);
        _16C.mult(quad);
    }

    pos.makeQuat(_16C);
    pos[0][3] = mPosition.x;
    pos[1][3] = mPosition.y;
    pos[2][3] = mPosition.z;
    MR::setBaseTRMtx(this, pos);
    _13C.setInline(pos);
}

void LavaProminence::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayerOrRide(pReceiver)) {
        MR::sendMsgEnemyAttackFireStrong(pReceiver, pSender);
    }
}

inline bool LavaProminence::isNrvExtra() {
    return isNerve(&NrvLavaProminence::HostTypeNrvMoveStartExtra::sInstance) || isNerve(&NrvLavaProminence::HostTypeNrvMoveLoop::sInstance) ||
           isNerve(&NrvLavaProminence::HostTypeNrvMoveEndExtra::sInstance);
}

inline f32 LavaProminence::getRadius(const char* pHitSensor) const {
    return getSensor(pHitSensor)->mRadius;
}
