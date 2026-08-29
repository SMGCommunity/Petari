#include "Game/MapObj/AstroDomeBlueStar.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/SphereSelector.hpp"
#include "Game/MapObj/GCapture.hpp"
#include "Game/MapObj/GCaptureRibbon.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

namespace {
    const Vec cZoomInOffset = {400.0f, 2550.0f, -7600.0f};
    const s32 cTryStartBindFrame = 30;
    const s32 cBindTractionFrame = 90;
    const s32 cBubbleGrowFrame = 30;
    const f32 cBubbleMaxRadius = 150.0f;
};  // namespace

AstroDomeBlueStar::CaptureActor::CaptureActor() : LiveActor("キャプチャ用アクター") {
    mHostMtx.identity();
}

void AstroDomeBlueStar::CaptureActor::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjDecorationMovement(this);

    initEffectKeeper(0, "GCapture", false);

    MR::setEffectHostMtx(this, "LightSplash", mHostMtx);
    MR::setEffectHostMtx(this, "RibbonPoint", mHostMtx);
    MR::setEffectHostMtx(this, "RibbonBreak", mHostMtx);

    MR::invalidateClipping(this);

    MR::registerDemoSimpleCastAll(this);

    makeActorDead();
}

void AstroDomeBlueStar::CaptureActor::kill() {
    LiveActor::kill();

    MR::forceDeleteEffect(this, "Light");
    MR::emitEffect(this, "LightBreak");
}

void AstroDomeBlueStar::CaptureActor::updateTransTraction(const TVec3f& rBasePos, const TVec3f& rTarget, s32 step) {
    mPosition.lerp(rBasePos, rTarget, MR::getEaseInOutValue(static_cast< f32 >(step) / ::cBindTractionFrame, 0.0f, 1.0f, 1.0f));

    TVec3f dir;
    dir.sub(rTarget, rBasePos);

    if (MR::normalizeOrZero(&dir)) {
        mHostMtx.setTrans(mPosition);
        return;
    }

    f32 dist = MR::getLinerValue(step < ::cBubbleGrowFrame ? step : ::cBubbleGrowFrame, 0.0f, ::cBubbleMaxRadius, ::cBubbleGrowFrame);
    TVec3f pos;

    if (mPosition.distance(rTarget) < dist) {
        pos.set(rTarget);
    } else {
        pos.add(mPosition, dir * dist);
    }

    MR::makeMtxUpNoSupportPos(&mHostMtx, dir, pos);
}

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
};  // namespace NrvAstroDomeBlueStar

AstroDomeBlueStar::AstroDomeBlueStar(const char* pName) : LiveActor(pName), mCaptureRibbon(), mCameraInfo(), mCaptureActor(), mZoomPos(gZeroVec) {
    mHostMtx.identity();
    mBindStartMtx.identity();
}

void AstroDomeBlueStar::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("GCaptureTarget", nullptr, false);

    MR::startBrkAndSetFrameAndStop(this, "Switch", 1.0f);

    MR::connectToScene(this, MR::MovementType_MapObj, MR::CalcAnimType_MapObj, MR::DrawBufferType_NoShadowedMapObj, MR::DrawType_GCapture);

    mCaptureActor = new CaptureActor();
    mCaptureActor->initWithoutIter();

    initHitSensor(1);

    TVec3f& vec = mCaptureActor->mPosition;
    MR::addHitSensorPosBinder(this, "bind", 16, 80.0f, &vec, TVec3f(0.0f, 0.0f, 0.0f));
    getSensor("bind")->invalidate();

    initEffectKeeper(0, nullptr, false);
    MR::initActorCamera(this, rIter, &mCameraInfo);
    initSound(6, false);
    MR::initStarPointerTarget(this, 200.0f);

    mCaptureRibbon = new GCaptureRibbon("Gキャプチャーリボン");
    mCaptureRibbon->initWithoutIter();

    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::tryRegisterDemoCast(mCaptureActor, rIter);
    }

    MR::tryRegisterDemoActionFunctor(this, MR::Functor(this, &AstroDomeBlueStar::forceKill), nullptr);
    MR::registerDemoCast(this, "バトラー報告", rIter);
    MR::registerDemoCast(this, "ドームレクチャー１", rIter);
    MR::registerDemoCast(this, "スターピース解説前半", rIter);
    MR::registerDemoCast(this, "スターピース解説後半", rIter);
    MR::registerDemoActionFunctorDirect(this, MR::Functor(this, &AstroDomeBlueStar::waitAppear), "スターピース解説後半", nullptr);
    MR::registerDemoCast(this, "天文ドームスター帰還", rIter);
    MR::registerDemoSimpleCastAll(this);
    SphereSelectorFunction::registerTarget(this);

    MR::needStageSwitchReadAppear(this, rIter);
    MR::listenStageSwitchOnAppear(this, MR::Functor(this, &AstroDomeBlueStar::appear));

    initNerve(GET_NERVE(AstroDomeBlueStar, AstroDomeBlueStarNrvWait));

    makeActorAppeared();
}

void AstroDomeBlueStar::appear() {
    LiveActor::appear();
    setNerve(GET_NERVE(AstroDomeBlueStar, AstroDomeBlueStarNrvAppear));
}

void AstroDomeBlueStar::kill() {
    LiveActor::kill();
    mCaptureActor->kill();
}

void AstroDomeBlueStar::draw() const {
    mCaptureRibbon->draw();
}

void AstroDomeBlueStar::waitAppear() {
    LiveActor::appear();
    setNerve(GET_NERVE(AstroDomeBlueStar, AstroDomeBlueStarNrvWait));
}

void AstroDomeBlueStar::forceKill() {
    MR::forceDeleteEffectAll(this);
    LiveActor::kill();
    mCaptureActor->forceKill();
}

void AstroDomeBlueStar::control() {
    bool result = isNerve(GET_NERVE(AstroDomeBlueStar, AstroDomeBlueStarNrvGalaxySelect)) ||
                  isNerve(GET_NERVE(AstroDomeBlueStar, AstroDomeBlueStarNrvGalaxyConfirmStart)) ||
                  isNerve(GET_NERVE(AstroDomeBlueStar, AstroDomeBlueStarNrvGalaxyConfirm)) ||
                  isNerve(GET_NERVE(AstroDomeBlueStar, AstroDomeBlueStarNrvGalaxyConfirmCancel));

    if (result) {
        MR::makeMtxUpFrontPos(&mHostMtx, MR::getCamYdir(), MR::getCamZdir(), mPosition);
        mCaptureActor->setPosAll(mPosition);
    }

    if (MR::isOnGameEventFlagOffAstroDomeGuidance()) {
        return;
    }

    if (!isValidBindStart()) {
        return;
    }

    TVec2f screenPos;

    if (!MR::calcScreenPosition(&screenPos, mPosition)) {
        return;
    }

    MR::requestBlueStarGuidance();
}

bool AstroDomeBlueStar::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgAutoRushBegin(msg)) {
        return tryStartBind(pSender->mHost);
    }

    if (MR::isMsgRushCancel(msg)) {
        if (isActiveBind()) {
            return true;
        }
    } else if (MR::isMsgUpdateBaseMtx(msg)) {
        MR::setPlayerBaseMtx(mHostMtx);
        return true;
    } else if (SphereSelectorFunction::isMsgSelectEnd(msg)) {
        setNerve(GET_NERVE(AstroDomeBlueStar, AstroDomeBlueStarNrvBindEnd));
        return true;
    }

    if (SphereSelectorFunction::isMsgConfirmStart(msg)) {
        setNerve(GET_NERVE(AstroDomeBlueStar, AstroDomeBlueStarNrvGalaxyConfirmStart));
        return true;
    }

    if (SphereSelectorFunction::isMsgConfirmCancel(msg)) {
        setNerve(GET_NERVE(AstroDomeBlueStar, AstroDomeBlueStarNrvGalaxyConfirmCancel));
        return true;
    }

    return false;
}

bool AstroDomeBlueStar::tryStartBind(const LiveActor* pActor) {
    if (isNerve(GET_NERVE(AstroDomeBlueStar, AstroDomeBlueStarNrvTryStartBind))) {
        mHostMtx.set(pActor->getBaseMtx());

        setNerve(GET_NERVE(AstroDomeBlueStar, AstroDomeBlueStarNrvBindTraction));

        return true;
    }

    return false;
}

bool AstroDomeBlueStar::isActiveBind() const {
    return isNerve(&NrvAstroDomeBlueStar::AstroDomeBlueStarNrvBindTraction::sInstance) ||
           isNerve(&NrvAstroDomeBlueStar::AstroDomeBlueStarNrvBindHold::sInstance) ||
           isNerve(&NrvAstroDomeBlueStar::AstroDomeBlueStarNrvGalaxySelect::sInstance) ||
           isNerve(&NrvAstroDomeBlueStar::AstroDomeBlueStarNrvGalaxyConfirmStart::sInstance) ||
           isNerve(&NrvAstroDomeBlueStar::AstroDomeBlueStarNrvGalaxyConfirm::sInstance) ||
           isNerve(&NrvAstroDomeBlueStar::AstroDomeBlueStarNrvGalaxyConfirmCancel::sInstance);
}

bool AstroDomeBlueStar::isValidBindStart() const {
    return !isNerve(&NrvAstroDomeBlueStar::AstroDomeBlueStarNrvAppear::sInstance) && !isActiveBind() && !MR::isDemoActive();
}

void AstroDomeBlueStar::calcZoomInPos(TVec3f* pZoomVec) const {
    TVec3f cameraRelative;
    TPos3f rotateMtx;
    SphereSelectorFunction::calcHandledRotateMtx(TVec3f(0.0f, 0.0f, 0.0f), &rotateMtx);

    TVec3f yDir;
    rotateMtx.getYDir(yDir);
    cameraRelative.sub(SphereSelectorFunction::getSelectedActorTrans(), MR::getCamPos());

    SphereSelectorFunction::calcOffsetPos(pZoomVec, SphereSelectorFunction::getSelectedActorTrans(), ::cZoomInOffset, cameraRelative, yDir);
}

void AstroDomeBlueStar::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Appear", nullptr);

        MR::emitEffect(this, "TargetLight");

        MR::startSystemSE("SE_SY_GCAPTURE_APPEAR");
        MR::startSound(this, "SE_OJ_GCAPTURE_APPEAR");
    }

    MR::setNerveAtBckStopped(this, GET_NERVE(AstroDomeBlueStar, AstroDomeBlueStarNrvWait));
}

void AstroDomeBlueStar::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::showModel(this);

        MR::tryStartBck(this, "Wait", nullptr);
        MR::setBckRate(this, 0.5f);

        MR::deleteEffect(this, "Target");
        MR::deleteEffect(this, "Active");
        MR::deleteEffect(this, "Target");
        MR::deleteEffect(this, "TargetLight");
        MR::emitEffect(this, "Wait");

        MR::validateClipping(this);

        mCaptureRibbon->reset();

        getSensor("bind")->invalidate();

        mCaptureActor->setPosAll(mPosition);

        mCaptureActor->kill();

        MR::tryShowTimeoutedStarPointerGuidance();
    }

    if (isValidBindStart() && MR::isStarPointerPointing(this, WPAD_CHAN0, true, "弱")) {
        setNerve(GET_NERVE(AstroDomeBlueStar, AstroDomeBlueStarNrvPointing));
    }
}

void AstroDomeBlueStar::exePointing() {
    if (MR::isFirstStep(this)) {
        MR::setBckRate(this, 1.0f);

        MR::emitEffect(this, "Target");

        MR::invalidateClipping(this);

        MR::startCSSound("CS_STAR_POWER", nullptr, WPAD_CHAN0);
    }

    MR::requestStarPointerModeBlueStarReady(this);

    MR::startLevelSound(this, "SE_OJ_LV_MAGIC_PNT_G_POINT");

    if (MR::isOnTractTrigger()) {
        setNerve(GET_NERVE(AstroDomeBlueStar, AstroDomeBlueStarNrvTryStartBind));
        return;
    }

    if (!isValidBindStart() || !MR::isStarPointerPointing(this, WPAD_CHAN0, true, "弱")) {
        setNerve(GET_NERVE(AstroDomeBlueStar, AstroDomeBlueStarNrvWait));
    }
}

void AstroDomeBlueStar::exeTryStartBind() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "React", nullptr);

        MR::deleteEffect(this, "Target");
        MR::emitEffect(this, "Active");

        MR::startSound(this, "SE_OJ_MAGIC_PNT_G_ON");

        getSensor("bind")->validate();

        mCaptureActor->setPosAll(mPosition);

        mCaptureActor->appear();

        MR::emitEffect(mCaptureActor, "RibbonPoint");
    }

    MR::requestStarPointerModeBlueStarReady(this);

    TVec3f resultPoint;
    resultPoint.lerp(mCaptureActor->mPosition, *MR::getPlayerCenterPos(), MR::calcNerveRate(this, ::cTryStartBindFrame));
    mCaptureActor->setPosAll(resultPoint);
    mCaptureRibbon->lengthen(mPosition, resultPoint);

    MR::startLevelSound(this, "SE_OJ_LV_MAGIC_PNT_G_PULL", MR::calcDistanceToPlayer(this));
    MR::startLevelSound(this, "SE_OJ_LV_MAGIC_PNT_G_POINT");

    if (MR::isStep(this, ::cTryStartBindFrame)) {
        setNerve(GET_NERVE(AstroDomeBlueStar, AstroDomeBlueStarNrvWait));
    }
}

void AstroDomeBlueStar::exeBindTraction() {
    if (MR::isFirstStep(this)) {
        getSensor("bind")->invalidate();

        MR::deleteEffect(mCaptureActor, "RibbonPoint");
        MR::emitEffect(mCaptureActor, "RibbonBreak");
        MR::emitEffect(mCaptureActor, "LightGrow");
        MR::emitEffect(mCaptureActor, "LightSplash");

        MR::startBckPlayer("SpaceStruggle", 20);

        mBindStartMtx.set(mHostMtx);

        MR::startActorCameraTargetPlayer(this, mCameraInfo, -1);
    }

    TVec3f trans;
    mBindStartMtx.getTrans(trans);
    mCaptureActor->updateTransTraction(trans, mPosition, getNerveStep());

    mHostMtx.setTrans(mCaptureActor->mPosition);

    TVec3f trans2;
    mCaptureActor->mHostMtx.getTrans(trans2);
    mCaptureRibbon->shorten(mPosition, trans2);

    MR::startLevelSound(this, "SE_OJ_LV_MAGIC_PNT_G_PULL", MR::calcDistanceToPlayer(this));
    MR::startLevelSound(this, "SE_OJ_LV_MAGIC_PNT_G_POINT");

    if (MR::isStep(this, 30)) {
        MR::emitEffect(mCaptureActor, "Light");
    }

    if (MR::isStep(this, ::cBindTractionFrame)) {
        setNerve(GET_NERVE(AstroDomeBlueStar, AstroDomeBlueStarNrvBindHold));
    }
}

void AstroDomeBlueStar::exeBindHold() {
    s32 cancelFrame = SphereSelectorFunction::getConfirmStartCancelFrame();

    if (MR::isFirstStep(this)) {
        MR::hideModel(this);

        MR::deleteEffectAll(this);

        mCaptureRibbon->reset();

        MR::deleteEffect(mCaptureActor, "LightSplash");

        MR::startBckPlayer("SpaceWait", 20);

        mCaptureActor->setPosAll(mPosition);

        SphereSelectorFunction::selectStart();
    }

    TPos3f frontMtx;
    MR::makeMtxUpFront(&frontMtx, MR::getCamYdir(), MR::getCamZdir());
    MR::blendMtxRotateSlerp(mBindStartMtx, frontMtx, MR::calcNerveRate(this, cancelFrame), mHostMtx);

    mHostMtx.setTrans(mPosition);

    MR::setNerveAtStep(this, GET_NERVE(AstroDomeBlueStar, AstroDomeBlueStarNrvGalaxySelect), cancelFrame);
}

void AstroDomeBlueStar::exeBindEnd() {
    if (MR::isFirstStep(this)) {
        MR::forceDeleteEffect(mCaptureActor, "Light");
        MR::emitEffect(mCaptureActor, "LightBreak");

        MR::endBindAndPlayerWeakGravityLimitJump(this, TVec3f(0.0f, 0.0f, 0.0f));
    }

    setNerve(GET_NERVE(AstroDomeBlueStar, AstroDomeBlueStarNrvWait));
}

void AstroDomeBlueStar::exeGalaxySelect() {
    if (MR::isFirstStep(this)) {
        mPosition.zero();
    }
}

void AstroDomeBlueStar::exeGalaxyConfirmStart() {
    s32 frame = SphereSelectorFunction::getConfirmStartCancelFrame();

    calcZoomInPos(&mZoomPos);
    mPosition.scale(MR::calcNerveEaseOutRate(this, frame), mZoomPos);

    MR::setNerveAtStep(this, GET_NERVE(AstroDomeBlueStar, AstroDomeBlueStarNrvGalaxyConfirm), frame);
}

void AstroDomeBlueStar::exeGalaxyConfirmCancel() {
    s32 frame = SphereSelectorFunction::getConfirmStartCancelFrame();
    f32 rate = MR::calcNerveEaseInRate(this, frame);

    mPosition.scale(1.0f - rate, mZoomPos);

    MR::setNerveAtStep(this, GET_NERVE(AstroDomeBlueStar, AstroDomeBlueStarNrvGalaxySelect), frame);
}

void AstroDomeBlueStar::exeGalaxyConfirm() {
    calcZoomInPos(&mZoomPos);
    mPosition.set(mZoomPos);
}
