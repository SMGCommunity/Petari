#pragma once

#include "Game/Screen/LayoutActor.hpp"

class ValueControl;

struct Timing {
    /* 0x0 */ s32  mScaleStartFrame;
    /* 0x4 */ s32  mScaleKeepFrame;
    /* 0x8 */ bool mIsScaleUp;
    /* 0x9 */ bool mIsScaleDown;
    /* 0xA */ bool _A;
};

class TimeUpLayout : public LayoutActor {
public:
    /// @brief Creates a new `TimeUpLayout`.
    TimeUpLayout()
        : LayoutActor("タイムアップ画面", true) {
    }

    virtual void init(const JMapInfoIter& rIter);
};

class TimeLimitLayout : public LayoutActor {
public:
    TimeLimitLayout(u32 timeLimit);

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void kill();
    virtual void control();

    void          setTimeLimit(u32);
    void          setDisplayModeOnNormal(bool);
    bool          isReadyToTimeUp() const;
    void          resetFrame();
    void          addFrame();
    void          updateTextBox();
    void          exeAppear();
    void          exeCountDown();
    void          exeScaleUp();
    void          exeScaleKeep();
    void          exeScaleDown();
    void          exeFadeout();
    void          exeTimeUpReady();
    const Timing* getCurrentTiming() const;
    bool          updateNormal();

private:
    /* 0x20 */ u32           mTime;
    /* 0x24 */ u32           mTimeLimit;
    /* 0x28 */ ValueControl* mScaleCtrl;
    /* 0x2C */ ValueControl* mAlphaCtrl;
    /* 0x30 */ const Timing* mCurrentTiming;
    /* 0x34 */ bool          mIsSuspend;
    /* 0x35 */ bool          _35;
};
