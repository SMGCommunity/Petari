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

namespace {
    LiveActor* createKoopaBodyParts(LiveActor* pActor, const char* pName, const char* pModelName, const char* pJointName) {
        PartsModel* pPartsModel = new PartsModel(pActor, pName, pModelName, nullptr, 18, false);

        pPartsModel->loadFixedPosition(pJointName);
        pPartsModel->initWithoutIter();

        MR::initLightCtrl(pPartsModel);
        MR::registerDemoSimpleCastAll(pPartsModel);

        return pPartsModel;
    };

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

KoopaParts::KoopaParts(Koopa* pKoopa, const JMapInfoIter& rIter)
    : mKoopa(pKoopa), mPlanetRadius(1300.0f), _8(), _C(), _10(), _14(), mBattleMapPlanet(), mPlanetShadow(), _20(), _24(), _28(), _2C(), _30(), _34(),
      _38(), _3C(), _40(), _44(), _48(), _4C(), _50(), _54(), _58(), _5C(), _60(), _64(), _68(), _6C(), _70(), mActorCameraInfo() {
    mActorCameraInfo = new ActorCameraInfo(rIter);
    MR::declareCameraRegisterVec(mKoopa, 0, &mKoopa->mPosition);
}

KoopaFireStairs* KoopaParts::emitFireStairsToPos(const KoopaBattleMapStair* pStair, const TVec3f& rPosition, bool useFront) {
    KoopaFireStairs* pStairs = static_cast< KoopaFireStairs* >(_28->getDeadActor());

    if (pStairs == nullptr) {
        return nullptr;
    }

    pStairs->mPosition.set(rPosition);

    useFront ? pStairs->setInfo(pStair, KoopaFunction::getKoopaFrontPtr(mKoopa)) : pStairs->setInfo(pStair, nullptr);

    pStairs->appear();

    return pStairs;
}

void KoopaParts::killFireStairsAll() {
    for (int idx = 0; idx < 16; idx++) {
        LiveActor* pActor = _28->getActor(idx);

        if (!MR::isDead(pActor)) {
            pActor->makeActorDead();
        }
    }
}

void KoopaParts::emitFireShort(bool, bool) {
}

void KoopaParts::emitFireLongTime() {
}

void KoopaParts::emitShockWave() {
    LiveActor* pActor = _20->getDeadActor();

    if (pActor != nullptr) {
        pActor->appear();
    }
}

TVec3f& KoopaParts::getPlanetPos() {
    if (KoopaFunction::isKoopaVs1(mKoopa) || KoopaFunction::isKoopaVs2(mKoopa)) {
        return mBattleMapPlanet->mPosition;
    }

    if (KoopaFunction::isKoopaLv1(mKoopa)) {
        return mBattleMapPlanet->mPosition;
    }

    if (KoopaFunction::isKoopaLv2(mKoopa)) {
        return _44->mPosition;
    }

    return _48->mPosition;
}

f32 KoopaParts::getPlanetRadius() {
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
    mBattleMapPlanet = new KoopaBattleMapPlanet("クッパ惑星", "KoopaBattleMapPlanet", false, false, false);
    MR::resetPosition(mBattleMapPlanet, "惑星中心");
    mBattleMapPlanet->initWithoutIter();

    createPlanetShadow();
    createCommonParts();
}

void KoopaParts::initVs2() {
    KoopaBattleMapPlanet* pBattleMapPlanet = new KoopaBattleMapPlanet("クッパ惑星", "KoopaBattleMapPlanetVs2", true, false, false);
    MR::resetPosition(pBattleMapPlanet, "惑星中心");
    pBattleMapPlanet->initWithoutIter();
    mBattleMapPlanet = pBattleMapPlanet;

    createCommonParts();
}

void KoopaParts::initVs3() {
    createFireStairs(true);
    
    KoopaBattleMapPlanet* pBattleMapPlanet = new KoopaBattleMapPlanet("クッパ惑星Ｌｖ１", "KoopaBattleMapPlanetVs3Lv1", false, false, true);
    MR::resetPosition(pBattleMapPlanet, "惑星中心");
    pBattleMapPlanet->initWithoutIter();
    mBattleMapPlanet = pBattleMapPlanet;

    pBattleMapPlanet = new KoopaBattleMapPlanet("クッパ惑星Ｌｖ２", "KoopaBattleMapPlanetVs3Lv2", false, false, true);
    MR::resetPosition(pBattleMapPlanet, "惑星Ｌｖ２");
    pBattleMapPlanet->initWithoutIter();
    _44 = pBattleMapPlanet;

    pBattleMapPlanet = new KoopaBattleMapPlanet("クッパ惑星Ｌｖ３", "KoopaBattleMapPlanetVs3Lv3", false, true, false);
    MR::resetPosition(pBattleMapPlanet, "惑星Ｌｖ３");
    pBattleMapPlanet->initWithoutIter();
    _48 = pBattleMapPlanet;

    MR::startBrk(mBattleMapPlanet, "Wait");
    MR::startBrk(_44, "Wait");

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

    _5C = createDemoNpc("ピーチ", "Peach");
    _60 = createDemoNpc("クッパＪｒ", "KoopaJr");
    _64 = createDemoNpc("クッパＪｒ戦艦", "KoopaJrShip");

    ModelObj* pModelObj = MR::createModelObjEnemy("デモ砲弾１", "MeteorStrike", nullptr);
    pModelObj->initWithoutIter();
    MR::resetPosition(pModelObj, "デモ中心");
    pModelObj->kill();
    _68 = pModelObj;

    pModelObj = MR::createModelObjEnemy("デモ砲弾２", "MeteorStrike", nullptr);
    pModelObj->initWithoutIter();
    MR::resetPosition(pModelObj, "デモ中心");
    pModelObj->kill();
    _6C = pModelObj;

    pModelObj = MR::createModelObjEnemy("デモ砲弾３", "MeteorStrike", nullptr);
    pModelObj->initWithoutIter();
    MR::resetPosition(pModelObj, "デモ中心");
    pModelObj->kill();
    _70 = pModelObj;

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
