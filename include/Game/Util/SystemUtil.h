#pragma once

#include "Game/Util/Functor.h"

namespace MR {
    bool isScreen16Per9();

    void startFunctionAsyncExecuteOnMainThread(const FunctorBase &, const char *);
};