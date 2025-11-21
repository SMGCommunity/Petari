#include "Game/MapObj/AstroDomeBlueStar.hpp"
#include "Game/LiveActor/LiveActor.hpp"
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
