#include "Game/Util/SystemUtil.hpp"
#include "Game/NameObj/NameObjHolder.hpp"
#include "Game/System/AudSystemWrapper.hpp"
#include "Game/System/FunctionAsyncExecutor.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/System/GameDataTemporaryInGalaxy.hpp"
#include "Game/System/GameSequenceDirector.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemFontHolder.hpp"
#include "Game/System/GameSystemFunction.hpp"
#include "Game/System/GameSystemObjHolder.hpp"
#include "Game/System/GameSystemSceneController.hpp"
#include "Game/System/MessageHolder.hpp"
#include "Game/System/RenderMode.hpp"
#include "Game/Util/HashUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SingletonHolder.hpp"
#include <nw4r/lyt/layout.h>
#include <nw4r/ut/ResFont.h>

GameSystemObjHolder* MR::getGameSystemObjHolder() {
    return SingletonHolder< GameSystem >::get()->mObjHolder;
}

namespace {
    FunctionAsyncExecutor* getFunctionAsyncExecutor() NO_INLINE {
        return MR::getGameSystemObjHolder()->mFunctionAsyncExecutor;
    }

    GameDataTemporaryInGalaxy* getGameDataTemporaryInGalaxy() {
        return SingletonHolder< GameSystem >::get()->mSequenceDirector->mGameDataTemporaryInGalaxy;
    }

    NameObjHolder* getSceneNameObjHolder() NO_INLINE {
        return SingletonHolder< GameSystem >::get()->mSceneController->mObjHolder;
    }
};  // namespace

nw4r::ut::Font* MR::getFontOnCurrentLanguage() {
    return SingletonHolder< GameSystem >::get()->mFontHolder->getMessageFont();
}

nw4r::ut::Font* MR::getPictureFontNW4R() {
    return SingletonHolder< GameSystem >::get()->mFontHolder->mPictureFont;
}

nw4r::ut::Font* MR::getMenuFontNW4R() {
    return SingletonHolder< GameSystem >::get()->mFontHolder->mMenuFont;
}

nw4r::ut::Font* MR::getNumberFontNW4R() {
    return SingletonHolder< GameSystem >::get()->mFontHolder->mNumberFont;
}

nw4r::ut::Font* MR::getCinemaFontNW4R() {
    return SingletonHolder< GameSystem >::get()->mFontHolder->mCinemaFont;
}

ParticleResourceHolder* MR::getParticleResourceHolder() {
    return getGameSystemObjHolder()->mParticleResHolder;
}

void MR::requestChangeArchivePlayer(bool isPlayerMario) {
    GameSystemFunction::requestChangeArchivePlayer(isPlayerMario);
}

void MR::waitEndChangeArchivePlayer() {
    while (!GameSystemFunction::isEndChangeArchivePlayer()) {
    }
}

void MR::callMethodAllSceneNameObj(NameObjMethod pMethod) {
    ::getSceneNameObjHolder()->callMethodAllObj(pMethod);
}

void MR::suspendAllSceneNameObj() {
    ::getSceneNameObjHolder()->suspendAllObj();
}

void MR::resumeAllSceneNameObj() {
    ::getSceneNameObjHolder()->resumeAllObj();
}

void MR::syncWithFlagsAllSceneNameObj() {
    ::getSceneNameObjHolder()->syncWithFlags();
}

void MR::setRandomSeedFromStageName() {
    u32 seed = getHashCode(getCurrentStageName());

    getGameSystemObjHolder()->mRandom.mSeed = seed;
}

void MR::clearFileLoaderRequestFileInfo(bool param1) {
    getGameSystemObjHolder()->clearRequestFileInfo(param1);
}

void MR::startFunctionAsyncExecute(const MR::FunctorBase& rFunc, int threadPriority, const char* pThreadName) {
    ::getFunctionAsyncExecutor()->start(rFunc, threadPriority, pThreadName);
}

bool MR::startFunctionAsyncExecuteOnMainThread(const MR::FunctorBase& rFunc, const char* pThreadName) {
    return ::getFunctionAsyncExecutor()->startOnMainThread(rFunc, pThreadName);
}

void MR::waitForEndFunctionAsyncExecute(const char* pThreadName) {
    ::getFunctionAsyncExecutor()->waitForEnd(pThreadName);
}

bool MR::isEndFunctionAsyncExecute(const char* pThreadName) {
    return ::getFunctionAsyncExecutor()->isEnd(pThreadName);
}

bool MR::tryEndFunctionAsyncExecute(const char* pThreadName) {
    if (isEndFunctionAsyncExecute(pThreadName)) {
        waitForEndFunctionAsyncExecute(pThreadName);

        return true;
    }

    return false;
}

void MR::suspendAsyncExecuteThread(const char* pThreadName) {
    OSSuspendThread(::getFunctionAsyncExecutor()->getOSThread(pThreadName));
}

void MR::resumeAsyncExecuteThread(const char* pThreadName) {
    OSThread* pThread = ::getFunctionAsyncExecutor()->getOSThread(pThreadName);

    OSResumeThread(::getFunctionAsyncExecutor()->getOSThread(pThreadName));
}

bool MR::isSuspendedAsyncExecuteThread(const char* pThreadName) {
    OSThread* pThread = ::getFunctionAsyncExecutor()->getOSThread(pThreadName);

    if (pThread == nullptr) {
        return false;
    }

    return OSIsThreadSuspended(pThread);
}

bool MR::isScreen16Per9() {
    return isAspectRatioFlag16Per9();
}

void MR::initSceneMessage() {
    getGameSystemObjHolder()->mMessageHolder->initSceneData();
}

void MR::destroySceneMessage() {
    getGameSystemObjHolder()->mMessageHolder->destroySceneData();
}

void MR::resetSystemAndGameStatus() {
    GameDataFunction::resetAllGameData();
    GameSystemFunction::resetAllControllerRumble();
    resetGlobalTimer();
}

void MR::stopAllSound(u32 param1) {
    getGameSystemObjHolder()->mAudioSystem->stopAllSound(param1);
}

void MR::setLayoutDefaultAllocator() {
    nw4r::lyt::Layout::mspAllocator = &MR::NewDeleteAllocator::sAllocator;
}

bool MR::isDisplayEncouragePal60Window() {
    return VIGetTvFormat() == VI_PAL;
}

JMapIdInfo* getPlayerRestartIdInfo() {
    return ::getGameDataTemporaryInGalaxy()->mPlayerRestartIdInfo;
}

void MR::setPlayerRestartIdInfo(const JMapIdInfo& rInfo) {
    ::getGameDataTemporaryInGalaxy()->setPlayerRestartIdInfo(rInfo);
}
