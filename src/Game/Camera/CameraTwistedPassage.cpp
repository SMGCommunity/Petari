#include "Game/Camera/CameraTwistedPassage.hpp"
#include "Game/Camera/CamTranslatorTwistedPassage.hpp"
#include "Game/Camera/CameraDirector.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraRailHolder.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/MathUtil.hpp"

void CameraTwistedPassage_FORCE_MATCH_SDATA2() {
    (void)0.0f;
}

namespace {
    static const f32 sUpVecBlend = 0.97f;
};  // namespace

CameraTwistedPassage::CameraTwistedPassage(const char* pName) : Camera(pName), mRailRider(), mDistMin(500.0f), mDistMax(1300.0f), mIsReversed() {
}

void CameraTwistedPassage::reset() {
    CameraLocalUtil::setPos(this, CameraLocalUtil::getPos(mCameraMan));
    CameraLocalUtil::setWatchPos(this, CameraLocalUtil::getWatchPos(mCameraMan));
    CameraLocalUtil::setUpVecAndWatchUpVec(this, -CameraLocalUtil::getTarget(this)->getGravityVector());

    if (mRailRider != nullptr) {
        initStartPos();
    }
}

CameraTargetObj* CameraTwistedPassage::calc() {
    if (mRailRider == nullptr) {
        return nullptr;
    }

    TVec3f watchPos;
    CameraLocalUtil::makeWatchPoint(&watchPos, this, CameraLocalUtil::getTarget(this), 0.1f / 15.0f);
    CameraLocalUtil::setWatchPos(this, watchPos);

    f32 nearCoord = calcNearestCoord(CameraLocalUtil::getTarget(this)->getPosition());
    f32 coord = mRailRider->mCoord;

    if (mIsReversed) {
        if (coord - nearCoord < mDistMin) {
            coord = nearCoord + mDistMin;
        } else if (coord - nearCoord > mDistMax) {
            coord = nearCoord + mDistMax;
        }
    } else {
        if (nearCoord - coord < mDistMin) {
            coord = nearCoord - mDistMin;
        } else if (nearCoord - coord > mDistMax) {
            coord = nearCoord - mDistMax;
        }
    }

    if (mRailRider->isLoop()) {
        while (coord >= mRailRider->getTotalLength()) {
            coord -= mRailRider->getTotalLength();
        }

        while (coord < 0.0f) {
            coord += mRailRider->getTotalLength();
        }
    } else {
        coord = MR::clamp(coord, 0.0f, mRailRider->getTotalLength());
    }

    f32 speed = coord - mRailRider->mCoord;

    if (speed >= 0.0f) {
        mRailRider->setSpeed(speed);
        mRailRider->move();
    } else {
        mRailRider->reverse();
        mRailRider->setSpeed(-speed);
        mRailRider->move();
        mRailRider->reverse();
    }

    CameraLocalUtil::setPos(this, mRailRider->mCurPos);
    TVec3f up = CameraLocalUtil::getUpVec(this) * ::sUpVecBlend - CameraLocalUtil::getTarget(this)->getGravityVector() * (1.0f - ::sUpVecBlend);
    CameraLocalUtil::setUpVecAndWatchUpVec(this, up);

    return CameraLocalUtil::getTarget(this);
}

void CameraTwistedPassage::setParam(s32 zoneId, s32 railId, f32 distMin, f32 distMax) {
    mRailRider = MR::getCameraDirector()->mRailHolder->getRider(zoneId, railId);
    mDistMin = distMin;
    mDistMax = distMax;
}

CamTranslatorBase* CameraTwistedPassage::createTranslator() {
    return new CamTranslatorTwistedPassage(this);
}

void CameraTwistedPassage::initStartPos() {
    mRailRider->moveToNearestPos(CameraLocalUtil::getTarget(this)->getPosition());
    if (mRailRider->mCoord < mRailRider->calcNearestPos(CameraLocalUtil::getPos(this))) {
        mIsReversed = true;
    } else {
        mIsReversed = false;
    }
}

f32 CameraTwistedPassage::calcNearestCoord(const TVec3f& rPos) const {
    f32 nearCoord = mRailRider->calcNearestPos(rPos);

    f32 coordHi1 = nearCoord;
    f32 coordLo1 = nearCoord - 110.0f;
    f32 coord = calcNearestCoordBetweenTwoRailCoord(coordHi1, coordLo1, rPos);
    if (coord >= coordLo1 || coord <= coordHi1) {
        return coord;
    }

    f32 coordHi2 = nearCoord;
    f32 coordLo2 = nearCoord + 110.0f;
    coord = calcNearestCoordBetweenTwoRailCoord(coordHi2, coordLo2, rPos);
    if (coord >= coordHi2 || coord <= coordLo2) {
        return coord;
    }

    return coordHi2;
}

f32 CameraTwistedPassage::calcNearestCoordBetweenTwoRailCoord(f32 coordA, f32 coordB, const TVec3f& rPos) const {
    TVec3f posA;
    mRailRider->calcPosAtCoord(&posA, coordA);
    TVec3f posB;
    mRailRider->calcPosAtCoord(&posB, coordB);

    TVec3f diff = posB - posA;
    f32 dist = diff.length();
    MR::normalizeOrZero(&diff);
    f32 dot = diff.dot(rPos - posA);
    return coordA + (coordB - coordA) * (dot / dist);
}
