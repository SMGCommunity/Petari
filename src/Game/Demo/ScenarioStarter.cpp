#include "Game/Demo/ScenarioStarter.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/SpinDriverCamera.hpp"
#include "Game/MapObj/SpinDriverShootPath.hpp"
#include "Game/Scene/GameSceneFunction.hpp"
#include "Game/Screen/ScenarioTitle.hpp"
#include "Game/Screen/SimpleLayout.hpp"
#include "Game/Util.hpp"


namespace NrvScenarioStarter {
    NEW_NERVE(ScenarioStarterWaitScenarioCameraEnd, ScenarioStarter, WaitScenarioCameraEnd);
    NEW_NERVE(ScenarioStarterWaitToStart, ScenarioStarter, WaitToStart);
    NEW_NERVE(ScenarioStarterCinemaFrameBlank, ScenarioStarter, CinemaFrameBlank);
    NEW_NERVE(ScenarioStarterRailMove, ScenarioStarter, RailMove);
    NEW_NERVE(ScenarioStarterRailMoveCanceled, ScenarioStarter, RailMoveCanceled);
    NEW_NERVE(ScenarioStarterShowWelcomeLayout, ScenarioStarter, ShowWelcomeLayout);
};  // namespace NrvScenarioStarter

ScenarioStarter::ScenarioStarter(const char* pName)
    : LiveActor(pName), _8C(), _90(0.0f, 1.0f, 0.0f), _9C(0.0f, 0.0f, 0.0f), _B8(), mSpinDriverCamera(), _C0(0.0f, 1.0f, 0.0f), _CC(0.0f, 0.0f, 1.0f),
      _D8(1.0f, 0.0f, 0.0f), _E4(), _E8(), _EC(), mFlightTime(300), mFlyMotionStartStep(), _F8(), mFallMotionStartStep(), mInitalPlayerRotation(), mWelcomeLayout(), mTitle(), _10C(0.0f, 0.0f, 0.0f),
      _118(0.0f, 0.0f, 0.0f), _124() {
    _A8.set(0.0f, 0.0f, 0.0f, 1.0f);
}

ScenarioStarter::~ScenarioStarter() {
}

void ScenarioStarter::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneMapObjMovement(this);
    MR::getJMapInfoArg0NoInit(rIter, &mFlightTime);
    initShootPath(rIter);

    mSpinDriverCamera = new SpinDriverCamera();
    mSpinDriverCamera->init(rIter, this);

    initHitSensor(1);
    MR::addHitSensor(this, "body", ACTMES_GROUP_LOD_LOW, 8, 200.0f, TVec3f(0.0f));
    MR::invalidateClipping(this);
    mWelcomeLayout = MR::createSimpleLayout("Welcome表示", "Welcome", 1);
    mWelcomeLayout->kill();
    MR::registerDemoSimpleCastAll(mWelcomeLayout);
    mTitle = new ScenarioTitle();
    mTitle->initWithoutIter();
    mTitle->kill();
    _10C.set(*MR::getPlayerPos());
    _118.set(*MR::getPlayerRotate());
    mPosition.set(_10C);
    initNerve(&NrvScenarioStarter::ScenarioStarterWaitScenarioCameraEnd::sInstance);
    if (MR::isBeginScenarioStarter()) {
        appear();
    } else {
        kill();
    }
}

void ScenarioStarter::exeWaitToStart() {
    if (MR::isFirstStep(this)) {
        _124 = false;
        MR::forceToBlankCinemaFrame();
    }
}

void ScenarioStarter::exeCinemaFrameBlank() {
    if (MR::isFirstStep(this) && !isStartBgmOnWelcome()) {
        MR::stopStageBGM(0);
        MR::startStageBGMFromStageName("Game", MR::getCurrentStageName(), MR::getCurrentScenarioNo());
    }
    MR::setNerveAtStep(this, &NrvScenarioStarter::ScenarioStarterRailMove::sInstance, 0);
}

void ScenarioStarter::exeRailMove() {
    if (MR::isFirstStep(this)) {
        calcShootMotionTime();
        MR::startBck(_8C, "SpaceFlyStart", nullptr);
        _90 = _C0;
        turnBindHead(_90, 1.0f);
        mPosition.set(MR::getRailPointPosStart(this));
        MR::endStartPosCamera();
        mSpinDriverCamera->start(_C0, _CC, mPosition);
        TPos3f rotMtx;
        _EC = 0.0f;
        MR::makeMtxUpFront(&rotMtx, _C0, _CC);
        rotMtx.getQuat(_A8);
    }
    if (MR::isStep(this, 10)) {
        MR::tryBlankToFrameCinemaFrame();
        mTitle->start();
    }
    updateBindPosition();
    if (!MR::isNearZero(_90)) {
        turnBindHead(_90, 0.4f);
    }
    updateShootMotion();
    mSpinDriverCamera->update(_90, mPosition);
    if (mFlyMotionStartStep <= getNerveStep() && getNerveStep() <= _F8) {
        _E4 = _E8 * MR::getEaseOutValue(MR::normalize(getNerveStep(), mFlyMotionStartStep, _F8), 0.0f, 1.0f, 1.0f);
    }
    _EC = MR::pi() * MR::normalize(getNerveStep(), _F8, mFallMotionStartStep);
    if (trySkipTrigger()) {
        MR::tryFrameToBlankCinemaFrame();
        _124 = true;
    }
    if (_124 && MR::isStopCinemaFrame()) {
        setNerve(&NrvScenarioStarter::ScenarioStarterRailMoveCanceled::sInstance);
        return;
    }
    s32 bgmStartTime = 0;
    if (MR::isEqualStageName("EggStarGalaxy")) {
        bgmStartTime = 20;
    } else if (MR::isEqualStageName("HoneyBeeKingdomGalaxy")) {
        bgmStartTime = 7;
    } else if (MR::isEqualStageName("FactoryGalaxy")) {
        bgmStartTime = 11;
    }
    if (MR::isStep(this, mFlightTime - bgmStartTime) && isStartBgmOnWelcome()) {
        MR::stopStageBGM(0);
        MR::startStageBGMFromStageName("Game", MR::getCurrentStageName(), MR::getCurrentScenarioNo());
    }
    if (MR::isGreaterEqualStep(this, mFlightTime)) {
        MR::endBindAndSpinDriverJump(this, _9C);
        _8C = nullptr;
        mSpinDriverCamera->end();
        setNerve(&NrvScenarioStarter::ScenarioStarterShowWelcomeLayout::sInstance);
    }
}

void ScenarioStarter::exeRailMoveCanceled() {
    if (MR::isFirstStep(this)) {
        MR::endBindAndPlayerWait(this);
        _8C = nullptr;
        mSpinDriverCamera->cancel();
        mTitle->kill();
        if (isStartBgmOnWelcome()) {
            MR::stopStageBGM(0);
            MR::startStageBGMFromStageName("Game", MR::getCurrentStageName(), MR::getCurrentScenarioNo());
        }
        setNerve(&NrvScenarioStarter::ScenarioStarterShowWelcomeLayout::sInstance);
    }
}

void ScenarioStarter::exeShowWelcomeLayout() {
    if (MR::isFirstStep(this)) {
        GameSceneFunction::notifyEndScenarioStarter();
        MR::tryFrameToScreenCinemaFrame();
        MR::startStartPosCamera(!_124);
        mWelcomeLayout->appear();
        if (!MR::isDead(mTitle)) {
            mTitle->end();
        }
        MR::startAnim(mWelcomeLayout, "Appear", 0);
        MR::executeOnWelcomeAndRetry();
    }
    if (MR::isStep(this, 90)) {
        MR::forceAppearDefaultGameLayout();
    }
    if (MR::isAnimStopped(mWelcomeLayout, 0)) {
        mWelcomeLayout->kill();
        kill();
    }
}

void ScenarioStarter::initShootPath(const JMapInfoIter& rIter) {
    initRailRider(rIter);
    mInitalPlayerRotation = 0.0f;
    MR::getJMapInfoArg5NoInit(rIter, &mInitalPlayerRotation);
    _B8 = new SpinDriverShootPath();
    _B8->init(rIter);
    _B8->calcInitPose(&_C0, &_CC, &_D8, mInitalPlayerRotation);
}

void ScenarioStarter::updateBindPosition() {
    TVec3f pathPos;
    TVec3f pathDir;
    f32 nerveRate = MR::calcNerveRate(this, mFlightTime);
    _B8->calcPosition(&pathPos, nerveRate);
    _B8->calcDirection(&pathDir, nerveRate, 0.01f);
    if (!MR::isNearZero(pathDir, 0.001f)) {
        _90 = pathDir;
    }
    _9C.set(pathPos);
    _9C -= mPosition;
    mPosition.set(pathPos);
}

/* void ScenarioStarter::updateBindActorMtx() {
    if (isNerve(&NrvScenarioStarter::ScenarioStarterRailMove::sInstance)) {
        TMtx34f mtx;
        TMtx34f mtx2;
        mtx.identity();
        mtx2.identity();
    }
} */

void ScenarioStarter::updateShootMotion() {
    if (MR::isStep(this, mFlyMotionStartStep)) {
        MR::startBckWithInterpole(_8C, "SpaceFlyLoop", 5);
    }
    if (MR::isLessStep(this, _F8)) {
        MR::startLevelSound(_8C, "SE_PM_LV_S_SPIN_DRV_FLY");
    }
    if (MR::isStep(this, _F8)) {
        MR::startBckWithInterpole(_8C, "SpaceFlyEnd", 0);
        MR::startSound(_8C, "SE_PM_S_SPIN_DRV_COOL_DOWN");
        MR::startSound(_8C, "SE_PV_JUMP_S");
    }
    if (MR::isStep(this, mFallMotionStartStep)) {
        MR::startBckWithInterpole(_8C, "Fall", 20);
    }
}

bool ScenarioStarter::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (_8C != nullptr && MR::isMsgAutoRushBegin(msg) && isNerve(&NrvScenarioStarter::ScenarioStarterWaitToStart::sInstance)) {
        _8C = MR::getSensorHost(pSender);
        setNerve(&NrvScenarioStarter::ScenarioStarterCinemaFrameBlank::sInstance);
        return true;
    }
    if (_8C == nullptr && MR::isMsgUpdateBaseMtx(msg)) {
        updateBindActorMtx();
        return true;
    }
    return false;
}

bool ScenarioStarter::trySkipTrigger() const {
    bool result;
    if (!MR::isAlreadyVisitedCurrentStageAndScenario()) {
        result = false;
    } else {
        result = false;
        if (MR::isGreaterStep(this, 10) && MR::isLessStep(this, _F8) && MR::testSystemPadTriggerDecide()) {
            result = true;
        }
    }
    return result;
}

bool ScenarioStarter::isStartBgmOnWelcome() {
    if (MR::isGalaxyRedCometAppearInCurrentStage() || MR::isGalaxyBlackCometAppearInCurrentStage()) {
        return false;
    }
    if (MR::isEqualStageName("EggStarGalaxy") && MR::getCurrentScenarioNo() == 1) {
        return true;
    }
    if (MR::isEqualStageName("HoneyBeeKingdomGalaxy") && MR::getCurrentScenarioNo() == 1) {
        return true;
    }
    if (MR::isEqualStageName("FactoryGalaxy") && MR::getCurrentScenarioNo() <= 3) {
        return true;
    }
    return false;
}

void ScenarioStarter::exeWaitScenarioCameraEnd() {
    if (!MR::isStageStateScenarioOpeningCamera()) {
        setNerve(&NrvScenarioStarter::ScenarioStarterWaitToStart::sInstance);
    }
}
