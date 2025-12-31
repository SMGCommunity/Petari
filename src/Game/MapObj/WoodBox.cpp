
#include "Game/MapObj/WoodBox.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/MapObj/BenefitItemObj.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace NrvWoodBox {
    NEW_NERVE(WoodBoxNrvWait, WoodBox, Wait);
    NEW_NERVE(WoodBoxNrvKilled, WoodBox, Killed);
    NEW_NERVE(WoodBoxNrvHit, WoodBox, Hit);
}  // namespace NrvWoodBox

WoodBox::WoodBox(const char* pName) : LiveActor(pName) {}

WoodBox::~WoodBox() {}

void WoodBox::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("WoodBox", nullptr, false);
    MR::connectToSceneMapObjStrongLight(this);
    MR::initLightCtrl(this);
    initSound(6, false);
    initNerve(&NrvWoodBox::WoodBoxNrvWait::sInstance);
    initEffectKeeper(0, nullptr, false);

    f32 radius = 120.0f * mScale.x;
    initHitSensor(1);
    TVec3f sensorOffset(0.0f, radius, 0.0f);
    MR::addHitSensor(this, "body", ATYPE_PUNCH_BOX, 8, radius, sensorOffset);

    s32 arg0_Coins = 1;
    s32 arg1_Respawn = -1;
    s32 arg2_StarBits = 0;
    s32 arg3 = 0;
    s32 arg4_1Up = 0;
    s32 arg7_PowerStar = 0;

    if (rIter.isValid()) {
        MR::getJMapInfoArg0NoInit(rIter, &arg0_Coins);
        MR::getJMapInfoArg1NoInit(rIter, &arg1_Respawn);
        MR::getJMapInfoArg2NoInit(rIter, &arg2_StarBits);
        MR::getJMapInfoArg3NoInit(rIter, &arg3);
        MR::getJMapInfoArg4NoInit(rIter, &arg4_1Up);
        MR::getJMapInfoArg7NoInit(rIter, &arg7_PowerStar);
    }

    calcAndSetBaseMtx();
    PSMTXCopy(getBaseMtx(), mBaseMtx);
    TVec3f transOffset(0.0f, 100.0f, 0.0f);
    MR::addTransMtx(mBaseMtx, transOffset);

    mHasPowerStar = false;
    if (arg7_PowerStar != 0) {
        mHasPowerStar = true;
        MR::declarePowerStar(this);
        mStarDemoModel = MR::createPowerStarDemoModel(this, "パワースターデモモデル", mBaseMtx);
        mStarDemoModel->makeActorDead();
    }

    mCoinCount = arg0_Coins;
    mStarBitCount = arg2_StarBits;
    mHitPoint = 1;
    mIsNoRespawn = true;
    if (arg1_Respawn == 0) {
        mIsNoRespawn = false;
    }
    mPlaySolveSE = false;
    if (arg3 == 1) {
        mPlaySolveSE = true;
    }

    mBreakModel = new ModelObj("壊れモデル", "WoodBoxBreak", getBaseMtx(), 0xF, -2, -2, false);
    mBreakModel->mScale.setPS2(mScale);
    MR::initLightCtrl(mBreakModel);
    mBreakModel->kill();
    mFloorTouchTimer = 0;

    setNerve(&NrvWoodBox::WoodBoxNrvWait::sInstance);
    HitSensor* pBodySensor = getSensor("body");
    MR::initCollisionParts(this, "WoodBox", pBodySensor, nullptr);

    MR::setGroupClipping(this, rIter, 32);
    MR::setClippingFar200m(this);
    MR::validateClipping(this);

    if (mCoinCount != 0) {
        MR::declareCoin(this, mCoinCount);
    }
    if (mStarBitCount != 0) {
        MR::declareStarPiece(this, mStarBitCount);
    }

    if (arg4_1Up != 0) {
        mOneUp = MR::createKinokoOneUp();
        mOneUp->_DD = 1;
    } else {
        mOneUp = nullptr;
    }

    MR::useStageSwitchWriteDead(this, rIter);
    MR::useStageSwitchSleep(this, rIter);

    appear();
}

void WoodBox::control() {
    if (mFloorTouchTimer > 0) {
        mFloorTouchTimer--;
    }
}

void WoodBox::calcViewAndEntry() {
    if (!MR::isDead(this) && !MR::isClipped(this)) {
        LiveActor::calcViewAndEntry();
    }
}

bool WoodBox::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (mHitPoint == 0) {
        return false;
    }

    if (MR::isMsgPlayerUpperPunch(msg)) {
        doHit(pSender, pReceiver);
        pSender->receiveMessage(ACTMES_REFLECT_V, pReceiver);
        return true;
    }

    if (MR::isMsgFireBallAttack(msg)) {
        doHit(pSender, pReceiver);
        return false;
    }

    if (MR::isMsgPlayerHitAll(msg)) {
        doHit(pSender, pReceiver);
        return true;
    }

    if (msg == ACTMES_PLAYER_HIP_DROP || msg == ACTMES_PLAYER_HIP_DROP_FLOOR) {
        if (mFloorTouchTimer != 0) {
            doHit(pSender, pReceiver);
            return true;
        }
        return true;
    }

    return false;
}

bool WoodBox::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_IS_PUNCH_ENABLE) {
        return mHitPoint != 0;
    }

    if (msg == ACTMES_IS_BROKEN) {
        return mHitPoint == 0;
    }

    if (msg == ACTMES_IS_RUSH_ENABLE && mHitPoint != 0) {
        return true;
    }

    if (msg == ACTMES_FLOOR_TOUCH) {
        mFloorTouchTimer = 2;
    }

    if (msg == ACTMES_IS_OPD_SITE_IN && mHitPoint != 0) {
        return true;
    }

    if ((msg == ACTMES_OPD_SHOOT || msg - ACTMES_BALL_DASH_WALL <= 1) && mHitPoint != 0) {
        doHit(pSender, pReceiver);
        return true;
    }

    if (mHitPoint != 0 && (msg - ACTMES_TURNIP_ATTACK <= 1)) {
        doHit(pSender, pReceiver);
        return true;
    }

    return false;
}

bool WoodBox::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (mHitPoint != 0) {
        doHit(pSender, pReceiver);
        return true;
    }
    return false;
}

void WoodBox::kill() {
    if (!mIsNoRespawn) {
        LiveActor::kill();
    } else {
        LiveActor::setNerve(&NrvWoodBox::WoodBoxNrvWait::sInstance);
        MR::invalidateHitSensors(this);
        MR::invalidateCollisionParts(this);
    }
    mBreakModel->kill();
    if (mHasPowerStar) {
        mStarDemoModel->makeActorDead();
        MR::requestAppearPowerStar(this, mPosition);
    }
}

void WoodBox::exeWait() {}
void WoodBox::exeHit() {
    if (LiveActor::getNerveStep() == 0) {
        LiveActor::getSensor("body")->invalidate();
        if (mHasPowerStar) {
            mStarDemoModel->makeActorAppeared();
        }
    }

    if (LiveActor::getNerveStep() == 15) {
        if (mCoinCount != 0) {
            MR::appearCoinPop(this, mPosition, mCoinCount);
        }

        if (mStarBitCount != 0) {
            MR::appearStarPiece(this, mPosition, mStarBitCount, 10.0f, 40.0f, false);
            MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
        }

        if (mOneUp) {
            TVec3f upVec;
            MR::calcUpVec(&upVec, this);
            Mtx spawnMtx;
            PSMTXCopy(getBaseMtx(), spawnMtx);

            f32 scale = 50.0f;
            TVec3f spawnOffset = upVec;
            spawnOffset.mult(scale);
            MR::addTransMtx(spawnMtx, spawnOffset);

            MR::appearKinokoOneUpPop((BenefitItemObj*)mOneUp, spawnMtx, 30.0f);
        }

        if (MR::isValidSwitchDead(this)) {
            MR::onSwitchDead(this);
        }

        if (mPlaySolveSE) {
            MR::startSystemSE("SE_SY_READ_RIDDLE_S", -1, -1);
        }
    }

    if (MR::isBckStopped(mBreakModel)) {
        kill();
    }
}
void WoodBox::exeKilled() {}

void WoodBox::doHit(HitSensor* pSender, HitSensor* pReceiver) {
    if (mHitPoint == 0) {
        return;
    }
    if (--mHitPoint == 0) {
        MR::invalidateCollisionParts(this);
    }

    if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
        MR::startSound(this, "SE_OJ_WOOD_BOX_BREAK_W", -1, -1);
    } else {
        MR::startSound(this, "SE_OJ_WOOD_BOX_BREAK", -1, -1);
    }
    if (!MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
        MR::startSound(this, "SE_EM_EXPLODE_S", -1, -1);
    }

    mBreakModel->appear();
    MR::startBck(mBreakModel, "Break", nullptr);

    if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
        MR::emitEffect(mBreakModel, "BreakWater");
    } else {
        MR::emitEffect(mBreakModel, "Break");
    }

    if (mIsNoRespawn) {
        MR::startBva(this, "WoodBox");
    } else {
        MR::hideModel(this);
    }

    setNerve(&NrvWoodBox::WoodBoxNrvHit::sInstance);
}
