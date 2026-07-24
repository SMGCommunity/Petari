#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioShadow.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"

const TVec3f& MarioActor::getGravityVec() const {
    return *mMario->getGravityVec();
}

const TVec3f& MarioActor::getGravityVector() const {
    return *mMario->getGravityVec();
}

const TVec3f& MarioActor::getAirGravityVec() const {
    return mMario->getAirGravityVec();
}

void MarioActor::getGravityVector(TVec3f* pVec) const {
    pVec->set(mMario->getAirGravityVec());
}

f32 MarioActor::getGravityRatio() const {
    return mGravityRatio;
}

GravityInfo* MarioActor::getGravityInfo() const {
    return mGravityInfo;
}

u8 MarioActor::getGravityLevel() const {
    return MR::isLightGravity(*mGravityInfo);
}

bool MarioActor::checkBeeWallStick(TVec3f& rVec) {
    if (mMario->getMovementStates()._8 && mMario->checkWallCode("Fur", false) && !mAlphaEnable && !_9F2) {
        TVec3f vec20 = mMario->mHeadVec;

        mAlphaEnable = 5;

        rVec = -mMario->getWallNorm();

        mPosition = mMario->getWallPos();

        mMario->stopJump();
        mMario->stopAnimation(nullptr);
        mMario->stopWalk();

        _240 = rVec;
        mMario->setGravityVec(rVec);
        mMario->setHeadVec(-rVec);
        mMario->setFrontVecKeepUp(vec20, 1UL);
        setBlendMtxTimer(2);

        _38C = 5;
        mMario->mMovementStates._31 = false;
        _214->_305 = true;

        return true;
    }

    return false;
}

bool MarioActor::checkBeeFloorStick(TVec3f& rVec) {
    if (mMario->getMovementStates()._1 && !strcmp("Fur", MR::getWallCodeString(mMario->getGroundPolygon())) && mAlphaEnable == 0) {
        mAlphaEnable = 5;
        rVec = -mMario->_368;

        return true;
    }

    return false;
}

void MarioActor::syncJumpBeeStickMode() {
    if (mAlphaEnable != 0 && selectQuickResetBeeWallGravity(mMario->_45C->mSensor->mHost->mName)) {
        mAlphaEnable = 0;
        _9F2 = 30;
    }

    if (!(MR::diffAngleAbs(_360, getGravityVec()) < MR::pi() / 36.0f)) {
        return;
    }

    _33C = mPosition - mMario->mFrontVec * 2.0f;
    _354 = mPosition - _33C;
    _360 = _2A0 - _33C;
    MR::normalize(&_360);
}

void MarioActor::updateBeeModeGravity(TVec3f& rVec) {
    u8 alpha = mAlphaEnable;

    if (_9F2 != 0) {
        _9F2--;
    }

    if (_9F2 != 0 || (!checkBeeCeilStick(rVec) && !checkBeeWallStick(rVec) && !checkBeeFloorStick(rVec))) {
        updateBeeStickMode(rVec);
    }

    if (mAlphaEnable == 5) {
        MR::vecBlendSphere(_240, rVec, &rVec, 0.1f);
        return;
    }

    if (mAlphaEnable != 0) {
        MR::vecBlendSphere(_240, rVec, &rVec, 0.3f);
        return;
    }

    if (alpha != 0) {
        _9F2 = 60;
    }
}

bool MarioActor::isInZeroGravitySpot() const {
    return MR::isNearZero(_24C);
}

// void MarioActor::updateGravityVec(bool, bool) {}

bool MarioActor::checkBeeCeilStick(TVec3f& rVec) {
    if ((mMario->isCeiling() || mMario->mDrawStates._15) && mAlphaEnable == 0 && _9F2 == 0) {
        bool out = false;
        if (mMario->mDrawStates._15) {
            out = true;
        } else {
            const char* wallCodeString = MR::getWallCodeString(mMario->_4C8);
            if (wallCodeString != nullptr && !strcmp(wallCodeString, "Fur")) {
                out = true;
            }
        }

        if (out) {
            Triangle triangle = Triangle();
            TVec3f vec;
            if (MR::getFirstPolyOnLineToMap(&vec, &triangle, mPosition, (-_240).multiplyOperatorInline(200.0f))) {
                entryWallWalkMode(vec, *(triangle.getNormal(0)));
            }
        }
    }

    return false;
}

// void MarioActor::updateBeeStickMode(TVec3f& rVec) {}
