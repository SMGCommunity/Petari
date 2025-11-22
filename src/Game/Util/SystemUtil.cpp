#include "Game/NameObj/NameObjHolder.hpp"
#include "Game/System/AudSystemWrapper.hpp"
#include "Game/System/FunctionAsyncExecutor.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemFontHolder.hpp"
#include "Game/System/GameSystemFunction.hpp"
#include "Game/System/GameSystemObjHolder.hpp"
#include "Game/System/GameSystemSceneController.hpp"
#include "Game/System/MessageHolder.hpp"
#include "Game/Util/HashUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/SystemUtil.hpp"
#include "Game/SingletonHolder.hpp"
#include <nw4r/lyt/layout.h>

namespace MR {
    GameSystemObjHolder* getGameSystemObjHolder() {
        return SingletonHolder< GameSystem >::get()->mObjHolder;
    }
}; // namespace MR

namespace {
    FunctionAsyncExecutor* getFunctionAsyncExecutor() NO_INLINE {
        return MR::getGameSystemObjHolder()->mAsyncExec;
    }
}; // namespace

namespace {
    NameObjHolder* getSceneNameObjHolder() NO_INLINE {
        return SingletonHolder< GameSystem >::get()->mSceneController->mObjHolder;
    }
}; // namespace

namespace MR {
    nw4r::ut::Font* getFontOnCurrentLanguage() {
        return SingletonHolder< GameSystem >::get()->mFontHolder->getMessageFont();
    }

    nw4r::ut::Font* getPictureFontNW4R() {
        // TODO: Explicit cast should be removed or adjusted when nw4r::ut::ResFont is properly declared.
        return reinterpret_cast< nw4r::ut::Font* >(SingletonHolder< GameSystem >::get()->mFontHolder->mPictureFont);
    }

    nw4r::ut::Font* getMenuFontNW4R() {
        // TODO: Explicit cast should be removed or adjusted when nw4r::ut::ResFont is properly declared.
        return reinterpret_cast< nw4r::ut::Font* >(SingletonHolder< GameSystem >::get()->mFontHolder->mMenuFont);
    }

    nw4r::ut::Font* getNumberFontNW4R() {
        // TODO: Explicit cast should be removed or adjusted when nw4r::ut::ResFont is properly declared.
        return reinterpret_cast< nw4r::ut::Font* >(SingletonHolder< GameSystem >::get()->mFontHolder->mNumberFont);
    }

    nw4r::ut::Font* getCinemaFontNW4R() {
        // TODO: Explicit cast should be removed or adjusted when nw4r::ut::ResFont is properly declared.
        return reinterpret_cast< nw4r::ut::Font* >(SingletonHolder< GameSystem >::get()->mFontHolder->mCinemaFont);
    }

    ParticleResourceHolder* getParticleResourceHolder() {
        return getGameSystemObjHolder()->mParticleResHolder;
    }

    void requestChangeArchivePlayer(bool isPlayerMario) {
        GameSystemFunction::requestChangeArchivePlayer(isPlayerMario);
    }

    void waitEndChangeArchivePlayer() {
        while (!GameSystemFunction::isEndChangeArchivePlayer()) {
        }
    }

    void callMethodAllSceneNameObj(NameObjMethod pMethod) {
        getSceneNameObjHolder()->callMethodAllObj(pMethod);
    }

    void suspendAllSceneNameObj() {
        getSceneNameObjHolder()->suspendAllObj();
    }

    void resumeAllSceneNameObj() {
        getSceneNameObjHolder()->resumeAllObj();
    }

    void syncWithFlagsAllSceneNameObj() {
        getSceneNameObjHolder()->syncWithFlags();
    }

    void setRandomSeedFromStageName() {
        u32 seed = getHashCode(getCurrentStageName());

        getGameSystemObjHolder()->mRandom.mSeed = seed;
    }

    void clearFileLoaderRequestFileInfo(bool param1) {
        getGameSystemObjHolder()->clearRequestFileInfo(param1);
    }

    void startFunctionAsyncExecute(const MR::FunctorBase& rFunc, int threadPriority, const char* pThreadName) {
        getFunctionAsyncExecutor()->start(rFunc, threadPriority, pThreadName);
    }

    bool startFunctionAsyncExecuteOnMainThread(const MR::FunctorBase& rFunc, const char* pThreadName) {
        return getFunctionAsyncExecutor()->startOnMainThread(rFunc, pThreadName);
    }

    void waitForEndFunctionAsyncExecute(const char* pThreadName) {
        getFunctionAsyncExecutor()->waitForEnd(pThreadName);
    }

    bool isEndFunctionAsyncExecute(const char* pThreadName) {
        return getFunctionAsyncExecutor()->isEnd(pThreadName);
    }

    bool tryEndFunctionAsyncExecute(const char* pThreadName) {
        if (isEndFunctionAsyncExecute(pThreadName)) {
            waitForEndFunctionAsyncExecute(pThreadName);

            return true;
        }

        return false;
    }

    void suspendAsyncExecuteThread(const char* pThreadName) {
        OSSuspendThread(getFunctionAsyncExecutor()->getOSThread(pThreadName));
    }

    void resumeAsyncExecuteThread(const char* pThreadName) {
        OSThread* pThread = getFunctionAsyncExecutor()->getOSThread(pThreadName);

        OSResumeThread(getFunctionAsyncExecutor()->getOSThread(pThreadName));
    }

    bool isSuspendedAsyncExecuteThread(const char* pThreadName) {
        OSThread* pThread = getFunctionAsyncExecutor()->getOSThread(pThreadName);

        if (pThread == nullptr) {
            return false;
        }

        return OSIsThreadSuspended(pThread);
    }

    // isScreen16Per9

    void initSceneMessage() {
        getGameSystemObjHolder()->mMsgHolder->initSceneData();
    }

    void destroySceneMessage() {
        getGameSystemObjHolder()->mMsgHolder->destroySceneData();
    }

    void resetSystemAndGameStatus() {
        GameDataFunction::resetAllGameData();
        GameSystemFunction::resetAllControllerRumble();
        resetGlobalTimer();
    }

    void stopAllSound(u32 param1) {
        getGameSystemObjHolder()->mSysWrapper->stopAllSound(param1);
    }

    void setLayoutDefaultAllocator() {
        nw4r::lyt::Layout::mspAllocator = &MR::NewDeleteAllocator::sAllocator;
    }

    bool isDisplayEncouragePal60Window() {
        return VIGetTvFormat() == 1;
    }
}; // namespace MR
