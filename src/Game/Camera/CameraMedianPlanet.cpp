#include "Game/Camera/CameraMedianPlanet.hpp"
#include "Game/Camera/CamTranslatorMedianPlanet.hpp"
#include "Game/Camera/CameraCalc.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "JSystem/JGeometry/TUtil.hpp"

void CameraMedianPlanet_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)MR::epsilon();
    (void)MR::pi();
}

namespace {
    static const f32 sAngleConstrainRate = 0.2f;
    static const f32 sPartialRate = 0.5f;
};  // namespace

CameraMedianPlanet::~CameraMedianPlanet() {
}

CameraMedianPlanet::CameraMedianPlanet(const char* pName)
    : Camera(pName), mString(), mDistMin(1200.0f), mDistMax(3000.0f), mAngleMin(MR::pi() / 4.0f), mPeepAngle(), mDist(2000.0f),
      mAngleConstrainRate(::sAngleConstrainRate), mPartialRate(::sPartialRate) {
}

void CameraMedianPlanet::reset() {
    CameraLocalUtil::setPos(this, CameraLocalUtil::getPos(mCameraMan));
    CameraLocalUtil::setWatchPos(this, CameraLocalUtil::getWatchPos(mCameraMan));
    CameraLocalUtil::setUpVec(this, CameraLocalUtil::getUpVec(mCameraMan));
    CameraLocalUtil::setWatchUpVec(this, CameraLocalUtil::getTarget(this)->getUpVec());
}

CameraTargetObj* CameraMedianPlanet::calc() {
    TVec3f pos;
    MR::getCameraInvViewMtx().getTrans(pos);
    constrainDipAngle(&pos);
    constrainLength(&pos);
    TVec3f watchPos;
    calcWatchPos(&watchPos);
    TVec3f up;
    if (!calcUpVec(&up, watchPos, pos)) {
        return CameraLocalUtil::getTarget(this);
    }

    CameraLocalUtil::setPos(this, pos);
    CameraLocalUtil::setWatchPos(this, watchPos);
    CameraLocalUtil::setUpVec(this, up);
    CameraLocalUtil::setWatchUpVec(this, CameraLocalUtil::getTarget(this)->getUpVec());
    return CameraLocalUtil::getTarget(this);
}

CamTranslatorBase* CameraMedianPlanet::createTranslator() {
    return new CamTranslatorMedianPlanet(this);
}

void CameraMedianPlanet::getPrevFront(TVec3f* pPrevFront) {
    MR::getCameraInvViewMtx().getZDir(*pPrevFront);
    pPrevFront->negate();
}

void CameraMedianPlanet::constrainDipAngle(TVec3f* pPos) {
    TPos3f targetMtx;
    makeTargetMtx(&targetMtx);
    peepMtx(&targetMtx);

    TPos3f inv = targetMtx;
    inv.invert(inv);
    inv.mult(*pPos, *pPos);

    f32 length, angleX, angleY;
    MR::crossToPolar(TVec3f(0.0f, 0.0f, 0.0f), *pPos, &length, &angleX, &angleY);

    if (angleX < mAngleMin) {
        angleX += (mAngleMin - angleX) * (CameraLocalUtil::isForceCameraChange() ? 1.0f : mAngleConstrainRate);
        MR::polarToCross(TVec3f(0.0f, 0.0f, 0.0f), pPos, length, angleX, angleY);
    }
    targetMtx.mult(*pPos, *pPos);
}

void CameraMedianPlanet_FORCE_MATCH_SDATA2_2() {
    (void)(0.1f / 15.0f);
}

void CameraMedianPlanet::makeTargetMtx(TPos3f* pMtx) {
    pMtx->setXDir(CameraLocalUtil::getTarget(this)->getSideVec());
    pMtx->setYDir(-CameraLocalUtil::getTarget(this)->getGravityVector());
    pMtx->setZDir(CameraLocalUtil::getTarget(this)->getFrontVec());
    pMtx->setTrans(CameraLocalUtil::getTarget(this)->getPosition());
}

void CameraMedianPlanet::peepMtx(TPos3f* pMtx) {
    TVec3f up;
    pMtx->getYDir(up);

    TVec3f front = CameraLocalUtil::getTarget(this)->getPosition() - CameraLocalUtil::getVecReg(mString);
    if (MR::isNearZero(front)) {
        return;
    }

    f32 dist = front.length();
    if (dist >= mDist) {
        return;
    }

    TVec3f side = up.cross(front);
    if (MR::isNearZero(side)) {
        return;
    }
    MR::normalize(&side);

    rotate33(pMtx, side, mPeepAngle * (mDist - dist) / mDist);
}

void CameraMedianPlanet::rotate33(TPos3f* pMtx, const TVec3f& axis, f32 angle) {
    TVec3f trans;
    pMtx->getTrans(trans);
    pMtx->setTrans(0.0f, 0.0f, 0.0f);
    TPos3f rot;
    rot.makeRotate(axis, angle);
    pMtx->concat(rot, *pMtx);
    pMtx->setTrans(trans);
}

void CameraMedianPlanet::constrainLength(TVec3f* pPos) {
    f32 rate = getMaxRate();
    TVec3f offset = *pPos - CameraLocalUtil::getTarget(this)->getPosition();
    offset.setLength(mDistMin * (1.0f - rate) + mDistMax * rate);
    pPos->set(CameraLocalUtil::getTarget(this)->getPosition() + offset);
}

f32 CameraMedianPlanet::getMaxRate() {
    TVec3f up;
    getRegisterUpVec(&up);
    f32 rate = up.angle(CameraLocalUtil::getTarget(this)->getUpVec()) / MR::pi();
    if (rate > 1.0f) {
        rate = 1.0f;
    }
    if (rate < 0.0f) {
        rate = 0.0f;
    }
    return rate;
}

void CameraMedianPlanet::getRegisterUpVec(TVec3f* pUp) {
    MR::calcGravityVector(this, CameraLocalUtil::getVecReg(mString), pUp, nullptr, 0);
    pUp->negate();
    if (MR::isNearZero(*pUp)) {
        pUp->set(CameraLocalUtil::getTarget(this)->getUpVec());
    }
}

void CameraMedianPlanet::calcWatchPos(TVec3f* pWatchPos) {
    TVec3f watchOffset;
    CameraLocalUtil::makeWatchPoint(&watchOffset, this, CameraLocalUtil::getTarget(this), 0.1f / 15.0f);
    watchOffset.sub(CameraLocalUtil::getTarget(this)->getPosition());

    pWatchPos->set(CameraLocalUtil::getTarget(this)->getPosition() * (1.0f - mPartialRate) + CameraLocalUtil::getVecReg(mString) * mPartialRate +
                   watchOffset);
}

bool CameraMedianPlanet::calcUpVec(TVec3f* pUp, const TVec3f& watchPos, const TVec3f& pos) {
    MR::getCameraInvViewMtx().getYDir(*pUp);
    TVec3f prevFront;
    getPrevFront(&prevFront);

    TVec3f front = watchPos - pos;
    if (MR::isNearZero(front)) {
        return false;
    }
    MR::normalize(&front);

    TQuat4f rot;
    rot.setRotate(prevFront, front);
    rot.transform(*pUp);

    return true;
}
