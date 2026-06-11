#include "Game/System/StarPointerOnOffController.hpp"
#include "Game/Screen/StarPointerDirector.hpp"
#include "Game/Screen/StarPointerLayout.hpp"
#include "Game/System/NerveExecutor.hpp"
#include "Game/Util/StarPointerUtil.hpp"

namespace {
    // static const f32 hDebugPrintX =
    // static const f32 hDebugPrintY =

    // debug symbol
    const char* getStringStarPointerMode(s32 mode);

    // to help clean up the code so it is not a nightmare to read
    inline StarPointerDirector* getStarPointerDirector() {
        return StarPointerFunction::getStarPointerDirector();
    }

    inline StarPointerLayout* getStarPointerLayout(s32 channel) {
        return getStarPointerDirector()->getStarPointerLayout(channel);
    }
};  // namespace

StarPointerOnOffController::StarPointerOnOffController() : NerveExecutor("スターポインタON/OFF制御"), _8(false), mMode(-1) {
    for (s32 idx = 0; idx < 16; idx++) {
        mRequests[idx] = new RequestInfo();
    }

    for (RequestInfo** request = mRequests.begin(); request < mRequests.end(); request++) {
        (*request)->mRequester = nullptr;
        (*request)->mMode = -1;
    }

    for (s32 idx = 0; idx < StarPointerMode_Count; idx++) {
        mModeCounter[idx] = 0;
        mRequested[idx] = false;
    }
}

void StarPointerOnOffController::update() {
    updateMode();
    for (s32 idx = 0; idx < StarPointerMode_Count; idx++) {
        mRequested[idx] = false;
    }
}

void StarPointerOnOffController::setStateToTitle(void* pRequester) {
    clearInfo();
    incModeCounter(pRequester, StarPointerMode_Title);
}

void StarPointerOnOffController::setStateToFileSelect(void* pRequester) {
    clearInfo();
    incModeCounter(pRequester, StarPointerMode_FileSelect);
}

void StarPointerOnOffController::setStateToBase(void* pRequester) {
    clearInfo();
}

void StarPointerOnOffController::popState(void* pRequester) {
    decModeCounter(pRequester);
}

void StarPointerOnOffController::requestMode(void* pRequester, StarPointerModeEnum mode) {
    mRequested[mode] = true;
}

void StarPointerOnOffController::setupHomeButton() {
    ::getStarPointerLayout(WPAD_CHAN0)->hide();
    ::getStarPointerLayout(WPAD_CHAN1)->hide();
    MR::deactiveStarPointerGuidance();
    MR::disableStarPointerShootStarPiece();
    ::getStarPointerLayout(WPAD_CHAN0)->validate();
    ::getStarPointerLayout(WPAD_CHAN1)->validate();
}

void StarPointerOnOffController::setupErrorWindow() {
    ::getStarPointerLayout(WPAD_CHAN0)->hide();
    ::getStarPointerLayout(WPAD_CHAN1)->hide();
    MR::deactiveStarPointerGuidance();
    MR::disableStarPointerShootStarPiece();
    ::getStarPointerLayout(WPAD_CHAN0)->validate();
    ::getStarPointerLayout(WPAD_CHAN1)->validate();
}

void StarPointerOnOffController::setupLogo() {
    ::getStarPointerLayout(WPAD_CHAN0)->hide();
    ::getStarPointerLayout(WPAD_CHAN1)->hide();
    ::getStarPointerDirector()->startStarPointer();
    ::getStarPointerLayout(WPAD_CHAN0)->invalidate();
    ::getStarPointerLayout(WPAD_CHAN1)->invalidate();
    MR::deactiveStarPointerGuidance();
    MR::disableStarPointerShootStarPiece();
}

void StarPointerOnOffController::setupTitle() {
    ::getStarPointerLayout(WPAD_CHAN0)->hide();
    ::getStarPointerLayout(WPAD_CHAN1)->hide();
    ::getStarPointerDirector()->startStarPointer();
    ::getStarPointerLayout(WPAD_CHAN0)->invalidate();
    ::getStarPointerLayout(WPAD_CHAN1)->invalidate();
    MR::deactiveStarPointerGuidance();
    MR::disableStarPointerShootStarPiece();
}

void StarPointerOnOffController::setupFileSelect() {
    ::getStarPointerLayout(WPAD_CHAN0)->show();
    ::getStarPointerLayout(WPAD_CHAN1)->show();
    ::getStarPointerDirector()->startFingerPointer();
    ::getStarPointerLayout(WPAD_CHAN0)->validate();
    ::getStarPointerLayout(WPAD_CHAN1)->validate();
    MR::activeStarPointerGuidance();
    MR::disableStarPointerShootStarPiece();
}

void StarPointerOnOffController::setupPictureBook() {
    ::getStarPointerLayout(WPAD_CHAN0)->show();
    ::getStarPointerLayout(WPAD_CHAN1)->show();
    ::getStarPointerDirector()->startFingerPointer();
    ::getStarPointerLayout(WPAD_CHAN0)->validate();
    ::getStarPointerLayout(WPAD_CHAN1)->invalidate();
    MR::deactiveStarPointerGuidance();
    MR::disableStarPointerShootStarPiece();
}

void StarPointerOnOffController::setupDemo() {
    ::getStarPointerLayout(WPAD_CHAN0)->hide();
    ::getStarPointerLayout(WPAD_CHAN1)->hide();
    ::getStarPointerDirector()->startStarPointer();
    ::getStarPointerLayout(WPAD_CHAN0)->invalidate();
    ::getStarPointerLayout(WPAD_CHAN1)->invalidate();
    MR::deactiveStarPointerGuidance();
    MR::disableStarPointerShootStarPiece();
}

void StarPointerOnOffController::setupDemoWithStarPointer() {
    ::getStarPointerLayout(WPAD_CHAN0)->show();
    ::getStarPointerLayout(WPAD_CHAN1)->show();
    ::getStarPointerDirector()->startStarPointer();
    ::getStarPointerLayout(WPAD_CHAN0)->validate();
    ::getStarPointerLayout(WPAD_CHAN1)->validate();
    MR::activeStarPointerGuidance();
    MR::disableStarPointerShootStarPiece();
}

void StarPointerOnOffController::setupDemoWithHandPointerFinger() {
    ::getStarPointerLayout(WPAD_CHAN0)->show();
    ::getStarPointerLayout(WPAD_CHAN1)->show();
    ::getStarPointerDirector()->startFingerPointer();
    ::getStarPointerLayout(WPAD_CHAN0)->validate();
    ::getStarPointerLayout(WPAD_CHAN1)->validate();
    MR::activeStarPointerGuidance();
    MR::disableStarPointerShootStarPiece();
}

void StarPointerOnOffController::setupDemoMarioDeath() {
    ::getStarPointerLayout(WPAD_CHAN0)->show();
    ::getStarPointerLayout(WPAD_CHAN1)->show();
    ::getStarPointerDirector()->startStarPointer();
    ::getStarPointerLayout(WPAD_CHAN0)->invalidate();
    ::getStarPointerLayout(WPAD_CHAN1)->invalidate();
    MR::activeStarPointerGuidance();
    MR::disableStarPointerShootStarPiece();
}

void StarPointerOnOffController::setup1PInvalid2PValid() {
    ::getStarPointerLayout(WPAD_CHAN0)->show();
    ::getStarPointerLayout(WPAD_CHAN1)->show();
    ::getStarPointerDirector()->startStarPointer();
    ::getStarPointerLayout(WPAD_CHAN0)->validate();
    ::getStarPointerLayout(WPAD_CHAN1)->validate();
    MR::deactiveStarPointerGuidance();
    MR::enableStarPointerShootStarPiece();
}

void StarPointerOnOffController::setupMarioLauncher() {
    ::getStarPointerLayout(WPAD_CHAN0)->hide();
    ::getStarPointerLayout(WPAD_CHAN1)->show();
    ::getStarPointerDirector()->startFingerPointer();
    ::getStarPointerLayout(WPAD_CHAN0)->invalidate();
    ::getStarPointerLayout(WPAD_CHAN1)->invalidate();
    MR::activeStarPointerGuidance();
    MR::disableStarPointerShootStarPiece();
}

void StarPointerOnOffController::setupChooseYesNo() {
    ::getStarPointerLayout(WPAD_CHAN0)->show();
    ::getStarPointerLayout(WPAD_CHAN1)->show();
    ::getStarPointerDirector()->startFingerPointer();
    ::getStarPointerLayout(WPAD_CHAN0)->validate();
    ::getStarPointerLayout(WPAD_CHAN1)->validate();
    MR::activeStarPointerGuidance();
    MR::disableStarPointerShootStarPiece();
}

void StarPointerOnOffController::setupPauseMenu() {
    ::getStarPointerLayout(WPAD_CHAN0)->show();
    ::getStarPointerLayout(WPAD_CHAN1)->show();
    ::getStarPointerDirector()->startFingerPointer();
    ::getStarPointerLayout(WPAD_CHAN0)->validate();
    ::getStarPointerLayout(WPAD_CHAN1)->validate();
    MR::deactiveStarPointerGuidance();
    MR::disableStarPointerShootStarPiece();
}

void StarPointerOnOffController::setupScenarioSelectScene() {
    ::getStarPointerLayout(WPAD_CHAN0)->show();
    ::getStarPointerLayout(WPAD_CHAN1)->show();
    ::getStarPointerDirector()->startFingerPointer();
    ::getStarPointerLayout(WPAD_CHAN0)->validate();
    ::getStarPointerLayout(WPAD_CHAN1)->validate();
    MR::activeStarPointerGuidance();
    MR::disableStarPointerShootStarPiece();
}

void StarPointerOnOffController::setupBlueStar() {
    ::getStarPointerLayout(WPAD_CHAN0)->show();
    ::getStarPointerLayout(WPAD_CHAN1)->show();
    ::getStarPointerDirector()->startHandPointer();
    ::getStarPointerLayout(WPAD_CHAN0)->validate();
    ::getStarPointerLayout(WPAD_CHAN1)->validate();
    MR::activeStarPointerGuidance();
    MR::enableStarPointerShootStarPiece();
}

void StarPointerOnOffController::setupBlueStarReady() {
    ::getStarPointerLayout(WPAD_CHAN0)->show();
    ::getStarPointerLayout(WPAD_CHAN1)->show();
    ::getStarPointerDirector()->startHandPointer();
    ::getStarPointerLayout(WPAD_CHAN0)->validate();
    ::getStarPointerLayout(WPAD_CHAN1)->validate();
    MR::activeStarPointerGuidance();
    MR::enableStarPointerShootStarPiece();
}

void StarPointerOnOffController::setupPowerStarGetDemo() {
    ::getStarPointerLayout(WPAD_CHAN0)->hide();
    ::getStarPointerLayout(WPAD_CHAN1)->hide();
    ::getStarPointerDirector()->startStarPointer();
    ::getStarPointerLayout(WPAD_CHAN0)->invalidate();
    ::getStarPointerLayout(WPAD_CHAN1)->invalidate();
    MR::deactiveStarPointerGuidance();
    MR::disableStarPointerShootStarPiece();
}

void StarPointerOnOffController::setupStarPieceTarget() {
    ::getStarPointerLayout(WPAD_CHAN0)->hide();
    ::getStarPointerLayout(WPAD_CHAN1)->hide();
    ::getStarPointerDirector()->startFingerPointer();
    ::getStarPointerLayout(WPAD_CHAN0)->validate();
    ::getStarPointerLayout(WPAD_CHAN1)->validate();
    MR::activeStarPointerGuidance();
    MR::disableStarPointerShootStarPiece();
}

void StarPointerOnOffController::setupSphereSelectorFinger() {
    ::getStarPointerLayout(WPAD_CHAN0)->show();
    ::getStarPointerLayout(WPAD_CHAN1)->show();
    ::getStarPointerDirector()->startFingerPointer();
    ::getStarPointerLayout(WPAD_CHAN0)->validate();
    ::getStarPointerLayout(WPAD_CHAN1)->validate();
    MR::activeStarPointerGuidance();
    MR::disableStarPointerShootStarPiece();
}

void StarPointerOnOffController::setupSphereSelectorOnReaction() {
    ::getStarPointerLayout(WPAD_CHAN0)->show();
    ::getStarPointerLayout(WPAD_CHAN1)->show();
    ::getStarPointerDirector()->startHandPointerReactionWithCrossCursor();
    ::getStarPointerLayout(WPAD_CHAN0)->validate();
    ::getStarPointerLayout(WPAD_CHAN1)->validate();
    MR::activeStarPointerGuidance();
    MR::disableStarPointerShootStarPiece();
}

void StarPointerOnOffController::setupBigBubble() {
    ::getStarPointerLayout(WPAD_CHAN0)->show();
    ::getStarPointerLayout(WPAD_CHAN1)->show();
    ::getStarPointerDirector()->startStarPointerNozzle();
    ::getStarPointerLayout(WPAD_CHAN0)->validate();
    ::getStarPointerLayout(WPAD_CHAN1)->validate();
    MR::activeStarPointerGuidance();
    MR::disableStarPointerShootStarPiece();
    ::getStarPointerDirector()->mIsAllowP2StarPieceShot = true;
}

void StarPointerOnOffController::setupCommandStream() {
    ::getStarPointerLayout(WPAD_CHAN0)->show();
    ::getStarPointerLayout(WPAD_CHAN1)->show();
    ::getStarPointerDirector()->startStarPointer();
    ::getStarPointerLayout(WPAD_CHAN0)->validate();
    ::getStarPointerLayout(WPAD_CHAN1)->validate();
    MR::activeStarPointerGuidance();
    MR::disableStarPointerShootStarPiece();
    ::getStarPointerDirector()->mIsAllowP2StarPieceShot = true;
}

void StarPointerOnOffController::setupGame() {
    ::getStarPointerLayout(WPAD_CHAN0)->show();
    ::getStarPointerLayout(WPAD_CHAN1)->show();
    ::getStarPointerDirector()->startStarPointer();
    ::getStarPointerLayout(WPAD_CHAN0)->validate();
    ::getStarPointerLayout(WPAD_CHAN1)->validate();
    MR::activeStarPointerGuidance();
    MR::enableStarPointerShootStarPiece();
}

void StarPointerOnOffController::setupBase() {
    ::getStarPointerLayout(WPAD_CHAN0)->hide();
    ::getStarPointerLayout(WPAD_CHAN1)->hide();
    ::getStarPointerDirector()->startStarPointer();
    ::getStarPointerLayout(WPAD_CHAN0)->invalidate();
    ::getStarPointerLayout(WPAD_CHAN1)->invalidate();
    MR::deactiveStarPointerGuidance();
    MR::disableStarPointerShootStarPiece();
}

void StarPointerOnOffController::updateMode() {
    if (::getStarPointerLayout(WPAD_CHAN0) == nullptr) {
        return;
    }

    updateNerve();

    for (s32 mode = 0; mode < StarPointerMode_Count; mode++) {
        if (mModeCounter[mode] > 0 || mRequested[mode]) {
            if (mode != mMode) {
                mMode = mode;

                switch (mMode) {
                case StarPointerMode_HomeButton:
                    setupHomeButton();
                    break;
                case StarPointerMode_ErrorWindow:
                    setupErrorWindow();
                    break;
                case StarPointerMode_SaveLoad:
                    setupPauseMenu();
                    break;
                case StarPointerMode_Logo:
                    setupLogo();
                    break;
                case StarPointerMode_Title:
                    setupTitle();
                    break;
                case StarPointerMode_FileSelect:
                    setupFileSelect();
                    break;
                case StarPointerMode_PauseMenu:
                    setupPauseMenu();
                    break;
                case StarPointerMode_ChooseYesNo:
                    setupChooseYesNo();
                    break;
                case StarPointerMode_PictureBook:
                    setupPictureBook();
                    break;
                case StarPointerMode_Demo:
                    setupDemo();
                    break;
                case StarPointerMode_DemoWithStarPointer:
                    setupDemoWithStarPointer();
                    break;
                case StarPointerMode_DemoWithHandPointerFinger:
                    setupDemoWithHandPointerFinger();
                    break;
                case StarPointerMode_DemoMarioDeath:
                    setupDemoMarioDeath();
                    break;
                case StarPointerMode_PowerStarGetDemo:
                    setupPowerStarGetDemo();
                    break;
                case StarPointerMode_1PInvalid2PValid:
                    setup1PInvalid2PValid();
                    break;
                case StarPointerMode_ScenarioSelectScene:
                    setupScenarioSelectScene();
                    break;
                case StarPointerMode_SphereSelectorOnReaction:
                    setupSphereSelectorOnReaction();
                    break;
                case StarPointerMode_SphereSelectorFinger:
                    setupSphereSelectorFinger();
                    break;
                case StarPointerMode_MarioLauncher:
                    setupMarioLauncher();
                    break;
                case StarPointerMode_BlueStar:
                    setupBlueStar();
                    break;
                case StarPointerMode_BlueStarReady:
                    setupBlueStarReady();
                    break;
                case StarPointerMode_StarPieceTarget:
                    setupStarPieceTarget();
                    break;
                case StarPointerMode_BigBubble:
                    setupBigBubble();
                    break;
                case StarPointerMode_CommandStream:
                    setupCommandStream();
                    break;
                case StarPointerMode_Base:
                    setupBase();
                    break;
                case StarPointerMode_Game:
                    setupGame();
                    break;
                }
            }
            return;
        }
    }

    mMode = -1;
    setupBase();
}

void StarPointerOnOffController::clearInfo() {
    for (s32 mode = 0; mode < StarPointerMode_Count; mode++) {
        mModeCounter[mode] = 0;
    }

    for (RequestInfo** request = mRequests.begin(); request < mRequests.end(); request++) {
        (*request)->mRequester = nullptr;
        (*request)->mMode = -1;
    }

    updateMode();
}

void StarPointerOnOffController::incModeCounter(void* pRequester, s32 mode) {
    if (mode == -1) {
        return;
    }

    for (RequestInfo** request = mRequests.begin(); request < mRequests.end(); request++) {
        if ((*request)->mRequester == nullptr) {
            if (mode == StarPointerMode_DemoMarioDeath && mModeCounter[mode] != 0) {
                return;
            }

            (*request)->mRequester = pRequester;
            (*request)->mMode = mode;

            mModeCounter[mode]++;
            updateMode();
            return;
        }
    }
}

void StarPointerOnOffController::decModeCounter(void* pRequester) {
    for (RequestInfo** request = mRequests.end() - 1; request >= mRequests.begin(); request--) {
        if ((*request)->mRequester == pRequester) {
            mModeCounter[(*request)->mMode]--;
            for (; request < mRequests.end() - 1; request++) {
                *(*request) = *(*(request + 1));
                (*(request + 1))->mRequester = nullptr;
                (*(request + 1))->mMode = -1;
            }
            updateMode();
            return;
        }
    }
}
