#include "Game/Ride/SlingShooter.hpp"
#include "Game/Camera/CameraTargetMtx.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/SpiderThread.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/mtx.h>
#include <revolution/wpad.h>

inline void endBindAndPlayerNoJump(LiveActor* pActor) {
    TVec3f jumpVec(0.0f, 0.0f, 0.0f);
    MR::endBindAndPlayerJump(pActor, jumpVec, 0);
}

namespace NrvSlingShooter {
    NEW_NERVE(SlingShooterNrvFree, SlingShooter, Free);
    NEW_NERVE(SlingShooterNrvFreeInvalid, SlingShooter, FreeInvalid);
    NEW_NERVE(SlingShooterNrvLand, SlingShooter, Land);
    NEW_NERVE(SlingShooterNrvWait, SlingShooter, Wait);
    NEW_NERVE(SlingShooterNrvAim, SlingShooter, Aim);
    NEW_NERVE(SlingShooterNrvAttack, SlingShooter, Attack);
    NEW_NERVE(SlingShooterNrvAttackSuccess, SlingShooter, AttackSuccess);
}  // namespace NrvSlingShooter

SlingShooter::SlingShooter(const char* pName)
    : LiveActor(pName), mIsKinopioCameraFocused(false), mPlayerZ(0.0f), mIsKinopioAttached(false), mIsKinopioTalkEnabled(true), mIsLastDamage(false),
      mBasePos(nullptr), mNeutralPos(nullptr), mUp(nullptr), mPointerPos(0.0f, 0.0f, 0.0f), mPadChannel(-1), mPassThroughEnable(false),
      mAttackVelocity(0.0f, 0.0f, 0.0f), mRider(nullptr), mCameraTargetMtx(nullptr), mCameraInfo(nullptr) {
    mBaseMtx.identity();
}

void SlingShooter::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    MR::getJMapInfoArg3NoInit(rIter, &mIsKinopioCameraFocused);

    initModelManagerWithAnm("SpiderThreadAttachPoint", nullptr, false);

    MR::connectToSceneRide(this);

    initHitSensor(3);
    MR::addHitSensorCallbackBinder(this, "bind", 8, 80.0f);
    MR::addHitSensorCallback(this, "npc", 14, 8, 150.0f);
    MR::addHitSensor(this, "attack", 12, 8, 100.0f, TVec3f(0.0f, 0.0f, 0.0f));

    initBinder(50.0f, 50.0f, 8);
    MR::offBind(this);

    initEffectKeeper(2, nullptr, false);
    MR::addEffectHitNormal(this, nullptr);
    MR::addEffect(this, "InvalidHitMark");
    initSound(8, false);

    if (mIsKinopioCameraFocused) {
        MR::useStageSwitchWriteA(this, rIter);
    }

    MR::calcGravity(this);
    MR::initShadowVolumeSphere(this, 100.0f);

    MR::initStarPointerTarget(this, 210.0f, TVec3f(0.0f, 50.0f, 0.0f));

    initNerve(&NrvSlingShooter::SlingShooterNrvFree::sInstance);

    mCameraTargetMtx = new CameraTargetMtx("カメラターゲットダミー");
    mCameraInfo = MR::createActorCameraInfo(rIter);

    MR::declareEventCamera(mCameraInfo, "プレーヤー射出者[ウェイト]");  // player launcher [wait]
    MR::declareEventCamera(mCameraInfo, "プレーヤー射出者[狙い中]");    // player launcher [aim]
    MR::declareEventCamera(mCameraInfo, "プレーヤー射出者[攻撃中]");    // player launcher [attack]
    MR::declareEventCamera(mCameraInfo, "キノピオ射出者[狙い中]");      // kinopio launcher [aim]

    makeActorAppeared();
}

void SlingShooter::initAfterPlacement() {
    MR::startActorBindToSpiderThread(this, &mBasePos, &mNeutralPos, &mUp, mPosition, TVec3f(0.0f, 0.0f, 0.0f));
}

void SlingShooter::calcAnim() {
    if (mIsKinopioAttached && mRider != nullptr) {
        updateActorMtx();
        MR::setBaseTRMtx(mRider, mBaseMtx);
        mRider->mPosition.set(mPosition);
    }
    LiveActor::calcAnim();
}

void SlingShooter::exeFree() {
    mPosition.set(*mBasePos);
    if (MR::isPlayerInBind() && !mIsKinopioCameraFocused) {
        setNerve(&NrvSlingShooter::SlingShooterNrvFreeInvalid::sInstance);
    }
}

void SlingShooter::exeFreeInvalid() {
    exeFree();
    if (MR::isGreaterStep(this, 30) && (mIsKinopioAttached || MR::isOnGroundPlayer())) {
        mIsKinopioAttached = false;
        setNerve(&NrvSlingShooter::SlingShooterNrvFree::sInstance);
    }
}

void SlingShooter::exeLand() {
    if (MR::isFirstStep(this)) {
        if (!mIsKinopioAttached) {
            MR::startBckPlayer("SpiderCocoonLand", 1);
            MR::startEventCamera(mCameraInfo, "プレーヤー射出者[ウェイト]", CameraTargetArg(mCameraTargetMtx), -1);
        } else {
            MR::startBckWithInterpole(mRider, "CocoonLand", 1);
        }
        mVelocity.zero();
    }

    if (mIsKinopioAttached) {
        setNerve(&NrvSlingShooter::SlingShooterNrvWait::sInstance);
        return;
    }

    if (updateWait()) {
        return;
    }

    if (MR::isBckStopped(mRider)) {
        setNerve(&NrvSlingShooter::SlingShooterNrvWait::sInstance);
        return;
    }

    if (MR::isGreaterStep(this, 10)) {
        if (tryCancel()) {
            return;
        }
    }
}

void SlingShooter::exeWait() {
    if (MR::isFirstStep(this)) {
        if (!mIsKinopioAttached) {
            MR::startBckPlayer("SpiderCocoonWait", static_cast< const char* >(nullptr));
        } else {
            MR::startBck(mRider, "SpiderCocoonWait", nullptr);
            if (mIsKinopioTalkEnabled) {
                MR::sendSimpleMsgToActor(ACTMES_NPC_EVENT_TALK_ENABLE, mRider);
            }
        }
    }

    if (!updateWait()) {
        if (tryCancel()) {
            return;
        }
    }
}

void SlingShooter::exeAim() {
    if (MR::isFirstStep(this)) {
        if (!mIsKinopioAttached) {
            MR::startSound(mRider, "SE_PV_CATCH", -1, -1);
        } else {
            MR::sendSimpleMsgToActor(ACTMES_NPC_EVENT_TALK_DISABLE, mRider);
        }

        MR::onBind(this);
        MR::startSound(this, "SE_OJ_SPACE_COCOON_ATTACH", -1, -1);
        if (mPadChannel == WPAD_CHAN1) {
            MR::startCSSound2P("CS_DPD_HIT", nullptr);
        }
    }

    f32 dist = PSVECDistance(&mPosition, mNeutralPos);
    if (dist >= 100.0f) {
        MR::startLevelSound(this, "SE_OJ_LV_SPACE_COCOON_DRAG", ((dist - 100.0f) / 200.0f) * 100.0f, -1, -1);
    }

    if (tryRelease()) {
        return;
    }

    if (!MR::testCorePadButtonA(mPadChannel)) {
        endCommandStream();

        if (!mIsKinopioAttached) {
            MR::endEventCamera(mCameraInfo, "プレーヤー射出者[狙い中]", true, -1);
            MR::startEventCamera(mCameraInfo, "プレーヤー射出者[ウェイト]", CameraTargetArg(mCameraTargetMtx), -1);
        } else if (mIsKinopioCameraFocused) {
            MR::endEventCamera(mCameraInfo, "キノピオ射出者[狙い中]", true, -1);
        }

        MR::offBind(this);
        MR::sendMsgToSpiderThread(ACTMES_SLING_SHOOT_ACTOR_HANG_END, getSensor("bind"));
        setNerve(&NrvSlingShooter::SlingShooterNrvWait::sInstance);
        return;
    }

    updateHang();
}

void SlingShooter::exeAttack() {
    mVelocity.mult(1.0f);

    if (tryAttackMap() || MR::isStep(this, 120)) {
        if (!mIsKinopioAttached) {
            MR::endEventCamera(mCameraInfo, "プレーヤー射出者[攻撃中]", true, -1);
            endBindAndPlayerNoJump(this);
            endBind();
        } else {
            endBind();
        }
    }
}

void SlingShooter::exeAttackSuccess() {
    if (MR::isFirstStep(this)) {
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
    }

    mVelocity.mult(1.0f);

    if (MR::isStep(this, 1)) {
        if (mIsLastDamage) {
            MR::stopScene(30);
        } else {
            MR::stopScene(15);
        }
        mIsLastDamage = false;
    }

    if (MR::isStep(this, 2)) {
        if (mPassThroughEnable) {
            mVelocity.set(mAttackVelocity);
            mPassThroughEnable = false;
            setNerve(&NrvSlingShooter::SlingShooterNrvAttack::sInstance);
            return;
        }

        if (!mIsKinopioAttached) {
            MR::endEventCamera(mCameraInfo, "プレーヤー射出者[攻撃中]", true, -1);
            MR::startBckPlayer("AirRotation", static_cast< const char* >(nullptr));
            endBindAndPlayerNoJump(this);
            endBind();
        } else {
            endBind();
        }
    }
}

void SlingShooter::updateHitSensor(HitSensor* pSensor) {
    if (MR::isSensorAutoRush(pSensor)) {
        pSensor->mPosition.x = mPosition.x;
        pSensor->mPosition.y = mPosition.y;
        pSensor->mPosition.z = MR::getSpiderThreadPosZ();
    }

    if (pSensor->isType(ATYPE_SPRING_ATTACKER_KINOPIO_BIND)) {
        pSensor->mPosition.x = mPosition.x;
        pSensor->mPosition.y = mPosition.y;
        pSensor->mPosition.z = MR::getSpiderThreadPosZ() + -200.0f;  // specifically written as + -200.0f
    }
}

void SlingShooter::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    // FIXME: TVec3f ordering issue due to inlining issue with MR::emitEffectHitBetweenSensors
    // https://decomp.me/scratch/e2M5n
    if (mIsKinopioCameraFocused && mRider == nullptr && !isNerve(&NrvSlingShooter::SlingShooterNrvFreeInvalid::sInstance) &&
        MR::isSensorNpc(pReceiver) && pReceiver->receiveMessage(ACTMES_NPC_EVENT_START, pSender)) {
        mIsKinopioAttached = true;

        startBind(pReceiver->mHost);
        getSensor("npc")->invalidate();
        mRider->mVelocity.zero();
        return;
    }

    if (mRider != nullptr && !MR::tryGetItem(pSender, pReceiver) &&
        (pSender->isType(ATYPE_SPRING_ATTACKER) || pSender->isType(ATYPE_SPRING_ATTACKER_KINOPIO)) &&
        isNerve(&NrvSlingShooter::SlingShooterNrvAttack::sInstance) && pReceiver->receiveMessage(ACTMES_SLING_SHOOT_ATTACK, pSender)) {
        if (pReceiver->receiveMessage(ACTMES_IS_BROKEN, pSender)) {
            // TODO: issue with MR::emitEffectHitBetweenSensors here?
            TVec3f vecBetween;
            MR::calcPosBetweenSensors(&vecBetween, pSender, pReceiver, 0.0f);
            MR::emitEffectHit(this, vecBetween, nullptr);

            mAttackVelocity.set(mVelocity);
            mVelocity.zero();

            mPassThroughEnable = pReceiver->receiveMessage(ACTMES_SLING_SHOOT_PASS_THROUGH_ENABLE, pSender);
            mIsLastDamage = pReceiver->receiveMessage(ACTMES_SLING_SHOOT_IS_LAST_DAMAGE, pSender);
            setNerve(&NrvSlingShooter::SlingShooterNrvAttackSuccess::sInstance);
            return;
        }

        // TODO: issue with MR::emitEffectHitBetweenSensors here?
        TVec3f vecBetween;
        MR::calcPosBetweenSensors(&vecBetween, pSender, pReceiver, 0.0f);
        MR::emitEffectHit(this, vecBetween, "InvalidHitMark");

        if (!mIsKinopioAttached || !pReceiver->isType(ATYPE_TOMB_SPIDER_COCOON)) {
            MR::startSound(this, "SE_BM_TSPIDER_BOUND", -1, -1);
        }

        if (!mIsKinopioAttached) {
            MR::startBckPlayer("WallHit", static_cast< const char* >(nullptr));
            MR::endEventCamera(mCameraInfo, "プレーヤー射出者[攻撃中]", true, -1);

            TVec3f jumpVec(0.0f, 0.0f, 0.0f);
            MR::endBindAndPlayerJump(this, jumpVec, 0);
            endBind();
        } else {
            endBind();
        }
    }
}

bool SlingShooter::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceReflect(msg)) {
        return true;
    }

    return false;
}

bool SlingShooter::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvSlingShooter::SlingShooterNrvLand::sInstance) || isNerve(&NrvSlingShooter::SlingShooterNrvWait::sInstance) ||
        isNerve(&NrvSlingShooter::SlingShooterNrvAim::sInstance) || isNerve(&NrvSlingShooter::SlingShooterNrvAttack::sInstance)) {
        if (!mIsKinopioAttached) {
            if (isNerve(&NrvSlingShooter::SlingShooterNrvAim::sInstance)) {
                MR::sendMsgToSpiderThread(ACTMES_SLING_SHOOT_ACTOR_HANG_END, getSensor("bind"));
            }

            endCommandStream();
            MR::endEventCamera(mCameraInfo, "プレーヤー射出者[ウェイト]", true, -1);
            MR::endEventCamera(mCameraInfo, "プレーヤー射出者[狙い中]", true, -1);
            MR::endEventCamera(mCameraInfo, "プレーヤー射出者[攻撃中]", true, -1);

            mPosition.z = mPlayerZ;
            MR::setPlayerPos(mPosition);

            MR::emitEffect(this, "Stick");

            TVec3f jumpVec(0.0f, 0.0f, 0.0f);
            MR::endBindAndPlayerDamage(this, jumpVec);
            endBind();
        }
        return true;
    }

    return false;
}

bool SlingShooter::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_AUTORUSH_BEGIN) {
        if (isNerve(&NrvSlingShooter::SlingShooterNrvFreeInvalid::sInstance)) {
            return false;
        }

        if (mIsKinopioAttached) {
            if (MR::touchActorToSpiderThread(*MR::getPlayerPos(), *MR::getPlayerVelocity())) {
                MR::startSound(this, "SE_OJ_SPIDER_THREAD_BOUND_L", -1, -1);
                if (mRider != nullptr) {
                    MR::startSound(mRider, "SE_SV_KINOPIO_POINTING", -1, -1);
                }
            }
            return false;
        }

        MR::touchActorToSpiderThread(*MR::getPlayerPos(), *MR::getPlayerVelocity());
        mIsKinopioAttached = false;
        startBind(pSender->mHost);
        return true;
    }

    if (msg == ACTMES_UPDATE_BASEMTX) {
        updateActorMtx();
        MR::setBaseTRMtx(mRider, mBaseMtx);
        return true;
    }

    // remove the camera area on battle start
    if (msg == ACTMES_TOMB_SPIDER_BATTLE_START && mIsKinopioCameraFocused && MR::isValidSwitchA(this)) {
        mIsKinopioCameraFocused = false;
        MR::onSwitchA(this);
        return true;
    }

    if (msg == ACTMES_TOMB_SPIDER_DEMO_START || msg == ACTMES_RUSH_CANCEL) {
        if (mIsKinopioAttached) {
            if (msg == ACTMES_TOMB_SPIDER_DEMO_START && isNerve(&NrvSlingShooter::SlingShooterNrvAim::sInstance)) {
                endCommandStream();
                MR::offBind(this);
                MR::sendMsgToSpiderThread(ACTMES_SLING_SHOOT_ACTOR_HANG_END, getSensor("bind"));
                setNerve(&NrvSlingShooter::SlingShooterNrvWait::sInstance);
                return true;
            }
            return false;
        }

        if (isNerve(&NrvSlingShooter::SlingShooterNrvLand::sInstance) || isNerve(&NrvSlingShooter::SlingShooterNrvWait::sInstance)) {
            startCancelWait();
            return true;
        }

        if (isNerve(&NrvSlingShooter::SlingShooterNrvAim::sInstance)) {
            MR::offBind(this);
            startCancelAim();
            MR::startBckPlayer("Wait", static_cast< const char* >(nullptr));

            endBindAndPlayerNoJump(this);
            endBind();
            return true;
        }

        if (msg == ACTMES_RUSH_CANCEL && mRider != nullptr &&
            (isNerve(&NrvSlingShooter::SlingShooterNrvAttack::sInstance) || isNerve(&NrvSlingShooter::SlingShooterNrvAttackSuccess::sInstance))) {
            MR::offBind(this);
            endBind();
            return true;
        }

        return true;
    }

    return false;
}

void SlingShooter::calcAndSetBaseMtx() {
    if (isNerve(&NrvSlingShooter::SlingShooterNrvLand::sInstance) || isNerve(&NrvSlingShooter::SlingShooterNrvWait::sInstance) ||
        isNerve(&NrvSlingShooter::SlingShooterNrvAim::sInstance)) {
        MR::setBaseTRMtx(this, mBaseMtx);
        return;
    }

    if (mUp == nullptr || mBasePos == nullptr) {
        return;
    }

    TPos3f mtx;
    calcBaseMtx(&mtx);
    MR::setBaseTRMtx(this, mtx);
}

void SlingShooter::calcBaseMtx(TPos3f* pBaseMtx) {
    TVec3f up(*mUp);
    up.scale(-1.0f);
    MR::normalize(&up);
    TVec3f front(0.0f, 0.0f, -1.0f);
    TVec3f side;
    PSVECCrossProduct(&up, &front, &side);
    MR::normalize(&side);
    PSVECCrossProduct(&side, &up, &front);
    MR::normalize(&front);
    pBaseMtx->setXYZDir(side, up, front);
    pBaseMtx->setTrans(*mBasePos);
}

bool SlingShooter::updateWait() {
    mPosition.set(*mBasePos);

    if (MR::isStarPointerPointing(this, 0, true, "弱")) {
        MR::requestStarPointerModeBlueStarReady(this);
    }

    bool set;
    if (MR::isStarPointerPointing(this, 0, true, "弱") && MR::testCorePadTriggerA(WPAD_CHAN0) &&
        MR::tryStartStarPointerCommandStream(this, &mPosition, 0, false)) {
        set = true;
    } else {
        set = false;
    }

    if (set) {
        mPadChannel = WPAD_CHAN0;
    }

    if (mPadChannel >= 0) {
        if (!MR::sendMsgToSpiderThread(ACTMES_SLING_SHOOT_ACTOR_HANG_START, getSensor("bind"))) {
            endCommandStream();
            return false;
        }

        MR::calcStarPointerPosOnPlane(&mPointerPos, *mNeutralPos, TVec3f(0.0f, 0.0f, 1.0f), mPadChannel, false);

        if (!mIsKinopioAttached) {
            MR::endEventCamera(mCameraInfo, "プレーヤー射出者[ウェイト]", true, -1);
            MR::startEventCamera(mCameraInfo, "プレーヤー射出者[狙い中]", CameraTargetArg(mCameraTargetMtx), -1);
        } else if (mIsKinopioCameraFocused) {
            MR::startEventCameraNoTarget(mCameraInfo, "キノピオ射出者[狙い中]", -1);
        }
        setNerve(&NrvSlingShooter::SlingShooterNrvAim::sInstance);
        return true;
    }

    return false;
}

void SlingShooter::updateHang() {
    f32 dist = PSVECDistance(&mPosition, mNeutralPos);

    if (MR::isStarPointerInScreen(mPadChannel)) {
        MR::calcStarPointerPosOnPlane(&mPointerPos, *mNeutralPos, TVec3f(0.0f, 0.0f, 1.0f), mPadChannel, false);
    }

    TVec3f v1(mPointerPos);

    if (PSVECDistance(&v1, mNeutralPos) > 300.0f) {
        v1.set(mPointerPos);
        v1.sub(*mNeutralPos);
        MR::normalize(&v1);
        v1.scale(300.0f);
        v1.add(*mNeutralPos);
    }

    TVec3f pos(v1.scaleInline(0.1f).addOperatorInLine(mPosition.scaleInline(0.9f)));

    mVelocity.setPS2(pos.subOperatorInLine(mPosition));

    if (mRider == nullptr) {
        return;
    }

    if (dist < 200.0f) {
        if (mIsKinopioAttached) {
            if (!MR::isBckPlaying(mRider, "CocoonStretch")) {
                MR::startBckWithInterpole(mRider, "CocoonStretch", 10);
            }
        } else {
            if (!MR::isBckPlaying(mRider, "SpiderCocoonWait")) {
                MR::startBckPlayer("SpiderCocoonWait", 10);
            }
        }
    }

    if (dist > 300.0f) {
        if (mIsKinopioAttached) {
            if (!MR::isBckPlaying(mRider, "CocoonStretch")) {
                MR::startBckWithInterpole(mRider, "CocoonStretch", 10);
            }
        } else {
            if (!MR::isBckPlaying(mRider, "SpiderCocoonStretch")) {
                MR::startBckPlayer("SpiderCocoonStretch", 10);
            }
        }
    }
}

void SlingShooter::updateActorMtx() {
    if (isNerve(&NrvSlingShooter::SlingShooterNrvAttack::sInstance) || isNerve(&NrvSlingShooter::SlingShooterNrvAttackSuccess::sInstance)) {
        mBaseMtx.setTrans(mPosition);
        return;
    }

    TVec3f up;
    if (isNerve(&NrvSlingShooter::SlingShooterNrvLand::sInstance) || isNerve(&NrvSlingShooter::SlingShooterNrvWait::sInstance)) {
        up.set(*mUp);
        up.scale(-1.0f);
    } else {
        up.set(*mNeutralPos);
        up.sub(mPosition);
    }

    if (!MR::isNearZero(up, 0.001f)) {
        MR::normalize(&up);
        TVec3f front;
        mBaseMtx.getZDir(front);
        TVec3f side;
        PSVECCrossProduct(&up, &front, &side);
        MR::normalize(&side);
        PSVECCrossProduct(&front, &side, &up);
        MR::normalize(&up);
        mBaseMtx.setXYZDir(side, up, front);
    }
    mBaseMtx.setTrans(mPosition);
}

bool SlingShooter::tryCancel() {
    if (!mIsKinopioAttached && MR::isPadSwing(WPAD_CHAN0) && MR::isGreaterStep(this, 10)) {
        MR::endEventCamera(mCameraInfo, "プレーヤー射出者[ウェイト]", true, -1);
        MR::startBckPlayer("Spin2nd", static_cast< const char* >(nullptr));
        MR::emitEffect(this, "Stick");
        MR::startSound(mRider, "SE_PV_JUMP_S", -1, -1);
        MR::startSound(mRider, "SE_PM_JUMP_M", -1, -1);
        startCancelWait();
        return true;
    }

    return false;
}

bool SlingShooter::tryRelease() {
    if (MR::testCorePadButtonA(mPadChannel)) {
        return false;
    }

    if (PSVECDistance(&mPosition, mNeutralPos) < 100.0f) {
        MR::sendMsgToSpiderThread(ACTMES_SLING_SHOOT_ACTOR_HANG_END, getSensor("bind"));
        endCommandStream();

        if (mIsKinopioAttached) {
            MR::endEventCamera(mCameraInfo, "キノピオ射出者[狙い中]", true, -1);
        } else {
            MR::endEventCamera(mCameraInfo, "プレーヤー射出者[狙い中]", true, -1);
            MR::startEventCamera(mCameraInfo, "プレーヤー射出者[ウェイト]", CameraTargetArg(mCameraTargetMtx), -1);
        }
        setNerve(&NrvSlingShooter::SlingShooterNrvWait::sInstance);
        return true;
    }

    startCancelAim();

    mIsKinopioTalkEnabled = false;

    mVelocity.set(*mNeutralPos);
    mVelocity.sub(mPosition);
    mVelocity.z = 0.0f;

    if (MR::isNearZero(mVelocity, 0.001f)) {
        mVelocity.set(*mUp);
    }

    MR::normalize(&mVelocity);

    TVec3f up(mVelocity);
    mVelocity.mult(45.0f);
    up.scale(-1.0f);

    TVec3f front;
    mBaseMtx.getZDir(front);
    TVec3f side;
    PSVECCrossProduct(&up, &front, &side);
    MR::normalize(&side);
    mBaseMtx.setXYZDir(side, up, front);

    MR::emitEffect(mRider, "SpaceCocoonBlur");
    MR::emitEffect(this, "Stick");
    MR::startSound(this, "SE_OJ_SPACE_COCOON_LAUNCH", -1, -1);

    if (!mIsKinopioAttached) {
        MR::startBckPlayer("CocoonFly", static_cast< const char* >(nullptr));
        MR::endEventCamera(mCameraInfo, "プレーヤー射出者[狙い中]", true, -1);
        MR::startEventCameraNoTarget(mCameraInfo, "プレーヤー射出者[攻撃中]", -1);
        MR::startSound(mRider, "SE_PV_JUMP_JOY", -1, -1);
        MR::validatePlayerSensor();
    } else {
        MR::startSound(mRider, "SE_SV_KINOPIO_TALK_GLAD_FLY", -1, -1);
        if (mIsKinopioCameraFocused) {
            MR::endEventCamera(mCameraInfo, "キノピオ射出者[狙い中]", true, -1);
        }
    }

    if (mIsKinopioCameraFocused) {
        if (MR::isValidSwitchA(this)) {
            MR::onSwitchA(this);
        }
        mIsKinopioCameraFocused = false;
    }

    setNerve(&NrvSlingShooter::SlingShooterNrvAttack::sInstance);
    return true;
}

bool SlingShooter::tryAttackMap() {
    if (!MR::isBinded(this)) {
        return false;
    }

    if (MR::isLessStep(this, 10)) {
        return false;
    }

    bool attackSuccess = true;
    bool b2 = false;
    for (s32 plane = 0; plane < MR::getBindedPlaneNum(this); plane++) {
        if (!(mVelocity.dot(*MR::getBindedPlaneNormal(this, plane)) > 0.0f)) {
            HitSensor* planeSensor = MR::getBindedPlaneSensor(this, plane);
            MR::shakeCameraNormal();
            if (!MR::sendMsgEnemyAttack(planeSensor, getSensor("attack"))) {
                if (!planeSensor->receiveMessage(ACTMES_IS_BROKEN, getSensor("attack"))) {
                    if (mIsKinopioAttached) {
                        MR::startSound(mRider, "SE_SV_KINOPIO_TRAMPLED", -1, -1);
                    } else {
                        MR::startSound(mRider, "SE_PV_DAMAGE_S", -1, -1);
                    }
                    MR::startSound(mRider, "SE_PM_LAND_HEAVY", -1, -1);
                    attackSuccess = false;
                    continue;
                }
            }
            b2 = true;
            MR::emitEffectHit(this, mPosition, nullptr);
        }
    }

    if (attackSuccess) {
        if (b2) {
            setNerve(&NrvSlingShooter::SlingShooterNrvAttackSuccess::sInstance);
        }
        return false;
    }

    return true;
}

void SlingShooter::startBind(LiveActor* pRider) {
    mRider = pRider;
    mPlayerZ = pRider->mPosition.z;

    MR::invalidateClipping(this);

    if (!mIsKinopioAttached) {
        TPos3f mtx;
        mtx.identity();
        calcBaseMtx(&mtx);
        mCameraTargetMtx->mMatrix.setInline(mtx);
        mCameraTargetMtx->mMatrix.setTrans(*mNeutralPos);
        getSensor("attack")->setType(ATYPE_SPRING_ATTACKER);
        MR::startSound(mRider, "SE_PV_CATCH", -1, -1);
    } else {
        getSensor("attack")->setType(ATYPE_SPRING_ATTACKER_KINOPIO);
    }

    if (!mIsKinopioCameraFocused) {
        MR::startSound(this, "SE_OJ_SPACE_COCOON_ATTACH", -1, -1);
    }

    updateActorMtx();
    setNerve(&NrvSlingShooter::SlingShooterNrvLand::sInstance);
}

void SlingShooter::startCancelWait() {
    mPosition.z = mPlayerZ;
    f32 y = MR::getPlayerPos()->y;
    f32 x = MR::getPlayerPos()->x;
    MR::setPlayerPos(TVec3f(x, y, mPosition.z));

    endBindAndPlayerNoJump(this);
    endBind();
    endCommandStream();
}

void SlingShooter::startCancelAim() {
    mPosition.z = MR::getSpiderThreadPosZ();

    f32 y = MR::getPlayerPos()->y;
    f32 x = MR::getPlayerPos()->x;
    MR::setPlayerPos(TVec3f(x, y, mPosition.z));

    MR::sendMsgToSpiderThread(ACTMES_SLING_SHOOT_ACTOR_HANG_END, getSensor("bind"));
    endCommandStream();
}

void SlingShooter::endBind() {
    if (mIsKinopioAttached) {
        LiveActor* rider = mRider;
        rider->mPosition.z = MR::getSpiderThreadPosZ() - 200.0f;
        mRider->mVelocity.zero();
        mRider->receiveMessage(ACTMES_NPC_EVENT_END, MR::getMessageSensor(), MR::getMessageSensor());
    }

    MR::deleteEffect(mRider, "SpaceCocoonBlur");
    mVelocity.zero();
    mRider = nullptr;
    MR::offBind(this);
    mPosition.set(*mBasePos);
    MR::validateClipping(this);
    setNerve(&NrvSlingShooter::SlingShooterNrvFreeInvalid::sInstance);
}

void SlingShooter::endCommandStream() {
    if (mPadChannel >= 0 && MR::isStarPointerCommandStream(this, mPadChannel)) {
        MR::tryEndStarPointerCommandStream(this, mPadChannel);
        mPadChannel = -1;
    }
}
