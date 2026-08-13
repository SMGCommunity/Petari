#include "Game/Demo/ScenarioStarter.hpp"
#include "Game/MapObj/SpinDriverCamera.hpp"
#include "Game/MapObj/SpinDriverShootPath.hpp"
#include "Game/Scene/GameSceneFunction.hpp"
#include "Game/Screen/ScenarioTitle.hpp"
#include "Game/Screen/SimpleLayout.hpp"
#include "Game/LiveActor/Nerve.hpp"
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
    : LiveActor(pName), _8C(), _90(0.0f, 1.0f, 0.0f), _9C(0.0f, 0.0f, 0.0f), _B8(), _BC(), _C0(0.0f, 1.0f, 0.0f), _CC(0.0f, 0.0f, 1.0f),
      _D8(1.0f, 0.0f, 0.0f), _E4(), _E8(), _EC(), _F0(300), _F4(), _F8(), _FC(), _100(), _104(), _108(), _10C(0.0f, 0.0f, 0.0f),
      _118(0.0f, 0.0f, 0.0f), _124() {
    _A8.set(0.0f, 0.0f, 0.0f, 1.0f);
}

ScenarioStarter::~ScenarioStarter() {
}

void ScenarioStarter::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneMapObjMovement(this);
    MR::getJMapInfoArg0NoInit(rIter, &_F0);
    initShootPath(rIter);

    _BC = new SpinDriverCamera();
    _BC->init(rIter, this);

    initHitSensor(1);
    MR::addHitSensor(this, "body", ACTMES_GROUP_LOD_LOW, 8, 0.0f, TVec3f(200.0f));
    MR::invalidateClipping(this);
    _104 = MR::createSimpleLayout("Welcome表示", "Welcome", 1);
    _104->kill();
    MR::registerDemoSimpleCastAll(_104);
    _108 = new ScenarioTitle();
    _108->initWithoutIter();
    _108->kill();
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
        _BC->start(_C0, _CC, mPosition);
        TRot3f rotMtx;
        _EC = 0.0f;
        MR::makeMtxUpFront(&(TPos3f&)rotMtx, _C0, _CC);
        rotMtx.getQuat(_A8);
    }
    if (MR::isStep(this, 10)) {
        MR::tryBlankToFrameCinemaFrame();
        _108->start();
        updateBindPosition();
        if (!MR::isNearZero(_90, 0.001f)) {
            turnBindHead(_90, 0.4f);
        }
        updateShootMotion();
        _BC->update(_90, mPosition);
        if (_F4 <= getNerveStep() && getNerveStep() <= _F8) {
            _E4 = _E8 * MR::getEaseOutValue(MR::normalize(getNerveStep(), _F4, _F8), 0.0f, 1.0f, 1.0f);
        }
        _EC = MR::pi() * MR::normalize(getNerveStep(), _F8, _FC);
        if (trySkipTrigger()) {
            MR::tryFrameToBlankCinemaFrame();
            _124 = true;
        }
        if (_124 && MR::isStopCinemaFrame()) {
            setNerve(&NrvScenarioStarter::ScenarioStarterRailMoveCanceled::sInstance);
            return;
        }
        s32 galaxyNum = 0;
        if (MR::isEqualStageName("EggStarGalaxy")) {
            galaxyNum = 20;
        } else if (MR::isEqualStageName("HoneyBeeKingdomGalaxy")) {
            galaxyNum = 7;
        } else if (MR::isEqualStageName("FactoryGalaxy")) {
            galaxyNum = 11;
        }
        if (MR::isStep(this, _F0 - galaxyNum) && isStartBgmOnWelcome()) {
            MR::stopStageBGM(0);
            MR::startStageBGMFromStageName("Game", MR::getCurrentStageName(), MR::getCurrentScenarioNo());
        }
        if (MR::isGreaterEqualStep(this, _F0)) {
            MR::endBindAndSpinDriverJump(this, _9C);
            _8C = nullptr;
            _BC->end();
            setNerve(&NrvScenarioStarter::ScenarioStarterShowWelcomeLayout::sInstance);
        }
    }
}

void ScenarioStarter::exeRailMoveCanceled() {
    if (MR::isFirstStep(this)) {
        MR::endBindAndPlayerWait(this);
        _8C = nullptr;
        _BC->cancel();
        _108->kill();
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
        _104->appear();
        if (!MR::isDead(_108)) {
            _108->end();
        }
        MR::startAnim(_104, "Appear", 0);
        MR::executeOnWelcomeAndRetry();
    }
    if (MR::isStep(this, 90)) {
        MR::forceAppearDefaultGameLayout();
    }
    if (MR::isAnimStopped(_104, 0)) {
        _104->kill();
        kill();
    }
}

void ScenarioStarter::initShootPath(const JMapInfoIter& rIter) {
    initRailRider(rIter);
    _100 = 0.0f;
    MR::getJMapInfoArg5NoInit(rIter, &_100);
    _B8 = new SpinDriverShootPath();
    _B8->init(rIter);
    _B8->calcInitPose(&_C0, &_CC, &_D8, _100);
}

void ScenarioStarter::updateBindPosition() {
    TVec3f pathPos;
    TVec3f pathDir;
    f32 nerveRate = MR::calcNerveRate(this, _F0);
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
    if (MR::isStep(this, _F4)) {
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
    if (MR::isStep(this, _FC)) {
        MR::startBckWithInterpole(_8C, "Fall", 20);
    }
}

bool ScenarioStarter::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!_8C && MR::isMsgAutoRushBegin(msg) && isNerve(&NrvScenarioStarter::ScenarioStarterWaitToStart::sInstance)) {
        _8C = MR::getSensorHost(pSender);
        setNerve(&NrvScenarioStarter::ScenarioStarterCinemaFrameBlank::sInstance);
        return true;
    }
    if (_8C && MR::isMsgUpdateBaseMtx(msg)) {
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
