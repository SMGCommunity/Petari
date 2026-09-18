#pragma once

#include "Game/Util/Array.hpp"
#include <revolution/types.h>

class CreateResourceHolderArgs;
class JKRHeap;
class LayoutHolder;
class ResourceHolder;
class ResourceHolderManager;

typedef bool (*MakeArchiveFileNameFuncPtr)(char*, u32, const char*);
typedef void (*FuncPtrB)(const char*, CreateResourceHolderArgs*);
typedef void (ResourceHolderManager::*FuncPtrC)(const char*, CreateResourceHolderArgs*);

class CreateResourceHolderArgs {
public:
    /// @brief Creates a new `CreateResourceHolderArgs`.
    CreateResourceHolderArgs();

    /* 0x0 */ ResourceHolder* mResourceHolder;
    /* 0x4 */ LayoutHolder* mLayoutHolder;
    /* 0x8 */ JKRHeap* mHeap;
};

class ResourceHolderManagerName2Resource {
public:
    /// @brief Creates a new `ResourceHolderManagerName2Resource`.
    ResourceHolderManagerName2Resource();

    /// @brief Copies the contents from `other` to `this`.
    /// @param other A reference to the `ResourceHolderManagerName2Resource` to copy.
    /// @return A reference to `this`.
    ResourceHolderManagerName2Resource& operator=(const ResourceHolderManagerName2Resource& other);

    /* 0x0 */ ResourceHolder* mResourceHolder;
    /* 0x4 */ LayoutHolder* mLayoutHolder;
    /* 0x8 */ u32 mHash;
    /* 0xC */ JKRHeap* mHeap;
};

class ResourceHolderManager {
public:
    /// @brief Creates a new `ResourceHolderManager`.
    ResourceHolderManager();

    ResourceHolder* createAndAdd(const char*, JKRHeap*);
    ResourceHolder* createAndAddStationed(const char*);
    LayoutHolder* createAndAddLayoutHolder(const char*, JKRHeap*);
    LayoutHolder* createAndAddLayoutHolderStationed(const char*);
    LayoutHolder* createAndAddLayoutHolderRawData(const char*);
    void removeIfIsEqualHeap(JKRHeap*);
    static void startCreateResourceHolderOnMainThread(const char*, CreateResourceHolderArgs*);
    static void startCreateLayoutHolderOnMainThread(const char*, CreateResourceHolderArgs*);

public:
    ResourceHolderManagerName2Resource* createAndAddInner(const char*, MakeArchiveFileNameFuncPtr, FuncPtrB);
    ResourceHolderManagerName2Resource* createAndAddInnerStationed(const char*, FuncPtrC);
    void createResourceHolder(const char*, CreateResourceHolderArgs*);
    void createLayoutHolder(const char*, CreateResourceHolderArgs*);
    ResourceHolderManagerName2Resource* add(const char*, const CreateResourceHolderArgs&);
    ResourceHolderManagerName2Resource* find(const char*);

public:
    /* 0x0 */ MR::Vector< MR::FixedArray< ResourceHolderManagerName2Resource, 512 > > mResourceArray;
};
