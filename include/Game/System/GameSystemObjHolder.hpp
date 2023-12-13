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