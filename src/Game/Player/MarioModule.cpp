#include "Game/Player/MarioModule.hpp"
#include "Game/Enemy/KarikariDirector.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioMapCode.hpp"
#include "Game/Player/MarioState.hpp"
#include "revolution/types.h"

Mario* MarioModule::getPlayer() const {
    return mActor->mMario;
}

MarioAnimator* MarioModule::getAnimator() const {
    return mActor->mMarioAnim;
}

TVec3f& MarioModule::getShadowPos() const {
    return mActor->mMario->mShadowPos;
}

const TVec3f& MarioModule::getGravityVec() const {
    return *mActor->mMario->getGravityVec();
}

const TVec3f& MarioModule::getAirGravityVec() const {
    return mActor->mMario->getAirGravityVec();
}

TVec3f& MarioModule::getFrontVec() const {
    return mActor->mMario->mFrontVec;
}

bool MarioModule::isStatusActiveID(u32 id) const {
    return mActor->mMario->isStatusActive(id);
}

bool MarioModule::isStatusActiveS(MarioState* pState) const {
    return mActor->mMario->isStatusActive(pState->mStatusId);
}

u16 MarioModule::getPlayerMode() const {
    return mActor->mPlayerMode;
}

bool MarioModule::isPlayerModeBee() const {
    return mActor->mPlayerMode == 4;
}

bool MarioModule::isPlayerModeIce() const {
    return mActor->mPlayerMode == 3;
}

bool MarioModule::isPlayerModeTeresa() const {
    return mActor->mPlayerMode == 6;
}

bool MarioModule::isPlayerModeHopper() const {
    return mActor->mPlayerMode == 5;
}

bool MarioModule::isPlayerModeInvincible() const {
    return mActor->mPlayerMode == 1;
}

bool MarioModule::isPlayerModeFoo() const {
    return mActor->mPlayerMode == 7;
}

void MarioModule::changeAnimation(const char* pAnim1, const char* pAnim2) {
    if (!mActor->_B90) {
        if (pAnim2) {
            mActor->mMarioAnim->changeDefault(pAnim2);
        }
        if (pAnim1) {
            mActor->mMarioAnim->change(pAnim1);
        }
    }
}

void MarioModule::changeAnimationNonStop(const char* pAnim) {
    if (!mActor->_B90) {
        if (pAnim) {
            changeAnimation(pAnim, (const char*)nullptr);
        }
        if (!mActor->mMarioAnim->mXanimePlayer->_20->mLoopMode) {
            mActor->mMarioAnim->mXanimePlayer->_20->mLoopMode = 1;
        }
    }
}

void MarioModule::changeAnimationWithAttr(const char* pAnim, u32 attr) {
    if (!mActor->_B90) {
        if (pAnim) {
            changeAnimation(pAnim, (const char*)nullptr);
        }
        mActor->mMarioAnim->mXanimePlayer->_20->mLoopMode = attr;
    }
}

void MarioModule::stopAnimation(const char* pAnim1, const char* pAnim2) {
    if (!mActor->_B90) {
        if (pAnim2) {
            mActor->mMarioAnim->changeDefault(pAnim2);
        }
        if (pAnim1) {
            mActor->mMarioAnim->mXanimePlayer->stopAnimation(pAnim1);
        } else {
            mActor->mMarioAnim->mXanimePlayer->stopAnimation();
        }
    }
}

bool MarioModule::isDefaultAnimationRun(const char* pAnim) const {
    return mActor->mMarioAnim->isDefaultAnimationRun(pAnim);
}

void MarioModule::changeAnimationInterpoleFrame(u32 a1) {
    return mActor->mMarioAnim->mXanimePlayer->changeInterpoleFrame(a1);
}

bool MarioModule::isAnimationRun(const char* pAnimName) const {
    if (!pAnimName) {
        XanimePlayer* pXanimePlayer = mActor->mMarioAnim->mXanimePlayer;
        return pXanimePlayer->mCurrentAnimation != pXanimePlayer->mDefaultAnimation;
    } else {
        bool run = mActor->mMarioAnim->mXanimePlayer->isRun(pAnimName);
        if (mActor->mMarioAnim->_6C) {
            run |= mActor->mMarioAnim->mXanimePlayerUpper->isRun(pAnimName);
        }
        return run;
    }
}

bool MarioModule::isAnimationTerminate(const char* pAnim) const {
    if (pAnim) {
        return mActor->mMarioAnim->mXanimePlayer->isTerminate(pAnim);
    }
    return mActor->mMarioAnim->mXanimePlayer->isTerminate();
}

bool MarioModule::isAnimationTerminateUpper(const char* pAnim) const {
    if (pAnim) {
        return mActor->mMarioAnim->mXanimePlayerUpper->isTerminate(pAnim);
    }
    return mActor->mMarioAnim->mXanimePlayerUpper->isTerminate();
}

const char* MarioModule::getAnimationStringPointer(const char* pAnim) const {
    return mActor->mMarioAnim->mXanimePlayer->getNameStringPointer(pAnim);
}

const char* MarioModule::getCurrentBckName() const {
    if (mActor->mMarioAnim->mXanimePlayer->isAnimationRunSimple()) {
        return mActor->mMarioAnim->mXanimePlayer->getCurrentBckName();
    } else {
        return mActor->mMarioAnim->mCurrBck;
    }
}

void MarioModule::changeAnimationUpper(const char* pAnim1, const char* pAnim2) {
    if (!mActor->_B90) {
        mActor->mMarioAnim->changeDefaultUpper(pAnim2);
        if (pAnim1) {
            mActor->mMarioAnim->changeUpper(pAnim1);
        }
    }
}

void MarioModule::changeAnimationUpperWeak(const char* pAnim1, const char* pAnim2) {
    if (!mActor->mMarioAnim->_6C && !mActor->_B90) {
        mActor->mMarioAnim->changeDefaultUpper(pAnim2);
        if (pAnim1) {
            mActor->mMarioAnim->changeUpper(pAnim1);
        }
    }
}

void MarioModule::stopAnimationUpper(const char* pAnim1, const char* pAnim2) {
    if (!mActor->_B90) {
        mActor->mMarioAnim->changeDefaultUpper(pAnim2);
        mActor->mMarioAnim->stopUpper(pAnim1);
    }
}

void MarioModule::stopAnimationUpperForce() {
    mActor->mMarioAnim->stopUpper(nullptr);
}

f32 MarioModule::getAnimationFrame() const {
    return mActor->mMarioAnim->getFrame();
}

// regswap
void MarioModule::changeAnimation(const char* pAnim, u32 index) {
    if (!mActor->_B90) {
        changeAnimation(pAnim, (char*)nullptr);
        mActor->mMario->_A6C[index] = 1;
    }
}

// regswap again (see above)
bool MarioModule::isAnimationRun(const char* pAnim, u32 index) {
    if (mActor->mMario->_A6C[index] == 0) {
        return false;
    }

    bool isAnimRun = isAnimationRun(pAnim);

    if (!isAnimRun) {
        mActor->mMario->_A6C[index] = 0;
    }

    return isAnimRun;
}

// regwap again
void MarioModule::stopAnimation(const char* pAnim, u32 index) {
    if (!mActor->_B90 && mActor->mMario->_A6C[index]) {
        stopAnimation(pAnim, (char*)nullptr);
        mActor->mMario->_A6C[index] = 0;
    }
}

void MarioModule::setJointGlobalMtx(u16 a1, MtxPtr mtx) {
    XjointTransform* pTransform = mActor->mMarioAnim->mXanimePlayer->mCore->mTransformList;
    if (pTransform == 0) {
        pTransform = 0;
    } else {
        pTransform += a1;
    }
    pTransform->_64 = mtx;
}

void MarioModule::setYangleOffset(f32 offset) {
    mActor->mMario->mYAngleOffset = offset;
}

bool MarioModule::isStickOn() const {
    return !MR::isNearZero(mActor->mMario->mStickPos.z);
}

bool MarioModule::isStickFull() const {
    return mActor->mMario->mStickPos.z > 0.9f;
}

TVec3f& MarioModule::getWorldPadDir() const {
    return mActor->mMario->mWorldPadDir;
}

// regswap
bool MarioModule::calcWorldPadDir(TVec3f* pDest, f32 a2, f32 a3, bool a4) {
    pDest->zero();
    if (MR::isNearZero(a2) && MR::isNearZero(a3)) {
        return false;
    }
    if (!mActor->mMario->_10._11) {
        if (__fabsf(a3) > mActor->mConst->getTable()->mStickMarginYstart) {
            if (__fabsf(a2) < mActor->mConst->getTable()->mStickMarginX) {
                a2 = 0.0f;
            } else if (a2 > 0.0f) {
                a2 = (a2 - mActor->mConst->getTable()->mStickMarginX) / (1.0f - mActor->mConst->getTable()->mStickMarginX);
            } else {
                a2 = (a2 + mActor->mConst->getTable()->mStickMarginX) / (1.0f - mActor->mConst->getTable()->mStickMarginX);
            }
        } else if (__fabsf(a2) > mActor->mConst->getTable()->mStickMarginXstart) {
            if (__fabsf(a3) < mActor->mConst->getTable()->mStickMarginY) {
                a3 = 0.0f;
            } else if (a3 > 0.0f) {
                a3 = (a3 - mActor->mConst->getTable()->mStickMarginY) / (1.0f - mActor->mConst->getTable()->mStickMarginY);
            } else {
                a3 = (a3 + mActor->mConst->getTable()->mStickMarginY) / (1.0f - mActor->mConst->getTable()->mStickMarginY);
            }
        }
    }
    mActor->mMario->calcMoveDir(a2, a3, pDest, a4);
    return true;
}

void MarioModule::addVelocity(const TVec3f& rAdd) {
    mActor->mMario->mVelocity += rAdd;
}

#ifdef __MWERKS__
void MarioModule::addVelocity(const register TVec3f& rAdd, register f32 scale) {
    register TVec3f* pVelocity = &mActor->mMario->mVelocity;
    asm {
        psq_l f4, 0(rAdd), 0, 0
        psq_l f2, 8(rAdd), 1, 0
        psq_l f3, 0(pVelocity), 0, 0
        psq_l f0, 8(pVelocity), 1, 0
        ps_madds0 f3, f4, scale, f3
        ps_madds0 f0, f2, scale, f0
        psq_st f3, 0(pVelocity), 0, 0
        psq_st f0, 8(pVelocity), 1, 0
    }
}
#endif

void MarioModule::addVelocityAfter(const TVec3f& rAdd) {
    mActor->mMario->mVelocityAfter += rAdd;
}

void MarioModule::clearVelocity() {
    mActor->mMario->mVelocity.zero();
}

TVec3f& MarioModule::getVelocity() const {
    return mActor->mMario->mVelocity;
}

void MarioModule::addTrans(const TVec3f& rShift, const char* pName) {
    mActor->mMario->addTrans(rShift, pName);
}

// regswap
void MarioModule::cutGravityElementFromJumpVec(bool a1) {
    if (a1) {
        MR::vecKillElement(mActor->mMario->mJumpVec, mActor->_240, &mActor->mMario->mJumpVec);
    } else {
        TVec3f* pJumpVec = &mActor->mMario->mJumpVec;
        MR::vecKillElement(*pJumpVec, mActor->getGravityVec(), pJumpVec);
    }
}

void MarioModule::cutVecElementFromJumpVec(const TVec3f& rCut) {
    MR::vecKillElement(mActor->mMario->mJumpVec, rCut, &mActor->mMario->mJumpVec);
}

TVec3f& MarioModule::getJumpVec() const {
    return mActor->mMario->mJumpVec;
}

void MarioModule::setJumpVec(const TVec3f& rVec) {
    mActor->mMario->mJumpVec.setPS2(rVec);
}

void MarioModule::playEffect(const char* pEffectName) {
    mActor->playEffect(pEffectName);
}

void MarioModule::playEffectTrans(const char* pEffectName, const TVec3f& rTrans) {
    mActor->playEffectTrans(pEffectName, rTrans);
}

void MarioModule::playEffectRT(const char* pEffectName, const TVec3f& a2, const TVec3f& a3) {
    mActor->playEffectRT(pEffectName, a2, a3);
}

void MarioModule::playEffectRTZ(const char* pEffectName, const TVec3f& a2, const TVec3f& a3) {
    mActor->playEffectRTZ(pEffectName, a2, a3);
}

void MarioModule::playEffectRTW(const char* pEffectName, const TVec3f& a2, const TVec3f& a3) {
    mActor->playEffectRTW(pEffectName, a2, a3);
}

void MarioModule::playEffectSRT(const char* pEffectName, float a2, const TVec3f& a3, const TVec3f& a4) {
    mActor->playEffectSRT(pEffectName, a2, a3, a4);
}

void MarioModule::stopEffect(const char* pEffectName) {
    mActor->stopEffect(pEffectName);
}

void MarioModule::stopEffectForce(const char* pEffectName) {
    mActor->stopEffectForce(pEffectName);
}

bool MarioModule::playSound(const char* pSoundName, s32 a2) {
    return mActor->mMario->playSoundJ(pSoundName, a2);
}

void MarioModule::stopSound(const char* pSoundName, u32 a2) {
    mActor->mMario->stopSoundJ(pSoundName, a2);
}

void MarioModule::startCamVib(u32 strength) {
    switch (strength) {
    case 1:
        MR::shakeCameraWeak();
        return;
    case 2:
        MR::shakeCameraWeak();
        return;
    case 0:
        MR::shakeCameraNormal();
        return;
    case 3:
        MR::shakeCameraNormalStrong();
        return;
    case 4:
        MR::shakeCameraStrong();
        return;
    default:
        return;
    }
}

TVec3f& MarioModule::getCamPos() const {
    return mActor->mCamPos;
}

TVec3f& MarioModule::getCamDirX() const {
    return mActor->mCamDirX;
}

TVec3f& MarioModule::getCamDirY() const {
    return mActor->mCamDirY;
}

TVec3f& MarioModule::getCamDirZ() const {
    return mActor->mCamDirZ;
}

void MarioModule::startPadVib(u32 strength) {
    switch (strength) {
    case 1:
        MR::tryRumblePadVeryWeak(mActor, 0);
        return;
    case 0:
        MR::tryRumblePadWeak(mActor, 0);
        return;
    case 2:
        MR::tryRumblePadMiddle(mActor, 0);
        return;
    case 3:
        MR::tryRumblePadStrong(mActor, 0);
        return;
    default:
        return;
    }
}

void MarioModule::startPadVib(const char* a1) {
    MR::tryRumblePad(mActor, a1, 0);
}

f32 MarioModule::getStickX() const {
    return mActor->mMario->mStickPos.x;
}

f32 MarioModule::getStickY() const {
    return mActor->mMario->mStickPos.y;
}

f32 MarioModule::getStickP() const {
    return mActor->mMario->mStickPos.z;
}

bool MarioModule::checkTrgA() const {
    return mActor->checkButtonType(3, false);
}

bool MarioModule::checkLvlA() const {
    return mActor->checkButtonType(5, false);
}

bool MarioModule::checkTrgZ() const {
    return mActor->checkButtonType(7, false);
}

bool MarioModule::checkLvlZ() const {
    return mActor->checkButtonType(9, false);
}

bool MarioModule::checkPreLvlZ() const {
    return mActor->checkButtonType(9, true);
}

bool MarioModule::checkSquat(bool a1) {
    if (MR::getKarikariClingNum() < 1 == false) {
        return false;
    }
    if (*(u32*)&mActor->_468.x) {
        return false;
    }
    if (mActor->_B94 != 0) {
        return false;
    }
    if (mActor->mAlphaEnable) {
        return false;
    }
    if (mActor->mMario->mMovementStates._21) {
        if (!mActor->checkButtonType(9, false)) {
            mActor->mMario->mMovementStates._21 = 0;
        }
        return false;
    }
    if (!a1 && mActor->mMario->_488 > 100.0f) {
        return false;
    }
    return mActor->checkButtonType(9, false);
}

TVec3f& MarioModule::getTrans() const {
    return mActor->mMario->mPosition;
}

const Triangle* MarioModule::getGroundPolygon() const {
    return mActor->mMario->mGroundPolygon;
}

bool MarioModule::isOnSlipGround() const {
    Mario* pMario = mActor->mMario;
    if (pMario->mMovementStates._1 && pMario->isSlipPolygon(pMario->mGroundPolygon)) {
        return true;
    }
    return false;
}

bool MarioModule::isSlipFloorCode(s32 a1) const {
    switch (a1) {
    case 2:
        return true;
    case 3:
    case 0xd:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x1e:
        return false;
    }
    return true;
}

bool MarioModule::isSlipPolygon(const Triangle* pTri) const {
    if (!pTri->isValid()) {
        return false;
    }

    u32 code = mActor->mMario->_95C->getCode(pTri);

    switch (code) {
    case 2:
        return !(calcAngleD(*pTri->getNormal(0)) <= 3.0f);
    case 3:
    case 0xd:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x1e:
        return false;
    }

    float angle = calcAngleD(*pTri->getNormal(0));

    return angle >= mActor->mConst->getTable()->mFlatAngle - 0.5f;
}

u32 MarioModule::getFloorCode() const {
    return mActor->mMario->_95C->getCode(mActor->mMario->mGroundPolygon);
}

f32 MarioModule::calcPolygonAngleD(const Triangle* pTri) const {
    if (pTri->isValid() == false) {
        return 0.0f;
    }
    if (MR::isNearZero(*pTri->getNormal(0))) {
        return 0.0f;
    }
    return calcAngleD(*pTri->getNormal(0));
}

f32 MarioModule::calcAngleD(const TVec3f& rVec) const {
    float f31 = (MR::acosEx(-mActor->mMario->getAirGravityVec().dot(rVec)) / 3.14159f) * 180.0f;
    if (MR::isNearZero(f31, 1.0f)) {
        f31 = 0.0f;
    }
    if (MR::isNearZero(f31 - 90.0f, 1.0f)) {
        f31 = 90.0f;
    }
    return f31;
}

Triangle* MarioModule::getTmpPolygon() const {
    return mActor->mMario->mTmpPolygon;
}

bool MarioModule::sendPunch(HitSensor* pSensor, bool a2) {
    if (!pSensor) {
        return false;
    }
    bool ret = mActor->sendPunch(pSensor, a2);
    if (ret) {
        mActor->reactionPunch(pSensor);
    }
    return ret;
}

HitSensor* MarioModule::getSensor(const Triangle* pTri) const {
    if (pTri->isValid() == false) {
        return nullptr;
    } else {
        return pTri->mSensor;
    }
}

f32 MarioModule::marioAcos(f32 a1) const {
    return MR::acosEx(a1);
}

bool MarioModule::isInputDisable() const {
    if (mActor->mMario->mMovementStates._22) {
        return true;
    }
    if (mActor->mMario->isStatusActive(18)) {
        return true;
    }
    if (isAnimationRun("ハード着地")) {  // "Hard landing"
        return true;
    }
    if (isAnimationRun("中ダメージ着地")) {
        return true;
    }
    if (isAnimationRun("中後ダメージ着地")) {
        return true;
    }
    if (isAnimationRun("ステージインB")) {
        return true;
    }
    return mActor->_3C0;
}
