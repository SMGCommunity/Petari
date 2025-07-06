#include "Game/Player/MarioAccess.hpp"
#include "Game/Player/MarioHolder.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Player/MarioFpView.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioHang.hpp"
#include "Game/Player/MarioSwim.hpp"
#include "Game/Player/RushEndInfo.hpp"
#include "Game/Util/FixedPosition.hpp"
#include "Game/MapObj/StarPiece.hpp"
#include "Game/MapObj/StarPieceDirector.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/LiveActor/Binder.hpp"
#include "JSystem/JMath/JMath.hpp"

void MarioAccess::getTakePos(TVec3f *pDest) {
    if (getPlayerActor()->_494) {
        getPlayerActor()->_494->calc();
        TPos3f* pMtx = &getPlayerActor()->_494->_1C;
        f32 z = (*pMtx)[2][3];
        f32 y = (*pMtx)[1][3];
        f32 x = (*pMtx)[0][3];
        pDest->set<f32>(x,y,z);
    }
}

bool MarioAccess::isOnActor(LiveActor *pActor) {
    if (getPlayerActor()->mMario->_1C._13) {
        Triangle* pGroundPoly = getPlayerActor()->mMario->mGroundPolygon;
        if (pGroundPoly->isValid()) {
            return pGroundPoly->mSensor->mActor == pActor;
        }
        return false;
    }
    if (getPlayerActor()->mMario->_1C._14 ||
    (getPlayerActor()->mMario->isSwimming() &&
     getPlayerActor()->mMario->mMovementStates._2)) {
        Triangle* pPoly = getPlayerActor()->mMario->_45C;
        if (!pPoly->isValid()) {
            return false;
        }
        return pPoly->mSensor->mActor == pActor;
    }
    return false;
}

bool MarioAccess::isOnGround(u32 a1) {
    if (a1) {
        return false;
    }
    if (getPlayerActor()->_934) {
        return MR::isOnGround(getPlayerActor()->_924->mActor);
    }
    else {
        return getPlayerActor()->mMario->mMovementStates._1;
    }
}

bool MarioAccess::isHipDropFalling() {
    if (getPlayerActor()->mMario->mMovementStates._0
        && getPlayerActor()->mMario->mMovementStates._B
        && !getPlayerActor()->mMario->mMovementStates._1
        && !getPlayerActor()->isJumpRising()) {
            return true;
    }
    return false;
}

bool MarioAccess::isHipDropLand() {
    return getPlayerActor()->mMario->mDrawStates._14;
}

bool MarioAccess::isSwingAction() {
    if (getPlayerActor()->_934) {
        return false;
    }
    if (getPlayerActor()->mMario->mMovementStates._F) {
        return true;
    }
    return getPlayerActor()->isPunching();
}

bool MarioAccess::isInRush() {
    bool ret = true;
    if (!getPlayerActor()->_934 && getPlayerActor()->mMario->isStatusActive(0x13) == false) {
        ret = false;
    }
    return ret;
}

bool MarioAccess::isSquat() {
    bool ret = false;
    if (getPlayerActor()->mMario->mMovementStates._A && getPlayerActor()->mMario->mMovementStates._1) {
        ret = true;
    }
    return ret;
}

bool MarioAccess::isParalyzing() {
    return getPlayerActor()->mMario->isStatusActive(0xb);
}

bool MarioAccess::isTeresaDisappear() {
    if (getPlayerActor()->mPlayerMode != 6) {
        return false;
    }
    return getPlayerActor()->mMario->_418 != 0;
}

bool MarioAccess::isFlying() {
    if (getPlayerActor()->mMario->isStatusActive(0x18)) {
        return true;
    }
    return getPlayerActor()->mMario->_10._21;
}

bool MarioAccess::isNeedBrakingCamera() {
    return getPlayerActor()->mMario->_1C._9;
}

s32 MarioAccess::getCameraCubeCode() {
    return getPlayerActor()->mMario->getCameraCubeCode();
}

bool MarioAccess::isSwimming() {
    return getPlayerActor()->mMario->isSwimming();
}

bool MarioAccess::isSkating() {
    return getPlayerActor()->mMario->isStatusActive(0x1f);
}

Triangle* MarioAccess::getGroundingPolygon(u32 arg) {
    if (getPlayerActor()->mMario->isSwimming()) {
        if (getPlayerActor()->mMario->mMovementStates._2) {
            return getPlayerActor()->mMario->_45C;
        }
        return nullptr;
    }
    if (!isOnGround(0)) {
        return nullptr;
    }
    if (getPlayerActor()->_934) {
        return &getPlayerActor()->_924->mActor->mBinder->mGroundInfo.mParentTriangle;
    }
    return getPlayerActor()->mMario->mGroundPolygon;
}

Triangle* MarioAccess::getShadowingPolygon() {
    if (getPlayerActor()->mMario->mMovementStates._2) {
        return getPlayerActor()->mMario->_45C;
    }
    return nullptr;
}

f32 MarioAccess::getShadowHeight() {
    return getPlayerActor()->mMario->mShadowHeight;
}

void MarioAccess::forceKill(u32 arg, u32 unused) {
    getPlayerActor()->forceKill(arg);
}

bool MarioAccess::isOnPress() {
    return getPlayerActor()->_390 != 0;
}

bool MarioAccess::isDisableFpView() {
    return getPlayerActor()->mMario->isDisableFpViewMode();
}

bool MarioAccess::isFpViewChangingFailure() {
    return getPlayerActor()->mMario->mFpViewChangingFailure;
}

void MarioAccess::stopFpView() {
    if (getPlayerActor()->mMario->isStatusActive(0x12)) {
        getPlayerActor()->mMario->mFpView->forceClose();
        getPlayerActor()->mMario->closeStatus(nullptr);
    }
}

void MarioAccess::noticeDashChance() {
    Mario* pMario = getPlayerActor()->mMario;
    pMario->_436 = 5;
}

void MarioAccess::setWalkingReset(f32 set) {
    getPlayerActor()->mMario->_2D0 = set; 
}

void MarioAccess::forceFly(const TVec3f &a1, const TVec3f &a2, s32 a3) {
    getPlayerActor()->mMario->doPointWarp(a1, a2, a3);
}

void MarioAccess::setJumpVec(const TVec3f &rSrc) {
    getPlayerActor()->mMario->mJumpVec = rSrc;
}

void MarioAccess::forceJump(const TVec3f &rDir, u32 unused) {
    getPlayerActor()->mMario->mMovementStates_HIGH_WORD |= 0x40000000;
    getPlayerActor()->mMario->tryForceJumpDelay(rDir);
}

void MarioAccess::freeJump(const TVec3f &rDir, u32 unused) {
    getPlayerActor()->mMario->mMovementStates_HIGH_WORD |= 0x40000000;
    getPlayerActor()->mMario->tryFreeJumpDelay(rDir);
}

// Instruction ordering stupidity
void MarioAccess::tornadoJump() {
    MarioActor* pActor = getPlayerActor();
    getPlayerActor()->mMario->_544 = pActor->mConst->mTable[pActor->mConst->mCurrentTable]->mTornadoTimeAir;
    getPlayerActor()->mMario->tryTornadoJump();
    getPlayerActor()->mMario->startTornadoCentering(getPlayerActor()->_928);
}

// More instruction ordering stupidity
/*
void MarioAccess::tornadoJumpMini() {
    TVec3f stack_14;
    TVec3f stack_8;
    MarioActor* pActor = getPlayerActor();
    getPlayerActor()->mMario->_544 = pActor->mConst->mTable[pActor->mConst->mCurrentTable]->mTornadoTimeAir;
    getPlayerActor()->mMario->tryTornadoJump();
    JMathInlineVEC::PSVECNegate(getPlayerActor()->mMario->getGravityVec(), &stack_8);
    MarioActor* pActor2 = getPlayerActor();
    float f3 = pActor2->mConst->mTable[pActor2->mConst->mCurrentTable]->mTornadoBoostPower;
    stack_14.setPS2(stack_8);
    stack_14.mult(f3);
    MarioActor* pActor3 = getPlayerActor();
    MarioConstTable* pConstTable1 = pActor3->mConst->mTable[pActor3->mConst->mCurrentTable];
    MarioActor* pActor4 = getPlayerActor();
    MarioConstTable* pConstTable2 = pActor4->mConst->mTable[pActor4->mConst->mCurrentTable];
    getPlayerActor()->mMario->setRocketBooster(stack_14, pConstTable1->mTornadoBoostAttnMini, pConstTable2->mTornadoBoostTimerMini);
    getPlayerActor()->mMario->startTornadoCentering(getPlayerActor()->_928);
} */

void MarioAccess::becomeNormalJumpStatus() {
    Mario* pMario = getPlayerActor()->mMario;
    pMario->_430 = 0;
}

void MarioAccess::setFrontVecKeepUp(const TVec3f &rSrc, u16 unused) {
    if (!getPlayerActor()->mMario->isStatusActive(5)) {
        getPlayerActor()->mMario->setFrontVecKeepUp(rSrc);
        getPlayerActor()->mFrontVec = getPlayerActor()->mMario->mFrontVec;
        getPlayerActor()->mUpVec = getPlayerActor()->mMario->mHeadVec;
        getPlayerActor()->mSideVec = getPlayerActor()->mMario->mSideVec;
    }
}

void MarioAccess::setFrontVecTarget(const TVec3f &rSrc, u16 a2) {
    MarioActor* pActor = getPlayerActor();
    pActor->_3C4 = rSrc;
    pActor->_3D0 = a2;
}

void MarioAccess::getThrowVec(TVec3f *pDest) {
    if (getPlayerActor()->mMario->isStatusActive(0x18)) {
        MarioActor* pActor = getPlayerActor();
        *pDest = pActor->mMario->mHeadVec;
        return;
    }
    if (getPlayerActor()->mMario->isSwimming()) {
        getPlayerActor()->getThrowVec(pDest);
        return;
    }
    else {
        MarioActor* pActor = getPlayerActor();
        pDest->set(pActor->_F3C_vec[pActor->_F40]); // needs to be un-inlined
    }
}

void MarioAccess::setTrans(const TVec3f &rSrc, u16 a2) {
    getPlayerActor()->mPosition.set(rSrc);
    getPlayerActor()->mMario->mPosition = rSrc;
    if (getPlayerActor()->mMario->mMovementStates._37) {
        getPlayerActor()->mMario->_688 = rSrc;
    }
    MarioActor* pActor = getPlayerActor();
    pActor->_1C0 = true;
    getPlayerActor()->mTransForCamera = rSrc;
    MR::updateHitSensorsAll(getPlayerActor());
}

void MarioAccess::endRush(const RushEndInfo *pRushEndInfo) {
    getPlayerActor()->endRush(pRushEndInfo);
}

void MarioAccess::incLife(u32 inc) {
    if (getPlayerActor()->mMario->isSwimming()) {
        for (u32 i = 0; i < inc; i++) {
            getPlayerActor()->mMario->mSwim->incLife();
        }
    }
    else {
        getPlayerActor()->incLife(inc);
    }
    if (getPlayerActor()->mPlayerMode == 4) {
        u32 i = 0;
        // this loop won't cooperate
        for (MarioActor* pActor = getPlayerActor(); i < pActor->mConst->mTable[pActor->mConst->mCurrentTable]->mAirWalkTime / 8; i++) {
            getPlayerActor()->mMario->incAirWalkTimer();
        }
    }
}

bool MarioAccess::isConfrontDeath() {
    if (!getPlayerActor()->mSuperKinokoCollected) {
        if (getPlayerActor()->mHealth == 0) {
            return true;
        }
        if (getPlayerActor()->mHealth == 1 && getPlayerActor()->mMario->isDamaging() && !getPlayerActor()->mMario->isSwimming()) {
            return true;
        }
    }
    if (getPlayerActor()->mMario->_735 && getPlayerActor()->mMario->isCurrentFloorSink()) {
        return true;
    }
    return !getPlayerActor()->isEnableNerveChange();
}

void MarioAccess::addStarPiece() {
    MarioActor* pActor = getPlayerActor();
    float fogLevel = pActor->mConst->mTable[pActor->mConst->mCurrentTable]->mStarPieceFogLevel;
    MarioActor* pActor2 = getPlayerActor();
    u8 fogTime = pActor2->mConst->mTable[pActor2->mConst->mCurrentTable]->mStarPieceFogTime;
    MarioActor* pActor3 = getPlayerActor();
    pActor3->_1AA = fogTime;
    pActor3->_1AC = fogLevel;
    pActor3->_1B0.r = 0xff;
    pActor3->_1B0.g = 0xff;
    pActor3->_1B0.b = 0xff;
    pActor3->_1B0.a = 0;
    pActor3->_1B5 = false;
}

void MarioAccess::getStarPieceDirect() {

}

MtxPtr MarioAccess::getJointMtx(const char *pName) {
    return getPlayerActor()->getGlobalJointMtx(pName);
}

const TVec3f& MarioAccess::getVelocity() {
    if (getPlayerActor()->_934) {
        return getPlayerActor()->getLastMove();
    }
    return getPlayerActor()->mMario->mVelocity;
}

const TVec3f& MarioAccess::getLastMove() {
    return getPlayerActor()->getLastMove();
}

void MarioAccess::hide() {
    MarioActor* pActor = getPlayerActor();
    pActor->_482 = true;
    MR::forceDeleteEffectAll(getPlayerActor());
    getPlayerActor()->updateHand();
    getPlayerActor()->updateFace();
}

void MarioAccess::show() {
    MarioActor* pActor = getPlayerActor();
    pActor->_482 = false;
}

HitSensor* MarioAccess::getTakingSensor() {
    if (getPlayerActor()->_428[0]) {
        return getPlayerActor()->_428[0];
    }
    if (getPlayerActor()->_424) {
        return getPlayerActor()->_424;
    }
    return nullptr;
}

void MarioAccess::dropTakingActor() {
    getPlayerActor()->rushDropThrowMemoSensor();
}

void MarioAccess::killTakingActor() {
    getPlayerActor()->damageDropThrowMemoSensor();
}

void MarioAccess::changeAnimationJ(const char *pAnimName) {
    getPlayerActor()->changeAnimationNonStop(pAnimName);
}

void MarioAccess::changeAnimationE(const char *pAnimName, s32 a2) {
    MarioActor* redundant = getPlayerActor();
    if (!getPlayerActor()->_B91) {
        if (getPlayerActor()->mMario->isPlayerModeTeresa()) {
            getPlayerActor()->changeTeresaAnimation(pAnimName, a2);
            return;
        }
        if (getPlayerActor()->_468.x == 0.0f) {
            getPlayerActor()->mMario->stopAnimationUpperForce();
        }
        MR::startBck(getPlayerActor(), pAnimName, nullptr);
        MarioActor* redundant2 = getPlayerActor();
        if (!(a2 < 0)) {
            getPlayerActor()->mMarioAnim->mXanimePlayer->changeInterpoleFrame(a2);
        }
        getPlayerActor()->setBlink(pAnimName);
        getPlayerActor()->mMarioAnim->closeCallback();
        getPlayerActor()->mMarioAnim->entryCallback(pAnimName);
    }
}

void MarioAccess::changeAnimationE(const char *pAnimName, const char *pAnimName2) {
    MarioActor* redundant = getPlayerActor();
    if (!getPlayerActor()->_B91) {
        if (getPlayerActor()->_468.x == 0.0f) {
            getPlayerActor()->mMario->stopAnimationUpperForce();
        }
        MR::startBck(getPlayerActor(), pAnimName, pAnimName2);
        getPlayerActor()->setBlink(pAnimName);
        getPlayerActor()->mMarioAnim->closeCallback();
        getPlayerActor()->mMarioAnim->entryCallback(pAnimName);
    }
}

void MarioAccess::changeAnimationE(const char *pAnimName, const BckCtrlData &bckData) {
    MarioActor* redundant = getPlayerActor();
    if (!getPlayerActor()->_B91) {
        if (getPlayerActor()->_468.x == 0.0f) {
            getPlayerActor()->mMario->stopAnimationUpperForce();
        }
        MR::startBck(getPlayerActor(), pAnimName, nullptr);
        MR::reflectBckCtrlData(getPlayerActor(), bckData);
        getPlayerActor()->setBlink(pAnimName);
        getPlayerActor()->mMarioAnim->closeCallback();
        getPlayerActor()->mMarioAnim->entryCallback(pAnimName);
    }
}

void MarioAccess::progressAnimation() {
    getPlayerActor()->mMarioAnim->update();
    getPlayerActor()->calcAnimInMovement();
    XanimePlayer* pAnimePlayer = getPlayerActor()->mMarioAnim->mXanimePlayer;
    pAnimePlayer->updateBeforeMovement();
    pAnimePlayer->updateAfterMovement();
}

const char* MarioAccess::getCurrentBckName() {
    return getPlayerActor()->mMario->getCurrentBckName();
}

void MarioAccess::setAnimationBlendWeight(const f32 *a1) {
    getPlayerActor()->mMarioAnim->forceSetBlendWeight(a1);
}

void MarioAccess::setSpot(f32, u32) {

}

void MarioAccess::offControl() {
    MarioActor* pActor = getPlayerActor();
    pActor->_3C0 = true;
}

bool MarioAccess::isOffControl() {
    return getPlayerActor()->_3C0;
}

void MarioAccess::onControl(bool resetCondition) {
    MarioActor* pActor = getPlayerActor();
    pActor->_3C0 = false;
    if (resetCondition) {
        getPlayerActor()->resetCondition();
    }
}

void MarioAccess::setStateWait() {
    getPlayerActor()->mMario->stopJump();
    getPlayerActor()->mMario->stopWalk();
}

void MarioAccess::startTalk(const LiveActor *pActor) {
    getPlayerActor()->mMario->startTalk(pActor);
    getPlayerActor()->stopSpinTicoEffect(false);
}

void MarioAccess::endTalk() {
    getPlayerActor()->mMario->endTalk();
}

/*
void MarioAccess::readyRemoteDemo() {
    if (!getPlayerActor()->_EA4) {
        if (getPlayerActor()->mMario->isStatusActive(0x13) || getPlayerActor()->mPlayerMode == 5) {
            MarioActor* pActor = getPlayerActor();
            pActor->_3C0 = true;
            getPlayerActor()->mMario->_10_HIGH_WORD |= 0x80000000;
            return;
        }
        getPlayerActor()->mMario->_10_HIGH_WORD &= 0x7fffffff;
        if (getPlayerActor()->_934) {
            MarioActor* pActor1 = getPlayerActor();
            MarioActor* pActor2 = getPlayerActor();
            pActor2->_924->receiveMessage(0x93, pActor1->getSensor("body"));
            if (getPlayerActor()->_934) {
                TVec3f stack_8 = TVec3f(0.0f, 0.0f, 0.0f);
                RushEndInfo stack_14 = RushEndInfo(nullptr, 4, stack_8, false, 0);
                getPlayerActor()->endRush(&stack_14);
            }
        }
        if (getPlayerActor()->mMario->isStatusActive(0x12)) {
            getPlayerActor()->mMario->closeStatus(nullptr);
        }
        getPlayerActor()->flushCoinPull();
        getPlayerActor()->control();
        getPlayerActor()->_3C0 = true;
        getPlayerActor()->_EA4 = true;
        MR::invalidateHitSensors(getPlayerActor());
        PSMTXCopy(getPlayerActor()->getBaseMtx(), getPlayerActor()->_EA8);
        getPlayerActor()->_B90 = true;
        getPlayerActor()->mMario->stopWalk();
        MR::deleteEffectAll(getPlayerActor());
        getPlayerActor()->_1E4 = 0.0f;
        getPlayerActor()->_ED8 = getPlayerActor()->mPosition;
        getPlayerActor()->_EE4 = getPlayerActor()->mMarioAnim->mXanimePlayer->getCurrentAnimationName();
        getPlayerActor()->_EA6 = false;
    }
}
*/

void MarioAccess::onFollowDemo() {
    if (getPlayerActor()->mPlayerMode == 6) {
        MR::requestMovementOn((LiveActor*)getPlayerActor()->_9A4);
    }
}

void MarioAccess::setBaseMtx(MtxPtr mtx) {
    getPlayerActor()->forceSetBaseMtx(mtx);
}

MtxPtr MarioAccess::getBaseMtx() {
    if (getPlayerActor()->_EA5) {
        return getPlayerActor()->_EA8;
    }
    return getPlayerActor()->getBaseMtx();
}

bool MarioAccess::calcSpinPullVelocity(TVec3f *a1, const TVec3f &a2) {
    getPlayerActor()->tryPullTrans(a1, a2);
    return true;
}

void MarioAccess::tryCoinPull() {
    bool temp = false;
    HitSensor* pSensor = getPlayerActor()->getSensor("eye");
    if (pSensor->mValidByHost && pSensor->mValidBySystem) {
        temp = true;
    }
    if (!temp) {
        getPlayerActor()->getSensor("eye")->validate();
    }
    MarioActor* pActor = getPlayerActor();
    pActor->_6D0 = 1;
}

void MarioAccess::addVelocity(const TVec3f &rIncrease) {
    if (!getPlayerActor()->mMario->isSwimming() || !getPlayerActor()->mMario->mSwim->_8A) {
        if (!getPlayerActor()->_EA4) {
            if (!getPlayerActor()->_934) {
                getPlayerActor()->mMario->push(rIncrease);
                if (getPlayerActor()->mMario->isStatusActive(0x5)) {
                    getPlayerActor()->mMario->mHang->forceDrop();
                }
            }
        }
    }
}

void MarioAccess::addVelocityFromArea(const TVec3f &rIncrease) {
    if (!getPlayerActor()->mMario->isSwimming() || !getPlayerActor()->mMario->mSwim->_8A) {
        if (!getPlayerActor()->_EA4) {
            if (!getPlayerActor()->_934) {
                getPlayerActor()->mMario->push(rIncrease);
            }
        }
    }
}

bool MarioAccess::isOnWaterSurface() {
    if (getPlayerActor()->mMario->isStatusActive(0x6)) {
        MarioSwim* pSwim = getPlayerActor()->mMario->mSwim;
        if (pSwim->_19 || pSwim->_20) {
            return true;
        }
        return false;
    }
    else {
        return getPlayerActor()->isAnimationRun("酸素回復");
    }
}

bool MarioAccess::calcWorldPadDir(TVec3f *pDest, f32 a2, f32 a3) {
    return getPlayerActor()->mMario->calcWorldPadDir(pDest, a2, a3, false);
}

void MarioAccess::preventRush() {
    getPlayerActor()->setNerve(&NrvMarioActor::MarioActorNrvNoRush::sInstance);
}

MarioActor* MarioAccess::getPlayerActor() {
    return MR::getMarioHolder()->getMarioActor();
}

void MarioAccess::validateSensor() {
    getPlayerActor()->getSensor("eye")->validate();
}

void MarioAccess::incOxygen(u32 amount) {
    if (getPlayerActor()->mMario->isSwimming()) {
        for (u32 i = 0; i < amount; i++) {
            getPlayerActor()->mMario->mSwim->incOxygen();
        }
        getPlayerActor()->playEffect("酸素回復");
    }
}

void MarioAccess::scatterStarPiece(u32 num) {
    if (MR::getStarPieceNum() > 0) {
        MR::startSound(getPlayerActor(), "SE_OJ_STAR_PIECE_BURST", -1, -1);
    }
    for (u32 i = 0; i < num; i++) {
        if (MR::getStarPieceNum() == 0) {
            break;
        }
        StarPiece* pStarPiece = MR::getDeadStarPiece();
        pStarPiece->launch(getPlayerActor()->_2A0, 24.0f, 24.0f, false, false);
        MR::addStarPiece(-1);
    }
}

void MarioAccess::startDownWipe() {
    if (!getPlayerActor()->isNerve(&NrvMarioActor::MarioActorNrvGameOverSink::sInstance)) {
        if (!getPlayerActor()->isAnimationRun("氷結")) {
            if (!getPlayerActor()->_390) {
                getPlayerActor()->_A61 = true;
            }
        }
    }
}

void MarioAccess::endRemoteDemo(const RushEndInfo *mRushInfo) {
    if (getPlayerActor()->mMario->isStatusActive(0x13)
        || getPlayerActor()->mMario->isStatusActive(0x22)
        || getPlayerActor()->mMario->_10._20
        || !getPlayerActor()->_EA4) {
            getPlayerActor()->_3C0 = false;
            return;
    }
    getPlayerActor()->_B90 = false;
    onControl(true);
    getPlayerActor()->_EA4 = false;
    getPlayerActor()->stopAnimation(nullptr);
    if (getPlayerActor()->_468.x != 0.0f) {
        HitSensor* pSensor;
        MarioActor* pActor = getPlayerActor();
        if (pActor->_468.x == 0.0f) {
            pSensor = nullptr;
        }
        else {
            pSensor = pActor->_428[0];
        }
        getPlayerActor()->mMarioAnim->updateTakingAnimation(pSensor);
    }
    MR::validateHitSensors(getPlayerActor());
    getPlayerActor()->getSensor("dummy")->invalidate();
    getPlayerActor()->_EA6 = false;
}

bool MarioAccess::isInWaterMode() {
    if (getPlayerActor()->mMario->isStatusActive(6)) {
        return true;
    }
    return getPlayerActor()->isAnimationRun("水泳ジェット");
}

void MarioAccess::changeItemStatus(u32 a1) {
    switch (a1) {
        case 0:
        case 8:
            getPlayerActor()->setPlayerMode(0, true);
            break;
        case 1:
            getPlayerActor()->setPlayerMode(5, true);
            break;
        case 2:
            getPlayerActor()->setPlayerMode(4, true);
            break;
        case 3:
            getPlayerActor()->setPlayerMode(6, true);
            break;
        case 4:
            getPlayerActor()->setPlayerMode(3, true);
            break;
        case 5:
            getPlayerActor()->setPlayerMode(2, true);
            break;
        case 6:
            getPlayerActor()->setPlayerMode(7, true);
            break;
        case 7:
            getPlayerActor()->setPlayerMode(1, true);
            break;
        case 9:
            getPlayerActor()->mMario->mMovementStates_LOW_WORD |= 0x10000;
            getPlayerActor()->mMario->_544 = 3;
            break;
        case 0xa:
            if (getPlayerActor()->isEnableNerveChange()) {
                if (getPlayerActor()->mMaxHealth > 3) {
                    MR::startSound(getPlayerActor(), "SE_OJ_MORPH_ITEM_INVALID", -1, -1);
                    getPlayerActor()->changeMaxLife(6);
                }
                else {
                    getPlayerActor()->mSuperKinokoCollected = true;
                }
            }
            break;
        default:
            break;
    }
}
