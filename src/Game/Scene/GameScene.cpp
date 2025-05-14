#include "Game/AudioLib/AudSceneMgr.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include "Game/LiveActor/AllLiveActorGroup.hpp"
#include "Game/Map/LightFunction.hpp"
#include "Game/Map/SleepControllerHolder.hpp"
#include "Game/MapObj/StarPieceDirector.hpp"
#include "Game/NPC/EventDirector.hpp"
#include "Game/NPC/NPCFunction.hpp"
#include "Game/NPC/TalkDirector.hpp"
#include "Game/Scene/GameScene.hpp"
#include "Game/Scene/GameSceneFunction.hpp"
#include "Game/Scene/GameScenePauseControl.hpp"
#include "Game/Scene/GameSceneScenarioOpeningCameraState.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Scene/SceneNameObjMovementController.hpp"
#include "Game/Screen/CometRetryButton.hpp"
#include "Game/Screen/GamePauseSequence.hpp"
#include "Game/Screen/GameStageClearSequence.hpp"
#include "Game/Screen/LensFlare.hpp"
#include "Game/Screen/MoviePlayingSequence.hpp"
#include "Game/Screen/OdhConverter.hpp"
#include "Game/Screen/ScreenAlphaCapture.hpp"
#include "Game/System/GalaxyMapController.hpp"
#include "Game/System/GameSequenceFunction.hpp"
#include <JSystem/J3DGraphBase/J3DSys.hpp>

namespace NrvGameScene {
    NEW_NERVE(GameSceneScenarioOpeningCamera, GameScene, ScenarioOpeningCamera);
    NEW_NERVE(GameSceneScenarioStarter, GameScene, ScenarioStarter);
    NEW_NERVE(GameSceneAction, GameScene, SceneAction);
    NEW_NERVE(GameScenePauseMenu, GameScene, PauseMenu);
    NEW_NERVE(GameScenePowerStarGet, GameScene, PowerStarGet);
    NEW_NERVE(GameSceneGrandStarGet, GameScene, GrandStarGet);
    NEW_NERVE(GameSceneGameOver, GameScene, GameOver);
    NEW_NERVE(GameSceneCometRetryAfterMiss, GameScene, CometRetryAfterMiss);
    NEW_NERVE(GameSceneSaveAfterGameOver, GameScene, SaveAfterGameOver);
    NEW_NERVE(GameScenePlayMovie, GameScene, PlayMovie);
    NEW_NERVE(GameSceneTimeUp, GameScene, TimeUp);
    NEW_NERVE(GameSceneGalaxyMap, GameScene, GalaxyMap);
    NEW_NERVE(GameSceneStaffRoll, GameScene, StaffRoll);
};

GameScene::GameScene() :
    Scene("GameScene"),
    _14(0),
    mScenarioCamera(nullptr),
    mPauseCtrl(nullptr),
    mPauseSeq(nullptr),
    mStageClearSeq(nullptr),
    mDraw3D(true),
    _29(1)
{
    
}

GameScene::~GameScene() {
    MR::destroySceneMessage();
    NPCFunction::deleteNPCData();
    MR::onStarPointerSceneOut();
}

void GameScene::init() {
    SceneFunction::createHioBasicNode(this);
    SceneFunction::startStageFileLoad();
    MR::requestChangeArchivePlayer(MR::isPlayerLuigi() == false);
    initNerve(&NrvGameScene::GameSceneScenarioOpeningCamera::sInstance);
    SceneFunction::initForNameObj();
    SceneFunction::initForLiveActor();
    initEffect();
    MR::createSceneObj(SceneObj_CameraContext);
    MR::createSceneObj(SceneObj_NameObjGroup);
    MR::createSceneObj(SceneObj_PlanetGravityManager);
    MR::createSceneObj(SceneObj_MarioHolder);
    MR::createSceneObj(SceneObj_AudCameraWatcher);
    MR::createSceneObj(SceneObj_AudEffectDirector);
    MR::createSceneObj(SceneObj_AudBgmConductor);
    MR::createSceneObj(SceneObj_ResourceShare);
    MR::createSceneObj(SceneObj_EventSequencer);
    MR::createSceneObj(SceneObj_ScenePlayingResult);
    MR::createSceneObj(SceneObj_FurDrawManager);
    MR::createSceneObj(SceneObj_PlacementStateChecker);
    MR::createScreenAlphaSceneObj(0, 1.0f);
    MR::createSceneObj(SceneObj_GroupCheckManager);
    MR::createSceneObj(SceneObj_CinemaFrame);
    MR::createSceneObj(SceneObj_PlanetMapCreator);

    if (MR::isStageEpilogueDemo()) {
        MR::createSceneObj(SceneObj_StaffRoll);
    }

    if (MR::isStageEpilogueDemo() || GameSequenceFunction::isNeedMoviePlayerForStorySequenceEvent()) {
        MR::createMoviePlayingSequence();
    }

    SceneFunction::waitDoneStageFileLoad();
    MR::waitEndChangeArchivePlayer();
    SceneFunction::startActorFileLoadCommon();
    MR::suspendAsyncExecuteThread("シーン初期化");

    if (!MR::isScenarioDecided()) {
        MR::receiveAllRequestedFile();
    }
    else {
        SceneFunction::initAfterScenarioSelected();
        NPCFunction::createNPCData();
        SceneFunction::startActorFileLoadScenario();
        MR::createSceneObj(SceneObj_EventDirector);
        MR::initSceneMessage();
        MR::createSceneObj(SceneObj_CameraDirector);
        MR::createSceneObj(SceneObj_GameSceneLayoutHolder);
        MR::createSceneObj(SceneObj_StarPieceDirector);
        MR::createSceneObj(SceneObj_SceneWipeHolder);
        MR::createSceneObj(SceneObj_NamePosHolder);
        MR::createSceneObj(SceneObj_InformationObserver);

        if (MR::isGalaxyAnyCometAppearInCurrentStage()) {
            MR::createSceneObj(SceneObj_CometRetryButton);
        }

        LightFunction::initLightData();
        initSequences();
        SceneFunction::startActorPlacement();
        GameSceneFunction::loadScenarioWaveData();
        MR::createStarPiece();
        MR::completeCameraParameters();
        MR::initStarPointerGameScene();
        MR::initEventSystemAfterPlacement();
        MR::endInitLiveActorSystemInfo();
        MR::setInitializeStateAfterPlacement();
        MR::callMethodAllSceneNameObj(&NameObj::initAfterPlacement);
        SleepControlFunc::initSyncSleepController();

        while (1) {
            if (GameSceneFunction::isLoadDoneScenarioWaveData()) {
                break;
            }
        }
    }
}

void GameScene::start() {
    AudWrap::getSceneMgr()->startScene();

    if (MR::isGlobalTimerEnd()) {
        MR::forceCloseWipeCircle();
    }
    else if (!MR::hasRetryGalaxySequence()) {
        startStagePlayFirst();
    }
    else {
        startStagePlayRetry();
    }
}

void GameScene::update() {
    mPauseCtrl->updateNerve();
    updateNerve();

    bool isTimeUp = MR::isGlobalTimerEnd()
        && !isNerve(&NrvGameScene::GameSceneTimeUp::sInstance)
        && MR::isGreaterEqualStep(this, 2);

    if (isTimeUp) {
        setNerve(&NrvGameScene::GameSceneTimeUp::sInstance);
    }
}

void GameScene::draw() const {
    MR::drawInit();
    LightFunction::initLightRegisterAll();
    drawOdhCapture();
    CategoryList::execute(MR::DrawType_MiiFaceIcon);
    CategoryList::execute(MR::DrawType_MiiFaceNew);
    drawMirror();
    draw3D();
    draw2D();
    MR::reinitGX();
}

void GameScene::calcAnim() {
    if (isPlayMovie()) {
        SceneFunction::executeCalcAnimListOnPlayingMovie();
    }
    else {
        SceneFunction::executeCalcAnimList();
    }

    if (!isNerve(&NrvGameScene::GameSceneTimeUp::sInstance)) {
        CategoryList::execute(MR::CalcAnimType_AnimParticleIgnorePause);
    }

    SceneFunction::executeCalcViewAndEntryList();
}

void GameScene::notifyEndScenarioStarter() {
    setNerve(&NrvGameScene::GameSceneAction::sInstance);
}

void GameScene::requestPlayMovieDemo() {
    setNerve(&NrvGameScene::GameScenePlayMovie::sInstance);
}

void GameScene::requestStartGameOverDemo() {
    if (!isNerve(&NrvGameScene::GameSceneGameOver::sInstance)) {
        setNerve(&NrvGameScene::GameSceneGameOver::sInstance);
    }
}

void GameScene::requestEndGameOverDemo() {
    setNerve(&NrvGameScene::GameSceneSaveAfterGameOver::sInstance);
}

void GameScene::requestEndMissDemo() {
    if (MR::isExistSceneObj(SceneObj_CometRetryButton)) {
        setNerve(&NrvGameScene::GameSceneCometRetryAfterMiss::sInstance);
    }
    else {
        MR::requestChangeStageAfterMiss();
    }
}

void GameScene::requestPowerStarGetDemo() {
    setNerve(&NrvGameScene::GameScenePowerStarGet::sInstance);
}

void GameScene::requestGrandStarGetDemo() {
    setNerve(&NrvGameScene::GameSceneGrandStarGet::sInstance);
}

void GameScene::setNerveAfterPauseMenu() {
    setNerve(&NrvGameScene::GameSceneAction::sInstance);
}

bool GameScene::isExecScenarioOpeningCamera() const {
    return isNerve(&NrvGameScene::GameSceneScenarioOpeningCamera::sInstance);
}

bool GameScene::isExecScenarioStarter() const {
    return isNerve(&NrvGameScene::GameSceneScenarioStarter::sInstance);
}

bool GameScene::isExecStageClearDemo() const {
    return isNerve(&NrvGameScene::GameScenePowerStarGet::sInstance)
        || isNerve(&NrvGameScene::GameSceneGrandStarGet::sInstance);
}

void GameScene::exeScenarioOpeningCamera() {
    mScenarioCamera->update();
    SceneFunction::movementStopSceneController();
    SceneFunction::executeMovementList();

    if (mScenarioCamera->isDone()) {
        if (!MR::isBeginScenarioStarter()) {
            MR::activateDefaultGameLayout();
            MR::tryFrameToScreenCinemaFrame();
            MR::stopSubBGM(0);
            MR::stopStageBGM(0);
            MR::startStageBGMFromStageName("Game", MR::getCurrentStageName(), MR::getCurrentScenarioNo());
            setNerve(&NrvGameScene::GameSceneAction::sInstance);
        }
        else {
            setNerve(&NrvGameScene::GameSceneScenarioStarter::sInstance);
        }
    }
}

void GameScene::exeCometRetryAfterMiss() {
    CometRetryButton* pCometRetryButton = MR::getSceneObj<CometRetryButton*>(SceneObj_CometRetryButton);

    if (MR::isFirstStep(this)) {
        MR::getSceneObj<CometRetryButton*>(SceneObj_CometRetryButton)->appear();
        MR::forceOpenWipeCircle();
    }

    pCometRetryButton->movement();
}

void GameScene::exeSaveAfterGameOver() {
    if (MR::isFirstStep(this)) {
        MR::incPlayerGameOverNum();
        GameSequenceFunction::startGameDataSaveSequence(true, true);
        MR::startStarPointerModePauseMenu(this);
    }

    if (!GameSequenceFunction::isActiveSaveDataHandleSequence()) {
        MR::endStarPointerMode(this);
        MR::requestChangeSceneAfterGameOver();
    }
}

void GameScene::exePlayMovie() {
    if (!MR::isActiveMoviePlayer() && !MR::isMoviePlayingOnSequence()) {
        setNerve(&NrvGameScene::GameSceneAction::sInstance);
        SceneFunction::movementStopSceneController();
        SceneFunction::executeMovementList();
    }
    else {
        SceneFunction::executeMovementListOnPlayingMovie();
    }
}

void GameScene::exeGalaxyMap() {
    if (MR::isFirstStep(this)) {
        MR::startStarPointerModePauseMenu(this);
    }

    if (!MR::isActiveGalaxyMapLayout()) {
        MR::endStarPointerMode(this);
        setNerve(&NrvGameScene::GameSceneAction::sInstance);
    }
    else {
        CategoryList::execute(MR::MovementType_LayoutOnPause);
    }
}

void GameScene::initSequences() {
    GameStageClearSequence* pStageClearSeq = new GameStageClearSequence();
    pStageClearSeq->initWithoutIter();
    mStageClearSeq = pStageClearSeq;

    GamePauseSequence* pPauseSeq = new GamePauseSequence();
    pPauseSeq->initWithoutIter();
    mPauseSeq = pPauseSeq;

    mScenarioCamera = new GameSceneScenarioOpeningCameraState();
    mPauseCtrl = new GameScenePauseControl(this);
    mPauseCtrl->registerNervePauseMenu(&NrvGameScene::GameScenePauseMenu::sInstance);

    MR::FunctorV0M<GameScenePauseControl *, void (GameScenePauseControl::*)()> func = MR::Functor_Inline<GameScenePauseControl>(mPauseCtrl, &GameScenePauseControl::requestPauseMenuOff);
    mPauseSeq->initWindowMenu(func);
}

void GameScene::initEffect() {
    if (MR::isEqualStageName("CosmosGardenGalaxy")) {
        SceneFunction::initEffectSystem(5120, 384);
    }
    else if (MR::isEqualStageName("KoopaBattleVs1Galaxy")) {
        SceneFunction::initEffectSystem(6144, 512);
    }
    else if (MR::isEqualStageName("AstroGalaxy")) {
        SceneFunction::initEffectSystem(3072, 512);
    }
    else {
        SceneFunction::initEffectSystem(3072, 256);
    }
}

void GameScene::drawMirror() const {
    if (isDrawMirror()) {
        PSMTXCopy(MR::getMirrorCameraViewMtx(), j3dSys.mViewMtx);
        MR::loadProjectionMtx();
        CategoryList::entryDrawBufferMirror();
        GXSetAlphaUpdate(GX_FALSE);
        GXSetColorUpdate(GX_TRUE);
        CategoryList::drawOpa(MR::DrawBufferType_MirrorMapObj);
        CategoryList::drawXlu(MR::DrawBufferType_MirrorMapObj);
        CategoryList::execute(MR::DrawType_CaptureScreenIndirect);
        MR::clearZBuffer();
        GXColor c = { 0, 0, -1, 0 };
        MR::fillScreen(c);
    }
}

// inline
bool GameScene::isPlayMovie() const {
    return MR::isActiveMoviePlayer()
        || MR::isMoviePlayingOnSequence()
        || isNerve(&NrvGameScene::GameScenePlayMovie::sInstance);
}

void GameScene::draw3D() const {
    if (!mDraw3D) {
        return;
    }

    if (isPlayMovie()) {
        return;
    }

    CategoryList::execute(MR::DrawType_AstroDomeSkyClear);
    MR::loadViewMtx();
    MR::loadProjectionMtx();
    MR::setDefaultViewportAndScissor();
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_TRUE);
    GXSetDstAlpha(GX_TRUE, 0);
    CategoryList::drawOpa(MR::DrawBufferType_ClippedMapParts);
    CategoryList::execute(MR::DrawType_ClipArea);
    CategoryList::execute(MR::DrawType_FallOutFieldDraw);
    SceneFunction::executeDrawBufferListNormalOpaBeforeVolumeShadow();
    GXSetAlphaUpdate(GX_TRUE);
    CategoryList::execute(MR::DrawType_ShadowVolume);
    GXSetColorUpdate(GX_TRUE);
    GXSetDstAlpha(GX_TRUE, 0);
    SceneFunction::executeDrawBufferListNormalOpaBeforeSilhouette();
    CategoryList::execute(MR::DrawType_0x28);
    SceneFunction::executeDrawSilhouetteAndFillShadow();
    SceneFunction::executeDrawAlphaShadow();
    CategoryList::execute(MR::DrawType_BrightSun);
    MR::setLensFlareDrawSyncToken();
    GXSetAlphaUpdate(GX_FALSE);
    GXSetDstAlpha(GX_FALSE, 0);
    SceneFunction::executeDrawBufferListNormalOpa();
    SceneFunction::executeDrawListOpa();
    CategoryList::drawOpa(MR::DrawBufferType_UNK_0x18);
    SceneFunction::executeDrawBufferListNormalXlu();
    SceneFunction::executeDrawListXlu();
    CategoryList::drawXlu(MR::DrawBufferType_UNK_0x18);
    CategoryList::execute(MR::DrawType_ShadowSurface);
    CategoryList::execute(MR::DrawType_EffectDraw3D);
    CategoryList::execute(MR::DrawType_EffectDrawForBloomEffect);
    CategoryList::execute(MR::DrawType_CenterScreenBlur);
    CategoryList::execute(MR::DrawType_CaptureScreenIndirect);
    SceneFunction::executeDrawAfterIndirect();
    CategoryList::execute(MR::DrawType_0x33);
    MR::setStarPointerDrawSyncToken();
    MR::setTalkDirectorDrawSyncToken();
    SceneFunction::executeDrawImageEffect();
    GXSetDither(GX_TRUE);
    MR::loadViewMtx();
    MR::loadProjectionMtx();
    CategoryList::execute(MR::DrawType_EffectDrawAfterImageEffect);
    MR::loadViewMtx();
    MR::loadProjectionMtx();
    MR::loadLightPlayer();
    CategoryList::execute(MR::DrawType_0x24);
    CategoryList::execute(MR::DrawType_0x3B);
    MR::clearZBuffer();
}

void GameScene::draw2D() const {
    if (isPlayMovie()) {
        SceneFunction::executeDrawList2DMovie();
    }
    else {
        SceneFunction::executeDrawList2DNormal();
    }

    CategoryList::execute(MR::DrawType_CameraCover);
    CategoryList::execute(MR::DrawType_LayoutOnPause);
    mPauseSeq->draw();
    MR::drawInitFor2DModel();
    CategoryList::drawOpa(MR::DrawBufferType_DrawBufferType_0x26);
    CategoryList::drawXlu(MR::DrawBufferType_DrawBufferType_0x26);
    CategoryList::execute(MR::DrawType_WipeLayout);
}

bool GameScene::isValidScenarioOpeningCamera() const {
    return MR::hasStartAnimCamera() != false;
}

void GameScene::drawOdhCapture() const {
    if (_29) {
        if (MR::isRequestedCaptureOdhImage()) {
            MR::setPortCaptureOdhImage();
            CategoryList::execute(MR::DrawType_MessageBoardCapture);
            MR::captureOdhImage();
            MR::reinitGX();
        }
    }
}

void GameScene::startStagePlayFirst() {
    if (isValidScenarioOpeningCamera()) {
        setNerve(&NrvGameScene::GameSceneScenarioOpeningCamera::sInstance);

        if (!MR::isBeginScenarioStarter()) {
            MR::stopSubBGM(0);
            MR::stopStageBGM(0);
            MR::startStageBGMFromStageName("Game", MR::getCurrentStageName(), MR::getCurrentScenarioNo());
        }
    }
    else if (MR::isBeginScenarioStarter()) {
        MR::forceToBlankCinemaFrame();
        MR::forceOpenWipeFade();
        setNerve(&NrvGameScene::GameSceneScenarioStarter::sInstance);
    }
    else {
        if (MR::isStageBeginFadeWipe()) {
            MR::openWipeFade(180);
        }
        else if (MR::isStageBeginTitleWipe()) {
            MR::openWipeFade(30);
        }
        else if (MR::isStageBeginWithoutWipe()) {
            MR::forceOpenWipeFade();
        }
        else {
            MR::openWipeCircle(-1);
        }

        MR::stopSubBGM(0);
        MR::stopStageBGM(0);
        MR::startStageBGMFromStageName("Game", MR::getCurrentStageName(), MR::getCurrentScenarioNo());
        MR::executeOnWelcomeAndRetry();
        setNerve(&NrvGameScene::GameSceneAction::sInstance);
    }
}

void GameScene::startStagePlayRetry() {
    MR::openWipeCircle(-1);

    if (MR::isEqualStageName("SurfingLv1Galaxy") && MR::getPlayerRestartIdInfo()->_0 == 1) {
        MR::stopSubBGM(0);
        MR::stopStageBGM(0);
    }
    else if (MR::isEqualStageName("SurfingLv2Galaxy") && MR::getPlayerRestartIdInfo()->_0 == 1) {
        MR::stopSubBGM(0);
        MR::stopStageBGM(0);
    }
    else {
        MR::stopSubBGM(0);
        MR::stopStageBGM(0);
        MR::startStageBGMFromStageName("Game", MR::getCurrentStageName(), MR::getCurrentScenarioNo());
    }

    MR::executeOnWelcomeAndRetry();
    setNerve(&NrvGameScene::GameSceneAction::sInstance);
}

bool GameScene::isPermitToPauseMenu() const {
    return !MR::isStageDisablePauseMenu()
        && isNerve(&NrvGameScene::GameSceneAction::sInstance)
        && !MR::isDemoActive()
        && !MR::isWipeActive()
        && !MR::isWipeBlank()
        && !MR::isPlayerDead()
        && !MR::isPlayerDamaging();
}

void GameScene::requestShowGalaxyMap() {
    setNerve(&NrvGameScene::GameSceneGalaxyMap::sInstance);
}

void GameScene::requestStaffRoll() {
    setNerve(&NrvGameScene::GameSceneStaffRoll::sInstance);
}

bool GameScene::isDrawMirror() const {
    if (!MR::isExistMirrorCamera()) {
        return false;
    }

    if (isPlayMovie()) {
        return false;
    }

    return MR::isPlayerInAreaObj("MirrorArea");
}

void GameScene::stageClear() {
    if (MR::isFirstStep(this)) {
        if (isNerve(&NrvGameScene::GameScenePowerStarGet::sInstance)) {
            mStageClearSeq->startPowerStarGetDemo();
        }
        else if (isNerve(&NrvGameScene::GameSceneGrandStarGet::sInstance)) {
            mStageClearSeq->startGrandStarGetDemo();
        }
    }

    mStageClearSeq->movement();
    SceneFunction::movementStopSceneController();
    SceneFunction::executeMovementList();
}

void GameScene::exeStaffRoll() {
    MR::getSceneNameObjMovementController()->movement();
    CategoryList::execute(MR::MovementType_Layout);
    CategoryList::execute(MR::DrawType_LayoutDecoration);
    CategoryList::execute(MR::MovementType_WipeLayout);
}

void GameScene::exeTimeUp() {
    if (MR::isFirstStep(this)) {
        MR::startGlobalTimerTimeUp();
    }
}

void GameScene::exeGameOver() {
    SceneFunction::movementStopSceneController();
    SceneFunction::executeMovementList();
}

void GameScene::exeGrandStarGet() {
    stageClear();
}

void GameScene::exePowerStarGet() {
    stageClear();
}

void GameScene::exePauseMenu() {
    MR::isFirstStep(this);
    mPauseSeq->movement();
    CategoryList::execute(MR::MovementType_LayoutOnPause);
}

void GameScene::exeSceneAction() {
    SceneFunction::movementStopSceneController();
    SceneFunction::executeMovementList();
}

void GameScene::exeScenarioStarter() {
    SceneFunction::movementStopSceneController();
    SceneFunction::executeMovementList();
}
