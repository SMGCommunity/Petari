#pragma once

#include "Game/Effect/ParticleResourceHolder.h"
#include "Game/NameObj/NameObj.h"
#include "Game/NameObj/NameObjHolder.h"
#include "Game/NameObj/NameObjRegister.h"
#include "Game/Screen/CaptureScreenDirector.h"
#include "Game/Screen/ScreenPreserver.h"
#include "Game/Screen/StarPointerDirector.h"
#include "Game/System/AudSystemWrapper.h"
#include "Game/System/FunctionAsyncExecutor.h"
#include "Game/System/MessageHolder.h"
#include "Game/System/WPadHolder.h"
#include <JSystem/JMath/random.h>

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

    NameObjHolder* mObjHolder;                      // _0
    ParticleResourceHolder* mParticleResHolder;     // _4
    GXRenderModeObj* mRenderModeObj;                // _8
    void* _C;
    void* _10;
    void* _14;
    CaptureScreenDirector* mCaptureScreenDir;       // _18
    ScreenPreserver* mScreenPreserver;              // _1C
    AudSystemWrapper* mSysWrapper;                  // _20
    WPadHolder* mWPadHolder;                        // _24
    FunctionAsyncExecutor* mAsyncExec;              // _28
    MessageHolder* mMsgHolder;                      // _2C
    StarPointerDirector* mStarPointerDir;           // _30
    JMath::TRandom_fast_ mRandom;                   // _34
    u32 mLanguage;                                  // _38
};