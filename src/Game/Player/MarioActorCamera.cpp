#include "Game/Player/MarioActor.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioSwim.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Util/MathUtil.hpp"

MtxPtr MarioActor::getMapBaseMtx() const {
    HitSensor* pSensor = mMario->_46C->mSensor;
    if (pSensor == nullptr) {
        return nullptr;
    }
    return pSensor->mActor->getBaseMtx();
}

bool MarioActor::isFastRise() const {
    if (_934 && _938.dot(getGravityVec()) < -60.0f) {
        return true;
    }
    return false;
}

// regswap
bool MarioActor::isLongDrop() const {
    if (mMario->mMovementStates._2 == false) {
        return true;
    }
    return _484.y > 1000.0f;
}

bool MarioActor::isFastDrop() const {
    return mMario->mMovementStates._B;
}

void MarioActor::getFrontVec(TVec3f *pRet) const {
    *pRet = mMario->mFrontVec;
}

void MarioActor::getSideVec(TVec3f *pRet) const {
    *pRet = mMario->mSideVec;
}

void MarioActor::getUpVec(TVec3f *pRet) const {
    *pRet = mUpVec;
}

const TVec3f& MarioActor::getLastMove() const {
    return mLastMove;
}

void MarioActor::getLastMove(TVec3f *pRet) const {
    *pRet = mLastMove;
}

const TVec3f& MarioActor::getTransForCamera() const {
    return mTransForCamera;
}

// regswap + stack shenanigans
void MarioActor::updateTransForCamera() {
    if (_EA5 == false) {
        if (isNerve(&NrvMarioActor::MarioActorNrvGameOverBlackHole::sInstance)) {
            mTransForCamera = (_F50 * 0.1f).translate(mTransForCamera * 0.9f);
            return;
        }
        if (mMario->isStatusActive(0x10) || mMario->isStatusActive(0x1e)) {
            mTransForCamera = (mTransForCamera * 0.8f).translate(mPosition * 0.2f);
            return;
        }
        mTransForCamera = (mMario->_13C).translate(mPosition);
        if (mMario->isSwimming()) {
            bool var = false;
            if (mMario->mSwim->_19 || mMario->mSwim->_20) {
                var = true;
            }
            if (var) {
                if (_332 < 100) {
                    _332++;
                }
                goto end;
            }
        }
        if (_332 != 0) {
            _332--;
        }
        end: 
            //TVec3f stack_8 = mMario->mHeadVec * (f32)_332;
            mTransForCamera += (mMario->mHeadVec * (f32)_332);
    }
    return;
}

// regswap
void MarioActor::updateForCamera() {
    if (_EA5 == false) {
        if (_300.epsilonEquals(mMario->mHeadVec, 0.0000038146973f) == false) {
            _300 = mMario->mHeadVec;
            _330 = mConst->mTable[mConst->mCurrentTable]->mCameraHeadRotationTimer;
        }
        if (_330 != 0) {
            u16 var = _330 - 1;
            _330 = var;
            if (MR::vecBlendSphere(mUpVec, _300, &mUpVec, 1.0f - (f32)var / (f32)mConst->mTable[mConst->mCurrentTable]->mCameraHeadRotationTimer) == 0) {
                mUpVec = _300;
            }
        }
        mFrontVec = mMario->mFrontVec;
        mSideVec = mMario->mSideVec;
    }
}
