#include "Game/NameObj/NameObjHolder.hpp"
#include "Game/System/FunctionAsyncExecutor.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemObjHolder.hpp"
#include "Game/System/GameSystemSceneController.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/SystemUtil.hpp"
#include "Game/SingletonHolder.hpp"
#include <nw4r/lyt/layout.h>

namespace MR {
    GameSystemObjHolder* getGameSystemObjHolder() {
        return SingletonHolder<GameSystem>::get()->mObjHolder;
    }
};

namespace {
    FunctionAsyncExecutor* getFunctionAsyncExecutor() NO_INLINE {
        return MR::getGameSystemObjHolder()->mAsyncExec;
    }
};

namespace {
    NameObjHolder* getSceneNameObjHolder() NO_INLINE {
        return SingletonHolder<GameSystem>::get()->mSceneController->mObjHolder;
    }
};

namespace MR {
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

    void setLayoutDefaultAllocator() {
        nw4r::lyt::Layout::mspAllocator = &MR::NewDeleteAllocator::sAllocator;
    }

    bool isDisplayEncouragePal60Window() {
        return VIGetTvFormat() == 1;
    }
};
