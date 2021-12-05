#include "Game/System/ResourceHolderManager.h"
#include "Game/Util.h"
#include "Game/SingletonHolder.h"

void ResourceHolderManager::startCreateResourceHolderOnMainThread(const char *pResource, CreateResourceHolderArgs *pArgs) {
    MR::FunctorV2M<ResourceHolderManager *, void (ResourceHolderManager::*)(const char *, CreateResourceHolderArgs *), const char *, CreateResourceHolderArgs *> func = MR::Functor<ResourceHolderManager, const char *, CreateResourceHolderArgs *>(SingletonHolder<ResourceHolderManager>::sInstance, &ResourceHolderManager::createResourceHolder, pResource, pArgs);
    MR::startFunctionAsyncExecuteOnMainThread(func, pResource);
}