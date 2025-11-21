#include "Game/MapObj/AstroDomeBlueStar.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/SphereSelector.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

AstroDomeBlueStar::CaptureActor::CaptureActor() : LiveActor("キャプチャ用アクター") {
    _8C.identity();
}

void AstroDomeBlueStar::CaptureActor::init(const JMapInfoIter &rIter) {
    MR::connectToSceneMapObjDecorationMovement(this);
    initEffectKeeper(0, "GCapture", false);
    MR::setEffectHostMtx(this, "LightSplash", _8C);
    MR::setEffectHostMtx(this, "RibbonPoint", _8C);
    MR::setEffectHostMtx(this, "RibbonBreak", _8C);
    MR::invalidateClipping(this);
    MR::registerDemoSimpleCastAll(this);
    makeActorDead();
}

void AstroDomeBlueStar::CaptureActor::kill() {
    LiveActor::kill();
    MR::forceDeleteEffect(this, "Light");
    MR::emitEffect(this, "LightBreak");
}

// AstroDomeBlueStar::CaptureActor::updateTransTraction

namespace NrvAstroDomeBlueStar {
    NEW_NERVE(AstroDomeBlueStarNrvAppear, AstroDomeBlueStar, Appear);
    NEW_NERVE(AstroDomeBlueStarNrvWait, AstroDomeBlueStar, Wait);
    NEW_NERVE(AstroDomeBlueStarNrvPointing, AstroDomeBlueStar, Pointing);
    NEW_NERVE(AstroDomeBlueStarNrvTryStartBind, AstroDomeBlueStar, TryStartBind);
    NEW_NERVE(AstroDomeBlueStarNrvBindTraction, AstroDomeBlueStar, BindTraction);
    NEW_NERVE(AstroDomeBlueStarNrvBindHold, AstroDomeBlueStar, BindHold);
    NEW_NERVE(AstroDomeBlueStarNrvBindEnd, AstroDomeBlueStar, BindEnd);
    NEW_NERVE(AstroDomeBlueStarNrvGalaxySelect, AstroDomeBlueStar, GalaxySelect);
    NEW_NERVE(AstroDomeBlueStarNrvGalaxyConfirmStart, AstroDomeBlueStar, GalaxyConfirmStart);
    NEW_NERVE(AstroDomeBlueStarNrvGalaxyConfirm, AstroDomeBlueStar, GalaxyConfirm);
    NEW_NERVE(AstroDomeBlueStarNrvGalaxyConfirmCancel, AstroDomeBlueStar, GalaxyConfirmCancel);
};

AstroDomeBlueStar::AstroDomeBlueStar(const char *pName) : LiveActor(pName), mCaptureRibbon(nullptr), _F0(0), mCaptureActor(nullptr),
    mZoomPos(gZeroVec) {
    
    _90.identity();
    _C0.identity();
}

// AstroDomeBlueStar::exeBindTraction
// AstroDomeBlueStar::exeBindHold

void AstroDomeBlueStar::exeBindEnd() {
    if (MR::isFirstStep(this)) {
        MR::forceDeleteEffect(mCaptureActor, "Light");
        MR::emitEffect(mCaptureActor, "LightBreak");
        MR::endBindAndPlayerWeakGravityLimitJump(this, TVec3f(0.0f, 0.0f, 0.0f));
    }

    setNerve(&NrvAstroDomeBlueStar::AstroDomeBlueStarNrvWait::sInstance);
}

void AstroDomeBlueStar::exeGalaxySelect() {
    if (MR::isFirstStep(this)) {
        mPosition.z = 0.0f;
        mPosition.y = 0.0f;
        mPosition.x = 0.0f;
    }
}

void AstroDomeBlueStar::exeGalaxyConfirmStart() {
    s32 frame = SphereSelectorFunction::getConfirmStartCancelFrame();
    calcZoomInPos(&mZoomPos);
    mPosition.scale(MR::calcNerveEaseOutRate(this, frame), mZoomPos);
    MR::setNerveAtStep(this, &NrvAstroDomeBlueStar::AstroDomeBlueStarNrvGalaxyConfirm::sInstance, frame);
}

void AstroDomeBlueStar::exeGalaxyConfirmCancel() {
    s32 frame = SphereSelectorFunction::getConfirmStartCancelFrame();
    f32 rate = MR::calcNerveEaseInRate(this, frame);
    mPosition.scale(1.0f - rate, mZoomPos);
    MR::setNerveAtStep(this, &NrvAstroDomeBlueStar::AstroDomeBlueStarNrvGalaxySelect::sInstance, frame);
}
