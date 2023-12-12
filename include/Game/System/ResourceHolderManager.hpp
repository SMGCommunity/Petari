#pragma once

#include <revolution.h>

class ResourceHolderManagerName2Resource {
public:
    ResourceHolderManagerName2Resource();

    u32 _0;
    u32 _4;
    u32 _8;
    u32 _C;
};

class CreateResourceHolderArgs {
public:
    u32 _0;
    u32 _4;
    u32 _8;
};

class ResourceHolderManager { 
public:
    ResourceHolderManager();

    void createResourceHolder(char const *, CreateResourceHolderArgs *);

    void createAndAddLayoutHolderStationed(const char *);
    void createAndAddStationed(const char *);

    ResourceHolder* createAndAdd(const char*, JKRHeap *);

    static void startCreateResourceHolderOnMainThread(const char *, CreateResourceHolderArgs *);

    ResourceHolderManagerName2Resource mResources[0x200];           // _0
    u32 mNumResources;                                              // _2000
};