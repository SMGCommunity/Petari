#include "Game/System/GameSystemObjHolder.hpp"
#include "Game/Effect/ParticleResourceHolder.hpp"
#include "Game/NameObj/NameObjRegister.hpp"
#include "Game/Screen/ScreenPreserver.hpp"
#include "Game/Screen/StarPointerDirector.hpp"
#include "Game/SingletonHolder.hpp"
#include "Game/System/AudSystemWrapper.hpp"
#include "Game/System/ErrorArchive.hpp"
#include "Game/System/FileLoader.hpp"
#include "Game/System/FunctionAsyncExecutor.hpp"
#include "Game/System/HeapMemoryWatcher.hpp"
#include "Game/System/Language.hpp"
#include "Game/System/MessageHolder.hpp"
#include "Game/System/NANDManager.hpp"
#include "Game/System/RenderMode.hpp"
#include "Game/System/ResourceHolderManager.hpp"
#include "Game/System/WPadHolder.hpp"
#include <JSystem/JKernel/JKRExpHeap.hpp>
#include <JSystem/JUtility/JUTVideo.hpp>
#include <revolution/gx/GXStruct.h>

GameSystemObjHolder::GameSystemObjHolder()
    : mObjHolder(nullptr), mParticleResHolder(nullptr), mRenderModeObj(nullptr), mCaptureScreenDir(nullptr), mScreenPreserver(nullptr),
      mSysWrapper(nullptr), mWPadHolder(nullptr), mAsyncExec(nullptr), mMsgHolder(nullptr), mStarPointerDir(nullptr), mRandom(0),
      mLanguage(MR::getDecidedLanguageFromIPL()) {
    initDvd();
    initNAND();
    initNameObj();
    MR::createAndAddArchive((void*)cErrorArchive, MR::getStationedHeapNapa(), "ErrorMessageArchive.arc");
}

void GameSystemObjHolder::initAfterStationedResourceLoaded() {
    SingletonHolder< NameObjRegister >::get()->setCurrentHolder(mObjHolder);
    mParticleResHolder = new ParticleResourceHolder("/ParticleData/Effect.arc");
    mMsgHolder->initGameData();
    MR::createStarPointerLayout();
}

void GameSystemObjHolder::initMessageResource() {
    JKRArchive* pArchive = nullptr;
    JKRHeap* pHeap = nullptr;
    MR::getMountedArchiveAndHeap("ErrorMessageArchive.arc", &pArchive, &pHeap);

    char systemFilePath[128];
    snprintf(systemFilePath, sizeof(systemFilePath), "/%s/MessageData/System.arc", MR::getCurrentLanguagePrefix());

    void* pSystemFile = MR::decompressFileFromArchive(pArchive, systemFilePath, nullptr, 0);
    MR::createAndAddArchive(pSystemFile, pHeap, "/Memory/SystemMessage.arc");

    void* pErrorFile = MR::decompressFileFromArchive(pArchive, "/LayoutData/ErrorMessageWindow.arc", pHeap, 32);
    MR::createAndAddArchive(pErrorFile, pHeap, "/Memory/ErrorMessageWindow.arc");

    MR::createAndAddLayoutHolderRawData("/Memory/ErrorMessageWindow.arc");

    mMsgHolder = new MessageHolder();
    mMsgHolder->initSystemData();
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
    if (mSysWrapper != nullptr) {
        mSysWrapper->createAudioSystem();
    }
}

void GameSystemObjHolder::update() {
    mSysWrapper->updateRhythm();
    mWPadHolder->update();
    mAsyncExec->update();
    mStarPointerDir->update();
}

void GameSystemObjHolder::updateAudioSystem() {
    if (mSysWrapper != nullptr) {
        mSysWrapper->movement();
    }
}

void GameSystemObjHolder::clearRequestFileInfo(bool unk) {
    SingletonHolder< FileLoader >::get()->clearRequestFileInfo(unk);
}

void GameSystemObjHolder::drawStarPointer() {
    mStarPointerDir->draw();
}

void GameSystemObjHolder::drawBeforeEndRender() {
    mScreenPreserver->draw();
}

void GameSystemObjHolder::captureIfAllowForScreenPreserver() {
    mScreenPreserver->captureIfAllow();
}

GXRenderModeObj* GameSystemObjHolder::getRenderModeObj() const {
    return JUTVideo::sManager->mRenderModeObj;
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
    mSysWrapper = new AudSystemWrapper(SingletonHolder< HeapMemoryWatcher >::get()->mAudSystemHeap, MR::getStationedHeapNapa());
    mSysWrapper->requestResourceForInitialize();
}

void GameSystemObjHolder::initRenderMode() {
    const GXRenderModeObj* pRenderModeObj;

    mRenderModeObj = new GXRenderModeObj();
    pRenderModeObj = MR::getSuitableRenderMode();

    mRenderModeObj->viTVmode = pRenderModeObj->viTVmode;
    mRenderModeObj->fbWidth = pRenderModeObj->fbWidth;
    mRenderModeObj->efbHeight = pRenderModeObj->efbHeight;
    mRenderModeObj->xfbHeight = pRenderModeObj->xfbHeight;
    mRenderModeObj->viXOrigin = pRenderModeObj->viXOrigin;
    mRenderModeObj->viYOrigin = pRenderModeObj->viYOrigin;
    mRenderModeObj->viWidth = pRenderModeObj->viWidth;
    mRenderModeObj->viHeight = pRenderModeObj->viHeight;
    mRenderModeObj->xFBmode = pRenderModeObj->xFBmode;
    mRenderModeObj->field_rendering = pRenderModeObj->field_rendering;
    mRenderModeObj->aa = pRenderModeObj->aa;

    /*for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 3; j++) {
            u8* pattern = pRenderModeObj->mSamplePattern[i][j];
            mRenderModeObj->sample_pattern[i][j] = pattern;
        }
    }*/

    mRenderModeObj->vfilter[0] = pRenderModeObj->vfilter[0];
    mRenderModeObj->vfilter[1] = pRenderModeObj->vfilter[1];
    mRenderModeObj->vfilter[2] = pRenderModeObj->vfilter[2];
    mRenderModeObj->vfilter[3] = pRenderModeObj->vfilter[3];
    mRenderModeObj->vfilter[4] = pRenderModeObj->vfilter[4];
    mRenderModeObj->vfilter[5] = pRenderModeObj->vfilter[5];
    mRenderModeObj->vfilter[6] = pRenderModeObj->vfilter[6];

    JUTVideo::createManager(mRenderModeObj);
}

void GameSystemObjHolder::initNameObj() {
    mObjHolder = new NameObjHolder(256);

    SingletonHolder< NameObjRegister >::init();
    SingletonHolder< NameObjRegister >::get()->setCurrentHolder(mObjHolder);
}

void GameSystemObjHolder::initFunctionAsyncExecutor() {
    mAsyncExec = new FunctionAsyncExecutor();
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
    mStarPointerDir = new StarPointerDirector();
}

// GameSystemObjHolder::initDisplay
