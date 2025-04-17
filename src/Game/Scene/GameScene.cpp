#include "Game/Scene/GameScene.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Screen/GamePauseSequence.hpp"
#include "Game/Screen/GameStageClearSequence.hpp"
#include "Game/Scene/SceneExecutor.hpp"
#include "Game/Scene/SceneNameObjMovementController.hpp"
#include "Game/Screen/MoviePlayingSequence.hpp"
#include "Game/Screen/OdhConverter.hpp"
#include "Game/Screen/LensFlare.hpp"
#include "Game/Util.hpp"
#include "Game/NPC/NPCFunction.hpp"
#include "Game/NPC/TalkDirector.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DrawUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/LightUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SequenceUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/SystemUtil.hpp"

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

GameScene::GameScene() : Scene("GameScene") {
    _14 = 0;
    mScenarioCamera = nullptr;
    mPauseCtrl = nullptr;
    mPauseSeq = nullptr;
    mStageClearSeq = nullptr;
    mDraw3D = true;
    _29 = 1;
}

GameScene::~GameScene() {
    MR::destroySceneMessage();
    NPCFunction::deleteNPCData();
    MR::onStarPointerSceneOut();
}

/*
void GameScene::init() {
    SceneFunction::createHioBasicNode(this);
    SceneFunction::startStageFileLoad();
    MR::requestChangeArchivePlayer(MR::isPlayerLuigi() == false);
}
*/

// GameScene::initEffect
// GameScene::drawMirror

// inline
bool GameScene::isPlayMovie() const {
    return MR::isActiveMoviePlayer() || MR::isMoviePlayingOnSequence() || isNerve(&NrvGameScene::GameScenePlayMovie::sInstance);
}

void GameScene::draw3D() const {
    if (mDraw3D) {
        if (!isPlayMovie()) {
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
    }
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
    bool ret = false;

    if (!MR::isStageDisablePauseMenu() && isNerve(&NrvGameScene::GameSceneAction::sInstance) && 
        !MR::isDemoActive() && !MR::isWipeActive() && !MR::isWipeBlank() && 
        !MR::isPlayerDead() && !MR::isPlayerDamaging()) {
        ret = true;
    }
    
    return ret;
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

    bool v3 = false;
    if (MR::isActiveMoviePlayer() || MR::isMoviePlayingOnSequence() || isNerve(&NrvGameScene::GameScenePlayMovie::sInstance)) {
        v3 = true;
    }

    if (v3) {
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
