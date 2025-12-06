#pragma once

#include <JSystem/JMath/random.hpp>
#include <revolution.h>

namespace JMath {
    struct TRandom_fast_;
};

class AudSystemWrapper;
class CaptureScreenDirector;
class FunctionAsyncExecutor;
class MessageHolder;
class NameObjHolder;
class ParticleResourceHolder;
class ScreenPreserver;
class StarPointerDirector;
class WPadHolder;

class GameSystemObjHolder {
public:
    GameSystemObjHolder();

    void initAfterStationedResourceLoaded();
    void initMessageResource();
    void init();
    void createAudioSystem();
    void update();
    void updateAudioSystem();
    void clearRequestFileInfo(bool);
    void drawStarPointer();
    void drawBeforeEndRender();
    void captureIfAllowForScreenPreserver();
    GXRenderModeObj* getRenderModeObj() const;
    void initDvd();
    void initNAND();
    void initAudio();
    void initRenderMode();
    void initNameObj();
    void initFunctionAsyncExecutor();
    void initResourceHolderManager();
    void initGameController();
    void initWPad();
    void initStarPointerDirector();
    void initDisplay();

    NameObjHolder* mObjHolder;                   // 0x0
    ParticleResourceHolder* mParticleResHolder;  // 0x4
    GXRenderModeObj* mRenderModeObj;             // 0x8
    void* _C;
    void* _10;
    void* _14;
    CaptureScreenDirector* mCaptureScreenDir;  // 0x18
    ScreenPreserver* mScreenPreserver;         // 0x1C
    AudSystemWrapper* mSysWrapper;             // 0x20
    WPadHolder* mWPadHolder;                   // 0x24
    FunctionAsyncExecutor* mAsyncExec;         // 0x28
    MessageHolder* mMsgHolder;                 // 0x2C
    StarPointerDirector* mStarPointerDir;      // 0x30
    JMath::TRandom_fast_ mRandom;              // 0x34
    u32 mLanguage;                             // 0x38
};
