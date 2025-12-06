#pragma once

#include "JSystem/JKernel/JKRDisposer.hpp"

class JKRArcFinder;

class JKRFileLoader : public JKRDisposer {
public:
    JKRFileLoader();
    virtual ~JKRFileLoader();

    virtual void unmount();

    virtual bool becomeCurrent(const char*) = 0;
    virtual void* getResource(const char*) = 0;
    virtual void* getResource(unsigned long, const char*) = 0;
    virtual u32 readResource(void*, unsigned long, const char*) = 0;
    virtual u32 readResource(void*, unsigned long, unsigned long, const char*) = 0;
    virtual void removeResourceAll() = 0;
    virtual bool removeResource(void*) = 0;
    virtual bool detachResource(void*) = 0;
    virtual s32 getResSize(const void*) const = 0;
    virtual s32 countFile(const char*) const = 0;
    virtual JKRArcFinder* getFirstFile(const char*) const = 0;
    virtual s32 getExpandedResSize(const void*) const = 0;

    static void* getGlbResource(const char*, JKRFileLoader*);
    static void initializeVolumeList();
    void prependVolumeList(JSULink< JKRFileLoader >*);
    void removeVolumeList(JSULink< JKRFileLoader >*);

    static JSUList< JKRFileLoader > sFileLoaderList;  // 0x8060CF9C
    static JKRFileLoader* gCurrentFileLoader;         // 0x806B7140;

    JSULink< JKRFileLoader > mLoaderLink;  // 0x18
    char* mLoaderName;                     // 0x28
    u32 mLoaderType;                       // 0x2C
    bool mIsMounted;                       // 0x30
    u8 _31[3];
    u32 _34;
};