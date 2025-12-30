#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/Array.hpp"

class LayoutActor;
class LiveActor;
class NerveExecutor;

class DemoStartInfo {
public:
    DemoStartInfo();
    DemoStartInfo& operator=(const DemoStartInfo&);

    u32 _0;
    u32 _4;
    u32 _8;
    u32 _C;
    u32 _10;
    u32 _14;
    const char* mDemoName;  // 0x18
    u32 _1C;
    u32 _20;
    u32 _24;
    u32 _28;
    u32 _2C;
    u32 _30;
    u32 _34;
};

namespace DemoStartRequestUtil {
    bool isEmpty(const DemoStartInfo*);
};

class DemoStartRequestHolder {
public:
    DemoStartRequestHolder();

    void pushRequest(LiveActor*, const char*);
    void pushRequest(LayoutActor*, const char*);
    void pushRequest(NerveExecutor*, const char*);
    void pushRequest(NameObj*, const char*);
    void popRequest();
    bool isExistRequest() const;
    const DemoStartInfo* getCurrentInfo() const;
    void registerStartDemoInfo(const DemoStartInfo&);
    DemoStartInfo* find(const LiveActor*, const char*) const;
    DemoStartInfo* find(const LayoutActor*, const char*) const;
    DemoStartInfo* find(const NerveExecutor*, const char*) const;
    DemoStartInfo* find(const NameObj*, const char*) const;
    DemoStartInfo* findEmpty() const;

    DemoStartInfo* mStartInfos[0x10];                                       // 0x0
    s32 mNumInfos;                                                          // 0x40
    MR::FixedRingBuffer<const DemoStartInfo*, 16> mRequestBuffer;           // 0x44
    NameObj* mProxyObj;                                                     // 0xA0
};
