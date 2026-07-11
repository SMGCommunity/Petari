#include "Game/Boss/KoopaFunction.hpp"
#include "Game/Boss/BossAccessor.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaBattleStairsBase.hpp"
#include "Game/Boss/KoopaBattleStairsVs1.hpp"
#include "Game/Boss/KoopaParts.hpp"
#include "Game/Boss/KoopaPowerUpSwitch.hpp"
#include "Game/Boss/KoopaSequencerVs1.hpp"
#include "Game/Boss/KoopaSequencerVs2.hpp"
#include "Game/Boss/KoopaSequencerVs3.hpp"
#include "Game/Boss/KoopaSwitchKeeper.hpp"
#include "Game/Boss/KoopaViewSwitchKeeper.hpp"
#include "Game/LiveActor/ActorJointCtrl.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/Map/KoopaBattleMapPlanet.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static const s32 sKoopaHitBlowStopSceneStep = 10;
};  // namespace

NameObj* KoopaFunction::createKoopaVs1(const char* pName) {
    KoopaSequencerVs1* pSequencer = new KoopaSequencerVs1();
    return new Koopa(pName, pSequencer);
}

NameObj* KoopaFunction::createKoopaVs2(const char* pName) {
    KoopaSequencerVs2* pSequencer = new KoopaSequencerVs2();
    return new Koopa(pName, pSequencer);
}

NameObj* KoopaFunction::createKoopaVs3(const char* pName) {
    KoopaSequencerVs3* pSequencer = new KoopaSequencerVs3();
    return new Koopa(pName, pSequencer);
}

void KoopaFunction::initKoopaPartsVs1(Koopa* pKoopa) {
    pKoopa->mParts->initVs1();
}

void KoopaFunction::initKoopaPartsVs2(Koopa* pKoopa) {
    pKoopa->mParts->initVs2();
}

void KoopaFunction::initKoopaVs3(Koopa* pKoopa) {
    pKoopa->mParts->initVs3();
}

void KoopaFunction::createKoopaRock(Koopa* pKoopa) {
    pKoopa->mParts->createRock();
}

void KoopaFunction::createKoopaRollBall(Koopa* pKoopa) {
    pKoopa->mParts->createRollBall();
}

s32 KoopaFunction::registerBattleMapStair(KoopaBattleMapStair* pMapStair) {
    KoopaBattleStairsVs1* pBattleStairsBase = static_cast< KoopaBattleStairsVs1* >(BossAccess::getBossAccessorKoopa()->mSequencer->mBattleStairs);
    return pBattleStairsBase->registerStair(pMapStair);
}

void KoopaFunction::setKoopaPos(Koopa* pKoopa, const char* pDemoName) {
    TPos3f transform;
    MR::findNamePos(pDemoName, transform);

    TVec3f yDir;
    transform.getYDirInline(yDir);

    pKoopa->mGravity.set(yDir.negateInline());
    transform.getZDirInline(pKoopa->mFront);
    transform.getTransInline(pKoopa->mPosition);

    bool isCalcGravity = MR::isCalcGravity(pKoopa);
    MR::offCalcGravity(pKoopa);
    MR::resetPosition(pKoopa);

    if (isCalcGravity) {
        MR::onCalcGravity(pKoopa);
    }
}

TVec3f& KoopaFunction::getPlanetCenterPos(const Koopa* pKoopa) {
    return pKoopa->mParts->getPlanetPos();
}

f32 KoopaFunction::getPlanetRadius(const Koopa* pKoopa) {
    return pKoopa->mParts->getPlanetRadius();
}

const TVec3f& KoopaFunction::getKoopaFront(const Koopa* pKoopa) {
    return pKoopa->mFront;
}

TVec3f* KoopaFunction::getKoopaFrontPtr(Koopa* pKoopa) {
    return &pKoopa->mFront;
}

bool KoopaFunction::isKoopaSightPlayer(const Koopa* pKoopa, const MR::ActorSightParam& rSightParam) {
    return MR::isInSightFanPlayer(pKoopa, pKoopa->mFront, rSightParam._0, rSightParam._4, rSightParam._8);
}

bool KoopaFunction::escapeKoopaFromPlayer(Koopa* pKoopa, const MR::ActorMoveParam& rMoveParam) {
    MR::escapeFromPlayer(pKoopa, &pKoopa->mFront, rMoveParam._0, rMoveParam._4, rMoveParam._8, rMoveParam._C);
    return MR::sendMsgEnemyAttackToBindedSensor(pKoopa, getKoopaMessageSensor(pKoopa));
}

bool KoopaFunction::isKoopaVs1(const Koopa* pKoopa) {
    return pKoopa->mSequencer->mVs == 1;
}

bool KoopaFunction::isKoopaVs2(const Koopa* pKoopa) {
    return pKoopa->mSequencer->mVs == 2;
}

bool KoopaFunction::isKoopaVs3(const Koopa* pKoopa) {
    return pKoopa->mSequencer->mVs == 3;
}

bool KoopaFunction::isKoopaLv1(const Koopa* pKoopa) {
    return pKoopa->mSequencer->mLv == 1;
}

bool KoopaFunction::isKoopaLv2(const Koopa* pKoopa) {
    return pKoopa->mSequencer->mLv == 2;
}

bool KoopaFunction::isKoopaLv3(const Koopa* pKoopa) {
    return pKoopa->mSequencer->mLv == 3;
}

bool KoopaFunction::isKoopaAngry(const Koopa* pKoopa) {
    return pKoopa->mSequencer->mIsAngry;
}

void KoopaFunction::startKoopaAngry(Koopa* pKoopa) {
    pKoopa->mSequencer->mIsAngry = true;
}

void KoopaFunction::startFaceCtrl(Koopa* pKoopa) {
    pKoopa->mJointCtrl->startFaceCtrl(-1);
}

void KoopaFunction::endFaceCtrl(Koopa* pKoopa, s32 val) {
    pKoopa->mJointCtrl->endFaceCtrl(val);
}

void KoopaFunction::endFaceCtrlDirect(Koopa* pKoopa) {
    pKoopa->mJointCtrl->endFaceCtrl(0);
}

void KoopaFunction::startRecoverKoopaArmor(Koopa* pKoopa) {
    pKoopa->mParts->mArmorBreak->kill();
    pKoopa->mParts->mThornBreak->kill();
}

void KoopaFunction::startBreakKoopaTailThorn(Koopa* pKoopa) {
    pKoopa->mParts->mThornBig->kill();
    pKoopa->mParts->mThornSmall->kill();
}

void KoopaFunction::startRecoverKoopaTailThorn(Koopa* pKoopa) {
    pKoopa->mParts->mThornBig->appear();
    pKoopa->mParts->mThornSmall->appear();
}

PartsModel* KoopaFunction::getKoopaRock(Koopa* pKoopa) {
    return pKoopa->mParts->mRock;
}

KoopaRockBreak* KoopaFunction::getKoopaRockBreak(Koopa* pKoopa) {
    return pKoopa->mParts->mRockBreak;
}

PartsModel* KoopaFunction::getKoopaRollBall(Koopa* pKoopa) {
    return pKoopa->mParts->mRollBall;
}

void KoopaFunction::emitKoopaFireShortSlow(Koopa* pKoopa) {
    pKoopa->mParts->emitFireShort(false, false);
}

void KoopaFunction::emitKoopaFireShortFast(Koopa* pKoopa) {
    pKoopa->mParts->emitFireShort(true, false);
}

void KoopaFunction::emitKoopaFireShortCurve(Koopa* pKoopa) {
    pKoopa->mParts->emitFireShort(false, true);
}

void KoopaFunction::emitKoopaFireLongTime(Koopa* pKoopa) {
    pKoopa->mParts->emitFireLongTime();
}

KoopaFireStairs* KoopaFunction::emitFireStairsToTarget(Koopa* pKoopa, const KoopaBattleMapStair* pMapStair, const TVec3f& rPosition, bool useFront) {
    return pKoopa->mParts->emitFireStairsToPos(pMapStair, rPosition, useFront);
}

void KoopaFunction::killKoopaFireStairsAll(Koopa* pKoopa) {
    pKoopa->mParts->killFireStairsAll();
}

void KoopaFunction::emitKoopaShockWave(Koopa* pKoopa) {
    pKoopa->mParts->emitShockWave();
}

KoopaBattleMapPlanet* KoopaFunction::getKoopaPlanet(Koopa* pKoopa) {
    return pKoopa->mParts->mPlanetLv1;
}

KoopaBattleMapPlanet* KoopaFunction::getKoopaPlanetLv2(Koopa* pKoopa) {
    return pKoopa->mParts->mPlanetLv2;
}

KoopaBattleMapPlanet* KoopaFunction::getKoopaPlanetLv3(Koopa* pKoopa) {
    return pKoopa->mParts->mPlanetLv3;
}

void KoopaFunction::appearKoopaHoleSunPlanetInside(Koopa* pKoopa) {
    pKoopa->mParts->appearHoleSunPlanetInside();
}

void KoopaFunction::appearKoopaHoleSunPlanetOutside(Koopa* pKoopa) {
    pKoopa->mParts->appearHoleSunPlanetOutside();
}

void KoopaFunction::killKoopaHoleSunPlanetOutside(Koopa* pKoopa) {
    pKoopa->mParts->killHoleSunPlanetOutside();
}

KoopaPlanetShadow* KoopaFunction::getKoopaPlanetShadow(Koopa* pKoopa) {
    return pKoopa->mParts->mPlanetShadow;
}

bool KoopaFunction::tryKoopaPushPlayer(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorType(pSender, ATYPE_KOOPA_PUSH) && MR::isSensorPlayer(pReceiver) && MR::sendMsgPush(pReceiver, pSender)) {
        return true;
    }

    return false;
}

bool KoopaFunction::tryKoopaBodyAttackPlayer(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorEnemyAttack(pSender) && MR::isSensorPlayer(pReceiver) && MR::sendMsgEnemyAttack(pReceiver, pSender)) {
        return true;
    }

    return false;
}

bool KoopaFunction::tryKoopaBodyAttackPlayerMaximum(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorEnemyAttack(pSender) && MR::isSensorPlayer(pReceiver) && MR::sendMsgEnemyAttackMaximum(pReceiver, pSender)) {
        return true;
    }

    return false;
}

bool KoopaFunction::tryKoopaAttackPlayerMaximum(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorType(pSender, ATYPE_KOOPA_ATTACK) && MR::isSensorPlayer(pReceiver) && MR::sendMsgEnemyAttackMaximum(pReceiver, pSender)) {
        return true;
    }

    return false;
}

bool KoopaFunction::tryKoopaAttackMapObj(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        return false;
    }

    if (MR::sendMsgToEnemyAttackTrample(pReceiver, pSender)) {
        return true;
    }

    if (MR::sendMsgEnemyAttack(pReceiver, pSender)) {
        return true;
    }

    if (MR::sendMsgPush(pReceiver, pSender)) {
        return true;
    }

    return false;
}

bool KoopaFunction::tryKoopaReflectStarPiece(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceReflect(msg) == false) {
        return false;
    }

    return pReceiver->isType(ATYPE_KOOPA_PUSH);
}

HitSensor* KoopaFunction::getKoopaMessageSensor(Koopa* pKoopa) {
    return pKoopa->getSensor("Body");
}

void KoopaFunction::registerKoopaSwitchKeeper(LiveActor* pActor) {
    BossAccess::getBossAccessorKoopa()->mParts->mKoopaSwitchKeeper = static_cast< KoopaSwitchKeeper* >(pActor);
}

KoopaSwitchKeeper* KoopaFunction::getKoopaSwitchKeeper(Koopa* pKoopa) {
    return pKoopa->mParts->mKoopaSwitchKeeper;
}

void KoopaFunction::registerKoopaViewSwitchKeeper(LiveActor* pActor) {
    BossAccess::getBossAccessorKoopa()->mParts->mKoopaViewSwitchKeeper = static_cast< KoopaViewSwitchKeeper* >(pActor);
}

KoopaViewSwitchKeeper* KoopaFunction::getKoopaViewSwitchKeeper(Koopa* pKoopa) {
    return pKoopa->mParts->mKoopaViewSwitchKeeper;
}

void KoopaFunction::registerKoopaPowerUpSwitch(LiveActor* pActor) {
    BossAccess::getBossAccessorKoopa()->mParts->mKoopaPowerUpSwitch = static_cast< KoopaPowerUpSwitch* >(pActor);
}

KoopaPowerUpSwitch* KoopaFunction::getKoopaPowerUpSwitch(Koopa* pKoopa) {
    return pKoopa->mParts->mKoopaPowerUpSwitch;
}

namespace {
    bool tryStartCameraDemo(Koopa* pKoopa, const char* pSmth, const char* pName, const char* pDemoName, bool isMario) {
        if (isMario) {
            if (!MR::tryStartDemoMarioPuppetable(pKoopa, pSmth)) {
                return false;
            }
        } else {
            if (!MR::tryStartDemo(pKoopa, pSmth)) {
                return false;
            }
        }

        MR::offCalcGravity(pKoopa);

        if (pDemoName != nullptr) {
            KoopaFunction::setKoopaPos(pKoopa, pDemoName);

            if (isMario) {
                MR::setPlayerPosAndWait(pDemoName);
            }
        }

        MR::startAction(pKoopa, pName);

        if (isMario) {
            MR::startBckPlayer(pName, static_cast< const char* >(nullptr));
        }

        MR::startAnimCameraTargetSelf(pKoopa, pKoopa->mParts->mActorCameraInfo, pName, 0, 1.0f);

        return true;
    }
};  // namespace

bool KoopaFunction::tryStartKoopaAndMarioCameraDemo(Koopa* pKoopa, const char* pSmth, const char* pName, const char* pDemoName) {
    return tryStartCameraDemo(pKoopa, pSmth, pName, pDemoName, true);
}

bool KoopaFunction::tryStartKoopaCameraDemo(Koopa* pKoopa, const char* pSmth, const char* pName, const char* pDemoName) {
    return tryStartCameraDemo(pKoopa, pSmth, pName, pDemoName, false);
}

bool KoopaFunction::tryEndKoopaCameraDemo(Koopa* pKoopa, const char* pName, const char* pSmth) {
    if (!MR::isActionEnd(pKoopa)) {
        return false;
    }

    MR::endAnimCamera(pKoopa, pKoopa->mParts->mActorCameraInfo, pSmth, 0, true);
    MR::endDemo(pKoopa, pName);
    MR::onCalcGravity(pKoopa);

    return true;
}

ModelObjNpc* KoopaFunction::getKoopaDemoPeach(Koopa* pKoopa) {
    return pKoopa->mParts->mPeach;
}

ModelObjNpc* KoopaFunction::getKoopaDemoKoopaJr(Koopa* pKoopa) {
    return pKoopa->mParts->mKoopaJr;
}

ModelObjNpc* KoopaFunction::getKoopaDemoKoopaJrShip(Koopa* pKoopa) {
    return pKoopa->mParts->mKoopaJrShip;
}

ModelObj* KoopaFunction::getKoopaDemoMeteor1(Koopa* pKoopa) {
    return pKoopa->mParts->mMeteor1;
}

ModelObj* KoopaFunction::getKoopaDemoMeteor2(Koopa* pKoopa) {
    return pKoopa->mParts->mMeteor2;
}

ModelObj* KoopaFunction::getKoopaDemoMeteor3(Koopa* pKoopa) {
    return pKoopa->mParts->mMeteor3;
}

void KoopaFunction::endKoopaDemoMeetPeach(Koopa* pKoopa) {
    MR::startAction(pKoopa->mParts->mPeach, "DemoKoopaVs3Wait");
    MR::startAction(pKoopa->mParts->mKoopaJr, "DemoKoopaVs3Wait");
    MR::startAction(pKoopa->mParts->mKoopaJrShip, "DemoKoopaVs3Wait");

    KoopaParts* pParts = pKoopa->mParts;
    pParts->mPeach->mLodCtrl->validate();
    pParts->mKoopaJr->mLodCtrl->validate();
    pParts->mKoopaJrShip->mLodCtrl->validate();

    pParts->mPeach->mJointCtrl->startFaceCtrl(-1);
    pParts->mKoopaJr->mJointCtrl->startFaceCtrl(-1);
}

void KoopaFunction::invalidateKoopaNpcLod(Koopa* pKoopa) {
    KoopaParts* pParts = pKoopa->mParts;
    pParts->mPeach->mLodCtrl->invalidate();
    pParts->mKoopaJr->mLodCtrl->invalidate();
    pParts->mKoopaJrShip->mLodCtrl->invalidate();
}

void KoopaFunction::initKoopaCamera(Koopa* pKoopa, const char* pName) {
    MR::initMultiActorCameraNoInit(pKoopa, pKoopa->mParts->mActorCameraInfo, pName);
}

void KoopaFunction::initKoopaAnimCamera(Koopa* pKoopa, const char* pName) {
    MR::initAnimCamera(pKoopa, pKoopa->mParts->mActorCameraInfo, pName);
}

void KoopaFunction::startKoopaCamera(Koopa* pKoopa, const char* pName) {
    MR::startMultiActorCameraNoTarget(pKoopa, pKoopa->mParts->mActorCameraInfo, pName, -1);
}

void KoopaFunction::startKoopaTargetCamera(Koopa* pKoopa, const char* pName) {
    MR::startMultiActorCameraTargetSelf(pKoopa, pKoopa->mParts->mActorCameraInfo, pName, -1);
}

void KoopaFunction::startKoopaAnimCamera(Koopa* pKoopa, const char* pName, s32 a1) {
    MR::startAnimCameraTargetSelf(pKoopa, pKoopa->mParts->mActorCameraInfo, pName, a1, 1.0f);
}

void KoopaFunction::endKoopaCamera(Koopa* pKoopa, const char* pName, bool a1, s32 a2) {
    MR::endMultiActorCamera(pKoopa, pKoopa->mParts->mActorCameraInfo, pName, a1, a2);
}

void KoopaFunction::endKoopaAnimCamera(Koopa* pKoopa, const char* pName, s32 a1) {
    MR::endAnimCamera(pKoopa, pKoopa->mParts->mActorCameraInfo, pName, a1, true);
}

void KoopaFunction::startKoopaPlateDamageAnimPowerStarAppear(Koopa* pKoopa) {
    if (isKoopaVs3(pKoopa)) {
        pKoopa->mParts->mPlanetLv3->startAnimPowerStarAppear();
    } else {
        pKoopa->mParts->mPlanetLv1->startAnimPowerStarAppear();
    }
}

void KoopaFunction::changeBgmStateNormal(u32 a1) {
    MR::setStageBGMState(1, a1);
}

void KoopaFunction::changeBgmStateEscape() {
    MR::setStageBGMState(2, 90);
}

void KoopaFunction::changeBgmStateDamageTail() {
    MR::setStageBGMState(3, 30);
}

void KoopaFunction::changeBgmStateFlyAway() {
    MR::setStageBGMState(4, 30);
}

void KoopaFunction::tryRestartKoopa() {
    Koopa* pKoopa = BossAccess::getBossAccessorKoopa();
    if (MR::isHiddenModel(pKoopa)) {
        MR::showModel(pKoopa);
        pKoopa->mParts->mPeach->appear();
        pKoopa->mParts->mKoopaJr->appear();
        pKoopa->mParts->mKoopaJrShip->appear();

        MR::startAction(pKoopa->mParts->mPeach, "DemoKoopaVs3Wait");
        MR::startAction(pKoopa->mParts->mKoopaJr, "DemoKoopaVs3Wait");
        MR::startAction(pKoopa->mParts->mKoopaJrShip, "DemoKoopaVs3Wait");

        // This is super ugly but idk how to fix the register issue
        // TODO: The following is an inline for `KoopaParts`.
        pKoopa = (Koopa*)pKoopa->mParts;
        ((KoopaParts*)pKoopa)->mPeach->mJointCtrl->startFaceCtrl(-1);
        ((KoopaParts*)pKoopa)->mKoopaJr->mJointCtrl->startFaceCtrl(-1);
    }
}

bool KoopaFunction::moveAndTurnKoopaToPlayer(Koopa* pKoopa, const MR::ActorMoveParam& rMoveParam) {
    MR::moveAndTurnToPlayer(pKoopa, &pKoopa->mFront, rMoveParam._0, rMoveParam._4, rMoveParam._8, rMoveParam._C);
    return MR::sendMsgEnemyAttackToBindedSensor(pKoopa, getKoopaMessageSensor(pKoopa));
}

void KoopaFunction::startBreakKoopaArmor(Koopa* pKoopa) {
    pKoopa->mParts->mArmorBreak->appear();
    pKoopa->mParts->mThornBreak->appear();

    MR::startAction(pKoopa->mParts->mThornBreak, "Break");
}

bool KoopaFunction::tryKoopaShellAttackPlayer(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensor(pSender, "AttackShell") && MR::isSensorPlayer(pReceiver) && MR::sendMsgEnemyAttack(pReceiver, pSender)) {
        return true;
    }

    return false;
}

void KoopaFunction::startKoopaSpinHitBlow(Koopa* pKoopa) {
    MR::tryRumblePadVeryStrong(pKoopa, 0);
    MR::stopSceneForDefaultHit(::sKoopaHitBlowStopSceneStep);
    MR::startSpinHitSound(pKoopa);
    MR::startBlowHitSound(pKoopa);

    MR::startSound(pKoopa, "SE_BM_KOOPA_DAMAGE_S");
    MR::startSound(pKoopa, "SE_BV_KOOPA_TAIL_RUN_START");
    MR::startSystemSE("SE_SY_VS_BOSS_DAMAGE_1");
}
