#pragma once

#include "Game/Effect/ParticleResourceHolder.hpp"
#include "Game/NameObj/NameObj.hpp"
#include "Game/NameObj/NameObjHolder.hpp"
#include "Game/NameObj/NameObjRegister.hpp"
#include "Game/Screen/CaptureScreenDirector.hpp"
#include "Game/Screen/ScreenPreserver.hpp"
#include "Game/Screen/StarPointerDirector.hpp"
#include "Game/System/AudSystemWrapper.hpp"
#include "Game/System/FunctionAsyncExecutor.hpp"
#include "Game/System/MessageHolder.hpp"
#include "Game/System/WPadHolder.hpp"
#include <JSystem/JMath/random.hpp>

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
    void initResourceHolderManager();
    void initGameController();
    void initDisplay();

    NameObjHolder* mObjHolder;                      // 0x0
    ParticleResourceHolder* mParticleResHolder;     // 0x4
    GXRenderModeObj* mRenderModeObj;                // 0x8
    void* _C;
    void* _10;
    void* _14;
    CaptureScreenDirector* mCaptureScreenDir;       // 0x18
    ScreenPreserver* mScreenPreserver;              // 0x1C
    AudSystemWrapper* mSysWrapper;                  // 0x20
    WPadHolder* mWPadHolder;                        // 0x24
    FunctionAsyncExecutor* mAsyncExec;              // 0x28
    MessageHolder* mMsgHolder;                      // 0x2C
    StarPointerDirector* mStarPointerDir;           // 0x30
    JMath::TRandom_fast_ mRandom;                   // 0x34
    u32 mLanguage;                                  // 0x38
};