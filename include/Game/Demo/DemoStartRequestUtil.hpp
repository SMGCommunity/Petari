#pragma once

#include "Game/Demo/DemoStartRequestHolder.hpp"

class Nerve;

namespace DemoStartRequestUtil {
    void startDemoSystem(NameObj*, const char*, s32, DemoStartInfo::DemoType, DemoStartInfo::CinemaFrameType, DemoStartInfo::StarPointerType,
                         DemoStartInfo::DeleteEffectType, const char*);
    void startDemoSystem(LiveActor*, const char*, s32, DemoStartInfo::DemoType, DemoStartInfo::CinemaFrameType, DemoStartInfo::StarPointerType,
                         DemoStartInfo::DeleteEffectType, const char*);
    void startDemoSystem(LayoutActor*, const char*, s32, DemoStartInfo::DemoType, DemoStartInfo::CinemaFrameType, DemoStartInfo::StarPointerType,
                         DemoStartInfo::DeleteEffectType, const char*);
    bool requestStartDemo(LiveActor*, const char*, const Nerve*, const Nerve*, s32, DemoStartInfo::DemoType, DemoStartInfo::CinemaFrameType,
                          DemoStartInfo::StarPointerType, DemoStartInfo::DeleteEffectType);
    bool requestStartDemo(LayoutActor*, const char*, const Nerve*, const Nerve*, s32, DemoStartInfo::DemoType, DemoStartInfo::CinemaFrameType,
                          DemoStartInfo::StarPointerType, DemoStartInfo::DeleteEffectType);
    bool requestStartDemo(NerveExecutor*, LiveActor*, const char*, const Nerve*, const Nerve*, s32, DemoStartInfo::DemoType,
                          DemoStartInfo::CinemaFrameType, DemoStartInfo::StarPointerType, DemoStartInfo::DeleteEffectType);
    bool requestStartTimeKeepDemo(LiveActor*, const char*, const char*, const Nerve*, const Nerve*, s32, DemoStartInfo::DemoType,
                                  DemoStartInfo::CinemaFrameType, DemoStartInfo::StarPointerType, DemoStartInfo::DeleteEffectType);
    bool requestStartTimeKeepDemo(NerveExecutor*, LiveActor*, const char*, const char*, const Nerve*, const Nerve*, s32, DemoStartInfo::DemoType,
                                  DemoStartInfo::CinemaFrameType, DemoStartInfo::StarPointerType, DemoStartInfo::DeleteEffectType);
    bool requestStartTimeKeepDemo(NameObj*, const char*, const char*, s32, DemoStartInfo::DemoType, DemoStartInfo::CinemaFrameType,
                                  DemoStartInfo::StarPointerType, DemoStartInfo::DeleteEffectType);
    void startDemo(const DemoStartInfo*);
    void startDemo(DemoStartRequestHolder*);
    bool popStartDemoRequest(DemoStartRequestHolder*);
    bool isExistStartDemoRequest(const DemoStartRequestHolder*);
    NameObj* getDemoStarter(const DemoStartInfo&);
    void setNerveToStarter(const DemoStartInfo*);
    void requestMovementOn(const DemoStartInfo*);
    bool isEmpty(const DemoStartInfo*);
    bool startDemoExecutorIfExist(const DemoStartInfo&);
};  // namespace DemoStartRequestUtil
