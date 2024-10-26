#include "Game/System/ResourceHolderManager.hpp"
#include "Game/Util.hpp"
#include "Game/SingletonHolder.hpp"

void ResourceHolderManager::startCreateResourceHolderOnMainThread(const char *pResource, CreateResourceHolderArgs *pArgs) {
    MR::FunctorV2M<ResourceHolderManager *, void (ResourceHolderManager::*)(const char *, CreateResourceHolderArgs *), const char *, CreateResourceHolderArgs *> func = MR::Functor<ResourceHolderManager, const char *, CreateResourceHolderArgs *>(SingletonHolder<ResourceHolderManager>::sInstance, &ResourceHolderManager::createResourceHolder, pResource, pArgs);
    MR::startFunctionAsyncExecuteOnMainThread(func, pResource);
}