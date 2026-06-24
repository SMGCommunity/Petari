#include "Game/MapObj/AstroDomeBlueStar.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/SphereSelector.hpp"
#include "Game/MapObj/GCapture.hpp"
#include "Game/MapObj/GCaptureRibbon.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util.hpp"

namespace {
    Vec cZoomInOffset = {0, 0, 0};  // 0 because idk which are the values (yet)
}

AstroDomeBlueStar::CaptureActor::CaptureActor() : LiveActor("キャプチャ用アクター") {
    _8C.identity();
}

void AstroDomeBlueStar::CaptureActor::init(const JMapInfoIter& rIter) {
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

void AstroDomeBlueStar::CaptureActor::updateTransTraction(const TVec3f& rBasePos, const TVec3f& rTarget, s32 step) {
    mPosition.lerp(rBasePos, rTarget, MR::getEaseInOutValue(step / 90.0f, 0.0f, 1.0f, 1.0f));

    TVec3f dir;
    dir.sub(rTarget, rBasePos);
    if (MR::normalizeOrZero(&dir)) {
        _8C.setTrans(mPosition);
        return;
    }

    f32 dist = MR::getLinerValue(step < 30 ? step : 30, 0.0f, 150.0f, 30.0f);
    TVec3f pos;
    if (mPosition.distance(rTarget) < dist) {
        pos.set(rTarget);
    } else {
        pos.add(mPosition, dir.multInLine(dist));
    }
    MR::makeMtxUpNoSupportPos(&_8C, dir, pos);
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

AstroDomeBlueStar::AstroDomeBlueStar(const char* pName)
    : LiveActor(pName), mCaptureRibbon(nullptr), _F0(0), mCaptureActor(nullptr), mZoomPos(gZeroVec) {
    _90.identity();
    _C0.identity();
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
    MR::initActorCamera(this, rIter, &_F0);
    initSound(6, false);
    MR::initStarPointerTarget(this, 200.0f, TVec3f::makeZeroVec());
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
    MR::listenStageSwitchOnAppear(this, MR::Functor(this, &AstroDomeBlueStar::control));
    initNerve(&NrvAstroDomeBlueStar::AstroDomeBlueStarNrvWait::sInstance);
    makeActorAppeared();
}

void AstroDomeBlueStar::appear() {
    LiveActor::appear();
    setNerve(&NrvAstroDomeBlueStar::AstroDomeBlueStarNrvAppear::sInstance);
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
    setNerve(&NrvAstroDomeBlueStar::AstroDomeBlueStarNrvWait::sInstance);
}

void AstroDomeBlueStar::forceKill() {
    MR::forceDeleteEffectAll(this);
    LiveActor::kill();
    mCaptureActor->forceKill();
}

void AstroDomeBlueStar::control() {
    bool result = isNerve(&NrvAstroDomeBlueStar::AstroDomeBlueStarNrvGalaxySelect::sInstance) ||
                  isNerve(&NrvAstroDomeBlueStar::AstroDomeBlueStarNrvGalaxyConfirmStart::sInstance) ||
                  isNerve(&NrvAstroDomeBlueStar::AstroDomeBlueStarNrvGalaxyConfirm::sInstance) ||
                  isNerve(&NrvAstroDomeBlueStar::AstroDomeBlueStarNrvGalaxyConfirmCancel::sInstance);
    if (result) {
        TVec3f mCamZDir = MR::getCamZdir();
        TVec3f mCamYDir = MR::getCamYdir();
        MR::makeMtxUpFrontPos(&_90, mCamYDir, mCamZDir, mPosition);

        mCaptureActor->setPosAll(mPosition);
    }
    if (!MR::isOnGameEventFlagOffAstroDomeGuidance()) {
        if (isValidBindStart()) {
            TVec2f screenPos;
            if (MR::calcScreenPosition(&screenPos, mPosition)) {
                MR::requestBlueStarGuidance();
            }
        }
    }
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
        MR::setPlayerBaseMtx(_90);
        return true;
    } else if (SphereSelectorFunction::isMsgSelectEnd(msg)) {
        setNerve(&NrvAstroDomeBlueStar::AstroDomeBlueStarNrvBindEnd::sInstance);
        return true;
    }
    if (SphereSelectorFunction::isMsgConfirmStart(msg)) {
        setNerve(&NrvAstroDomeBlueStar::AstroDomeBlueStarNrvGalaxyConfirmStart::sInstance);
        return true;
    }
    if (SphereSelectorFunction::isMsgConfirmCancel(msg)) {
        setNerve(&NrvAstroDomeBlueStar::AstroDomeBlueStarNrvGalaxyConfirmCancel::sInstance);
        return true;
    }
    return false;
}

bool AstroDomeBlueStar::tryStartBind(const LiveActor* pActor) {
    if (isNerve(&NrvAstroDomeBlueStar::AstroDomeBlueStarNrvTryStartBind::sInstance)) {
        _90.setInline(pActor->getBaseMtx());
        setNerve(&NrvAstroDomeBlueStar::AstroDomeBlueStarNrvBindTraction::sInstance);
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

void AstroDomeBlueStar::calcZoomInPos(TVec3f* zoomVec) const {
    TVec3f cameraRelative;
    TVec3f vec;
    TPos3f pos;
    SphereSelectorFunction::calcHandledRotateMtx(TVec3f(0.0f, 0.0f, 0.0f), &pos);
    pos.getYDirInline(vec);
    cameraRelative.sub(SphereSelectorFunction::getSelectedActorTrans(), MR::getCamPos());

    SphereSelectorFunction::calcOffsetPos(zoomVec, SphereSelectorFunction::getSelectedActorTrans(), TVec3f(cZoomInOffset), cameraRelative, vec);
}

void AstroDomeBlueStar::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Appear", nullptr);
        MR::emitEffect(this, "TargetLight");
        MR::startSystemSE("SE_SY_GCAPTURE_AREA");
        MR::startSound(this, "SE_OJ_GCAPTURE_AREA");
    }
    MR::setNerveAtBckStopped(this, &NrvAstroDomeBlueStar::AstroDomeBlueStarNrvWait::sInstance);
}

void AstroDomeBlueStar::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::showModel(this);
        MR::tryStartBck(this, "Wait", nullptr);
        MR::setBckRate(this, 0.5);
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
    if (isValidBindStart() && MR::isStarPointerPointing(this, 0, true, "弱")) {
        setNerve(&NrvAstroDomeBlueStar::AstroDomeBlueStarNrvPointing::sInstance);
    }
}

void AstroDomeBlueStar::exePointing() {
    if (MR::isFirstStep(this)) {
        MR::setBckRate(this, 1.0f);
        MR::emitEffect(this, "Target");
        MR::invalidateClipping(this);
        MR::startCSSound("CS_STAR_POWER", nullptr, 0);
    }
    MR::requestStarPointerModeBlueStarReady(this);
    MR::startLevelSound(this, "SE_OJ_LV_MAGIC_PNT_G_POINT");
    if (MR::isOnTractTrigger()) {
        setNerve(&NrvAstroDomeBlueStar::AstroDomeBlueStarNrvTryStartBind::sInstance);
    } else if (!isValidBindStart() || !MR::isStarPointerPointing(this, 0, true, "弱")) {
        setNerve(&NrvAstroDomeBlueStar::AstroDomeBlueStarNrvWait::sInstance);
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
    resultPoint.lerp(mCaptureActor->mPosition, *MR::getPlayerCenterPos(), MR::calcNerveRate(this, 30));
    mCaptureActor->setPosAll(resultPoint);
    mCaptureRibbon->lengthen(mPosition, resultPoint);
    MR::startLevelSound(this, "SE_OJ_LV_MAGIC_PNT_G_PULL", MR::calcDistanceToPlayer(this));
    MR::startLevelSound(this, "SE_OJ_LV_MAGIC_PNT_G_POINT");
    if (MR::isStep(this, 30)) {
        setNerve(&NrvAstroDomeBlueStar::AstroDomeBlueStarNrvWait::sInstance);
    }
}

void AstroDomeBlueStar::exeBindTraction() {
    if (MR::isFirstStep(this)) {
        getSensor("bind")->invalidate();
        MR::deleteEffect(this, "RibbonPoint");
        MR::emitEffect(this, "RibbonBreak");
        MR::emitEffect(mCaptureActor, "LightGrow");
        MR::emitEffect(mCaptureActor, "LightSplash");
        MR::startBckPlayer("SpaceStruggle", 20);
        _C0.setInline(_90);
        MR::startActorCameraTargetPlayer(this, _F0, -1);
    }
    TVec3f trans;
    _C0.getTrans(trans);
    mCaptureActor->updateTransTraction(trans, mPosition, getNerveStep());
    _90.setTrans(mCaptureActor->mPosition);
    TVec3f trans2;
    mCaptureActor->_8C.getTrans(trans2);
    mCaptureRibbon->shorten(mPosition, trans2);
    MR::startLevelSound(this, "SE_OJ_LV_MAGIC_PNT_G_PULL", MR::calcDistanceToPlayer(this));
    MR::startLevelSound(this, "SE_OJ_LV_MAGIC_PNT_G_POINT");
    if (MR::isStep(this, 30)) {
        MR::emitEffect(mCaptureActor, "Light");
    }
    if (MR::isStep(this, 90)) {
        setNerve(&NrvAstroDomeBlueStar::AstroDomeBlueStarNrvBindHold::sInstance);
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
    TVec3f mCamZDir = MR::getCamZdir();
    TVec3f mCamYDir = MR::getCamYdir();
    TPos3f frontMtx;
    MR::makeMtxUpFront(&frontMtx, mCamYDir, mCamZDir);
    MR::blendMtxRotateSlerp(_C0, frontMtx, MR::calcNerveRate(this, cancelFrame), _90);
    _90.setTrans(mPosition);
    MR::setNerveAtStep(this, &NrvAstroDomeBlueStar::AstroDomeBlueStarNrvGalaxySelect::sInstance, cancelFrame);
}

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

void AstroDomeBlueStar::exeGalaxyConfirm() {
    calcZoomInPos(&mZoomPos);
    mPosition.set(mZoomPos);
}

AstroDomeBlueStar::CaptureActor::~CaptureActor() {
}
AstroDomeBlueStar::~AstroDomeBlueStar() {
}
