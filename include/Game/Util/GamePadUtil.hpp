#pragma once

#include <JSystem/JGeometry/TVec.hpp>

class WPadRumble;

namespace MR {
    void getCorePadPointingPosBasedOnScreen(TVec2f* pPos, s32 channel);
    void getCorePadPointingPos(TVec2f* pPos, s32 channel);
    void getCorePadPastPointingPos(TVec2f* pPos, s32 idx, s32 channel);
    s32 getCorePadEnablePastCount(s32 channel);

    /// @brief Determines if the given Wii Remote is pointing at the screen.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if the Wii Remote is pointing at the screen, `false` otherwise.
    bool isCorePadPointInScreen(s32 channel);

    /// @brief Returns the distance between the Sensor Bar and the Wii Remote, in meters.
    /// @param channel The handle to the Wii Remote.
    /// @return The distance between the Sensor Bar and the Wii Remote.
    f32 getCorePadDistanceToDisplay(s32 channel);

    /// @brief Returns the acceleration of the given Wii Remote by pointer.
    /// @param pAccel The output pointer for the acceleration vector.
    /// @param channel The handle to the Wii Remote.
    void getCorePadAcceleration(TVec3f* pAccel, s32 channel);

    /// @brief Determines if up on the given Wii Remote's +Control Pad is pressed.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if up is pressed, `false` otherwise.
    bool testCorePadButtonUp(s32 channel);

    /// @brief Determines if down on the given Wii Remote's +Control Pad is pressed.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if down is pressed, `false` otherwise.
    bool testCorePadButtonDown(s32 channel);

    /// @brief Determines if the A Button on the given Wii Remote is pressed.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if the A Button is pressed, `false` otherwise.
    bool testCorePadButtonA(s32 channel);

    /// @brief Determines if the B Button on the given Wii Remote is pressed.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if the B Button is pressed, `false` otherwise.
    bool testCorePadButtonB(s32 channel);

    /// @brief Determines if the Plus (+) Button on the given Wii Remote is pressed.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if the Plus (+) Button is pressed, `false` otherwise.
    bool testCorePadButtonPlus(s32 channel);

    /// @brief Determines if the Minus (-) Button on the given Wii Remote is pressed.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if the Minus (-) Button is pressed, `false` otherwise.
    bool testCorePadButtonMinus(s32 channel);

    /// @brief Determines if the C Button on the given Nunchuk is pressed.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if the C Button is pressed, `false` otherwise.
    bool testSubPadButtonC(s32 channel);

    /// @brief Determines if the Z Button on the given Nunchuk is pressed.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if the Z Button is pressed, `false` otherwise.
    bool testSubPadButtonZ(s32 channel);

    /// @brief Determines if any button except the HOME Button on the given Wii Remote or Nunchuk is pressed.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if any valid button is pressed, `false` otherwise.
    bool testPadButtonAnyWithoutHome(s32 channel);

    /// @brief Determines if up on the given Wii Remote's +Control Pad was just pressed.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if up was just pressed, `false` otherwise.
    bool testCorePadTriggerUp(s32 channel);

    /// @brief Determines if down on the given Wii Remote's +Control Pad was just pressed.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if down was just pressed, `false` otherwise.
    bool testCorePadTriggerDown(s32 channel);

    /// @brief Determines if left on the given Wii Remote's +Control Pad was just pressed.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if left was just pressed, `false` otherwise.
    bool testCorePadTriggerLeft(s32 channel);

    /// @brief Determines if right on the given Wii Remote's +Control Pad was just pressed.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if right was just pressed, `false` otherwise.
    bool testCorePadTriggerRight(s32 channel);

    /// @brief Determines if the A Button on the given Wii Remote was just pressed.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if the A Button was just pressed, `false` otherwise.
    bool testCorePadTriggerA(s32 channel);

    /// @brief Determines if the B Button on the given Wii Remote was just pressed.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if the B Button was just pressed, `false` otherwise.
    bool testCorePadTriggerB(s32 channel);

    /// @brief Determines if the Plus (+) Button on the given Wii Remote was just pressed.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if the Plus (+) Button was just pressed, `false` otherwise.
    bool testCorePadTriggerPlus(s32 channel);

    /// @brief Determines if the Minus (-) Button on the given Wii Remote was just pressed.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if the Minus (-) Button was just pressed, `false` otherwise.
    bool testCorePadTriggerMinus(s32 channel);

    /// @brief Determines if any button except the HOME Button on the given Wii Remote was just pressed.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if any valid button was just pressed, `false` otherwise.
    bool testCorePadTriggerAnyWithoutHome(s32 channel);

    /// @brief Determines if the HOME Button on the given Wii Remote was just pressed.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if the HOME Button was just pressed, `false` otherwise.
    bool testCorePadTriggerHome(s32 channel);

    /// @brief Determines if the C Button on the given Nunchuk was just pressed.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if the C Button was just pressed, `false` otherwise.
    bool testSubPadTriggerC(s32 channel);

    /// @brief Determines if the Z Button on the given Nunchuk was just pressed.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if the Z Button was just pressed, `false` otherwise.
    bool testSubPadTriggerZ(s32 channel);

    /// @brief Determines if the Z Button on the given Nunchuk was just released.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if the Z Button was just released, `false` otherwise.
    bool testSubPadReleaseZ(s32 channel);

    /// @brief Determines if the given Wii Remove is being shaken.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if the Wii Remote is being shaken, `false` otherwise.
    bool isCorePadSwing(s32 channel);

    /// @brief Determines if the given Wii Remote was just shaken.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if the Wii Remote was just shaken, `false` otherwise.
    bool isCorePadSwingTrigger(s32 channel);

    /// @brief Returns the coordinate of the Control Stick from the given Nunchuk along the x-axis.
    /// @param channel The handle to the Wii Remote.
    /// @return The coordinate along the x-axis.
    f32 getSubPadStickX(s32 channel);

    /// @brief Returns the coordinate of the Control Stick from the given Nunchuk along the y-axis.
    /// @param channel The handle to the Wii Remote.
    /// @return The coordinate along the y-axis.
    f32 getSubPadStickY(s32 channel);

    /// @brief Determines if up on the given Nunchuk's Control Stick was just tilted.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if up was just tilted, `false` otherwise.
    bool testSubPadStickTriggerUp(s32 channel);

    /// @brief Determines if down on the given Nunchuk's Control Stick was just tilted.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if down was just tilted, `false` otherwise.
    bool testSubPadStickTriggerDown(s32 channel);

    /// @brief Determines if left on the given Nunchuk's Control Stick was just tilted.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if left was just tilted, `false` otherwise.
    bool testSubPadStickTriggerLeft(s32 channel);

    /// @brief Determines if right on the given Nunchuk's Control Stick was just tilted.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if right was just tilted, `false` otherwise.
    bool testSubPadStickTriggerRight(s32 channel);

    /// @brief Returns the acceleration of the given Nunchuk by pointer.
    /// @param pAccel The output pointer for the acceleration vector.
    /// @param channel The handle to the Wii Remote.
    void getSubPadAcceleration(TVec3f* pAccel, s32 channel);

    /// @brief Determines if the given Nunchuk is being shaken.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if the Nunchuk is being shaken, `false` otherwise.
    bool isSubPadSwing(s32 channel);

    /// @brief Determines if the given Wii Remote or Nunchuk is being shaken.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if the Wii Remote or Nunchuk is being shaken, `false` otherwise.
    bool isPadSwing(s32 channel);

    /// @brief Determines if the button to continue past a prompt on the primary Wii Remote was just pressed.
    /// @return `true` if the button was just pressed, `false` otherwise.
    bool testSystemPadTriggerDecide();

    /// @brief Determines if the A Button on the primary Wii Remote was just pressed.
    /// @return `true` if the button was just pressed, `false` otherwise.
    bool testSystemTriggerA();

    /// @brief Determines if the B Button on the primary Wii Remote was just pressed.
    /// @return `true` if the button was just pressed, `false` otherwise.
    bool testSystemTriggerB();

    /// @brief Determines if the button to select an item using the primary Wii Remote's Pointer was just pressed.
    /// @return `true` if the button was just pressed, `false` otherwise.
    bool testDPDMenuPadDecideTrigger();

    /// @brief Determines if the button to change the camera view into the first-person view was just pressed.
    /// @return `true` if the button was just pressed, `false` otherwise.
    bool testFpViewStartTrigger();

    /// @brief Determines if the button to change the camera view from the first-person view was just pressed.
    /// @return `true` if the button was just pressed, `false` otherwise.
    bool testFpViewOutTrigger();

    /// @brief Returns the coordinate of the Control Stick from the Nunchuk controlling the player character along the x-axis.
    /// @return The coordinate along the x-axis.
    f32 getPlayerStickX();

    /// @brief Returns the coordinate of the Control Stick from the Nunchuk controlling the player character along the y-axis.
    /// @return The coordinate along the y-axis.
    f32 getPlayerStickY();

    /// @brief Determines if the A Button on the Wii Remote controlling the player character was just pressed.
    /// @return `true` if the A Button was just pressed, `false` otherwise.
    bool getPlayerTriggerA();

    /// @brief Determines if the B Button on the Wii Remote controlling the player character was just pressed.
    /// @return `true` if the B Button was just pressed, `false` otherwise.
    bool getPlayerTriggerB();

    /// @brief Determines if the Z Button on the Nunchuk controlling the player character was just pressed.
    /// @return `true` if the Z Button was just pressed, `false` otherwise.
    bool getPlayerTriggerZ();

    /// @brief Determines if the C Button on the Nunchuk controlling the player character was just pressed.
    /// @return `true` if the C Button was just pressed, `false` otherwise.
    bool getPlayerTriggerC();

    /// @brief Determines if the A Button on the Wii Remote controlling the player character is pressed.
    /// @return `true` if the A Button is pressed, `false` otherwise.
    bool getPlayerLevelA();

    /// @brief Determines if the B Button on the Wii Remote controlling the player character is pressed.
    /// @return `true` if the B Button is pressed, `false` otherwise.
    bool getPlayerLevelB();

    /// @brief Determines if the Z Button on the Nunchuk controlling the player character is pressed.
    /// @return `true` if the Z Button is pressed, `false` otherwise.
    bool getPlayerLevelZ();

    /// @brief Determines if the C Button on the Nunchuk controlling the player character is pressed.
    /// @return `true` if the C Button is pressed, `false` otherwise.
    bool getPlayerLevelC();

    /// @brief Determines if any direction on the given Nunchuk's Control Stick is tilted.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if any direction is tilted, `false` otherwise.
    bool isGamePadStickOperated(s32 channel);

    void calcWorldStickDirectionXZ(f32* pDirX, f32* pDirZ, s32 channel);
    void calcWorldStickDirectionXZ(TVec3f* pDir, s32 channel);

    /// @brief Returns the maximum number of allowed controllers.
    /// @return The maximum number of allowed controllers.
    u32 getWPadMaxCount();

    /// @brief Determines if the given Wii Remote is connected.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if the Wii Remote is connected, `false` otherwise.
    bool isConnectedWPad(s32 channel);

    /// @brief Determines if the given Wii Remote is being controlled.
    /// @param channel The handle to the Wii Remote.
    /// @return `true` if the Wii Remote is being controlled, `false` otherwise.
    bool isOperatingWPad(s32 channel);
};  // namespace MR

namespace WPadFunction {
    WPadRumble* getWPadRumble(s32 channel);
};
