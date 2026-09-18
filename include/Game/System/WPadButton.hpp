#pragma once

#include <revolution.h>

class WPad;

/// @brief The wrapper around the state of a Wii Remote's buttons.
class WPadButton {
public:
    /// @brief Creates a new `WPadButton`.
    /// @param pPad The pointer to the state of the owning Wii Remote.
    WPadButton(const WPad* pPad);

    /// @brief Updates the state of the Wii Remote's buttons.
    void update();

    /// @brief Determines if up on the +Control Pad is pressed.
    /// @return `true` if up is pressed, `false` otherwise.
    bool testButtonUp() const;

    /// @brief Determines if down on the +Control Pad is pressed.
    /// @return `true` if down is pressed, `false` otherwise.
    bool testButtonDown() const;

    /// @brief Determines if left on the +Control Pad is pressed.
    /// @return `true` if left is pressed, `false` otherwise.
    bool testButtonLeft() const;

    /// @brief Determines if right on the +Control Pad is pressed.
    /// @return `true` if right is pressed, `false` otherwise.
    bool testButtonRight() const;

    /// @brief Determines if the A Button is pressed.
    /// @return `true` if the A Button is pressed, `false` otherwise.
    bool testButtonA() const;

    /// @brief Determines if the B Button is pressed.
    /// @return `true` if the B Button is pressed, `false` otherwise.
    bool testButtonB() const;

    /// @brief Determines if the Minus (-) Button is pressed.
    /// @return `true` if the Minus (-) Button is pressed, `false` otherwise.
    bool testButtonMinus() const;

    /// @brief Determines if the Plus (+) Button is pressed.
    /// @return `true` if the Plus (+) Button is pressed, `false` otherwise.
    bool testButtonPlus() const;

    /// @brief Determines if the 1 Button is pressed.
    /// @return `true` if the 1 Button is pressed, `false` otherwise.
    bool testButton1() const;

    /// @brief Determines if the 2 Button is pressed.
    /// @return `true` if the 2 Button is pressed, `false` otherwise.
    bool testButton2() const;

    /// @brief Determines if the C Button is pressed.
    /// @return `true` if the C Button is pressed, `false` otherwise.
    bool testButtonC() const;

    /// @brief Determines if the Z Button is pressed.
    /// @return `true` if the Z Button is pressed, `false` otherwise.
    bool testButtonZ() const;

    /// @brief Determines if up on the +Control Pad was just pressed.
    /// @return `true` if up was just pressed, `false` otherwise.
    bool testTriggerUp() const;

    /// @brief Determines if down on the +Control Pad was just pressed.
    /// @return `true` if down was just pressed, `false` otherwise.
    bool testTriggerDown() const;

    /// @brief Determines if left on the +Control Pad was just pressed.
    /// @return `true` if left was just pressed, `false` otherwise.
    bool testTriggerLeft() const;

    /// @brief Determines if right on the +Control Pad was just pressed.
    /// @return `true` if right was just pressed, `false` otherwise.
    bool testTriggerRight() const;

    /// @brief Determines if the A Button was just pressed.
    /// @return `true` if the A Button was just pressed, `false` otherwise.
    bool testTriggerA() const;

    /// @brief Determines if the B Button was just pressed.
    /// @return `true` if the B Button was just pressed, `false` otherwise.
    bool testTriggerB() const;

    /// @brief Determines if the Minus (-) Button was just pressed.
    /// @return `true` if the Minus (-) Button was just pressed, `false` otherwise.
    bool testTriggerMinus() const;

    /// @brief Determines if the HOME Button was just pressed.
    /// @return `true` if the HOME Button was just pressed, `false` otherwise.
    bool testTriggerHome() const;

    /// @brief Determines if the Plus (+) Button was just pressed.
    /// @return `true` if the Plus (+) Button was just pressed, `false` otherwise.
    bool testTriggerPlus() const;

    /// @brief Determines if the 1 Button was just pressed.
    /// @return `true` if the 1 Button was just pressed, `false` otherwise.
    bool testTrigger1() const;

    /// @brief Determines if the 2 Button was just pressed.
    /// @return `true` if the 2 Button was just pressed, `false` otherwise.
    bool testTrigger2() const;

    /// @brief Determines if the C Button was just pressed.
    /// @return `true` if the C Button was just pressed, `false` otherwise.
    bool testTriggerC() const;

    /// @brief Determines if the Z Button was just pressed.
    /// @return `true` if the Z Button was just pressed, `false` otherwise.
    bool testTriggerZ() const;

    /// @brief Determines if the Z Button was just released.
    /// @return `true` if the Z Button was just released, `false` otherwise.
    bool testReleaseZ() const;

    /// @brief Determines if any button was just pressed or released.
    /// @return `true` if any button was just pressed or released, `false` otherwise.
    bool isChangeAnyState() const;

public:
    /// @brief The pointer to the state of the owning Wii Remote.
    /* 0x00 */ const WPad* mPad;

    /// @brief The combination of buttons pressed.
    /* 0x04 */ u32 mHold;

    /// @brief The combination of buttons just pressed.
    /* 0x08 */ u32 mTrigger;

    /// @brief The combination of buttons just released.
    /* 0x0C */ u32 mRelease;

    /// @brief The combination of buttons pressed during the repeat period.
    /* 0x10 */ u32 mRepeat;

    /// @brief The number of seconds to hold down any button before the `KPAD_BUTTON_RPT` flag is set.
    /* 0x14 */ f32 mDelaySec;

    /// @brief The number of seconds to hold down any button after @ref mDelaySec before the `KPAD_BUTTON_RPT` flag is set.
    /* 0x18 */ f32 mPulseSec;
};
