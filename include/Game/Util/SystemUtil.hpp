#pragma once

#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapIdInfo.hpp"

class ParticleResourceHolder;

namespace MR {
    bool isScreen16Per9();

    void setLayoutDefaultAllocator();

    void startFunctionAsyncExecute(const FunctorBase &, int, const char *);

    void startFunctionAsyncExecuteOnMainThread(const FunctorBase&, const char*);
    void waitForEndFunctionAsyncExecute(const char*);
    bool isEndFunctionAsyncExecute(const char*);
    bool tryEndFunctionAsyncExecute(const char*);
    void suspendAsyncExecuteThread(const char*);
    void resumeAsyncExecuteThread(const char*);
    bool isSuspendAsyncExecuteThread(const char*);

    void clearFileLoaderRequestFileInfo(bool);

    ParticleResourceHolder* getParticleResourceHolder();

    void destroySceneMessage();

    void requestChangeArchivePlayer(bool);

    JMapIdInfo* getPlayerRestartIdInfo();

};  // namespace MR
