#include "Game/Player/MarioAccess.hpp"
#include "Game/LiveActor/Binder.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/MapObj/CollectCounter.hpp"
#include "Game/MapObj/StarPiece.hpp"
#include "Game/MapObj/StarPieceDirector.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioFpView.hpp"
#include "Game/Player/MarioHang.hpp"
#include "Game/Player/MarioHolder.hpp"
#include "Game/Player/MarioParts.hpp"
#include "Game/Player/MarioState.hpp"
#include "Game/Player/MarioSwim.hpp"
#include "Game/Player/RushEndInfo.hpp"
#include "Game/Util.hpp"

namespace MarioAccess {
    void getTakePos(TVec3f* pOut) {
        if (getPlayerActor()->_494 != nullptr) {
            getPlayerActor()->_494->calc();
            getPlayerActor()->_494->copyTrans(pOut);
        }
    }

    bool isOnActor(const LiveActor* pActor) {
        if (getPlayerActor()->getMario()->_1C._13) {
            Triangle* marioGroundPolygon = getPlayerActor()->getMario()->mGroundPolygon;
            if (marioGroundPolygon->isValid()) {
                return marioGroundPolygon->mSensor->mHost == pActor;
            }

            return false;
        }

        if (getPlayerActor()->getMario()->_1C._14 || (getPlayerActor()->IsMarioSwimming() && getPlayerActor()->getMovementStates()._2)) {
            Triangle* marioTri = getPlayerActor()->getMario()->_45C;
            if (!marioTri->isValid()) {
                return false;
            }

            return marioTri->mSensor->mHost == pActor;
        }

        return false;
    }

    bool isOnGround(u32 a1) {
        if (a1 != 0) {
            return false;
        }

        if (getPlayerActor()->_934) {
            return MR::isOnGround(getPlayerActor()->_924->mHost);
        }

        return getPlayerActor()->getMovementStates()._1;
    }

    bool isHipDropFalling() {
        if (getPlayerActor()->getMovementStates().jumping && getPlayerActor()->getMovementStates()._B && !getPlayerActor()->getMovementStates()._1 &&
            !getPlayerActor()->isJumpRising()) {
            return true;
        }

        return false;
    }

    bool isHipDropLand() {
        return getPlayerActor()->getDrawStates()._14;
    }

    bool isSwingAction() {
        if (getPlayerActor()->_934) {
            return false;
        }

        if (getPlayerActor()->getMovementStates()._F) {
            return true;
        }

        return getPlayerActor()->isPunching();
    }

    bool isInRush() {
        return getPlayerActor()->_934 || getPlayerActor()->getMario()->isStatusActive(MarioStatus_13);
    }

    bool isSquat() {
        return getPlayerActor()->getMovementStates()._A && getPlayerActor()->getMovementStates()._1;
    }

    bool isParalyzing() {
        return getPlayerActor()->getMario()->isStatusActive(MarioStatus_Paralyze);
    }

    bool isTeresaDisappear() {
        if (getPlayerActor()->mPlayerMode != PlayerMode_Teresa) {
            return false;
        }

        return getPlayerActor()->getMario()->_418;
    }

    bool isFlying() {
        if (getPlayerActor()->getMario()->isStatusActive(MarioStatus_Foo)) {
            return true;
        }

        return getPlayerActor()->getMario()->_10._21;
    }

    bool isNeedBrakingCamera() {
        return getPlayerActor()->getMario()->_1C._9;
    }

    CubeCameraArea* getCameraCubeCode() {
        return getPlayerActor()->getMario()->getCameraCubeCode();
    }

    bool isSwimming() {
        return getPlayerActor()->getMario()->isSwimming();
    }

    bool isSkating() {
        return getPlayerActor()->getMario()->isStatusActive(MarioStatus_Skate);
    }

    Triangle* getGroundingPolygon(u32) {
        if (isSwimming()) {
            if (getPlayerActor()->getMovementStates()._2) {
                return getPlayerActor()->getMario()->_45C;
            }

            return nullptr;
        }

        if (!isOnGround(0)) {
            return nullptr;
        }

        if (getPlayerActor()->_934) {
            return &getPlayerActor()->_924->mHost->mBinder->mGroundInfo.mParentTriangle;
        }

        return getPlayerActor()->getMario()->mGroundPolygon;
    }

    Triangle* getShadowingPolygon() {
        if (getPlayerActor()->getMovementStates()._2) {
            return getPlayerActor()->getMario()->_45C;
        }

        return nullptr;
    }

    f32 getShadowHeight() {
        return getPlayerActor()->getMario()->mVerticalSpeed;
    }

    void forceKill(u32 type, u32) {
        getPlayerActor()->forceKill(type);
    }

    bool isOnPress() {
        return getPlayerActor()->_390 != 0;
    }

    bool isDisableFpView() {
        return getPlayerActor()->getMario()->isDisableFpViewMode();
    }

    bool isFpViewChangingFailure() {
        return getPlayerActor()->getMario()->_898;
    }

    void stopFpView() {
        if (getPlayerActor()->getMario()->isStatusActive(MarioStatus_FpView)) {
            getPlayerActor()->getMario()->mFpView->forceClose();
            getPlayerActor()->getMario()->closeStatus(nullptr);
        }
    }

    void noticeDashChance() {
        getPlayerActor()->getMario()->_436 = 5;
    }

    void setWalkingResist(f32 walkingResist) {
        getPlayerActor()->getMario()->_2D0 = walkingResist;
    }

    void forceFly(const TVec3f& rVec1, const TVec3f& rVec2, s32 a3) {
        getPlayerActor()->getMario()->doPointWarp(rVec1, rVec2, a3);
    }

    void setJumpVec(const TVec3f& rJumpVec) {
        getPlayerActor()->getMario()->mJumpVec = rJumpVec;
    }

    void forceJump(const TVec3f& rVec, u32) {
        getPlayerActor()->getMario()->mMovementStates._21 = true;
        getPlayerActor()->getMario()->tryForceJumpDelay(rVec);
    }

    void freeJump(const TVec3f& rVec, u32) {
        getPlayerActor()->getMario()->mMovementStates._21 = true;
        getPlayerActor()->getMario()->tryFreeJumpDelay(rVec);
    }

    void tornadoJump() {
        getPlayerActor()->getMario()->_544 = getPlayerActor()->getConst().getTable()->mTornadoTimeAir;
        getPlayerActor()->getMario()->tryTornadoJump();
        getPlayerActor()->mMario->startTornadoCentering(getPlayerActor()->_928);
    }

    void tornadoJumpMini() {
        getPlayerActor()->getMario()->_544 = getPlayerActor()->getConst().getTable()->mTornadoTimeAir;
        getPlayerActor()->getMario()->tryTornadoJump();
        TVec3f up(-*getPlayerActor()->getMario()->getGravityVec() * getPlayerActor()->getConst().getTable()->mTornadoBoostPower);
        getPlayerActor()->getMario()->setRocketBooster(up, getPlayerActor()->getConst().getTable()->mTornadoBoostAttnMini,
                                                       getPlayerActor()->getConst().getTable()->mTornadoBoostTimerMini);
        getPlayerActor()->mMario->startTornadoCentering(getPlayerActor()->_928);
    }

    void becomeNormalJumpStatus() {
        getPlayerActor()->getMario()->_430 = 0;
    }

    void setFrontVecKeepUp(const TVec3f& rVec, u16) {
        if (getPlayerActor()->getMario()->isStatusActive(MarioStatus_Hang)) {
            return;
        }

        getPlayerActor()->getMario()->setFrontVecKeepUp(rVec);
        getPlayerActor()->_2DC = getPlayerActor()->getMario()->mFrontVec;
        getPlayerActor()->mUpVec = getPlayerActor()->getMario()->mHeadVec;
        getPlayerActor()->_2E8 = getPlayerActor()->getMario()->mSideVec;
    }

    void setFrontVecTarget(const TVec3f& rVec, u16 a2) {
        MarioActor* actor = getPlayerActor();
        actor->_3C4 = rVec;
        actor->_3D0 = a2;
    }

    void getThrowVec(TVec3f* pOut) {
        if (getPlayerActor()->getMario()->isStatusActive(MarioStatus_Foo)) {
            *pOut = getPlayerActor()->getMario()->mHeadVec;
            return;
        }

        if (getPlayerActor()->getMario()->isSwimming()) {
            getPlayerActor()->getThrowVec(pOut);
            return;
        }

        pOut->set(getPlayerActor()->_F3CVec[getPlayerActor()->_F40]);
    }

    void setTrans(const TVec3f& rTrans, u16) {
        getPlayerActor()->mPosition.set(rTrans);
        getPlayerActor()->getMario()->mPosition = rTrans;

        if (getPlayerActor()->getMovementStates()._37) {
            getPlayerActor()->getMario()->_688 = rTrans;
        }

        getPlayerActor()->_1C0 = true;

        getPlayerActor()->mCameraTrans = rTrans;
        MR::updateHitSensorsAll(getPlayerActor());
    }

    void endRush(const RushEndInfo* pRushEndInfo) {
        getPlayerActor()->endRush(pRushEndInfo);
    }

    void incLife(u32 amt) {
        if (getPlayerActor()->IsMarioSwimming()) {
            for (int i = 0; i < amt; i++) {
                getPlayerActor()->getMario()->mSwim->incLife();
            }
        } else {
            getPlayerActor()->incLife(amt);
        }

        if (getPlayerActor()->mPlayerMode == PlayerMode_Bee) {
            for (u32 i = 0; i < getPlayerActor()->getConst().getTable()->mAirWalkTime / 8; i++) {
                getPlayerActor()->getMario()->incAirWalkTimer();
            }
        }
    }

    bool isConfrontDeath() {
        if (!getPlayerActor()->mSuperKinokoCollected) {
            if (getPlayerActor()->mHealth == 0) {
                return true;
            }

            if (getPlayerActor()->mHealth == 1 && getPlayerActor()->getMario()->isDamaging() && !getPlayerActor()->IsMarioSwimming()) {
                return true;
            }
        }

        if (getPlayerActor()->getMario()->mSinkTimer != 0 && getPlayerActor()->getMario()->isCurrentFloorSink()) {
            return true;
        }

        return !getPlayerActor()->isEnableNerveChange();
    }

    void addStarPiece() {
        f32 fogLevel = getPlayerActor()->getConst().getTable()->mStarPieceFogLevel;
        u8 fogTime = getPlayerActor()->getConst().getTable()->mStarPieceFogTime;
        MarioActor* actor = getPlayerActor();
        actor->_1AA = fogTime;
        actor->_1AC = fogLevel;
        actor->_1B0.set(0xFF, 0xFF, 0xFF, 0);
        actor->_1B5 = false;
    }

    void getStarPieceDirect() {
    }

    MtxPtr getJointMtx(const char* pName) {
        return getPlayerActor()->getGlobalJointMtx(pName);
    }

    TVec3f* getVelocity() {
        if (getPlayerActor()->_934) {
            return getLastMove();
        }

        return &getPlayerActor()->getMario()->mVelocity;
    }

    TVec3f* getLastMove() {
        return const_cast< TVec3f* >(&getPlayerActor()->getLastMove());
    }

    void hide() {
        getPlayerActor()->_482 = true;
        MR::forceDeleteEffectAll(getPlayerActor());
        getPlayerActor()->updateHand();
        getPlayerActor()->updateFace();
    }

    void show() {
        getPlayerActor()->_482 = false;
    }

    HitSensor* getTakingSensor() {
        if (getPlayerActor()->_428[0] != nullptr) {
            return getPlayerActor()->_428[0];
        }

        if (getPlayerActor()->_424 != nullptr) {
            return getPlayerActor()->_424;
        }

        return nullptr;
    }

    void dropTakingActor() {
        getPlayerActor()->rushDropThrowMemoSensor();
    }

    void killTakingActor() {
        getPlayerActor()->damageDropThrowMemoSensor();
    }

    f32 getAnimationFrameMax() {
        return getPlayerActor()->mMarioAnim->getXanimePlayer()->_20->mEnd;
    }

    void changeAnimationJ(const char* pAnimName) {
        getPlayerActor()->changeAnimationNonStop(pAnimName);
    }

    void changeAnimationE(const char* pAnimName, s32 a2) {
        // unused
        getPlayerActor();
        if (getPlayerActor()->_B91) {
            return;
        }

        if (getPlayerActor()->getMario()->isPlayerModeTeresa()) {
            getPlayerActor()->changeTeresaAnimation(pAnimName, a2);
            return;
        }

        if (getPlayerActor()->_468 == 0) {
            getPlayerActor()->getMario()->stopAnimationUpperForce();
        }

        MR::startBck(getPlayerActor(), pAnimName, nullptr);

        // unused
        getPlayerActor();

        if (a2 >= 0) {
            getPlayerActor()->mMarioAnim->getXanimePlayer()->changeInterpoleFrame(a2);
        }

        getPlayerActor()->setBlink(pAnimName);
        getPlayerActor()->mMarioAnim->closeCallback();
        getPlayerActor()->mMarioAnim->entryCallback(pAnimName);
    }

    void changeAnimationE(const char* pAnimName, const char* pChar2) {
        // unused
        getPlayerActor();
        if (getPlayerActor()->_B91) {
            return;
        }

        if (getPlayerActor()->_468 == 0) {
            getPlayerActor()->getMario()->stopAnimationUpperForce();
        }

        MR::startBck(getPlayerActor(), pAnimName, pChar2);

        getPlayerActor()->setBlink(pAnimName);
        getPlayerActor()->mMarioAnim->closeCallback();
        getPlayerActor()->mMarioAnim->entryCallback(pAnimName);
    }

    void changeAnimationE(const char* pAnimName, const BckCtrlData& rBckCtrlData) {
        // unused
        getPlayerActor();
        if (getPlayerActor()->_B91) {
            return;
        }

        if (getPlayerActor()->_468 == 0) {
            getPlayerActor()->getMario()->stopAnimationUpperForce();
        }

        MR::startBck(getPlayerActor(), pAnimName, nullptr);

        MR::reflectBckCtrlData(getPlayerActor(), rBckCtrlData);

        getPlayerActor()->setBlink(pAnimName);
        getPlayerActor()->mMarioAnim->closeCallback();
        getPlayerActor()->mMarioAnim->entryCallback(pAnimName);
    }

    void keepCurrentAnimation() {
        if (getPlayerActor()->mMarioAnim->getXanimePlayer()->_20->getAttribute() == 0) {
            getPlayerActor()->mMarioAnim->getXanimePlayer()->_20->setAttribute(1);
        }
    }

    void progressAnimation() {
        getPlayerActor()->mMarioAnim->update();
        getPlayerActor()->calcAnimInMovement();

        XanimePlayer* animePlayer = getPlayerActor()->mMarioAnim->getXanimePlayer();
        animePlayer->updateBeforeMovement();
        animePlayer->updateAfterMovement();
    }

    const char* getCurrentBckName() {
        return getPlayerActor()->getMario()->getCurrentBckName();
    }

    void setAnimationBlendWeight(const f32* pWeights) {
        getPlayerActor()->mMarioAnim->forceSetBlendWeight(pWeights);
    }

    void setSpot(f32, u32) {
    }

    void offControl() {
        getPlayerActor()->_3C0 = true;
    }

    bool isOffControl() {
        return getPlayerActor()->_3C0;
    }

    void onControl(bool resetCondition) {
        getPlayerActor()->_3C0 = false;

        if (resetCondition) {
            getPlayerActor()->resetCondition();
        }
    }

    void setStateWait() {
        getPlayerActor()->getMario()->stopJump();
        getPlayerActor()->getMario()->stopWalk();
    }

    void startTalk(const LiveActor* pTalkActor) {
        getPlayerActor()->getMario()->startTalk(pTalkActor);
        getPlayerActor()->stopSpinTicoEffect(false);
    }

    void endTalk() {
        getPlayerActor()->getMario()->endTalk();
    }

    void readyRemoteDemo() {
        if (getPlayerActor()->_EA4) {
            return;
        }

        if (getPlayerActor()->getMario()->isStatusActive(MarioStatus_13) || getPlayerActor()->mPlayerMode == PlayerMode_Hopper) {
            offControl();
            getPlayerActor()->getMario()->_10._20 = true;
            return;
        }

        getPlayerActor()->getMario()->_10._20 = false;

        if (getPlayerActor()->_934) {
            getPlayerActor()->_924->receiveMessage(ACTMES_RUSH_CANCEL, getPlayerActor()->getSensor("body"));
            if (getPlayerActor()->_934) {
                RushEndInfo endInfo(nullptr, 4, TVec3f(0.0f, 0.0f, 0.0f), false, 0);
                endRush(&endInfo);
            }
        }

        if (getPlayerActor()->getMario()->isStatusActive(MarioStatus_FpView)) {
            getPlayerActor()->getMario()->closeStatus(nullptr);
        }

        getPlayerActor()->flushCoinPull();
        getPlayerActor()->calcAndSetBaseMtx();

        offControl();

        getPlayerActor()->_EA4 = true;
        MR::invalidateHitSensors(getPlayerActor());

        PSMTXCopy(getPlayerActor()->getBaseMtx(), getPlayerActor()->_EA8);

        getPlayerActor()->_B90 = true;
        getPlayerActor()->getMario()->stopWalk();

        MR::deleteEffectAll(getPlayerActor());

        getPlayerActor()->_1B8->kill();
        getPlayerActor()->_1E4 = 0.0f;
        getPlayerActor()->_ED8 = getPlayerActor()->mPosition;
        getPlayerActor()->_EE4 = getPlayerActor()->mMarioAnim->getXanimePlayer()->getCurrentAnimationName();
        getPlayerActor()->_EA6 = false;
    }

    void onFollowDemo() {
        if (getPlayerActor()->mPlayerMode == PlayerMode_Teresa) {
            MR::requestMovementOn(getPlayerActor()->_9A4);
        }
    }

    void setBaseMtx(MtxPtr mtx) {
        getPlayerActor()->forceSetBaseMtx(mtx);
    }

    MtxPtr getBaseMtx() {
        if (getPlayerActor()->_EA5) {
            return getPlayerActor()->_EA8;
        }

        return getPlayerActor()->getBaseMtx();
    }

    bool calcSpinPullVelocity(TVec3f* pOut, const TVec3f& rVec) {
        getPlayerActor()->tryPullTrans(pOut, rVec);
        return true;
    }

    void tryCoinPull() {
        if (!getPlayerActor()->getSensor("eye")->isValid()) {
            validateSensor();
        }
        getPlayerActor()->_6D0 = true;
    }

    void addVelocity(const TVec3f& rVec) {
        if (getPlayerActor()->IsMarioSwimming() && getPlayerActor()->getMario()->mSwim->mJetTimer != 0) {
            return;
        }

        if (getPlayerActor()->_EA4) {
            return;
        }

        if (getPlayerActor()->_934) {
            return;
        }

        getPlayerActor()->getMario()->push(rVec);

        if (getPlayerActor()->getMario()->isStatusActive(MarioStatus_Hang)) {
            getPlayerActor()->getMario()->mHang->forceDrop();
        }
    }

    void addVelocityFromArea(const TVec3f& rVec) {
        if (getPlayerActor()->IsMarioSwimming() && getPlayerActor()->getMario()->mSwim->mJetTimer != 0) {
            return;
        }

        if (getPlayerActor()->_EA4) {
            return;
        }

        if (getPlayerActor()->_934) {
            return;
        }

        getPlayerActor()->getMario()->push(rVec);
    }

    bool isOnWaterSurface() {
        if (getPlayerActor()->getMario()->isStatusActive(MarioStatus_Swim)) {
            return getPlayerActor()->getMario()->mSwim->isOnWaterSurface();
        }

        return getPlayerActor()->isAnimationRun("水泳ジェット");
    }

    void calcWorldPadDir(TVec3f* pOut, f32 f1, f32 f2) {
        getPlayerActor()->getMario()->calcWorldPadDir(pOut, f1, f2, false);
    }

    void preventRush() {
        getPlayerActor()->setNerve(&NrvMarioActor::MarioActorNrvNoRush::sInstance);
    }

    MarioActor* getPlayerActor() {
        return MR::getMarioHolder()->getMarioActor();
    }

    void validateSensor() {
        getPlayerActor()->getSensor("eye")->validate();
    }

    void incOxygen(u32 amt) {
        if (!getPlayerActor()->IsMarioSwimming()) {
            return;
        }

        for (int i = 0; i < amt; i++) {
            getPlayerActor()->getMario()->mSwim->incOxygen();
        }

        getPlayerActor()->playEffect("酸素回復");
    }

    void scatterStarPiece(u32 amt) {
        if (MR::getStarPieceNum() > 0) {
            MR::startSound(getPlayerActor(), "SE_OJ_STAR_PIECE_BURST");
        }

        for (int i = 0; i < amt; i++) {
            if (MR::getStarPieceNum() == 0) {
                return;
            }

            StarPiece* launchPiece = MR::getDeadStarPiece();

            launchPiece->launch(getPlayerActor()->_2A0, 24.0f, 24.0f, false, false);
            MR::addStarPiece(-1);
        }
    }

    void startDownWipe() {
        if (getPlayerActor()->isNerve(&NrvMarioActor::MarioActorNrvGameOverSink::sInstance)) {
            return;
        }

        if (getPlayerActor()->isAnimationRun("氷結")) {
            return;
        }

        if (getPlayerActor()->_390 != 0) {
            return;
        }

        getPlayerActor()->_A61 = true;
    }

    void readyDemo() {
        if (getPlayerActor()->_934) {
            if (!getPlayerActor()->_924->receiveMessage(ACTMES_RUSH_CANCEL, getPlayerActor()->getSensor("body"))) {
                getPlayerActor()->_924->receiveMessage(ACTMES_RUSH_FORCE_CANCEL, getPlayerActor()->getSensor("body"));
            }

            if (getPlayerActor()->_934) {
                RushEndInfo endInfo(nullptr, 4, TVec3f(0.0f, 0.0f, 0.0f), false, 0);
                endRush(&endInfo);
            }
        }

        if (getPlayerActor()->getMario()->isStatusActive(MarioStatus_FpView)) {
            getPlayerActor()->getMario()->closeStatus(nullptr);
        }
    }

    void endRemoteDemo(const RushEndInfo* pEndInfo) {
        if (getPlayerActor()->getMario()->isStatusActive(MarioStatus_13) || getPlayerActor()->getMario()->isStatusActive(MarioStatus_Talk) ||
            getPlayerActor()->getMario()->_10._20 || !getPlayerActor()->_EA4) {
            getPlayerActor()->_3C0 = false;
            return;
        }

        getPlayerActor()->_B90 = false;
        onControl(true);
        getPlayerActor()->_EA4 = false;
        getPlayerActor()->stopAnimation(nullptr);

        if (getPlayerActor()->_468 != 0) {
            MarioActor* actor = getPlayerActor();
            getPlayerActor()->mMarioAnim->updateTakingAnimation(actor->_468 == 0 ? nullptr : actor->_428[0]);
        }

        MR::validateHitSensors(getPlayerActor());
        getPlayerActor()->getSensor("dummy")->invalidate();
        getPlayerActor()->_EA6 = false;
    }

    bool isInWaterMode() {
        if (getPlayerActor()->getMario()->isStatusActive(MarioStatus_Swim)) {
            return true;
        }

        return getPlayerActor()->isAnimationRun("水泳ジェット");
    }

    void changeItemStatus(s32 a1) {
        switch (a1) {
        case 0:
        case 8:
            getPlayerActor()->setPlayerMode(PlayerMode_0, true);
            break;
        case 1:
            getPlayerActor()->setPlayerMode(PlayerMode_Hopper, true);
            break;
        case 2:
            getPlayerActor()->setPlayerMode(PlayerMode_Bee, true);
            break;
        case 3:
            getPlayerActor()->setPlayerMode(PlayerMode_Teresa, true);
            break;
        case 4:
            getPlayerActor()->setPlayerMode(PlayerMode_Ice, true);
            break;
        case 5:
            getPlayerActor()->setPlayerMode(PlayerMode_2, true);
            break;
        case 6:
            getPlayerActor()->setPlayerMode(PlayerMode_Foo, true);
            break;
        case 7:
            getPlayerActor()->setPlayerMode(PlayerMode_Invincible, true);
            break;
        case 9:
            getPlayerActor()->getMario()->mMovementStates._F = true;
            getPlayerActor()->getMario()->_544 = 3;
            break;
        case 10:
            if (!getPlayerActor()->isEnableNerveChange()) {
                return;
            }

            if (getPlayerActor()->mMaxHealth > 3) {
                MR::startSound(getPlayerActor(), "SE_OJ_MORPH_ITEM_INVALID");
                getPlayerActor()->changeMaxLife(6);
                return;
            }

            getPlayerActor()->mSuperKinokoCollected = true;
        }
    }
};  // namespace MarioAccess
