#include "Game/System/GameSystemObjHolder.hpp"
#include "Game/System/ErrorArchive.hpp"
#include "Game/System/FileLoader.hpp"
#include "Game/System/HeapMemoryWatcher.hpp"
#include "Game/System/Language.hpp"
#include "Game/System/ResourceHolderManager.hpp"
#include "Game/System/NANDManager.hpp"
#include "Game/SingletonHolder.hpp"
#include "Game/Util/RenderMode.hpp"
#include <JSystem/JUtility/JUTVideo.hpp>
#include <cstdio>

GameSystemObjHolder::GameSystemObjHolder() : 
    mObjHolder(nullptr), mParticleResHolder(nullptr), mRenderModeObj(nullptr),
    mCaptureScreenDir(nullptr), mScreenPreserver(nullptr), mSysWrapper(nullptr),
    mWPadHolder(nullptr), mAsyncExec(nullptr), mMsgHolder(nullptr), mStarPointerDir(nullptr),
    mRandom(0)
    {
        mLanguage = MR::getDecidedLanguageFromIPL();
        initDvd();
        initNAND();
        initNameObj(); 
        MR::createAndAddArchive((void*)cErrorArchive, MR::getStationedHeapNapa(), "ErrorMessageArchive.arc");
}

void GameSystemObjHolder::initAfterStationedResourceLoaded() {
    SingletonHolder<NameObjRegister>::get()->setCurrentHolder(mObjHolder);
    mParticleResHolder = new ParticleResourceHolder("/ParticleData/Effect.arc");
    mMsgHolder->initGameData();
    MR::createStarPointerLayout(); 
}

void GameSystemObjHolder::initMessageResource() {
    JKRArchive* archive = nullptr;
    JKRHeap* heap = nullptr;
    MR::getMountedArchiveAndHeap("ErrorMessageArchive.arc", &archive, &heap);
    
    char buf[0x80];
    snprintf(buf, sizeof(buf), "/%s/MessageData/System.arc", MR::getCurrentLanguagePrefix());
    void* systemFile = MR::decompressFileFromArchive(archive, buf, nullptr, 0);
    MR::createAndAddArchive(systemFile, heap, "/Memory/SystemMessage.arc");
    void* errorFile = MR::decompressFileFromArchive(archive, "/LayoutData/ErrorMessageWindow.arc", heap, 0x20);
    MR::createAndAddArchive(errorFile, heap, "/Memory/ErrorMessageWindow.arc");
    MR::createAndAddLayoutHolderRawData("/Memory/ErrorMessageWindow.arc");
    mMsgHolder = new MessageHolder();
    mMsgHolder->initSystemData();
}

void GameSystemObjHolder::init() {
    initAudio();
    initRenderMode();
    initResourceHolderManager();
    initMessageResource();
    mAsyncExec = new FunctionAsyncExecutor();
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
    SingletonHolder<FileLoader>::get()->clearRequestFileInfo(unk);
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

    if (!SingletonHolder<NANDManager>::exists()) {
        SingletonHolder<FileLoader>::set(new FileLoader());
    }
}

void GameSystemObjHolder::initNAND() {
    if (!SingletonHolder<NANDManager>::exists()) {
        SingletonHolder<NANDManager>::set(new NANDManager());
    }
}

void GameSystemObjHolder::initAudio() {
    mSysWrapper = new AudSystemWrapper(MR::getAudHeap(), MR::getStationedHeapNapa());
    mSysWrapper->requestResourceForInitialize();
}

void GameSystemObjHolder::initRenderMode() {
    mRenderModeObj = new GXRenderModeObj();
    RenderModeObj* obj = MR::getSuitableRenderMode();

    mRenderModeObj->viTVmode = obj->mTVMode;
    mRenderModeObj->fbWidth = obj->mFrameBufWidth;
    mRenderModeObj->efbHeight = obj->mEFBHeight;
    mRenderModeObj->xfbHeight = obj->mXFBHeight;
    mRenderModeObj->viXOrigin = obj->mXOrigin;
    mRenderModeObj->viYOrigin = obj->mYOrigin;
    mRenderModeObj->viWidth = obj->mVIWidth;
    mRenderModeObj->viHeight = obj->mVIHeight;
    mRenderModeObj->xFBmode = obj->mMode;
    mRenderModeObj->field_rendering = obj->mFieldRendering;
    mRenderModeObj->aa = obj->mAA;

    /*for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 3; j++) {
            u8* pattern = obj->mSamplePattern[i][j];
            mRenderModeObj->sample_pattern[i][j] = pattern;
        }
    }*/

    mRenderModeObj->vfilter[0] = obj->mFilter[0];
    mRenderModeObj->vfilter[1] = obj->mFilter[1];
    mRenderModeObj->vfilter[2] = obj->mFilter[2];
    mRenderModeObj->vfilter[3] = obj->mFilter[3];
    mRenderModeObj->vfilter[4] = obj->mFilter[4];
    mRenderModeObj->vfilter[5] = obj->mFilter[5];
    mRenderModeObj->vfilter[6] = obj->mFilter[6];

    JUTVideo::createManager(mRenderModeObj);
}

void GameSystemObjHolder::initNameObj() {
    mObjHolder = new NameObjHolder(0x100);

    if (!SingletonHolder<NameObjRegister>::exists()) {
        SingletonHolder<NameObjRegister>::set(new NameObjRegister());
    }

    SingletonHolder<NameObjRegister>::get()->setCurrentHolder(mObjHolder);
}

void GameSystemObjHolder::initResourceHolderManager() {
    if (!SingletonHolder<ResourceHolderManager>::exists()) {
        SingletonHolder<ResourceHolderManager>::set(new ResourceHolderManager());
    }
}

void GameSystemObjHolder::initGameController() {
    mWPadHolder = new WPadHolder();
    mStarPointerDir = new StarPointerDirector();
}