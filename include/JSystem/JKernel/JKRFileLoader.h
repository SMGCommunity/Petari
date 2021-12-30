#pragma once

#include "JSystem/JKernel/JKRDisposer.h"

class JKRFileLoader : public JKRDisposer {
public:
    JKRFileLoader();    
    virtual ~JKRFileLoader();

    virtual void unmount();

    virtual void becomeCurrent(const char *) = 0;
    virtual void getResource(const char *) = 0;
    virtual void getResource(unsigned long, const char *) = 0;
    virtual void readResource(void *, unsigned long, const char *) = 0;
    virtual void readResource(void *, unsigned long, unsigned long, const char *) = 0;
    virtual void removeResourceAll() = 0;
    virtual void removeResource(void *) = 0;
    virtual void detachResource(void *) = 0;
    virtual void getResSize(const void *) const = 0;
    virtual void countFile(const char *) const = 0;
    virtual void getFirstFile(const char *) const = 0;
    virtual void getExpandedResSize(const void *) const = 0;

    void getGlbResource(const char *, JKRFileLoader *);
    void initializeVolumeList(void);
    void prependVolumeList(JSULink<JKRFileLoader>*);
    void removeVolumeList(JSULink<JKRFileLoader> *);

    JSULink<JKRFileLoader> _18;
    u32 _28;
    u32 _2C;
    u32 _30;
    u32 _34;
};