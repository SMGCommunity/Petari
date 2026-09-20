#include "Game/Scene/LogoScene.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Screen/IsbnManager.hpp"
#include "Game/Screen/LogoFader.hpp"
#include "Game/Screen/SimpleLayout.hpp"
#include "Game/System/GameSequenceFunction.hpp"
#include "Game/System/GameSystemFunction.hpp"
#include "Game/System/Language.hpp"
#include "Game/System/MainLoopFramework.hpp"
#include "Game/Util/DrawUtil.hpp"
#include "Game/Util/FileUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "Game/Util/SystemUtil.hpp"
#include <JSystem/JKernel/JKRDvdRipper.hpp>

const wchar_t* pIsbnNumber = L"0000000000000";
const wchar_t* pRegistNumber = L"0000000000";
const wchar_t* pOtherNumber = L"0000000";

namespace {
    const s32 FADEINOUT_FRAME = 30;
    const s32 CENSORSHIP_DISPLAY_FRAME = 180;
    const s32 STRAP_DISPLAY_MIN_FRAME = 450;
    const s32 STRAP_DISPLAY_MAX_FRAME = 1200;

    NEW_NERVE(LogoSceneCensorshipFadein, LogoScene, CensorshipFadein);
    NEW_NERVE(LogoSceneCensorshipDisplay, LogoScene, CensorshipDisplay);
    NEW_NERVE(LogoSceneCensorshipFadeout, LogoScene, CensorshipFadeout);
    NEW_NERVE(LogoSceneStrapFadein, LogoScene, StrapFadein);
    NEW_NERVE(LogoSceneStrapDisplay, LogoScene, StrapDisplay);
    NEW_NERVE(LogoSceneStrapFadeout, LogoScene, StrapFadeout);
    NEW_NERVE(LogoSceneMountGameData, LogoScene, MountGameData);
    NEW_NERVE(LogoSceneWaitReadDoneSystemArchive, LogoScene, WaitReadDoneSystemArchive);
    NEW_NERVE(LogoSceneDeactive, LogoScene, Deactive);
};  // namespace

LogoScene::LogoScene() : Scene("LogoScene"), mIsbnManager(), mStrapLayout(), mLogoFader() {
    MainLoopFramework::getManager()->mUseVFilter = false;
    MainLoopFramework::getManager()->mUseAlpha = false;
}

LogoScene::~LogoScene() {
    MainLoopFramework::getManager()->mUseVFilter = true;
    MainLoopFramework::getManager()->mUseAlpha = true;
}

void LogoScene::init() {
    if (MR::isEqualString(MR::getCurrentRegionPrefix(), "Cn")) {
        initNerve(GET_NERVE_ANON(LogoSceneCensorshipFadein));
    } else {
        initNerve(GET_NERVE_ANON(LogoSceneStrapFadein));
    }

    SceneFunction::createHioBasicNode(this);
    SceneFunction::initForNameObj();
    MR::createSceneObj(SceneObj_CameraContext);
    MR::createSceneObj(SceneObj_NameObjGroup);
    initLayout();
}

void LogoScene::update() {
    GameSystemFunction::restartControllerLeaveWatcher();
    updateNerve();
    SceneFunction::executeMovementList();
}

void LogoScene::calcAnim() {
    SceneFunction::executeCalcAnimList();
    SceneFunction::executeCalcViewAndEntryList2D();
}

void LogoScene::draw() const {
    MR::drawInit();

    GXColor fillColor;
    fillColor.r = 255;
    fillColor.g = 255;
    fillColor.b = 255;
    fillColor.a = 255;

    MR::fillScreen(fillColor);
    MR::clearZBuffer();
    MR::drawInitFor2DModel();

    bool isCensorship = isNerve(GET_NERVE_ANON(LogoSceneCensorshipFadein)) || isNerve(GET_NERVE_ANON(LogoSceneCensorshipDisplay)) ||
                        isNerve(GET_NERVE_ANON(LogoSceneCensorshipFadeout));

    if (isCensorship) {
        MR::setupDrawForNW4RLayout(1.0f, true);
        mIsbnManager->draw();
    }

    CategoryList::drawOpa(MR::DrawBufferType_Model3DFor2D);
    CategoryList::drawXlu(MR::DrawBufferType_Model3DFor2D);
    CategoryList::execute(MR::DrawType_Layout);
    CategoryList::execute(MR::DrawType_EffectDraw2D);
    CategoryList::execute(MR::DrawType_EffectDrawFor2DModel);
    CategoryList::execute(MR::DrawType_CometScreenFilter);
    CategoryList::execute(MR::DrawType_WipeLayout);
}

bool LogoScene::isDisplayStrapRemineder() const {
    return isNerve(GET_NERVE_ANON(LogoSceneDeactive)) || isNerve(GET_NERVE_ANON(LogoSceneCensorshipFadein)) ||
           isNerve(GET_NERVE_ANON(LogoSceneCensorshipDisplay)) || isNerve(GET_NERVE_ANON(LogoSceneCensorshipFadeout)) ||
           isNerve(GET_NERVE_ANON(LogoSceneStrapFadein)) || isNerve(GET_NERVE_ANON(LogoSceneStrapDisplay)) ||
           isNerve(GET_NERVE_ANON(LogoSceneStrapFadeout)) || isNerve(GET_NERVE_ANON(LogoSceneWaitReadDoneSystemArchive));
}

void LogoScene::exeCensorshipFadein() {
    if (MR::isFirstStep(this)) {
        mLogoFader->mMaxStep = FADEINOUT_FRAME;
    }

    mIsbnManager->calc(true);

    if (tryFadeinLayout()) {
        setNerve(GET_NERVE_ANON(LogoSceneCensorshipDisplay));
    }
}

void LogoScene::exeCensorshipDisplay() {
    if (MR::isFirstStep(this)) {
        mIsbnManager->reset();
    }

    mIsbnManager->calc(true);

    if (MR::isGreaterStep(this, CENSORSHIP_DISPLAY_FRAME)) {
        setNerve(GET_NERVE_ANON(LogoSceneCensorshipFadeout));
    }
}

void LogoScene::exeCensorshipFadeout() {
    mIsbnManager->calc(true);

    if (tryFadeoutLayout()) {
        setNerve(GET_NERVE_ANON(LogoSceneStrapFadein));
    }
}

void LogoScene::exeStrapFadein() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(mStrapLayout, "Strap", 0);
        MR::setAnimFrameAndStop(mStrapLayout, 0.0f, 0);

        mLogoFader->mMaxStep = FADEINOUT_FRAME;
    }

    if (tryFadeinLayout(mStrapLayout)) {
        setNerve(GET_NERVE_ANON(LogoSceneStrapDisplay));
    }
}

void LogoScene::exeStrapDisplay() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(mStrapLayout, "Strap", 0);
    }

    if (MR::isStep(this, STRAP_DISPLAY_MAX_FRAME - 1)) {
        MR::setAnimRate(mStrapLayout, 0.0f, 0);
    }

    if (MR::isGreaterStep(this, STRAP_DISPLAY_MIN_FRAME) &&
        (MR::testCorePadTriggerAnyWithoutHome(WPAD_CHAN0) || MR::isGreaterEqualStep(this, STRAP_DISPLAY_MAX_FRAME))) {
        setNerve(GET_NERVE_ANON(LogoSceneStrapFadeout));
    }
}

void LogoScene::exeStrapFadeout() {
    if (tryFadeoutLayout(mStrapLayout)) {
        setNerve(GET_NERVE_ANON(LogoSceneWaitReadDoneSystemArchive));
    }
}

void LogoScene::exeMountGameData() {
    if (MR::isFirstStep(this)) {
        GameSequenceFunction::startPreLoadSaveDataSequence();
    }

    if (GameSequenceFunction::isActiveSaveDataHandleSequence()) {
        return;
    }

    setNerve(GET_NERVE_ANON(LogoSceneDeactive));
}

void LogoScene::exeWaitReadDoneSystemArchive() {
    if (GameSystemFunction::isDoneLoadSystemArchive()) {
        setNerve(GET_NERVE_ANON(LogoSceneMountGameData));
    }
}

void LogoScene::exeDeactive() {
    if (MR::isFirstStep(this)) {
        GameSequenceFunction::notifyToGameSequenceProgressToEndScene();
    }
}

void LogoScene::initLayout() {
    mStrapLayout = MR::createSimpleLayout("ストラップ着用画面", "WiiRemoteStrap", 1);
    mStrapLayout->kill();

    mLogoFader = new LogoFader("ロゴフェーダ");
    mLogoFader->initWithoutIter();
    mLogoFader->setBlank();
    mLogoFader->appear();
    MR::connectToSceneLayout(mLogoFader);

    if (!MR::isEqualString(MR::getCurrentRegionPrefix(), "Cn")) {
        return;
    }

    mIsbnManager =
        IsbnManager::create(MR::loadToMainRAM("CnSimpChinese/LayoutData/ISBNLayoutData.arc", nullptr, nullptr, JKRDvdRipper::ALLOC_DIRECTION_FORWARD),
                            &MR::NewDeleteAllocator::sAllocator);
    mIsbnManager->setNumber(pIsbnNumber, pRegistNumber, pOtherNumber);

    if (MR::isScreen16Per9()) {
        mIsbnManager->setAdjustRate(0.75f, 1.0f);
    }
}

bool LogoScene::tryFadeinLayout(LayoutActor* pActor) {
    if (MR::isFirstStep(this)) {
        pActor->appear();
    }

    if (tryFadeinLayout()) {
        return true;
    }

    return false;
}

bool LogoScene::tryFadeoutLayout(LayoutActor* pActor) {
    if (tryFadeoutLayout()) {
        pActor->kill();

        return true;
    }

    return false;
}

bool LogoScene::tryFadeinLayout() {
    if (MR::isFirstStep(this)) {
        mLogoFader->startFadeIn();
    }

    if (mLogoFader->isFadeEnd()) {
        return true;
    }

    return false;
}

bool LogoScene::tryFadeoutLayout() {
    if (MR::isFirstStep(this)) {
        mLogoFader->startFadeOut();
    }

    if (mLogoFader->isFadeEnd()) {
        return true;
    }

    return false;
}
