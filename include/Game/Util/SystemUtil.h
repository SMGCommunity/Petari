#pragma once

#include "Game/Util/Functor.h"

namespace MR {
    bool isScreen16Per9();

    void setLayoutDefaultAllocator();

    void startFunctionAsyncExecuteOnMainThread(const FunctorBase &, const char *);
    void waitForEndFunctionAsyncExecute(const char *);
    bool isEndFunctionAsyncExecute(const char *);
    bool tryEndFunctionAsyncExecute(const char *);
    void suspendAsyncExecuteThread(const char *);
    void resumeAsyncExecuteThread(const char *);
    bool isSuspendAsyncExecuteThread(const char *);
};