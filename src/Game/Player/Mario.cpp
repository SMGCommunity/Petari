#include "Game/Player/Mario.hpp"
#include "Game/LiveActor/Binder.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Player/MarioAbyssDamage.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioBlown.hpp"
#include "Game/Player/MarioBump.hpp"
#include "Game/Player/MarioBury.hpp"
#include "Game/Player/MarioClimb.hpp"
#include "Game/Player/MarioCrush.hpp"
#include "Game/Player/MarioDamage.hpp"
#include "Game/Player/MarioDarkDamage.hpp"
#include "Game/Player/MarioFaint.hpp"
#include "Game/Player/MarioFireDamage.hpp"
#include "Game/Player/MarioFireDance.hpp"
#include "Game/Player/MarioFireRun.hpp"
#include "Game/Player/MarioFlip.hpp"
#include "Game/Player/MarioFlow.hpp"
#include "Game/Player/MarioFoo.hpp"
#include "Game/Player/MarioFpView.hpp"
#include "Game/Player/MarioFreeze.hpp"
#include "Game/Player/MarioFrontStep.hpp"
#include "Game/Player/MarioHang.hpp"
#include "Game/Player/MarioMagic.hpp"
#include "Game/Player/MarioMapCode.hpp"
#include "Game/Player/MarioMove.hpp"
#include "Game/Player/MarioParalyze.hpp"
#include "Game/Player/MarioRabbit.hpp"
#include "Game/Player/MarioRecovery.hpp"
#include "Game/Player/MarioSideStep.hpp"
#include "Game/Player/MarioSkate.hpp"
#include "Game/Player/MarioSlider.hpp"
#include "Game/Player/MarioStep.hpp"
#include "Game/Player/MarioStick.hpp"
#include "Game/Player/MarioStun.hpp"
#include "Game/Player/MarioSukekiyo.hpp"
#include "Game/Player/MarioSwim.hpp"
#include "Game/Player/MarioTalk.hpp"
#include "Game/Player/MarioTeresa.hpp"
#include "Game/Player/MarioWait.hpp"
#include "Game/Player/MarioWall.hpp"
#include "Game/Player/MarioWarp.hpp"
#include "Game/Util/DirectDraw.hpp"
#include "revolution/mtx.h"

Mario::Mario(MarioActor *actor) : MarioModule(actor)
{
    initMember();
    mVelocity.zero();
    _13C.zero();
    _148.zero();
    _154.zero();
    mFrontVec.set(1.0f, 0.0f, 0.0f);
    _220 = mFrontVec;
    _43C = mFrontVec;
    _214.set(1.0f, 0.0f, 0.0f);
    _22C.set(1.0f, 0.0f, 0.0f);
    _3E4.set(1.0f, 0.0f, 0.0f);
    _2B4 = 0.0f;
    mSideVec.set(0.0f, 0.0f, 1.0f);
    _290 = mSideVec;
    mAirGravityVec.set(0.0f, -1.0f, 0.0f);
    _1E4.zero();
    bool nearZero = MR::isNearZero(mAirGravityVec, 0.0010000000475f);
    mHeadVec = -mAirGravityVec;
    _1FC = mHeadVec;
    _938 = mHeadVec;
    _944 = mFrontVec;
    _950 = mSideVec;
    _60D = 0;
    _71C = 0;
    _278 = 0.0f;
    _2D0 = 0.0f;
    mJumpVec.zero();
    _2E0.zero();
    _2EC.zero();
    _42C = 0;
    _2F8.zero();
    _328.zero();
    mLastNonFixMoveVec.zero();
    _488 = 0.0f;
    _430 = 0;
    _3BC = 0;
    _3BE = 0;
    _3C0 = 0;
    _3CE = 0;
    _3CC = 0;
    _3CA = 0;
    _3D0 = 0;
    _3D4 = 0;
    _424 = 0;
    _434 = 0;
    _436 = 0;
    _428 = 0;
    _3D2 = 0;
    _404 = 0;
    _406 = 0;
    _408 = 0;
    _40A = 0;
    _426 = 0;
    _418 = 0;
    _41A = 0;
    _774 = 0;
    _3C2 = 0;
    _3C4 = 0;
    _3C6 = 0;
    _3C8 = 0;
    _7D0 = 0x78;
    _524 = 0.0f;
    _528 = 0.0f;
    _52C = 0.0f;
    _530 = 0.0f;
    _534 = 0;
    _538 = 0.0f;
    _53C = 0.0f;
    _544 = 0;
    mYAngleOffset = 0.0f;
    _3F8 = 0;
    _3FA = 0;
    _3FC = 0;
    _3FE = 0;
    _400 = 0;
    _402 = 0;
    _27C = 0.0f;
    _734 = 0;
    _735 = 0;
    _748 = 0;
    _71E = 0;
    _414 = 0;
    mMorphResetTimer = 0;
    _41C = 0;
    _41E = 0;
    _410 = 0;
    _420 = 0;
    _43A = 0;
    _3F4 = 0.0f;
    _3F0 = 1.0f;
    _58.zero();
    _12C = 0;
    _76C = 0;
    _770 = 0.0f;
    _738 = 0.0f;
    _73C = 0.0f;
    _740 = 1.0f;
    _744 = 0.0f;
    _368.set(0.0f, 1.0f, 0.0f);
    _374 = -_368;
    _380 = _368;
    _38C = -_368;
    _398.set(0.0f, 1.0f, 0.0f);
    _344.set(1.0f, 0.0f, 0.0f);
    _74C = 0.0f;
    _750 = 0;
    _754 = 0;
    _40C = 0;
    _40E = 0;
    _412 = 0;
    _558 = 0;
    _2A8 = -_368;
    _29C = _368;
    _548 = _128 = _124 = 0.0f;
    _54C = mHeadVec;
    PSMTXIdentity(_C4);
    PSMTXIdentity(_64);
    PSMTXIdentity(_94);
    clearSlope();
    _910.zero();
    _91C.zero();
    _928.zero();
    _934 = 0.8f;
    _3C = -0.515f;
    mMovementStates_LOW_WORD = 0;
    mMovementStates_HIGH_WORD = 0;
    _10_LOW_WORD = 0;
    _10_HIGH_WORD = 0;
    mDrawStates_WORD = 0;
    _1C_WORD = 0;
    _20 = 0;
    _24 = 0;
    _28 = 0;

    _458 = TriangleFilterDelegator<Mario>::allocateDelegator(this, &Mario::isIgnoreTriangle);

    _45C = new Triangle();
    _460 = new Triangle();
    mGroundPolygon = new Triangle();
    _468 = new Triangle();
    _46C = new Triangle();

    _4C8 = new Triangle();
    mFrontWallTriangle = new Triangle();
    mBackWallTriangle = new Triangle();
    mSideWallTriangle = new Triangle();
    _4D8 = new Triangle();
    _4DC = new Triangle();

    _470 = new Triangle();
    _474 = new Triangle();
    mTmpPolygon = new Triangle();

    _7E0 = new Triangle();

    _820 = new Triangle();

    _47C = new Triangle();
    _480 = new Triangle();
    _484 = new Triangle();

    for (unsigned int i = 0; i < 0x20; i++) {
        _57C[i] = new Triangle();
    }

    _578 = 0;

    _4E8.zero();
    _4F4.zero();

    _7D4.zero();
    _8A4.zero();

    _8C8 = new Triangle();

    _8E8 = new Triangle();

    _8CC = new Triangle();
    _8D0 = new Triangle();

    _564 = -1;
    _568 = 0;
    _56C = 0;
    _570 = 0;

    for (unsigned int i = 0; i < 9; i++) {
        _A8C[i].zero();
    }

    _4A4.zero();
    _4B0.zero();

    _25C.zero();
    _268.zero();

    _350.zero();
    _35C.zero();

    _184.zero();

    _1A8.zero();

    _190.zero();
    _19C.zero();

    _4E4 = 0.0f;
    _4E0 = 0.0f;

    _72C = 200.0f;

    _730 = 0;

    _8D8 = 0;
    _8D4 = 0;

    _A38 = 0;
    _A68 = 0;

    _5FC = 0;

    mWorldPadDir.zero();

    _38 = 0.0f;
    mStickPos.z = 0.0f;
    mStickPos.y = 0.0f;
    mStickPos.x = 0.0f;

    _3D8.zero();

    _244.zero();
    _238.zero();
    _250.zero();

    clear2DStick();

    _6AC = 0;
    _6AD = 0;
    _6B0.zero();
    _6BC.zero();

    _63C.set(1.0f, 0.0f, 0.0f);
    _648.set(0.0f, 1.0f, 0.0f);
    _654.set(0.0f, 0.0f, 1.0f);
    _6A0 = _654;

    initSound();

    _97C = 0;

    mFlow = new MarioFlow(mActor);
    mWall = new MarioWall(mActor);
    mDamage = new MarioDamage(mActor);
    mFaint = new MarioFaint(mActor);
    mBlown = new MarioBlown(mActor);
    mHang = new MarioHang(mActor);
    mSwim = new MarioSwim(mActor);
    mSlider = new MarioSlider(mActor);
    mFireDamage = new MarioFireDamage(mActor);
    mFireRun = new MarioFireRun(mActor);
    mFireDance = new MarioFireDance(mActor);
    mAbyssDamage = new MarioAbyssDamage(mActor);
    mDarkDamage = new MarioDarkDamage(mActor);
    mStep = new MarioStep(mActor);
    mBump = new MarioBump(mActor);
    mParalyze = new MarioParalyze(mActor);
    mStun = new MarioStun(mActor);
    mCrush = new MarioCrush(mActor);
    mFreeze = new MarioFreeze(mActor);
    mMagic = new MarioMagic(mActor);
    mFpView = new MarioFpView(mActor);
    mRecovery = new MarioRecovery(mActor);
    mFlip = new MarioFlip(mActor);
    mSideStep = new MarioSideStep(mActor);
    mFrontStep = new MarioFrontStep(mActor);
    mStick = new MarioStick(mActor);
    mRabbit = new MarioRabbit(mActor);
    mSukekiyo = new MarioSukekiyo(mActor);
    mBury = new MarioBury(mActor);
    mWait = new MarioWait(mActor);
    mClimb = new MarioClimb(mActor);
    mSkate = new MarioSkate(mActor);
    mFoo = new MarioFoo(mActor);
    mWarp = new MarioWarp(mActor);
    mTeresa = new MarioTeresa(mActor);
    mTalk = new MarioTalk(mActor);
    mMove = new MarioMove(mActor);

    initTask();
    _95C = new FloorCode();
    _960 = 0;
    _962 = 0;
    for (int i = 0; i < 3; i++) {
        _964[i] = _960;
    }
    _96A = 0;
    _574 = 0;
    _898 = 0;
}

void Mario::updateAndClearStrideParameter() {
    _43C = mFrontVec;
    _16C = mVelocity;
    mPosition = mActor->mPosition;
    mVelocityAfter.zero();
    mVelocity.zero();
    _20 = mMovementStates_LOW_WORD;
    _24 = mMovementStates_HIGH_WORD;
    _28 = _10_LOW_WORD;
    _2C = _10_HIGH_WORD;
    mPrevDrawStates = mDrawStates;
    _34 = _1C_WORD;
    mDrawStates_WORD = 0;
    _1C_WORD = 0;
    mMovementStates._28 = 0;
    if (mMovementStates._1) {
        _10._1B = 0;
        mWall->_1C = 0;
    }
    if (_10._25) {
        _1C._1 = 1;
    }
    if (_10._26) {
        _1C._2 = 1;
    }
    if (_10._24) {
        _1C._0 = 1;
    }
    _10._25 = 0;
    _10._26 = 0;
}

void Mario::checkKeyLock() {
    if (!mMovementStates._0) {
        mMovementStates._22 = 0;
    }
    if (mActor->_37C < 0x4b) {
        mMovementStates._22 = 1;
    }
    if (mActor->isEnableNerveChange() == false) {
        mMovementStates._22 = 1;
    }
    if (isSwimming()) {
        mMovementStates._22 = 0;
    }
    if (MR::testSubPadButtonC(0)) {
        mDrawStates._7 = 1;
    }
}

// Regswap
void Mario::updateTimers() {
    if (mMovementStates._1) {
        _3CE++;
    }
    if (_414 != 0) {
        _414--;
    }
    if (_434 != 0) {
        _434--;
    }
    MarioHang* hang = mHang;
    u16 hangTimer = hang->mHangTimer;
    if (hangTimer == 0) {
        return;
    }
    else {
        hang->mHangTimer = hangTimer + -1;
    }
    
}

/*
void Mario::setFrontVecKeepUp(const TVec3f &v)
{
    TVec3f stack_38(mFrontVec);
    if (!MR::isNearZero(v, 0.001f)) {
        setFrontVec(v);
    }
    TVec3f stack_2C;
    PSVECCrossProduct(&mHeadVec, &mFrontVec, &stack_2C);

    if (MR::isNearZero(stack_2C, 0.001f)) {
        TVec3f stack_20;
        MR::vecBlendSphere(stack_38, v, &stack_20, 0.5f);
        PSVECCrossProduct(&mHeadVec, &stack_20, &stack_2C);
        setFrontVec(stack_20);
        if (MR::isNearZero(stack_2C, 0.001f)) {
            setFrontVecKeepSide(v);
            return;
        }
    }
    MR::normalize(&stack_2C);
    mSideVec = stack_2C;
    MR::normalize(&mSideVec);
    TVec3f stack_14;
    PSVECCrossProduct(&mSideVec, &mHeadVec, &stack_14);
    setFrontVec(stack_14);
    _22C = mFrontVec;
    _328 = mFrontVec % PSVECMag(_328);
    _344 = mSideVec;
}
*/

void Mario::updateMorphResetTimer() {
    u16 timer = mMorphResetTimer;
    if (timer == 0) {
        return;
    }
    timer--;
    mMorphResetTimer = timer;
    if (timer != 0) {
        return;
    }
    mActor->setPlayerMode(0, true);
    return;
}

/* void Mario::doExtraServices() {
    TVec3f stack_2c;
    TVec3f stack_20;
    MarioConst* pConst = mActor->mConst;
    MarioConstTable* pTable = pConst->mTable[pConst->mCurrentTable];
    if (MR::diffAngleAbs(_43C, mFrontVec) < pTable->mFrontAngleFixMargin) {
        setFrontVec(_43C);
    }
    if (!isPlayerModeBee() && !isPlayerModeTeresa() && mMovementStates._0 && _3BC > 600 && !MR::isNearZero(getGravityVec(), 0.001f)) {
        float f1 = MR::vecKillElement(_2D4, getGravityVec(), &stack_2c);
        TVec3f stack_14(getGravityVec());
        stack_14.scale(f1);
        _2D4 = stack_14;
    }
    if (!isPlayerModeBee() && !isPlayerModeTeresa() && mMovementStates._0 && _3BC > 300) {
        TVec3f* pTrans = getLastSafetyTrans(nullptr);
        TVec3f stack_8(_130);
        stack_8 -= *pTrans;
        MR::isNearZero(mAirGravityVec, 0.001f);
        if (MR::vecKillElement(stack_8, mAirGravityVec, &stack_20) > 28000.0f) {
            if (_488 > 2499.0f || mActor->isInZeroGravitySpot()) {
                mActor->forceKill(0);
            }
        }
    }
    if (getCurrentStatus() != 0 || _3C0 != 0 || mActor->_EA4) {
        _10._C = 0;
    }
} */

bool Mario::isEnableCheckGround() {
    return !isStatusActive(6);
}

void Mario::setGroundNorm(const TVec3f &rVec) {
    MR::isNearZero(mAirGravityVec, 0.001f);
    TVec3f negRVec = -rVec;
    if (MR::diffAngleAbs(negRVec, mAirGravityVec) > 1.4959966f) {
    } 
    else {
        _368 = rVec;
        _374 = -rVec;
    }
}

// Nearly matches
void Mario::createMtxDir(MtxPtr mtx, const TVec3f &rFront, const TVec3f &rUp, const TVec3f &rSide) {
    TVec3f side;
    TVec3f front;
    TVec3f up;
    side = rSide;
    front = rFront;
    up = rUp;
    if (!isPlayerModeTeresa()) {
        if (mMovementStates._37 || _10._15) {
            f32 sideAngleDiff = MR::diffAngleAbs(side, _950);
            f32 frontAngleDiff = MR::diffAngleAbs(front, _944);
            f32 upAngleDiff = MR::diffAngleAbs(up, _938);
            if (sideAngleDiff < mActor->mConst->mTable[mActor->mConst->mCurrentTable]->mVibrationAbsorbAngleS) {
                side = _950;
            }
            else {
                _950 = side;
            }
            if (frontAngleDiff < mActor->mConst->mTable[mActor->mConst->mCurrentTable]->mVibrationAbsorbAngleF) {
                front = _944;
            }
            else {
                _944 = front;
            }
            if (upAngleDiff < mActor->mConst->mTable[mActor->mConst->mCurrentTable]->mVibrationAbsorbAngleH) {
                up = _938;
            }
            else {
                _938 = up;
            }
        }
    }
    mtx[0][0] = side.x;
    mtx[1][0] = side.y;
    mtx[2][0] = side.z;
    mtx[0][1] = up.x;
    mtx[1][1] = up.y;
    mtx[2][1] = up.z;
    mtx[0][2] = front.x;
    mtx[1][2] = front.y;
    mtx[2][2] = front.z;
}

bool Mario::isNonFixHeadVec() const {
    if (isStatusActive(6)) {
        return true;
    }
    if (isStatusActive(1)) {
        return true;
    }
    if (isStatusActive(0x15)) {
        return true;
    }
    if (isStatusActive(0x18)) {
        return true;
    }
    if (isStatusActive(0x16)) {
        return true;
    }
    if (mMovementStates._3E) {
        return true;
    }
    if (mActor->_EA5) {
        return true;
    }
    if (isStatusActive(0x22)) {
        return false;
    }
    return mActor->_EA4;
    
}

/* void Mario::createDirectionMtx(MtxPtr mtx) {
    TVec3f stack_20;
    TVec3f stack_14;
    TVec3f stack_8;
    stack_14 = _1FC;
    stack_20 = mSideVec;
    stack_8 = mFrontVec;
    if (mDrawStates._11) {
        MR::normalize(&stack_14);
        PSVECCrossProduct(&stack_20, &stack_14, &stack_8);
        MR::normalizeOrZero(&stack_8);
        if (MR::isNearZero(stack_8, 0.001f)) {
            stack_8 = mFrontVec;
        }
        PSVECCrossProduct(&stack_14, &stack_8, &stack_20);
        MR::normalizeOrZero(&stack_20);
        if (MR::isNearZero(stack_20, 0.001f)) {
            stack_20 = mSideVec;
        }
    }
    else {
        MR::normalizeOrZero(&stack_14);
        PSVECCrossProduct(&stack_14, &mFrontVec, &stack_20);
        MR::normalizeOrZero(&stack_20);
        if (MR::isNearZero(stack_20, 0.001f)) {
            stack_20 = mSideVec;
        }
        PSVECCrossProduct(&stack_20, &stack_14, &stack_8);
        MR::normalizeOrZero(&stack_8);
        if (MR::isNearZero(stack_8, 0.001f)) {
            stack_8 = mFrontVec;
        }
    }
    createMtxDir(mtx, stack_8, stack_14, stack_20);
    MR::setMtxTrans(mtx, 0.0f, 0.0f, 0.0f);
} */

void Mario::slopeTiltHead(TVec3f *pVec) {
    if (mMovementStates._1) {
        if (_8F0 > 0.0f && !mMovementStates._23) {
            float blendFactor = _8F0 / 5.0f;
            if (blendFactor > 1.0f) {
                blendFactor = 1.0f;
            }
            MR::vecBlend(_368, *pVec, pVec, blendFactor);
            MR::normalize(pVec);
        }
        else if (mDrawStates._4) {
            MR::vecBlend(_368, *pVec, pVec, 0.2);
            MR::normalize(pVec);
        }
    }
}

void Mario::fixFrontVecByGravity() {
    if (isNonFixHeadVec() == false) {
        MR::isNearZero(mAirGravityVec, 0.001f);
        TVec3f up = -mAirGravityVec;
        MR::normalize(&up);
        if (_10._13) {
            TVec3f side(mPosition);
            side -= _6F4;
            MR::vecKillElement(side, _700, &side);
            MR::normalizeOrZero(&side);
            if (_344.dot(side) >= 0.0f) {
                _344 = side;
            }
            else {
                _344 = -side;
            }
            if (mSideVec.dot(_344) >= 0.0f) {
                mSideVec = _344;
                MR::normalize(&mSideVec);
            }
            else {
               // TVec3f stack_14 = -_344;
                mSideVec = -_344;
                MR::normalize(&mSideVec);
            }
        }
        else if (_1C._2) {
            TVec3f side2;
            PSVECCrossProduct(&up, &mFrontVec, &side2);
            MR::normalizeOrZero(&side2);
            if (MR::isNearZero(side2, 0.001f) == false) {
                _344 = side2;
            }
        }
        TVec3f front;
        PSVECCrossProduct(&_344, &up, &front);
        if (MR::normalizeOrZero(&front) == nullptr) {
            setFrontVec(front);
            _22C = mFrontVec;
            f32 _328mag = PSVECMag(&_328);
            _328 = mFrontVec.scaleInline(_328mag);
        }
    }
}

void Mario::fixFrontVecFromUpSide() {
    TVec3f frontVec;
    PSVECCrossProduct((Vec*)&mSideVec, (Vec*)&mHeadVec, (Vec*)&frontVec);
    MR::normalizeOrZero(&frontVec);
    if (MR::isNearZero(frontVec, 0.001f) == false) {
        setFrontVec(frontVec);
        _22C = mFrontVec;
        float scaleFactor = PSVECMag((Vec*)&_328);
        TVec3f scaledFront(mFrontVec); // inlined when it shouldn't be
        scaledFront.scale(scaleFactor);
        _328 = scaledFront;
    }
}

void Mario::fixSideVecFromFrontUp() {
    TVec3f side;
    PSVECCrossProduct((Vec*)&mHeadVec, (Vec*)&mFrontVec, (Vec*)&side);
    MR::normalizeOrZero(&side);
    if (MR::isNearZero(side, 0.001f) == false) {
        mSideVec = side;
    }
}

void Mario::setSideVec(const TVec3f &rSide) {
    mSideVec = rSide;
    MR::normalize(&mSideVec);
}

void Mario::setHeadVec(const TVec3f &rHead) {
    mHeadVec = rHead;
    MR::normalize(&mHeadVec);
}

void Mario::setFrontVec(const TVec3f &rFront) {
    TVec3f plusMinus6A0;
    TVec3f frontWithout6A0;
    if (mMovementStates._37 && !isStatusActive(0x22)) {
        if (getCamDirZ().dot(_6A0) > 0.0f) {
            plusMinus6A0 = _6A0;
        }
        else {
            TVec3f neg6A0 = -_6A0;
            plusMinus6A0 = neg6A0;
        }
        if (plusMinus6A0.dot(rFront) > 0.0f) {
            // maybe this reflects rFront around _6A0?? (very unsure)
            float f1 = MR::vecKillElement(rFront, plusMinus6A0, &frontWithout6A0);
            TVec3f scaled6A0(plusMinus6A0);
            scaled6A0.scale(f1);
            frontWithout6A0 -= scaled6A0;
            MR::normalizeOrZero(&frontWithout6A0);
            mFrontVec = frontWithout6A0;
        }
        else {
            mFrontVec = rFront;
        }
    }
    else {
        mFrontVec = rFront;
    }
    MR::normalize(&mFrontVec);
}

void Mario::setFrontVecKeepUp(const TVec3f &rFront, f32 blendFactor) {
    TVec3f blendedFront;
    if (MR::isNearZero(rFront, 0.001f) == false) {
        TVec3f normalizedFront(rFront);
        MR::normalize(&normalizedFront);
        s32 blend = 0;
        f32 angleDiff = MR::diffAngleAbs(mFrontVec, normalizedFront);
        if (angleDiff < HALF_PI) {
            blend = MR::vecBlendSphere(mFrontVec, normalizedFront, &blendedFront, blendFactor);
        }
        if (blend == 0) {
            MR::vecRotAxis(mFrontVec, normalizedFront, mHeadVec, &blendedFront, angleDiff * blendFactor);
        }
        setFrontVecKeepUp(blendedFront);
    }
}

void Mario::setFrontVecKeepUpAngle(const TVec3f &frontVec, f32 angle) {
    if (MR::isNearZero(frontVec, 0.001f) == false) {
        f32 angleDiff = MR::diffAngleAbs(mFrontVec, frontVec);
        if (angleDiff != 0.0f) {
            if (angleDiff < angle) {
                setFrontVecKeepUp(frontVec);
            }
            else {
                setFrontVecKeepUp(frontVec, angle/angleDiff);
            }
        }
    }
}

void Mario::setFrontVecKeepUp(const TVec3f &rFront, u32 _750_set) {
    Mtx rotMtx;
    TVec3f rotFrontVec;
    TVec3f rotVec2;
    PSMTXRotAxisRad(rotMtx, &mHeadVec, _74C);
    PSMTXMultVec(rotMtx, &mFrontVec, &rotFrontVec);
    float angleDiff = MR::diffAngleAbs(rFront, rotFrontVec);
    if (angleDiff > 0.024543693f) {
        PSMTXRotAxisRad(rotMtx, &mHeadVec, PI_180);
        PSMTXMultVec(rotMtx, &rotFrontVec, &rotVec2);
        float angleDiff2 = MR::diffAngleAbs(rFront, rotVec2);
        if (angleDiff2 < angleDiff) {
            _74C = -angleDiff;
        }
        else {
            _74C = angleDiff;
        }
    }
    setFrontVecKeepUp(rFront);
    if (_750_set != 0) {
        _750 = _750_set;
    }
}

void Mario::setFrontVecKeepUp(const TVec3f &rFront) {
    TVec3f front = mFrontVec;
    TVec3f side;
    TVec3f blendedFront;
    TVec3f front2;
    if (MR::isNearZero(rFront, 0.001f) == false) {
        setFrontVec(rFront);
    }
    PSVECCrossProduct(&mHeadVec, &mFrontVec, &side);
    if (MR::isNearZero(side, 0.001f)) {
        MR::vecBlendSphere(front, rFront, &blendedFront, 0.5f);
        PSVECCrossProduct(&mHeadVec, &blendedFront, &side);
        setFrontVec(blendedFront);
        if (MR::isNearZero(side, 0.001f)) {
            setFrontVecKeepSide(rFront);
            return;
        }
    }
    MR::normalize(&side);
    mSideVec = side;
    MR::normalize(&mSideVec);
    PSVECCrossProduct(&mSideVec, &mHeadVec, &front2);
    setFrontVec(front2);
    _22C = mFrontVec;
    f32 _328mag = PSVECMag(&_328);
    TVec3f scaledFront(mFrontVec);
    scaledFront.scale(_328mag);
    _328 = scaledFront;
    _344 = mSideVec;
}

void Mario::setFrontVecKeepSide(const TVec3f &rFront) {
    TVec3f headVec;
    TVec3f front;
    if (MR::isNearZero(rFront, 0.001f) == false) {
        setFrontVec(rFront);
    }
    PSVECCrossProduct(&mFrontVec, &mSideVec, &headVec);
    if (MR::normalizeOrZero(&headVec) != nullptr) {
        const TVec3f* gravity = getGravityVec();
        TVec3f up = -(*gravity);
        mHeadVec = up;
        MR::normalize(&mHeadVec);
    }
    else {
        mHeadVec = headVec;
        MR::normalize(&mHeadVec);
    }
    PSVECCrossProduct(&mSideVec, &mHeadVec, &front);
    setFrontVec(front);
    _22C = mFrontVec;
    float _328mag = PSVECMag(&_328);
    TVec3f scaledFront(mFrontVec);
    scaledFront.scale(_328mag);
    _328 = scaledFront;
}

void Mario::setHeadAndFrontVecFromRotate(const TVec3f &rRotate) {
    TVec3f frontVec = TVec3f(0.0f, 0.0f, 1.0f);
    TVec3f headVec = TVec3f(0.0f, 1.0f, 0.0f);
    Mtx mtx;
    MR::makeMtxTR(mtx, 0.0f, 0.0f, 0.0f, rRotate.x, rRotate.y, rRotate.z);
    PSMTXMultVec(mtx, (Vec*)&frontVec, (Vec*)&frontVec);
    PSMTXMultVecSR(mtx, (Vec*)&headVec, (Vec*)&headVec);
    mHeadVec = headVec;
    MR::normalize(&mHeadVec);
    setFrontVecKeepUp(frontVec);
}

void Mario::forceSetHeadVecKeepSide(const TVec3f &rVec) {
    mHeadVec = rVec;
    _1FC = rVec;
    if (MR::isSameDirection(rVec, mSideVec, 0.01f)) {
        fixSideVecFromFrontUp();
    }
    fixFrontVecFromUpSide();
    _43C = mFrontVec;
    _750 = 0;
}

// float regswap + bad stack ordering
void Mario::draw() const {
    if (mDrawStates._1) {
        TDDraw::cameraInit3D();
        TDDraw::setup(0,1,0);
        TVec3f* p_2A0 = &mActor->_2A0;
        TVec3f diff31C_2A0(mShadowPos);
        diff31C_2A0 -= *p_2A0;
        TDDraw::drawCylinder(mPosition, diff31C_2A0, 32.0f, 0xffff0010, 0xff000004, 8);
        TVec3f diff2A0_31C(mActor->_2A0);
        diff2A0_31C -= mShadowPos;
        f32 diffMag = PSVECMag(&diff2A0_31C);
        MR::normalize(&diff2A0_31C);
        TVec3f _31CPlusStuff(mShadowPos);
        f32 _37CMod10 = mActor->_37C % 0xa;
        _31CPlusStuff += diff2A0_31C.scaleInline(diffMag).scaleInline(0.125f).scaleInline(_37CMod10).scaleInline(0.1f);
        f32 one_eighth = 0.125f;
        for (u32 i = 0; i < 8; i++) {
            TDDraw::drawCircle(_31CPlusStuff, diff2A0_31C, 100.0f, 0xffffff70, 0x10);
            _31CPlusStuff += diff2A0_31C.scaleInline(one_eighth).scaleInline(diffMag);
        }
    }
    if (_97C != nullptr) {
        _97C->draw3D();
    }
    drawTask();
}
/*
    r3 = mActor
    r0 = 0x4330
    r5 = 0x43300000
    r6 = 0xa
    r7 = mActor->_37C;
    r3 = stack_20
    stack_78 = 0x4330
    r4 = stack_68
    r0 = mActor->_37C / 0xa
    f1 = 0x43300000
    r0 = mActor->_37C / 0xa * 0xa
    r0 = mActor->_37C & 0xa

*/

void Mario::addTrans(const TVec3f &rShift, const char *a2) {
    mPosition += rShift;
    TVec3f _148shift(_148);
    _148shift -= rShift;
    f32 _148shiftTemp = __fabsf(_148shift.x);
    f32 _148Temp = __fabsf(_148.x);
    if (_148Temp <= _148shiftTemp) {
        _148.x = 0.0f;
    }
    _148shiftTemp = __fabsf(_148shift.y);
    _148Temp = __fabsf(_148.y);
    if (_148Temp <= _148shiftTemp) {
        _148.y= 0.0f;
    }
    _148shiftTemp = __fabsf(_148shift.z);
    _148Temp = __fabsf(_148.z);
    if (_148Temp <= _148shiftTemp) {
        _148.z = 0.0f;
    }
}

void Mario::setTrans(const TVec3f &rShift, const char *a2) {
    TVec3f reqShift(rShift);
    reqShift -= mPosition;
    mPosition = rShift;
    TVec3f _148shift(_148);
    _148shift -= reqShift;
    f32 _148shiftTemp = __fabsf(_148shift.x);
    f32 _148Temp = __fabsf(_148.x);
    if (_148Temp <= _148shiftTemp) {
        _148.x = 0.0f;
    }
    _148shiftTemp = __fabsf(_148shift.y);
    _148Temp = __fabsf(_148.y);
    if (_148Temp <= _148shiftTemp) {
        _148.y= 0.0f;
    }
    _148shiftTemp = __fabsf(_148shift.z);
    _148Temp = __fabsf(_148.z);
    if (_148Temp <= _148shiftTemp) {
        _148.z = 0.0f;
    }
}

bool Mario::isEnableRush() const {
    if (isHanging()) {
        return false;
    }
    if (isDamaging()) {
        return false;
    }
    if (isStatusActive(0x19)) {
        return false;
    }
    if (mMovementStates._B && mMovementStates._0) {
        return false;
    }
    return true;
}

bool Mario::isInvincible() const {
    if (isPlayerModeInvincible()) {
        return true;
    }
    if (mMovementStates._1F) {
        return true;
    }
    else {
        return mActor->_3D8 != false;
    }
}

bool Mario::isCeiling() const {
    if (_4C8->isValid() == false) {
        return false;
    }
    else {
        return !MR::isNearZero(_3A4, 0.001f);
    }
}

void Mario::setGravityVec(const TVec3f &rGravity) {
    if (MR::isSameDirection(mAirGravityVec, rGravity, 0.01)) {
        if (MR::isOppositeDirection(mAirGravityVec, rGravity, 0.01)) {
            _10._24 = 1;
            _10._25 = 1;
        }
        _1E4.zero();
    }
    else {
        PSVECCrossProduct(&mAirGravityVec, &rGravity, &_1E4);
        MR::normalizeOrZero(&_1E4);
        f32 frontDot = __fabsf(mFrontVec.dot(_1E4));
        f32 sideDot = __fabsf(mSideVec.dot(_1E4));
        f32 headDot = __fabsf(mHeadVec.dot(_1E4));
        if (frontDot > sideDot && frontDot > headDot) {
            _10._A = 0;
            _10._26 = 1;
        }
        _10._24 = 1;
    }
    TVec3f stack_8 = -_368;
    _374 = stack_8;
    mAirGravityVec = rGravity;
}

void Mario::updateSoundCode() {
    s32 soundCode = -1;
    Mario* pPlayer = getPlayer();
    if (pPlayer->mMovementStates._1) {
        if (mGroundPolygon->isValid()) {
            soundCode = MR::getSoundCodeIndex(mGroundPolygon->getAttributes());
        }
    }
    else if (_45C->isValid()) {
        soundCode = MR::getSoundCodeIndex(_45C->getAttributes());
    }
    if (isStatusActive(1)) {
        if (mMovementStates._8) {
            soundCode = MR::getSoundCodeIndex(mFrontWallTriangle->getAttributes());
        }
        else if (mMovementStates._19) {
            soundCode = MR::getSoundCodeIndex(mBackWallTriangle->getAttributes());
        }
        else if (mMovementStates._1A) {
            soundCode = MR::getSoundCodeIndex(mSideWallTriangle->getAttributes());
        }
    }
    MR::setMapSondCodeGravity(mActor, soundCode);
}

// nearly, last conditional doesn't want to cooperate
bool Mario::isForceStopRush() const {
    if (_488 != 0.0f) {
        goto exit_false;
    }
    u16 temp = _960;
    if (temp < (s16)0xf) {
        if (temp == (s16)4) {
            goto exit_true;
        }
        if (temp < (s16)4) {
            if (temp == (s16)1) {
                goto exit_true;
            }
            goto exit_false;
        }
        if (temp == (s16)0xa) {
            goto exit_true;
        }
        goto exit_false;
    }
    if (temp == (s16)0x18) {
        goto exit_true;
    }
    if (temp < (s16)0x18) {
        if (temp < (s16)0x11 == false) {
            goto exit_false;
        }
        goto exit_true;
    }
    if (temp == (s16)0x81 != false) {
        goto exit_true;
    }
    goto exit_false;
    

    exit_true:
        return true;
    exit_false:
        return false;
}

const TVec3f &Mario::getShadowNorm() const {
    return *_45C->getNormal(0);
}

const TVec3f &Mario::getAirGravityVec() const {
    MR::isNearZero(this->mAirGravityVec, 0.001f);
    return mAirGravityVec;
}

/* const TVec3f &Mario::getAirFrontVec() const {
    TVec3f stack_8;
    MR::vecKillElement(mFrontVec, mAirGravityVec, &stack_8);
    TVec3f* stack_8_pointer = &stack_8;
    MR::normalizeOrZero(stack_8_pointer);
    if (stack_8_pointer) {
        return mFrontVec;
    }
    else {
        return *stack_8_pointer;
    }
} */

void Mario::initAfterConst() {
    MarioConst* pConst = mActor->mConst;
    _402 = pConst->mTable[pConst->mCurrentTable]->mAirWalkTime;
    mPosition = mActor->mPosition;
    mShadowPos = mPosition;
    mGroundPos = mPosition;
    mMove->initAfter();
    mFoo->init();
    mSwim->init();
    _7D4 = mPosition;
    _814 = mPosition;
    PSMTXIdentity(_7E4);
    PSMTXIdentity(_824);
    _720 = getAnimationStringPointer("ヒップドロップ開始");
    _724 = getAnimationStringPointer("ヒップドロップ");
    _728 = getAnimationStringPointer("ヒップドロップ着地");
}

/* void Mario::writeBackPhysicalVector() {
    TVec3f stack_ec;
    TVec3f stack_e0;
    TVec3f stack_d4;
    TVec3f stack_c8;
    TVec3f stack_bc;
    TVec3f stack_b0;
    TVec3f stack_98;
    TVec3f stack_8c;
    TVec3f stack_14;
    TVec3f stack_8;
    if (_1C._10) {
        TVec3f stack_104(_130);
        addTrans(_160, nullptr);
        if (calcDistToCeil(false) < 200.0f) {
            _160.zero();
            stopWalk();
        }
        _130 = stack_104;
    }
    if (mMovementStates._37 || _10._15) {
        TVec3f stack_f8(_130);
        stack_f8 -= _688;
        MR::vecKillElement(stack_f8, _6A0, &stack_f8);
        TVec3f stack_80(_688);
        stack_80 += stack_f8;
        _130 = stack_80;
        MR::vecKillElement(_160, _6A0, &_160);
        TVec3f stack_74(_694);
        stack_74 -= _688;
        float f29 = MR::vecKillElement(stack_74, _6A0, &stack_ec);
        if (f29 > 1.0f) {
            f29 = 1.0f;
        }
        else if (f29 < -1.0f) {
            f29 = 1.0f;
        }
        stack_ec = _6A0;
        stack_ec.setLength(f29);
        _688 += stack_ec;
    }
    fixPositionInTower();
    if (mMovementStates._0) {
        bool rising = false;
        if (isRising()) {
            rising = true;
        }
        if (mActor->_334 != 0) {
            _578 = 0;
        }
        if (!mMovementStates._22) {
            float PI_30 = 0.10471976;
            float zero = 0.0f;
            float SQRT2_2 = 0.707f;
            for (u32 i = 0; i < _578; i++) {
                if (MR::vecKillElement(_160, *_57C[i]->getNormal(0), &stack_e0) < zero) {
                    _160 = stack_e0;
                    float f28 = MR::vecKillElement(_2D4, *_57C[i]->getNormal(0), &stack_d4);
                    if (f28 <= 0.0f) {
                        TVec3f stack_50(*_57C[i]->getNormal(0));
                        stack_50.scale(f28);
                        TVec3f stack_5c(stack_50);
                        stack_5c.scale(0.5f);
                        TVec3f stack_68(stack_d4);
                        stack_68 += stack_5c;
                        _2D4 = stack_68;
                    }
                    else if (MR::diffAngleAbs(*getGravityVec(), _2D4) < PI_30) {
                        MR::vecKillElement(*_57C[i]->getNormal(0), *getGravityVec(), &stack_c8);
                        float _2D4Mag = PSVECMag(&_2D4);
                        MR::normalizeOrZero(&stack_c8);
                        _2D4 += stack_c8;
                        _2D4.setLength(_2D4Mag);
                    }
                    if (rising) {
                        if (_57C[i]->getNormal(0)->dot(*getGravityVec()) > SQRT2_2) {
                            mActor->sendMsgUpperPunch(_57C[i]->mSensor);
                        }
                    }
                    if (isPlayerModeBee() && !mActor->mAlphaEnable && strcmp(MR::getWallCodeString(_57C[i]), "Fur")) {
                        Triangle stack_110 = Triangle();
                        MarioActor* pActor = mActor;
                        TVec3f stack_38 = -(*_57C[i]->getNormal(0));
                        TVec3f stack_44(stack_38);
                        stack_44.scale(300.0f);
                        if (MR::getFirstPolyOnLineToMap(&stack_bc, &stack_110, mActor->_2A0, stack_44) && strcmp(MR::getWallCodeString(_57C[i]), "Fur")) {
                            MarioActor* pActor2 = mActor;
                            pActor2->entryWallWalkMode(stack_bc, *stack_110.getNormal(0));
                        }
                    }
                }
            }
        }
    }
    else if (isSwimming() && !mMovementStates._22) {
        float SQRT2_2 = 0.707f;
        for (u32 i = 0; i < _578; i++) {
            if (MR::vecKillElement(_160, *_57C[i]->getNormal(0), &stack_b0) < 0.0f) {
                _160 = stack_b0;
                float f28 = MR::vecKillElement(mSwim->_A0, *_57C[i]->getNormal(0), &mSwim->_A0);
                TVec3f stack_20(*_57C[i]->getNormal(0));
                stack_20.scale(f28);
                TVec3f stack_2c(stack_20);
                stack_2c.scale(0.5f);
                mSwim->_A0 += stack_2c;
            }
        }
    }
    else if (!_1C._B && mActor->_288.dot(_368) < 0.707f && MR::isNearZero(mActor->_288, 0.001f)) {
        TVec3f stack_a4(mActor->_288);
        MR::normalizeOrZero(&stack_a4);
        float f2 = MR::vecKillElement(_160, stack_a4, &stack_98);
        if (f2 < 0.0f) {
            _160 = stack_98;
            MR::normalizeOrZero(&stack_a4);
            float f1 = 1.0f - MR::clamp((-stack_a4.dot(mFrontVec)-0.3f) * 1.4285715f, 0.0f, 1.0f);
            if (!mMovementStates._8 && f1 < 0.1f) {
                if (mMovementStates._1A) {
                    TVec3f stack_14(getSideWallNorm());
                    stack_14.scale(-f2);
                    _160 += stack_14;
                }
                else {
                    int s28 = mActor->mBinder->_28;
                    if (s28 != 0) {
                        float f28 = 0.0f;
                        int index = 0;
                        for (int i = 0; i < s28; i++) {
                            float dot = mActor->mBinder->getPlane(i)->getNormal(0)->dot(mFrontVec);
                            if (dot < 0.0f) {
                                index = i;
                                f28 = dot;
                            }
                        }

                    }
                }
            }
        }
    }
}

void Mario::actionMain() {
    if (!mMovementStates._16) {
        if (checkDamage() == false) {
            if (_97C) {
                sendStateMsg(2);
            }
            else if (mMovementStates._0) {
                procJump(false);
                checkTornado();
                checkHang();
                checkWallStick();
            }
            else {
                mainMove();
                updateWalkSpeed();
            }
            if (mMovementStates._1) {
                mMovementStates._30 = 1;
                mMovementStates._31 = 1;
                doFrontStep();
            }
        }
        if (!mMovementStates._23) {
            _1A8 = _160;
            _3C2 = 0;
            _280 = 0.0f;
            _284.zero();
        }
        else {
            _3C2++;
        }
        if (_3C4 != 0) {
            _3C4--;
        }
        _A8C[2] = _160;
        if (mMovementStates._1 && !mMovementStates._0) {
            if (isEnableSlopeMove()) {
                slopeMove();
            }
            else {
                clearSlope();
            }
        }
        if (isStickOn() && isAnimationRun("水泳陸うちあげ")) {
            stopAnimation(nullptr, (char*)nullptr);
        }
        if (!mDrawStates._4) {
            _3FE = 0;
        }
        callExtraTasks(0);
        beeMarioOnGround();
        checkSpecialWaitAnimation();
        if (!mDrawStates._16) {
            retainMoveDir(_1C0.x, _1C0.y, nullptr);
        }
    }
}

/* void Mario::updateGroundInfo() {
    checkMap();
    if (isEnableCheckGround()) {
        bool b1 = false;
        if (!mMovementStates._0 && !mMovementStates._1) {
            b1 = true;
        }
        checkGround();
        mMovementStates._1 = 0;
        if (mDrawStates._18) {
            _3C6++;
        }
        else {
            _3C6 = 0;
        }
        if (b1 && mMovementStates._1 && isDefaultAnimationRun("落下")) {
            changeAnimation(nullptr, "基本");
        }
        if (!isStatusActive(0x13)) {
            if (!isStatusActive(0x13) && !mMovementStates._1 && mMovementStates._3E == 1 && _2D4.dot(Mario::getGravityVec()) > 0.0f
                && _2D4.dot(*_45C->getNormal(0)) < 0.0f && _1FC.dot(Mario::getGravityVec()) > 0.0f && _488 < 170.0f) {
                mMovementStates._1 = 1;
                setGroundNorm(*_45C->getNormal(0));
                setTrans(mShadowPos, nullptr);
            }
        }
    }
    updateCameraPolygon();
    if (mMovementStates._1) {
        mMovementStates._29 = 0;
        if (_43A == 0 && isSlipPolygon(_464)) {
            mDrawStates._C = 1;
        }
    }
    if (_43A != 0) {
        _43A--;
    }
    updateFloorCode();
    updateSoundCode();
    updateOnSand();
    updateOnWater();
    updateOnPoison();
} */

// conditionals won't behave
const TVec3f* Mario::getGravityVec() const {
    if (isStatusActive(0x1e) || isStatusActive(0x1d)) {
        return &_790;
    }
    if (mMovementStates._1 && !isSlipFloorCode(_960))  {
        if (mActor->mAlphaEnable) {
            return &mAirGravityVec;
        }
        if (isAnimationRun("ハード着地")) {
            return &mAirGravityVec;
        }
        if (_3CE < 0x10 && _3BC > 3 && !mMovementStates._B) {
            return &mAirGravityVec;
        }
        if (MR::isNearZero(_374, 0.001f) == false && _1C._13 && _1C._14) {
            return &_374;
        }
    }
    if (isSwimming()) {
        if (mSwim->_19C >= -150.0f && mDrawStates._F && MR::isNearZero(_38C, 0.001f) == false) {
            return &_38C;
        } 
        return &mAirGravityVec;
    }
    MR::isNearZero(mAirGravityVec, 0.001f);
    bool b1 = true;
    if (_430 == 0xc) {
        b1 = false;
    }
    else if (_430 == 0xd) {
        b1 = false;
    }
    if (!isSlipFloorCode(_960) && b1 && !isPlayerModeHopper() && !isPlayerModeTeresa() && !isDamaging()
        && !isStatusActive(6) && !isStatusActive(4) && !isStatusActive(0x13) && _430 != 5 && mActor->_334 == 0
        && mMovementStates._0 && !mMovementStates._22 && _3BC < 8) {
            return &_374;
    }
    return &mAirGravityVec;
}


void Mario::touchWater() {
    if (mActor->isActionOk("水解除") && mMorphResetTimer == 0) {
        mMorphResetTimer = 10;
    }
}
