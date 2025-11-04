#pragma once

#include "Game/Screen/LayoutActor.hpp"

class TriggerChecker;
class ValueControl;

class GameSystemResetAndPowerProcess : public LayoutActor {
public:
    /// @brief Creates a new `GameSystemResetAndPowerProcess`.
    GameSystemResetAndPowerProcess();

    virtual void init(const JMapInfoIter&);
    virtual void draw() const;
    virtual void control();

    bool isActive() const NO_INLINE;
    void setResetOperationApplicationReset();
    void setResetOperationReturnToMenu();
    void requestReset(bool);
    void requestGoWiiMenu(bool);
    void notifyCheckDiskResult(bool);
    void exePolling();
    void exeWaitResetPermitted();
    void exePrepareReset();
    void exeReset();
    void exeWaitPrepareFadein();
    void exeFadein();
    void exitApplication();
    bool tryPermitReset() NO_INLINE;
    bool tryAcceptPowerOff();
    bool isResetAcceptAudio() const;
    static void handleOSPowerCallback();
    static void handleCheckDiskAsync(s32, DVDCommandBlock*);

    /* 0x20 */ TriggerChecker* mResetTriggerChecker;
    /* 0x24 */ ValueControl* mFadeinValueControl;
    /* 0x28 */ DVDCommandBlock mCommandBlock;
    /* 0x58 */ s32 mResetOperation;
    /* 0x5C */ bool _5C;
    /* 0x5D */ bool mIsValidPowerOff;
    /* 0x5E */ bool _5E;
};
