#include "Game/Demo/StarReturnDemoStarter.hpp"
#include "Game/Demo/ReturnDemoRailMove.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/MapObj/PowerStar.hpp"
#include "Game/MapObj/SpinDriverPathDrawer.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Screen/StageResultInformer.hpp"
#include "Game/System/GameSequenceFunction.hpp"

namespace {
    const char* cDemoMovePartName = "移動";
    const char* cDemoWaitPartName = "ウェイト";
}  // namespace

StarReturnDemoStarter::StarReturnDemoStarter(const char* pName)
    : LiveActor(pName), mReturnDemoRailMove(), mStageResultInformer(), mPosition(gZeroVec), mPowerStar(), mLuma(), mHair(), mFrame() {
    mTransform.identity();
}

namespace NrvStarReturnDemoStarter {
    NEW_NERVE(StarReturnDemoStarterNrvAppearWait, StarReturnDemoStarter, AppearWait);
    NEW_NERVE(StarReturnDemoStarterNrvMove, StarReturnDemoStarter, Move);
    NEW_NERVE(StarReturnDemoStarterNrvFlyWaitPowerStar, StarReturnDemoStarter, FlyWaitPowerStar);
    NEW_NERVE(StarReturnDemoStarterNrvFlyWaitGrandStar, StarReturnDemoStarter, FlyWaitGrandStar);
    NEW_NERVE(StarReturnDemoStarterNrvFall, StarReturnDemoStarter, Fall);
    NEW_NERVE(StarReturnDemoStarterNrvLand, StarReturnDemoStarter, Land);
    NEW_NERVE(StarReturnDemoStarterNrvWait, StarReturnDemoStarter, Wait);
    NEW_NERVE(StarReturnDemoStarterNrvStageResult, StarReturnDemoStarter, StageResult);
    NEW_NERVE(StarReturnDemoStarterNrvFadeOut, StarReturnDemoStarter, FadeOut);
    NEW_NERVE(StarReturnDemoStarterNrvStageResultAfter, StarReturnDemoStarter, StageResultAfter);
}  // namespace NrvStarReturnDemoStarter

StarReturnDemoStarter::~StarReturnDemoStarter() {
}

void StarReturnDemoStarter::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneMapObjMovement(this);
    MR::invalidateClipping(this);

    mStageResultInformer = new StageResultInformer();
    mStageResultInformer->initWithoutIter();

    mPowerStar = reinterpret_cast< PowerStar* >(
        createSubModel("スターモデル", GameSequenceFunction::isGrandStarAtResultSequence() ? "GrandStar" : "PowerStar", false));
    mLuma = createSubModel("チコモデル", "SpinTico", false);

    if (!MR::isPlayerLuigi()) {
        mHair = createSubModel("髪の毛モデル", "MarioHair", true);
    }

    mReturnDemoRailMove = new ReturnDemoRailMove(this, mPowerStar, rIter, GameSequenceFunction::isGrandStarAtResultSequence(), &mTransform);

    tryRegisterDemo("グランドスター帰還[２回目以降]", rIter);
    tryRegisterDemo("パワースター帰還", rIter);
    tryRegisterDemo("天文ドームスター帰還", rIter);

    initNerve(&NrvStarReturnDemoStarter::StarReturnDemoStarterNrvAppearWait::sInstance);
    makeActorDead();
}

void StarReturnDemoStarter::appear() {
    LiveActor::appear();
    mReturnDemoRailMove->posToStart();
    mPowerStar->setupColorAtResultSequence(mPowerStar, GameSequenceFunction::isGrandStarAtResultSequence());

    if (MR::isDemoActive("天文ドームスター帰還")) {
        MR::onDrawSpinDriverPathAtOpa();
    }

    setNerve(&NrvStarReturnDemoStarter::StarReturnDemoStarterNrvAppearWait::sInstance);
}

void StarReturnDemoStarter::kill() {
    LiveActor::kill();
    mLuma->kill();

    if (mHair != nullptr) {
        mHair->kill();
    }

    if (MR::isDrawSpinDriverPathAtOpa()) {
        MR::offDrawSpinDriverPathAtOpa();
    }
}

void StarReturnDemoStarter::makeArchiveList(NameObjArchiveListCollector* pArchiveList, const JMapInfoIter&) {
    if (GameSequenceFunction::isGrandStarAtResultSequence()) {
        pArchiveList->addArchive("GrandStar");
    }

    pArchiveList->addArchive("SpinTico");
    pArchiveList->addArchive("MarioHair");
    pArchiveList->addArchive("SpinDriverPath");
}

void StarReturnDemoStarter::control() {
    if (!isNerve(&NrvStarReturnDemoStarter::StarReturnDemoStarterNrvStageResultAfter::sInstance)) {
        MR::setPlayerBaseMtx(mTransform);
    }
}

ModelObj* StarReturnDemoStarter::createSubModel(const char* pName, const char* pModelName, bool isPlayerDecoration) {
    ModelObj* pSubModel =
        isPlayerDecoration ? MR::createModelObjPlayerDecoration(pName, pModelName, mTransform) : MR::createModelObjNpc(pName, pModelName, mTransform);

    if (MR::getLightNumMax(pSubModel) > 0) {
        MR::initLightCtrl(pSubModel);
    }

    MR::invalidateClipping(pSubModel);
    pSubModel->kill();

    return pSubModel;
}

void StarReturnDemoStarter::tryRegisterDemo(const char* pDemoName, const JMapInfoIter& rIter) {
    if (MR::isDemoExist(pDemoName) && MR::tryRegisterDemoCast(this, pDemoName, rIter)) {
        MR::tryRegisterDemoCast(mPowerStar, pDemoName, rIter);
        MR::tryRegisterDemoCast(mLuma, pDemoName, rIter);

        if (mHair != nullptr) {
            MR::tryRegisterDemoCast(mHair, pDemoName, rIter);
        }
    }
}

void StarReturnDemoStarter::tryStartStageResult() {
    if (MR::isDemoPartLastStep(cDemoWaitPartName)) {
        if (GameSequenceFunction::hasStageResultSequence()) {
            mStageResultInformer->appear();
            MR::requestMovementOn(mStageResultInformer);
        }

        MR::pauseTimeKeepDemo(this);
        setNerve(&NrvStarReturnDemoStarter::StarReturnDemoStarterNrvStageResult::sInstance);
    }
}

void StarReturnDemoStarter::tryStartResultWait() {
    if (MR::isBckOneTimeAndStoppedPlayer()) {
        MR::startBckPlayer("ResultWait", reinterpret_cast< char* >(nullptr));
    } else if (mHair != nullptr) {
        if (mFrame == 21) {
            MR::hidePlayerJoint("Cap0");
        }

        if (mFrame == 221) {
            MR::showPlayerJoint("Cap0");
        }

        mFrame++;
    }
}

const char* playerAnimName() {
    return MR::isPlayerLuigi() ? "LuigiResultWaitStart" : "ResultWaitStart";
}

void StarReturnDemoStarter::exeAppearWait() {
    if (MR::isFirstStep(this)) {
        MR::hidePlayer();
        mPowerStar->appear();
        MR::hideModelAndOnCalcAnim(mPowerStar);

        if (MR::isEqualStageName("AstroGalaxy")) {
            MR::startStageBGM("MBGM_GALAXY_24", false);
        }
    }

    if (MR::isStep(this, 30)) {
        MR::emitEffect(mPowerStar, "DemoFlyLightDemo");
        MR::startSound(mPowerStar, "SE_PM_DEMO_RETURN_LIGHT", -1, -1);
    }

    MR::setNerveAtStep(this, &NrvStarReturnDemoStarter::StarReturnDemoStarterNrvMove::sInstance, 45);
}

void StarReturnDemoStarter::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::showPlayer();
        MR::showModel(mPowerStar);
        mReturnDemoRailMove->start();
    }

    const char* pDemoName = cDemoMovePartName;
    int partStep = MR::getDemoPartStep(pDemoName) - 44;
    int totalStep = MR::getDemoPartTotalStep(pDemoName) - 45;
    mReturnDemoRailMove->update(partStep, totalStep);
    mPowerStar->requestPointLightAtResultSequence(mPowerStar);
    MR::startLevelSoundPlayer("SE_PM_LV_SPIN_DRV_FLY", -1);

    if (MR::isDemoPartLastStep(pDemoName)) {
        GameSequenceFunction::isGrandStarAtResultSequence() ?
            setNerve(&NrvStarReturnDemoStarter::StarReturnDemoStarterNrvFlyWaitGrandStar::sInstance) :
            setNerve(&NrvStarReturnDemoStarter::StarReturnDemoStarterNrvFlyWaitPowerStar::sInstance);
    }
}

void StarReturnDemoStarter::exeFlyWaitPowerStar() {
    if (MR::isFirstStep(this)) {
        mReturnDemoRailMove->posToEnd();
    }

    PowerStar::requestPointLightAtResultSequence(mPowerStar);

    int brakeFrame = mReturnDemoRailMove->getDemoFlyBrakeFrame();
    if (MR::isLessStep(this, 115 - brakeFrame)) {
        MR::startLevelSound(mPowerStar, "SE_OJ_LV_RET_POW_STAR_FLY", -1, -1, -1);
    }

    if (MR::isStep(this, 115 - brakeFrame)) {
        MR::startSound(mPowerStar, "SE_OJ_RET_POW_STAR_LEAVE", -1, -1);
    }

    if (MR::isBckStoppedPlayer()) {
        setNerve(&NrvStarReturnDemoStarter::StarReturnDemoStarterNrvFall::sInstance);
    }
}

void StarReturnDemoStarter::exeFlyWaitGrandStar() {
    if (MR::isFirstStep(this)) {
        mReturnDemoRailMove->posToEnd();
    }

    PowerStar::requestPointLightAtResultSequence(mPowerStar);

    if (MR::isLessStep(this, MR::getBckFrameMaxPlayer("ResultFlyGrandStarEnd") - mReturnDemoRailMove->getDemoFlyBrakeFrame())) {
        MR::startLevelSound(mPowerStar, "SE_OJ_LV_RET_POW_STAR_FLY", -1, -1, -1);
    }

    if (MR::isBckOneTimeAndStoppedPlayer()) {
        MR::startBckPlayer("ResultFlyGrandStarFlyAway", reinterpret_cast< char* >(nullptr));
        MR::startBck(mPowerStar, "ResultFlyGrandStarFlyAway", nullptr);
        MR::shakeCameraWeak();
        MR::tryRumblePadWeak(this, 0);
    }

    if (MR::isStep(this, 360)) {
        setNerve(&NrvStarReturnDemoStarter::StarReturnDemoStarterNrvFall::sInstance);
    }
}

void StarReturnDemoStarter::exeFall() {
    TVec3f down = TVec3f(0.0f, -1.0f, 0.0f);

    TVec3f position;
    mTransform.getTransInline(position);

    if (MR::isFirstStep(this)) {
        MR::startSoundPlayer("SE_PV_DM_ASTRO_RET_LANDING", -1);
        MR::startBckPlayer("Fall", reinterpret_cast< char* >(nullptr));
        mPowerStar->kill();

        TVec3f scaled;
        scaled.scale(3000.0f, down);
        MR::getFirstPolyOnLineToMap(&mPosition, nullptr, position, scaled);
    }

    TVec3f offset;
    offset.scale(1.8f * getNerveStep(), down);

    offset.y = MR::max(offset.y, -35.0f);
    position.addInline(offset);

    if (position.y < mPosition.y) {
        position.set(mPosition);

        setNerve(&NrvStarReturnDemoStarter::StarReturnDemoStarterNrvLand::sInstance);
    }

    mTransform.setPos(position);
}

void StarReturnDemoStarter::exeLand() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("Land", reinterpret_cast< char* >(nullptr));
    }

    if (MR::isBckStoppedPlayer()) {
        ModelObj* lumaModel = mLuma;
        lumaModel->appear();
        MR::startAllAnim(lumaModel, playerAnimName());

        ModelObj* hairModel = mHair;
        if (hairModel) {
            hairModel->appear();
            MR::startAllAnim(hairModel, playerAnimName());
        }

        setNerve(&NrvStarReturnDemoStarter::StarReturnDemoStarterNrvWait::sInstance);
    }
}

void StarReturnDemoStarter::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer(playerAnimName(), reinterpret_cast< char* >(nullptr));

        mFrame = 0;
        if (MR::isEqualStageName("AstroGalaxy")) {
            MR::stopStageBGM(90);
        }
    }

    tryStartResultWait();
    tryStartStageResult();
}

void StarReturnDemoStarter::exeStageResult() {
    tryStartResultWait();

    if (MR::isDead(mStageResultInformer) && MR::isEqualStringCase(MR::getPlayerCurrentBckName(), "ResultWait")) {
        GameSequenceFunction::hasNextDemoForStorySequenceEvent() ?
            setNerve(&NrvStarReturnDemoStarter::StarReturnDemoStarterNrvFadeOut::sInstance) :
            setNerve(&NrvStarReturnDemoStarter::StarReturnDemoStarterNrvStageResultAfter::sInstance);
    }
}

void StarReturnDemoStarter::exeFadeOut() {
    if (MR::isFirstStep(this)) {
        MR::closeWipeFade(60);
    }

    if (!MR::isWipeActive()) {
        setNerve(&NrvStarReturnDemoStarter::StarReturnDemoStarterNrvStageResultAfter::sInstance);
    }
}

void StarReturnDemoStarter::exeStageResultAfter() {
    if (MR::isFirstStep(this)) {
        mReturnDemoRailMove->offPathDraw();
        MR::resumeTimeKeepDemo(this);

        if (MR::isEqualStageName("AstroGalaxy")) {
            MR::startCurrentStageBGM();
            MR::overlayWithPreviousScreen(3);
            MR::endStartPosCamera();
        }
    }

    if (!MR::isTimeKeepDemoActive()) {
        kill();
    }
}
