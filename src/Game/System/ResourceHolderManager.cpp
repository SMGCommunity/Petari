#include "Game/System/LayoutHolder.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/System/ResourceHolderManager.hpp"
#include "Game/Util/FileUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/HashUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "Game/Util/SystemUtil.hpp"
#include "Game/SingletonHolder.hpp"
#include <revolution/gd/GDBase.h>

namespace {
    class GDCurrentRestorer {
    public:
        GDCurrentRestorer() :
            mObj(__GDCurrentDL)
        {
            
        }

        ~GDCurrentRestorer() {
            __GDCurrentDL = mObj;
        }

    private:
        /* 0x0 */ GDLObj* mObj;
    };
};

ResourceHolderManager::ResourceHolderManager() {
    mResourceArray.clear();
}

ResourceHolder* ResourceHolderManager::createAndAdd(const char* pArcName, JKRHeap*) {
    return createAndAddInner(pArcName, MR::makeObjectArchiveFileName, startCreateResourceHolderOnMainThread)->mResourceHolder;
}

ResourceHolder* ResourceHolderManager::createAndAddStationed(const char* pParam1) {
    return createAndAddInnerStationed(pParam1, &ResourceHolderManager::createResourceHolder)->mResourceHolder;
}

LayoutHolder* ResourceHolderManager::createAndAddLayoutHolder(const char* pParam1, JKRHeap*) {
    return createAndAddInner(pParam1, MR::makeLayoutArchiveFileName, startCreateLayoutHolderOnMainThread)->mLayoutHolder;
}

LayoutHolder* ResourceHolderManager::createAndAddLayoutHolderStationed(const char* pParam1) {
    return createAndAddInnerStationed(pParam1, &ResourceHolderManager::createLayoutHolder)->mLayoutHolder;
}

LayoutHolder* ResourceHolderManager::createAndAddLayoutHolderRawData(const char* pParam1) {
    CreateResourceHolderArgs args = CreateResourceHolderArgs();

    // TODO: Pointer-to-member-function call. Might be an inlined functor?

    return add(pParam1, args)->mLayoutHolder;
}

void ResourceHolderManager::removeIfIsEqualHeap(JKRHeap* pHeap) {
    if (pHeap == nullptr) {
        return;
    }

    for (ResourceHolderManagerName2Resource* pIter = mResourceArray.begin(); pIter != mResourceArray.end(); pIter++) {
        if (pIter->mHeap != pHeap
            && MR::getHeapNapa(pIter->mHeap) != pHeap
            && MR::getHeapGDDR3(pIter->mHeap) != pHeap)
        {
            continue;
        }

        if (pIter->mResourceHolder != nullptr) {
            delete pIter->mResourceHolder;
        }

        if (pIter->mLayoutHolder != nullptr) {
            delete pIter->mLayoutHolder;
        }

        pIter->mHeap = nullptr;
        pIter->mLayoutHolder = nullptr;
        pIter->mResourceHolder = nullptr;
    }

    for (ResourceHolderManagerName2Resource* pIter = mResourceArray.begin(); pIter != mResourceArray.end(); pIter++) {
        if (pIter->mHeap != nullptr) {
            continue;
        }

        // FIXME: Supposed inline of MR::Vector::erase.
        mResourceArray.erase(pIter);
    }
}

void ResourceHolderManager::startCreateResourceHolderOnMainThread(const char* pParam1, CreateResourceHolderArgs* pArgs) {
    MR::FunctorV2M<ResourceHolderManager*, FuncPtrC, const char*, CreateResourceHolderArgs*> func
        = MR::Functor(SingletonHolder<ResourceHolderManager>::get(), createResourceHolder, pParam1, pArgs);

    MR::startFunctionAsyncExecuteOnMainThread(func, pParam1);
}

void ResourceHolderManager::startCreateLayoutHolderOnMainThread(const char* pParam1, CreateResourceHolderArgs* pArgs) {
    MR::FunctorV2M<ResourceHolderManager*, FuncPtrC, const char*, CreateResourceHolderArgs*> func
        = MR::Functor(SingletonHolder<ResourceHolderManager>::get(), createLayoutHolder, pParam1, pArgs);

    MR::startFunctionAsyncExecuteOnMainThread(func, pParam1);
}

ResourceHolderManagerName2Resource* ResourceHolderManager::createAndAddInner(const char* pArcName, MakeArchiveFileNameFuncPtr pMakeArchiveFileNameFunc, FuncPtrB pParam3) {
    ResourceHolderManagerName2Resource* pName2Resource = find(pArcName);

    if (pName2Resource != nullptr) {
        return pName2Resource;
    }

    char archivePath[128];
    (*pMakeArchiveFileNameFunc)(archivePath, sizeof(archivePath), pArcName);

    if (!MR::receiveArchive(archivePath)) {
        MR::mountArchive(archivePath, MR::getCurrentHeap());
    }

    CreateResourceHolderArgs args = CreateResourceHolderArgs();

    (*pParam3)(archivePath, &args);
    MR::waitForEndFunctionAsyncExecute(archivePath);

    return add(archivePath, args);
}

ResourceHolderManagerName2Resource* ResourceHolderManager::createAndAddInnerStationed(const char* pParam1, FuncPtrC pParam2) {
    CreateResourceHolderArgs args = CreateResourceHolderArgs();
    MR::FunctorV2M<ResourceHolderManager*, FuncPtrC, const char*, CreateResourceHolderArgs*> func
        = MR::Functor(this, pParam2, pParam1, &args);

    MR::startFunctionAsyncExecuteOnMainThread(func, pParam1);

    ResourceHolderManagerName2Resource* pName2Resource = add(pParam1, args);

    return pName2Resource;
}

void ResourceHolderManager::createResourceHolder(const char* pParam1, CreateResourceHolderArgs* pArgs) {
    GDCurrentRestorer gdRestorer = GDCurrentRestorer();
    JKRArchive* pArchive;

    MR::getMountedArchiveAndHeap(pParam1, &pArchive, &pArgs->mHeap);
    MR::CurrentHeapRestorer heapRestorer(pArgs->mHeap);

    pArgs->mResourceHolder = new ResourceHolder(*pArchive);
}

void ResourceHolderManager::createLayoutHolder(const char* pParam1, CreateResourceHolderArgs* pArgs) {
    GDCurrentRestorer gdRestorer = GDCurrentRestorer();
    JKRArchive* pArchive;

    MR::getMountedArchiveAndHeap(pParam1, &pArchive, &pArgs->mHeap);
    MR::CurrentHeapRestorer heapRestorer(pArgs->mHeap);

    pArgs->mLayoutHolder = new LayoutHolder(*pArchive);
}

ResourceHolderManagerName2Resource* ResourceHolderManager::add(const char* pParam1, const CreateResourceHolderArgs& pArgs) {
    ResourceHolderManagerName2Resource name2Resource = ResourceHolderManagerName2Resource();
    name2Resource.mHash = MR::getHashCodeLower(MR::getBasename(pParam1));
    name2Resource.mResourceHolder = pArgs.mResourceHolder;
    name2Resource.mLayoutHolder = pArgs.mLayoutHolder;
    name2Resource.mHeap = pArgs.mHeap;

    mResourceArray.push_back(name2Resource);

    return &mResourceArray[mResourceArray.size() - 1];
}

ResourceHolderManagerName2Resource* ResourceHolderManager::find(const char* pParam1) {
    ResourceHolderManagerName2Resource name2Resource = ResourceHolderManagerName2Resource();
    name2Resource.mHash = MR::getHashCodeLower(pParam1);

    ResourceHolderManagerName2Resource* pIter;
    ResourceHolderManagerName2Resource* pEnd = mResourceArray.end();

    for (pIter = mResourceArray.begin(); pIter != pEnd && pIter->mHash != name2Resource.mHash; pIter++) {
        
    }

    if (pIter == pEnd) {
        return nullptr;
    }

    return pIter;
}

ResourceHolderManagerName2Resource::ResourceHolderManagerName2Resource() :
    mResourceHolder(nullptr),
    mLayoutHolder(nullptr),
    mHeap(nullptr)
{}

ResourceHolderManagerName2Resource& ResourceHolderManagerName2Resource::operator=(const ResourceHolderManagerName2Resource& other) {
    mResourceHolder = other.mResourceHolder;
    mLayoutHolder = other.mLayoutHolder;
    mHash = other.mHash;
    mHeap = other.mHeap;

    return *this;
}

CreateResourceHolderArgs::CreateResourceHolderArgs() :
    mResourceHolder(nullptr),
    mLayoutHolder(nullptr),
    mHeap(nullptr)
{}
