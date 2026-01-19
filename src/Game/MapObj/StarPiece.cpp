#include "Game/MapObj/StarPiece.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/MapObj/PowerStar.hpp"
#include "Game/MapObj/StarPieceDirector.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/TriangleFilter.hpp"
#include "JSystem/J3DGraphBase/J3DStruct.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/gx/GXEnum.h"
#include "revolution/gx/GXStruct.h"
#include "revolution/mtx.h"
#include "revolution/types.h"
#include <JSystem/JMath/JMATrigonometric.hpp>

const GXColor effectColors[] = {{0x00, 0x55, 0xff, 0xFF}, {0xFF, 0xFF, 0x00, 0xFF}, {0x55, 0xFF, 0x00, 0xFF}, {0x7F, 0x7F, 0x00, 0xFF}};
const GXColor initColors[] = {{0x80, 0x00, 0x99, 0xFF}, {0xE6, 0xA0, 0x00, 0xFF}, {0x46, 0xA1, 0x08, 0xFF},
                              {0x37, 0x5A, 0xA0, 0xFF}, {0xBE, 0x33, 0x0B, 0xFF}, {0x80, 0x80, 0x80, 0xFF}};

J3DGXColor defaultCol = {0, 0, 0, 0xFF};

namespace NrvStarPiece {
    NEW_NERVE(HostTypeNrvFloating, StarPiece, Floating);
    NEW_NERVE(HostTypeNrvHop, StarPiece, Hop);
    NEW_NERVE(HostTypeNrvFall, StarPiece, Fall);
    NEW_NERVE(HostTypeNrvFallAfterReflect, StarPiece, Fall);
    NEW_NERVE(HostTypeNrvToTarget, StarPiece, ToTarget);
    NEW_NERVE(HostTypeNrvSpinDrained, StarPiece, SpinDrained);
    NEW_NERVE(HostTypeNrvToPlayerEnd, StarPiece, ToPlayerEnd);
    NEW_NERVE_ONEND(HostTypeNrvThrow, StarPiece, Throw, Throw);
    NEW_NERVE_ONEND(HostTypeNrvThrowNoFall, StarPiece, Throw, Throw);
    NEW_NERVE_ONEND(HostTypeNrvThrowFall, StarPiece, ThrowFall, Throw);
    NEW_NERVE(HostTypeNrvFollowPlayer, StarPiece, FollowPlayer);
    NEW_NERVE(HostTypeNrvRailMove, StarPiece, RailMove);

}  // namespace NrvStarPiece

StarPiece::StarPiece(const char* pName)
    : LiveActor(pName), _8C(0, 0, 1), _98(0.02f), _9C(0.0f), _A0(0.0f), _A4(0.0f), _A8(0, 0, 0), _B4(0, 0, 0), mDelegator(nullptr), mTargetSensor(nullptr),
      _C8(-1), mGettableDelayCounter(-1), mFallKillTimer(0), mColor(defaultCol), mGroupType(groupType_noGroup), mHostInfo(nullptr), mReceiverInfo(0),
      mNumGift(1) {
    MR::incNumStarPieceNewed();
    mFlags.isGoToPlayer = false;
    mFlags._1 = true;
    mFlags._2 = false;
    mFlags._3 = false;
    mFlags.isGroup = false;
    mFlags.InWater = false;
    mFlags._6 = false;
    mFlags.isLaunched = false;
}

void StarPiece::initAndSetFloatingFromGroup(const JMapInfoIter& rIter) {
    mGroupType = groupType_FloatingGroup;
    MR::incNumStarPieceGettable(0);
    init(rIter);
    MR::invalidateClipping(this);
    mFlags.isGroup = true;
}

void StarPiece::initAndSetRailMoveFromGroup(const JMapInfoIter& rIter) {
    mGroupType = groupType_RailMoveGroup;
    MR::incNumStarPieceGettable(0);
    init(rIter);
    MR::invalidateClipping(this);
    mFlags.isGroup = true;
}

void StarPiece::init(const JMapInfoIter& rIter) {
    s32 arg3 = -1;

    if (MR::isValidInfo(rIter)) {
        if (mGroupType == groupType_noGroup) {
            MR::incNumStarPieceGettable(0);
        }
        MR::initDefaultPos(this, rIter);
        MR::getJMapInfoArg3NoInit(rIter, &arg3);
        // unused
        const char* objName = nullptr;
        MR::getObjectName(&objName, rIter);
    }
    s32 numInitColors = sizeof(initColors) / sizeof(initColors[0]);
    if (arg3 >= 0 && arg3 < numInitColors) {
        mColor = initColors[arg3];
    } else {
        mColor = initColors[MR::getRandom(0, numInitColors)];
    }

    initModelManagerWithAnm("StarPiece", nullptr, true);
    MR::initDLMakerMatColor0(this, "StarPiece1_v", &mColor);
    MR::newDifferedDLBuffer(this);
    MR::connectToSceneNoSilhouettedMapObj(this);
    f32 flt = 30.0f;
    mScale.set(1.0f);
    initBinder(40.0f, 0.0f, 0);

    switch (mGroupType) {
    case groupType_noGroup:
    case groupType_FloatingGroup:
        initNerve(&NrvStarPiece::HostTypeNrvFloating::sInstance);
        break;
    case groupType_RailMoveGroup:
        initNerve(&NrvStarPiece::HostTypeNrvRailMove::sInstance);
        break;
    }
    initEffectKeeper(0, "StarPiece", false);
    initSound(4, false);
    initHitSensor(2);
    MR::addHitSensorEye(this, "attack", 16, flt, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensor(this, "body", ATYPE_STAR_PIECE, 16, flt, TVec3f(0.0f, 0.0f, 0.0f));
    MR::initShadowVolumeSphere(this, flt);
    MR::offCalcShadow(this, nullptr);
    MR::offCalcShadowDropPrivateGravity(this, nullptr);
    MR::onCalcShadowOneTime(this, nullptr);
    MR::onCalcShadowDropPrivateGravityOneTime(this, nullptr);

    if (MR::isValidInfo(rIter)) {
        s32 arg4 = -1;
        MR::getJMapInfoArg4NoInit(rIter, &arg4);
        if (arg4 == 0) {
            MR::onCalcShadow(this, nullptr);
            MR::onCalcShadowDropPrivateGravity(this, nullptr);
            mFlags._2 = true;
            mFlags._3 = false;
        } else if (arg4 == -1) {
            mFlags._2 = false;
            mFlags._3 = false;
        } else if (arg4 == 1) {
            mFlags._2 = false;
            mFlags._3 = true;
            MR::invalidateShadow(this, nullptr);
        }
    }
    MR::initStarPointerTarget(this, 28.0f, TVec3f(0, 0, 0));
    MR::offCalcGravity(this);
    mDelegator = TriangleFilterDelegator< StarPiece >::allocateDelegator(this, &isIgnoreTriOnThrow);
    MR::tryCreateMirrorActor(this, "StarPiece");
    MR::startBtk(this, "Gift");
    MR::setBtkFrameAndStop(this, 5.0f);
    if (mGroupType == groupType_noGroup) {
        appear();
    } else {
        makeActorDead();
    }
}

void StarPiece::initAfterPlacement() {
    MR::isDead(this);
}

void StarPiece::appear() {
    LiveActor::appear();
    MR::onCalcShadow(this, nullptr);
    MR::onCalcShadowDropPrivateGravity(this, nullptr);
}

void StarPiece::setColor(s32 colorIndex) {
    mColor = initColors[colorIndex];
}

s32 StarPiece::getNumColor() {
    return sizeof(initColors) / sizeof(initColors[0]);
}

void StarPiece::appearFromGroup() {
    appear();
    switch (mGroupType) {
    case 1:
        setNerve(&NrvStarPiece::HostTypeNrvFloating::sInstance);
        break;
    case 2:
        setNerve(&NrvStarPiece::HostTypeNrvRailMove::sInstance);
        break;
    }
}

void StarPiece::makeActorAppeared() {
    LiveActor::makeActorAppeared();
    getSensor("attack")->invalidate();
    _C8 = -1;
}

void StarPiece::kill() {
    MR::forceDeleteEffect(this, "StarPieceFlyingBlur");
    if (200.0f < calcDistToCamera()) {
        if (isEffectLight()) {
            MR::emitEffect(this, "StarPieceBreakS");
        } else {
            MR::emitEffect(this, "StarPieceBreak");
        }
    }
    if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
        MR::startSound(this, "SE_OJ_STAR_PIECE_BREAK_W", -1, -1);
    } else {
        MR::startSound(this, "SE_OJ_STAR_PIECE_BREAK", -1, -1);
    }
    LiveActor::kill();
}

void StarPiece::makeActorDead() {
    LiveActor::makeActorDead();
    mTargetSensor = nullptr;
    mReceiverInfo = 0;
    _C8 = -1;

    mFlags._6 = false;
    if (mFlags.isLaunched == true) {
        MR::getStarPieceDirector()->mNumStarPiecesLaunched--;
        mFlags.isLaunched = false;
    }
    if (mHostInfo != nullptr) {
        mHostInfo->_8--;
        mHostInfo = nullptr;
    }
}

void StarPiece::startClipped() {
    LiveActor::startClipped();
    if (!isNerve(&NrvStarPiece::HostTypeNrvFloating::sInstance) && !isNerve(&NrvStarPiece::HostTypeNrvRailMove::sInstance)) {
        makeActorDead();
    }
    _C8 = -1;
}

void StarPiece::changeScale(f32 scale) {
    mScale.set(scale);
    //probably some clamp inline function
    f32 clampedScale;
        if(scale < 0.1f) {
            clampedScale = 0.1f;
        } else if(scale > 2.0f) {
            clampedScale = 2.0f;
        } else{
        clampedScale = scale;
    }
    clampedScale = (clampedScale - 0.1f) / 1.9f;
    MR::setBtkFrameAndStop(this, MR::getBtkFrameMax(this) * (1.0f - clampedScale));
    MR::setBinderRadius(this, 40.0f * scale);
    MR::setShadowVolumeSphereRadius(this, nullptr, 30.0f * scale);
}

void StarPiece::control() {
    if (MR::isFirstStep(this)) {
        if (!isNerve(&NrvStarPiece::HostTypeNrvToTarget::sInstance) || mFlags.isGoToPlayer) {
            MR::setBtkFrameAndStop(this, 5.0f);
        }
    }

    if (mGettableDelayCounter >= 0) {
        mGettableDelayCounter -= 1;
    }

    if (isNerve(&NrvStarPiece::HostTypeNrvFall::sInstance) || isNerve(&NrvStarPiece::HostTypeNrvFallAfterReflect::sInstance)) {
        if (MR::isInDeath(this, TVec3f(0.0f, 0.0f, 0.0f))) {
            kill();
            return;
        }
        if (MR::isPressedRoofAndGround(this)) {
            kill();
            return;
        }
    }
    if (isNerve(&NrvStarPiece::HostTypeNrvFall::sInstance) || isNerve(&NrvStarPiece::HostTypeNrvFallAfterReflect::sInstance) ||
        isNerve(&NrvStarPiece::HostTypeNrvThrow::sInstance) || isNerve(&NrvStarPiece::HostTypeNrvThrowNoFall::sInstance) ||
        isNerve(&NrvStarPiece::HostTypeNrvThrowFall::sInstance)) {
        changeScale(calcNearCameraScale());
    }
}

void StarPiece::exeFloating() {
    if (MR::isFirstStep(this)) {
        MR::offBind(this);
        tryCalcGravity();
        if (!mFlags._3) {
            MR::validateShadow(this, nullptr);
        } else {
            MR::invalidateShadow(this, nullptr);
        }
        if (mFlags.isGroup) {
            MR::invalidateClipping(this);
        } else {
            MR::validateClipping(this);
        }
    }
    mRotation.y += 1.0f;
    MR::repeatDegree(&mRotation.y);
    tryGotJudge();
    if (mFlags._2) {
        HitInfo info;
        MR::checkStrikePointToMap(mPosition, &info);
        if (MR::isCodeSand(&info.mParentTriangle)) {
            kill();
            MR::emitEffect(this, "SandColumnS");
        }
    }
}

void StarPiece::appearHop(const TVec3f& rVec1, const TVec3f& rVec2) {
    if (MR::isNearZero(rVec2)) {
        mGravity.set(0.0f, -1.0f, 0.0f);
    } else {
        MR::normalize(-rVec2, &mGravity);
    }
    mPosition.set(rVec1);
    makeActorAppeared();
    mVelocity.set(rVec2.scaleInline(30.0f));
    setNerve(&NrvStarPiece::HostTypeNrvHop::sInstance);
}

void StarPiece::exeHop() {
    if (MR::isFirstStep(this)) {
        MR::gotByPlayer();
        MR::invalidateClipping(this);
        MR::offBind(this);
        MR::invalidateShadow(this, nullptr);
    }
    mVelocity.add(mGravity);
    if (MR::isGreaterStep(this, 30)) {
        if (mHostInfo != nullptr) {
            mHostInfo->_C++;
        }
        kill();
    }
}

void StarPiece::exeFall() {
    if (MR::isFirstStep(this)) {
        if (isEffectLight()) {
            MR::emitEffect(this, "StarPieceLightS");
        } else {
            MR::emitEffect(this, "StarPieceLight");
        }
        MR::onBind(this);
        MR::invalidateClipping(this);
        MR::validateShadow(this, nullptr);
        MR::onCalcShadow(this, nullptr);
        MR::onCalcShadowDropGravity(this, nullptr);
        mFallKillTimer = 0;
    }
    tryCalcGravity();
    if (mFlags.InWater) {
        mVelocity.scale(0.97f);
        mVelocity.add(mGravity.scaleInline(0.1f));
    } else {
        mVelocity.scale(0.97f);
        mVelocity.add(mGravity.scaleInline(1.0f));
    }
    mRotation.y += 15.0f;
    MR::repeatDegree(&mRotation.y);
    if (MR::reboundVelocityFromCollision(this, 0.99f, 0.0f, 1.0f) == true) {
        if (MR::isBindedGround(this) && !MR::isBindedWall(this) && !MR::isBindedRoof(this)) {
            TVec3f* gravity = &mGravity;
            mVelocity.scale(gravity->dot(mVelocity), *gravity);
        }
        MR::startBck(this, "Land", nullptr);
        if (!MR::isDemoActive()) {
            if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
                MR::startSound(this, "SE_OJ_STAR_PIECE_HIT_W", -1, -1);
            } else {
                MR::startSound(this, "SE_OJ_STAR_PIECE_HIT", -1, -1);
            }
        }
        if (mFlags.InWater) {
            if (mVelocity.squared() < 100.0f) {
                mVelocity.setLength(10.0f);
            }
        } else {
            if (mVelocity.squared() < 400.0f) {
                mVelocity.setLength(20.0f);
            }
        }
    }
    if (!MR::isDemoActive()) {
        mFallKillTimer++;
        if (mFallKillTimer > 600) {
            kill();
            return;
        }
    }
    if (isNerve(&NrvStarPiece::HostTypeNrvFallAfterReflect::sInstance)) {
        if (MR::isGreaterStep(this, 9)) {
            kill();
            return;
        }
    }
    if (!isNerve(&NrvStarPiece::HostTypeNrvFallAfterReflect::sInstance)) {
        tryGotJudge();
    }
}

bool StarPiece::isTouchToTarget(TVec3f* pVec, f32 flt) {
    TVec3f* vec;
    if (mFlags.isGoToPlayer) {
        vec = MR::getPlayerCenterPos();
    } else {
        vec = &mTargetSensor->mPosition;
    }
    TVec3f vec2(*vec);
    vec2.sub(mGravity.scaleInline(flt));
    vec2.sub(mPosition);
    if (pVec != nullptr) {
        pVec->set(vec2);
    }
    f32 distToTouch;
    if (mFlags.isGoToPlayer) {
        distToTouch = 160.0f;
    } else {
        distToTouch = mTargetSensor->mRadius + getSensor("body")->mRadius;
    }
    return vec2.length() < distToTouch;
}

bool StarPiece::isEffectLight() {
    bool retval = true;
    if (!mFlags._6 && MR::getStarPieceDirector()->mNumStarPiecesLaunched <= 10) {
        retval = false;
    }
    return retval;
}

void StarPiece::exeToTarget() {
    if (MR::isFirstStep(this)) {
        if (mFlags.isGoToPlayer) {
            if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
                MR::startSound(this, "SE_OJ_STAR_PIECE_FLY_W", -1, -1);
            } else {
                MR::startSound(this, "SE_OJ_STAR_PIECE_FLY", -1, -1);
            }
        }
        MR::offBind(this);
        MR::invalidateClipping(this);
        mVelocity.zero();
        _98 = 0.02f;
        _9C = 0.0f;
        MR::forceDeleteEffect(this, "StarPieceFlyingBlur");
        MR::emitEffect(this, "StarPieceGetBlur");
        if (mFlags.isGoToPlayer) {
            emitTouchEffect(*MR::getStarPointerLastPointedPort(this), 10.0f);
        } else {
            emitTouchEffect(0, 10.0f);
        }
        MR::invalidateShadow(this, nullptr);
    }
    tryCalcGravity();
    mRotation.y += 15.0f;
    MR::repeatDegree(&mRotation.y);
    if (!mFlags.isGoToPlayer) {
        mScale.add(TVec3f(0.5f, 0.5f, 0.5f).subOperatorInLine(mScale).scaleInline(0.05f));
        changeScale(mScale.y);
    }
    TVec3f touchTargetVec;
    if (isTouchToTarget(&touchTargetVec, 0.0f)) {
        if (mFlags.isGoToPlayer) {
            touchPlayer();
            return;
        }
        if (mReceiverInfo != nullptr) {
            mReceiverInfo->mGotCount += mNumGift;
            mReceiverInfo->_8 -= mNumGift;
        }
        MR::sendMsgStarPieceGift(mTargetSensor, getSensor("attack"), mNumGift);
        changeScale(1.0f);
        MR::emitEffectHit(this, getSensor("attack")->mPosition, "InvalidHitMark");
        makeActorDead();
        return;
    }

    if (mFlags.isGoToPlayer && MR::isResetChasingStarPiece()) {
        kill();
        return;
    }
    //probably an inlined max function twice
    f32 max;
    if(_98 + 0.1f >= 1.0f) {
        max = 1.0f;
    } else {
        max = _98 + 0.1f;
    }
    _98 = max;
    f32 max2;
    if(_9C + 0.03f >= 1.0f) {
        max2 = 1.0f;
    } else {
        max2 = _9C +0.03f;
    }
    _9C = max2;
    MR::normalizeOrZero(&touchTargetVec);
    TQuat4f rotateQuat;
    // inlined TQuat4 functions, not matching because functions are not defined yet
    rotateQuat.setRotate(_8C, touchTargetVec, _98);
    rotateQuat.rotate(_8C);

    MR::normalizeOrZero(&_8C);

    TVec3f vec2(_8C);
    vec2.scale(_9C * 85.0f + 1.0f);
    mVelocity.set(vec2);

    TVec3f* vel;
    if (mFlags.isGoToPlayer) {
        vel = MR::getPlayerVelocity();
    } else {
        vel = &mTargetSensor->mHost->mVelocity;
    }
    TVec3f velNormalized(*vel);
    MR::normalizeOrZero(&velNormalized);
    if (!MR::isNearZero(velNormalized)) {
        f32 dot = _8C.dot(velNormalized);
        if (dot < 0.0f) {
            if (mFlags.isGoToPlayer) {
                mVelocity.add(MR::getPlayerVelocity()->scaleInline(_9C).scaleInline(dot));
            } else {
                mVelocity.add(mTargetSensor->mHost->mVelocity.scaleInline(_9C).scaleInline(dot));
            }
        }
    }
}

void StarPiece::exeSpinDrained() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);
        MR::offBind(this);
        MR::onCalcShadow(this, nullptr);
        MR::onCalcShadowDropPrivateGravity(this, nullptr);
        MR::emitEffect(this, "StarPieceFlyingBlur");
        if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
            MR::startSound(this, "SE_OJ_STAR_PIECE_FLY_W", -1, -1);
        } else {
            MR::startSound(this, "SE_OJ_STAR_PIECE_FLY", -1, -1);
        }
    }
    tryCalcGravity();
}

void StarPiece::exeToPlayerEnd() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);
        MR::invalidateShadow(this, nullptr);
        _B4.set(mPosition);
        _B4.sub(*MR::getPlayerCenterPos());
        mVelocity.zero();
        MR::deleteEffect(this, "StarPieceGetBlur");
        if (mHostInfo != nullptr) {
            mHostInfo->_C++;
            mHostInfo->_8--;
            mHostInfo = nullptr;
        }
    }
    f32 newScale = 1.0f - MR::calcNerveRate(this, 10);
    changeScale(newScale);
    mPosition.set(*MR::getPlayerCenterPos());
    mPosition.add(_B4.scaleInline(newScale));
    if (MR::isGreaterStep(this, 30)) {
        getSensor("body")->validate();
        changeScale(1.0f);
        makeActorDead();
    }
}

void StarPiece::exeThrow() {
    if (MR::isFirstStep(this)) {
        mVelocity.zero();
        MR::offBind(this);
        MR::setBinderRadius(this, 20.0f);
        MR::setBindTriangleFilter(this, mDelegator);
        getSensor("attack")->validate();
        MR::emitEffect(this, "StarPieceFlyingBlur");
        MR::invalidateClipping(this);
        mGravity.set(*MR::getPlayerGravity());
        MR::validateShadow(this, nullptr);
        MR::onCalcShadow(this, nullptr);
        MR::onCalcShadowDropPrivateGravity(this, nullptr);
    }
    mRotation.y += 15.0f;

    MR::repeatDegree(&mRotation.y);
    TVec3f vec1(mPosition);
    TVec3f vec2(0, 0, 0);
    f32 flt = getNerveStep() / 30.0f;
    TVec3f vec3;
    vec3.set(_B4);
    vec3.sub(_A8);
    f32 flt2 = 0.0f;
    vec2 = _A8.addOperatorInLine(vec3.scaleInline(flt)).addOperatorInLine((-mGravity).scaleInline(flt2));
    if (mTargetSensor != nullptr) {
        f32 flt3 = MR::calcNerveRate(this, 30);
        vec2 = mTargetSensor->mPosition.scaleInline(flt3).addOperatorInLine(vec2.scaleInline(1.0f - flt3));
    }
    mVelocity.set(vec2);
    mVelocity.sub(vec1);

    TVec3f camZDir(MR::getCamZdir());
    TVec3f camPos(MR::getCamPos());
    TVec3f vec10(mPosition);
    TVec3f playerCenterPos(*MR::getPlayerCenterPos());
    vec10.sub(camPos);
    playerCenterPos.sub(camPos);
    if (MR::isNoBind(this)) {
        if ((1500.0f < PSVECDistance(&MR::getCamPos(), &mPosition)) || (camZDir.dot(playerCenterPos) < camZDir.dot(vec10)) ||
            MR::isGreaterStep(this, 28)) {
            MR::onBind(this);
        }
    }
    if (MR::isBinded(this)) {
        if (MR::sendMsgToBindedSensor(ACTMES_IS_STAR_PIECE_REFLECT, this, getSensor("attack"))) {
            setReflect(*MR::getBindedNormal(this), *MR::getBindedHitPos(this));
            return;
        }
        if (MR::sendMsgToBindedSensor(ACTMES_STAR_PIECE_ATTACK, this, getSensor("attack"))) {
            MR::startSound(this, "SE_OJ_STAR_PIECE_HIT_ENEMY", -1, -1);
        }
        kill();
        return;
    }
    if (MR::isGreaterStep(this, 30)) {
        setNerve(&NrvStarPiece::HostTypeNrvThrowFall::sInstance);
    }
}

void StarPiece::endThrow() {
    getSensor("attack")->invalidate();
    MR::onBind(this);
    MR::setBinderRadius(this, 40.0f);
    MR::setBindTriangleFilter(this, nullptr);
}
void StarPiece::exeThrowFall() {
    if (MR::isFirstStep(this)) {
        getSensor("attack")->validate();
        MR::invalidateClipping(this);
        MR::onCalcShadow(this, nullptr);
        MR::onCalcShadowDropPrivateGravity(this, nullptr);
    }
    tryCalcGravity();
    mVelocity.scale(0.97f);
    mVelocity.add(mGravity.scaleInline(1.0f));

    mRotation.y += 15.0f;
    MR::repeatDegree(&mRotation.y);

    if (MR::isBinded(this)) {
        if (MR::sendMsgToBindedSensor(ACTMES_IS_STAR_PIECE_REFLECT, this, getSensor("attack"))) {
            setReflect(*MR::getBindedNormal(this), *MR::getBindedHitPos(this));
            return;
        }
        if (MR::sendMsgToBindedSensor(ACTMES_STAR_PIECE_ATTACK, this, getSensor("attack"))) {
            MR::startSound(this, "SE_OJ_STAR_PIECE_HIT_ENEMY", -1, -1);
        }
        kill();
        return;
    }
    if (MR::isGreaterStep(this, 60)) {
        kill();
    }
}

void StarPiece::exeFollowPlayer() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);
        MR::emitEffect(this, "StarPieceFlyingBlur");
        MR::offBind(this);
        MR::invalidateShadow(this, nullptr);
        MR::offCalcShadow(this, nullptr);
    }
    tryGotJudge();
}

void StarPiece::exeRailMove() {
    if (MR::isFirstStep(this)) {
        MR::offBind(this);
        MR::invalidateClipping(this);
        if (!mFlags._3) {
            MR::validateShadow(this, nullptr);
        } else {
            MR::invalidateShadow(this, nullptr);
        }
        MR::onCalcShadow(this, nullptr);
        MR::onCalcShadowDropPrivateGravity(this, nullptr);
    }
    tryCalcGravity();
    mRotation.y += 15.0f;
    MR::repeatDegree(&mRotation.y);
    tryGotJudge();
}

void StarPiece::setFollowPlayerAndAppear() {
    appear();
    MR::invalidateClipping(this);
    setNerve(&NrvStarPiece::HostTypeNrvFollowPlayer::sInstance);
}

bool StarPiece::tryCalcGravity() {
    TVec3f gravity;
    MR::calcGravityVectorOrZero(this, &gravity, nullptr, 0);
    if (!MR::isNearZero(gravity)) {
        mGravity.set(gravity);
        return true;
    }
    return false;
}

bool StarPiece::trySetGravityAndFront(const TVec3f& rVec1) {
    if (!tryCalcGravity()) {
        mGravity.set(rVec1);
        MR::normalizeOrZero(&mGravity);
    }
    if (MR::isSameDirection(mGravity, _8C, 0.01f)) {
        _8C.set< f32 >(mGravity.x, mGravity.z, mGravity.y);
    }
}

bool StarPiece::throwToTarget(HitSensor* pSensor, const TVec3f& rVec1, const TVec3f& rVec2, f32 f1) {
    throwToTargetCore(pSensor->mPosition, rVec1, rVec2, f1, true);
    mTargetSensor = pSensor;
    return true;
}

bool StarPiece::throwToTargetCore(const TVec3f& rVec1, const TVec3f& rVec2, const TVec3f& rVec3, f32 f1, bool a1) {
    mGravity.set(rVec3);
    _B4.set(rVec1);
    _A8.set(rVec2);
    //result is completely unused
    MR::calcParabolicFunctionParam(&_A0, &_A4, f1, 0.0f);
    MR::showModel(this);
    MR::resetPosition(this, _A8);
    MR::invalidateClipping(this);
    makeActorAppeared();
    if (a1) {
        setNerve(&NrvStarPiece::HostTypeNrvThrow::sInstance);
    } else {
        setNerve(&NrvStarPiece::HostTypeNrvThrowNoFall::sInstance);
    }
    MR::addStarPiece(-1);
    return true;
}

void StarPiece::giftToTarget(StarPieceReceiverInfo* receiverInfo, u32 numGift, HitSensor* pSensor, const TVec3f& rVec1) {
    mReceiverInfo = receiverInfo;
    mNumGift = numGift;
    mFlags.isGoToPlayer = false;
    mTargetSensor = pSensor;
    mPosition.set(rVec1);
    _8C.set(pSensor->mPosition.subOperatorInLine(rVec1));
    MR::normalizeOrZero(&_8C);
    MR::showModel(this);
    MR::resetPosition(this);
    appear();
    MR::invalidateClipping(this);
    setNerve(&NrvStarPiece::HostTypeNrvToTarget::sInstance);
    f32 scale = 1.0f;
    if (numGift != 1) {
        scale = 2.0f;
    }
    changeScale(scale);
    MR::addStarPiece(-numGift);
}

void StarPiece::goToPlayer(TVec3f vec) {
    _8C.set(mVelocity);
    MR::normalizeOrZero(&_8C);
    if (MR::isNearZero(_8C)) {
        MR::calcFrontVec(&_8C, this);
    }
    if (isNerve(&NrvStarPiece::HostTypeNrvFloating::sInstance)) {
        if (isEffectLight()) {
            MR::emitEffect(this, "StarPieceLightS");
        } else {
            MR::emitEffect(this, "StarPieceLight");
        }
    } else if (isNerve(&NrvStarPiece::HostTypeNrvFollowPlayer::sInstance)) {
        f32 dot = _8C.dot(mGravity);
        if (0.0f < dot) {
            _8C.sub(mGravity.scaleInline(dot).scaleInline(2.0f));
        }
    }
    mFlags.isGoToPlayer = true;
    mGettableDelayCounter = 15;
    mVelocity.set(vec);
    _8C.set(vec);
    MR::invalidateClipping(this);
    setNerve(&NrvStarPiece::HostTypeNrvToTarget::sInstance);
}

void StarPiece::launch(const TVec3f& rVec, f32 f1, f32 f2, bool notCheckInWater, bool a2) {
    mPosition.set(rVec);
    tryCalcGravity();
    if (MR::isNearZero(mGravity)) {
        MR::calcUpVec(&mGravity, this);
        mGravity.scale(-1.0f);
        trySetGravityAndFront(mGravity);
    }
    TVec3f randVec(MR::getRandom(-f1, f1), MR::getRandom(-f1, f1), MR::getRandom(-f1, f1));
    mVelocity.set((-mGravity).scaleInline(f2));
    mVelocity.add(randVec);
    if (notCheckInWater) {
        mFlags.InWater = false;
    } else {
        mFlags.InWater = MR::isInWater(mPosition);
        if (mFlags.InWater) {
            mVelocity.scale(0.5f);
        }
    }
    MR::resetPosition(this, rVec);
    appear();
    setNerve(&NrvStarPiece::HostTypeNrvFall::sInstance);
    MR::invalidateClipping(this);
    mGettableDelayCounter = 25;
    mFlags._6 = a2;
    MR::getStarPieceDirector()->mNumStarPiecesLaunched++;

    mFlags.isLaunched = true;
}

void StarPiece::launch(const TVec3f& rVec1, const TVec3f& rVec2, f32 f1, f32 f2, bool notCheckInWater, bool a2) {
    mPosition.set(rVec1);
    trySetGravityAndFront(-rVec2);
    TVec3f randVec(MR::getRandom(-f1, f1), MR::getRandom(-f1, f1), MR::getRandom(-f1, f1));
    mVelocity.set(rVec2.scaleInline(f2));
    mVelocity.add(randVec);
    if (notCheckInWater) {
        mFlags.InWater = false;
    } else {
        mFlags.InWater = MR::isInWater(mPosition);
        if (mFlags.InWater) {
            mVelocity.scale(0.5f);
        }
    }
    MR::resetPosition(this, rVec1);
    appear();
    setNerve(&NrvStarPiece::HostTypeNrvFall::sInstance);
    MR::invalidateClipping(this);
    mGettableDelayCounter = 25;
    mFlags._6 = a2;
    MR::getStarPieceDirector()->mNumStarPiecesLaunched++;

    mFlags.isLaunched = true;
}

void StarPiece::launch(const TVec3f& rVec1, const TVec3f& rVec2, bool notCheckInWater, bool a2) {
    mPosition.set(rVec1);
    trySetGravityAndFront(rVec2);
    mVelocity.set(rVec2);
    if (notCheckInWater) {
        mFlags.InWater = false;
    } else {
        mFlags.InWater = MR::isInWater(mPosition);
        if (mFlags.InWater) {
            mVelocity.scale(0.5f);
        }
    }
    MR::resetPosition(this, rVec1);
    appear();
    setNerve(&NrvStarPiece::HostTypeNrvFall::sInstance);
    MR::invalidateClipping(this);
    mGettableDelayCounter = 25;
    mFlags._6 = a2;
    MR::getStarPieceDirector()->mNumStarPiecesLaunched++;

    mFlags.isLaunched = true;
}

f32 StarPiece::calcEffectScale(f32 f1, f32 f2, bool a1) {
    f32 retval = f2;
    f32 pointRadius2D = MR::calcPointRadius2D(mPosition, 30.0f);

    // why not just do an or here??
    if (a1) {
        retval = f1 / pointRadius2D;
    } else if (pointRadius2D * f2 < f1) {
        retval = f1 / pointRadius2D;
    }
    return retval;
}

f32 StarPiece::calcNearCameraScale() {
    f32 pointRadius2D = MR::calcPointRadius2D(mPosition, 30.0f);
    if (30.0f < pointRadius2D) {
        return 30.0f / pointRadius2D;
    }
    return 1.0f;
}

f32 StarPiece::calcDistToCamera() {
    TVec3f dirToCamera(getSensor("body")->mPosition);
    dirToCamera.sub(MR::getCamPos());
    return dirToCamera.length();
}

void StarPiece::emitTouchEffect(s32 colorIndex, f32 f1) {
    f32 effectScale = calcEffectScale(f1, 1.0f, false);
    if (200.0f < calcDistToCamera()) {
        MR::emitEffectWithScale(this, "StarPieceTouchLight", effectScale, -1);
        MR::setEffectPrmColor(this, "StarPieceTouchLight", effectColors[colorIndex].r, effectColors[colorIndex].g, effectColors[colorIndex].b);
    }
}

void StarPiece::emitGettableEffect(f32 f1) {
    f32 effectScale = calcEffectScale(f1, 0.8f, true);
    if (200.0f < calcDistToCamera()) {
        MR::emitEffectWithScale(this, "GetAble", effectScale, -1);
    }
    u8 clampedR = MR::clamp((s32)mColor.r + 0xFF, 0x00, 0xFF);
    u8 clampedG = MR::clamp((s32)mColor.g + 0xFF, 0x00, 0xFF);
    u8 clampedB = MR::clamp((s32)mColor.b + 0xFF, 0x00, 0xFF);
    u8 clampedR2 = MR::clamp((s32)mColor.r + 0x20, 0x00, 0xFF);
    u8 clampedG2 = MR::clamp((s32)mColor.g + 0x20, 0x00, 0xFF);
    u8 clampedB2 = MR::clamp((s32)mColor.b + 0x20, 0x00, 0xFF);
    MR::setEffectColor(this, "GetAble", clampedR, clampedG, clampedB, clampedR2, clampedG2, clampedB2);
}

void StarPiece::tryGotJudge() {
    if (225000000.0f < MR::getPlayerCenterPos()->subOperatorInLine(mPosition).squared() || !mFlags._1) {
        _C8 = -1;
        return;
    }
    _C8 += 91;
    _C8 %= 90;
    if (_C8 == 20) {
        emitGettableEffect(4.0f);
    }
    if (mGettableDelayCounter < 0) {
        bool isPointing = MR::isStarPointerPointing1Por2P(this, "弱", false, false);
        MR::getStarPointerLastPointedPort(this);
        if (isPointing == true) {
            goToPlayer(TVec3f(_8C));
        }
    }
}

bool StarPiece::isFollowing() {
    return isNerve(&NrvStarPiece::HostTypeNrvFollowPlayer::sInstance);
}

bool StarPiece::setFall() {
    if (MR::isDead(this)) {
        return false;
    }
    if (isNerve(&NrvStarPiece::HostTypeNrvRailMove::sInstance) || isNerve(&NrvStarPiece::HostTypeNrvFloating::sInstance)) {
        mFlags._1 = true;
        setNerve(&NrvStarPiece::HostTypeNrvFall::sInstance);
        return true;
    }
    return false;
}

bool StarPiece::isOnRailMove() {
    if (MR::isDead(this)) {
        return false;
    }
    if (isNerve(&NrvStarPiece::HostTypeNrvRailMove::sInstance)) {
        return true;
    }
    return false;
}

bool StarPiece::isFloat() {
    if (MR::isDead(this)) {
        return false;
    }
    if (isNerve(&NrvStarPiece::HostTypeNrvFloating::sInstance)) {
        return true;
    }
    return false;
}

void StarPiece::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorEye(pSender)) {
        return;
    }
    if (!isNerve(&NrvStarPiece::HostTypeNrvThrow::sInstance) && !isNerve(&NrvStarPiece::HostTypeNrvThrowNoFall::sInstance) &&
        !isNerve(&NrvStarPiece::HostTypeNrvThrowFall::sInstance)) {
        return;
    }
    if (MR::sendArbitraryMsg(ACTMES_IS_STAR_PIECE_REFLECT, pReceiver, pSender)) {
        TVec3f dirReceiverToSender(pSender->mPosition.subOperatorInLine(pReceiver->mPosition));
        MR::normalizeOrZero(&dirReceiverToSender);
        TVec3f vec2(pReceiver->mPosition);
        vec2.add(dirReceiverToSender.scaleInline(pReceiver->mRadius));
        setReflect(dirReceiverToSender, vec2);
        return;
    }
    if (MR::sendMsgStarPieceAttack(pReceiver, pSender)) {
        MR::startSound(this, "SE_OJ_STAR_PIECE_HIT_ENEMY", -1, -1);
        kill();
    }
}

void StarPiece::setReflect(const TVec3f& rVec1, const TVec3f& rVec2) {
    if (!MR::isNearZero(rVec1)) {
        MR::calcReflectionVector(&mPosition, rVec1, 0.5f, 0.0f);
    }
    mVelocity.add(mGravity.scaleInline(-35.0f));
    if (200.0f < calcDistToCamera()) {
        MR::emitEffectHit(this, rVec2, "InvalidHitMark");
    }
    if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
        MR::startSound(this, "SE_OJ_STAR_PIECE_REFLECT_W", -1, -1);
    } else {
        MR::startSound(this, "SE_OJ_STAR_PIECE_REFLECT", -1, -1);
    }
    setNerve(&NrvStarPiece::HostTypeNrvFallAfterReflect::sInstance);
}

bool StarPiece::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgInhaleBlackHole(msg)) {
        if (isNerve(&NrvStarPiece::HostTypeNrvFall::sInstance) || isNerve(&NrvStarPiece::HostTypeNrvFallAfterReflect::sInstance) ? true : false) {
            kill();
            return true;
        }
    }
    if (MR::isMsgItemGet(msg)) {
        if (isNerve(&NrvStarPiece::HostTypeNrvThrow::sInstance) || isNerve(&NrvStarPiece::HostTypeNrvThrowNoFall::sInstance) ||
            isNerve(&NrvStarPiece::HostTypeNrvThrowFall::sInstance)) {
            return false;
        }
        if (isNerve(&NrvStarPiece::HostTypeNrvToPlayerEnd::sInstance)) {
            return true;
        }
        if (mFlags.isGoToPlayer && isNerve(&NrvStarPiece::HostTypeNrvToTarget::sInstance)) {
            return true;
        }
        if (isNerve(&NrvStarPiece::HostTypeNrvFloating::sInstance) || isNerve(&NrvStarPiece::HostTypeNrvFall::sInstance) ||
            isNerve(&NrvStarPiece::HostTypeNrvSpinDrained::sInstance) || isNerve(&NrvStarPiece::HostTypeNrvRailMove::sInstance)) {
            if (mGettableDelayCounter < 0) {
                touchPlayer();
                MR::notifyDirectGetStarPiecePlayer();
            }
            return true;
        }
    }
    if (MR::isMsgItemPull(msg) && mGettableDelayCounter < 0) {
        if (isNerve(&NrvStarPiece::HostTypeNrvSpinDrained::sInstance) || isNerve(&NrvStarPiece::HostTypeNrvToPlayerEnd::sInstance) ||
            isNerve(&NrvStarPiece::HostTypeNrvToTarget::sInstance)) {
            return false;
        }
        mTargetSensor = pSender;
        if (MR::isSensorPlayer(mTargetSensor)) {
            mFlags.isGoToPlayer = true;
        } else {
            mFlags.isGoToPlayer = false;
        }
        setNerve(&NrvStarPiece::HostTypeNrvSpinDrained::sInstance);
        return true;
    }
    if (MR::isMsgStartPowerStarGet(msg)) {
        if ((isNerve(&NrvStarPiece::HostTypeNrvToTarget::sInstance) && mFlags.isGoToPlayer) ||
            isNerve(&NrvStarPiece::HostTypeNrvSpinDrained::sInstance) || isNerve(&NrvStarPiece::HostTypeNrvToPlayerEnd::sInstance)) {
            MR::gotByPlayer();
        }
        makeActorDead();
        return true;
    }
    return false;
}

bool StarPiece::isIgnoreTriOnThrow(const Triangle* pTriangle) {
    return 0.0f <= pTriangle->getFaceNormal()->dot(mVelocity);
}

bool StarPiece::touchPlayer() {
    if (MR::gotByPlayer()) {
        if (200.0f < calcDistToCamera()) {
            if (isEffectLight()) {
                MR::emitEffect(this, "StarPieceGetS");
            } else {
                MR::emitEffect(this, "StarPieceGet");
            }
        }
        MR::tryRumblePadMiddle(this, 0);
    }
    MR::stopSound(this, "SE_OJ_STAR_PIECE_FLY_W", 0);
    MR::stopSound(this, "SE_OJ_STAR_PIECE_FLY", 0);
    MR::forceDeleteEffect(this, "StarPieceFlyingBlur");
    setNerve(&NrvStarPiece::HostTypeNrvToPlayerEnd::sInstance);
    getSensor("body")->invalidate();
    return true;
}

void StarPiece::calcAndSetBaseMtx() {
    TPos3f pos;

    if (MR::isNearZero(mGravity)) {
        pos.set(getBaseMtx());
        pos.setTrans(mPosition);
    } else if (MR::isNearZero(_8C)) {
        pos.set(getBaseMtx());
        pos.setTrans(mPosition);
    } else if (MR::isSameDirection(mGravity, _8C, 0.01f)) {
        pos.set(getBaseMtx());
        pos.setTrans(mPosition);
    } else {
        MR::isNearZero(mGravity);
        MR::isNearZero(_8C);
        MR::isSameDirection(mGravity, _8C, 0.01f);
        MR::calcMtxFromGravityAndZAxis(&pos, this, mGravity, _8C);
    }

    TRot3f rotate;
    rotate.setRotate(TVec3f(0.0f, 1.0f, 0.0f), 0.17453294f * mRotation.y);
    pos.concat(pos, rotate);
    MR::setBaseTRMtx(this, pos);
}

void StarPiece::setHostInfo(StarPieceHostInfo* pHostInfo) {
    mHostInfo = pHostInfo;
}
