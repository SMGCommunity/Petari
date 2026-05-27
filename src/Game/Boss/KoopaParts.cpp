#include "Game/Boss/KoopaParts.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaFireShort.hpp"
#include "Game/Boss/KoopaFireStairs.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/Boss/KoopaPlanetShadow.hpp"
#include "Game/Boss/KoopaRockBreak.hpp"
#include "Game/Boss/KoopaShockWave.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/Map/KoopaBattleMapPlanet.hpp"

KoopaParts::KoopaParts(Koopa* pKoopa, const JMapInfoIter& rIter)
    : mKoopa(pKoopa), mPlanetRadius(1300.0f), mThornBig(), mThornSmall(), mArmorBreak(), mThornBreak(), mPlanetLv1(), mPlanetShadow(), mShockWave(), mFireShort(), mFireStairs(), mKoopaSwitchKeeper(),
      mKoopaViewSwitchKeeper(), mKoopaPowerUpSwitch(), mRock(), mRockBreak(), mRollBall(), mPlanetLv2(), mPlanetLv3(), mHoleSunPlanetOutside(), mHoleSunPlanetOutsideBloom(),
      mHoleSunPlanetInside(), mHoleSunPlanetInsideBloom(), mPeach(), mKoopaJr(), mKoopaJrShip(), mMeteor1(), mMeteor2(), mMeteor3(), mActorCameraInfo() {
    mActorCameraInfo = new ActorCameraInfo(rIter);
    MR::declareCameraRegisterVec(mKoopa, 0, &mKoopa->mPosition);
}

namespace {
    PartsModel* createKoopaBodyParts(LiveActor* pActor, const char* pName, const char* pModelName, const char* pJointName) {
        PartsModel* pPartsModel = new PartsModel(pActor, pName, pModelName, nullptr, 18, false);

        pPartsModel->loadFixedPosition(pJointName);
        pPartsModel->initWithoutIter();

        MR::initLightCtrl(pPartsModel);
        MR::registerDemoSimpleCastAll(pPartsModel);

        return pPartsModel;
    };
}  // namespace

KoopaFireStairs* KoopaParts::emitFireStairsToPos(const KoopaBattleMapStair* pMapStair, const TVec3f& rPosition, bool useFront) {
    KoopaFireStairs* pMapStairs = static_cast< KoopaFireStairs* >(mFireStairs->getDeadActor());

    if (pMapStairs == nullptr) {
        return nullptr;
    }

    pMapStairs->mPosition.set(rPosition);

    useFront ? pMapStairs->setInfo(pMapStair, KoopaFunction::getKoopaFrontPtr(mKoopa)) : pMapStairs->setInfo(pMapStair, nullptr);

    pMapStairs->appear();

    return pMapStairs;
}

void KoopaParts::killFireStairsAll() {
    for (int idx = 0; idx < 16; idx++) {
        LiveActor* pActor = mFireStairs->getActor(idx);

        if (!MR::isDead(pActor)) {
            pActor->makeActorDead();
        }
    }
}

void KoopaParts::emitFireShort(bool isFast, bool isCurve) {
    KoopaFireShort* pFireShort = static_cast< KoopaFireShort* >(mFireShort->getDeadActor());

    if (pFireShort != nullptr) {
        if (isCurve) {
            pFireShort->emitCurve();
        } else if (isFast) {
            pFireShort->emitFast();
        } else {
            pFireShort->emitNormal();
        }
    }
}

void KoopaParts::emitFireLongTime() {
    KoopaFireShort* pFireShort = static_cast< KoopaFireShort* >(mFireShort->getDeadActor());

    if (pFireShort != nullptr) {
        pFireShort->emitLongTime();
    }
}

void KoopaParts::emitShockWave() {
    LiveActor* pActor = mShockWave->getDeadActor();

    if (pActor != nullptr) {
        pActor->appear();
    }
}

TVec3f& KoopaParts::getPlanetPos() const {
    if (KoopaFunction::isKoopaVs1(mKoopa) || KoopaFunction::isKoopaVs2(mKoopa)) {
        return mPlanetLv1->mPosition;
    }

    if (KoopaFunction::isKoopaLv1(mKoopa)) {
        return mPlanetLv1->mPosition;
    }

    if (KoopaFunction::isKoopaLv2(mKoopa)) {
        return mPlanetLv2->mPosition;
    }

    return mPlanetLv3->mPosition;
}

f32 KoopaParts::getPlanetRadius() const {
    return mPlanetRadius;
}

void KoopaParts::appearHoleSunPlanetInside() {
    mHoleSunPlanetInside->appear();
}

void KoopaParts::appearHoleSunPlanetOutside() {
    mHoleSunPlanetOutside->appear();
}

void KoopaParts::killHoleSunPlanetOutside() {
    mHoleSunPlanetOutside->kill();
}

void KoopaParts::createPlanetShadow() {
    mPlanetShadow = new KoopaPlanetShadow(mKoopa);
    mPlanetShadow->initWithoutIter();
    mPlanetShadow->kill();
}

void KoopaParts::initVs1() {
    createFireStairs(false);

    const char* pResetPositionName = "惑星中心";
    const char* pName2 = "KoopaBattleMapPlanet";
    const char* pName1 = "クッパ惑星";
    KoopaBattleMapPlanet* pBattleMapPlanet = new KoopaBattleMapPlanet(pName1, pName2, false, false, false);
    MR::resetPosition(pBattleMapPlanet, pResetPositionName);
    pBattleMapPlanet->initWithoutIter();
    mPlanetLv1 = pBattleMapPlanet;

    createPlanetShadow();
    createCommonParts();
}

void KoopaParts::initVs2() {
    const char* pResetPositionName = "惑星中心";
    const char* pName2 = "KoopaBattleMapPlanetVs2";
    const char* pName1 = "クッパ惑星";
    KoopaBattleMapPlanet* pBattleMapPlanet = new KoopaBattleMapPlanet(pName1, pName2, true, false, false);
    MR::resetPosition(pBattleMapPlanet, pResetPositionName);
    pBattleMapPlanet->initWithoutIter();
    mPlanetLv1 = pBattleMapPlanet;

    createCommonParts();
}

namespace {
    ModelObjNpc* createDemoNpc(const char* pName, const char* a2) {
        ModelObjNpc* pModelObjNpc = new ModelObjNpc(pName, a2, nullptr);
        pModelObjNpc->initWithoutIter();
        MR::resetPosition(pModelObjNpc, "デモ中心");

        pModelObjNpc->mLodCtrl->invalidateClipping();
        pModelObjNpc->mLodCtrl->invalidate();

        pModelObjNpc->mJointCtrl->endFaceCtrl(-1);

        MR::initLightCtrl(pModelObjNpc);

        pModelObjNpc->kill();

        return pModelObjNpc;
    };
}  // namespace

void KoopaParts::initVs3() {
    createFireStairs(true);

    const char* pResetPositionName = "惑星中心";
    const char* pName2 = "KoopaBattleMapPlanetVs3Lv1";
    const char* pName1 = "クッパ惑星Ｌｖ１";
    KoopaBattleMapPlanet* pBattleMapPlanet = new KoopaBattleMapPlanet(pName1, pName2, false, false, true);
    MR::resetPosition(pBattleMapPlanet, pResetPositionName);
    pBattleMapPlanet->initWithoutIter();
    mPlanetLv1 = pBattleMapPlanet;

    pResetPositionName = "惑星Ｌｖ２";
    pName2 = "KoopaBattleMapPlanetVs3Lv2";
    pName1 = "クッパ惑星Ｌｖ２";
    pBattleMapPlanet = new KoopaBattleMapPlanet(pName1, pName2, false, false, true);
    MR::resetPosition(pBattleMapPlanet, pResetPositionName);
    pBattleMapPlanet->initWithoutIter();
    mPlanetLv2 = pBattleMapPlanet;

    pResetPositionName = "惑星Ｌｖ３";
    pName2 = "KoopaBattleMapPlanetVs3Lv3";
    pName1 = "クッパ惑星Ｌｖ３";
    pBattleMapPlanet = new KoopaBattleMapPlanet(pName1, pName2, false, true, false);
    MR::resetPosition(pBattleMapPlanet, pResetPositionName);
    pBattleMapPlanet->initWithoutIter();
    mPlanetLv3 = pBattleMapPlanet;

    MR::startBrk(mPlanetLv1, "Wait");
    MR::startBrk(mPlanetLv2, "Wait");

    createPlanetShadow();

    mHoleSunPlanetOutside = MR::createModelObjMapObj("穴あき太陽（外側）", "KoopaVS3HoleSunPlanet", nullptr);
    MR::setClippingTypeSphere(mHoleSunPlanetOutside, 40000.0f);
    MR::setClippingFarMax(mHoleSunPlanetOutside);
    MR::resetPosition(mHoleSunPlanetOutside, "惑星Ｌｖ３");
    MR::tryStartAllAnim(mHoleSunPlanetOutside, "KoopaVS3HoleSunPlanet");

    mHoleSunPlanetOutsideBloom = MR::createBloomModel(mHoleSunPlanetOutside, nullptr);

    mHoleSunPlanetInside = MR::createModelObjMapObj("穴あき太陽（内側）", "KoopaVS3HoleSunInsidePlanet", nullptr);
    MR::invalidateClipping(mHoleSunPlanetInside);
    MR::resetPosition(mHoleSunPlanetInside, "惑星Ｌｖ３");
    MR::tryStartAllAnim(mHoleSunPlanetInside, "KoopaVS3HoleSunInsidePlanet");

    mHoleSunPlanetInsideBloom = MR::createBloomModel(mHoleSunPlanetInside, nullptr);
    mHoleSunPlanetInside->kill();

    MR::registerDemoSimpleCastAll(mHoleSunPlanetOutside);
    MR::registerDemoSimpleCastAll(mHoleSunPlanetOutsideBloom);
    MR::registerDemoSimpleCastAll(mHoleSunPlanetInside);
    MR::registerDemoSimpleCastAll(mHoleSunPlanetInsideBloom);

    mPeach = createDemoNpc("ピーチ", "Peach");
    mKoopaJr = createDemoNpc("クッパＪｒ", "KoopaJr");
    mKoopaJrShip = createDemoNpc("クッパＪｒ戦艦", "KoopaJrShip");

    pName2 = "MeteorStrike";
    pName1 = "デモ砲弾１";
    ModelObj* pModelObj = MR::createModelObjEnemy(pName1, pName2, nullptr);
    pModelObj->initWithoutIter();
    MR::resetPosition(pModelObj, "デモ中心");
    pModelObj->kill();
    mMeteor1 = pModelObj;

    pName2 = "MeteorStrike";
    pName1 = "デモ砲弾２";
    pModelObj = MR::createModelObjEnemy(pName1, pName2, nullptr);
    pModelObj->initWithoutIter();
    MR::resetPosition(pModelObj, "デモ中心");
    pModelObj->kill();
    mMeteor2 = pModelObj;

    pName2 = "MeteorStrike";
    pName1 = "デモ砲弾３";
    pModelObj = MR::createModelObjEnemy(pName1, pName2, nullptr);
    pModelObj->initWithoutIter();
    MR::resetPosition(pModelObj, "デモ中心");
    pModelObj->kill();
    mMeteor3 = pModelObj;

    createCommonParts();
}

void KoopaParts::createRock() {
    if (mRock == nullptr) {
        mRock = createKoopaBodyParts(mKoopa, "クッパ岩", "KoopaRock", "RockFixPos");
        mRock->kill();

        mRockBreak = new KoopaRockBreak(mKoopa);
        mRockBreak->initWithoutIter();
    }
}

void KoopaParts::createRollBall() {
    if (mRollBall == nullptr) {
        mRollBall = createKoopaBodyParts(mKoopa, "回転攻撃ボール", "KoopaRollBall", "RollBallFixPos");
        mRollBall->kill();
    }
}

void KoopaParts::createCommonParts() {
    mThornBig = createKoopaBodyParts(mKoopa, "尻尾のトゲ（大）", "KoopaThorn", "TailThornBigFixPos");
    mThornSmall = createKoopaBodyParts(mKoopa, "尻尾のトゲ（小）", "KoopaThorn", "TailThornSmallFixPos");
    mArmorBreak = createKoopaBodyParts(mKoopa, "壊れ甲羅", "KoopaArmorBreak", "ArmorBreakFixPos");
    mArmorBreak->kill();

    mThornBreak = createKoopaBodyParts(mKoopa, "トゲ破片", "KoopaThornBreak", "ThornBreakFixPos");
    mThornBreak->kill();

    mFireShort = new LiveActorGroup("ショート炎", 32);
    mFireShort->initWithoutIter();

    for (int idx = 0; idx < 32; idx++) {
        KoopaFireShort* pFireShort = new KoopaFireShort(mKoopa);
        pFireShort->initWithoutIter();
        mFireShort->registerActor(pFireShort);
    }

    mShockWave = new LiveActorGroup("衝撃波（球状）", 8);
    mShockWave->initWithoutIter();

    for (int idx = 0; idx < 8; idx++) {
        KoopaShockWave* pShockWave = new KoopaShockWave(mKoopa);
        pShockWave->initWithoutIter();
        mShockWave->registerActor(pShockWave);
    }
}

void KoopaParts::createFireStairs(bool a1) {
    mFireStairs = new LiveActorGroup("炎（階段用）保持", 16);
    mFireStairs->initWithoutIter();

    for (int idx = 0; idx < 16; idx++) {
        KoopaFireStairs* pFireStairs = new KoopaFireStairs("炎（階段用）", a1);
        pFireStairs->initWithoutIter();
        mFireStairs->registerActor(pFireStairs);
    }
}
