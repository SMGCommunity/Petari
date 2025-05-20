#include "Game/NameObj/NameObjHolder.hpp"
#include "Game/System/FunctionAsyncExecutor.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemObjHolder.hpp"
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
    void callMethodAllSceneNameObj(void (NameObj::*pMethod)()) {
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

    void startFunctionAsyncExecute(const MR::FunctorBase& rFunctor, int param1, const char* pParam3) {
        getFunctionAsyncExecutor()->start(rFunctor, param1, pParam3);
    }

    bool startFunctionAsyncExecuteOnMainThread(const MR::FunctorBase& rFunctor, const char* pParam2) {
        return getFunctionAsyncExecutor()->startOnMainThread(rFunctor, pParam2);
    }

    void waitForEndFunctionAsyncExecute(const char* pParam1) {
        getFunctionAsyncExecutor()->waitForEnd(pParam1);
    }

    bool isEndFunctionAsyncExecute(const char* pParam1) {
        return getFunctionAsyncExecutor()->isEnd(pParam1);
    }

    bool tryEndFunctionAsyncExecute(const char* pParam1) {
        if (getFunctionAsyncExecutor()->isEnd(pParam1)) {
            getFunctionAsyncExecutor()->waitForEnd(pParam1);

            return true;
        }

        return false;
    }

    void suspendAsyncExecuteThread(const char* pParam1) {
        OSSuspendThread(getFunctionAsyncExecutor()->getOSThread(pParam1));
    }

    void resumeAsyncExecuteThread(const char* pParam1) {
        OSThread* pThread = getFunctionAsyncExecutor()->getOSThread(pParam1);

        OSResumeThread(getFunctionAsyncExecutor()->getOSThread(pParam1));
    }

    bool isSuspendedAsyncExecuteThread(const char* pParam1) {
        OSThread* pThread = getFunctionAsyncExecutor()->getOSThread(pParam1);

        if (pThread == NULL) {
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
