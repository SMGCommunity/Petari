#include "Game/MapObj/AstroCountDownPlate.hpp"
#include "Game/MapObj/AstroMapObjFunction.hpp"
#include "Game/LiveActor/MaterialCtrl.hpp"

namespace {
    const char* cMaterialName01 = "StarNumber01_v_x";
    const char* cMaterialName10 = "StarNumber01_v_x(2)";
    const char* cLastBattleDemoName = "ロゼッタ最終決戦デモ";
    const char* cStartCountDownDemoName = "ロゼッタカウントダウン開始デモ";
};

AstroCountDownPlate::AstroCountDownPlate(const char *pName) : LiveActor(pName) {
    _8C.mInfo = j3dDefaultTexMtxInfo;
    _120.mInfo = j3dDefaultTexMtxInfo;
    _1B4 = 0;
}

void AstroCountDownPlate::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("AstroCountDownPlate", nullptr, true);
    initTextureAtNumLeftStar();
    MR::newDifferedDLBuffer(this);
    MR::connectToSceneMapObj(this);
    initEffectKeeper(0, nullptr, false);
    initSound(2, false);
    MR::invalidateClipping(this);
    
    if (MR::tryRegisterDemoCast(this, cStartCountDownDemoName, rIter)) {
        MR::FunctorV0M<AstroCountDownPlate *, void (AstroCountDownPlate::*)()> startDemoFunc = MR::Functor<AstroCountDownPlate>(this, &AstroCountDownPlate::startDemoStartCountDown);
        MR::tryRegisterDemoActionFunctorDirect(this, startDemoFunc, cStartCountDownDemoName, nullptr);
    }

    if (MR::tryRegisterDemoCast(this, cLastBattleDemoName, rIter)) {
        MR::FunctorV0M<AstroCountDownPlate *, void (AstroCountDownPlate::*)()> lastBattleFunc = MR::Functor<AstroCountDownPlate>(this, &AstroCountDownPlate::startDemoLastBattle);
        MR::tryRegisterDemoActionFunctorDirect(this, lastBattleFunc, cLastBattleDemoName, nullptr);
    }

    MR::registerDemoSimpleCastAll(this);
    initNerve(&NrvAstroCountDownPlate::AstroCountDownPlateNrvDead::sInstance);
    selectNrvWait();
    makeActorAppeared();
}

void AstroCountDownPlate::exeWait() {
    if (MR::isFirstStep(this) && isNerve(&NrvAstroCountDownPlate::AstroCountDownPlateNrvAlive::sInstance) && !MR::isEffectValid(this, "Light")) {
        MR::emitEffect(this, "Light");
        _1B4 = 1;
    }

    if (_1B4) {
        MR::startLevelSound(this, "SE_OJ_LV_CDN_PLATE_LIGHT", -1, -1, -1);
    }
}

void AstroCountDownPlate::exeRevival() {
    if (MR::isFirstStep(this)) {
        MR::showMaterial(this, cMaterialName01);
        MR::showMaterial(this, cMaterialName10);
        MR::tryStartAllAnim(this, "Revival");
        MR::emitEffect(this, "Light");
        _1B4 = 1;
    }

    if (_1B4) {
        MR::startLevelSound(this, "SE_OJ_LV_CDN_PLATE_LIGHT", -1, -1, -1);
    }

    if (MR::isBrkStopped(this)) {
        setNerve(&NrvAstroCountDownPlate::AstroCountDownPlateNrvAlive::sInstance);
    }
}

void AstroCountDownPlate::exeCountToZero() {
    if (MR::isFirstStep(this)) {
        MR::showMaterial(this, cMaterialName01);
        MR::showMaterial(this, cMaterialName10);
        MR::startSound(this, "SE_OJ_CDN_PLATE_ONE_TO_ZERO", -1, -1);
    }

    f32 v2 = MR::calcNerveRate(this, 50);
    f32 v3 = MR::getConvergeVibrationValue(v2, 0.1f, 0.0f, 0.30000001f, 4.0f);
    _8C.mInfo.mSRTInfo.mTransY = v3;

    if (_1B4) {
        MR::startLevelSound(this, "SE_OJ_LV_CDN_PLATE_LIGHT", -1, -1, -1);
    }

    if (MR::isStep(this, 50)) {
        setNerve(&NrvAstroCountDownPlate::AstroCountDownPlateNrvAlive::sInstance);
    }
}

void AstroCountDownPlate::initTextureAtNumLeftStar() {
    MR::initDLMakerTexMtx(this, cMaterialName01)->setTexMtx(0, &_8C);
    MR::initDLMakerTexMtx(this, cMaterialName10)->setTexMtx(0, &_120);
    setNumLeftStar();
}

void AstroCountDownPlate::setNumLeftStar() {
    s32 starLeft = MR::getPowerStarLeftToDisplayCountDownPlate();
    f32 mod = starLeft % 10;
    _120.mInfo.mSRTInfo.mTransY = 0.1f * (starLeft / 10);
    _8C.mInfo.mSRTInfo.mTransY = 0.1f * mod;
}

void AstroCountDownPlate::selectNrvWait() {
    if (AstroMapObjFunction::isAlreadyRevival("AstroCountDownPlate", -1)) {
        setNerve(&NrvAstroCountDownPlate::AstroCountDownPlateNrvAlive::sInstance);
    }
    else {
        setNerve(&NrvAstroCountDownPlate::AstroCountDownPlateNrvDead::sInstance);
    }

    setupStateWait();
}

void AstroCountDownPlate::setupStateWait() {
    if (isNerve(&NrvAstroCountDownPlate::AstroCountDownPlateNrvDead::sInstance)) {
        MR::hideMaterial(this, cMaterialName01);
        MR::hideMaterial(this, cMaterialName10);
    }
    else {
        MR::showMaterial(this, cMaterialName01);
        MR::showMaterial(this, cMaterialName10);
        if (!MR::isOnGameEventFlagViewNormalEnding()) {
            MR::startBrk(this, "Red");
        }
        else {
            MR::startBrk(this, "Green");
        }
    }
}

void AstroCountDownPlate::startDemoStartCountDown() {
    setNerve(&NrvAstroCountDownPlate::AstroCountDownPlateNrvRevival::sInstance);
}

void AstroCountDownPlate::startDemoLastBattle() {
    setNerve(&NrvAstroCountDownPlate::AstroCountDownPlateNrvCountToZero::sInstance);
}

namespace NrvAstroCountDownPlate {
    INIT_NERVE(AstroCountDownPlateNrvDead);
    INIT_NERVE(AstroCountDownPlateNrvAlive);
    INIT_NERVE(AstroCountDownPlateNrvRevival);
    INIT_NERVE(AstroCountDownPlateNrvCountToZero);
};

AstroCountDownPlate::~AstroCountDownPlate() {

}
