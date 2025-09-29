#include "Game/Boss/TripodBossLeg.hpp"
#include "Game/Boss/TripodBoss.hpp"
#include "Game/AudioLib/AudAnmSoundObject.hpp"
#include "Game/Boss/TripodBossMovableArea.hpp"
#include "Game/Boss/TripodBossStepPoint.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/IKJointCtrl.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/IKJoint.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

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
};

TripodBossLeg::~TripodBossLeg() {

}

TripodBossLeg::TripodBossLeg(const char *pName) : LiveActor(pName), mBoss(nullptr), mMoveArea(nullptr), _94(0), _98(0), mJoint(new IKJoint()),
    _1F0(0, 0, 0, 1), _200(0, 0, 0, 1), _210(0, 0, 0), _21C(0, 0, 0), mForceEndPoint(0, 0, 0), _234(0, 0, 0), _240(0, 0, 0) {

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

void TripodBossLeg::init(const JMapInfoIter &rIter) {
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

void TripodBossLeg::initShadow(const char *pName) {
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

void TripodBossLeg::attackSensor(HitSensor *pSender, HitSensor *pReceiver) {
    if (pReceiver->receiveMessage(0xA9, pSender)) {
        _254 = 1;
    }

    if (MR::isSensorPlayer(pReceiver)) {
        mIsPressPlayer = 1;
    }
}

void TripodBossLeg::setBody(TripodBoss *pBoss) {
    mBoss = pBoss;
}

void TripodBossLeg::setMovableArea(TripodBossMovableArea *pArea) {
    mMoveArea = pArea;
}

void TripodBossLeg::setIKParam(f32 rootLength, f32 middleLength, const TVec3f &a3, const TVec3f &a4, const TVec3f &a5) {
    mJoint->setRootBoneLength(rootLength);
    mJoint->setMiddleBoneLength(middleLength);
    mJoint->setFirstPose(a4, a5);
    _210 = a3;
    TVec3f v12(_210);
    v12.y = 0.0f;
    _A0.identity();

    if (!MR::isNearZero(v12, 0.001f)) {
        MR::normalize(&v12);
        _A0.mMtx[0][0] = v12.x;
        _A0.mMtx[1][0] = v12.y;
        _A0.mMtx[2][0] = v12.z;
        TVec3f v11(-v12.z, 0.0f, v12.x);
        _A0.mMtx[0][2] = v11.x;
        _A0.mMtx[1][2] = v11.y;
        _A0.mMtx[2][2] = v11.z;
    }

    _A0.setTrans(_210);
}

void TripodBossLeg::setStepTarget(TripodBossStepPoint *pPoint) {
    _94 = pPoint;
    _98 = pPoint;
    mForceEndPoint = pPoint->mStepPosition;
    MR::makeMtxSideUpPos(&mEndJointMtx, -_98->mStepNormal, _98->mStepFront, mForceEndPoint);
}

void TripodBossLeg::setWait() {
    setNerve(&NrvTripodBossLeg::TripodBossLegNrvHold::sInstance);
}

void TripodBossLeg::setForceEndPoint(const TVec3f &rPoint) {
    mForceEndPoint.set<f32>(rPoint);
}

void TripodBossLeg::setDemoEffectTiming(bool timg) {
    if (timg) {
        if (mDemoTiming == 0) {
            mDemoTiming = 1;
        }
        else {
            mDemoTiming = 2;
        }
    }
    else {
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

void TripodBossLeg::requestStepTarget(TripodBossStepPoint *pPoint) {
    bool v4 = false;
    if (isNerve(&NrvTripodBossLeg::TripodBossLegNrvDamage::sInstance) || isNerve(&NrvTripodBossLeg::TripodBossLegNrvBreak::sInstance)) {
        v4 = true;
    }

    if (!v4) {
        _94 = _98;
        _98 = pPoint;
        if (isNerve(&NrvTripodBossLeg::TripodBossLegNrvHold::sInstance)) {
            setNerve(&NrvTripodBossLeg::TripodBossLegNrvUp::sInstance);
        }
        else {
            setNerve(&NrvTripodBossLeg::TripodBossLegNrvMoveToLandingPos::sInstance);
        }
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
    _24C= 0;
    setNerve(&NrvTripodBossLeg::TripodBossLegNrvHold::sInstance);
}

bool TripodBossLeg::isLanding() const {
    return isNerve(&NrvTripodBossLeg::TripodBossLegNrvHold::sInstance);
}

bool TripodBossLeg::isDamage() const {
    bool ret = false;

    if (isNerve(&NrvTripodBossLeg::TripodBossLegNrvDamageVibration::sInstance) || isNerve(&NrvTripodBossLeg::TripodBossLegNrvDamage::sInstance)) {
        ret = true;
    }

    return ret;
}

bool TripodBossLeg::isStop() const {
    bool ret = false;
    if (isNerve(&NrvTripodBossLeg::TripodBossLegNrvHold::sInstance) || isNerve(&NrvTripodBossLeg::TripodBossLegNrvDemo::sInstance) || isNerve(&NrvTripodBossLeg::TripodBossLegNrvDamage::sInstance) || isNerve(&NrvTripodBossLeg::TripodBossLegNrvBreak::sInstance)) {
        ret = true;
    }
    return ret;
}

bool TripodBossLeg::isBroken() const {
    return isNerve(&NrvTripodBossLeg::TripodBossLegNrvBreak::sInstance);
}

bool TripodBossLeg::isPressPlayer() const {
    return mIsPressPlayer;
}

bool TripodBossLeg::canStep() const {
    bool ret = false;
    if (!isNerve(&NrvTripodBossLeg::TripodBossLegNrvDamage::sInstance) && !isNerve(&NrvTripodBossLeg::TripodBossLegNrvBreak::sInstance)) {
        ret = true;
    }
    return ret;
}

bool TripodBossLeg::canCancelStep() const {
    bool ret = false;
    if (!isNerve(&NrvTripodBossLeg::TripodBossLegNrvLanding::sInstance) && !isNerve(&NrvTripodBossLeg::TripodBossLegNrvStampSign::sInstance)) {
        ret = true;
    }
    return ret;
}

bool TripodBossLeg::canWeighting() const {
    bool ret = false;
    if (isNerve(&NrvTripodBossLeg::TripodBossLegNrvHold::sInstance) || isNerve(&NrvTripodBossLeg::TripodBossLegNrvLanding::sInstance) || isNerve(&NrvTripodBossLeg::TripodBossLegNrvLeaveOut::sInstance) && MR::isLessStep(this, 60)) {
        ret = true;
    }
    return ret;
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
        TVec3f v6(_98->mStepNormal);
        v6 *= 1000.0f;
        TVec3f v7(_98->mStepPosition);
        v7 += v6;
        _240 = v7;
    }

    if (MR::isLessStep(this, 60)) {
        _234.zero();
        updatePose();
    }
    else if (calcLegHeight(_98) < 1000.0f) {
        addAccelUpLeg(_98, 0.80000001f);
        updateAnkleUp(calcLegHeight(_98));
    }
    else {
        addToTargetPower(_240, 1.0f);
        addIKLimitPower();
        mForceEndPoint += _234;
        _234.x *= 0.89999998f;
        _234.y *= 0.89999998f;
        _234.z *= 0.89999998f;
        updatePose();
        updateAnkleSlerpToBasePose();
    }
}

void TripodBossLeg::exeMove() {
    MR::startSoundObjectLevel(_260, "SE_BM_LV_TRIPOD_LEG_MOVE", -1);
    addToTargetPower(_240, 1.0f);
    addIKLimitPower();
    mForceEndPoint += _234;
    _234.x *= 0.89999998f;
    _234.y *= 0.89999998f;
    _234.z *= 0.89999998f;
    updatePose();
}

void TripodBossLeg::exeMoveToLandingPos() {
    if (MR::isFirstStep(this)) {
        TVec3f v14 = _98->mStepPosition.translate(_98->mStepNormal.multiplyOperatorInline(1000.0f));
        _240 = v14;
    }

    MR::startSoundObjectLevel(_260, "SE_BM_LV_TRIPOD_LEG_MOVE", -1);
    addToTargetPower(_240, 1.0f);
    addIKLimitPower();
    mForceEndPoint += _234;
    _234.x *= 0.94999999f;
    _234.y *= 0.94999999f;
    _234.z *= 0.94999999f;
    updatePose();
    updateAnkleSlerpToBasePose();

    if (JMathInlineVEC::PSVECSquareDistance(&mForceEndPoint, &_240) < 90000.0f) {
        setNerve(&NrvTripodBossLeg::TripodBossLegNrvStampSign::sInstance);
    }
}

// float regswap
void TripodBossLeg::exeStampSign() {
    if (MR::isFirstStep(this)) {
        TVec3f v6(_98->mStepNormal);
        v6 *= 1000.0f;
        TVec3f v7(_98->mStepPosition);
        v7 += v6;
        _240 = v7;
    }

    MR::startSoundObjectLevel(_260, "SE_BM_LV_TRIPOD_LEG_PREFALL", -1);
    addToTargetPower(_240, 1.0f);
    addIKLimitPower();
    mForceEndPoint += _234;
    _234.x *= 0.89999998f;
    _234.y *= 0.89999998f;
    _234.z *= 0.89999998f;

    if (MR::isGreaterStep(this, 20)) {
        TVec3f v8(_98->mStepNormal);
        f32 v4 = MR::sin(getNerveStep() * 6.2831855f / 10.0f) * 20.0f;
        TVec3f v5(v8);
        v5 *= v4;
        mForceEndPoint += v5;
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
        TVec3f* step = &_98->mStepPosition;
        TVec3f v16(mForceEndPoint);
        v16 -= *step;
        _250 = v16.dot(_98->mStepNormal);
    }

    TVec3f v15(_240);
    v15 -= mForceEndPoint;
    MR::normalizeOrZero(&v15);
    TVec3f v9(v15);
    v9.mult(1.0f);
    _234 += v9;
    addIKLimitPower();
    mForceEndPoint += _234;
    _234.x *= 0.98000002f;
    _234.y *= 0.98000002f;
    _234.z *= 0.98000002f;
    updateIKPose();
    TVec3f v12;
    mEndJointMtx.getTrans(v12);
    TVec3f endJointPos;
    mJoint->getEndJointPosition(&endJointPos);
    HitResult hitResult;
    TVec3f v8(endJointPos);
    v8 -= v12;
    if (mMoveArea->collideSphere(&hitResult, v12, 0.0f, v8)) {
        MR::startSoundObject(_260, "SE_BM_TRIPOD_LAND");
        f32 dist = PSVECDistance(&mForceEndPoint, MR::getPlayerPos());
        if (dist < 2000.0f) {
            MR::shakeCameraStrong();
        }
        else {
            if (dist < 3500.0f) {
                MR::shakeCameraNormal();
            }
            else {
                MR::shakeCameraWeak();
            }
        }

        MR::emitEffect(this, "LegSmoke");
        mForceEndPoint = hitResult._0;
        _234.zero();
        if (_254) {
            setNerve(&NrvTripodBossLeg::TripodBossLegNrvDamageVibration::sInstance);
        }
        else {
            setNerve(&NrvTripodBossLeg::TripodBossLegNrvHold::sInstance);
        }

        updateIKPose();
        updateAnkleLanding();
    }
    else {
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
    f32 v2 = (MR::sin(((6.2831855f * getNerveStep() * 25.0f))) * 40.0f);
    TVec3f v3(v5);
    v3 *= v2;
    TVec3f v4(_240);
    v4 += v3;
    mForceEndPoint = v4;
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

void TripodBossLeg::addToTargetPower(const TVec3f &a1, f32 a2) {
    TVec3f v11(a1);
    v11 -= mForceEndPoint;
    f32 v7;
    MR::separateScalarAndDirection(&v7, &v11, v11);
    f32 v5 = a2 * MR::clamp(v7 / 300.0f, -1.0f, 1.0f);
    TVec3f v10(v11);
    v10 *= v5;
    _234 += v10;
    f32 v6 = (1.0f - (v7 / 300.0f));
    if (v6 > 0.0f) {
        TVec3f v8(_234);
        v8 *= v6;
        TVec3f v9(v8);
        v9 *= 0.1f;
        _234 -= v9;
    }
}

void TripodBossLeg::addIKLimitPower() {
    TVec3f rootJoint;
    mJoint->getRootJointPosition(&rootJoint);
    TVec3f v12(rootJoint);
    v12 -= mForceEndPoint;
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
    }
    else {
        v5 = 0.89999998f;
        v6 = 5.0f;
    }

    if (v5 < v8) {
        f32 ease = MR::getEaseInOutValue(((1.0f / (1.0f - v5))) * (v8 - v5), 0.0f, 1.0f, 1.0f);
        TVec3f v9(v12);
        v9 *= ease;
        TVec3f v10(v9);
        v10 *= v6;
        _234 += v10;
    }
}

void TripodBossLeg::addAccelUpLeg(TripodBossStepPoint *pPoint, f32 a3) {
    TVec3f v8(pPoint->mStepNormal);
    TVec3f v7(v8);
    v7 *= a3;
    _234 += v7;
    addIKLimitPower();
    mForceEndPoint += _234;
    _234.x *= 0.89999998f;
    _234.y *= 0.89999998f;
    _234.z *= 0.89999998f;
    updatePose();
}

f32 TripodBossLeg::calcLegHeight(TripodBossStepPoint *pPoint) const {
   TVec3f v4(mForceEndPoint);
   v4 -= pPoint->mStepPosition;
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
        case 0:
        {
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
    TVec3f v4(endJointPos);
    v4 -= v6;

    if (mMoveArea->collideSphere(&hitResult, v6, 0.0f, v4)) {
        mForceEndPoint = hitResult._C;
        if (_234.dot(hitResult._18) < 0.0f) {
            f32 dot = hitResult._18.dot(_234);
            JMAVECScaleAdd(&hitResult._18, &_234, &_234, -dot);
        }

        return true;
    }

    return false;
}

void TripodBossLeg::separateLocalAxisRootJoint(const TPos3f &rPos) {
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
    TVec3f* stepPos = &_98->mStepPosition;
    TVec3f v6(mForceEndPoint);
    v6 -= *stepPos;
    f32 v3 = v6.dot(_98->mStepNormal);
    f32 v4 = MR::normalize((v3 / _250), 0.15000001f, 1.0f);
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
    v9.set<f32>(mEndJointMtx.get(0, 0), mEndJointMtx.get(1, 0), mEndJointMtx.get(2, 0));
    v8.set<f32>(mEndJointMtx.get(0, 1), mEndJointMtx.get(1, 1), mEndJointMtx.get(2, 1));
    v7.set<f32>(mEndJointMtx.get(0, 2), mEndJointMtx.get(1, 2), mEndJointMtx.get(2, 2));
    _1C0.setVec(-v7, -v9, v8);

    TVec3f v2(landingNormal);
    v2 *= 630.0f;
    TVec3f v3(mForceEndPoint);
    v3 -= v2;
    _1C0.setTrans(v3);
}

namespace MR {
    void separateMatrixRotateYZX(TPos3f *a1, TPos3f *a2, const TPos3f &a3, const TPos3f &a4) {
        TPos3f v17;
        v17.invert(a3);
        v17.concat(v17, a4);
        TVec3f v16;
        f32 z = v17.mMtx[2][0];
        f32 y = v17.mMtx[1][0];
        f32 x = v17.mMtx[0][0];
        v16.set<f32>(x, y, z);
        f32 v10 = MR::speedySqrtf((v16.x * v16.x) + (v16.z * v16.z));

        if (MR::isNearZero(v10, 0.000001f)) {
            if (v16.y >= 0.0f) {
                a1->setXDir(0.0f, 1.0f, 0.0f);
                a1->setYDir(-1.0f, 0.0f, 0.0f);
                
            }
            else {
                a1->setXDir(0.0f, -1.0f, 0.0f);
                a1->setYDir(1.0f, 0.0f, 0.0f);
            }

            
            a1->setZDir(0.0f, 0.0f, 1.0f);
            a1->concat(a3, *a1);
            a2->set(*a1);
        }
        else {
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
};
