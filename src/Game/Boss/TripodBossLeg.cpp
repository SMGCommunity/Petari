#include "Game/Boss/TripodBossLeg.hpp"
#include "Game/AudioLib/AudAnmSoundObject.hpp"
#include "Game/Boss/TripodBoss.hpp"
#include "Game/Boss/TripodBossMovableArea.hpp"
#include "Game/Boss/TripodBossStepPoint.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/IKJointCtrl.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/IKJoint.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace NrvTripodBossLeg {
    NEW_NERVE(TripodBossLegNrvHold, TripodBossLeg, Hold);
    NEW_NERVE(TripodBossLegNrvDemo, TripodBossLeg, Demo);
    NEW_NERVE(TripodBossLegNrvMove, TripodBossLeg, Move);
    NEW_NERVE(TripodBossLegNrvUp, TripodBossLeg, Up);
    NEW_NERVE(TripodBossLegNrvLeaveOut, TripodBossLeg, LeaveOut);
    NEW_NERVE(TripodBossLegNrvMoveToLandingPos, TripodBossLeg, MoveToLandingPos);
    NEW_NERVE(TripodBossLegNrvStampSign, TripodBossLeg, StampSign);
    NEW_NERVE(TripodBossLegNrvLanding, TripodBossLeg, Landing);
    NEW_NERVE(TripodBossLegNrvDamageVibration, TripodBossLeg, DamageVibration);
    NEW_NERVE(TripodBossLegNrvDamage, TripodBossLeg, Damage);
    NEW_NERVE(TripodBossLegNrvBreak, TripodBossLeg, Break);
};  // namespace NrvTripodBossLeg

TripodBossLeg::~TripodBossLeg() {
}

TripodBossLeg::TripodBossLeg(const char* pName)
    : LiveActor(pName), mBoss(nullptr), mMoveArea(nullptr), _94(0), _98(0), mJoint(new IKJoint()), _1F0(0, 0, 0, 1), _200(0, 0, 0, 1), _210(0, 0, 0),
      _21C(0, 0, 0), mForceEndPoint(0, 0, 0), _234(0, 0, 0), _240(0, 0, 0) {
    _254 = 0;
    _24C = 1;
    _250 = 0.0f;
    mIsPressPlayer = false;
    mDemoTiming = 0;
    _A0.identity();
    mEndJointMtx.identity();
    _1C0.identity();
    mRootLocalYMtx.identity();
    mRootLocalYZMtx.identity();
    mAnkleLocalXMtx.identity();
    mAnkleLocalXZMtx.identity();
}

void TripodBossLeg::init(const JMapInfoIter& rIter) {
    initNerve(&NrvTripodBossLeg::TripodBossLegNrvHold::sInstance);
    MR::invalidateClipping(this);
    initHitSensor(1);
    MR::addHitSensorMtxMapObj(this, "body", 16, 1000.0f, mEndJointMtx, TVec3f(500.0f, 200.0f, 0.0f));
    initSound(6, false);
    _25C = new AudAnmSoundObject(&_21C, 6, MR::getCurrentHeap());
    _260 = new AudAnmSoundObject(&mForceEndPoint, 6, MR::getCurrentHeap());
    initEffectKeeper(0, "TripodBoss", false);
    MR::setEffectHostMtx(this, "LegSmoke", mEndJointMtx);
    makeActorAppeared();
}

void TripodBossLeg::initShadow(const char* pName) {
    MR::initShadowVolumeFlatModel(this, pName, _1C0);
    MR::setShadowDropLength(this, nullptr, 2000.0f);
}

void TripodBossLeg::makeActorAppeared() {
    LiveActor::makeActorAppeared();
}

void TripodBossLeg::makeActorDead() {
    LiveActor::makeActorDead();
}

void TripodBossLeg::control() {
    _254 = 0;
    mIsPressPlayer = false;
    mEndJointMtx.setTrans(mForceEndPoint);
    separateAnkleJointLocalAxis();
    updateAnkleShadowMatrix();
    _25C->process();
    _260->process();
}

void TripodBossLeg::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (pReceiver->receiveMessage(0xA9, pSender)) {
        _254 = 1;
    }

    if (MR::isSensorPlayer(pReceiver)) {
        mIsPressPlayer = 1;
    }
}

void TripodBossLeg::setBody(TripodBoss* pBoss) {
    mBoss = pBoss;
}

void TripodBossLeg::setMovableArea(TripodBossMovableArea* pArea) {
    mMoveArea = pArea;
}

void TripodBossLeg::setIKParam(f32 rootLength, f32 middleLength, const TVec3f& a3, const TVec3f& a4, const TVec3f& a5) {
    mJoint->setRootBoneLength(rootLength);
    mJoint->setMiddleBoneLength(middleLength);
    mJoint->setFirstPose(a4, a5);
    _210 = a3;
    TVec3f v12(_210);
    v12.y = 0.0f;
    _A0.identity();

    if (!MR::isNearZero(v12)) {
        MR::normalize(&v12);
        _A0.setXDir(v12);
        _A0.setZDir(TVec3f(-v12.z, 0.0f, v12.x));
    }

    _A0.setTrans(_210);
}

void TripodBossLeg::setStepTarget(TripodBossStepPoint* pPoint) {
    _94 = pPoint;
    _98 = pPoint;
    mForceEndPoint = pPoint->mStepPosition;
    MR::makeMtxSideUpPos(&mEndJointMtx, -_98->mStepNormal, _98->mStepFront, mForceEndPoint);
}

void TripodBossLeg::setWait() {
    setNerve(&NrvTripodBossLeg::TripodBossLegNrvHold::sInstance);
}

void TripodBossLeg::setForceEndPoint(const TVec3f& rPoint) {
    mForceEndPoint.set< f32 >(rPoint);
}

void TripodBossLeg::setDemoEffectTiming(bool timg) {
    if (timg) {
        if (mDemoTiming == 0) {
            mDemoTiming = 1;
        } else {
            mDemoTiming = 2;
        }
    } else {
        mDemoTiming = 0;
    }
}

const TPos3f& TripodBossLeg::getRootJointMatrix() const {
    return mJoint->_30;
}

const TPos3f& TripodBossLeg::getMiddleJointMatrix() const {
    return mJoint->_60;
}

const TPos3f& TripodBossLeg::getEndJointMatrix() const {
    return mEndJointMtx;
}

const TPos3f& TripodBossLeg::getRootLocalYMatrix() const {
    return mRootLocalYMtx;
}

const TPos3f& TripodBossLeg::getRootLocalYZMatrix() const {
    return mRootLocalYZMtx;
}

const TPos3f& TripodBossLeg::getAnkleLocalXMatrix() const {
    return mAnkleLocalXMtx;
}

const TPos3f& TripodBossLeg::getAnkleLocalXZMatrix() const {
    return mAnkleLocalXZMtx;
}

void TripodBossLeg::requestStepTarget(TripodBossStepPoint* pPoint) {
    bool v4 = isNerve(&NrvTripodBossLeg::TripodBossLegNrvDamage::sInstance) || isNerve(&NrvTripodBossLeg::TripodBossLegNrvBreak::sInstance);

    if (v4) {
        return;
    }

    _94 = _98;
    _98 = pPoint;

    if (isNerve(&NrvTripodBossLeg::TripodBossLegNrvHold::sInstance)) {
        setNerve(&NrvTripodBossLeg::TripodBossLegNrvUp::sInstance);
    } else {
        setNerve(&NrvTripodBossLeg::TripodBossLegNrvMoveToLandingPos::sInstance);
    }
}

void TripodBossLeg::requestLeaveOut() {
    setNerve(&NrvTripodBossLeg::TripodBossLegNrvLeaveOut::sInstance);
}

void TripodBossLeg::requestBreak() {
    if (!isNerve(&NrvTripodBossLeg::TripodBossLegNrvBreak::sInstance)) {
        setNerve(&NrvTripodBossLeg::TripodBossLegNrvBreak::sInstance);
    }
}

void TripodBossLeg::requestStartDemo() {
    _24C = 1;
    setNerve(&NrvTripodBossLeg::TripodBossLegNrvDemo::sInstance);
}

void TripodBossLeg::requestEndDemo() {
    _24C = 0;
    setNerve(&NrvTripodBossLeg::TripodBossLegNrvHold::sInstance);
}

bool TripodBossLeg::isLanding() const {
    return isNerve(&NrvTripodBossLeg::TripodBossLegNrvHold::sInstance);
}

bool TripodBossLeg::isDamage() const {
    return isNerve(&NrvTripodBossLeg::TripodBossLegNrvDamageVibration::sInstance) || isNerve(&NrvTripodBossLeg::TripodBossLegNrvDamage::sInstance);
}

bool TripodBossLeg::isStop() const {
    return isNerve(&NrvTripodBossLeg::TripodBossLegNrvHold::sInstance) || isNerve(&NrvTripodBossLeg::TripodBossLegNrvDemo::sInstance) ||
           isNerve(&NrvTripodBossLeg::TripodBossLegNrvDamage::sInstance) || isNerve(&NrvTripodBossLeg::TripodBossLegNrvBreak::sInstance);
}

bool TripodBossLeg::isBroken() const {
    return isNerve(&NrvTripodBossLeg::TripodBossLegNrvBreak::sInstance);
}

bool TripodBossLeg::isPressPlayer() const {
    return mIsPressPlayer;
}

bool TripodBossLeg::canStep() const {
    return !isNerve(&NrvTripodBossLeg::TripodBossLegNrvDamage::sInstance) && !isNerve(&NrvTripodBossLeg::TripodBossLegNrvBreak::sInstance);
}

bool TripodBossLeg::canCancelStep() const {
    return !isNerve(&NrvTripodBossLeg::TripodBossLegNrvLanding::sInstance) && !isNerve(&NrvTripodBossLeg::TripodBossLegNrvStampSign::sInstance);
}

bool TripodBossLeg::canWeighting() const {
    return isNerve(&NrvTripodBossLeg::TripodBossLegNrvHold::sInstance) || isNerve(&NrvTripodBossLeg::TripodBossLegNrvLanding::sInstance) ||
           isNerve(&NrvTripodBossLeg::TripodBossLegNrvLeaveOut::sInstance) && MR::isLessStep(this, 60);
}

void TripodBossLeg::exeDemo() {
    if (MR::isFirstStep(this)) {
        _234.zero();
    }

    if (mDemoTiming == 1) {
        MR::startSoundObject(_260, "SE_BM_TRIPOD_LAND");
        MR::emitEffect(this, "LegSmoke");
    }

    updateIKPose();
    updateAnkleUp(1000.0f);
}

void TripodBossLeg::exeUp() {
    if (MR::isFirstStep(this)) {
        MR::startSoundObject(_260, "SE_BM_TRIPOD_LEG_UP");
        TPos3f v5;
        v5.identity();
        MR::makeMtxSideUp(&v5, -_94->mStepNormal, _94->mStepFront);
        v5.getQuat(_1F0);
    }

    addAccelUpLeg(_94, 1.5f);
    f32 legHeight = calcLegHeight(_94);
    updateAnkleUp(legHeight);
    if (legHeight > 1000.0f) {
        setNerve(&NrvTripodBossLeg::TripodBossLegNrvMoveToLandingPos::sInstance);
    }
}

void TripodBossLeg::exeLeaveOut() {
    if (MR::isFirstStep(this)) {
        MR::startSoundObject(_260, "SE_BM_TRIPOD_LEG_UP");
        TPos3f v9;
        v9.identity();
        MR::makeMtxSideUp(&v9, -_98->mStepNormal, _98->mStepFront);
        v9.getQuat(_1F0);
        _240 = _98->mStepPosition + _98->mStepNormal * 1000.0f;
    }

    if (MR::isLessStep(this, 60)) {
        _234.zero();
        updatePose();
    } else if (calcLegHeight(_98) < 1000.0f) {
        addAccelUpLeg(_98, 0.8f);
        updateAnkleUp(calcLegHeight(_98));
    } else {
        addToTargetPower(_240, 1.0f);
        addIKLimitPower();
        mForceEndPoint += _234;
        _234.x *= 0.9f;
        _234.y *= 0.9f;
        _234.z *= 0.9f;
        updatePose();
        updateAnkleSlerpToBasePose();
    }
}

void TripodBossLeg::exeMove() {
    MR::startSoundObjectLevel(_260, "SE_BM_LV_TRIPOD_LEG_MOVE");
    addToTargetPower(_240, 1.0f);
    addIKLimitPower();
    mForceEndPoint += _234;
    _234.x *= 0.9f;
    _234.y *= 0.9f;
    _234.z *= 0.9f;
    updatePose();
}

void TripodBossLeg::exeMoveToLandingPos() {
    if (MR::isFirstStep(this)) {
        TVec3f v14 = _98->mStepPosition + _98->mStepNormal * 1000.0f;
        _240 = v14;
    }

    MR::startSoundObjectLevel(_260, "SE_BM_LV_TRIPOD_LEG_MOVE");
    addToTargetPower(_240, 1.0f);
    addIKLimitPower();
    mForceEndPoint += _234;
    _234.x *= 0.95f;
    _234.y *= 0.95f;
    _234.z *= 0.95f;
    updatePose();
    updateAnkleSlerpToBasePose();

    if (mForceEndPoint.squared(_240) < 90000.0f) {
        setNerve(&NrvTripodBossLeg::TripodBossLegNrvStampSign::sInstance);
    }
}

// float regswap
void TripodBossLeg::exeStampSign() {
    if (MR::isFirstStep(this)) {
        _240 = _98->mStepPosition + _98->mStepNormal * 1000.0f;
    }

    MR::startSoundObjectLevel(_260, "SE_BM_LV_TRIPOD_LEG_PREFALL");
    addToTargetPower(_240, 1.0f);
    addIKLimitPower();
    mForceEndPoint += _234;
    _234.x *= 0.9f;
    _234.y *= 0.9f;
    _234.z *= 0.9f;

    if (MR::isGreaterStep(this, 20)) {
        TVec3f v8(_98->mStepNormal);
        mForceEndPoint += v8 * (20.0f * MR::sin(getNerveStep() * 6.2831855f / 10.0f));
    }

    updatePose();
    updateAnkleSlerpToBasePose();
    if (MR::isGreaterStep(this, 60)) {
        _240 = _98->mStepPosition;
        setNerve(&NrvTripodBossLeg::TripodBossLegNrvLanding::sInstance);
    }
}

void TripodBossLeg::exeLanding() {
    if (MR::isFirstStep(this)) {
        MR::startSoundObject(_260, "SE_BM_TRIPOD_FALL");
        _234.zero();
        mEndJointMtx.getQuat(_1F0);
        TPos3f v18;
        v18.identity();
        TripodBossStepPoint* point = _98;
        MR::makeMtxSideUp(&v18, -point->mStepNormal, point->mStepFront);
        v18.getQuat(_200);
        _250 = (mForceEndPoint - _98->mStepPosition).dot(_98->mStepNormal);
    }

    TVec3f v15(_240 - mForceEndPoint);
    MR::normalizeOrZero(&v15);
    TVec3f v9(v15);
    v9.mult(1.0f);
    _234 += v9;
    addIKLimitPower();
    mForceEndPoint += _234;
    _234.x *= 0.98f;
    _234.y *= 0.98f;
    _234.z *= 0.98f;
    updateIKPose();
    TVec3f v12;
    mEndJointMtx.getTrans(v12);
    TVec3f endJointPos;
    mJoint->getEndJointPosition(&endJointPos);
    HitResult hitResult;
    if (mMoveArea->collideSphere(&hitResult, v12, 0.0f, endJointPos - v12)) {
        MR::startSoundObject(_260, "SE_BM_TRIPOD_LAND");
        f32 dist = mForceEndPoint.distance(*MR::getPlayerPos());
        if (dist < 2000.0f) {
            MR::shakeCameraStrong();
        } else if (dist < 3500.0f) {
            MR::shakeCameraNormal();
        } else {
            MR::shakeCameraWeak();
        }

        MR::emitEffect(this, "LegSmoke");
        mForceEndPoint = hitResult._0;
        _234.zero();
        if (_254) {
            setNerve(&NrvTripodBossLeg::TripodBossLegNrvDamageVibration::sInstance);
        } else {
            setNerve(&NrvTripodBossLeg::TripodBossLegNrvHold::sInstance);
        }

        updateIKPose();
        updateAnkleLanding();
    } else {
        mJoint->getEndJointPosition(&mForceEndPoint);
        updateAnkleLanding();
    }
}

void TripodBossLeg::exeDamageVibration() {
    if (MR::isFirstStep(this)) {
        _234.zero();
        _240 = mForceEndPoint;
    }

    TVec3f v5(_98->mStepNormal);
    mForceEndPoint = _240 + v5 * (MR::sin(((6.2831855f * getNerveStep() * 25.0f))) * 40.0f);
    updateIKPose();

    if (MR::isGreaterStep(this, 27)) {
        mForceEndPoint = _240;
        setNerve(&NrvTripodBossLeg::TripodBossLegNrvDamage::sInstance);
    }
}

void TripodBossLeg::exeBreak() {
    if (MR::isFirstStep(this)) {
        _234.zero();
    }

    updateIKPose();
    kill();
}

void TripodBossLeg::exeDamage() {
    updateIKPose();
}

void TripodBossLeg::exeHold() {
    if (MR::isFirstStep(this)) {
        _234.zero();
    }

    updateIKPose();
}

void TripodBossLeg::addToTargetPower(const TVec3f& a1, f32 a2) {
    TVec3f v11(a1 - mForceEndPoint);
    f32 v7;
    MR::separateScalarAndDirection(&v7, &v11, v11);
    _234 += v11 * (a2 * MR::clamp(v7 / 300.0f, -1.0f, 1.0f));
    f32 v6 = (1.0f - (v7 / 300.0f));
    if (v6 > 0.0f) {
        _234 -= _234 * v6 * 0.1f;
    }
}

void TripodBossLeg::addIKLimitPower() {
    TVec3f rootJoint;
    mJoint->getRootJointPosition(&rootJoint);
    TVec3f v12(rootJoint - mForceEndPoint);
    f32 v8;
    MR::separateScalarAndDirection(&v8, &v12, v12);
    f32 maxDist = mJoint->getMaxLimitDistance();
    f32 minDist = mJoint->getMinLimitDistance();
    f32 v4 = (f32)MR::normalize(v8, minDist, maxDist);
    v8 = v4;
    f32 v5, v6;

    if (v8 < 0.5f) {
        v8 = 1.0f - v8;
        v12 = -v12;
        v5 = 0.5f;
        v6 = 0.0f;
    } else {
        v5 = 0.9f;
        v6 = 5.0f;
    }

    if (v5 < v8) {
        f32 ease = MR::getEaseInOutValue(((1.0f / (1.0f - v5))) * (v8 - v5), 0.0f, 1.0f, 1.0f);
        _234 += v12 * ease * v6;
    }
}

void TripodBossLeg::addAccelUpLeg(TripodBossStepPoint* pPoint, f32 a3) {
    TVec3f v8(pPoint->mStepNormal);
    _234 += v8 * a3;
    addIKLimitPower();
    mForceEndPoint += _234;
    _234.x *= 0.9f;
    _234.y *= 0.9f;
    _234.z *= 0.9f;
    updatePose();
}

f32 TripodBossLeg::calcLegHeight(TripodBossStepPoint* pPoint) const {
    TVec3f v4(mForceEndPoint - pPoint->mStepPosition);
    return v4.dot(pPoint->mStepNormal);
}

void TripodBossLeg::updatePose() {
    updateIKPose();
    if (bindEndPosition()) {
        updateIKPose();
    }

    mJoint->getEndJointPosition(&mForceEndPoint);
}

void TripodBossLeg::updateIKPose() {
    TPos3f bodyMtx;
    mBoss->getBodyMatrix(&bodyMtx);
    switch (_24C) {
    case 0: {
        TVec3f v4;
        bodyMtx.mult(_210, v4);
        TVec3f legUp;
        mBoss->calcLegUpVector(&legUp, mForceEndPoint);
        mJoint->updateByUpVector(v4, mForceEndPoint, legUp);
        break;
    }
    case 1:
        mJoint->updateByLocalRootAndWorldTarget(bodyMtx, _210, mForceEndPoint);
        break;
    }

    separateLocalAxisRootJoint(bodyMtx);
}

bool TripodBossLeg::bindEndPosition() {
    TVec3f v6;
    mEndJointMtx.getTrans(v6);
    TVec3f endJointPos;
    mJoint->getEndJointPosition(&endJointPos);
    HitResult hitResult;
    if (mMoveArea->collideSphere(&hitResult, v6, 0.0f, endJointPos - v6)) {
        mForceEndPoint = hitResult._C;
        if (_234.dot(hitResult._18) < 0.0f) {
            _234.orthogonalize2(hitResult._18);
        }

        return true;
    }

    return false;
}

void TripodBossLeg::separateLocalAxisRootJoint(const TPos3f& rPos) {
    TPos3f v5;
    v5.set(rPos);
    v5.setTrans(0.0f, 0.0f, 0.0f);
    v5.concat(rPos, _A0);
    MR::separateMatrixRotateYZX(&mRootLocalYMtx, &mRootLocalYZMtx, v5, mJoint->_30);
    TVec3f jointRootPos;
    mJoint->getRootJointPosition(&jointRootPos);
    mRootLocalYMtx.setTrans(jointRootPos);
    mRootLocalYZMtx.setTrans(jointRootPos);
}

void TripodBossLeg::separateAnkleJointLocalAxis() {
    MR::separateMatrixRotateYZX(&mAnkleLocalXZMtx, &mAnkleLocalXMtx, mEndJointMtx, mJoint->_60);
    mAnkleLocalXMtx.setTrans(mForceEndPoint);
    mAnkleLocalXZMtx.setTrans(mForceEndPoint);
}

void TripodBossLeg::updateAnkleUp(f32 angle) {
    TPos3f v9;
    TQuat4f quat;
    TVec3f landingNormal;
    TVec3f landingFront;

    mMoveArea->calcLandingNormal(&landingNormal, mForceEndPoint);
    mMoveArea->calcLandingFront(&landingFront, mForceEndPoint);

    v9.identity();
    MR::makeMtxSideUp(&v9, -landingNormal, landingFront);

    v9.getQuat(quat);
    f32 v4 = MR::clamp((angle / 1000.0f), 0.0f, 1.0f);
    quat.slerp(_1F0, (1.0f - (v4 * (v4 * v4))));
    quat.normalize();
    mEndJointMtx.makeQuat(quat);
}

void TripodBossLeg::updateAnkleSlerpToBasePose() {
    TQuat4f quat;
    TVec3f landingNormal;
    TVec3f landingFront;
    mEndJointMtx.getQuat(quat);
    mMoveArea->calcLandingNormal(&landingNormal, mForceEndPoint);
    mMoveArea->calcLandingFront(&landingFront, mForceEndPoint);
    TPos3f v7;
    v7.identity();
    MR::makeMtxSideUp(&v7, -landingNormal, landingFront);
    TQuat4f v5;
    v7.getQuat(v5);
    quat.slerp(quat, v5, 0.2f);
    quat.normalize();
    mEndJointMtx.makeQuat(quat);
}

void TripodBossLeg::updateAnkleLanding() {
    TVec3f v6(mForceEndPoint - _98->mStepPosition);
    f32 v3 = v6.dot(_98->mStepNormal);
    f32 v4 = MR::normalize((v3 / _250), 0.15f, 1.0f);
    TQuat4f quat;
    quat.slerp(_1F0, _200, ((1.0f - v4) * ((1.0f - v4) * (1.0f - v4))));
    quat.normalize();
    mEndJointMtx.makeQuat(quat);
}

void TripodBossLeg::updateAnkleShadowMatrix() {
    TVec3f v7, v8, v9;
    TVec3f landingPosition;
    TVec3f landingNormal;

    mMoveArea->calcNearLandingPosition(&landingPosition, mForceEndPoint);
    mMoveArea->calcLandingNormal(&landingNormal, landingPosition);

    MR::setShadowDropDirection(this, nullptr, -landingNormal);
    mEndJointMtx.getXYZDir(v7, v8, v9);
    _1C0.setXYZDir(-v9, -v7, v8);

    TVec3f v2(landingNormal);
    v2 *= 630.0f;
    TVec3f v3(mForceEndPoint);
    v3 -= v2;
    _1C0.setTrans(v3);
}

namespace MR {
    void separateMatrixRotateYZX(TPos3f* a1, TPos3f* a2, const TPos3f& a3, const TPos3f& a4) {
        TPos3f v17;
        v17.invert(a3);
        v17.concat(v17, a4);
        TVec3f v16;
        v17.getXDir(v16);
        f32 v10 = MR::sqrt((v16.x * v16.x) + (v16.z * v16.z));

        if (MR::isNearZero(v10, 0.000001f)) {
            if (v16.y >= 0.0f) {
                a1->setXDir(0.0f, 1.0f, 0.0f);
                a1->setYDir(-1.0f, 0.0f, 0.0f);

            } else {
                a1->setXDir(0.0f, -1.0f, 0.0f);
                a1->setYDir(1.0f, 0.0f, 0.0f);
            }

            a1->setZDir(0.0f, 0.0f, 1.0f);
            a1->concat(a3, *a1);
            a2->set(*a1);
        } else {
            f32 v11 = v16.length();
            f32 v12 = (v10 / v11);
            f32 v14 = (v16.y / v11);
            f32 v15 = (v16.x / v10);
            f32 v13 = (v16.z / v10);

            a1->setXDir(v15, 0.0f, v13);
            a1->setYDir(0.0f, 1.0f, 0.0f);
            a1->setZDir(-v13, 0.0f, v15);
            a1->setTrans(0.0f, 0.0f, 0.0f);
            a2->setXDir(v12, v14, 0.0f);
            a2->setYDir(-v14, v12, 0.0f);
            a2->setZDir(0.0f, 0.0f, 1.0f);
            a2->setTrans(0.0f, 0.0f, 0.0f);
            a1->concat(a3, *a1);
            a2->concat(*a1, *a2);
        }
    }
};  // namespace MR
