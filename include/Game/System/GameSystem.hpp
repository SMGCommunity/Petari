#pragma once

#include "Game/System/NerveExecutor.hpp"

class GameSequenceDirector;
class GameSystemDimmingWatcher;
class GameSystemErrorWatcher;
class GameSystemFontHolder;
class GameSystemFrameControl;
class GameSystemObjHolder;
class GameSystemSceneController;
class GameSystemStationedArchiveLoader;
class HomeButtonLayout;
class HomeButtonStateNotifier;
class SystemWipeHolder;

#ifdef __MWERKS__
void main(void);
#endif

class GameSystem : public NerveExecutor {
public:
    /// @brief Creates a new `GameSystem`.
    GameSystem();

    void init();
    bool isExecuteLoadSystemArchive() const;
    bool isDoneLoadSystemArchive() const;
    void startToLoadSystemArchive();
    void exeInitializeAudio();
    void exeInitializeLogoScene();
    void exeLoadStationedArchive();
    void exeWaitForReboot();
    void exeNormal();
    void initGX();
    void initAfterStationedResourceLoaded();
    void prepareReset();
    bool isPreparedReset() const;
    void frameLoop();
    void draw();
    void update();
    void updateSceneController();
    void calcAnim();

    /* 0x08 */ void* mFifoBase;
    /* 0x0C */ GameSequenceDirector* mSequenceDirector;
    /* 0x10 */ GameSystemDimmingWatcher* mDimmingWatcher;
    /* 0x14 */ GameSystemErrorWatcher* mErrorWatcher;
    /* 0x18 */ GameSystemFontHolder* mFontHolder;
    /* 0x1C */ GameSystemFrameControl* mFrameControl;
    /* 0x20 */ GameSystemObjHolder* mObjHolder;
    /* 0x24 */ GameSystemSceneController* mSceneController;
    /* 0x28 */ GameSystemStationedArchiveLoader* mStationedArchiveLoader;
    /* 0x2C */ HomeButtonLayout* mHomeButtonLayout;
    /* 0x30 */ SystemWipeHolder* mSystemWipeHolder;
    /* 0x34 */ HomeButtonStateNotifier* mHomeButtonStateNotifier;
    /* 0x38 */ bool mIsExecuteLoadSystemArchive;
};
