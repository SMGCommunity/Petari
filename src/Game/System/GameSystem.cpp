#include "Game/LiveActor/Nerve.hpp"
#include "Game/NameObj/NameObjRegister.hpp"
#include "Game/Screen/HomeButtonLayout.hpp"
#include "Game/Screen/SystemWipeHolder.hpp"
#include "Game/System/AudSystemWrapper.hpp"
#include "Game/System/DrawSyncManager.hpp"
#include "Game/System/FileRipper.hpp"
#include "Game/System/GameSequenceDirector.hpp"
#include "Game/System/GameSequenceFunction.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemDimmingWatcher.hpp"
#include "Game/System/GameSystemErrorWatcher.hpp"
#include "Game/System/GameSystemException.hpp"
#include "Game/System/GameSystemFontHolder.hpp"
#include "Game/System/GameSystemFrameControl.hpp"
#include "Game/System/GameSystemFunction.hpp"
#include "Game/System/GameSystemObjHolder.hpp"
#include "Game/System/GameSystemResetAndPowerProcess.hpp"
#include "Game/System/GameSystemSceneController.hpp"
#include "Game/System/GameSystemStationedArchiveLoader.hpp"
#include "Game/System/HeapMemoryWatcher.hpp"
#include "Game/System/HomeButtonStateNotifier.hpp"
#include "Game/System/MainLoopFramework.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/MutexHolder.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/SequenceUtil.hpp"
#include "Game/Util/SystemUtil.hpp"
#include "Game/SingletonHolder.hpp"
#include <JSystem/JKernel/JKRAram.hpp>
#include <nw4r/lyt/init.h>
#include <revolution.h>

namespace NrvGameSystem {
    NEW_NERVE(GameSystemInitializeAudio, GameSystem, InitializeAudio);
    NEW_NERVE(GameSystemInitializeLogoScene, GameSystem, InitializeLogoScene);
    NEW_NERVE(GameSystemLoadStationedArchive, GameSystem, LoadStationedArchive);
    NEW_NERVE(GameSystemWaitForReboot, GameSystem, WaitForReboot);
    NEW_NERVE(GameSystemNormal, GameSystem, Normal);
};

void main(void) {
    __asm {
        li r3, 4
        oris r3, r3, 4
        mtspr 0x392, r3
        li r3, 5
        oris r3, r3, 5
        mtspr 0x393, r3
        li r3, 6
        oris r3, r3, 6
        mtspr 0x394, r3
        li r3, 7
        oris r3, r3, 7
        mtspr 0x395, r3
    };

    DVDInit();
    VIInit();
    HeapMemoryWatcher::createRootHeap();
    OSInitMutex(&MR::MutexHolder<0>::sMutex);
    OSInitMutex(&MR::MutexHolder<1>::sMutex);
    OSInitMutex(&MR::MutexHolder<2>::sMutex);
    nw4r::lyt::LytInit();
    MR::setLayoutDefaultAllocator();
    SingletonHolder<HeapMemoryWatcher>::init();
    SingletonHolder<HeapMemoryWatcher>::get()->setCurrentHeapToStationedHeap();
    FileRipper::setup(0x20000, MR::getStationedHeapNapa());
    GameSystemException::init();
    MR::initAcosTable();
    SingletonHolder<GameSystem>::init();
    SingletonHolder<GameSystem>::get()->init();

    GameSystem* pGameSystem = SingletonHolder<GameSystem>::get();

    while (true) {
        pGameSystem->frameLoop();
    }
}

GameSystem::GameSystem() :
    NerveExecutor("GameSystem"),
    mFifoBase(NULL),
    mSequenceDirector(NULL),
    mErrorWatcher(NULL),
    mFontHolder(NULL),
    mFrameControl(NULL),
    mObjHolder(NULL),
    mSceneController(NULL),
    mStationedArchiveLoader(NULL),
    mHomeButtonLayout(NULL),
    mSystemWipeHolder(NULL),
    mHomeButtonStateNotifier(NULL),
    mIsExecuteLoadSystemArchive(false)
{
    
}

void GameSystem::init() {
    JKRAram::create(0xe00000, 0xffffffff, 8, 7, 3);
    mObjHolder = new GameSystemObjHolder();
    mFontHolder = new GameSystemFontHolder();
    mFontHolder->createFontFromEmbeddedData();
    initNerve(&NrvGameSystem::GameSystemInitializeAudio::sInstance);
    mSequenceDirector = new GameSequenceDirector();
    initGX();
    DrawSyncManager::start(0x300, 0xf);
    mSceneController = new GameSystemSceneController();
    mObjHolder->init();
    mErrorWatcher = new GameSystemErrorWatcher();
    mFrameControl = new GameSystemFrameControl();
    SingletonHolder<GameSystemResetAndPowerProcess>::init();
    SingletonHolder<GameSystemResetAndPowerProcess>::get()->initWithoutIter();
    mStationedArchiveLoader = new GameSystemStationedArchiveLoader();
    mHomeButtonLayout = new HomeButtonLayout();
    mHomeButtonStateNotifier = new HomeButtonStateNotifier();
    mDimmingWatcher = new GameSystemDimmingWatcher();
    setNerve(&NrvGameSystem::GameSystemInitializeAudio::sInstance);
}

bool GameSystem::isExecuteLoadSystemArchive() const {
    return mIsExecuteLoadSystemArchive;
}

bool GameSystem::isDoneLoadSystemArchive() const {
    return isNerve(&NrvGameSystem::GameSystemNormal::sInstance);
}

void GameSystem::startToLoadSystemArchive() {
    mIsExecuteLoadSystemArchive = true;

    SingletonHolder<HeapMemoryWatcher>::get()->setCurrentHeapToStationedHeap();
    SingletonHolder<NameObjRegister>::get()->setCurrentHolder(mObjHolder->mObjHolder);
    setNerve(&NrvGameSystem::GameSystemLoadStationedArchive::sInstance);
}

void GameSystem::exeInitializeAudio() {
    if (MR::isFirstStep(this)) {
        MR::startFunctionAsyncExecute(MR::Functor_Inline<GameSystemObjHolder>(mObjHolder, &GameSystemObjHolder::createAudioSystem), 0xe, INIT_AUDIO_KEY);
    }
    updateSceneController();
    if (MR::isEndFunctionAsyncExecute(INIT_AUDIO_KEY) 
        && mObjHolder->mSysWrapper->isLoadDoneWaveDataAtSystemInit()) {
        MR::waitForEndFunctionAsyncExecute(INIT_AUDIO_KEY);
        setNerve(&NrvGameSystem::GameSystemInitializeLogoScene::sInstance);
    }
}

void GameSystem::exeInitializeLogoScene() {
    if (GameSystemFunction::isResetProcessing()) {
        setNerve(&NrvGameSystem::GameSystemWaitForReboot::sInstance);
    }
    else{
        if (MR::isFirstStep(this)) {
            MR::requestChangeScene("Logo");
        }
        updateSceneController();
    }
}

void GameSystem::exeLoadStationedArchive() {
    mStationedArchiveLoader->update();
    updateSceneController();

    if (mStationedArchiveLoader->isDone()) {
        setNerve(&NrvGameSystem::GameSystemNormal::sInstance);
    }
}

void GameSystem::exeWaitForReboot() {
    
}

void GameSystem::exeNormal() {
    updateSceneController();
    mStationedArchiveLoader->update();
}

void GameSystem::initGX() {
    if (!mFifoBase) {
        mFifoBase = new (0x20) u8[GX_FIFO_SIZE];
    }
    GXInit(mFifoBase, GX_FIFO_SIZE);
}

void GameSystem::initAfterStationedResourceLoaded() {
    mFontHolder->createFontFromFile();
    mObjHolder->initAfterStationedResourceLoaded();
    mHomeButtonLayout->initWithoutIter();
    mErrorWatcher->initAfterResourceLoaded();
    mSystemWipeHolder = MR::createSystemWipeHolder();
    mSceneController->initAfterStationedResourceLoaded();
    mSequenceDirector->initAfterResourceLoaded();
}

void GameSystem::prepareReset() {
    mStationedArchiveLoader->prepareReset();
}

inline bool isSystemWaitForReboot(const GameSystem* pGameSys) {
    return pGameSys->isNerve(&NrvGameSystem::GameSystemWaitForReboot::sInstance);
}

inline bool isSystemNormal(const GameSystem* pGameSys) {
    return pGameSys->isNerve(&NrvGameSystem::GameSystemNormal::sInstance);
}

bool GameSystem::isPreparedReset() const {
    return isSystemWaitForReboot(this) || isSystemNormal(this) || mStationedArchiveLoader->isPreparedReset();
}

void GameSystem::frameLoop() {
    MainLoopFramework::sManager->beginRender();
    draw();
    MainLoopFramework::sManager->endRender();
    update();
    calcAnim();
    mObjHolder->captureIfAllowForScreenPreserver();
    MainLoopFramework::sManager->endFrame();
    MainLoopFramework::sManager->waitForRetrace();
}

void GameSystem::draw() {
    mSceneController->drawScene();
    mSequenceDirector->draw();
    mObjHolder->drawStarPointer();
    mObjHolder->drawBeforeEndRender();
    if (mSystemWipeHolder) {
        mSystemWipeHolder->draw();
    }
    mErrorWatcher->draw();
    mHomeButtonLayout->draw();
    SingletonHolder<GameSystemResetAndPowerProcess>::get()->draw();
}

void GameSystem::update() {
    SingletonHolder<GameSystemResetAndPowerProcess>::get()->movement();
    mSceneController->checkRequestAndChangeScene();
    mObjHolder->update();
    mHomeButtonLayout->movement();
    if (!mHomeButtonLayout->isActive()) {
        mErrorWatcher->movement();
    }
    mDimmingWatcher->_5 = mErrorWatcher->isWarning() 
                          || mHomeButtonLayout->isActive() 
                          || GameSequenceFunction::isActiveSaveDataHandleSequence();
    mDimmingWatcher->update();
    updateNerve();
}

void GameSystem::updateSceneController() {
    bool doSceneUpdate = true;
    bool resetProcessing = SingletonHolder<GameSystemResetAndPowerProcess>::get()->isActive();
    
    mObjHolder->updateAudioSystem();
    
    if (resetProcessing) {
        doSceneUpdate = false;
    }
    if (mHomeButtonLayout->isActive()) {
        doSceneUpdate = false;
    }
    if (GameSystemFunction::isOccurredSystemWarning()) {
        doSceneUpdate = false;
    }

    mHomeButtonStateNotifier->update(mHomeButtonLayout->isActive() || GameSystemFunction::isOccurredSystemWarning());

    if (doSceneUpdate || resetProcessing) {
        mSequenceDirector->update();
    }
    
    if (doSceneUpdate || mSceneController->isFirstUpdateSceneNerveNormal()) {
        if (mSystemWipeHolder) {
            mSystemWipeHolder->movement();
        }
        mSceneController->updateScene();
    }
    
    if (resetProcessing) {
        mSceneController->updateSceneDuringResetProcessing();
    }
}

void GameSystem::calcAnim() {
    mSceneController->calcAnimScene();
    if (mSystemWipeHolder) {
        mSystemWipeHolder->calcAnim();
    }
}
