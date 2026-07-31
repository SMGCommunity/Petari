#include "Game/MapObj/AstroCountDownPlate.hpp"
#include "Game/LiveActor/MaterialCtrl.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/AstroMapObjFunction.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <JSystem/J3DGraphBase/J3DTevs.hpp>

namespace NrvAstroCountDownPlate {
    NEW_NERVE(AstroCountDownPlateNrvDead, AstroCountDownPlate, Wait);
    NEW_NERVE(AstroCountDownPlateNrvAlive, AstroCountDownPlate, Wait);
    NEW_NERVE(AstroCountDownPlateNrvRevival, AstroCountDownPlate, Revival);
    NEW_NERVE(AstroCountDownPlateNrvCountToZero, AstroCountDownPlate, CountToZero);
};  // namespace NrvAstroCountDownPlate

namespace {
    const char* cMaterialName01 = "StarNumber01_v_x";
    const char* cMaterialName10 = "StarNumber01_v_x(2)";
    const char* cLastBattleDemoName = "ロゼッタ最終決戦デモ";
    const char* cStartCountDownDemoName = "ロゼッタカウントダウン開始デモ";
    static const s32 sStepForCountToZero = 50;
};  // namespace

AstroCountDownPlate::AstroCountDownPlate(const char* pName) : LiveActor(pName), _1B4() {
}

void AstroCountDownPlate::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("AstroCountDownPlate", nullptr, true);
    initTextureAtNumLeftStar();
    MR::newDifferedDLBuffer(this);
    MR::connectToSceneMapObj(this);
    initEffectKeeper(0, nullptr, false);
    initSound(2, false);
    MR::invalidateClipping(this);

    if (MR::tryRegisterDemoCast(this, ::cStartCountDownDemoName, rIter)) {
        MR::tryRegisterDemoActionFunctorDirect(this, MR::Functor(this, &AstroCountDownPlate::startDemoStartCountDown), ::cStartCountDownDemoName,
                                               nullptr);
    }

    if (MR::tryRegisterDemoCast(this, ::cLastBattleDemoName, rIter)) {
        MR::tryRegisterDemoActionFunctorDirect(this, MR::Functor(this, &AstroCountDownPlate::startDemoLastBattle), ::cLastBattleDemoName, nullptr);
    }

    MR::registerDemoSimpleCastAll(this);
    initNerve(&NrvAstroCountDownPlate::AstroCountDownPlateNrvDead::sInstance);
    selectNrvWait();
    makeActorAppeared();
}

void AstroCountDownPlate::exeWait() {
    if (MR::isFirstStep(this) && isNerve(&NrvAstroCountDownPlate::AstroCountDownPlateNrvAlive::sInstance) && !MR::isEffectValid(this, "Light")) {
        MR::emitEffect(this, "Light");

        _1B4 = true;
    }

    if (_1B4) {
        MR::startLevelSound(this, "SE_OJ_LV_CDN_PLATE_LIGHT");
    }
}

void AstroCountDownPlate::exeRevival() {
    if (MR::isFirstStep(this)) {
        MR::showMaterial(this, ::cMaterialName01);
        MR::showMaterial(this, ::cMaterialName10);
        MR::tryStartAllAnim(this, "Revival");
        MR::emitEffect(this, "Light");

        _1B4 = true;
    }

    if (_1B4) {
        MR::startLevelSound(this, "SE_OJ_LV_CDN_PLATE_LIGHT");
    }

    if (MR::isBrkStopped(this)) {
        setNerve(&NrvAstroCountDownPlate::AstroCountDownPlateNrvAlive::sInstance);
    }
}

void AstroCountDownPlate::exeCountToZero() {
    if (MR::isFirstStep(this)) {
        MR::showMaterial(this, ::cMaterialName01);
        MR::showMaterial(this, ::cMaterialName10);
        MR::startSound(this, "SE_OJ_CDN_PLATE_ONE_TO_ZERO");
    }

    f32 v2 = MR::calcNerveRate(this, ::sStepForCountToZero);
    f32 v3 = MR::getConvergeVibrationValue(v2, 0.1f, 0.0f, 0.3f, 4.0f);
    _8C.getTexMtxInfo().mSRT.mTranslationY = v3;

    if (_1B4) {
        MR::startLevelSound(this, "SE_OJ_LV_CDN_PLATE_LIGHT");
    }

    if (MR::isStep(this, ::sStepForCountToZero)) {
        setNerve(&NrvAstroCountDownPlate::AstroCountDownPlateNrvAlive::sInstance);
    }
}

void AstroCountDownPlate::initTextureAtNumLeftStar() {
    MR::initDLMakerTexMtx(this, ::cMaterialName01)->setTexMtx(0, &_8C);
    MR::initDLMakerTexMtx(this, ::cMaterialName10)->setTexMtx(0, &_120);
    setNumLeftStar();
}

void AstroCountDownPlate::setNumLeftStar() {
    s32 starLeft = MR::getPowerStarLeftToDisplayCountDownPlate();
    f32 mod = starLeft % 10;
    _120.getTexMtxInfo().mSRT.mTranslationY = 0.1f * (starLeft / 10);
    _8C.getTexMtxInfo().mSRT.mTranslationY = 0.1f * mod;
}

void AstroCountDownPlate::selectNrvWait() {
    if (AstroMapObjFunction::isAlreadyRevival("AstroCountDownPlate", -1)) {
        setNerve(&NrvAstroCountDownPlate::AstroCountDownPlateNrvAlive::sInstance);
    } else {
        setNerve(&NrvAstroCountDownPlate::AstroCountDownPlateNrvDead::sInstance);
    }

    setupStateWait();
}

void AstroCountDownPlate::setupStateWait() {
    if (isNerve(&NrvAstroCountDownPlate::AstroCountDownPlateNrvDead::sInstance)) {
        MR::hideMaterial(this, ::cMaterialName01);
        MR::hideMaterial(this, ::cMaterialName10);
    } else {
        MR::showMaterial(this, ::cMaterialName01);
        MR::showMaterial(this, ::cMaterialName10);

        if (!MR::isOnGameEventFlagViewNormalEnding()) {
            MR::startBrk(this, "Red");
        } else {
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
