#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioState.hpp"
#include "Game/Player/MarioSwim.hpp"
#include "Game/Util/MathUtil.hpp"

MtxPtr MarioActor::getMapBaseMtx() const {
    HitSensor* pSensor = mMario->_46C->mSensor;
    if (pSensor == nullptr) {
        return nullptr;
    }

    return pSensor->mHost->getBaseMtx();
}

bool MarioActor::isFastRise() const {
    if (_934) {
        if (_938.dot(getGravityVec()) < -60.0f) {
            return true;
        }
    }

    return false;
}

bool MarioActor::isLongDrop() const {
    // FIXME
    if (mMario->getMovementStates()._2) {
        return true;
    }

    return mMario->mVerticalSpeed > 1000.0f;
}

bool MarioActor::isFastDrop() const {
    return mMario->getMovementStates()._B;
}

u32 MarioActor::getSpecialMode() const {
    u32 mode = 0;
    if (mMario->getMovementStates().jumping && mMario->getMovementStates()._F) {
        mode = 1;
    }

    if (mMario->getMovementStates().jumping && mMario->getMovementStates()._11) {
        mode = 1;
    }

    if (mode == 0) {
        return mode;
    }

    if (mMario->mVerticalSpeed < 300.0f) {
        mode = 2;
    }

    if (mMario->mVerticalSpeed < 800.0f) {
        return mode;
    }

    return 3;
}

bool MarioActor::isCameraStateOn(SPECIAL_STATUS_FOR_CAMERA status) const {
    switch (status) {
    case CASE_0:
        if (mMario->isStatusActive(MarioStatus_FireDance)) {
            return true;
        }
        break;

    case CASE_1:
        if (isJumping() && mMario->_430 == 11) {
            return true;
        }
        break;
    }

    return false;
}

void MarioActor::getFrontVec(TVec3f* pVec) const {
    *pVec = mMario->mFrontVec;
}

void MarioActor::getSideVec(TVec3f* pVec) const {
    *pVec = mMario->mSideVec;
}

void MarioActor::getUpVec(TVec3f* pVec) const {
    *pVec = mUpVec;
}

void MarioActor::getLastMove(TVec3f* pVec) const {
    *pVec = mLastMove;
}

const TVec3f& MarioActor::getLastMove() const {
    return mLastMove;
}

const TVec3f& MarioActor::getTransForCamera() const {
    return mCameraTrans;
}

void MarioActor::updateTransForCamera() {
    if (_EA5) {
        return;
    }

    if (isNerve(&NrvMarioActor::MarioActorNrvGameOverBlackHole::sInstance)) {
        mCameraTrans = mCameraTrans * 0.9f + mBlackHolePosition * 0.1f;
        return;
    }

    if (mMario->isStatusActive(MarioStatus_Step) || mMario->isStatusActive(MarioStatus_Bump)) {
        mCameraTrans = mPosition * 0.2f + mCameraTrans * 0.8f;
        return;
    }

    mCameraTrans = mMario->_13C + mPosition;  // FIXME: missing TVec3f ctor

    if (mMario->isSwimming() && mMario->mSwim->isOnWaterSurface()) {
        if (_332 < 100) {
            _332++;
        }
    } else if (_332 != 0) {
        _332--;
    }

    mCameraTrans += mMario->mHeadVec * _332;
}

void MarioActor::updateForCamera() {
    if (_EA5) {
        return;
    }

    if (!(_300 == mMario->mHeadVec)) {
        _300 = mMario->mHeadVec;
        _330 = mConst->getTable()->mCameraHeadRotationTimer;
    }

    if (_330 != 0) {
        _330--;

        if (!MR::vecBlendSphere(mUpVec, _300, &mUpVec, 1.0f - static_cast< f32 >(_330) / mConst->getTable()->mCameraHeadRotationTimer)) {
            mUpVec = _300;
        }
    }

    _2DC = mMario->mFrontVec;
    _2E8 = mMario->mSideVec;
}
