#include "Game/Boss/KoopaParts.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaFireShort.hpp"
#include "Game/Boss/KoopaFireStairs.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/Boss/KoopaPlanetShadow.hpp"
#include "Game/Boss/KoopaRockBreak.hpp"
#include "Game/Boss/KoopaShockWave.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Map/KoopaBattleMapPlanet.hpp"
#include "Game/Map/KoopaBattleMapStair.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "revolution/types.h"

KoopaParts::KoopaParts(Koopa* pKoopa, const JMapInfoIter& rIter)
    : mKoopa(pKoopa), mPlanetRadius(1300.0f), _8(), _C(), _10(), _14(), mPlanetLv1(), mPlanetShadow(), _20(), _24(), _28(), _2C(), _30(), _34(),
      _38(), _3C(), _40(), mPlanetLv2(), mPlanetLv3(), _4C(), _50(), _54(), _58(), mPeach(), mKoopaJr(), mKoopaJrShip(), mMeteor1(), mMeteor2(), mMeteor3(), mActorCameraInfo() {
    mActorCameraInfo = new ActorCameraInfo(rIter);
    MR::declareCameraRegisterVec(mKoopa, 0, &mKoopa->mPosition);
}

namespace {
    LiveActor* createKoopaBodyParts(LiveActor* pActor, const char* pName, const char* pModelName, const char* pJointName) {
        PartsModel* pPartsModel = new PartsModel(pActor, pName, pModelName, nullptr, 18, false);

        pPartsModel->loadFixedPosition(pJointName);
        pPartsModel->initWithoutIter();

        MR::initLightCtrl(pPartsModel);
        MR::registerDemoSimpleCastAll(pPartsModel);

        return pPartsModel;
    };
}  // namespace

KoopaFireStairs* KoopaParts::emitFireStairsToPos(const KoopaBattleMapStair* pMapStair, const TVec3f& rPosition, bool useFront) {
    KoopaFireStairs* pMapStairs = static_cast< KoopaFireStairs* >(_28->getDeadActor());

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
        LiveActor* pActor = _28->getActor(idx);

        if (!MR::isDead(pActor)) {
            pActor->makeActorDead();
        }
    }
}

void KoopaParts::emitFireShort(bool fast, bool curve) {
    KoopaFireShort* pFireShort = static_cast<KoopaFireShort *>(_24->getDeadActor());

    if (pFireShort != nullptr) {
        if (curve) {
            pFireShort->emitCurve();
        } else if (fast) {
            pFireShort->emitFast();
        } else {
            pFireShort->emitNormal();
        }
    }
}

void KoopaParts::emitFireLongTime() {
    KoopaFireShort* pFireShort = static_cast<KoopaFireShort *>(_24->getDeadActor());

    if (pFireShort != nullptr) {
        pFireShort->emitLongTime();
    }
}

void KoopaParts::emitShockWave() {
    LiveActor* pActor = _20->getDeadActor();

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
    _54->appear();
}

void KoopaParts::appearHoleSunPlanetOutside() {
    _4C->appear();
}

void KoopaParts::killHoleSunPlanetOutside() {
    _4C->kill();
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

    _4C = MR::createModelObjMapObj("穴あき太陽（外側）", "KoopaVS3HoleSunPlanet", nullptr);
    MR::setClippingTypeSphere(_4C, 40000.0f);
    MR::setClippingFarMax(_4C);
    MR::resetPosition(_4C, "惑星Ｌｖ３");
    MR::tryStartAllAnim(_4C, "KoopaVS3HoleSunPlanet");

    _50 = MR::createBloomModel(_4C, nullptr);

    _54 = MR::createModelObjMapObj("穴あき太陽（内側）", "KoopaVS3HoleSunInsidePlanet", nullptr);
    MR::invalidateClipping(_54);
    MR::resetPosition(_54, "惑星Ｌｖ３");
    MR::tryStartAllAnim(_54, "KoopaVS3HoleSunInsidePlanet");

    _58 = MR::createBloomModel(_54, nullptr);
    _54->kill();

    MR::registerDemoSimpleCastAll(_4C);
    MR::registerDemoSimpleCastAll(_50);
    MR::registerDemoSimpleCastAll(_54);
    MR::registerDemoSimpleCastAll(_58);

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
    if (_38 == nullptr) {
        _38 = createKoopaBodyParts(mKoopa, "クッパ岩", "KoopaRock", "RockFixPos");
        _38->kill();

        _3C = new KoopaRockBreak(mKoopa);
        _3C->initWithoutIter();
    }
}

void KoopaParts::createRollBall() {
    if (_40 == nullptr) {
        _40 = createKoopaBodyParts(mKoopa, "回転攻撃ボール", "KoopaRollBall", "RollBallFixPos");
        _40->kill();
    }
}

void KoopaParts::createCommonParts() {
    _8 = createKoopaBodyParts(mKoopa, "尻尾のトゲ（大）", "KoopaThorn", "TailThornBigFixPos");
    _C = createKoopaBodyParts(mKoopa, "尻尾のトゲ（小）", "KoopaThorn", "TailThornSmallFixPos");
    _10 = createKoopaBodyParts(mKoopa, "壊れ甲羅", "KoopaArmorBreak", "ArmorBreakFixPos");
    _10->kill();

    _14 = createKoopaBodyParts(mKoopa, "トゲ破片", "KoopaThornBreak", "ThornBreakFixPos");
    _14->kill();

    _24 = new LiveActorGroup("ショート炎", 32);
    _24->initWithoutIter();

    for (int idx = 0; idx < 32; idx++) {
        KoopaFireShort* pFireShort = new KoopaFireShort(mKoopa);
        pFireShort->initWithoutIter();
        _24->registerActor(pFireShort);
    }

    _20 = new LiveActorGroup("衝撃波（球状）", 8);
    _20->initWithoutIter();

    for (int idx = 0; idx < 8; idx++) {
        KoopaShockWave* pShockWave = new KoopaShockWave(mKoopa);
        pShockWave->initWithoutIter();
        _20->registerActor(pShockWave);
    }
}

void KoopaParts::createFireStairs(bool a1) {
    _28 = new LiveActorGroup("炎（階段用）保持", 16);
    _28->initWithoutIter();

    for (int idx = 0; idx < 16; idx++) {
        KoopaFireStairs* pFireStairs = new KoopaFireStairs("炎（階段用）", a1);
        pFireStairs->initWithoutIter();
        _28->registerActor(pFireStairs);
    }
}
