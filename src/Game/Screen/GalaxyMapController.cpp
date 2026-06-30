#include "Game/Screen/GalaxyMapController.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/NPC/TalkDirector.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Screen/BackButton.hpp"
#include "Game/Screen/GalaxyMap.hpp"
#include "Game/Screen/GalaxyMapBackground.hpp"
#include "Game/Screen/GalaxyMapSelectButton.hpp"
#include "Game/Screen/PowerStarList.hpp"
#include "Game/Screen/WipeFade.hpp"
#include "Game/Util/DrawUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SequenceUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/SystemUtil.hpp"
#include <JSystem/JKernel/JKRSolidHeap.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <nw4r/lyt/drawInfo.h>

namespace {
    static const s32 sDisplayKeepFrame = 30;
    static const s32 sMapFadeinoutFrame = 12;
    static const s32 sTicoCometFadeinoutFrame = 60;
    static const s32 sTicoCometWaitFrame = 60;
    static const s32 sWaitForNewGalaxyDiscoverPhaseAstroMap = 1;
    // static const s32 sCaptureWidth = _;
    // static const s32 sCaptureHeight = _;
};  // namespace

namespace {
    NEW_NERVE(GalaxyMapControllerReadyModeMap, GalaxyMapController, ReadyModeMap);
    NEW_NERVE(GalaxyMapControllerFadeinModeMap, GalaxyMapController, FadeinModeMap);
    NEW_NERVE(GalaxyMapControllerAppearGalaxyMap, GalaxyMapController, AppearGalaxyMap);
    NEW_NERVE(GalaxyMapControllerDisplayGalaxyMap, GalaxyMapController, DisplayGalaxyMap);
    NEW_NERVE(GalaxyMapControllerAppearAstroMap, GalaxyMapController, AppearAstroMap);
    NEW_NERVE(GalaxyMapControllerDisplayAstroMap, GalaxyMapController, DisplayAstroMap);
    NEW_NERVE(GalaxyMapControllerReadyPowerStarList, GalaxyMapController, ReadyPowerStarList);
    NEW_NERVE(GalaxyMapControllerAppearPowerStarList, GalaxyMapController, AppearPowerStarList);
    NEW_NERVE(GalaxyMapControllerDisplayPowerStarList, GalaxyMapController, DisplayPowerStarList);
    NEW_NERVE(GalaxyMapControllerDisappearPowerStarList, GalaxyMapController, DisappearPowerStarList);
    NEW_NERVE(GalaxyMapControllerNewGalaxyDiscoverPhaseAstroMap, GalaxyMapController, NewGalaxyDiscoverPhaseAstroMap);
    NEW_NERVE(GalaxyMapControllerNewGalaxyDiscoverPhaseAppearGalaxyMap, GalaxyMapController, NewGalaxyDiscoverPhaseAppearGalaxyMap);
    NEW_NERVE(GalaxyMapControllerNewGalaxyDiscoverPhaseDisplayGalaxyMap, GalaxyMapController, NewGalaxyDiscoverPhaseDisplayGalaxyMap);
    NEW_NERVE(GalaxyMapControllerNewTicoGalaxyDiscoverPhaseAstroMap, GalaxyMapController, NewTicoGalaxyDiscoverPhaseAstroMap);
    NEW_NERVE(GalaxyMapControllerNewDomeDiscoverPhaseAstroMap, GalaxyMapController, NewDomeDiscoverPhaseAstroMap);
    NEW_NERVE(GalaxyMapControllerTicoCometFadeoutWhite, GalaxyMapController, TicoCometFadeoutWhite);
    NEW_NERVE(GalaxyMapControllerTicoCometWaitWhite, GalaxyMapController, TicoCometWaitWhite);
    NEW_NERVE(GalaxyMapControllerTicoCometPray, GalaxyMapController, TicoCometPray);
    NEW_NERVE(GalaxyMapControllerChallengeGalaxyDiscoverPhaseAstroMap, GalaxyMapController, ChallengeGalaxyDiscoverPhaseAstroMap);
    NEW_NERVE(GalaxyMapControllerFadeout, GalaxyMapController, Fadeout);
    NEW_NERVE(GalaxyMapControllerReadyGame, GalaxyMapController, ReadyGame);
};  // namespace

namespace {
    GalaxyMapController* getGalaxyMapController() {
        return MR::getSceneObj< GalaxyMapController >(SceneObj_GalaxyMapController);
    }
};  // namespace

GalaxyMapController::GalaxyMapController()
    : LayoutActor("GalaxyMapController", true), mMap(), mMapBackground(), mPowerStarList(), mBackButton(), mWipe(), _38(), mMode(),
      mWipeFrame(::sMapFadeinoutFrame) {
}

void GalaxyMapController::init(const JMapInfoIter& rIter) {
    MR::connectToSceneLayoutOnPause(this);

    mMap = new GalaxyMap();
    mMap->initWithoutIter();
    mMap->kill();

    mPowerStarList = new PowerStarList();
    mPowerStarList->initWithoutIter();
    mPowerStarList->kill();

    mBackButton = new BackButton("戻る", 0);
    mBackButton->initWithoutIter();
    mBackButton->kill();

    mMapSelectButton = new GalaxyMapSelectButton();
    mMapSelectButton->initWithoutIter();
    mMapSelectButton->kill();

    mMapBackground = new GalaxyMapBackground();

    mWipe = new WipeFade("フェードワイプ", Color8(0, 0, 0, 255));
    mWipe->initWithoutIter();

    {
        MR::CurrentHeapRestorer heapRestorer(MR::getSceneHeapGDDR3());

        _38 = new JUTTexture(MR::getScreenWidth() / 2, MR::getScreenHeight() / 2, GX_TF_RGB565);
    }

    if (MR::startFunctionAsyncExecuteOnMainThread(MR::Functor_Inline(this, GalaxyMapController::capture), "GalaxyMapController::capture")) {
        MR::waitForEndFunctionAsyncExecute("GalaxyMapController::capture");
    }

    initNerve(&::GalaxyMapControllerDisplayAstroMap::sInstance);
    kill();
}

void GalaxyMapController::appear() {
    LayoutActor::appear();
}

void GalaxyMapController::kill() {
    LayoutActor::kill();
    killAllLayout();
    mWipe->kill();
}

void GalaxyMapController::movement() {
    LayoutActor::movement();
    mMap->movement();
    mMapBackground->movement();

    if (!isNerve(&::GalaxyMapControllerDisappearPowerStarList::sInstance)) {
        mPowerStarList->movement();
    }

    if (!mPowerStarList->isExecCapture() && !mMap->isShowDetail()) {
        mBackButton->movement();
    }

    mWipe->movement();

    if (!mMap->isShowDetail()) {
        mMapSelectButton->movement();
    }
}

void GalaxyMapController::calcAnim() {
    LayoutActor::calcAnim();
    mMap->calcAnim();
    mMapBackground->calcAnim();
    mPowerStarList->calcAnim();
    mBackButton->calcAnim();
    mWipe->calcAnim();
    mMapSelectButton->calcAnim();
}

void GalaxyMapController::draw() const {
    if (MR::isDead(this)) {
        return;
    }

    mMapBackground->draw();
    mMap->draw();
    mPowerStarList->draw();
    mBackButton->draw();
    mMapSelectButton->draw();
    mMap->drawGalaxyInfo();

    if (isNerve(&::GalaxyMapControllerReadyGame::sInstance) && MR::isFirstStep(this)) {
        const_cast< GalaxyMapController* >(this)->capture();
    }

    mWipe->draw();
}

void GalaxyMapController::drawForCapture() {
    killAllLayout();
    mMapBackground->appear();
    mMap->setModeCapture();
    mMap->movementForCapture();

    nw4r::lyt::DrawInfo drawInfo = nw4r::lyt::DrawInfo();

    MR::copyLayoutDrawInfoWithAspect(&drawInfo, mMap, true);
    mMapBackground->calcAnim();
    mMap->calcAnim();
    MR::calcAnimLayoutWithDrawInfo(mMapBackground, drawInfo);
    mMap->calcAnimForCapture(drawInfo);
    MR::drawLayoutWithDrawInfoWithoutProjectionSetup(mMapBackground, drawInfo);
    mMap->drawForCapture(drawInfo);
}

static const GXColor sWipeColor = {0, 0, 0, 255};

void GalaxyMapController::startGalaxyMap() {
    appear();
    mMap->setModeNormal();
    setFaderParam(sWipeColor, ::sMapFadeinoutFrame);

    mMode = Mode_GalaxyMap;

    setNerve(&::GalaxyMapControllerReadyModeMap::sInstance);
}

void GalaxyMapController::startAstroMap() {
    appear();
    mMap->setModeNormal();
    setFaderParam(sWipeColor, ::sMapFadeinoutFrame);

    mMode = Mode_AstroMap;

    setNerve(&::GalaxyMapControllerReadyModeMap::sInstance);
}

void GalaxyMapController::startNewGalaxyDiscover() {
    appear();
    mMap->setModeNewGalaxyDiscover();
    setFaderParam(sWipeColor, ::sMapFadeinoutFrame);

    mMode = Mode_NewGalaxyDiscover;

    setNerve(&::GalaxyMapControllerReadyModeMap::sInstance);
}

void GalaxyMapController::startNewTicoGalaxyDiscover() {
    appear();

    mMap->setModeNewTicoGalaxyDiscover();
    setFaderParam(sWipeColor, ::sMapFadeinoutFrame);

    mMode = Mode_NewTicoGalaxyDiscover;

    setNerve(&::GalaxyMapControllerReadyModeMap::sInstance);
}

void GalaxyMapController::startNewDomeDiscover() {
    appear();

    mMap->setModeNewDomeDiscover();
    setFaderParam(sWipeColor, ::sMapFadeinoutFrame);

    mMode = Mode_NewDomeDiscover;

    mWipe->forceClose();

    setNerve(&::GalaxyMapControllerFadeinModeMap::sInstance);
}

static const GXColor sTicoCometPrayWipeColor = {255, 255, 255, 255};

void GalaxyMapController::startTicoCometPray() {
    appear();

    mMap->setModeTicoCometPray();
    setFaderParam(sTicoCometPrayWipeColor, ::sTicoCometFadeinoutFrame);

    mMode = Mode_TicoCometPray;

    mWipe->forceClose();

    setNerve(&::GalaxyMapControllerTicoCometWaitWhite::sInstance);
}

void GalaxyMapController::startChallengeGalaxyDiscover() {
    appear();

    mMap->setModeNewGalaxyDiscover();
    setFaderParam(sWipeColor, ::sMapFadeinoutFrame);

    mMode = Mode_ChallengeGalaxyDiscover;

    setNerve(&::GalaxyMapControllerReadyModeMap::sInstance);
}

void GalaxyMapController::exeReadyModeMap() {
    if (MR::isFirstStep(this)) {
        mWipe->forceOpen();
        mWipe->wipe(mWipeFrame);
    }

    if (mWipe->isClose()) {
        setNerve(&::GalaxyMapControllerFadeinModeMap::sInstance);
    }
}

void GalaxyMapController::exeFadeinModeMap() {
    if (MR::isFirstStep(this)) {
        mWipe->forceClose();
        mWipe->wipe(mWipeFrame);

        switch (mMode) {
        case Mode_AstroMap:
            mBackButton->appear();
            mMapSelectButton->startAstroMap();
        case Mode_ChallengeGalaxyDiscover:
            mMap->forceToAstroMap();
            break;
        case Mode_GalaxyMap:
            mBackButton->appear();
            mMapSelectButton->startAstroMap();
        case Mode_NewDomeDiscover:
            mMap->forceToGalaxyMap();
            break;
        }

        mMapBackground->appear();
        MR::deactivateGameSceneDraw3D();
    }

    if (mWipe->isOpen()) {
        switch (mMode) {
        case Mode_AstroMap:
            setNerve(&::GalaxyMapControllerDisplayAstroMap::sInstance);
            break;
        case Mode_GalaxyMap:
            setNerve(&::GalaxyMapControllerDisplayGalaxyMap::sInstance);
            break;
        case Mode_NewGalaxyDiscover:
            setNerve(&::GalaxyMapControllerNewGalaxyDiscoverPhaseAstroMap::sInstance);
            break;
        case Mode_NewTicoGalaxyDiscover:
            setNerve(&::GalaxyMapControllerNewTicoGalaxyDiscoverPhaseAstroMap::sInstance);
            break;
        case Mode_NewDomeDiscover:
            setNerve(&::GalaxyMapControllerNewDomeDiscoverPhaseAstroMap::sInstance);
            break;
        case Mode_TicoCometPray:
            setNerve(&::GalaxyMapControllerTicoCometPray::sInstance);
            break;
        case Mode_ChallengeGalaxyDiscover:
            setNerve(&::GalaxyMapControllerChallengeGalaxyDiscoverPhaseAstroMap::sInstance);
            break;
        }
    }
}

void GalaxyMapController::exeAppearGalaxyMap() {
    if (tryAppearGalaxyMap()) {
        setNerve(&::GalaxyMapControllerDisplayGalaxyMap::sInstance);
    }
}

void GalaxyMapController::exeDisplayGalaxyMap() {
    if (MR::isFirstStep(this)) {
        mMap->_71 = true;
    }

    if (!mMap->isShowDetail() && tryPushBackButton()) {
        setNerve(&::GalaxyMapControllerFadeout::sInstance);
    } else if (tryChangeMode()) {
        setNerveAccordingToNextModeFromMap();
    }
}

void GalaxyMapController::exeAppearAstroMap() {
    if (MR::isFirstStep(this)) {
        mMap->changeToAstroMap();
    }

    if (mMap->isTransition()) {
        return;
    }

    setNerve(&::GalaxyMapControllerDisplayAstroMap::sInstance);
}

void GalaxyMapController::exeDisplayAstroMap() {
    if (MR::isFirstStep(this)) {
        mMap->_71 = true;
    }

    if (!mMap->isShowDetail() && tryPushBackButton()) {
        setNerve(&::GalaxyMapControllerFadeout::sInstance);
    } else if (tryChangeMode()) {
        setNerveAccordingToNextModeFromMap();
    }
}

void GalaxyMapController::exeReadyPowerStarList() {
    if (MR::isFirstStep(this)) {
        mWipe->forceOpen();
        mWipe->wipe(::sMapFadeinoutFrame);
        mMap->_71 = false;

        mMapSelectButton->changeToStarList();
    }

    if (mWipe->isClose()) {
        setNerve(&::GalaxyMapControllerAppearPowerStarList::sInstance);
    }
}

void GalaxyMapController::exeAppearPowerStarList() {
    if (MR::isFirstStep(this)) {
        mWipe->forceClose();
        mWipe->wipe(::sMapFadeinoutFrame);
        mPowerStarList->appear();
        mMap->kill();
    }

    if (mPowerStarList->isAppearing()) {
        return;
    }

    if (mWipe->isOpen()) {
        setNerve(&::GalaxyMapControllerDisplayPowerStarList::sInstance);
    }
}

void GalaxyMapController::exeDisplayPowerStarList() {
    if (!mPowerStarList->isExecCapture() && tryPushBackButton()) {
        mMode = Mode_AstroMap;

        setNerve(&::GalaxyMapControllerDisappearPowerStarList::sInstance);
    } else if (tryChangeMode()) {
        setNerve(&::GalaxyMapControllerDisappearPowerStarList::sInstance);
    }
}

void GalaxyMapController::exeDisappearPowerStarList() {
    if (MR::isFirstStep(this)) {
        mWipe->forceOpen();
        mWipe->wipe(::sMapFadeinoutFrame);
    }

    if (mWipe->isClose()) {
        mMapSelectButton->startAstroMap();
        mPowerStarList->kill();

        switch (mMode) {
        case Mode_AstroMap:
            mMap->forceToAstroMap();
            break;
        case Mode_GalaxyMap:
            mMap->forceToGalaxyMap();
            break;
        }

        setNerve(&::GalaxyMapControllerFadeinModeMap::sInstance);
    }
}

void GalaxyMapController::exeNewGalaxyDiscoverPhaseAstroMap() {
    if (MR::isGreaterStep(this, ::sWaitForNewGalaxyDiscoverPhaseAstroMap)) {
        mMode = Mode_GalaxyMap;

        setNerve(&::GalaxyMapControllerNewGalaxyDiscoverPhaseAppearGalaxyMap::sInstance);
    }
}

void GalaxyMapController::exeNewGalaxyDiscoverPhaseAppearGalaxyMap() {
    if (tryAppearGalaxyMap()) {
        setNerve(&::GalaxyMapControllerNewGalaxyDiscoverPhaseDisplayGalaxyMap::sInstance);
    }
}

void GalaxyMapController::exeNewGalaxyDiscoverPhaseDisplayGalaxyMap() {
    tryDisplayAndKeyWait();
    MR::startSystemLevelSE("SE_SY_LV_GALAMAP_BLINK_NEW");
}

void GalaxyMapController::exeNewTicoGalaxyDiscoverPhaseAstroMap() {
    tryDisplayAndKeyWait();
    MR::startSystemLevelSE("SE_SY_LV_GALAMAP_BLINK_FAT");
}

void GalaxyMapController::exeNewDomeDiscoverPhaseAstroMap() {
    tryDisplayAndKeyWait();
    MR::startSystemLevelSE("SE_SY_LV_GALAMAP_BLINK_DOME");
}

void GalaxyMapController::exeTicoCometFadeoutWhite() {
    if (MR::isFirstStep(this)) {
        mWipe->forceOpen();
        mWipe->wipe(mWipeFrame);

        MR::startSystemSE("SE_DM_TICOCOMET_MORPH_WIPE_OUT");
    }

    if (mWipe->isClose()) {
        setNerve(&::GalaxyMapControllerTicoCometWaitWhite::sInstance);
    }
}

void GalaxyMapController::exeTicoCometWaitWhite() {
    MR::setNerveAtStep(this, &::GalaxyMapControllerFadeinModeMap::sInstance, ::sTicoCometWaitFrame);
}

void GalaxyMapController::exeTicoCometPray() {
    tryDisplayAndKeyWait();
}

void GalaxyMapController::exeChallengeGalaxyDiscoverPhaseAstroMap() {
    tryDisplayAndKeyWait();
    MR::startSystemLevelSE("SE_SY_LV_GALAMAP_BLINK_NEW");
}

void GalaxyMapController::exeFadeout() {
    if (MR::isFirstStep(this)) {
        mWipe->setColor(sWipeColor);
        mWipe->forceOpen();
        mWipe->wipe(::sMapFadeinoutFrame);
    }

    if (mWipe->isClose()) {
        setNerve(&::GalaxyMapControllerReadyGame::sInstance);
    }
}

void GalaxyMapController::exeReadyGame() {
    if (MR::isFirstStep(this)) {
        MR::balloonOffTalkDirector();
        killAllLayout();
        MR::activateGameSceneDraw3D();
        mWipe->forceClose();
        mWipe->wipe(::sMapFadeinoutFrame);
    }

    if (mWipe->isOpen()) {
        kill();
    }
}

void GalaxyMapController::killAllLayout() {
    mMap->kill();
    mMapBackground->kill();
    mPowerStarList->kill();
    mBackButton->kill();
    mMapSelectButton->kill();
}

bool GalaxyMapController::tryPushBackButton() {
    bool isTriggerB = MR::testSystemTriggerB();

    if (mBackButton->_24 || isTriggerB) {
        if (isTriggerB) {
            MR::startSystemSE("SE_SY_GALAXY_DECIDE_CANCEL");
        }

        return true;
    }

    return false;
}

bool GalaxyMapController::tryChangeMode() {
    if (mMapSelectButton->isDecidedList()) {
        mMode = Mode_PowerStarList;

        return true;
    }

    if (!mMapSelectButton->isPointingAnything() && !mMap->isPointingAnything() && !mBackButton->isPointing() && MR::testCorePadTriggerA(WPAD_CHAN0)) {
        switch (mMode) {
        case Mode_AstroMap:
            MR::startSystemSE("SE_SY_GALAMAP_PAGE_CHANGE");
            MR::startSystemSE("SE_SY_GALAMAP_CH_PITCH_UP");
            break;
        case Mode_GalaxyMap:
            MR::startSystemSE("SE_SY_GALAMAP_PAGE_CHANGE");
            MR::startSystemSE("SE_SY_GALAMAP_CH_PITCH_DOWN");
            break;
        }

        switch (mMode) {
        case Mode_AstroMap:
            mMode = Mode_GalaxyMap;
            return true;
        case Mode_GalaxyMap:
            mMode = Mode_AstroMap;
            return true;
        }

        // FIXME: Missing duplicate unconditional branch instruction at end of switch statement.
    }

    return false;
}

bool GalaxyMapController::tryAppearGalaxyMap() {
    if (MR::isFirstStep(this)) {
        mMap->changeToGalaxyMap();
    }

    if (!mMap->isTransition()) {
        return true;
    }

    return false;
}

bool GalaxyMapController::tryDisplayAndKeyWait() {
    if (MR::isFirstStep(this)) {
        mMap->_71 = false;
    }

    if (MR::isStep(this, ::sDisplayKeepFrame)) {
        mMap->dispIconAButton();
    }

    if (MR::isGreaterStep(this, ::sDisplayKeepFrame) && MR::testCorePadTriggerA(WPAD_CHAN0)) {
        setNerve(&::GalaxyMapControllerFadeout::sInstance);

        return true;
    }

    return false;
}

void GalaxyMapController::setNerveAccordingToNextModeFromMap() {
    switch (mMode) {
    case Mode_PowerStarList:
        setNerve(&::GalaxyMapControllerReadyPowerStarList::sInstance);
        break;
    case Mode_GalaxyMap:
        setNerve(&::GalaxyMapControllerAppearGalaxyMap::sInstance);
        break;
    case Mode_AstroMap:
        setNerve(&::GalaxyMapControllerAppearAstroMap::sInstance);
        break;
    }
}

void GalaxyMapController::setFaderParam(GXColor color, int frame) {
    mWipe->setColor(color);
    mWipeFrame = frame;
}

void GalaxyMapController::capture() {
    Mtx44 projectionMtx;
    f32 halfHeight = MR::getScreenHeight() * 0.5f;
    C_MTXOrtho(projectionMtx, halfHeight, -halfHeight, -304.0f, 304.0f, -1000.0f, 1000.0f);
    GXSetProjection(projectionMtx, GX_ORTHOGRAPHIC);
    drawForCapture();
    _38->capture(0, 0, GX_TF_RGB565, true, 0);
}

namespace MR {
    void startGalaxyMapLayout() {
        MR::requestShowGalaxyMap();
        ::getGalaxyMapController()->startGalaxyMap();
    }

    void startAstroMapLayout() {
        MR::requestShowGalaxyMap();
        ::getGalaxyMapController()->startAstroMap();
    }

    void startAstroMapLayoutForNewGalaxyDiscover() {
        MR::requestShowGalaxyMap();
        ::getGalaxyMapController()->startNewGalaxyDiscover();
    }

    void startAstroMapLayoutForNewTicoGalaxyDiscover() {
        MR::requestShowGalaxyMap();
        ::getGalaxyMapController()->startNewTicoGalaxyDiscover();
    }

    void startAstroMapLayoutForNewDomeDiscover() {
        MR::requestShowGalaxyMap();
        ::getGalaxyMapController()->startNewDomeDiscover();
    }

    void startGalaxyMapLayoutForTicoComet() {
        MR::requestShowGalaxyMap();
        ::getGalaxyMapController()->startTicoCometPray();
    }

    void startAstroMapLayoutForChallengeGalaxyDiscover() {
        MR::requestShowGalaxyMap();
        ::getGalaxyMapController()->startChallengeGalaxyDiscover();
    }

    bool isActiveGalaxyMapLayout() {
        return !MR::isDead(::getGalaxyMapController());
    }

    const ResTIMG* getGalaxyMapResTIMG() {
        return ::getGalaxyMapController()->_38->mTIMG;
    }
};  // namespace MR
