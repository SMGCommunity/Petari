#pragma once

#include "Game/Util/Array.hpp"

class DemoExecutor;
class LayoutActor;
class LiveActor;
class NameObj;
class Nerve;
class NerveExecutor;

class DemoStartInfo {
public:
    enum DemoType { DemoType_0, DemoType_TimeKeep };
    enum CinemaFrameType { CinemaFrameType_0 };
    enum StarPointerType { StarPointerType_0, StarPointerType_1, StarPointerType_2 };
    enum DeleteEffectType { DeleteEffectType_0, DeleteEffectType_1};

    DemoStartInfo();
    DemoStartInfo& operator=(const DemoStartInfo&);

    /* 0x00 */ LiveActor* _0;
    /* 0x04 */ LayoutActor* _4;
    /* 0x08 */ NerveExecutor* _8;
    /* 0x0C */ NameObj* _C;
    /* 0x10 */ NameObj* _10;
    /* 0x14 */ DemoExecutor* mDemoExecutor;
    /* 0x18 */ const char* mDemoName;
    /* 0x1C */ const char* _1C;
    /* 0x20 */ const Nerve* _20;
    /* 0x24 */ u32 _24;
    /* 0x28 */ DemoType mDemoType;
    /* 0x2C */ CinemaFrameType mFrameType;
    /* 0x30 */ StarPointerType mPointerType;
    /* 0x34 */ DeleteEffectType mDeleteEffectType;
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

    /* 0x00 */ DemoStartInfo* mStartInfos[0x10];
    /* 0x40 */ s32 mNumInfos;
    /* 0x44 */ MR::FixedRingBuffer< const DemoStartInfo*, 16 > mRequestBuffer;
    /* 0xA0 */ NameObj* mProxyObj;
};
