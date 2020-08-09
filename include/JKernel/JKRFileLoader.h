#pragma once

#include "JKernel/JKRDisposer.h"
#include <revolution/os.h>

class JKRFileLoader : public JKRDisposer
{
public:
    JKRFileLoader();
    
    virtual ~JKRFileLoader();
    virtual void unmount();

    static void* getGlbResource(const char *, JKRFileLoader *);
    static void initializeVolumeList();
    void prependVolumeList(JSULink<JKRFileLoader> *);
    void removeVolumeList(JSULink<JKRFileLoader> *);

    JSUPtrLink _18;
    u32 _28;
    u32 _2C;
    u8 _30;
    u8 _31;
    u8 _32;
    u8 _33;
    u32 _34;

    static JSUList<JKRFileLoader> sVolumeList;
    static OSMutex sVolumeListMutex;
    
    static const char* sCurrentVolume;
};