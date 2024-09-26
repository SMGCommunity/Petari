#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "JSystem/JUtility/JUTTexture.hpp"

struct TimingInfo {
    const char* name;   // 0x0
    s32 timing;         // 0x4
    u32 _8;
    u32 _C;
};

namespace {
    const TimingInfo cTimingInfo[6] = {
        { "GameScreen", 0x20, 0x1000000, 1 },
        { "Indirect", 1, 0, 0 },
        { "Camera", 4, 0x1000000, 1 },
        { "WipeMap", 5, 0x1000000, 1 },
        { "SystemWipe", 0x14, 0, 1 },
        { "Unused", 0x10, 0, 0 }
    };
};

class CaptureScreenDirector : public NameObj {
public:
    CaptureScreenDirector();

    virtual ~CaptureScreenDirector();

    void captureIfAllow(const char *);
    void capture();
    void requestCaptureTiming(const char *);
    void invalidateCaptureTiming(const char *);
    ResTIMG* getResTIMG() const;
    ResTIMG* getTexImage() const;
    const TimingInfo* getUsingTiming() const;
    const TimingInfo* getCurrentTiming() const;
    const TimingInfo* findFromName(const char *) const;

    const char* _C;
    const char* mTimingType;    // 0x10
    JUTTexture* mTexture;       // 0x14
    u8 _18;
};

class CaptureScreenActor : public NameObj {
public:
    CaptureScreenActor(u32, const char *);

    virtual ~CaptureScreenActor();
    virtual void draw() const;

    const char* mCameraName;    // 0xC
};