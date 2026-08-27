#include "Game/MapObj/MeteorStrike.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    const f32 cSensorRadius = 90.0f;
    const f32 cBinderRadius = 80.0f;
    const f32 cShadowRadius = 120.0f;
    const f32 cRumbleDistance = 1000.0f;
    const f32 cRumbleDistanceCannonL = 2500.0f;
    const f32 cRotateSpeed = -7.5f;
    const s32 cShowShadowFrame = 150;
};  // namespace

namespace NrvMeteorStrike {
    NEW_NERVE(MeteorStrikeNrvMove, MeteorStrike, Move);
    NEW_NERVE(MeteorStrikeNrvBreak, MeteorStrike, Break);
};  // namespace NrvMeteorStrike

MeteorStrike::MeteorStrike(const char* pName)
    : LiveActor(pName), mType(), mStepSize(10.0f), mRailDir(0.0f, -1.0f, 0.0f), mBrokenModel(), mStepsLeft(), mTotalSteps() {
    mLavaColumnPos.identity();
}

void MeteorStrike::init(const JMapInfoIter& rIter) {
    initMapToolInfo(rIter);
    initModel();

    MR::connectToSceneNoShadowedMapObjStrongLight(this);

    if (mType != MeteorStrikeType_Environment) {
        initHitSensor(1);
        MR::addHitSensorMapObj(this, "body", 8, ::cSensorRadius, TVec3f(0.0f, 0.0f, 0.0f));
    }

    initBinder(::cBinderRadius, 0.0f, 0);
    initEffectKeeper(0, nullptr, false);

    MR::setEffectHostMtx(this, "LavaColumnAttrDefault", mLavaColumnPos);
    MR::setEffectHostMtx(this, "LavaColumnAttrDamageFire", mLavaColumnPos);

    initRailRider(rIter);

    TVec3f pos0;
    MR::calcRailPointPos(&pos0, this, 0);

    TVec3f pos1;
    MR::calcRailPointPos(&pos1, this, 1);

    mRailDir.sub(pos1, pos0);
    MR::normalize(&mRailDir);

    MR::initShadowVolumeSphere(this, ::cShadowRadius);
    MR::setShadowDropLength(this, nullptr, 3000.0f);
    MR::offShadowVisibleSyncHost(this, nullptr);
    MR::invalidateShadow(this, nullptr);

    initSound(4, false);

    initNerve(GET_NERVE(MeteorStrike, MeteorStrikeNrvMove));

    makeActorDead();
}

void MeteorStrike::initAfterPlacement() {
    TVec3f pos0, pos1;
    MR::calcRailPointPos(&pos0, this, 0);

    TVec3f gravityVec;
    MR::calcGravityVector(this, pos0, &gravityVec, nullptr, 0);

    gravityVec.scale(80.0f);
    pos0.add(gravityVec);

    MR::calcRailPointPos(&pos1, this, 1);
    MR::calcGravityVector(this, pos1, &gravityVec, nullptr, 0);

    gravityVec.scale(80.0f);
    pos1.add(gravityVec);

    TVec3f toNextPos;
    toNextPos.sub(pos1, pos0);

    Triangle triangle = Triangle();
    TVec3f nextPos;
    if (!MR::getFirstPolyOnLineToMap(&nextPos, &triangle, pos0, toNextPos)) {
        nextPos.set(pos1);

        MR::offBind(this);
    }

    mTotalSteps = static_cast< s32 >(pos0.distance(nextPos) / mStepSize) + 1;
}

void MeteorStrike::appear() {
    appear(0);
}

void MeteorStrike::appear(s32 step) {
    mStepsLeft = mTotalSteps - step;

    TVec3f movedPos;
    getMovedPos(&movedPos, step);

    mPosition.set(movedPos);
    mRotation.zero();

    MR::onCalcGravity(this);

    LiveActor::appear();

    MR::hideModel(this);

    MR::invalidateClipping(this);
    MR::invalidateShadow(this, nullptr);

    if (mType == MeteorStrikeType_Cannon) {
        MR::startSound(this, "SE_OJ_CANNON_SHELL_SHOOT");
    }

    setNerve(GET_NERVE(MeteorStrike, MeteorStrikeNrvMove));
}

void MeteorStrike::kill() {
    LiveActor::kill();

    if (mType != MeteorStrikeType_Environment) {
        mBrokenModel->kill();
    }

    MR::invalidateShadow(this, nullptr);
}

bool MeteorStrike::getMovedPos(TVec3f* pDst, s32 step) const {
    if (mTotalSteps < step) {
        return false;
    }

    MR::calcRailPointPos(pDst, this, 0);

    TVec3f vec;
    vec.scale(mStepSize * step, mRailDir);
    pDst->add(*pDst, vec);

    return true;
}

f32 MeteorStrike::getSpeed(const JMapInfoIter& rIter) {
    f32 arg0 = 10.0f;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);
    return arg0;
}

void MeteorStrike::calcAndSetBaseMtx() {
    TPos3f mtx;
    TVec3f negGravity;
    negGravity.negate(mGravity);
    if (MR::isSameDirection(mRailDir, negGravity)) {
        MR::makeMtxFrontNoSupportPos(&mtx, mRailDir, mPosition);
    } else {
        MR::makeMtxFrontUpPos(&mtx, mRailDir, negGravity, mPosition);
    }

    TPos3f rotateMtx;
    rotateMtx.makeRotate(TVec3f(1.0f, 0.0f, 0.0f), MR::toRadian(mRotation.x));
    mtx.concat(rotateMtx);
    MR::setBaseTRMtx(this, mtx);
}

void MeteorStrike::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(MeteorStrike, MeteorStrikeNrvBreak))) {
        return;
    }

    if (!MR::isSensorPlayerOrRide(pReceiver)) {
        return;
    }

    if (MR::isSensorPlayer(pReceiver)) {
        if (!MR::sendMsgEnemyAttackStrong(pReceiver, pSender) && !MR::sendMsgPush(pReceiver, pSender)) {
            return;
        }
    } else {
        MR::sendMsgEnemyAttackExplosion(pReceiver, pSender);
    }

    TVec3f negGravity;
    negGravity.negate(mGravity);
    calcBreakPosture(negGravity, nullptr);

    setNerve(GET_NERVE(MeteorStrike, MeteorStrikeNrvBreak));
}

bool MeteorStrike::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return MR::isMsgStarPieceReflect(msg);
}

void MeteorStrike::initMapToolInfo(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    mStepSize = getSpeed(rIter);

    if (MR::isEqualObjectName(rIter, "MeteorStrikeEnvironment")) {
        mType = MeteorStrikeType_Environment;
    } else if (MR::isEqualObjectName(rIter, "MeteorCannon")) {
        mType = MeteorStrikeType_Cannon;
    }
}

void MeteorStrike::initModel() {
    initModelManagerWithAnm("MeteorStrike", nullptr, false);

    if (mType == MeteorStrikeType_Environment) {
        return;
    }

    if (mType == MeteorStrikeType_Cannon) {
        mBrokenModel = MR::createModelObjMapObjStrongLight("メテオキャノン壊れモデル", "MeteorCannonBreak", nullptr);
    } else {
        mBrokenModel = MR::createModelObjMapObjStrongLight("メテオストライク壊れモデル", "MeteorStrikeBreak", nullptr);
    }

    mBrokenModel->mPosition.set(mPosition);
    mBrokenModel->initWithoutIter();
    mBrokenModel->makeActorDead();
}

void MeteorStrike::calcBreakPosture(const TVec3f& rVec, TPos3f* pDst) {
    TPos3f mtx;
    if (pDst == nullptr) {
        pDst = &mtx;
    }

    TVec3f vec;
    MR::normalize(rVec, &vec);
    MR::makeMtxUpNoSupportPos(pDst, vec, mPosition);

    if (mType == MeteorStrikeType_Environment) {
        return;
    }

    mBrokenModel->mPosition.set(mPosition);

    // FIXME: not inlined set< f32 >
    pDst->getEulerDegree(vec);
    mBrokenModel->mRotation = vec;
    mBrokenModel->mScale.set(mScale);
}

void MeteorStrike::emitEffectColumn(const TPos3f& rPos) {
    mLavaColumnPos.set(rPos);
    MR::emitEffect(this, "LavaColumn");
}

void MeteorStrike::startRumble() {
    f32 strongDistMax = mType == MeteorStrikeType_Cannon ? ::cRumbleDistanceCannonL : ::cRumbleDistance;
    f32 mediumDistMax = mType == MeteorStrikeType_Cannon ? FLOAT_MAX : ::cRumbleDistance * 2;
    MR::startRumbleWithShakeCameraWeak(this, "強", "中", strongDistMax, mediumDistMax);
}

bool MeteorStrike::isInScreen() const {
    TVec3f vec(mRailDir);
    vec.scale(mStepSize);

    TVec3f vec2;
    vec2.add(mPosition, vec);

    return MR::isJudgedToClipFrustum(vec2, 200.0f) == false;
}

void MeteorStrike::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "MeteorStrike");
        mVelocity.scale(mStepSize, mRailDir);
    }

    if (MR::isHiddenModel(this)) {
        if (isInScreen()) {
            MR::showModel(this);

            MR::emitEffect(this, "MeteorStrike");

            MR::startSound(this, "SE_OJ_METEOR_FALL");
        }
    } else if (mType == MeteorStrikeType_Environment && !isInScreen()) {
        kill();
        return;
    }

    if (mType == MeteorStrikeType_Environment) {
        MR::startLevelSound(this, "SE_OJ_LV_METEOR_ENV_FLY");
    }

    mRotation.x = MR::repeatDegree(::cRotateSpeed + mRotation.x);

    if (!MR::isNoBind(this)) {
        s32 frame;
        if (mStepsLeft > ::cShowShadowFrame) {
            frame = getNerveStep() + ::cShowShadowFrame - mStepsLeft;
        } else {
            frame = getNerveStep();
        }

        if (frame >= 0) {
            MR::setShadowVolumeSphereRadius(this, nullptr, MR::getLinerValue(frame, 0.0f, 120.0f, MR::min(mStepsLeft, ::cShowShadowFrame)));

            MR::validateShadow(this, nullptr);
        } else {
            MR::invalidateShadow(this, nullptr);
        }
    }

    if (MR::isBindedGround(this)) {
        if (MR::isHiddenModel(this)) {
            kill();
            return;
        }

        TPos3f pos;
        calcBreakPosture(*MR::getGroundNormal(this), &pos);
        emitEffectColumn(pos);

        if (MR::isBindedGroundDamageFire(this)) {
            MR::startSound(this, "SE_OJ_METEOR_FALL_MAGMA");
            startRumble();

            kill();

            return;
        }

        setNerve(GET_NERVE(MeteorStrike, MeteorStrikeNrvBreak));

        return;
    }

    if (MR::isNoBind(this) && MR::isStep(this, mStepsLeft)) {
        kill();
    }
}

void MeteorStrike::exeBreak() {
    if (MR::isFirstStep(this)) {
        mVelocity.zero();

        MR::hideModel(this);

        MR::offCalcGravity(this);

        MR::invalidateShadow(this, nullptr);

        MR::deleteEffect(this, "MeteorStrike");
        MR::emitEffect(this, "MeteorStrikeBreak");

        MR::startSound(this, "SE_OJ_METEOR_BREAK");

        startRumble();

        mBrokenModel->appear();
        MR::invalidateClipping(mBrokenModel);
        MR::startBck(mBrokenModel, "Break", nullptr);
        MR::startBrk(mBrokenModel, "Break");
    }

    if (MR::isBckStopped(mBrokenModel)) {
        kill();
    }
}
