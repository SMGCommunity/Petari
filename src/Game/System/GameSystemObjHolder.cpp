#include "Game/System/GameSystemObjHolder.hpp"
#include "Game/Effect/ParticleResourceHolder.hpp"
#include "Game/NameObj/NameObjRegister.hpp"
#include "Game/Screen/CaptureScreenDirector.hpp"
#include "Game/Screen/ScreenPreserver.hpp"
#include "Game/Screen/StarPointerDirector.hpp"
#include "Game/System/AudSystemWrapper.hpp"
#include "Game/System/ErrorMessageArchive.hpp"
#include "Game/System/FileLoader.hpp"
#include "Game/System/FunctionAsyncExecutor.hpp"
#include "Game/System/HeapMemoryWatcher.hpp"
#include "Game/System/Language.hpp"
#include "Game/System/MainLoopFramework.hpp"
#include "Game/System/MessageHolder.hpp"
#include "Game/System/NANDManager.hpp"
#include "Game/System/RenderMode.hpp"
#include "Game/System/ResourceHolderManager.hpp"
#include "Game/System/WPadHolder.hpp"
#include "Game/Util/FileUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/SingletonHolder.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include <JSystem/JKernel/JKRExpHeap.hpp>
#include <JSystem/JUtility/JUTVideo.hpp>
#include <revolution/vi.h>

GameSystemObjHolder::GameSystemObjHolder()
    : mObjHolder(), mParticleResHolder(), mRenderModeObj(), mCaptureScreenDirector(), mScreenPreserver(), mAudioSystem(), mWPadHolder(),
      mFunctionAsyncExecutor(), mMessageHolder(), mStarPointerDirector(), mRandom(0), mLanguage(MR::getDecidedLanguageFromIPL()) {
    initDvd();
    initNAND();
    initNameObj();
    MR::createAndAddArchive((void*)cErrorMessageArchive, MR::getStationedHeapNapa(), "ErrorMessageArchive.arc");
}

void GameSystemObjHolder::initAfterStationedResourceLoaded() {
    SingletonHolder< NameObjRegister >::get()->setCurrentHolder(mObjHolder);
    mParticleResHolder = new ParticleResourceHolder("/ParticleData/Effect.arc");
    mMessageHolder->initGameData();
    MR::createStarPointerLayout();
}

void GameSystemObjHolder::initMessageResource() {
    JKRArchive* pArchive = nullptr;
    JKRHeap* pHeap = nullptr;
    MR::getMountedArchiveAndHeap("ErrorMessageArchive.arc", &pArchive, &pHeap);

    char systemMessagePath[128];
    const char* languagePrefix = MR::getCurrentLanguagePrefix();
    snprintf(systemMessagePath, sizeof(systemMessagePath), "/%s/MessageData/System.arc", languagePrefix);

    void* pSystemMessageArchive = MR::decompressFileFromArchive(pArchive, systemMessagePath, nullptr, 0);
    MR::createAndAddArchive(pSystemMessageArchive, pHeap, "/Memory/SystemMessage.arc");

    void* pErrorMessageWindowArchive = MR::decompressFileFromArchive(pArchive, "/LayoutData/ErrorMessageWindow.arc", pHeap, 32);
    MR::createAndAddArchive(pErrorMessageWindowArchive, pHeap, "/Memory/ErrorMessageWindow.arc");

    MR::createAndAddLayoutHolderRawData("/Memory/ErrorMessageWindow.arc");

    mMessageHolder = new MessageHolder();
    mMessageHolder->initSystemData();
}

void GameSystemObjHolder::init() {
    initAudio();
    initRenderMode();
    initResourceHolderManager();
    initMessageResource();
    initFunctionAsyncExecutor();
    initGameController();
    initDisplay();
}

void GameSystemObjHolder::createAudioSystem() {
    if (mAudioSystem != nullptr) {
        mAudioSystem->createAudioSystem();
    }
}

void GameSystemObjHolder::update() {
    mAudioSystem->updateRhythm();
    mWPadHolder->update();
    mFunctionAsyncExecutor->update();
    mStarPointerDirector->update();
}

void GameSystemObjHolder::updateAudioSystem() {
    if (mAudioSystem != nullptr) {
        mAudioSystem->movement();
    }
}

void GameSystemObjHolder::clearRequestFileInfo(bool unk) {
    SingletonHolder< FileLoader >::get()->clearRequestFileInfo(unk);
}

void GameSystemObjHolder::drawStarPointer() {
    mStarPointerDirector->draw();
}

void GameSystemObjHolder::drawBeforeEndRender() {
    mScreenPreserver->draw();
}

void GameSystemObjHolder::captureIfAllowForScreenPreserver() {
    mScreenPreserver->captureIfAllow();
}

GXRenderModeObj* GameSystemObjHolder::getRenderModeObj() const {
    return JUTVideo::getManager()->getRenderMode();
}

void GameSystemObjHolder::initDvd() {
    JKRMemArchive archive;

    JKRFileLoader::initializeVolumeList();
    SingletonHolder< FileLoader >::init();
}

void GameSystemObjHolder::initNAND() {
    SingletonHolder< NANDManager >::init();
}

void GameSystemObjHolder::initAudio() {
    mAudioSystem = new AudSystemWrapper(SingletonHolder< HeapMemoryWatcher >::get()->getAudSystemHeap(), MR::getStationedHeapNapa());
    mAudioSystem->requestResourceForInitialize();
}

void GameSystemObjHolder::initRenderMode() {
    mRenderModeObj = new GXRenderModeObj();
    *mRenderModeObj = *MR::getSuitableRenderMode();

    JUTVideo::createManager(mRenderModeObj);
}

void GameSystemObjHolder::initNameObj() {
    mObjHolder = new NameObjHolder(256);

    SingletonHolder< NameObjRegister >::init();
    SingletonHolder< NameObjRegister >::get()->setCurrentHolder(mObjHolder);
}

void GameSystemObjHolder::initFunctionAsyncExecutor() {
    mFunctionAsyncExecutor = new FunctionAsyncExecutor();
}

void GameSystemObjHolder::initResourceHolderManager() {
    SingletonHolder< ResourceHolderManager >::init();
}

void GameSystemObjHolder::initGameController() {
    initWPad();
    initStarPointerDirector();
}

void GameSystemObjHolder::initWPad() {
    mWPadHolder = new WPadHolder();
}

void GameSystemObjHolder::initStarPointerDirector() {
    mStarPointerDirector = new StarPointerDirector();
}

void GameSystemObjHolder::initDisplay() {
    MainLoopFramework* pManager;
    s32 size = MR::getRequiredExternalFrameBufferSize();
    JKRHeap* pStationedHeap = MR::getStationedHeapGDDR3();

    _C = new (pStationedHeap, 32) u8[size];
    _10 = new (pStationedHeap, 32) u8[size];
    _14 = new (pStationedHeap, 32) u8[size];

    pManager = MainLoopFramework::createManager(nullptr, _C, _10, _14, true);
    pManager->setClearColor(Color8(30, 30, 200, 0));

    mCaptureScreenDirector = new CaptureScreenDirector();
    mScreenPreserver = new ScreenPreserver();

    VISetTrapFilter(VI_TRUE);
    VISetTrapFilter(VI_FALSE);
}
