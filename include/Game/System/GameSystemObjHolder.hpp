#pragma once

#include <JSystem/JMath/random.hpp>
#include <revolution.h>

namespace JMath {
    struct TRandom_fast_;
};  // namespace JMath

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
    /// @brief Creates a new `GameSystemObjHolder`.
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

    /* 0x00 */ NameObjHolder* mObjHolder;
    /* 0x04 */ ParticleResourceHolder* mParticleResHolder;
    /* 0x08 */ GXRenderModeObj* mRenderModeObj;
    /* 0x0C */ void* _C;
    /* 0x10 */ void* _10;
    /* 0x14 */ void* _14;
    /* 0x18 */ CaptureScreenDirector* mCaptureScreenDirector;
    /* 0x1C */ ScreenPreserver* mScreenPreserver;
    /* 0x20 */ AudSystemWrapper* mAudioSystem;
    /* 0x24 */ WPadHolder* mWPadHolder;
    /* 0x28 */ FunctionAsyncExecutor* mFunctionAsyncExecutor;
    /* 0x2C */ MessageHolder* mMessageHolder;
    /* 0x30 */ StarPointerDirector* mStarPointerDirector;
    /* 0x34 */ JMath::TRandom_fast_ mRandom;
    /* 0x38 */ u32 mLanguage;
};
