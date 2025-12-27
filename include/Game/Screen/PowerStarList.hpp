#pragma once

#include "Game/Screen/LayoutActor.hpp"

class ButtonPaneController;
class GalaxyStatusAccessor;
class SysInfoWindow;

class PowerStarList : public LayoutActor {
public:
    class Separator {
    public:
        /// @brief Creates a new `Separator`.
        Separator();

        /* 0x00 */ u8 _0[4];
        /* 0x04 */ TVec2f _4;
    };

    /// @brief Creates a new `PowerStarList`.
    PowerStarList();

    virtual void init(const JMapInfoIter& rIter);
    virtual void calcAnim();
    virtual void appear();
    virtual void control();

    void drawForMessageBoardCapture();
    bool isAppearing() const;
    bool isExecCapture() const;
    ButtonPaneController* createButtonController(const char*, const char*);
    void resetButtonAll();
    void updateButtonAppearance();
    void updateList(s32, bool);
    void makeGalaxyNameText(wchar_t*, const GalaxyStatusAccessor*, bool) const;
    void makeStarNumText(wchar_t*, const GalaxyStatusAccessor*, bool) const;
    void makeCoinText(wchar_t*, const GalaxyStatusAccessor*, bool) const;
    void makeCrownText(wchar_t*, const GalaxyStatusAccessor*, bool) const;
    void makeRaceNameText(wchar_t*, int, bool) const;
    void makeRaceTimeText(wchar_t*, int, bool) const;
    void startScrollAnimNext(bool);
    void startScrollAnimPrev();
    bool tryShowSeparator(s32, f32);
    void setSeparatorPaneSize();
    void setTotalPowerStarNumForMessageBoardCapture();
    void exeAppear();
    void exeWait();
    void exeDisappear();
    void exePageNext();
    void exePagePrev();
    void exeCaptureStart();
    void exeCaptureConfirm();
    void exeCaptureWait();
    void exeCaptureSend();

private:
    /* 0x20 */ ButtonPaneController* mArrowUpButtonCtrl;
    /* 0x24 */ ButtonPaneController* mArrowDownButtonCtrl;
    /* 0x28 */ ButtonPaneController* mCaptureButtonCtrl;
    /* 0x2C */ SysInfoWindow* _2C;
    /* 0x30 */ s32 _30;
    /* 0x34 */ s32 _34;
    /* 0x38 */ Separator* mSeparatorArray;
    /* 0x3C */ u32 mMailMessageLength;
    /* 0x40 */ wchar_t* mMailMessage;
};
