#include "Game/Boss/TombSpiderDemo.hpp"
#include "Game/Boss/TombSpider.hpp"
#include "Game/Boss/TombSpiderFunction.hpp"
#include "Game/Boss/TombSpiderGland.hpp"
#include "Game/Boss/TombSpiderVitalSpot.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/Map/PlanetMap.hpp"
#include "Game/MapObj/SpiderThread.hpp"
#include "Game/NPC/TalkDirector.hpp"
#include <JSystem/JGeometry/TVec.hpp>

namespace {
    static const s32 sStepDemoGateOpen = 180;
    static const f32 sGateOpenPlayerDistance = 6000.0f;
    static const f32 sPlanetRadius = 7000.0f;
    static const f32 sPlanetInsideRadius = 1800.0f;
    static const s32 sStepGateBreak = 75;
    static const f32 sGateOpenPlayerSpeed = 60.0f;
    static const s32 sStepGateOpenChangeCamera = 120;
    static const s32 sStepCocoonBreak = 60;
    static const s32 sStepJumpRotateToPlayerStart = 20;
    static const s32 sStepJumpRotateToPlayerEnd = 70;
    static const s32 sStepBattle2ndStartCameraInterpolateFrame = 10;
    static const s32 sStepChanceDamagePlanetBreak = 30;
    static const s32 sStepBattle2ndStartJumpStart = 150;
    static const s32 sStepBattle2ndStartJumpEnd = 190;
    static const s32 sStepBattleEndPlanetBreak = 15;
}  // namespace

TombSpiderDemo::TombSpiderDemo(TombSpider* pParent) : mParent(pParent), mRotateSpeed(0.0f) {
    mMtx.identity();
    MR::declareEventCamera(getCameraInfo(), "ゲートオープンデモ[トゥームスパイダー]");
    MR::declareEventCamera(getCameraInfo(), "マリオ着地デモ[トゥームスパイダー]");
    MR::declareEventCamera(getCameraInfo(), "チャンス開始[トゥームスパイダー]");
    MR::initAnimCamera(TombSpiderFunction::getPlanet(mParent), getCameraInfo(), "Appear");
    MR::initAnimCamera(TombSpiderFunction::getPlanet(mParent), getCameraInfo(), "Battle2ndStart");
    MR::initAnimCamera(TombSpiderFunction::getPlanet(mParent), getCameraInfo(), "Death");
}

bool TombSpiderDemo::updateGateOpen() {
    if (MR::isFirstStep(mParent)) {
        TombSpiderFunction::startTombSpiderDemo(mParent, "ゲートオープンデモ[トゥームスパイダー]", nullptr);
        MR::startEventCamera(getCameraInfo(), "ゲートオープンデモ[トゥームスパイダー]", CameraTargetArg(mParent), -1);

        TVec3f pos(mParent->mPosition);
        pos.z += sPlanetRadius;
        MR::makeMtxTR(mMtx, pos, TVec3f(270.0f, 180.0f, 0.0f));

        MR::startBckWithInterpole(MR::getPlayerDemoActor(), "CocoonFly", 0);
        MR::setBckFrameAndStop(MR::getPlayerDemoActor(), 50.0f);
        MR::emitEffect(MR::getPlayerDemoActor(), "SpaceCocoonBlur");
        MR::appearSpiderThread();
    }

    if (MR::isLessEqualStep(mParent, sStepGateBreak)) {
        mMtx[2][3] -= sGateOpenPlayerSpeed;
    } else {
        TVec3f startPos(mParent->mPosition.x, mParent->mPosition.y, mParent->mPosition.z + (sPlanetRadius - sGateOpenPlayerSpeed * sStepGateBreak));
        TVec3f endPos(mParent->mPosition.x, mParent->mPosition.y - sPlanetInsideRadius, mParent->mPosition.z);

        f32 t1 = (mParent->getNerveStep() - (sStepGateBreak + 3)) / static_cast< f32 >(sStepDemoGateOpen - (sStepGateBreak + 3));
        f32 t2 = MR::getEaseInValue(t1, 0.0f, 1.0f, 1.0f);
        TVec3f pos;
        pos.x = startPos.x * (1.0f - t1) + endPos.x * t1;
        pos.y = startPos.y * (1.0f - t2) + endPos.y * t2;
        pos.z = startPos.z * (1.0f - t1) + endPos.z * t1;
        mMtx.setPos(pos);
    }

    MR::setPlayerBaseMtx(mMtx);

    if (MR::isStep(mParent, sStepGateBreak)) {
        MR::startBck(MR::getPlayerDemoActor(), "AirRotation", nullptr);
        MR::startBck(TombSpiderFunction::getPlanet(mParent), "BattleStart", nullptr);
        MR::onCalcAnim(TombSpiderFunction::getPlanet(mParent));
        TombSpiderFunction::getCocoon(mParent)->appear();
        MR::deleteEffect(MR::getPlayerDemoActor(), "SpaceCocoonBlur");
        MR::startSystemSE("SE_OJ_TSPIDER_PLANET_BREAK1", -1, -1);
    }

    if (MR::isStep(mParent, sStepGateBreak - 1)) {
        MR::shakeCameraNormal();
    }

    if (MR::isStep(mParent, sStepGateOpenChangeCamera)) {
        MR::endEventCamera(getCameraInfo(), "ゲートオープンデモ[トゥームスパイダー]", false, -1);
        MR::startEventCameraNoTarget(getCameraInfo(), "マリオ着地デモ[トゥームスパイダー]", -1);
    }

    if (MR::isStep(mParent, sStepDemoGateOpen)) {
        MR::startSound(MR::getPlayerDemoActor(), "SE_PM_LAND_HEAVY", -1, -1);
        MR::startSound(MR::getPlayerDemoActor(), "SE_PV_LAND", -1, -1);
        TombSpiderFunction::resetPlayerPosTombSpider(mParent, false);
        TombSpiderFunction::endTombSpiderDemo(mParent, "ゲートオープンデモ[トゥームスパイダー]", "マリオ着地デモ[トゥームスパイダー]");
        return true;
    }
    return false;
}

bool TombSpiderDemo::updateCocoonBreak() {
    if (MR::isFirstStep(mParent)) {
        TombSpiderFunction::startTombSpiderAnimCameraDemo(mParent, "出現", "Appear", 0);
        TombSpiderFunction::resetPlayerPosTombSpider(mParent, true);

        MR::sendMsgToAllLiveActor(ACTMES_TOMB_SPIDER_BATTLE_START, nullptr);
        MR::invalidateTalkDirector();

        MR::startBck(TombSpiderFunction::getCocoon(mParent), "Open", nullptr);
        MR::startBrk(TombSpiderFunction::getCocoon(mParent), "Open");
        MR::startBckWithInterpole(mParent, "Appear", 0);
        MR::setBckFrameAndStop(mParent, 0.0f);

        MR::showModel(mParent);

        MR::startBrk(mParent, "Battle1st");
        MR::startBrk(TombSpiderFunction::getGlandFrontL(mParent), "Battle1st");
        MR::startBrk(TombSpiderFunction::getGlandFrontR(mParent), "Battle1st");
        MR::startBrk(TombSpiderFunction::getGlandRearL(mParent), "Battle1st");
        MR::startBrk(TombSpiderFunction::getGlandRearR(mParent), "Battle1st");

        TombSpiderFunction::appearThreadAttacherAll(mParent);
    }

    if (MR::isStep(mParent, sStepCocoonBreak)) {
        return true;
    }

    return false;
}

bool TombSpiderDemo::updateBattle1stStart() {
    if (MR::isFirstStep(mParent)) {
        MR::startBck(mParent, "Appear", nullptr);
        MR::startSound(mParent, "SE_BV_TSPIDER_APPEAR", -1, -1);
    }

    if (MR::isBckStopped(mParent)) {
        return true;
    }

    return false;
}

bool TombSpiderDemo::updateBattle1stEnd() {
    if (MR::isFirstStep(mParent)) {
        TombSpiderFunction::startTombSpiderAnimCameraDemo(mParent, "１回戦終了", "Battle2ndStart", sStepBattle2ndStartCameraInterpolateFrame);
        TombSpiderFunction::resetPlayerPosTombSpider(mParent, true);
        MR::startBck(mParent, "ChanceDamageLast", nullptr);
    }

    if (MR::isStep(mParent, sStepChanceDamagePlanetBreak)) {
        MR::startBck(TombSpiderFunction::getPlanet(mParent), "BattleDamage", nullptr);
        MR::startSystemSE("SE_OJ_TSPI_PLANET_BREAK_M", -1, -1);
    }

    if (MR::isBckStopped(mParent)) {
        return true;
    }

    return false;
}

bool TombSpiderDemo::updateBattle2ndStart() {
    if (MR::isFirstStep(mParent)) {
        MR::startBck(mParent, "Battle2ndStart", nullptr);
        MR::startBrk(mParent, "Battle2nd");

        TombSpiderFunction::getGlandFrontL(mParent)->startActive();
        TombSpiderFunction::getGlandFrontR(mParent)->startActive();
        TombSpiderFunction::getGlandRearL(mParent)->startActive();
        TombSpiderFunction::getGlandRearR(mParent)->startActive();

        TombSpiderFunction::getVitalSpotC(mParent)->recover();
        TombSpiderFunction::getVitalSpotL(mParent)->recover();
        TombSpiderFunction::getVitalSpotR(mParent)->recover();
        mRotateSpeed = TombSpiderFunction::calcRotateSpeedToPlayer(mParent, sStepBattle2ndStartJumpEnd - sStepBattle2ndStartJumpStart);
    }

    if (MR::isGreaterEqualStep(mParent, sStepBattle2ndStartJumpStart) && MR::isLessStep(mParent, sStepBattle2ndStartJumpEnd)) {
        mParent->mRotation.z += mRotateSpeed;
    }

    if (MR::isBckStopped(mParent)) {
        for (s32 idx = 0; idx < 25; idx++) {
            TVec3f dir(1.0f, 0.0f, 0.0f);
            MR::rotateVecDegree(&dir, TVec3f(0.0f, 0.0f, 1.0f), MR::getRandom() * 360.0f);
            MR::appearStarPieceToDirection(mParent, mParent->mPosition, dir, 1, 0.0f, 40.0f, false);
            MR::startSound(mParent, "SE_OJ_STAR_PIECE_BURST", -1, -1);
        }
        TombSpiderFunction::endTombSpiderAnimCameraDemo(mParent, "１回戦終了", "Battle2ndStart");
        return true;
    }

    return false;
}

bool TombSpiderDemo::updateDeath() {
    if (MR::isFirstStep(mParent)) {
        TombSpiderFunction::startTombSpiderAnimCameraDemo(mParent, "死亡", "Death", 0);
        TombSpiderFunction::resetPlayerPosTombSpider(mParent, true);
        MR::startSystemSE("SE_BV_TSPIDER_DEATH", -1, -1);
        MR::startBck(mParent, "Death", nullptr);
        MR::startSpiderThreadBattleEnd();
        TombSpiderFunction::killThreadAttacherAll(mParent);
    }

    bool isDeathAnimDone = false;
    if (!MR::isHiddenModel(mParent) && MR::isBckStopped(mParent)) {
        isDeathAnimDone = true;
    }

    if (MR::isStep(mParent, sStepBattleEndPlanetBreak)) {
        MR::startBck(TombSpiderFunction::getPlanet(mParent), "BattleEnd", nullptr);
        MR::startSystemSE("SE_OJ_TSPIDER_PLANET_BREAK2", -1, -1);
        MR::onCalcAnim(TombSpiderFunction::getPlanet(mParent));
        MR::onSpiderThreadBloom();
        MR::onSwitchB(mParent);
    }

    if (isDeathAnimDone) {
        mParent->mPosition.setTrans(MR::getJointMtx(mParent, "Body"));

        mParent->calcAnim();
        MR::hideModel(mParent);
        MR::emitEffect(mParent, "Death");
        MR::startSound(mParent, "SE_BM_TSPIDER_EXPLOSION", -1, -1);
        MR::startAfterBossBGM();
        TombSpiderFunction::endTombSpiderAnimCameraDemo(mParent, "死亡", "Death");
        MR::onSwitchA(mParent);
        MR::requestAppearPowerStar(mParent, mParent->mPosition);
        return true;
    }

    return false;
}

void TombSpiderDemo::updateJumpRotateToPlayer() {
    if (MR::isFirstStep(mParent)) {
        MR::startBck(mParent, "Jump", nullptr);
        MR::startSound(mParent, "SE_BM_TSPIDER_JUMP", -1, -1);
        mRotateSpeed = TombSpiderFunction::calcRotateSpeedToPlayer(mParent, sStepJumpRotateToPlayerEnd - sStepJumpRotateToPlayerStart);
    }

    if (MR::isGreaterEqualStep(mParent, sStepJumpRotateToPlayerStart) && MR::isLessStep(mParent, sStepJumpRotateToPlayerEnd)) {
        mParent->mRotation.z += mRotateSpeed;
    }
}

bool TombSpiderDemo::isStartDemoGateOpen() const {
    return PSVECDistance(*MR::getPlayerPos(), mParent->mPosition) < sGateOpenPlayerDistance;
}

bool TombSpiderDemo::updateBattle1stStartJumpToPlayer() {
    if (MR::isAnimCameraEnd(TombSpiderFunction::getPlanet(mParent), getCameraInfo(), "Appear")) {
        MR::endAnimCamera(TombSpiderFunction::getPlanet(mParent), getCameraInfo(), "Appear", -1, true);
    }

    updateJumpRotateToPlayer();

    if (MR::isBckStopped(mParent)) {
        if (!MR::isDead(TombSpiderFunction::getCocoon(mParent))) {
            TombSpiderFunction::getCocoon(mParent)->kill();
        }
        TombSpiderFunction::endTombSpiderAnimCameraDemo(mParent, "出現", "Appear");
        return true;
    }

    return false;
}
